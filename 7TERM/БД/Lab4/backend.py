from fastapi import FastAPI, HTTPException, Depends, status
from fastapi.responses import JSONResponse
from fastapi.security import HTTPBasic, HTTPBasicCredentials
from pydantic import BaseModel
import uvicorn
from typing import List, Dict, Any, Optional
from types import SimpleNamespace
import os
import subprocess
import datetime
import json
import pandas as pd

from config import JE_ENV_PATH, JE_JAR_PATH, SUPERUSER_PASSWORD, LOOKUP_TABLES, API_PORT

# Типы для совместимости с UI
TYPE_ALIASES = {
    'int': 'INTEGER',
    'integer': 'INTEGER',
    'bigint': 'BIGINT',
    'smallint': 'SMALLINT',
    'bool': 'BOOLEAN',
    'boolean': 'BOOLEAN',
    'text': 'TEXT',
    'varchar': 'VARCHAR',
    'char': 'CHAR',
    'date': 'DATE',
    'time': 'TIME'
}

def normalize_type(typ: str) -> str:
    return TYPE_ALIASES.get(typ.strip().lower(), typ)

app = FastAPI(
    title="Lab4 API (Berkeley DB)",
    description="Backend работает с Berkeley DB (JE) — без SQL.",
    version="1.0.0"
)

security = HTTPBasic()
security_optional = HTTPBasic(auto_error=False)

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

def get_user_or_guest(credentials: HTTPBasicCredentials | None = Depends(security_optional)):
    if credentials is None:
        return {"role": "user"}
    if credentials.username == 'user' and credentials.password == 'user':
        return {"role": "user"}
    elif credentials.username == 'superuser' and credentials.password == SUPERUSER_PASSWORD:
        return {"role": "superuser"}
    else:
        return {"role": "user"}

# -------------------- JE helpers --------------------

def _java_classpath_for_bridge() -> str:
    # Windows CP separator is ';'
    lab4_dir = os.path.dirname(os.path.abspath(__file__))
    return f"{JE_JAR_PATH};{lab4_dir}"

def _run_je_bridge(args: List[str]) -> subprocess.CompletedProcess:
    cp = _java_classpath_for_bridge()
    cmd = ['java', '-cp', cp, 'JeBridge'] + args
    env = os.environ.copy()
    # Prefer UTF-8 for Java default charset
    env['JAVA_TOOL_OPTIONS'] = env.get('JAVA_TOOL_OPTIONS', '') + ' -Dfile.encoding=UTF-8'
    # Capture raw bytes, then decode with UTF-8 fallback to CP1251
    proc = subprocess.run(cmd, capture_output=True, text=False, env=env)
    def _decode(buf: bytes) -> str:
        try:
            return buf.decode('utf-8')
        except UnicodeDecodeError:
            try:
                return buf.decode('cp1251')
            except UnicodeDecodeError:
                return buf.decode('utf-8', errors='replace')
    proc.stdout = _decode(proc.stdout)
    proc.stderr = _decode(proc.stderr)
    return proc

def _list_tables() -> List[str]:
    proc = _run_je_bridge(['--env', JE_ENV_PATH, '--op', 'list'])
    if proc.returncode != 0:
        raise HTTPException(status_code=500, detail=f"JE list error: {proc.stderr}")
    return [l.strip() for l in proc.stdout.splitlines() if l.strip()]

@app.exception_handler(HTTPException)
async def http_exception_handler(request, exc):
    return JSONResponse(
        status_code=exc.status_code,
        content={"detail": exc.detail},
    )

def _get_schema(table_name: str) -> Dict[str, Any]:
    proc = _run_je_bridge(['--env', JE_ENV_PATH, '--op', 'schema_get', '--db', table_name])
    if proc.returncode != 0:
        raise HTTPException(status_code=500, detail=f"JE schema_get error: {proc.stderr}")
    try:
        data = json.loads(proc.stdout.strip() or '{}')
        if not isinstance(data, dict):
            data = {}
        return {'pk': data.get('pk') or [], 'columns': data.get('columns') or {}}
    except json.JSONDecodeError:
        return {'pk': [], 'columns': {}}

