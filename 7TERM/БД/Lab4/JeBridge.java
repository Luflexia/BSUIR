import java.io.*;
import java.nio.charset.StandardCharsets;
import java.nio.file.*;
import java.util.*;

import com.sleepycat.je.*;

/**
 * JeBridge — утилита для операций записи/удаления в Berkeley DB JE.
 * Поддерживаемые операции:
 *   --env <path> --op list
 *   --env <path> --op dump --db <name>
 *   --env <path> --op put  --db <name> --key <k> --value <json>
 *   --env <path> --op del  --db <name> --key <k>
 *   --env <path> --op create_db --db <name>
 *   --env <path> --op drop_db   --db <name>
 *   --env <path> --op schema_set --db <name> --json <schema-json>
 *   --env <path> --op schema_get --db <name>
 *
 * Схема таблицы хранится как специальная запись с ключом "__schema__".
 */
public class JeBridge {
    public static void main(String[] args) throws Exception {
        Path envPath = null;
        String op = null;
        String dbName = null;
        String key = null;
        String value = null;
        String json = null;

        for (int i = 0; i < args.length; i++) {
            String a = args[i];
            switch (a) {
                case "--env": envPath = Paths.get(args[++i]); break;
                case "--op": op = args[++i]; break;
                case "--db": dbName = args[++i]; break;
                case "--key": key = args[++i]; break;
                case "--value": value = args[++i]; break;
                case "--json": json = args[++i]; break;
                default: // ignore
            }
        }

        if (envPath == null) throw new IllegalArgumentException("--env is required");
        if (!Files.exists(envPath)) throw new IllegalStateException("Env not found: " + envPath.toAbsolutePath());
        if (op == null) throw new IllegalArgumentException("--op is required");

        EnvironmentConfig cfg = new EnvironmentConfig();
        cfg.setAllowCreate(true);
        try (Environment env = new Environment(envPath.toFile(), cfg)) {
            switch (op) {
                case "list":
                    for (String name : env.getDatabaseNames()) {
                        System.out.println(name);
                    }
                    break;
                case "dump": {
                    require(dbName != null, "--db required");
                    DatabaseConfig dc = new DatabaseConfig();
                    dc.setAllowCreate(false);
                    try (Database db = env.openDatabase(null, dbName, dc)) {
                        Cursor cursor = db.openCursor(null, null);
                        DatabaseEntry k = new DatabaseEntry();
                        DatabaseEntry v = new DatabaseEntry();
                        System.out.println("VERSION=bridge");
                        System.out.println("db=" + dbName);
                        System.out.println("HEADER=END");
                        while (cursor.getNext(k, v, LockMode.DEFAULT) == OperationStatus.SUCCESS) {
                            String ks = new String(k.getData(), StandardCharsets.UTF_8);
                            String vs = new String(v.getData(), StandardCharsets.UTF_8);
                            System.out.println(ks);
                            System.out.println(vs);
                        }
                        cursor.close();
                    }
                    break;
                }
                case "put": {
                    require(dbName != null, "--db required");
                    require(key != null, "--key required");
                    require(value != null, "--value required");
                    DatabaseConfig dc = new DatabaseConfig();
                    dc.setAllowCreate(true);
                    try (Database db = env.openDatabase(null, dbName, dc)) {
                        DatabaseEntry k = new DatabaseEntry(key.getBytes(StandardCharsets.UTF_8));
                        DatabaseEntry v = new DatabaseEntry(value.getBytes(StandardCharsets.UTF_8));
                        db.put(null, k, v);
                    }
                    System.out.println("OK");
                    break;
                }
                case "del": {
                    require(dbName != null, "--db required");
                    require(key != null, "--key required");
                    DatabaseConfig dc = new DatabaseConfig();
                    dc.setAllowCreate(false);
                    try (Database db = env.openDatabase(null, dbName, dc)) {
                        DatabaseEntry k = new DatabaseEntry(key.getBytes(StandardCharsets.UTF_8));
                        OperationStatus st = db.delete(null, k);
                        if (st != OperationStatus.SUCCESS) {
                            System.out.println("NOT_FOUND");
                        } else {
                            System.out.println("OK");
                        }
                    }
                    break;
                }
                case "create_db": {
                    require(dbName != null, "--db required");
                    DatabaseConfig dc = new DatabaseConfig();
                    dc.setAllowCreate(true);
                    try (Database db = env.openDatabase(null, dbName, dc)) {
                        // just open to create
                    }
                    System.out.println("OK");
                    break;
                }
                case "drop_db": {
                    require(dbName != null, "--db required");
                    env.removeDatabase(null, dbName);
                    System.out.println("OK");
                    break;
                }
                case "schema_set": {
                    require(dbName != null, "--db required");
                    require(json != null, "--json required");
                    DatabaseConfig dc = new DatabaseConfig();
                    dc.setAllowCreate(true);
                    try (Database db = env.openDatabase(null, dbName, dc)) {
                        DatabaseEntry k = new DatabaseEntry("__schema__".getBytes(StandardCharsets.UTF_8));
                        DatabaseEntry v = new DatabaseEntry(json.getBytes(StandardCharsets.UTF_8));
                        db.put(null, k, v);
                    }
                    System.out.println("OK");
                    break;
                }
                case "schema_get": {
                    require(dbName != null, "--db required");
                    DatabaseConfig dc = new DatabaseConfig();
                    dc.setAllowCreate(false);
                    try (Database db = env.openDatabase(null, dbName, dc)) {
                        DatabaseEntry k = new DatabaseEntry("__schema__".getBytes(StandardCharsets.UTF_8));
                        DatabaseEntry v = new DatabaseEntry();
                        OperationStatus st = db.get(null, k, v, LockMode.DEFAULT);
                        if (st == OperationStatus.SUCCESS) {
                            System.out.println(new String(v.getData(), StandardCharsets.UTF_8));
                        } else {
                            System.out.println("{}");
                        }
                    }
                    break;
                }
                default:
                    throw new IllegalArgumentException("Unknown op: " + op);
            }
        }
    }

    private static void require(boolean cond, String msg) {
        if (!cond) throw new IllegalArgumentException(msg);
    }
}