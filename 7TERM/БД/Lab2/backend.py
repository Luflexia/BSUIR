# backend.py

import psycopg2
from fastapi import FastAPI, HTTPException, Depends, status, Response
from fastapi.security import HTTPBasic, HTTPBasicCredentials
from pydantic import BaseModel
import uvicorn
from typing import List, Dict, Any
import os
import subprocess
import datetime

from config import DB_CONFIG, SUPERUSER_PASSWORD, LOOKUP_TABLES

# Type normalization for user-friendly inputs
TYPE_ALIASES = {
    'int': 'INTEGER',
    'integer': 'INTEGER',
    'bigint': 'BIGINT',
    'smallint': 'SMALLINT',
    'serial': 'SERIAL',
    'serial4': 'SERIAL',
    'serial8': 'BIGSERIAL',
    'bool': 'BOOLEAN',
    'boolean': 'BOOLEAN',
    'text': 'TEXT',
    'varchar': 'VARCHAR',
    'char': 'CHAR',
    'date': 'DATE',
    'time': 'TIME',
    'timestamp': 'TIMESTAMP',
    'numeric': 'NUMERIC',
    'decimal': 'DECIMAL',
    'float': 'DOUBLE PRECISION',
    'double': 'DOUBLE PRECISION',
    'real': 'REAL'
}

def normalize_type(typ: str) -> str:
    t = typ.strip()
    tl = t.lower()
    # Preserve parameterized types like varchar(100), numeric(10,2)
    if '(' in t and ')' in t:
        base = tl.split('(')[0]
        if base in TYPE_ALIASES and base not in ['varchar', 'char', 'numeric', 'decimal']:
            # For fixed alias types with params we ignore params
            return TYPE_ALIASES[base]
        # Force base name to proper case but keep params
        if base in ['varchar', 'char', 'numeric', 'decimal']:
            proper = base.upper()
            return proper + t[t.find('('):]
        return t
    # Non-parameterized alias
    if tl in TYPE_ALIASES:
        return TYPE_ALIASES[tl]
    return t

app = FastAPI(
    title="Railway DB API",
    description="API for managing the railway database, with role-based access control.",
    version="1.0.0"
)

security = HTTPBasic()
security_optional = HTTPBasic(auto_error=False)

def get_db_connection():
    """Establishes a connection to the database."""
    try:
        conn = psycopg2.connect(**DB_CONFIG)
        yield conn
    finally:
        conn.close()

class User(BaseModel):
    username: str

def get_current_user(credentials: HTTPBasicCredentials = Depends(security)):
    if credentials.username == 'user' and credentials.password == 'user':
        return {"role": "user"}
    elif credentials.username == 'superuser' and credentials.password == SUPERUSER_PASSWORD:
        return {"role": "superuser"}
    else:
        raise HTTPException(
            status_code=status.HTTP_401_UNAUTHORIZED,
            detail="Incorrect username or password",
            headers={"WWW-Authenticate": "Basic"},
        )

# Optional auth for read-only endpoints

def get_user_or_guest(credentials: HTTPBasicCredentials | None = Depends(security_optional)):
    if credentials is None:
        return {"role": "user"}
    if credentials.username == 'user' and credentials.password == 'user':
        return {"role": "user"}
    elif credentials.username == 'superuser' and credentials.password == SUPERUSER_PASSWORD:
        return {"role": "superuser"}
    else:
        # If provided but incorrect, treat as unauthenticated user for read-only
        return {"role": "user"}

@app.get("/tables", response_model=List[str], summary="Get a list of all tables")
def get_tables(conn=Depends(get_db_connection), user: dict = Depends(get_user_or_guest)):
    """Retrieves a list of all tables in the public schema."""
    try:
        with conn.cursor() as cursor:
            cursor.execute("""
                SELECT table_name FROM information_schema.tables
                WHERE table_schema = 'public'
            """)
            tables = [table[0] for table in cursor.fetchall()]
            return tables
    except Exception as e:
        raise HTTPException(status_code=500, detail=str(e))