def _set_schema(table_name: str, schema: Dict[str, Any]):
    payload = json.dumps(schema, ensure_ascii=False)
    proc = _run_je_bridge(['--env', JE_ENV_PATH, '--op', 'schema_set', '--db', table_name, '--json', payload])
    if proc.returncode != 0:
        raise HTTPException(status_code=500, detail=f"JE schema_set error: {proc.stderr}")

def _dump_table_raw(table_name: str) -> List[tuple]:
    proc = _run_je_bridge(['--env', JE_ENV_PATH, '--op', 'dump', '--db', table_name])
    if proc.returncode != 0:
        raise HTTPException(status_code=500, detail=f"JE dump error: {proc.stderr}")
    lines = [l.strip() for l in proc.stdout.splitlines() if l.strip()]
    pairs: List[tuple] = []
    i = 0
    while i < len(lines):
        if lines[i].startswith('VERSION=') or lines[i].startswith('HEADER=') or lines[i].startswith('db='):
            i += 1
            continue
        if i + 1 < len(lines):
            pairs.append((lines[i], lines[i+1]))
            i += 2
        else:
            break
    return pairs

def _compose_key_from_row(schema: Dict[str, Any], row: Dict[str, Any]) -> str:
    pk_cols = schema.get('pk') or []
    if not pk_cols:
        if '_key' in row:
            return str(row['_key'])
        raise HTTPException(status_code=400, detail='Schema PK is not defined for table')
    try:
        parts = [str(row[c]) for c in pk_cols]
    except KeyError as e:
        raise HTTPException(status_code=400, detail=f"Missing PK column in row: {e}")
    return '|'.join(parts)

def _split_row(schema: Dict[str, Any], key: str, value_json: str) -> Dict[str, Any]:
    row: Dict[str, Any] = {}
    try:
        val = json.loads(value_json)
        if not isinstance(val, dict):
            val = {'_value': val}
    except json.JSONDecodeError:
        val = {'_value': value_json}
    pk_cols = schema.get('pk') or []
    key_parts = key.split('|') if key is not None else []
    if pk_cols and len(pk_cols) == len(key_parts):
        for c, v in zip(pk_cols, key_parts):
            row[c] = v
    for k, v in val.items():
        row[k] = v
    row['_key'] = key
    return row

# -------------------- Модели --------------------

class Row(BaseModel):
    data: Dict[str, Any]

class ColumnDefinition(BaseModel):
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

class Query(BaseModel):
    query: str

# -------------------- Эндпоинты --------------------

@app.get("/tables", response_model=List[str], summary="Список таблиц (JE databases)")
def get_tables(user: dict = Depends(get_user_or_guest)):
    return _list_tables()

@app.get("/tables/{table_name}", response_model=List[Dict[str, Any]], summary="Содержимое таблицы (JE)")
def get_table_content(table_name: str, user: dict = Depends(get_user_or_guest)):
    schema = _get_schema(table_name)
    pairs = _dump_table_raw(table_name)
    data = []
    for k, v in pairs:
        if k == '__schema__':
            continue
        data.append(_split_row(schema, k, v))
    return data

@app.get("/tables/{table_name}/schema", summary="Схема таблицы (JE)")
def get_table_schema(table_name: str, user: dict = Depends(get_user_or_guest)):
    schema = _get_schema(table_name)
    # Возвращаем только список колонок для совместимости с фронтендом Lab2/Lab4
    columns = [{"name": k, "type": v} for k, v in schema.get('columns', {}).items()]
    return columns

@app.post("/tables/{table_name}", summary="Добавить строку (JE)")
def add_row(table_name: str, row: Row, user: dict = Depends(get_current_user)):
    if f'"{table_name}"' in LOOKUP_TABLES and user['role'] != 'superuser':
        raise HTTPException(status_code=401, detail="User cannot edit lookup tables")
    schema = _get_schema(table_name)
    key = _compose_key_from_row(schema, row.data)
    value = json.dumps({k: v for k, v in row.data.items() if k not in (schema.get('pk') or [])}, ensure_ascii=False)
    proc = _run_je_bridge(['--env', JE_ENV_PATH, '--op', 'put', '--db', table_name, '--key', key, '--value', value])
    if proc.returncode != 0:
        raise HTTPException(status_code=500, detail=f"JE put error: {proc.stderr}")
    return {"message": "Row added successfully"}

