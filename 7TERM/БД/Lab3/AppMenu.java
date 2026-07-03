import java.io.*;
import java.nio.charset.StandardCharsets;
import java.nio.file.*;
import java.util.*;
import java.net.*;

public class AppMenu {
    private static final String DEFAULT_ENV = "berkeley_data";
    private static final String CONFIG_FILE = "config.json";

    private static Path getBaseDir() {
        try {
            Path loc = Paths.get(AppMenu.class.getProtectionDomain().getCodeSource().getLocation().toURI());
            // Если класс загружен из каталога (например, Lab3), используем его напрямую
            if (Files.isDirectory(loc)) {
                return loc;
            }
            // Иначе это путь к JAR/файлу — используем его родитель как базу
            Path parent = loc.getParent();
            return parent != null ? parent : Paths.get(System.getProperty("user.dir"));
        } catch (Exception e) {
            // Фолбек: если есть подкаталог Lab3 в текущей директории — используем его
            Path cwd = Paths.get(System.getProperty("user.dir"));
            Path lab3 = cwd.resolve("Lab3");
            return Files.isDirectory(lab3) ? lab3 : cwd;
        }
    }

    private static String resolveEnv(Path baseDir) {
        Path cfg = baseDir.resolve(CONFIG_FILE);
        if (Files.exists(cfg)) {
            try {
                String text = Files.readString(cfg, StandardCharsets.UTF_8);
                // Простой разбор без зависимостей
                java.util.regex.Matcher m = java.util.regex.Pattern
                    .compile("\"berkeley_env\"\\s*:\\s*\"([^\"]+)\"")
                    .matcher(text);
                if (m.find()) {
                    String val = m.group(1).trim();
                    if (val.isEmpty()) {
                        return baseDir.resolve(DEFAULT_ENV).toString();
                    }
                    Path p = Paths.get(val);
                    return p.isAbsolute() ? p.toString() : baseDir.resolve(val).toString();
                }
            } catch (IOException ignored) {}
        }
        return baseDir.resolve(DEFAULT_ENV).toString();
    }

    private static void runConverter(Path baseDir) {
        // Запускаем конвертер во внешнем процессе, чтобы System.exit внутри него не завершал меню
        try {
            String javaHome = System.getProperty("java.home");
            Path javaBin = Paths.get(javaHome, "bin", "java.exe");
            String javaExe = Files.exists(javaBin) ? javaBin.toString() : "java";

            // Собираем classpath: Lab3 + все JAR'ы из Lab3/lib
            StringBuilder cp = new StringBuilder(baseDir.toString());
            Path lib = baseDir.resolve("lib");
            if (Files.isDirectory(lib)) {
                try (DirectoryStream<Path> stream = Files.newDirectoryStream(lib, "*.jar")) {
                    for (Path p : stream) {
                        cp.append(';').append(p.toAbsolutePath());
                    }
                }
            }

            List<String> cmd = new ArrayList<>();
            cmd.add(javaExe);
            cmd.add("-Dfile.encoding=UTF-8");
            cmd.add("-cp");
            cmd.add(cp.toString());
            cmd.add("PgToBerkeley");

            ProcessBuilder pb = new ProcessBuilder(cmd);
            Path workDir = baseDir.getParent() != null ? baseDir.getParent() : baseDir;
            pb.directory(workDir.toFile());
            // Включаем Unicode-логи конвертера
            Map<String, String> env = pb.environment();
            env.put("PG2B_LOGS", "utf8");
            pb.redirectErrorStream(true);
            Process proc = pb.start();
            try (BufferedReader br = new BufferedReader(new InputStreamReader(proc.getInputStream(), StandardCharsets.UTF_8))) {
                String line;
                while ((line = br.readLine()) != null) {
                    System.out.println(line);
                }
            }
            int code = proc.waitFor();
            if (code != 0) {
                System.out.println("Конвертер завершился с кодом: " + code);
                System.out.println("Если видите ошибки драйвера (No suitable driver), добавьте postgresql-*.jar в 'Lab3/lib'.");
            }
        } catch (Throwable e) {
            System.out.println("Ошибка запуска конвертера: " + e.getMessage());
            System.out.println("Проверьте зависимости: требуется JAR Berkeley JE (например, je-18.3.12.jar) и драйвер PostgreSQL (например, postgresql-42.7.3.jar). Поместите их в папку 'Lab3/lib' и запустите снова.");
        }
    }

    private static void listTables(String env) {
        if (!Files.exists(Paths.get(env))) {
            System.out.println("Среда Berkeley DB не найдена: " + Paths.get(env).toAbsolutePath());
            System.out.println("Запустите сначала конвертацию (пункт 1), чтобы создать данные.");
            return;
        }
        try {
            DumpBerkeley.main(new String[]{"--env", env, "--list"});
        } catch (Throwable e) {
            System.out.println("Ошибка списка таблиц: " + e.getMessage());
            System.out.println("Вероятно, не найден JAR Berkeley JE. Поместите je-*.jar в папку 'Lab3/lib'.");
        }
    }

