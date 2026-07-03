import sys
from PyQt6.QtWidgets import (
    QApplication, QMainWindow, QWidget, QVBoxLayout, QHBoxLayout,
    QTableWidget, QTableWidgetItem, QPushButton, QTextEdit, QLabel,
    QInputDialog, QMessageBox, QFileDialog, QDialog, QComboBox,
    QLineEdit
)
from PyQt6.QtCore import Qt
import requests
import json
import pandas as pd

class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()
        self.api_url = "http://127.0.0.1:8000"
        self.is_superuser = False
        self.init_ui()
        self.update_tables_list()

    def init_ui(self):
        self.setWindowTitle('Управление базой данных Railway')
        self.setGeometry(100, 100, 1200, 800)

        central_widget = QWidget()
        self.setCentralWidget(central_widget)
        main_layout = QHBoxLayout(central_widget)

        # Left panel for table list
        left_panel = QWidget()
        left_layout = QVBoxLayout(left_panel)
        tables_label = QLabel('Список таблиц:')
        tables_label.setAlignment(Qt.AlignmentFlag.AlignCenter)
        tables_label.setStyleSheet('font-weight: bold; font-size: 14px;')
        left_layout.addWidget(tables_label)

        self.tables_list = QTableWidget()
        self.tables_list.setColumnCount(1)
        self.tables_list.setHorizontalHeaderLabels(['Таблицы'])
        self.tables_list.horizontalHeader().setStretchLastSection(True)
        self.tables_list.itemClicked.connect(self.load_table_data)
        left_layout.addWidget(self.tables_list)

        btn_refresh_tables = QPushButton('Обновить список')
        btn_refresh_tables.clicked.connect(self.update_tables_list)
        left_layout.addWidget(btn_refresh_tables)
        
        # Superuser section
        self.superuser_button = QPushButton('Войти как Superuser')
        self.superuser_button.setCheckable(True)
        self.superuser_button.clicked.connect(self.toggle_superuser_mode)
        left_layout.addWidget(self.superuser_button)


        left_panel.setFixedWidth(250)
        main_layout.addWidget(left_panel)

        # Right panel for data and queries
        right_panel = QWidget()
        right_layout = QVBoxLayout(right_panel)

        # Filter
        filter_layout = QHBoxLayout()
        self.filter_input = QLineEdit()
        self.filter_input.setPlaceholderText("Фильтр...")
        self.filter_input.textChanged.connect(self.filter_table)
        filter_layout.addWidget(self.filter_input)
        right_layout.addLayout(filter_layout)


        table_button_layout = QHBoxLayout()
        btn_add_row = QPushButton('Добавить строку')
        btn_delete_row = QPushButton('Удалить строку')
        btn_export_table = QPushButton('Экспорт таблицы в Excel')

        table_button_layout.addWidget(btn_add_row)
        table_button_layout.addWidget(btn_delete_row)
        table_button_layout.addWidget(btn_export_table)
        
        btn_add_row.clicked.connect(self.add_row)
        btn_delete_row.clicked.connect(self.delete_row)
        btn_export_table.clicked.connect(self.export_to_excel)

        self.table_widget = QTableWidget()
        self.table_widget.cellDoubleClicked.connect(self.edit_cell)
        
        self.query_edit = QTextEdit()
        self.query_edit.setPlaceholderText('Введите SQL запрос здесь...')
        self.query_edit.setMaximumHeight(100)

        sql_button_layout = QHBoxLayout()
        # Кнопки для работы с таблицами
        btn_create_table = QPushButton('Создать таблицу')
        btn_delete_table = QPushButton('Удалить таблицу')
        btn_backup_table = QPushButton('Резервная копия таблицы')
        btn_restore_table = QPushButton('Восстановить таблицу')

        # Добавляем кнопки на левую панель
        left_layout.addWidget(btn_create_table)
        left_layout.addWidget(btn_delete_table)
        left_layout.addWidget(btn_backup_table)
        left_layout.addWidget(btn_restore_table)
        # Привязка обработчиков к кнопкам таблиц
        btn_create_table.clicked.connect(self.create_table)
        btn_delete_table.clicked.connect(self.delete_table)
        btn_backup_table.clicked.connect(self.backup_table)
        btn_restore_table.clicked.connect(self.restore_table)

        # Добавляем кнопки для резервного копирования и восстановления всей БД
        btn_backup_db = QPushButton('Резервная копия БД')
        btn_restore_db = QPushButton('Восстановить БД')
        left_layout.addWidget(btn_backup_db)
        left_layout.addWidget(btn_restore_db)
        # Привязка обработчиков к кнопкам БД
        btn_backup_db.clicked.connect(self.backup_database)
        btn_restore_db.clicked.connect(self.restore_database)
        btn_export_query = QPushButton('Экспорт результата в Excel')

        # Кнопки для работы с SQL запросами
        btn_execute_query = QPushButton('Выполнить запрос')
        btn_new_query = QPushButton('Новый запрос')
        btn_run_saved_query = QPushButton('Сохраненные запросы')

        # Добавляем кнопки на панель работы с SQL
        sql_button_layout.addWidget(btn_execute_query)
        sql_button_layout.addWidget(btn_new_query)
        sql_button_layout.addWidget(btn_run_saved_query)
        sql_button_layout.addWidget(btn_export_query)

        btn_execute_query.clicked.connect(self.execute_query)
        btn_new_query.clicked.connect(self.new_query)
        btn_run_saved_query.clicked.connect(self.run_saved_query)
        btn_export_query.clicked.connect(self.export_query_results)

        right_layout.addLayout(table_button_layout)
        right_layout.addWidget(self.table_widget)
        right_layout.addWidget(self.query_edit)
        right_layout.addLayout(sql_button_layout)

        main_layout.addWidget(right_panel)

    def get_auth(self):
        if self.is_superuser:
            return ('superuser', 'super')
        return ('user', 'user')

    def toggle_superuser_mode(self):
        if self.superuser_button.isChecked():
            password, ok = QInputDialog.getText(self, 'Superuser', 'Введите пароль:', QLineEdit.EchoMode.Password)
            if not ok:
                # Пользователь закрыл диалог — просто выходим без сообщений
                self.superuser_button.setChecked(False)
                return
            if password == 'super':
                self.is_superuser = True
                self.superuser_button.setText('Выйти из режима Superuser')
                QMessageBox.information(self, 'Superuser', 'Вход в режим суперпользователя выполнен.')
            else:
                self.superuser_button.setChecked(False)
                QMessageBox.warning(self, 'Superuser', 'Неверный пароль.')
        else:
            self.is_superuser = False
            self.superuser_button.setText('Войти как Superuser')

    def update_tables_list(self):
        try:
            response = requests.get(f"{self.api_url}/tables", auth=self.get_auth())
            response.raise_for_status()
            tables = response.json()
            self.tables_list.setRowCount(len(tables))
            for i, table_name in enumerate(tables):
                self.tables_list.setItem(i, 0, QTableWidgetItem(table_name))
        except requests.exceptions.RequestException as e:
            QMessageBox.critical(self, 'Ошибка', f'Не удалось получить список таблиц: {e}')

    def load_table_data(self, item):
        table_name = item.text()
        try:
            response = requests.get(f"{self.api_url}/tables/{table_name}", auth=self.get_auth())
            response.raise_for_status()
            data = response.json()
            if not data:
                # Если таблица пустая, загрузим схему, чтобы показать заголовки
                schema = []
                try:
                    sresp = requests.get(f"{self.api_url}/tables/{table_name}/schema", auth=self.get_auth())
                    sresp.raise_for_status()
                    schema = sresp.json()
                except requests.exceptions.RequestException:
                    pass
                self.table_widget.setRowCount(0)
                if schema:
                    self.table_widget.setColumnCount(len(schema))
                    self.table_widget.setHorizontalHeaderLabels([c['name'] for c in schema])
                else:
                    self.table_widget.setColumnCount(0)
                return

            self.current_table = table_name
            self.table_widget.setRowCount(len(data))
            self.table_widget.setColumnCount(len(data[0]))
            self.table_widget.setHorizontalHeaderLabels(data[0].keys())

            for i, row in enumerate(data):
                for j, value in enumerate(row.values()):
                    self.table_widget.setItem(i, j, QTableWidgetItem(str(value)))
            self.table_widget.resizeColumnsToContents()
        except requests.exceptions.RequestException as e:
            QMessageBox.critical(self, 'Ошибка', f'Не удалось загрузить данные таблицы: {e}')

    def add_row(self):
        if not hasattr(self, 'current_table'):
            QMessageBox.warning(self, 'Предупреждение', 'Сначала выберите таблицу')
            return

        dialog = QDialog(self)
        dialog.setWindowTitle("Добавить строку")
        layout = QVBoxLayout(dialog)
        
        # Get column names from table header
        column_names = []
        for j in range(self.table_widget.columnCount()):
            header_item = self.table_widget.horizontalHeaderItem(j)
            if header_item is not None:
                column_names.append(header_item.text())

        # Create input fields for each column except 'id'
        line_edits = {}
        for col_name in column_names:
            if col_name.lower() == 'id':
                continue
            layout.addWidget(QLabel(col_name))
            line_edit = QLineEdit()
            line_edits[col_name] = line_edit
            layout.addWidget(line_edit)

        button_box = QHBoxLayout()
        ok_button = QPushButton("OK")
        cancel_button = QPushButton("Отмена")
        button_box.addWidget(ok_button)
        button_box.addWidget(cancel_button)
        layout.addLayout(button_box)

        ok_button.clicked.connect(dialog.accept)
        cancel_button.clicked.connect(dialog.reject)

        if dialog.exec():
            new_row_data = {col: le.text() for col, le in line_edits.items()}
            try:
                response = requests.post(
                    f"{self.api_url}/tables/{self.current_table}",
                    auth=self.get_auth(),
                    json={"data": new_row_data}
                )
                response.raise_for_status()
                self.load_table_data(self.tables_list.currentItem())
            except requests.exceptions.RequestException as e:
                QMessageBox.critical(self, 'Ошибка', f'Не удалось добавить строку: {e.response.json().get("detail") if e.response else e}')


    def delete_row(self):
        selected_rows = self.table_widget.selectionModel().selectedRows()
        if not selected_rows:
            QMessageBox.warning(self, 'Предупреждение', 'Выберите строку для удаления')
            return
        
        row_index = selected_rows[0].row()
        row_id = self.table_widget.item(row_index, 0).text() # Assuming id is the first column

        reply = QMessageBox.question(
            self, 'Подтверждение',
            f'Вы уверены, что хотите удалить строку с ID={row_id}?',
            QMessageBox.StandardButton.Yes | QMessageBox.StandardButton.No
        )

        if reply == QMessageBox.StandardButton.Yes:
            try:
                response = requests.delete(
                    f"{self.api_url}/tables/{self.current_table}/rows/{row_id}",
                    auth=self.get_auth()
                )
                response.raise_for_status()
                self.load_table_data(self.tables_list.currentItem())
            except requests.exceptions.RequestException as e:
                QMessageBox.critical(self, 'Ошибка', f'Не удалось удалить строку: {e.response.json().get("detail") if e.response else e}')

    def edit_cell(self, row, column):
        if not hasattr(self, 'current_table'):
            return

        header = self.table_widget.horizontalHeaderItem(column).text()
        if header.lower() == 'id':
            QMessageBox.information(self, 'Информация', 'Столбец ID не может быть отредактирован')
            return
            
        row_id = self.table_widget.item(row, 0).text()
        current_value = self.table_widget.item(row, column).text()
        
        new_value, ok = QInputDialog.getText(self, 'Редактирование', f'Новое значение для {header}:', text=current_value)

        if ok and new_value != current_value:
            update_data = {header: new_value}
            try:
                response = requests.put(
                    f"{self.api_url}/tables/{self.current_table}/rows/{row_id}",
                    auth=self.get_auth(),
                    json={"data": update_data}
                )
                response.raise_for_status()
                self.table_widget.item(row, column).setText(new_value)
            except requests.exceptions.RequestException as e:
                if e.response is not None and e.response.status_code == 403:
                    QMessageBox.warning(self, 'Доступ запрещен', 
                                        'У вас нет прав для выполнения этого действия. '
                                        'Пожалуйста, войдите в режим Superuser, чтобы редактировать данные.')
                else:
                    QMessageBox.critical(self, 'Ошибка', f'Не удалось обновить ячейку: {e.response.json().get("detail") if e.response else e}')


    def execute_query(self):
        query = self.query_edit.toPlainText().strip()
        if not query:
            QMessageBox.warning(self, 'Предупреждение', 'Введите SQL запрос')
            return
        
        try:
            response = requests.post(f"{self.api_url}/query", auth=self.get_auth(), json={"query": query})
            response.raise_for_status()
            result = response.json()

            if "columns" in result and "data" in result:
                self.table_widget.setRowCount(len(result['data']))
                self.table_widget.setColumnCount(len(result['columns']))
                self.table_widget.setHorizontalHeaderLabels(result['columns'])

                for i, row_data in enumerate(result['data']):
                    for j, (key, value) in enumerate(row_data.items()):
                        self.table_widget.setItem(i, j, QTableWidgetItem(str(value)))
                self.table_widget.resizeColumnsToContents()
            else:
                QMessageBox.information(self, 'Успех', result.get("message", "Запрос выполнен"))

        except requests.exceptions.RequestException as e:
            QMessageBox.critical(self, 'Ошибка', f'Ошибка выполнения запроса: {e.response.json().get("detail") if e.response else e}')

    def export_to_excel(self):
        if not hasattr(self, 'current_table'):
            QMessageBox.warning(self, 'Предупреждение', 'Сначала выберите таблицу')
            return
            
        file_path, _ = QFileDialog.getSaveFileName(self, 'Сохранить как Excel', f'{self.current_table}.xlsx', 'Excel Files (*.xlsx)')
        if not file_path:
            return

        try:
            response = requests.get(f"{self.api_url}/tables/{self.current_table}", auth=self.get_auth())
            response.raise_for_status()
            data = response.json()
            df = pd.DataFrame(data)
            df.to_excel(file_path, index=False)
            QMessageBox.information(self, 'Успех', 'Таблица успешно экспортирована в Excel')
        except Exception as e:
            QMessageBox.critical(self, 'Ошибка', f'Ошибка экспорта: {e}')
            
    def export_query_results(self):
        if self.table_widget.rowCount() == 0:
            QMessageBox.warning(self, 'Предупреждение', 'Нет данных для экспорта')
            return

        columns = []
        for j in range(self.table_widget.columnCount()):
            columns.append(self.table_widget.horizontalHeaderItem(j).text())

        data = []
        for i in range(self.table_widget.rowCount()):
            row = []
            for j in range(self.table_widget.columnCount()):
                item = self.table_widget.item(i, j)
                row.append(item.text() if item else '')
            data.append(row)

        df = pd.DataFrame(data, columns=columns)

        file_path, _ = QFileDialog.getSaveFileName(self, 'Сохранить результаты как Excel', 'query_results.xlsx', 'Excel Files (*.xlsx)')
        if file_path:
            try:
                df.to_excel(file_path, index=False)
                QMessageBox.information(self, 'Успех', 'Данные успешно экспортированы в Excel')
            except Exception as e:
                QMessageBox.critical(self, 'Ошибка', f'Ошибка экспорта данных: {str(e)}')

    def backup_database(self):
        try:
            response = requests.post(f"{self.api_url}/database/backup", auth=self.get_auth())
            response.raise_for_status()
            QMessageBox.information(self, 'Успех', response.json().get("message"))
        except requests.exceptions.RequestException as e:
            QMessageBox.critical(self, 'Ошибка', f'Ошибка резервного копирования: {e.response.json().get("detail") if e.response else e}')

    def restore_database(self):
        file_path, _ = QFileDialog.getOpenFileName(self, 'Выбрать файл для восстановления', '', 'SQL Files (*.sql);;All Files (*)')
        if not file_path:
            return

        try:
            response = requests.post(f"{self.api_url}/database/restore", auth=self.get_auth(), json={"backup_file": file_path})
            response.raise_for_status()
            QMessageBox.information(self, 'Успех', response.json().get("message"))
            self.update_tables_list()
        except requests.exceptions.RequestException as e:
            QMessageBox.critical(self, 'Ошибка', f'Ошибка восстановления: {e.response.json().get("detail") if e.response else e}')

    def create_table(self):
        table_name, ok = QInputDialog.getText(self, 'Создать таблицу', 'Введите имя таблицы:')
        if not (ok and table_name):
            return

        columns_str, ok = QInputDialog.getText(self, 'Столбцы', 'Введите столбцы (name1 type1, name2 type2, ...):')
        if not (ok and columns_str):
            return

        columns = []
        for col in columns_str.split(','):
            parts = col.strip().split()
            if len(parts) == 2:
                columns.append({"name": parts[0], "type": parts[1]})

        try:
            response = requests.post(
                f"{self.api_url}/tables/{table_name}/create",
                auth=self.get_auth(),
                json={"columns": columns}
            )
            response.raise_for_status()
            QMessageBox.information(self, 'Успех', f'Таблица {table_name} создана')
            self.update_tables_list()
        except requests.exceptions.RequestException as e:
            QMessageBox.critical(self, 'Ошибка', f'Не удалось создать таблицу: {e.response.json().get("detail") if e.response else e}')

    def delete_table(self):
        if not hasattr(self, 'current_table'):
            QMessageBox.warning(self, 'Предупреждение', 'Сначала выберите таблицу')
            return

        reply = QMessageBox.question(
            self, 'Подтверждение',
            f'Вы уверены, что хотите удалить таблицу {self.current_table}?',
            QMessageBox.StandardButton.Yes | QMessageBox.StandardButton.No
        )

        if reply == QMessageBox.StandardButton.Yes:
            try:
                response = requests.delete(f"{self.api_url}/tables/{self.current_table}/delete", auth=self.get_auth())
                response.raise_for_status()
                QMessageBox.information(self, 'Успех', f'Таблица {self.current_table} удалена')
                self.update_tables_list()
            except requests.exceptions.RequestException as e:
                QMessageBox.critical(self, 'Ошибка', f'Не удалось удалить таблицу: {e.response.json().get("detail") if e.response else e}')

    def backup_table(self):
        if not hasattr(self, 'current_table'):
            QMessageBox.warning(self, 'Предупреждение', 'Сначала выберите таблицу')
            return

        try:
            response = requests.post(f"{self.api_url}/tables/{self.current_table}/backup", auth=self.get_auth())
            response.raise_for_status()
            QMessageBox.information(self, 'Успех', response.json().get("message"))
        except requests.exceptions.RequestException as e:
            QMessageBox.critical(self, 'Ошибка', f'Ошибка резервного копирования таблицы: {e.response.json().get("detail") if e.response else e}')

    def restore_table(self):
        if not hasattr(self, 'current_table'):
            QMessageBox.warning(self, 'Предупреждение', 'Сначала выберите таблицу')
            return

        file_path, _ = QFileDialog.getOpenFileName(self, 'Выбрать файл для восстановления', '', 'Excel Files (*.xlsx)')
        if not file_path:
            return

        try:
            response = requests.post(
                f"{self.api_url}/tables/{self.current_table}/restore",
                auth=self.get_auth(),
                json={"file_path": file_path}
            )
            response.raise_for_status()
            QMessageBox.information(self, 'Успех', response.json().get("message"))
            self.load_table_data(self.tables_list.currentItem())
        except requests.exceptions.RequestException as e:
            QMessageBox.critical(self, 'Ошибка', f'Ошибка восстановления таблицы: {e.response.json().get("detail") if e.response else e}')

    def new_query(self):
        self.query_edit.clear()

    def run_saved_query(self):
        try:
            response = requests.get(f"{self.api_url}/queries", auth=self.get_auth())
            response.raise_for_status()
            data = response.json()
            items = data.get("queries", [])
            if not items:
                QMessageBox.information(self, 'Сохраненные запросы', 'Список пуст')
                return
            names = []
            mapping = {}
            for item in items:
                if ':' in item:
                    name, q = item.split(':', 1)
                    names.append(name)
                    mapping[name] = q
                else:
                    names.append(item)
                    mapping[item] = item
            sel, ok = QInputDialog.getItem(self, "Сохраненные запросы", "Выберите запрос:", names, 0, False)
            if ok and sel:
                self.query_edit.setText(mapping[sel])
                self.execute_query()

        except requests.exceptions.RequestException as e:
            QMessageBox.critical(self, 'Ошибка', f'Не удалось загрузить сохраненные запросы: {e.response.json().get("detail") if e.response else e}')

    def filter_table(self, text):
        for i in range(self.table_widget.rowCount()):
            match = False
            for j in range(self.table_widget.columnCount()):
                item = self.table_widget.item(i, j)
                if item and text.lower() in item.text().lower():
                    match = True
                    break
            self.table_widget.setRowHidden(i, not match)


if __name__ == '__main__':
    app = QApplication(sys.argv)
    window = MainWindow()
    window.show()
    sys.exit(app.exec())