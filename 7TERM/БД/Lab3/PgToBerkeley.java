import java.io.*;
import java.nio.charset.StandardCharsets;
import java.nio.file.*;
import java.sql.*;
import java.util.*;

// Berkeley DB Java Edition
import com.sleepycat.je.Database;
import com.sleepycat.je.DatabaseConfig;
import com.sleepycat.je.DatabaseEntry;
import com.sleepycat.je.Environment;
import com.sleepycat.je.EnvironmentConfig;

/**
 * PgToBerkeley — консольный конвертер PostgreSQL -> BerkeleyDB (Java Edition).
 *
 * Алгоритм:
 * 1) Читает DB_CONFIG из существующего config.py
 * 2) Подключается к Postgres через JDBC
 * 3) Для каждой таблицы схемы public:
 *    - читает список столбцов и первичных ключей
 *    - выгружает строки; ключ = значение(я) PK, значение = JSON остальных столбцов
 *    - сохраняет записи в BerkeleyDB (одна база на таблицу в одном окружении)
 * 4) Закрывает соединения и завершает работу
 *
 * Требуются JAR-зависимости:
 * - PostgreSQL JDBC (org.postgresql)
 * - Berkeley DB Java Edition (com.sleepycat.je)
 */
public class PgToBerkeley {
    // Определяем рабочую папку динамически от текущего каталога запуска
    private static final Path WORKSPACE = Paths.get("").toAbsolutePath();
    private static final Path CONFIG_PY = WORKSPACE.resolve("config.py");
    private static final Path LAB3_DIR = WORKSPACE.resolve("Lab3");
    private static final Path OUTPUT_DIR = LAB3_DIR.resolve("berkeley_data");
    private static final boolean UNICODE_LOGS =
            Optional.ofNullable(System.getenv("PG2B_LOGS"))
                    .map(v -> v.equalsIgnoreCase("unicode") || v.equalsIgnoreCase("utf8"))
                    .orElse(false);
    private static PrintWriter LOG;

    public static void main(String[] args) {
        // Принудительно выставляем UTF-8 для консольного вывода,
        // чтобы кириллица не превращалась в "????" на Windows-консоли.
        try {
            System.setOut(new PrintStream(new BufferedOutputStream(new FileOutputStream(FileDescriptor.out)), true, StandardCharsets.UTF_8));
            System.setErr(new PrintStream(new BufferedOutputStream(new FileOutputStream(FileDescriptor.err)), true, StandardCharsets.UTF_8));
        } catch (Exception ignore) {}
        try {
            Files.createDirectories(LAB3_DIR);
            LOG = new PrintWriter(Files.newBufferedWriter(
                    LAB3_DIR.resolve("converter.log"),
                    StandardCharsets.UTF_8,
                    StandardOpenOption.CREATE,
                    StandardOpenOption.TRUNCATE_EXISTING
            ));
        } catch (IOException ioe) {
            LOG = null; // fallback to console-only
        }
        log("[PgToBerkeley] Conversion start...");
        try {
            Map<String, String> cfg = parsePythonConfig(CONFIG_PY);
            String host = cfg.getOrDefault("host", "127.0.0.1");
            String port = cfg.getOrDefault("port", "5432");
            String db = cfg.getOrDefault("database", "postgres");
            String user = cfg.getOrDefault("user", "postgres");
            String pass = cfg.getOrDefault("password", "");

            String url = "jdbc:postgresql://" + host + ":" + port + "/" + db;
            log("[PgToBerkeley] Connecting to " + url);

            Properties props = new Properties();
            props.setProperty("user", user);
            props.setProperty("password", pass);
            try (Connection conn = DriverManager.getConnection(url, props)) {
                Files.createDirectories(OUTPUT_DIR);
                convertAllTables(conn, OUTPUT_DIR);
            }

            log("[PgToBerkeley] Conversion finished successfully.");
        } catch (Exception e) {
            logErr("[PgToBerkeley] Error: " + asciiSafe(e.getMessage()));
            e.printStackTrace();
            System.exit(1);
        } finally {
            if (LOG != null) {
                try { LOG.flush(); LOG.close(); } catch (Exception ignore) {}
            }
        }
    }

