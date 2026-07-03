import sys
from PyQt6.QtWidgets import (
    QApplication, QMainWindow, QWidget, QVBoxLayout, QHBoxLayout,
    QTableWidget, QTableWidgetItem, QPushButton, QTextEdit, QLabel,
    QInputDialog, QMessageBox, QLineEdit, QListWidget
)
from PyQt6.QtCore import Qt
import socket
import numpy as np
import time
import pickle
import asyncio
import threading

# Functions for sending and receiving data over a socket
def send_matrix(matrix, host, port):
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((host, port))
        data_to_send = pickle.dumps(matrix)
        s.sendall(len(data_to_send).to_bytes(8, 'big'))
        s.sendall(data_to_send)
        result = receive_full_data(s)
    return pickle.loads(result)

def receive_full_data(sock):
    data_length = int.from_bytes(sock.recv(8), 'big')
    data = b''
    while len(data) < data_length:
        part = sock.recv(4096)
        if not part:
            raise EOFError("Connection closed prematurely")
        data += part
    return data

def blocking_matrix_multiply(matrix_a, matrix_b, client_hosts):
    parts = np.array_split(matrix_a, len(client_hosts))
    results = []
    for i, part in enumerate(parts):
        result = send_matrix((part, matrix_b), client_hosts[i][0], client_hosts[i][1])
        results.append(result)
    return np.vstack(results)

async def send_and_receive_async(part, matrix_b, host, port):
    reader, writer = await asyncio.open_connection(host, port)

    data_to_send = pickle.dumps((part, matrix_b))
    writer.write(len(data_to_send).to_bytes(8, 'big'))
    writer.write(data_to_send)
    await writer.drain()

    full_data = b''
    data_length = int.from_bytes(await reader.read(8), 'big')

    while len(full_data) < data_length:
        data = await reader.read(4096)
        if not data:
            raise EOFError("Connection closed prematurely")
        full_data += data

    writer.close()
    await writer.wait_closed()

    return pickle.loads(full_data)

async def non_blocking_matrix_multiply(matrix_a, matrix_b, client_hosts):
    parts = np.array_split(matrix_a, len(client_hosts))

    tasks = [send_and_receive_async(part, matrix_b, client_hosts[i][0], client_hosts[i][1]) for i, part in
             enumerate(parts)]

    results = await asyncio.gather(*tasks)
    return np.vstack(results)

