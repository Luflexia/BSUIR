import sys
import os
import datetime
import psycopg2
from PyQt6.QtWidgets import (QApplication, QMainWindow, QWidget, QVBoxLayout,
                             QHBoxLayout, QPushButton, QTableWidget, QTableWidgetItem,
                             QMessageBox, QInputDialog, QFileDialog, QTextEdit,
                             QDialog, QLabel, QLineEdit, QComboBox, QFormLayout)
from PyQt6.QtCore import Qt
import pandas as pd

class DatabaseManager:
    def __init__(self):
        self.conn = None
        self.cursor = None
        self.saved_queries = {}
        self.load_saved_queries()

    def load_saved_queries(self):
        try:
            with open('saved_queries.txt', 'r', encoding='utf-8') as f:
                for line in f:
                    if ':' in line:
                        name, query = line.strip().split(':', 1)
                        self.saved_queries[name] = query
        except FileNotFoundError:
            pass
            
    def import_queries_from_file(self, file_path):
        try:
            with open(file_path, 'r', encoding='utf-8') as f:
                for line in f:
                    if ':' in line:
                        name, query = line.strip().split(':', 1)
                        self.save_query(name, query)
            return True
        except Exception as e:
            QMessageBox.critical(None, 'Ошибка импорта запросов', str(e))
            return False

    def save_query(self, name, query):
        self.saved_queries[name] = query
        try:
            with open('saved_queries.txt', 'a', encoding='utf-8') as f:
                f.write(f'{name}:{query}\n')
            return True
        except Exception as e:
            QMessageBox.critical(None, 'Ошибка сохранения запроса', str(e))
            return False

    def execute_query(self, query):
        try:
            # Экранируем имена таблиц, если они содержат только цифры
            words = query.strip().split()
            for i, word in enumerate(words):
                # Удаляем возможные знаки пунктуации
                clean_word = word.rstrip(';').rstrip(')')
                if clean_word.isdigit():
                    words[i] = words[i].replace(clean_word, f'"{clean_word}"')
            query = ' '.join(words)
            
            # Проверяем, является ли запрос INSERT и есть ли поле ID
            if query.upper().startswith('INSERT') and 'id' in [col.lower() for col in self.cursor.description]:
                # Получаем информацию о типе поля ID
                table_name = query.split()[2]  # Получаем имя таблицы из запроса
                self.cursor.execute(f"""
                    SELECT column_name, data_type, column_default 
                    FROM information_schema.columns 
                    WHERE table_name = '{table_name}' AND column_name = 'id'
                """)
                id_info = self.cursor.fetchone()
                
                # Если поле ID имеет тип SERIAL, не передаем значение для него
                if id_info and 'nextval' in str(id_info[2]):
                    # Модифицируем запрос, чтобы не передавать значение для ID
                    query = query.replace('id,', '').replace('id)', ')').replace('VALUES (NULL,', 'VALUES (')
            
            self.cursor.execute(query)
            self.conn.commit()
            if self.cursor.description:
                columns = [desc[0] for desc in self.cursor.description]
                data = self.cursor.fetchall()
                return True, columns, data
            return True, None, None
        except Exception as e:
            self.conn.rollback()
            return False, None, str(e)

    def create_new_table(self, table_name, columns):
        try:
            # Проверяем, что имя таблицы не содержит пробелов и кириллицы
            # Экранируем имя таблицы
            escaped_table_name = f'"{table_name}"'
            
            # Добавляем поле ID с автоинкрементом, если его нет в колонках
            has_id = any(col['name'].lower() == 'id' for col in columns)
            if not has_id:
                columns.insert(0, {'name': 'id', 'type': 'SERIAL PRIMARY KEY'})
            
            # Экранируем имена столбцов
            column_definitions = [f'"{col["name"]}" {col["type"]}' for col in columns]
            create_query = f"CREATE TABLE {escaped_table_name} ({', '.join(column_definitions)})"
            self.cursor.execute(create_query)
            
            # Добавляем пустую строку в новую таблицу (исключая поле ID)
            column_names = [f'"{col["name"]}"' for col in columns if col['name'].lower() != 'id']
            if column_names:  # Проверяем, есть ли столбцы для вставки
                placeholders = ', '.join(['NULL'] * len(column_names))
                insert_query = f"INSERT INTO {escaped_table_name} ({', '.join(column_names)}) VALUES ({placeholders})"
                self.cursor.execute(insert_query)
            
            self.conn.commit()
            return True
        except Exception as e:
            self.conn.rollback()
            QMessageBox.critical(None, 'Ошибка создания таблицы', str(e))
            return False

    def delete_table(self, table_name):
        try:
            # Экранируем имя таблицы, если оно содержит только цифры
            if table_name.isdigit():
                table_name = f'"{table_name}"'
            self.cursor.execute(f"DROP TABLE {table_name}")
            self.conn.commit()
            return True
        except Exception as e:
            self.conn.rollback()
            QMessageBox.critical(None, 'Ошибка удаления таблицы', str(e))
            return False

    def export_to_excel(self, table_name):
        try:
            self.cursor.execute(f"SELECT * FROM {table_name}")
            data = self.cursor.fetchall()
            columns = [desc[0] for desc in self.cursor.description]
            df = pd.DataFrame(data, columns=columns)
            
            file_path, _ = QFileDialog.getSaveFileName(
                None,
                'Сохранить как Excel',
                f'{table_name}.xlsx',
                'Excel Files (*.xlsx)'
            )
            
            if file_path:
                df.to_excel(file_path, index=False, engine='openpyxl')
                return True
            return False
        except ImportError:
            QMessageBox.critical(None, 'Ошибка', 'Модуль openpyxl не установлен. Установите его командой: pip install openpyxl')
            return False
        except Exception as e:
            QMessageBox.critical(None, 'Ошибка экспорта', str(e))
            return False

    def get_table_structure(self, table_name):
        try:
            # Экранируем имя таблицы, если оно содержит только цифры
            if table_name.isdigit():
                table_name = f'"{table_name}"'
            self.cursor.execute(f"""
                SELECT column_name, data_type
                FROM information_schema.columns
                WHERE table_name = {table_name if table_name.startswith('"') else f"'{table_name}'"}
                ORDER BY ordinal_position
            """)
            return self.cursor.fetchall()
        except Exception as e:
            QMessageBox.critical(None, 'Ошибка получения структуры таблицы', str(e))
            return []

    def connect(self):
        try:
            self.conn = psycopg2.connect(
                host='localhost',
                port=5432,
                database='Railway',
                user='postgres',
                password='1111'
            )
            self.cursor = self.conn.cursor()
            return True
        except Exception as e:
            QMessageBox.critical(None, 'Ошибка подключения', str(e))
            return False

    def disconnect(self):
        if self.cursor:
            self.cursor.close()
        if self.conn:
            self.conn.close()

    def get_tables(self):
        self.cursor.execute("""
            SELECT table_name FROM information_schema.tables
            WHERE table_schema = 'public'
        """)
        return [table[0] for table in self.cursor.fetchall()]

    def backup_table(self, table_name):
        try:
            # Получаем структуру таблицы
            self.cursor.execute(f"SELECT * FROM {table_name} LIMIT 0")
            columns = [desc[0] for desc in self.cursor.description]
            
            # Получаем данные
            self.cursor.execute(f"SELECT * FROM {table_name}")
            data = self.cursor.fetchall()
            
            # Сохраняем в DataFrame и экспортируем в CSV
            df = pd.DataFrame(data, columns=columns)
            backup_path = f'backup_{table_name}.csv'
            df.to_csv(backup_path, index=False)
            return True, backup_path
        except Exception as e:
            QMessageBox.critical(None, 'Ошибка резервного копирования', str(e))
            return False, None
            
    def restore_table(self, backup_path):
        try:
            # Проверяем существование файла резервной копии
            if not os.path.exists(backup_path):
                raise FileNotFoundError(f"Файл резервной копии {backup_path} не найден")
                
            # Загружаем данные из CSV
            df = pd.read_csv(backup_path)
            
            # Получаем имя таблицы из имени файла резервной копии
            table_name = os.path.splitext(os.path.basename(backup_path))[0]
            
            # Экранируем имя таблицы
            escaped_table_name = f'"{table_name}"'
            
            # Проверяем существование таблицы
            self.cursor.execute(f"""
                SELECT EXISTS (
                    SELECT FROM information_schema.tables 
                    WHERE table_schema = 'public' AND table_name = '{table_name}'
                );
            """)
            table_exists = self.cursor.fetchone()[0]
            
            if not table_exists:
                # Если таблица не существует, создаем ее
                columns = df.columns.tolist()
                
                # Проверяем наличие поля ID и его тип
                has_id = 'id' in [col.lower() for col in columns]
                column_defs = []
                
                for col in columns:
                    if col.lower() == 'id':
                        # Если это поле ID, устанавливаем тип SERIAL PRIMARY KEY
                        column_defs.append(f'"{col}" SERIAL PRIMARY KEY')
                    else:
                        # Для остальных полей используем TEXT
                        column_defs.append(f'"{col}" TEXT')
                
                create_query = f"CREATE TABLE {escaped_table_name} ({', '.join(column_defs)});"
                self.cursor.execute(create_query)
            else:
                # Если таблица существует, удаляем все данные из нее
                self.cursor.execute(f"DELETE FROM {escaped_table_name}")
            
            # Вставляем данные из резервной копии
            if not df.empty:
                columns = df.columns.tolist()
                
                # Если есть поле ID, исключаем его из запроса INSERT
                if 'id' in [col.lower() for col in columns]:
                    columns = [col for col in columns if col.lower() != 'id']
                
                if columns:  # Проверяем, остались ли столбцы для вставки
                    placeholders = ', '.join(['%s'] * len(columns))
                    query = f"INSERT INTO {escaped_table_name} ({', '.join(columns)}) VALUES ({placeholders})"
                    
                    for _, row in df.iterrows():
                        # Преобразуем строку DataFrame в список значений, исключая ID если он есть
                        values = [None if pd.isna(val) else val 
                                 for col, val in zip(df.columns, row.tolist()) 
                                 if col.lower() != 'id']
                        self.cursor.execute(query, values)
            
            self.conn.commit()
            return True
        except Exception as e:
            self.conn.rollback()
            QMessageBox.critical(None, 'Ошибка восстановления таблицы', str(e))
            return False
            
    def backup_database(self):
        try:
            # Получаем список всех таблиц
            tables = self.get_tables()
            
            # Создаем директорию для резервных копий, если она не существует
            backup_dir = 'database_backup'
            if not os.path.exists(backup_dir):
                os.makedirs(backup_dir)
                
            # Текущая дата и время для имени директории
            timestamp = datetime.datetime.now().strftime('%Y%m%d_%H%M%S')
            backup_path = os.path.join(backup_dir, f'backup_{timestamp}')
            os.makedirs(backup_path)
            
            # Создаем резервную копию каждой таблицы
            for table in tables:
                self.cursor.execute(f"SELECT * FROM {table}")
                data = self.cursor.fetchall()
                columns = [desc[0] for desc in self.cursor.description]
                
                df = pd.DataFrame(data, columns=columns)
                table_backup_path = os.path.join(backup_path, f'{table}.csv')
                df.to_csv(table_backup_path, index=False)
            
            # Создаем файл с метаданными
            with open(os.path.join(backup_path, 'metadata.txt'), 'w') as f:
                f.write(f"Backup created: {datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')}\n")
                f.write(f"Tables: {', '.join(tables)}\n")
            
            return True, backup_path
        except Exception as e:
            QMessageBox.critical(None, 'Ошибка резервного копирования базы данных', str(e))
            return False, None
            
    def restore_database(self, backup_dir):
        try:
            # Проверяем существование директории с резервной копией
            if not os.path.exists(backup_dir):
                raise FileNotFoundError(f"Директория резервной копии {backup_dir} не найдена")
                
            # Получаем список файлов CSV в директории
            csv_files = [f for f in os.listdir(backup_dir) if f.endswith('.csv')]
            
            if not csv_files:
                raise FileNotFoundError("В директории резервной копии не найдены файлы CSV")
                
            # Получаем список всех текущих таблиц
            current_tables = self.get_tables()
            
            # Удаляем все внешние ключи
            for table in current_tables:
                # Получаем список внешних ключей для таблицы
                self.cursor.execute(f"""
                    SELECT conname, pg_get_constraintdef(oid) 
                    FROM pg_constraint 
                    WHERE conrelid = '{table}'::regclass AND contype = 'f'
                """)
                constraints = self.cursor.fetchall()
                
                # Удаляем каждый внешний ключ
                for conname, condef in constraints:
                    self.cursor.execute(f"ALTER TABLE {table} DROP CONSTRAINT {conname}")
            
            # Удаляем все таблицы
            for table in current_tables:
                if not self.delete_table(table):
                    return False
                    
            # Восстанавливаем каждую таблицу из резервной копии
            for csv_file in csv_files:
                table_name = os.path.splitext(csv_file)[0]
                backup_path = os.path.join(backup_dir, csv_file)
                
                # Восстанавливаем таблицу
                if not self.restore_table(backup_path):
                    return False
            
            # TODO: Восстановить внешние ключи из файла метаданных
            # Пока просто возвращаем успех
            return True
        except Exception as e:
            self.conn.rollback()
            QMessageBox.critical(None, 'Ошибка восстановления базы данных', str(e))
            return False
            
    def export_query_results_to_excel(self, query):
        try:
            # Выполняем запрос
            self.cursor.execute(query)
            data = self.cursor.fetchall()
            columns = [desc[0] for desc in self.cursor.description]
            
            # Создаем DataFrame
            df = pd.DataFrame(data, columns=columns)
            
            # Запрашиваем путь для сохранения файла
            file_path, _ = QFileDialog.getSaveFileName(
                None,
                'Сохранить результаты запроса как Excel',
                'query_results.xlsx',
                'Excel Files (*.xlsx)'
            )
            
            if file_path:
                df.to_excel(file_path, index=False)
                return True
            return False
        except Exception as e:
            QMessageBox.critical(None, 'Ошибка экспорта результатов запроса', str(e))
            return False