    private static Map<String, String> parsePythonConfig(Path path) throws IOException {
        Map<String, String> out = new HashMap<>();
        if (!Files.exists(path)) {
            throw new FileNotFoundException("config.py not found: " + path.toString());
        }
        List<String> lines = Files.readAllLines(path, StandardCharsets.UTF_8);
        // Простой парсер DB_CONFIG = { 'host': '...', "port": 5432, ... }
        for (String line : lines) {
            String l = line.trim();
            if (l.startsWith("'host'")) {
                out.put("host", extractValue(l));
            } else if (l.startsWith("\"host\"")) {
                out.put("host", extractValue(l));
            } else if (l.startsWith("'port'") || l.startsWith("\"port\"")) {
                out.put("port", extractValue(l));
            } else if (l.startsWith("'database'") || l.startsWith("\"database\"")) {
                out.put("database", extractValue(l));
            } else if (l.startsWith("'user'") || l.startsWith("\"user\"")) {
                out.put("user", extractValue(l));
            } else if (l.startsWith("'password'") || l.startsWith("\"password\"")) {
                out.put("password", extractValue(l));
            }
        }
        return out;
    }

    private static String extractValue(String line) {
        // ожидаем формат: 'key': 'value', или "key": "value",
        String[] parts = line.split(":", 2);
        if (parts.length < 2) return "";
        String v = parts[1].trim();
        // убрать завершающую запятую
        if (v.endsWith(",")) v = v.substring(0, v.length() - 1).trim();
        // убрать кавычки
        if ((v.startsWith("'") && v.endsWith("'")) || (v.startsWith("\"") && v.endsWith("\""))) {
            v = v.substring(1, v.length() - 1);
        }
        return v;
    }

    private static void convertAllTables(Connection conn, Path envDir) throws Exception {
        List<String> tables = listPublicTables(conn);
        if (tables.isEmpty()) {
            log("[PgToBerkeley] No tables in public schema.");
            return;
        }

        EnvironmentConfig envCfg = new EnvironmentConfig();
        envCfg.setAllowCreate(true);
        try (Environment env = new Environment(envDir.toFile(), envCfg)) {
            for (String table : tables) {
                List<String> pkCols = listPrimaryKeyColumns(conn, table);
                if (pkCols.isEmpty()) {
                    log("[WARN] Table '" + displayName(table) + "' skipped: no primary key.");
                    continue;
                }

                List<String> allCols = listColumns(conn, table);
                List<String> valueCols = new ArrayList<>();
                for (String c : allCols) {
                    if (!pkCols.contains(c)) valueCols.add(c);
                }
                if (valueCols.isEmpty()) {
                    log("[WARN] Table '" + displayName(table) + "' skipped: no value columns.");
                    continue;
                }

                DatabaseConfig dbCfg = new DatabaseConfig();
                dbCfg.setAllowCreate(true);
                String dbName = table; // one JE database per table
                try (Database db = env.openDatabase(null, dbName, dbCfg)) {
                    long written = 0;
                    String quotedTable = '"' + table + '"';
                    String sql = "SELECT * FROM public." + quotedTable;
                    try (Statement st = conn.createStatement(ResultSet.TYPE_FORWARD_ONLY, ResultSet.CONCUR_READ_ONLY)) {
                        st.setFetchSize(1000);
                        try (ResultSet rs = st.executeQuery(sql)) {
                            ResultSetMetaData md = rs.getMetaData();
                            int colCount = md.getColumnCount();
                            while (rs.next()) {
                                String key = buildKey(rs, pkCols, md);
                                String json = buildJson(rs, valueCols, md);
                                DatabaseEntry k = new DatabaseEntry(key.getBytes(StandardCharsets.UTF_8));
                                DatabaseEntry v = new DatabaseEntry(json.getBytes(StandardCharsets.UTF_8));
                                db.put(null, k, v);
                                written++;
                            }
                        }
                    }
                    log(String.format("[PgToBerkeley] Table %s -> rows: %d", displayName(table), written));
                }
            }
        }
    }

    private static List<String> listPublicTables(Connection conn) throws SQLException {
        String q = "SELECT table_name FROM information_schema.tables WHERE table_schema='public' AND table_type='BASE TABLE' ORDER BY table_name";
        List<String> out = new ArrayList<>();
        try (Statement st = conn.createStatement(); ResultSet rs = st.executeQuery(q)) {
            while (rs.next()) out.add(rs.getString(1));
        }
        return out;
    }