class MatrixMultiplierApp(QMainWindow):
    def __init__(self):
        super().__init__()
        self.client_hosts = []
        self.matrix_a = None
        self.matrix_b = None
        self.init_ui()
        self.loop = asyncio.new_event_loop()
        threading.Thread(target=self.run_loop, daemon=True).start()

    def init_ui(self):
        self.setWindowTitle('Система распределенных вычислений')
        self.setGeometry(100, 100, 1200, 600)

        central_widget = QWidget()
        self.setCentralWidget(central_widget)
        main_layout = QHBoxLayout(central_widget)

        # Left panel for nodes management
        left_panel = QWidget()
        left_layout = QVBoxLayout(left_panel)
        nodes_label = QLabel('Управление вычислительными узлами')
        nodes_label.setAlignment(Qt.AlignmentFlag.AlignCenter)
        nodes_label.setStyleSheet('font-weight: bold; font-size: 14px;')
        left_layout.addWidget(nodes_label)

        self.ip_entry = QLineEdit()
        self.ip_entry.setPlaceholderText("IP:порт (например: 127.0.0.1:65434)")
        left_layout.addWidget(self.ip_entry)

        button_layout = QHBoxLayout()
        self.add_ip_button = QPushButton("Добавить узел")
        self.add_ip_button.clicked.connect(self.add_ip)
        button_layout.addWidget(self.add_ip_button)

        self.delete_ip_button = QPushButton("Удалить узел")
        self.delete_ip_button.clicked.connect(self.delete_ip)
        button_layout.addWidget(self.delete_ip_button)
        left_layout.addLayout(button_layout)

        self.ip_listbox = QListWidget()
        left_layout.addWidget(self.ip_listbox)
        left_panel.setFixedWidth(300)
        main_layout.addWidget(left_panel)

        # Right panel for computations and results
        right_panel = QWidget()
        right_layout = QVBoxLayout(right_panel)

        # Computation controls
        control_frame = QWidget()
        control_layout = QHBoxLayout(control_frame)
        
        self.run_blocking_button = QPushButton("Синхронный режим")
        self.run_blocking_button.clicked.connect(self.run_blocking)
        control_layout.addWidget(self.run_blocking_button)

        self.run_async_button = QPushButton("Асинхронный режим")
        self.run_async_button.clicked.connect(self.run_async)
        control_layout.addWidget(self.run_async_button)

        self.compare_button = QPushButton("Сравнить методы")
        self.compare_button.clicked.connect(self.compare_methods)
        control_layout.addWidget(self.compare_button)

        self.clear_button = QPushButton("Очистить результаты")
        self.clear_button.clicked.connect(self.clear_output)
        control_layout.addWidget(self.clear_button)
        right_layout.addWidget(control_frame)

        # Time output
        time_layout = QHBoxLayout()
        time_layout.addWidget(QLabel("Время выполнения:"))
        self.time_output = QLabel("не измерялось")
        time_layout.addWidget(self.time_output)
        right_layout.addLayout(time_layout)

        # Results
        self.output_text = QTextEdit()
        self.output_text.setReadOnly(True)
        right_layout.addWidget(self.output_text)

        # Status bar
        self.status_label = QLabel("Готов к работе")
        right_layout.addWidget(self.status_label)
        
        main_layout.addWidget(right_panel)

    def run_loop(self):
        asyncio.set_event_loop(self.loop)
        self.loop.run_forever()

    def add_ip(self):
        ip_port = self.ip_entry.text()
        if ip_port:
            try:
                if ':' in ip_port:
                    ip, port = ip_port.split(':')
                    port = int(port)
                else:
                    ip = ip_port
                    port = 65434

                self.client_hosts.append((ip, port))
                self.ip_listbox.addItem(f"{ip}:{port}")
                self.ip_entry.clear()
                self.status_label.setText(f"Узел добавлен: {ip}:{port}")
            except ValueError:
                QMessageBox.critical(self, "Ошибка", "Неверный формат адреса")
        else:
            QMessageBox.warning(self, "Предупреждение", "Введите адрес узла")

    def delete_ip(self):
        selected_item = self.ip_listbox.currentItem()
        if selected_item:
            index = self.ip_listbox.row(selected_item)
            removed_node = self.client_hosts.pop(index)
            self.ip_listbox.takeItem(index)
            self.status_label.setText(f"Узел удален: {removed_node[0]}:{removed_node[1]}")
        else:
            QMessageBox.warning(self, "Предупреждение", "Выберите узел для удаления")

    def clear_output(self):
        self.output_text.clear()
        self.time_output.setText("не измерялось")
        self.status_label.setText("Результаты очищены")

    def run_blocking(self):
        if not self.client_hosts:
            QMessageBox.critical(self, "Ошибка", "Не добавлены вычислительные узлы")
            return
        if self.matrix_a is None or self.matrix_b is None:
            self.generate_matrices()

        self.status_label.setText("Выполняется синхронное умножение...")
        start_time = time.time()
        result = blocking_matrix_multiply(self.matrix_a, self.matrix_b, self.client_hosts)
        end_time = time.time() - start_time

        self.output_text.append(f'Результат синхронного умножения:\n{result}\n\n')
        self.time_output.setText(f"{end_time:.2f} секунд")
        self.status_label.setText("Синхронное умножение завершено")

    def run_async(self):
        if not self.client_hosts:
            QMessageBox.critical(self, "Ошибка", "Не добавлены вычислительные узлы")
            return
        if self.matrix_a is None or self.matrix_b is None:
            self.generate_matrices()

        self.status_label.setText("Выполняется асинхронное умножение...")
        start_time = time.time()
        asyncio.run_coroutine_threadsafe(self.async_multiply(start_time), self.loop)

    async def async_multiply(self, start_time):
        result = await non_blocking_matrix_multiply(self.matrix_a, self.matrix_b, self.client_hosts)
        end_time = time.time() - start_time

        self.output_text.append(f'Результат асинхронного умножения:\n{result}\n\n')
        self.time_output.setText(f"{end_time:.2f} секунд")
        self.status_label.setText("Асинхронное умножение завершено")

    def generate_matrices(self):
        size = 1000
        self.matrix_a = np.random.rand(size, size)
        self.matrix_b = np.random.rand(size, size)
        self.status_label.setText("Матрицы сгенерированы (1000x1000)")

    def compare_methods(self):
        if not self.client_hosts:
            QMessageBox.critical(self, "Ошибка", "Не добавлены вычислительные узлы")
            return
        self.generate_matrices()

        self.status_label.setText("Сравнение методов...")
        # Синхронное вычисление
        start_sync = time.time()
        blocking_result = blocking_matrix_multiply(self.matrix_a, self.matrix_b, self.client_hosts)
        sync_time = time.time() - start_sync

        # Асинхронное вычисление
        start_async = time.time()
        future = asyncio.run_coroutine_threadsafe(non_blocking_matrix_multiply(self.matrix_a, self.matrix_b, self.client_hosts), self.loop)
        async_result = future.result()
        async_time = time.time() - start_async

        # Сравнение результатов
        comparison = np.allclose(blocking_result, async_result)

        self.output_text.append(f'Сравнение методов:\n')
        self.output_text.append(f'Результаты: {"совпадают" if comparison else "различаются"}\n')
        self.output_text.append(f'Синхронное время: {sync_time:.2f} секунд\n')
        self.output_text.append(f'Асинхронное время: {async_time:.2f} секунд\n')
        self.output_text.append(f'Выигрыш: {sync_time/async_time:.2f}x\n\n')
        
        self.status_label.setText("Сравнение методов завершено")

if __name__ == "__main__":
    app = QApplication(sys.argv)
    main_win = MatrixMultiplierApp()
    main_win.show()
    sys.exit(app.exec())