    private static void showTable(String env, String table) {
        if (!Files.exists(Paths.get(env))) {
            System.out.println("Среда Berkeley DB не найдена: " + Paths.get(env).toAbsolutePath());
            System.out.println("Запустите сначала конвертацию (пункт 1).");
            return;
        }
        try {
            DumpBerkeley.main(new String[]{"--env", env, table});
        } catch (Throwable e) {
            System.out.println("Ошибка просмотра таблицы: " + e.getMessage());
            System.out.println("Вероятно, не найден JAR Berkeley JE. Поместите je-*.jar в папку 'Lab3/lib'.");
        }
    }

    private static void pause(Scanner sc) {
        System.out.print("Нажмите Enter, чтобы вернуться в меню...");
        try { sc.nextLine(); } catch (Exception ignored) {}
        System.out.println();
    }

    private static void saveTable(String env, String table, Path outFile) {
        PrintStream orig = System.out;
        try (PrintStream ps = new PrintStream(Files.newOutputStream(outFile), true, StandardCharsets.UTF_8)) {
            System.setOut(ps);
            try {
                if (!Files.exists(Paths.get(env))) {
                    orig.println("Среда Berkeley DB не найдена: " + Paths.get(env).toAbsolutePath());
                    orig.println("Запустите конвертацию (пункт 1) и повторите.");
                } else {
                    DumpBerkeley.main(new String[]{"--env", env, table});
                }
            } catch (Throwable e) {
                orig.println("Ошибка сохранения таблицы: " + e.getMessage());
                orig.println("Вероятно, не найден JAR Berkeley JE. Поместите je-*.jar в папку 'Lab3/lib'.");
            }
        } catch (IOException e) {
            orig.println("Ошибка записи файла: " + e.getMessage());
        } finally {
            System.setOut(orig);
        }
    }

    private static void printMenu() {
        System.out.println();
        System.out.println("Выберите действие:");
        System.out.println("  1. Запустить конвертацию (PostgreSQL → Berkeley DB)");
        System.out.println("  2. Показать список таблиц");
        System.out.println("  3. Показать таблицу");
        System.out.println("  4. Сохранить таблицу в файл (UTF-8)");
        System.out.println("  0. Выход");
        System.out.print("Ваш выбор: ");
    }

    public static void main(String[] args) {
        try {
            System.setOut(new PrintStream(new BufferedOutputStream(new FileOutputStream(FileDescriptor.out)), true, StandardCharsets.UTF_8));
            System.setErr(new PrintStream(new BufferedOutputStream(new FileOutputStream(FileDescriptor.err)), true, StandardCharsets.UTF_8));
        } catch (Exception ignore) {}
        // Базовая папка — та, где лежит AppMenu.class (Lab3)
        Path baseDir = getBaseDir();
        String env = resolveEnv(baseDir);

        Scanner sc = new Scanner(System.in, StandardCharsets.UTF_8);

        while (true) {
            printMenu();
            String choice = sc.nextLine().trim();
            switch (choice) {
                case "1":
                    System.out.println("→ Запуск конвертации...");
                    runConverter(baseDir);
                    System.out.println("✓ Конвертация завершена");
                    pause(sc);
                    break;
                case "2":
                    System.out.println("→ Список таблиц (env=" + env + "):");
                    listTables(env);
                    pause(sc);
                    break;
                case "3":
                    System.out.print("Введите имя таблицы: ");
                    String table = sc.nextLine().trim();
                    if (table.isEmpty()) {
                        System.out.println("Имя таблицы не задано");
                        break;
                    }
                    System.out.println("→ Печать таблицы '" + table + "' (env=" + env + "):");
                    showTable(env, table);
                    pause(sc);
                    break;
                case "4":
                    System.out.print("Имя таблицы: ");
                    String t = sc.nextLine().trim();
                    if (t.isEmpty()) { System.out.println("Имя таблицы не задано"); break; }
                    System.out.print("Путь файла (например, view_" + t + ".txt): ");
                    String path = sc.nextLine().trim();
                    if (path.isEmpty()) { System.out.println("Путь файла не задан"); break; }
                    Path out = baseDir.resolve(path);
                    System.out.println("→ Сохранение в файл: " + out.toAbsolutePath());
                    saveTable(env, t, out);
                    System.out.println("✓ Готово");
                    pause(sc);
                    break;
                case "0":
                    System.out.println("Выход");
                    return;
                default:
                    System.out.println("Неизвестный выбор: '" + choice + "'. Попробуйте снова.");
            }
        }
    }
}