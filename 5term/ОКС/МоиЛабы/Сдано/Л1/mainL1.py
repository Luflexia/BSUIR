import tkinter as tk
from tkinter import ttk, messagebox
import serial
import serial.tools.list_ports
import threading
import time


class SerialApp:
    def __init__(self, root):
        self.root = root
        self.root.title("Ком порты")
        self.root.geometry('600x500')
        self.root.resizable(False, False)

        self.ser_in = None
        self.ser_out = None
        self.sent_portions = 0
        self.receive_thread = None
        self.stop_receive_thread = False

        # Окно ввода
        tk.Label(root, text="Окно ввода").grid(row=0, column=0, padx=10, pady=5, sticky='w')
        self.input_frame = tk.Frame(root)
        self.input_frame.grid(row=1, column=0, padx=10, pady=5, sticky='nsew')
        self.input_text = tk.Text(self.input_frame, width=30, height=10, wrap=tk.WORD)
        self.input_text.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)
        self.input_scroll = tk.Scrollbar(self.input_frame, command=self.input_text.yview)
        self.input_scroll.pack(side=tk.RIGHT, fill=tk.Y)
        self.input_text['yscrollcommand'] = self.input_scroll.set
        self.input_text.bind('<Return>', self.send_data)
        self.input_text.bind('<Tab>', lambda e: 'break')  # Prevent Tab key input

        # Окно вывода
        tk.Label(root, text="Окно вывода").grid(row=0, column=1, padx=10, pady=5, sticky='w')
        self.output_frame = tk.Frame(root)
        self.output_frame.grid(row=1, column=1, padx=10, pady=5, sticky='nsew')
        self.output_text = tk.Text(self.output_frame, width=30, height=10, state='disabled', wrap=tk.WORD)
        self.output_text.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)
        self.output_scroll = tk.Scrollbar(self.output_frame, command=self.output_text.yview)
        self.output_scroll.pack(side=tk.RIGHT, fill=tk.Y)
        self.output_text['yscrollcommand'] = self.output_scroll.set

        # Выбор COM порта
        tk.Label(root, text="Порт передачи").grid(row=2, column=0, padx=10, pady=5, sticky='w')
        self.output_port_combobox = ttk.Combobox(root, state="readonly")
        self.output_port_combobox.grid(row=3, column=0, padx=10, pady=5, sticky="w")
        self.output_port_combobox.bind("<Button-1>", self.update_com_ports)
        self.output_port_combobox.bind("<<ComboboxSelected>>", self.toggle_output_port)

        tk.Label(root, text="Порт приёма").grid(row=2, column=1, padx=10, pady=5, sticky='w')
        self.input_port_combobox = ttk.Combobox(root, state="readonly")
        self.input_port_combobox.grid(row=3, column=1, padx=10, pady=5, sticky="w")
        self.input_port_combobox.bind("<Button-1>", self.update_com_ports)
        self.input_port_combobox.bind("<<ComboboxSelected>>", self.toggle_input_port)

        # Окно состояния
        tk.Label(root, text="Окно состояния").grid(row=5, column=0, columnspan=2, padx=10, pady=5, sticky='w')
        self.status_frame = tk.Frame(root)
        self.status_frame.grid(row=6, column=0, columnspan=2, padx=10, pady=5, sticky='nsew')
        self.status_text = tk.Text(self.status_frame, width=70, height=6, state='disabled', relief=tk.SUNKEN,
                                   wrap=tk.WORD)
        self.status_text.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)
        self.status_scroll = tk.Scrollbar(self.status_frame, command=self.status_text.yview)
        self.status_scroll.pack(side=tk.RIGHT, fill=tk.Y)
        self.status_text['yscrollcommand'] = self.status_scroll.set

        self.update_com_ports()
        self.root.protocol("WM_DELETE_WINDOW", self.on_closing)

        # Настройка сетки
        root.grid_columnconfigure(0, weight=1)
        root.grid_columnconfigure(1, weight=1)
        root.grid_rowconfigure(1, weight=1)
        root.grid_rowconfigure(6, weight=1)

    @staticmethod
    def get_com_ports():
        ports = serial.tools.list_ports.comports()
        available_ports = []
        for port_info in ports:
            port_name = port_info.device
            try:
                temp_serial = serial.Serial(port_name)
                if temp_serial.is_open:
                    temp_serial.close()
                available_ports.append(port_name)
            except serial.SerialException:
                pass
        return available_ports

    def update_com_ports(self, event=None):
        available_ports = self.get_com_ports()

        if self.ser_in and self.ser_in.is_open:
            corresponding_sending_port = f"COM{int(self.ser_in.port[3:]) - 1}"
            if corresponding_sending_port in available_ports:
                available_ports.remove(corresponding_sending_port)

        if self.ser_out and self.ser_out.is_open:
            corresponding_receiving_port = f"COM{int(self.ser_out.port[3:]) + 1}"
            if corresponding_receiving_port in available_ports:
                available_ports.remove(corresponding_receiving_port)

        output_ports = available_ports[:]
        input_ports = available_ports[:]

        if self.ser_out and self.ser_out.is_open:
            output_ports.append("Закрыть порт")
        if self.ser_in and self.ser_in.is_open:
            input_ports.append("Закрыть порт")

        self.output_port_combobox['values'] = output_ports
        self.input_port_combobox['values'] = input_ports

    def toggle_input_port(self, event):
        selected_port = self.input_port_combobox.get()
        if selected_port == "Закрыть порт":
            self.close_input_port()
            self.input_port_combobox.set("")  # Очищаем поле только при закрытии порта
        else:
            if self.ser_in and self.ser_in.is_open:
                # Если уже открыт порт, закрываем его перед открытием нового
                self.close_input_port()
            self.open_input_port(selected_port)
            self.input_port_combobox.set(selected_port)  # Сохраняем выбранный порт в combobox
        self.update_com_ports()

    def toggle_output_port(self, event):
        selected_port = self.output_port_combobox.get()
        if selected_port == "Закрыть порт":
            self.close_output_port()
            self.output_port_combobox.set("")  # Очищаем поле только при закрытии порта
        else:
            if self.ser_out and self.ser_out.is_open:
                # Если уже открыт порт, закрываем его перед открытием нового
                self.close_output_port()
            self.open_output_port(selected_port)
            self.output_port_combobox.set(selected_port)  # Сохраняем выбранный порт в combobox
        self.update_com_ports()

    def open_input_port(self, selected_port):
        try:
            self.ser_in = serial.Serial(selected_port, baudrate=9600, timeout=1)
            self.log_status(f"Порт приёма открыт: {selected_port}")
            self.log_port_params(self.ser_in)
            self.receive_thread = threading.Thread(target=self.receive_data)
            self.receive_thread.start()
        except serial.SerialException as e:
            messagebox.showerror("Ошибка", f"Не удалось открыть порт приёма: {str(e)}")

    def open_output_port(self, selected_port):
        try:
            self.ser_out = serial.Serial(selected_port, baudrate=9600, timeout=1, write_timeout=1)
            self.log_status(f"Порт передачи открыт: {selected_port}")
            self.log_port_params(self.ser_out)
        except serial.SerialException as e:
            messagebox.showerror("Ошибка", f"Не удалось открыть порт передачи: {str(e)}")

    def close_input_port(self):
        self.stop_receive_thread = True
        if self.receive_thread and self.receive_thread.is_alive():
            self.receive_thread.join(timeout=1.0)  # Ждем завершения потока не более 1 секунды
        if self.ser_in:
            try:
                self.ser_in.close()
            except serial.SerialException:
                pass
            self.ser_in = None
        self.input_port_combobox.set("")
        self.log_status("Порт приёма закрыт.")
        self.update_com_ports()
        self.stop_receive_thread = False
        self.receive_thread = None

    def close_output_port(self):
        if self.ser_out and self.ser_out.is_open:
            self.ser_out.close()
            self.log_status("Порт передачи закрыт.")
            self.output_port_combobox.set("")

    def close_serial_ports(self):
        self.stop_receive_thread = True
        if self.receive_thread and self.receive_thread.is_alive():
            self.receive_thread.join()
        if self.ser_out:
            try:
                self.ser_out.close()
            except serial.SerialException:
                pass
        if self.ser_in:
            try:
                self.ser_in.close()
            except serial.SerialException:
                pass
        self.receive_thread = None
        self.stop_receive_thread = False
        self.log_status("Все порты закрыты.")

    def send_data(self, event=None):
        if self.ser_out and self.ser_out.is_open:
            data = self.input_text.get("1.0", tk.END).strip()
            self.sent_portions += 1
            self.log_status(f"Отправлено порций: {self.sent_portions}")
            try:
                if data:
                    self.ser_out.write(data.encode('utf-8') + b'\n')
                    self.input_text.delete("1.0", tk.END)
                else:
                    self.ser_out.write(b'\n')
            except serial.SerialTimeoutException:
                messagebox.showerror("Ошибка", "Превышено время ожидания отправки. Возможно, приемник не готов.")
            except serial.SerialException as e:
                if "WriteTimeout" in str(e):
                    messagebox.showerror("Ошибка", "Превышено время ожидания отправки. Возможно, приемник не готов.")
                else:
                    messagebox.showerror("Ошибка", f"Не удалось отправить данные: {str(e)}")
        else:
            messagebox.showerror("Ошибка", "Порт передачи не открыт.")
        return 'break'

    def receive_data(self):
        data = b""
        while not self.stop_receive_thread:
            if self.ser_in and self.ser_in.is_open:
                try:
                    time.sleep(0.1)
                    if self.ser_in.in_waiting > 0:
                        while self.ser_in.in_waiting > 0:
                            char = self.ser_in.read()
                            data += char
                            if char == b'\n':
                                decoded_data = data.decode('utf-8')
                                self.update_output_text(decoded_data)
                                data = b""
                except serial.SerialException as e:
                    self.log_status(f"Ошибка при чтении данных: {str(e)}")
                    break

    def update_output_text(self, text):
        self.output_text.config(state='normal')
        if text.strip():  # Если текст не пустой (игнорируем пробельные символы)
            self.output_text.insert(tk.END, text)
            if not text.endswith('\n'):
                self.output_text.insert(tk.END, '\n')
        else:  # Если текст пустой, добавляем только один символ новой строки
            self.output_text.insert(tk.END, '\n')
        self.output_text.see(tk.END)
        self.output_text.config(state='disabled')

    def log_status(self, message):
        self.status_text.config(state='normal')
        self.status_text.insert(tk.END, message + '\n')
        self.status_text.see(tk.END)
        self.status_text.config(state='disabled')

    def log_port_params(self, serial_port):
        parity_map = {
            serial.PARITY_NONE: "PARITY_NONE",
            serial.PARITY_EVEN: "PARITY_EVEN",
            serial.PARITY_ODD: "PARITY_ODD",
        }

        stopbits_map = {
            serial.STOPBITS_ONE: "STOPBITS_ONE",
            serial.STOPBITS_ONE_POINT_FIVE: "STOPBITS_ONE_POINT_FIVE",
            serial.STOPBITS_TWO: "STOPBITS_TWO"
        }

        params = (
            f"port - {serial_port.port}, "
            f"baudrate - {serial_port.baudrate}, "
            f"bytesize - {serial_port.bytesize}, "
            f"parity - {parity_map.get(serial_port.parity, 'UNKNOWN')}, "
            f"stopbits - {stopbits_map.get(serial_port.stopbits, 'UNKNOWN')}, "
            f"timeout - {serial_port.timeout}, "
            f"write_timeout - {serial_port.write_timeout}, "
            f"xonxoff - {serial_port.xonxoff}, "
            f"rtscts - {serial_port.rtscts}, "
            f"dsrdtr - {serial_port.dsrdtr}, "
            f"inter_byte_timeout - {serial_port.inter_byte_timeout}, "
            f"exclusive - {serial_port.exclusive}"
        )
        self.log_status(params)

    # def log_port_params(self, serial_port):
    #     params = (f"baudrate - {serial_port.baudrate}, bytesize - {serial_port.bytesize},"
    #               f" stopbits - {serial_port.stopbits}, parity - {serial_port.parity}")
    #     self.log_status(params)

    def on_closing(self):
        self.close_serial_ports()
        self.root.destroy()


if __name__ == "__main__":
    root = tk.Tk()
    app = SerialApp(root)
    root.mainloop()

# pip install pyserial