class QueryDialog(QDialog):
    def __init__(self, parent=None):
        super().__init__(parent)
        self.setWindowTitle('Новый запрос')
        self.setModal(True)
        
        layout = QVBoxLayout(self)
        
        layout.addWidget(QLabel('Название запроса:'))
        self.name_edit = QLineEdit()
        layout.addWidget(self.name_edit)
        
        layout.addWidget(QLabel('SQL запрос:'))
        self.query_edit = QTextEdit()
        layout.addWidget(self.query_edit)
        
        btn_layout = QHBoxLayout()
        save_btn = QPushButton('Сохранить')
        save_btn.clicked.connect(self.accept)
        import_btn = QPushButton('Добавить из файла')
        import_btn.clicked.connect(self.import_queries)
        cancel_btn = QPushButton('Отмена')
        cancel_btn.clicked.connect(self.reject)
        btn_layout.addWidget(save_btn)
        btn_layout.addWidget(import_btn)
        btn_layout.addWidget(cancel_btn)
        layout.addLayout(btn_layout)
        
    def import_queries(self):
        file_path, _ = QFileDialog.getOpenFileName(
            self,
            'Выберите файл с запросами',
            '',
            'Text Files (*.txt)'
        )
        if file_path:
            if self.parent().db.import_queries_from_file(file_path):
                QMessageBox.information(self, 'Успех', 'Запросы успешно импортированы')
                self.accept()