@app.get("/tables/{table_name}", response_model=List[Dict[str, Any]], summary="Get table content")
def get_table_content(table_name: str, conn=Depends(get_db_connection), user: dict = Depends(get_user_or_guest)):
    """Retrieves the content of a specific table."""
    try:
        with conn.cursor() as cursor:
            # Sanitize table_name to prevent SQL injection
            cursor.execute("SELECT table_name FROM information_schema.tables WHERE table_name = %s", (table_name,))
            if cursor.fetchone() is None:
                raise HTTPException(status_code=404, detail="Table not found")

            cursor.execute(f'SELECT * FROM "{table_name}"')
            columns = [desc[0] for desc in cursor.description]
            rows = cursor.fetchall()
            
            data = []
            for row in rows:
                row_dict = {}
                for i, value in enumerate(row):
                    col_name = columns[i]
                    if isinstance(value, datetime.timedelta):
                        total_seconds = int(value.total_seconds())
                        hours, remainder = divmod(total_seconds, 3600)
                        minutes, seconds = divmod(remainder, 60)
                        row_dict[col_name] = f"{hours:02}:{minutes:02}:{seconds:02}"
                    elif isinstance(value, datetime.time):
                        row_dict[col_name] = value.strftime('%H:%M:%S')
                    else:
                        row_dict[col_name] = value
                data.append(row_dict)
            return data
    except Exception as e:
        raise HTTPException(status_code=500, detail=str(e))

@app.get("/tables/{table_name}/schema", response_model=List[Dict[str, Any]], summary="Get table schema")
def get_table_schema(table_name: str, conn=Depends(get_db_connection), user: dict = Depends(get_user_or_guest)):
    try:
        with conn.cursor() as cursor:
            cursor.execute("SELECT table_name FROM information_schema.tables WHERE table_name = %s", (table_name,))
            if cursor.fetchone() is None:
                raise HTTPException(status_code=404, detail="Table not found")
            cursor.execute("""
                SELECT column_name, data_type
                FROM information_schema.columns
                WHERE table_schema='public' AND table_name=%s
                ORDER BY ordinal_position
            """, (table_name,))
            rows = cursor.fetchall()
            return [{"name": r[0], "type": r[1]} for r in rows]
    except Exception as e:
        raise HTTPException(status_code=500, detail=str(e))

class Row(BaseModel):
    data: Dict[str, Any]

class ColumnDefinition(BaseModel):
    name: str
    type: str

class ColumnInfo(BaseModel):
    name: str
    type: str

class CreateTableRequest(BaseModel):
    columns: List[ColumnDefinition]

class TableRestoreRequest(BaseModel):
    file_path: str

class DatabaseRestoreRequest(BaseModel):
    backup_file: str

class QuerySave(BaseModel):
    query: str

@app.post("/tables/{table_name}", summary="Add a new row to a table")
def add_row(table_name: str, row: Row, conn=Depends(get_db_connection), user: dict = Depends(get_current_user)):
    if f'"{table_name}"' in LOOKUP_TABLES and user['role'] != 'superuser':
        raise HTTPException(status_code=403, detail="User cannot edit lookup tables")
    try:
        with conn.cursor() as cursor:
            columns = ', '.join(row.data.keys())
            values = ', '.join(['%s'] * len(row.data.values()))
            query = f'INSERT INTO "{table_name}" ({columns}) VALUES ({values})'
            cursor.execute(query, list(row.data.values()))
            conn.commit()
            return {"message": "Row added successfully"}
    except Exception as e:
        conn.rollback()
        raise HTTPException(status_code=500, detail=str(e))