    private static List<String> listPrimaryKeyColumns(Connection conn, String table) throws SQLException {
        String q = "SELECT kcu.column_name\n" +
                "FROM information_schema.table_constraints tc\n" +
                "JOIN information_schema.key_column_usage kcu\n" +
                "  ON tc.constraint_name = kcu.constraint_name AND tc.table_schema = kcu.table_schema\n" +
                "WHERE tc.table_schema='public' AND tc.table_name=? AND tc.constraint_type='PRIMARY KEY'\n" +
                "ORDER BY kcu.ordinal_position";
        List<String> out = new ArrayList<>();
        try (PreparedStatement ps = conn.prepareStatement(q)) {
            ps.setString(1, table);
            try (ResultSet rs = ps.executeQuery()) {
                while (rs.next()) out.add(rs.getString(1));
            }
        }
        return out;
    }

    private static List<String> listColumns(Connection conn, String table) throws SQLException {
        String q = "SELECT column_name FROM information_schema.columns WHERE table_schema='public' AND table_name=? ORDER BY ordinal_position";
        List<String> out = new ArrayList<>();
        try (PreparedStatement ps = conn.prepareStatement(q)) {
            ps.setString(1, table);
            try (ResultSet rs = ps.executeQuery()) {
                while (rs.next()) out.add(rs.getString(1));
            }
        }
        return out;
    }

    private static String buildKey(ResultSet rs, List<String> pkCols, ResultSetMetaData md) throws SQLException {
        // Если PK составной, ключ = значения через '|'
        StringBuilder sb = new StringBuilder();
        for (int i = 0; i < pkCols.size(); i++) {
            String col = pkCols.get(i);
            Object val = rs.getObject(col);
            if (val != null) sb.append(val.toString());
            if (i < pkCols.size() - 1) sb.append('|');
        }
        return sb.toString();
    }

    private static String buildJson(ResultSet rs, List<String> cols, ResultSetMetaData md) throws SQLException {
        StringBuilder sb = new StringBuilder();
        sb.append('{');
        for (int i = 0; i < cols.size(); i++) {
            String col = cols.get(i);
            Object val = rs.getObject(col);
            sb.append('"').append(escapeJson(col)).append('"').append(':');
            if (val == null) {
                sb.append("null");
            } else if (val instanceof Number) {
                sb.append(val.toString());
            } else if (val instanceof Boolean) {
                sb.append(((Boolean) val) ? "true" : "false");
            } else {
                sb.append('"').append(escapeJson(val.toString())).append('"');
            }
            if (i < cols.size() - 1) sb.append(',');
        }
        sb.append('}');
        return sb.toString();
    }

    private static String escapeJson(String s) {
        StringBuilder sb = new StringBuilder();
        for (int i = 0; i < s.length(); i++) {
            char c = s.charAt(i);
            switch (c) {
                case '\\': sb.append("\\\\"); break;
                case '"': sb.append("\\\""); break;
                case '\n': sb.append("\\n"); break;
                case '\r': sb.append("\\r"); break;
                case '\t': sb.append("\\t"); break;
                default:
                    if (c < 0x20) {
                        sb.append(String.format("\\u%04x", (int)c));
                    } else {
                        sb.append(c);
                    }
            }
        }
        return sb.toString();
    }

    private static String asciiSafe(String s) {
        if (s == null) return "";
        StringBuilder sb = new StringBuilder();
        for (int i = 0; i < s.length(); i++) {
            char c = s.charAt(i);
            if (c <= 0x7F) {
                sb.append(c);
            } else {
                sb.append(String.format("\\u%04X", (int) c));
            }
        }
        return sb.toString();
    }

    private static String displayName(String s) {
        return UNICODE_LOGS ? s : asciiSafe(s);
    }

    private static void log(String msg) {
        System.out.println(msg);
        if (LOG != null) {
            LOG.println(msg);
        }
    }

    private static void logErr(String msg) {
        System.err.println(msg);
        if (LOG != null) {
            LOG.println(msg);
        }
    }
}