@app.put("/tables/{table_name}/rows/{key}", summary="Обновить строку (JE)")
def update_row(table_name: str, key: str, row: Row, user: dict = Depends(get_current_user)):
    if f'"{table_name}"' in LOOKUP_TABLES and user['role'] != 'superuser':
        raise HTTPException(status_code=401, detail="User cannot edit lookup tables")
    schema = _get_schema(table_name)
    new_key = _compose_key_from_row(schema, row.data)
    value = json.dumps({k: v for k, v in row.data.items() if k not in (schema.get('pk') or [])}, ensure_ascii=False)
    if new_key != key:
        dproc = _run_je_bridge(['--env', JE_ENV_PATH, '--op', 'del', '--db', table_name, '--key', key])
        if dproc.returncode != 0:
            raise HTTPException(status_code=500, detail=f"JE del error: {dproc.stderr}")
    pproc = _run_je_bridge(['--env', JE_ENV_PATH, '--op', 'put', '--db', table_name, '--key', new_key, '--value', value])
    if pproc.returncode != 0:
        raise HTTPException(status_code=500, detail=f"JE put error: {pproc.stderr}")
    return {"message": "Row updated successfully"}

@app.delete("/tables/{table_name}/rows/{key}", summary="Удалить строку (JE)")
def delete_row(table_name: str, key: str, user: dict = Depends(get_current_user)):
    if f'"{table_name}"' in LOOKUP_TABLES and user['role'] != 'superuser':
        raise HTTPException(status_code=401, detail="User cannot edit lookup tables")
    proc = _run_je_bridge(['--env', JE_ENV_PATH, '--op', 'del', '--db', table_name, '--key', key])
    if proc.returncode != 0:
        raise HTTPException(status_code=500, detail=f"JE del error: {proc.stderr}")
    return {"message": "Row deleted successfully"}

def _parse_simple_select(q: str) -> Optional[Dict[str, Any]]:
    # Supports: SELECT * FROM "Table" WHERE col (=|contains|startswith) 'value'
    import re
    print(f"DEBUG: Parsing query: '{q}'")
    
    # Improved regex to handle quoted and unquoted table names safely
    # Group 1: Table in quotes
    # Group 2: Table without quotes
    # Group 3: Column
    # Group 4: Operator
    # Group 5: Value
    # Updated to support <, >, <=, >=, !=, <>
    pattern = r'^\s*select\s*\*\s*from\s*(?:"([^"]+)"|([^\s]+))\s*(?:where\s*([^\s=<>!]+)\s*(<=|>=|<>|!=|<|>|=|contains|startswith)\s*\'?([^\']*)\'?\s*)?$'
    
    m = re.match(pattern, q.strip(), flags=re.IGNORECASE)
    if not m:
        print("DEBUG: Regex did not match!")
        return None
    
    table = m.group(1) if m.group(1) else m.group(2)
    col = m.group(3)
    op = m.group(4)
    val = m.group(5)
    
    print(f"DEBUG: Matched! Table={table}, Col={col}, Op={op}, Val={val}")
    return {"table": table, "filter": (col, op, val) if col else None}