@app.put("/tables/{table_name}/rows/{row_id}", summary="Update a row in a table")
def update_row(table_name: str, row_id: int, row: Row, conn=Depends(get_db_connection), user: dict = Depends(get_current_user)):
    if f'"{table_name}"' in LOOKUP_TABLES and user['role'] != 'superuser':
        raise HTTPException(status_code=403, detail="User cannot edit lookup tables")
    try:
        with conn.cursor() as cursor:
            updates = ', '.join([f"{key} = %s" for key in row.data.keys()])
            query = f'UPDATE "{table_name}" SET {updates} WHERE id = %s'
            cursor.execute(query, list(row.data.values()) + [row_id])
            conn.commit()
            return {"message": "Row updated successfully"}
    except Exception as e:
        conn.rollback()
        raise HTTPException(status_code=500, detail=str(e))

@app.delete("/tables/{table_name}/rows/{row_id}", summary="Delete a row from a table")
def delete_row(table_name: str, row_id: int, conn=Depends(get_db_connection), user: dict = Depends(get_current_user)):
    if f'"{table_name}"' in LOOKUP_TABLES and user['role'] != 'superuser':
        raise HTTPException(status_code=403, detail="User cannot edit lookup tables")
    try:
        with conn.cursor() as cursor:
            query = f'DELETE FROM "{table_name}" WHERE id = %s'
            cursor.execute(query, (row_id,))
            conn.commit()
            return {"message": "Row deleted successfully"}
    except Exception as e:
        conn.rollback()
        raise HTTPException(status_code=500, detail=str(e))

class Query(BaseModel):
    query: str

@app.post("/query", summary="Execute a custom SQL query")
def execute_query(query: Query, conn=Depends(get_db_connection), user: dict = Depends(get_current_user)):
    try:
        with conn.cursor() as cursor:
            cursor.execute(query.query)
            if cursor.description:
                columns = [desc[0] for desc in cursor.description]
                rows = cursor.fetchall()
                
                data = []
                for row in rows:
                    row_dict = {}
                    for i, value in enumerate(row):
                        col_name = columns[i]
                        if isinstance(value, datetime.timedelta):
                            total_seconds = int(value.total_seconds())
                            hours, remainder = divmod(total_seconds, 3600)
                            minutes, seconds = divmod(remainder, 60)
                            row_dict[col_name] = f"{hours:02}:{minutes:02}:{seconds:02}"
                        elif isinstance(value, datetime.time):
                            row_dict[col_name] = value.strftime('%H:%M:%S')
                        else:
                            row_dict[col_name] = value
                    data.append(row_dict)
                return {"columns": columns, "data": data}
            else:
                conn.commit()
                return {"message": "Query executed successfully"}
    except Exception as e:
        conn.rollback()
        raise HTTPException(status_code=500, detail=str(e))

@app.post("/queries/new", summary="Create a new query")
def new_query(user: dict = Depends(get_current_user)):
    # This endpoint is mainly for frontend to clear the query text
    return {"message": "New query created"}

@app.post("/queries/run_saved", summary="Run a saved query")
def run_saved_query(query: Query, conn=Depends(get_db_connection), user: dict = Depends(get_current_user)):
    return execute_query(query, conn, user)





@app.post("/tables/{table_name}/create", summary="Create a table")
def create_table(table_name: str, request: CreateTableRequest, conn=Depends(get_db_connection), user: dict = Depends(get_current_user)):
    # Restrict only for lookup tables; allow normal user for other tables
    if f'"{table_name}"' in LOOKUP_TABLES and user['role'] != 'superuser':
        raise HTTPException(status_code=403, detail="Only superusers can create lookup tables")
    try:
        with conn.cursor() as cursor:
            cols = []
            seen = set()
            for c in request.columns:
                name = c.name.strip()
                typ = normalize_type(c.type.strip())
                if not name or not typ:
                    raise HTTPException(status_code=422, detail="Column name and type are required")
                if name.lower() in seen:
                    raise HTTPException(status_code=422, detail=f"Duplicate column name: {name}")
                seen.add(name.lower())
                cols.append(ColumnDefinition(name=name, type=typ))

            has_id = any(col.name.lower() == 'id' for col in cols)
            if not has_id:
                cols.insert(0, ColumnDefinition(name='id', type='SERIAL PRIMARY KEY'))

            column_defs = ', '.join([f'"{col.name}" {col.type}' for col in cols])
            create_query = f'CREATE TABLE "{table_name}" ({column_defs})'
            cursor.execute(create_query)
            # Insert one empty row (NULLs) to match UI expectations
            non_id_cols = [f'"{col.name}"' for col in cols if col.name.lower() != 'id']
            if non_id_cols:
                placeholders = ', '.join(['%s'] * len(non_id_cols))
                insert_query = f'INSERT INTO "{table_name}" ({", ".join(non_id_cols)}) VALUES ({placeholders})'
                cursor.execute(insert_query, [None] * len(non_id_cols))
            conn.commit()
            return {"message": f"Table {table_name} created successfully."}
    except HTTPException:
        raise
    except Exception as e:
        conn.rollback()
        raise HTTPException(status_code=500, detail=str(e))

