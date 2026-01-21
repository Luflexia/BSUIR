import os

# Абсолютный путь к каталогу, в котором находится этот config.py
_lab4_dir = os.path.dirname(os.path.abspath(__file__))

# Абсолютный путь к родительскому каталогу 'Labs'
_labs_dir = os.path.dirname(_lab4_dir)

# Путь к окружению BerkeleyDB (каталог с файлами базы данных)
JE_ENV_PATH = os.path.join(_lab4_dir, "berkeley_data")

# Путь к библиотеке BerkeleyDB JE JAR, используется для запуска Java-утилит
JE_JAR_PATH = os.path.join(_labs_dir, "Lab3", "lib", "je-18.3.12.jar")

# Порт API
API_PORT = 8000

# Пароль суперпользователя (для привилегированных операций)
SUPERUSER_PASSWORD = 'super'

# Таблицы-справочники (редактируются только суперпользователем)
LOOKUP_TABLES = [
    '"Маршрут"',
    '"Станция"',
    '"Поезд"',
    '"Сотрудник"'
]