class AddRowDialog(QDialog):
    def __init__(self, parent=None, structure=None):
        super().__init__(parent)
        self.setWindowTitle('Добавление новой строки')
        self.setModal(True)
        self.structure = structure
        self.inputs = {}
        
        layout = QVBoxLayout(self)
        form_layout = QFormLayout()
        
        # Создаем поля ввода для каждого столбца таблицы
        for column_name, data_type in structure:
            # Если это поле id, показываем метку 'auto' вместо поля ввода
            if column_name.lower() == 'id':
                label = QLabel('auto (заполнится автоматически)')
                label.setStyleSheet('color: gray; font-style: italic;')
                form_layout.addRow(f"{column_name} ({data_type}):", label)
                # Добавляем пустое поле в словарь, чтобы сохранить порядок полей
                self.inputs[column_name] = None
            elif data_type.upper() in ['BOOLEAN']:
                input_field = QComboBox()
                input_field.addItems(['TRUE', 'FALSE'])
                form_layout.addRow(f"{column_name} ({data_type}):", input_field)
                self.inputs[column_name] = input_field
            else:
                input_field = QLineEdit()
                form_layout.addRow(f"{column_name} ({data_type}):", input_field)
                self.inputs[column_name] = input_field
        
        layout.addLayout(form_layout)
        
        btn_layout = QHBoxLayout()
        save_btn = QPushButton('Сохранить')
        save_btn.clicked.connect(self.accept)
        cancel_btn = QPushButton('Отмена')
        cancel_btn.clicked.connect(self.reject)
        btn_layout.addWidget(save_btn)
        btn_layout.addWidget(cancel_btn)
        layout.addLayout(btn_layout)
    
    def get_values(self):
        values = []
        columns = []
        for column_name, data_type in self.structure:
            # Пропускаем поле id, чтобы оно заполнялось автоматически
            if column_name.lower() == 'id':
                continue
                
            input_field = self.inputs[column_name]
            
            if isinstance(input_field, QComboBox):
                value = input_field.currentText()
            elif input_field is not None:
                value = input_field.text()
                # Преобразуем пустые строки в None
                if value == '':
                    value = None
            else:
                value = None
                
            values.append(value)
            columns.append(column_name)
        
        return values, columns

