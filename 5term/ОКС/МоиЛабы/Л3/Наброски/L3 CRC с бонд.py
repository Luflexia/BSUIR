import tkinter as tk
from tkinter import ttk, messagebox
import serial
import serial.tools.list_ports
import threading
import random

# Выбираем непечатный символ в качестве терминатора
# Например, можно использовать символ EOT (End of Transmission, ASCII 4)
TERMINATOR = b'\x04'

INITIAL_VALUE = 0x00
POLYNOM = 0x29

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

    def byte_stuffing(self, data):
        stuffed_data = data.replace(b'@u', b'\x1B\x01')
        return stuffed_data

    def unstuffing(self, data):
        unstuffed_data = data.replace(b'\x1B\x01', b'@u')
        return unstuffed_data

    def create_frame(self, data):
        flag = b'@u'  # '@' и 'u' (ASCII 117, что равно 'a' + 21 - 1)
        destination_address = b'\x00'
        source_address = bytes([int(self.ser_out.port[3:])])

        if len(data) < 21:
            # Если данные короче 21 байта, добавляем терминатор и заполняем нулями
            padded_data = data + TERMINATOR + b'0' * (20 - len(data))
        else:
            # Если данные ровно 21 байт, оставляем как есть
            padded_data = data

        # Вычисляем CRC8 для padded_data
        crc = self.compute_crc8(padded_data)

        # Применяем коррекцию циклическим сдвигом
        corrected_data = self.cyclic_shift_correction(padded_data, crc)

        # Объединяем все части кадра, кроме флага
        frame_without_flag = destination_address + source_address + corrected_data + bytes([crc])

        return flag, frame_without_flag

    def introduce_error(self, data, probability=0.4):
        if random.random() <= probability:
            byte_array = bytearray(data)
            byte_index = random.randint(0, len(byte_array) - 1)
            bit_index = random.randint(0, 7)
            byte_array[byte_index] ^= (1 << bit_index)
            data = bytes(byte_array)
        return data

    def compute_crc8(self, data: bytes) -> int:
        remainder = INITIAL_VALUE
        for byte in data:
            remainder ^= byte
            for _ in range(8):
                remainder <<= 1
                if (remainder & 0x100) != 0:
                    remainder ^= (POLYNOM | 0x100)
        return remainder & 0xFF

    def cyclic_shift_correction(self, message: bytes, crc: int) -> bytes:
        weight = bin(crc).count('1')
        shift_message = bytearray(message)
        if weight <= 1:
            shift_message[-1] ^= crc
            return bytes(shift_message)
        count_shifts = 0
        size_bits = len(shift_message) * 8
        for _ in range(size_bits):
            count_shifts += 1
            shift_message = self.left_cycle_shift(shift_message)
            crc = self.compute_crc8(shift_message)
            if bin(crc).count('1') <= 1:
                shift_message[-1] ^= crc
                for _ in range(count_shifts):
                    shift_message = self.right_cycle_shift(shift_message)
                return bytes(shift_message)
        return message

    def right_cycle_shift(self, byte_message: bytearray) -> bytearray:
        shift_byte_message = bytearray(len(byte_message))
        lowest_bit_of_last_byte = byte_message[-1] & 1
        for i in range(len(byte_message)):
            shift_byte_message[i] = (byte_message[i] & 0xFF) >> 1
            if i > 0:
                shift_byte_message[i] |= (byte_message[i - 1] & 1) << 7
        shift_byte_message[0] |= lowest_bit_of_last_byte << 7
        return shift_byte_message

    def left_cycle_shift(self, byte_message: bytearray) -> bytearray:
        shift_byte_message = bytearray(len(byte_message))
        highest_bit_of_first_byte = (byte_message[0] & 0x80) >> 7
        for i in range(len(byte_message)):
            shift_byte_message[i] = ((byte_message[i] & 0xFF) << 1) & 0xFF
            if i < len(byte_message) - 1:
                shift_byte_message[i] |= (byte_message[i + 1] & 0x80) >> 7
        shift_byte_message[-1] |= highest_bit_of_first_byte
        return shift_byte_message

    def send_data(self, event=None):
        if self.ser_out and self.ser_out.is_open:
            data = self.input_text.get("1.0", tk.END).strip()
            try:
                if not data:  # Если данные пустые (пользователь просто нажал Enter)
                    message = b'\n'  # Отправляем символ новой строки
                else:
                    message = data.encode('utf-8')
                while message:
                    portion = message[:21]  # Берем до 21 байта для поля данных
                    message = message[21:]

                    flag, frame_without_flag = self.create_frame(portion)
                    stuffed_frame = self.byte_stuffing(frame_without_flag)  # Применяем байт-стаффинг к кадру без флага
                    full_frame = flag + stuffed_frame  # Добавляем флаг к стаффированному кадру
                    self.ser_out.write(full_frame)

                    # Отображение структуры кадра с выделением модифицированных байтов
                    highlighted_frame_hex = ""
                    i = 0
                    while i < len(full_frame):
                        if full_frame[i:i + 2] == b'\x1B\x01':
                            highlighted_frame_hex += f"[0x{full_frame[i]:02X} 0x{full_frame[i + 1]:02X}] "
                            i += 2
                        else:
                            highlighted_frame_hex += f"0x{full_frame[i]:02X} "
                            i += 1
                    self.log_status(f"Отправлен кадр: {highlighted_frame_hex}")

                self.sent_portions += 1
                self.log_status(f"Отправлено порций: {self.sent_portions}")
                self.input_text.delete("1.0", tk.END)
            except serial.SerialException as e:
                messagebox.showerror("Ошибка", f"Не удалось отправить данные: {str(e)}")
        else:
            messagebox.showerror("Ошибка", "Порт передачи не открыт.")
        return 'break'

    def receive_data(self):
        buffer = b''
        expected_frame_length = 26  # 2 (flag) + 1 (dest) + 1 (source) + 21 (data) + 1 (CRC)
        while not self.stop_receive_thread:
            if self.ser_in and self.ser_in.is_open:
                try:
                    if self.ser_in.in_waiting >= expected_frame_length:
                        buffer = self.ser_in.read(self.ser_in.in_waiting)
                        if buffer.startswith(b'@u'):
                            flag = buffer[0:2]
                            frame_without_flag = buffer[2:]  # Исключаем флаг из кадра

                            # Анстаффинг без флага
                            unstuffed_frame = self.unstuffing(frame_without_flag)

                            destination_address = unstuffed_frame[0]
                            source_address = unstuffed_frame[1]
                            data = unstuffed_frame[2:23]  # 21 байт данных
                            received_crc = unstuffed_frame[23]  # Последний байт - CRC

                            # Вносим ошибку с вероятностью 40%
                            data_with_error = self.introduce_error(data)

                            # Проверяем CRC
                            calculated_crc = self.compute_crc8(data_with_error)
                            if calculated_crc == received_crc:
                                # Ищем терминатор и обрезаем данные, если он найден
                                if TERMINATOR in data_with_error:
                                    actual_data = data_with_error.split(TERMINATOR)[0]
                                else:
                                    actual_data = data_with_error

                                self.update_output_text(actual_data.decode(errors='ignore'))
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
        if message.startswith("Отправлен кадр:"):
            parts = message.split()
            frame_hex = " ".join(parts[2:-1])  # Все части кроме "Отправлен кадр:" и FCS
            fcs_hex = parts[-1]  # Последняя часть - FCS
            formatted_message = f"Отправлен кадр: {frame_hex} \xa0\xa0 {fcs_hex}\n"
        else:
            formatted_message = message + '\n'
        self.status_text.insert(tk.END, formatted_message)
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

    def on_closing(self):
        self.close_serial_ports()
        self.root.destroy()


if __name__ == "__main__":
    root = tk.Tk()
    app = SerialApp(root)
    root.mainloop()

# pip install pyserial