@app.delete("/tables/{table_name}/delete", summary="Delete a table")
def delete_table(table_name: str, conn=Depends(get_db_connection), user: dict = Depends(get_current_user)):
    # Restrict only lookup tables for superuser
    if f'"{table_name}"' in LOOKUP_TABLES and user['role'] != 'superuser':
        raise HTTPException(status_code=403, detail="Only superusers can delete lookup tables")
    try:
        with conn.cursor() as cursor:
            cursor.execute(f'DROP TABLE "{table_name}"')
            conn.commit()
            return {"message": f"Table {table_name} deleted successfully."}
    except Exception as e:
        conn.rollback()
        raise HTTPException(status_code=500, detail=str(e))

import pandas as pd

@app.post("/tables/{table_name}/backup", summary="Backup a table")
def backup_table(table_name: str, conn=Depends(get_db_connection), user: dict = Depends(get_current_user)):
    # Allow backup for any table; restrict edits elsewhere
    try:
        with conn.cursor() as cursor:
            cursor.execute(f'SELECT * FROM "{table_name}"')
            data = cursor.fetchall()
            columns = [desc[0] for desc in cursor.description]
            df = pd.DataFrame(data, columns=columns)
            backup_dir = "table_backups"
            if not os.path.exists(backup_dir):
                os.makedirs(backup_dir)
            timestamp = datetime.datetime.now().strftime("%Y%m%d%H%M%S")
            backup_file = os.path.join(backup_dir, f'{table_name}_backup_{timestamp}.xlsx')
            df.to_excel(backup_file, index=False)
            return {"message": f"Table {table_name} backed up successfully to {backup_file}"}
    except Exception as e:
        raise HTTPException(status_code=500, detail=str(e))

@app.post("/tables/{table_name}/restore", summary="Restore a table from Excel")
def restore_table(table_name: str, request: TableRestoreRequest, conn=Depends(get_db_connection), user: dict = Depends(get_current_user)):
    if user['role'] != 'superuser':
        raise HTTPException(status_code=403, detail="Only superusers can restore tables")
    try:
        df = pd.read_excel(request.file_path)
        with conn.cursor() as cursor:
            cursor.execute(f'TRUNCATE TABLE "{table_name}" RESTART IDENTITY CASCADE')
            columns = ', '.join([f'"{col}"' for col in df.columns])
            values_placeholder = ', '.join(['%s'] * len(df.columns))
            insert_query = f'INSERT INTO "{table_name}" ({columns}) VALUES ({values_placeholder})'
            for _, row in df.iterrows():
                cursor.execute(insert_query, tuple(row))
            conn.commit()
            return {"message": f"Table {table_name} restored successfully from {request.file_path}"}
    except Exception as e:
        conn.rollback()
        raise HTTPException(status_code=500, detail=str(e))