class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()
        self.db = DatabaseManager()
        self.init_ui()
        
        # Создаем строку состояния
        self.statusBar().showMessage('Готово к работе')
        
        if not self.db.connect():
            sys.exit(1)
            
        # Отображаем список таблиц при запуске
        self.update_tables_list()

    def init_ui(self):
        self.setWindowTitle('Управление базой данных Railway')
        self.setGeometry(100, 100, 1000, 700)

        # Создаем центральный виджет и главный layout
        central_widget = QWidget()
        self.setCentralWidget(central_widget)
        main_layout = QHBoxLayout(central_widget)
        
        # Создаем левую панель для списка таблиц
        left_panel = QWidget()
        left_layout = QVBoxLayout(left_panel)
        
        # Добавляем заголовок для списка таблиц
        tables_label = QLabel('Список таблиц:')
        tables_label.setAlignment(Qt.AlignmentFlag.AlignCenter)
        tables_label.setStyleSheet('font-weight: bold; font-size: 14px;')
        left_layout.addWidget(tables_label)
        
        # Создаем список таблиц
        self.tables_list = QTableWidget()
        self.tables_list.setColumnCount(1)
        self.tables_list.setHorizontalHeaderLabels(['Таблицы'])
        self.tables_list.horizontalHeader().setStretchLastSection(True)
        self.tables_list.setSelectionBehavior(QTableWidget.SelectionBehavior.SelectRows)
        self.tables_list.setSelectionMode(QTableWidget.SelectionMode.SingleSelection)
        self.tables_list.setEditTriggers(QTableWidget.EditTrigger.NoEditTriggers)
        self.tables_list.itemClicked.connect(self.load_table_data)
        left_layout.addWidget(self.tables_list)
        
        # Кнопки для работы с таблицами
        btn_create_table = QPushButton('Создать таблицу')
        btn_delete_table = QPushButton('Удалить таблицу')
        btn_refresh_tables = QPushButton('Обновить список')
        
        # Добавляем кнопки на левую панель
        left_layout.addWidget(btn_create_table)
        left_layout.addWidget(btn_delete_table)
        left_layout.addWidget(btn_refresh_tables)
        
        # Добавляем кнопки для резервного копирования и восстановления всей БД
        btn_backup_db = QPushButton('Резервная копия БД')
        btn_restore_db = QPushButton('Восстановить БД')
        left_layout.addWidget(btn_backup_db)
        left_layout.addWidget(btn_restore_db)
        
        # Устанавливаем фиксированную ширину для левой панели
        left_panel.setFixedWidth(200)
        main_layout.addWidget(left_panel)
        
        # Создаем правую панель для отображения данных и запросов
        right_panel = QWidget()
        right_layout = QVBoxLayout(right_panel)
        
        # Создаем панель кнопок для работы с данными таблицы
        table_button_layout = QHBoxLayout()
        
        # Кнопки для работы с данными таблицы
        btn_add_row = QPushButton('Добавить строку')
        btn_delete_row = QPushButton('Удалить строку')
        btn_backup_table = QPushButton('Резервная копия таблицы')
        btn_restore_table = QPushButton('Восстановить таблицу')
        btn_export_table = QPushButton('Экспорт таблицы в Excel')

        # Добавляем кнопки на панель работы с таблицей
        table_button_layout.addWidget(btn_add_row)
        table_button_layout.addWidget(btn_delete_row)
        table_button_layout.addWidget(btn_backup_table)
        table_button_layout.addWidget(btn_restore_table)
        table_button_layout.addWidget(btn_export_table)

        # Создаем таблицу для отображения данных
        self.table_widget = QTableWidget()
        self.table_widget.setSelectionBehavior(QTableWidget.SelectionBehavior.SelectRows)
        self.table_widget.setSelectionMode(QTableWidget.SelectionMode.SingleSelection)
        self.table_widget.setEditTriggers(QTableWidget.EditTrigger.NoEditTriggers)
        # Подключаем сигнал двойного клика по ячейке
        self.table_widget.cellDoubleClicked.connect(self.cell_double_clicked)

        # Добавляем поле для ввода SQL запросов
        self.query_edit = QTextEdit()
        self.query_edit.setPlaceholderText('Введите SQL запрос здесь...')
        self.query_edit.setMaximumHeight(100)
        
        # Создаем панель кнопок для работы с SQL запросами
        sql_button_layout = QHBoxLayout()
        
        # Кнопки для работы с SQL запросами
        btn_execute_query = QPushButton('Выполнить запрос')
        btn_export_query = QPushButton('Экспорт результата в Excel')
        btn_new_query = QPushButton('Новый запрос')
        btn_run_saved_query = QPushButton('Сохраненные запросы')

        # Добавляем кнопки на панель работы с SQL
        sql_button_layout.addWidget(btn_execute_query)
        sql_button_layout.addWidget(btn_export_query)
        sql_button_layout.addWidget(btn_new_query)
        sql_button_layout.addWidget(btn_run_saved_query)
        
        # Добавляем все элементы в правый layout
        right_layout.addLayout(table_button_layout)
        right_layout.addWidget(self.table_widget)
        right_layout.addWidget(self.query_edit)
        right_layout.addLayout(sql_button_layout)
        
        # Добавляем правую панель в главный layout
        main_layout.addWidget(right_panel)

        # Подключаем сигналы к слотам
        btn_create_table.clicked.connect(self.create_table)
        btn_delete_table.clicked.connect(self.delete_table)
        btn_refresh_tables.clicked.connect(self.update_tables_list)
        btn_add_row.clicked.connect(self.add_row)
        btn_delete_row.clicked.connect(self.delete_row)
        btn_backup_table.clicked.connect(self.backup_table)
        btn_restore_table.clicked.connect(self.restore_table)
        btn_export_table.clicked.connect(self.export_to_excel)
        btn_backup_db.clicked.connect(self.backup_database)
        btn_restore_db.clicked.connect(self.restore_database)
        btn_new_query.clicked.connect(self.new_query)
        btn_run_saved_query.clicked.connect(self.run_saved_query)
        btn_execute_query.clicked.connect(self.execute_query)
        btn_export_query.clicked.connect(self.export_query_results)

    def create_table(self):
        table_name, ok = QInputDialog.getText(
            self, 'Создание таблицы',
            'Введите имя таблицы:'
        )
        
        if not ok or not table_name:
            return

        columns = []
        while True:
            column_name, ok1 = QInputDialog.getText(
                self, 'Добавление столбца',
                'Введите имя столбца (или оставьте пустым для завершения):'
            )
            
            if not ok1 or not column_name:
                break

            column_type, ok2 = QInputDialog.getItem(
                self, 'Тип данных',
                'Выберите тип данных:',
                ['INTEGER', 'TEXT', 'FLOAT', 'DATE', 'BOOLEAN'],
                0, False
            )
            
            if not ok2:
                break

            columns.append({'name': column_name, 'type': column_type})

        if columns and self.db.create_new_table(table_name, columns):
            QMessageBox.information(
                self, 'Успех',
                f'Таблица {table_name} успешно создана'
            )
            # Обновляем список таблиц
            self.update_tables_list()

    def delete_table(self):
        # Проверяем, выбрана ли таблица в списке
        selected_items = self.tables_list.selectedItems()
        if not selected_items:
            QMessageBox.warning(self, 'Предупреждение', 'Выберите таблицу для удаления')
            return
            
        table_name = selected_items[0].text()
        if table_name:
            reply = QMessageBox.question(
                self, 'Подтверждение',
                f'Вы уверены, что хотите удалить таблицу {table_name}?',
                QMessageBox.StandardButton.Yes | QMessageBox.StandardButton.No
            )
            
            if reply == QMessageBox.StandardButton.Yes:
                if self.db.delete_table(table_name):
                    QMessageBox.information(
                        self, 'Успех',
                        f'Таблица {table_name} успешно удалена'
                    )
                    # Обновляем список таблиц
                    self.update_tables_list()
                    # Очищаем таблицу данных
                    self.table_widget.setRowCount(0)
                    self.table_widget.setColumnCount(0)
                    # Удаляем ссылку на текущую таблицу
                    if hasattr(self, 'current_table'):
                        delattr(self, 'current_table')

    def backup_table(self):
        # Проверяем, выбрана ли таблица в списке
        selected_items = self.tables_list.selectedItems()
        if not selected_items:
            QMessageBox.warning(self, 'Предупреждение', 'Выберите таблицу для резервного копирования')
            return
            
        table_name = selected_items[0].text()
        if table_name:
            success, backup_path = self.db.backup_table(table_name)
            if success:
                QMessageBox.information(
                    self, 'Успех',
                    f'Резервная копия таблицы {table_name} создана успешно: {backup_path}'
                )
                
    def restore_table(self):
        # Проверяем, выбрана ли таблица в списке
        selected_items = self.tables_list.selectedItems()
        if not selected_items:
            QMessageBox.warning(self, 'Предупреждение', 'Выберите таблицу для восстановления')
            return
            
        table_name = selected_items[0].text()
        if table_name:
            # Запрашиваем файл резервной копии
            backup_path, _ = QFileDialog.getOpenFileName(
                self,
                'Выберите файл резервной копии',
                '',
                'CSV Files (*.csv)'
            )
            
            if backup_path:
                reply = QMessageBox.question(
                    self, 'Подтверждение',
                    f'Вы уверены, что хотите восстановить таблицу {table_name} из файла {backup_path}?\n'
                    'Это заменит все существующие данные в таблице.',
                    QMessageBox.StandardButton.Yes | QMessageBox.StandardButton.No
                )
                
                if reply == QMessageBox.StandardButton.Yes:
                    if self.db.restore_table(backup_path):
                        QMessageBox.information(
                            self, 'Успех',
                            f'Таблица {table_name} успешно восстановлена'
                        )
                        # Обновляем отображение таблицы
                        self.load_table_data(selected_items[0])
                        
    def backup_database(self):
        success, backup_path = self.db.backup_database()
        if success:
            QMessageBox.information(
                self, 'Успех',
                f'Резервная копия базы данных создана успешно: {backup_path}'
            )
            
    def restore_database(self):
        # Запрашиваем директорию с резервной копией
        backup_dir = QFileDialog.getExistingDirectory(
            self,
            'Выберите директорию с резервной копией базы данных'
        )
        
        if backup_dir:
            reply = QMessageBox.question(
                self, 'Подтверждение',
                'Вы уверены, что хотите восстановить базу данных из выбранной резервной копии?\n'
                'Это заменит все существующие данные в базе.',
                QMessageBox.StandardButton.Yes | QMessageBox.StandardButton.No
            )
            
            if reply == QMessageBox.StandardButton.Yes:
                if self.db.restore_database(backup_dir):
                    QMessageBox.information(
                        self, 'Успех',
                        'База данных успешно восстановлена'
                    )
                    # Обновляем список таблиц
                    self.update_tables_list()

    def export_to_excel(self):
        # Проверяем, выбрана ли таблица в списке
        selected_items = self.tables_list.selectedItems()
        if not selected_items:
            QMessageBox.warning(self, 'Предупреждение', 'Выберите таблицу для экспорта в Excel')
            return
            
        table_name = selected_items[0].text()
        if table_name:
            if self.db.export_to_excel(table_name):
                QMessageBox.information(
                    self, 'Успех',
                    f'Таблица {table_name} успешно экспортирована в Excel'
                )
                
    def export_query_results(self):
        # Проверяем, есть ли данные в таблице
        if self.table_widget.rowCount() == 0:
            QMessageBox.warning(self, 'Предупреждение', 'Нет данных для экспорта')
            return

        # Получаем заголовки столбцов
        columns = []
        for j in range(self.table_widget.columnCount()):
            columns.append(self.table_widget.horizontalHeaderItem(j).text())

        # Получаем данные из таблицы
        data = []
        for i in range(self.table_widget.rowCount()):
            row = []
            for j in range(self.table_widget.columnCount()):
                item = self.table_widget.item(i, j)
                row.append(item.text() if item else '')
            data.append(row)

        # Создаем DataFrame
        df = pd.DataFrame(data, columns=columns)

        # Запрашиваем путь для сохранения файла
        file_path, _ = QFileDialog.getSaveFileName(
            self,
            'Сохранить результаты как Excel',
            'query_results.xlsx',
            'Excel Files (*.xlsx)'
        )

        if file_path:
            try:
                df.to_excel(file_path, index=False)
                QMessageBox.information(self, 'Успех', 'Данные успешно экспортированы в Excel')
            except Exception as e:
                QMessageBox.critical(self, 'Ошибка', f'Ошибка экспорта данных: {str(e)}')
            
    def add_row(self):
        # Проверяем, загружена ли таблица
        if not hasattr(self, 'current_table'):
            QMessageBox.warning(self, 'Предупреждение', 'Сначала выберите таблицу')
            return
            
        # Получаем структуру таблицы
        structure = self.db.get_table_structure(self.current_table)
        if not structure:
            QMessageBox.warning(self, 'Ошибка', f'Не удалось получить структуру таблицы {self.current_table}')
            return
            
        # Проверяем, является ли поле ID автоинкрементным
        is_auto_id = False
        for column in structure:
            if column[0].lower() == 'id':
                self.db.cursor.execute(f"""
                    SELECT column_default 
                    FROM information_schema.columns 
                    WHERE table_name = '{self.current_table}' AND column_name = 'id'
                """)
                id_info = self.db.cursor.fetchone()
                if id_info and 'nextval' in str(id_info[0]):
                    is_auto_id = True
                break
            
        # Создаем диалог для ввода данных
        dialog = AddRowDialog(self, structure)
        if dialog.exec():
            # Получаем введенные данные и названия столбцов
            values, columns = dialog.get_values()
            
            if not columns:
                QMessageBox.warning(self, 'Предупреждение', 'Нет данных для вставки')
                return
                
            # Если ID автоинкрементный, исключаем его из запроса
            if is_auto_id:
                # Сохраняем оригинальные columns и values
                orig_columns = columns
                orig_values = values
                
                # Фильтруем только не-ID колонки
                columns = [col for col in columns if col.lower() != 'id']
                values = [val for col, val in zip(orig_columns, orig_values) if col.lower() != 'id']
                
                # Если остались другие колонки кроме ID, продолжаем вставку
                if columns:
                    pass
                # Если остался только ID, но есть другие значения - все равно вставляем
                elif len(orig_columns) > 1 or (len(orig_columns) == 1 and len(orig_values) > 0):
                    columns = [col for col in orig_columns if col.lower() != 'id']
                    values = [val for col, val in zip(orig_columns, orig_values) if col.lower() != 'id']
                else:
                    QMessageBox.warning(self, 'Предупреждение', 'Нет данных для вставки')
                    return
            
            # Экранируем имя таблицы, если оно содержит только цифры
            table_name = f'"{self.current_table}"' if self.current_table.isdigit() else self.current_table
            
            # Создаем placeholders только для отфильтрованных столбцов
            placeholders = ', '.join(['%s'] * len(columns))
            query = f"INSERT INTO {table_name} ({', '.join(columns)}) VALUES ({placeholders})"
            
            # Выполняем запрос
            try:
                self.db.cursor.execute(query, values)
                self.db.conn.commit()
                QMessageBox.information(self, 'Успех', 'Строка успешно добавлена')
                
                # Обновляем отображение таблицы
                selected_items = self.tables_list.selectedItems()
                if selected_items:
                    self.load_table_data(selected_items[0])
            except Exception as e:
                self.db.conn.rollback()
                QMessageBox.critical(self, 'Ошибка', f'Ошибка добавления строки: {str(e)}')

    def new_query(self):
        dialog = QueryDialog(self)
        if dialog.exec():
            name = dialog.name_edit.text().strip()
            query = dialog.query_edit.toPlainText().strip()
            
            if name and query:
                if self.db.save_query(name, query):
                    QMessageBox.information(self, 'Успех', 'Запрос успешно сохранен')

    def execute_query(self):
        query = self.query_edit.toPlainText().strip()
        if not query:
            QMessageBox.warning(self, 'Предупреждение', 'Введите SQL запрос')
            return
            
        success, columns, data = self.db.execute_query(query)
        
        if success:
            if columns and data:
                self.table_widget.setRowCount(len(data))
                self.table_widget.setColumnCount(len(columns))
                self.table_widget.setHorizontalHeaderLabels(columns)

                for i, row in enumerate(data):
                    for j, value in enumerate(row):
                        item = QTableWidgetItem(str(value))
                        self.table_widget.setItem(i, j, item)
                
                # Настраиваем автоматическую подгонку ширины столбцов под содержимое
                self.table_widget.resizeColumnsToContents()
                
                self.statusBar().showMessage('Запрос успешно выполнен', 3000)
            else:
                self.statusBar().showMessage('Запрос успешно выполнен (без данных для отображения)', 3000)
        else:
            QMessageBox.critical(self, 'Ошибка', f'Ошибка выполнения запроса: {data}')
            
    def load_table_data(self, item):
        # Получаем имя выбранной таблицы
        table_name = item.text()
        
        # Экранируем имя таблицы, если оно содержит только цифры
        if table_name.isdigit():
            table_name = f'"{table_name}"'
            
        # Выполняем запрос для получения данных таблицы
        success, columns, data = self.db.execute_query(f"SELECT * FROM {table_name}")
        
        if success and columns and data:
            self.table_widget.setRowCount(len(data))
            self.table_widget.setColumnCount(len(columns))
            self.table_widget.setHorizontalHeaderLabels(columns)

            for i, row in enumerate(data):
                for j, value in enumerate(row):
                    item = QTableWidgetItem(str(value))
                    self.table_widget.setItem(i, j, item)
            
            # Настраиваем автоматическую подгонку ширины столбцов под содержимое
            self.table_widget.resizeColumnsToContents()
            
            # Сохраняем имя текущей таблицы
            self.current_table = table_name
            self.statusBar().showMessage(f'Загружена таблица: {table_name}', 3000)
        else:
            QMessageBox.warning(self, 'Ошибка', f'Не удалось загрузить данные таблицы {table_name}')
            
    def cell_double_clicked(self, row, column):
        # Проверяем, загружена ли таблица
        if not hasattr(self, 'current_table'):
            return
            
        # Получаем структуру таблицы
        structure = self.db.get_table_structure(self.current_table)
        if not structure:
            return
            
        # Получаем имя столбца и его тип
        column_name = self.table_widget.horizontalHeaderItem(column).text()
        column_type = None
        
        for col_name, col_type in structure:
            if col_name == column_name:
                column_type = col_type
                break
                
        # Не позволяем редактировать столбец id
        if column_name.lower() == 'id':
            QMessageBox.information(self, 'Информация', 'Столбец ID не может быть отредактирован')
            return
            
        # Получаем текущее значение ячейки
        current_value = self.table_widget.item(row, column).text()
        
        # Получаем значение первичного ключа (предполагаем, что это первый столбец)
        id_column = self.table_widget.horizontalHeaderItem(0).text()
        id_value = self.table_widget.item(row, 0).text()
        
        # Запрашиваем новое значение
        if column_type and column_type.upper() in ['BOOLEAN']:
            new_value, ok = QInputDialog.getItem(
                self, 'Редактирование ячейки',
                f'Введите новое значение для {column_name}:',
                ['TRUE', 'FALSE'], 0 if current_value.upper() == 'TRUE' else 1, False
            )
        else:
            new_value, ok = QInputDialog.getText(
                self, 'Редактирование ячейки',
                f'Введите новое значение для {column_name}:',
                text=current_value
            )
        
        if ok and new_value != current_value:
            # Формируем SQL запрос для обновления
            query = f"UPDATE {self.current_table} SET {column_name}='{new_value}' WHERE {id_column}='{id_value}'"
            
            # Выполняем запрос
            success, _, error = self.db.execute_query(query)
            
            if success:
                # Обновляем ячейку в таблице
                self.table_widget.item(row, column).setText(new_value)
                self.statusBar().showMessage('Ячейка успешно обновлена', 3000)
            else:
                QMessageBox.critical(self, 'Ошибка', f'Ошибка обновления ячейки: {error}')
            
    def delete_row(self):
        # Проверяем, выбрана ли строка
        selected_rows = self.table_widget.selectionModel().selectedRows()
        if not selected_rows:
            QMessageBox.warning(self, 'Предупреждение', 'Выберите строку для удаления')
            return
            
        # Проверяем, загружена ли таблица
        if not hasattr(self, 'current_table'):
            QMessageBox.warning(self, 'Предупреждение', 'Сначала выберите таблицу')
            return
            
        # Получаем индекс выбранной строки
        row_index = selected_rows[0].row()
        
        # Получаем структуру таблицы для определения первичного ключа
        structure = self.db.get_table_structure(self.current_table)
        
        # Предполагаем, что первый столбец - это первичный ключ или уникальный идентификатор
        id_column = self.table_widget.horizontalHeaderItem(0).text()
        id_value = self.table_widget.item(row_index, 0).text()
        
        # Запрашиваем подтверждение
        reply = QMessageBox.question(
            self, 'Подтверждение',
            f'Вы уверены, что хотите удалить строку с {id_column}={id_value}?',
            QMessageBox.StandardButton.Yes | QMessageBox.StandardButton.No
        )
        
        if reply == QMessageBox.StandardButton.Yes:
            # Выполняем запрос на удаление
            success, _, error = self.db.execute_query(
                f"DELETE FROM {self.current_table} WHERE {id_column}='{id_value}'"
            )
            
            if success:
                # Обновляем отображение таблицы
                self.table_widget.removeRow(row_index)
                self.statusBar().showMessage('Строка успешно удалена', 3000)
            else:
                QMessageBox.critical(self, 'Ошибка', f'Ошибка удаления строки: {error}')

            
    def run_saved_query(self):
        if not self.db.saved_queries:
            QMessageBox.warning(self, 'Предупреждение', 'Нет сохраненных запросов')
            return

        # Создаем диалоговое окно
        dialog = QDialog(self)
        dialog.setWindowTitle('Сохраненные запросы')
        layout = QVBoxLayout(dialog)

        # Создаем выпадающий список запросов
        combo = QComboBox()
        combo.addItems(list(self.db.saved_queries.keys()))
        layout.addWidget(combo)

        # Создаем кнопки
        btn_layout = QHBoxLayout()
        select_btn = QPushButton('Выбрать')
        delete_btn = QPushButton('Удалить все запросы')
        cancel_btn = QPushButton('Отмена')

        btn_layout.addWidget(select_btn)
        btn_layout.addWidget(delete_btn)
        btn_layout.addWidget(cancel_btn)
        layout.addLayout(btn_layout)

        # Подключаем обработчик для кнопки удаления
        delete_btn.clicked.connect(lambda: self.delete_saved_queries(dialog))

        # Подключаем обработчики событий
        select_btn.clicked.connect(dialog.accept)
        cancel_btn.clicked.connect(dialog.reject)

        # Инициализируем переменную query_name
        query_name = None
        
        if dialog.exec():
            query_name = combo.currentText()

            if query_name:
                query = self.db.saved_queries[query_name]
                success, columns, data = self.db.execute_query(query)
                
                if success:
                    if columns and data:
                        self.table_widget.setRowCount(len(data))
                        self.table_widget.setColumnCount(len(columns))
                        self.table_widget.setHorizontalHeaderLabels(columns)

                        for i, row in enumerate(data):
                            for j, value in enumerate(row):
                                item = QTableWidgetItem(str(value))
                                self.table_widget.setItem(i, j, item)
                        
                        # Настраиваем автоматическую подгонку ширины столбцов под содержимое
                        self.table_widget.resizeColumnsToContents()
                        
                        QMessageBox.information(self, 'Успех', 'Запрос успешно выполнен')
                    else:
                        QMessageBox.information(self, 'Успех', 'Запрос успешно выполнен (без данных для отображения)')
                else:
                    QMessageBox.critical(self, 'Ошибка', f'Ошибка выполнения запроса: {data}')

    def delete_saved_queries(self, dialog):
        reply = QMessageBox.question(
            self, 'Подтверждение',
            'Вы уверены, что хотите удалить все сохраненные запросы?',
            QMessageBox.StandardButton.Yes | QMessageBox.StandardButton.No
        )
        
        if reply == QMessageBox.StandardButton.Yes:
            try:
                # Очищаем словарь сохраненных запросов
                self.db.saved_queries.clear()
                # Очищаем файл с сохраненными запросами
                with open('saved_queries.txt', 'w', encoding='utf-8') as f:
                    f.write('')
                QMessageBox.information(self, 'Успех', 'Все сохраненные запросы удалены')
                dialog.reject()  # Закрываем диалог после удаления
            except Exception as e:
                QMessageBox.critical(self, 'Ошибка', f'Ошибка удаления запросов: {str(e)}')

    def closeEvent(self, event):
        self.db.disconnect()
        event.accept()
        
    def update_tables_list(self):
        tables = self.db.get_tables()
        self.tables_list.setRowCount(len(tables))
        
        for i, table_name in enumerate(tables):
            item = QTableWidgetItem(table_name)
            self.tables_list.setItem(i, 0, item)
            
        if not tables:
            self.statusBar().showMessage('Нет доступных таблиц в базе данных', 3000)

if __name__ == '__main__':
    app = QApplication(sys.argv)
    window = MainWindow()
    window.show()
    sys.exit(app.exec())