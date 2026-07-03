import java.io.*;
import java.nio.charset.StandardCharsets;
import java.nio.file.*;
import java.util.*;

import com.sleepycat.je.*;

/**
 * DumpBerkeley — простой просмотрщик для Berkeley DB JE.
 * Печатает пары key/value как UTF‑8 (без hex/\xNN),
 * чтобы удобно смотреть JSON-значения.
 *
 * Использование:
 *   java -cp <Lab3>;<je.jar> DumpBerkeley [--env <path>] [--out <file>] <dbName>
 *   java -cp <Lab3>;<je.jar> DumpBerkeley [--env <path>] --list
 */
public class DumpBerkeley {
    public static void main(String[] args) throws Exception {
        Path envPath = Paths.get("Lab3", "berkeley_data");
        String dbName = null;
        Path outFile = null;
        boolean listOnly = false;

        for (int i = 0; i < args.length; i++) {
            String a = args[i];
            switch (a) {
                case "--env":
                    envPath = Paths.get(args[++i]);
                    break;
                case "--out":
                    outFile = Paths.get(args[++i]);
                    break;
                case "--list":
                    listOnly = true;
                    break;
                default:
                    dbName = a;
            }
        }

        if (!Files.exists(envPath)) {
            throw new IllegalStateException("Environment not found: " + envPath.toAbsolutePath());
        }

        EnvironmentConfig cfg = new EnvironmentConfig();
        cfg.setAllowCreate(false);
        try (Environment env = new Environment(envPath.toFile(), cfg)) {
            if (listOnly) {
                for (String name : env.getDatabaseNames()) {
                    System.out.println(name);
                }
                return;
            }

            if (dbName == null) {
                throw new IllegalArgumentException("Usage: DumpBerkeley [--env <path>] [--out <file>] <dbName> | --list");
            }

            DatabaseConfig dc = new DatabaseConfig();
            dc.setAllowCreate(false);
            try (Database db = env.openDatabase(null, dbName, dc)) {
                boolean toFile = (outFile != null);
                if (toFile) {
                    try (BufferedWriter bw = Files.newBufferedWriter(outFile, StandardCharsets.UTF_8)) {
                        bw.write("VERSION=viewer\n");
                        bw.write("db=" + dbName + "\n");
                        bw.write("HEADER=END\n");
                        Cursor cursor = db.openCursor(null, null);
                        DatabaseEntry k = new DatabaseEntry();
                        DatabaseEntry v = new DatabaseEntry();
                        while (cursor.getNext(k, v, LockMode.DEFAULT) == OperationStatus.SUCCESS) {
                            String key = new String(k.getData(), StandardCharsets.UTF_8);
                            String val = new String(v.getData(), StandardCharsets.UTF_8);
                            bw.write(key);
                            bw.write("\n");
                            bw.write(val);
                            bw.write("\n");
                        }
                        cursor.close();
                        bw.flush();
                    }
                } else {
                    BufferedWriter bw = new BufferedWriter(new OutputStreamWriter(System.out, StandardCharsets.UTF_8));
                    bw.write("VERSION=viewer\n");
                    bw.write("db=" + dbName + "\n");
                    bw.write("HEADER=END\n");
                    Cursor cursor = db.openCursor(null, null);
                    DatabaseEntry k = new DatabaseEntry();
                    DatabaseEntry v = new DatabaseEntry();
                    while (cursor.getNext(k, v, LockMode.DEFAULT) == OperationStatus.SUCCESS) {
                        String key = new String(k.getData(), StandardCharsets.UTF_8);
                        String val = new String(v.getData(), StandardCharsets.UTF_8);
                        bw.write(key);
                        bw.write("\n");
                        bw.write(val);
                        bw.write("\n");
                    }
                    cursor.close();
                    bw.flush(); // Не закрываем System.out
                }
            }
        }
    }
}