@app.post("/query", summary="Выполнить запрос (ограниченный синтаксис)")
def execute_query(query: Query, user: dict = Depends(get_current_user)):
    try:
        print(f"DEBUG: execute_query called with: {query.query}")
        parsed = _parse_simple_select(query.query)
        if not parsed:
            msg = f"Ошибка синтаксиса. Запрос '{query.query}' не соответствует формату: SELECT * FROM \"Table\" [WHERE col (=|contains|startswith|<|>|<=|>=|!=) 'value']"
            print(f"DEBUG: {msg}")
            raise HTTPException(status_code=422, detail=msg)
        table = parsed['table']
        schema = _get_schema(table)
        pairs = _dump_table_raw(table)
        data = []
        
        # Helper for type conversion
        def to_type(v, type_name):
            tn = str(type_name).upper()
            if v is None: return None
            try:
                if 'INT' in tn: return int(v)
                if 'FLOAT' in tn or 'NUMERIC' in tn or 'REAL' in tn or 'DOUBLE' in tn: return float(v)
            except:
                pass
            return str(v)

        for k, v in pairs:
            if k == '__schema__':
                continue
            row = _split_row(schema, k, v)
            if parsed['filter']:
                col, op, val = parsed['filter']
                
                # Get raw value from row
                row_raw = row.get(col)
                
                # Determine column type from schema
                col_type = schema.get('columns', {}).get(col, 'TEXT')
                
                # Heuristic: If schema type is TEXT (default) but operator implies numeric comparison (<, >, <=, >=),
                # or if the query value looks like a number, try to force numeric comparison.
                is_numeric_op = op in ('<', '>', '<=', '>=')
                
                if col_type == 'TEXT':
                    # Try to see if the value in the query is a number
                    try:
                        float(val)
                        # If it is a number, and we are doing inequality, OR even equality,
                        # let's try to treat the column as numeric if the row value also parses.
                        if is_numeric_op:
                            col_type = 'FLOAT'
                        else:
                            pass
                    except ValueError:
                        pass

                # Convert both to appropriate type for comparison
                rv = to_type(row_raw, col_type)
                tv = to_type(val, col_type)
                
                ok = False
                try:
                    if op == '=':
                        # For strict equality, we might want string comparison if types fail, 
                        # but usually it's safe to compare typed values
                        ok = (str(rv) == str(tv))
                    elif op == 'contains':
                        ok = (str(val) in str(row_raw))
                    elif op == 'startswith':
                        ok = str(row_raw).startswith(str(val))
                    elif op == '>':
                        ok = (rv > tv)
                    elif op == '<':
                        ok = (rv < tv)
                    elif op == '>=':
                        ok = (rv >= tv)
                    elif op == '<=':
                        ok = (rv <= tv)
                    elif op in ('!=', '<>'):
                        ok = (str(rv) != str(tv))
                except Exception as e:
                    # If comparison fails (e.g. str vs int), treat as no match
                    ok = False
                
                if not ok:
                    continue
            data.append(row)
        return data
    except HTTPException:
        raise
    except Exception as e:
        import traceback
        traceback.print_exc()
        print(f"DEBUG: Internal Server Error: {e}")
        raise HTTPException(status_code=500, detail=str(e))

@app.post("/queries/new", summary="Очистить редактор запроса")
def new_query(user: dict = Depends(get_current_user)):
    return {"message": "New query created"}

@app.post("/queries/run_saved", summary="Выполнить сохранённый запрос")
def run_saved_query(query: Query, user: dict = Depends(get_current_user)):
    return execute_query(query, user)

@app.post("/tables/{table_name}/create", summary="Создать таблицу (JE)")
def create_table(table_name: str, request: CreateTableRequest, user: dict = Depends(get_current_user)):
    if f'"{table_name}"' in LOOKUP_TABLES and user['role'] != 'superuser':
        raise HTTPException(status_code=403, detail="Only superusers can create lookup tables")
    proc = _run_je_bridge(['--env', JE_ENV_PATH, '--op', 'create_db', '--db', table_name])
    if proc.returncode != 0:
        raise HTTPException(status_code=500, detail=f"JE create_db error: {proc.stderr}")
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
    pk = []
    for col in cols:
        if col.name.lower() == 'id':
            pk = ['id']
            break
    if not pk and cols:
        pk = [cols[0].name]
    schema = {"pk": pk, "columns": {c.name: c.type for c in cols}}
    _set_schema(table_name, schema)
    return {"message": f"Table {table_name} created successfully."}

@app.delete("/tables/{table_name}/delete", summary="Удалить таблицу (JE)")
def delete_table(table_name: str, user: dict = Depends(get_current_user)):
    if f'"{table_name}"' in LOOKUP_TABLES and user['role'] != 'superuser':
        raise HTTPException(status_code=403, detail="Only superusers can delete lookup tables")
    proc = _run_je_bridge(['--env', JE_ENV_PATH, '--op', 'drop_db', '--db', table_name])
    if proc.returncode != 0:
        raise HTTPException(status_code=500, detail=f"JE drop_db error: {proc.stderr}")
    return {"message": f"Table {table_name} deleted successfully."}