@app.post("/database/backup", summary="Backup database")
def backup_database(user: dict = Depends(get_current_user)):
    # Allow backup for any user; operation is read-only
    backup_dir = "database_backups"
    if not os.path.exists(backup_dir):
        os.makedirs(backup_dir)

    timestamp = datetime.datetime.now().strftime("%Y%m%d%H%M%S")
    backup_file = os.path.join(backup_dir, f"backup_{timestamp}.sql")

    try:
        command = [
            "pg_dump",
            "-U", DB_CONFIG["user"],
            "-h", DB_CONFIG["host"],
            "-p", str(DB_CONFIG["port"]),
            "-d", DB_CONFIG["database"],
            "-f", backup_file,
            "--format=p",
            "--no-owner",
            "--no-acl"
        ]
        env = os.environ.copy()
        env["PGPASSWORD"] = DB_CONFIG["password"]
        subprocess.run(command, check=True, env=env, capture_output=True, text=True)
        return {"message": f"Backup created successfully at {backup_file}"}
    except subprocess.CalledProcessError as e:
        raise HTTPException(status_code=500, detail=f"Backup failed: {e.stderr}")
    except Exception as e:
        raise HTTPException(status_code=400, detail=str(e))

@app.post("/database/restore", summary="Restore database from SQL")
def restore_database(request: DatabaseRestoreRequest, user: dict = Depends(get_current_user)):
    # Allow restore for any user (uses DB_CONFIG credentials under the hood)
    try:
        if not os.path.exists(request.backup_file):
            raise HTTPException(status_code=400, detail=f"Backup file not found: {request.backup_file}")

        temp_conn = psycopg2.connect(
            host=DB_CONFIG["host"],
            port=DB_CONFIG["port"],
            database='postgres',
            user=DB_CONFIG["user"],
            password=DB_CONFIG["password"]
        )
        temp_conn.autocommit = True
        temp_cursor = temp_conn.cursor()
        # Terminate any connections to the target DB before dropping
        temp_cursor.execute(
            """
            SELECT pg_terminate_backend(pid)
            FROM pg_stat_activity
            WHERE datname = %s AND pid <> pg_backend_pid();
            """,
            (DB_CONFIG['database'],)
        )
        temp_cursor.execute(f"DROP DATABASE IF EXISTS \"{DB_CONFIG['database']}\"")
        temp_cursor.execute(f"CREATE DATABASE \"{DB_CONFIG['database']}\"")
        temp_cursor.close()
        temp_conn.close()

        command = [
            "psql",
            "-U", DB_CONFIG["user"],
            "-h", DB_CONFIG["host"],
            "-p", str(DB_CONFIG["port"]),
            "-d", DB_CONFIG["database"],
            "-f", request.backup_file
        ]
        env = os.environ.copy()
        env["PGPASSWORD"] = DB_CONFIG["password"]
        subprocess.run(command, check=True, env=env, capture_output=True, text=True)
        return {"message": "Database restored successfully."}
    except subprocess.CalledProcessError as e:
        raise HTTPException(status_code=500, detail=f"Restore failed: {e.stderr}")
    except HTTPException as e:
        # pass-through for explicit HTTP errors above
        raise e
    except Exception as e:
        raise HTTPException(status_code=500, detail=f"Unexpected error: {str(e)}")

@app.get("/queries", summary="Get saved queries")
def get_saved_queries(user: dict = Depends(get_user_or_guest)):
    try:
        if not os.path.exists("saved_queries.txt"):
            return {"queries": []}
        with open("saved_queries.txt", "r", encoding="utf-8") as f:
            queries = [line.strip() for line in f.readlines()]
        return {"queries": queries}
    except Exception as e:
        raise HTTPException(status_code=500, detail=str(e))

@app.post("/queries", summary="Save a query")
def save_query(query: QuerySave, user: dict = Depends(get_user_or_guest)):
    try:
        with open("saved_queries.txt", "a", encoding="utf-8") as f:
            f.write(query.query + '\n')
        return {"message": "Query saved successfully."}
    except Exception as e:
        raise HTTPException(status_code=500, detail=str(e))



if __name__ == "__main__":
    uvicorn.run(app, host="127.0.0.1", port=8000)