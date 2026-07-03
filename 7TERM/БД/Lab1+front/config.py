# config.py

# Database connection settings
DB_CONFIG = {
    'host': 'localhost',
    'port': 5432,
    'database': 'Railway',
    'user': 'postgres',
    'password': '1111'
}

# Superuser password
SUPERUSER_PASSWORD = 'super'

# List of lookup tables that can only be edited by a superuser
LOOKUP_TABLES = [
    '"Маршрут"',
    '"Станция"',
    '"Поезд"',
    '"Сотрудник"'
]