@app.post("/tables/{table_name}/backup", summary="Резервная копия таблицы (Excel)")
def backup_table(table_name: str, user: dict = Depends(get_current_user)):
    data = get_table_content(table_name, user)
    df = pd.DataFrame(data)
    backup_dir = "table_backups"
    os.makedirs(backup_dir, exist_ok=True)
    timestamp = datetime.datetime.now().strftime("%Y%m%d%H%M%S")
    backup_file = os.path.join(backup_dir, f'{table_name}_backup_{timestamp}.xlsx')
    df.to_excel(backup_file, index=False)
    return {"message": f"Table {table_name} backed up successfully to {backup_file}"}

@app.post("/tables/{table_name}/restore", summary="Восстановление таблицы из Excel")
def restore_table(table_name: str, request: TableRestoreRequest, user: dict = Depends(get_current_user)):
    if user['role'] != 'superuser':
        raise HTTPException(status_code=403, detail="Only superusers can restore tables")
    df = pd.read_excel(request.file_path)
    for k, v in _dump_table_raw(table_name):
        if k == '__schema__':
            continue
        _run_je_bridge(['--env', JE_ENV_PATH, '--op', 'del', '--db', table_name, '--key', k])
    schema = _get_schema(table_name)
    pk = schema.get('pk') or []
    for _, r in df.iterrows():
        row = {c: r[c] for c in df.columns}
        key = _compose_key_from_row(schema, row)
        value = json.dumps({k: v for k, v in row.items() if k not in pk}, ensure_ascii=False)
        _run_je_bridge(['--env', JE_ENV_PATH, '--op', 'put', '--db', table_name, '--key', key, '--value', value])
    return {"message": f"Table {table_name} restored successfully from {request.file_path}"}

@app.post("/database/backup", summary="Резервная копия базы (JE env zip)")
def backup_database(user: dict = Depends(get_current_user)):
    backup_dir = "database_backups"
    os.makedirs(backup_dir, exist_ok=True)
    timestamp = datetime.datetime.now().strftime("%Y%m%d%H%M%S")
    zip_path = os.path.join(backup_dir, f"backup_{timestamp}.zip")
    import shutil
    shutil.make_archive(zip_path[:-4], 'zip', JE_ENV_PATH)
    return {"message": f"Backup created successfully at {zip_path}"}

@app.post("/database/restore", summary="Восстановить базу из zip")
def restore_database(request: DatabaseRestoreRequest, user: dict = Depends(get_current_user)):
    if not os.path.exists(request.backup_file):
        raise HTTPException(status_code=400, detail=f"Backup file not found: {request.backup_file}")
    import shutil
    for name in os.listdir(JE_ENV_PATH):
        p = os.path.join(JE_ENV_PATH, name)
        try:
            if os.path.isdir(p):
                shutil.rmtree(p)
            else:
                os.remove(p)
        except Exception:
            pass
    shutil.unpack_archive(request.backup_file, JE_ENV_PATH)
    return {"message": "Database restored successfully."}

@app.get("/queries", summary="Список сохранённых запросов")
def get_saved_queries(user: dict = Depends(get_user_or_guest)):
    if not os.path.exists("saved_queries.txt"):
        return {"queries": []}
    with open("saved_queries.txt", "r", encoding="utf-8") as f:
        queries = [line.strip() for line in f.readlines()]
    return {"queries": queries}

@app.post("/queries", summary="Сохранить запрос")
def save_query(query: QuerySave, user: dict = Depends(get_user_or_guest)):
    with open("saved_queries.txt", "a", encoding="utf-8") as f:
        f.write(query.query + '\n')
    return {"message": "Query saved successfully."}

@app.get("/", include_in_schema=False)
def root_info():
    return {"name": "Lab4 API", "backend": "Berkeley DB (JE)", "note": "Backend работает с Berkeley DB (JE) — без SQL"}

def _bootstrap_db():
    # Create tables on startup if they don't exist
    db_names = set(_list_tables())
    required_tables = {"Сотрудник", "Маршрут", "Станция", "Поезд", "Билет"}
    for table in required_tables:
        if table not in db_names:
            _run_je_bridge(['--env', JE_ENV_PATH, '--op', 'create_db', '--db', table])
            # You might want to define a default schema here as well
            # For example:
            # schema = {"pk": ["id"], "columns": {"id": "INTEGER", "name": "TEXT"}}
            # _set_schema(table, schema)

if __name__ == "__main__":
    _bootstrap_db()
    uvicorn.run(app, host="127.0.0.1", port=API_PORT)