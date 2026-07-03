import serial
import threading
import tkinter as tk
import time
from tkinter import ttk, messagebox
import serial.tools.list_ports
from cycle_code import compute_crc8, cyclic_shift_correction, introduce_error
from collision import Collision

available_ports = ["Не выбран"] + [f"COM{i}" for i in range(1, 20)]

class COMPort:
    def __init__(self, port_name=None, baudrate=9600):
        self._port_name = port_name
        self._baudrate = baudrate
        self._portID = None
        self._init_flag = False
        self.messages_received = 0
        self._stop_thread = False
        self._thread = None
        self.collision = Collision()

    def create_packet(self, data: str, port_number: int):
        n = 3  
        if len(data) > n:
            data = data[:n]  
        elif len(data) < n:
            data = data.ljust(n, '\x00')  

        flag = '@' + chr(ord('a') + (n - 1))
        destination_address = chr(0)
        source_address = chr(port_number) 
        packet_without_crc = f"{flag}{destination_address}{source_address}{data}"
        
        crc = compute_crc8(packet_without_crc)
        if crc == 10:
            crc = 0
        packet_with_crc = f"{packet_without_crc}{chr(crc)}"  
        
        return packet_with_crc

    def byte_stuffing(self, packet: str):
        flag = packet[0:2]  
        rest_of_packet = packet[2:]  

        stuffed_packet = flag  
        modified_bytes = []
        i = 0

        while i < len(rest_of_packet):
            if rest_of_packet[i:i+2] == '@c':
                stuffed_packet += '96'
                modified_bytes.append(f'@c -> 96')
                i += 2
            elif rest_of_packet[i] == '9':
                stuffed_packet += '95'
                modified_bytes.append(f'9 -> 95')
                i += 1
            else:
                stuffed_packet += rest_of_packet[i]
                i += 1
        return stuffed_packet, modified_bytes

    def byte_destuffing(self):
        destuffed_data = ""
        i = 0
        while True:
            c = self._portID.read(1).decode('utf-8', 'ignore')
            if c == '9':
                n = self._portID.read(1).decode('utf-8', 'ignore')
                if n == '5':
                    destuffed_data += '9'
                    i += 1
                elif n == '6':
                    destuffed_data += '@c'
                    i += 2
                else:
                    destuffed_data += c + n
                    i += 2        
            else:
                destuffed_data += c
                i += 1

            if i == 3:
                break

        c = self._portID.read(1).decode('utf-8', 'ignore')
        return destuffed_data + c
    
        
    def init_com_port(self):
        try:    
            if self._portID and self._portID.is_open:
                raise serial.SerialException(f"COM порт {self._port_name} уже открыт.")
            self._portID = serial.Serial(self._port_name, baudrate=self._baudrate, timeout=1)
            self._init_flag = True
            self._stop_thread = False
        except serial.SerialException as e:
            self._init_flag = False
            messagebox.showerror("Ошибка COM порта", f"Ошибка открытия COM порта {self._port_name}: {"COM-порт открыт в другом окне. Выберите другой, соответствующий топологии"}")

    def close_com_port(self):
        try:
            if self._portID and self._portID.is_open:
                self._portID.reset_input_buffer()
                self._portID.reset_output_buffer()
                self._stop_thread = True  
                if self._thread and self._thread.is_alive():
                    self._thread.join(timeout=1) 
                self._portID.close()
                self._init_flag = False
        except serial.SerialException as e:
            messagebox.showerror("Ошибка COM порта", f"Ошибка закрытия COM порта {self._port_name}: {"Не удалось закрыть COM-порт"}")

    def read_from_port(self, output_textbox, status_label):
        if not self._init_flag:
            return
        self._thread = threading.Thread(target=self._read_loop, args=(output_textbox, status_label), daemon=True)
        self._thread.start()

    def _read_loop(self, output_textbox, status_label):
        buffer = ""
        packet_started = False
        while self._init_flag and not self._stop_thread:
            char = self._portID.read(1)
            if char:
                decoded_char = char.decode('utf-8', 'ignore')
                if decoded_char in [' ', '\x00']:
                    continue
                if decoded_char == '@':
                    second_char = self._portID.read(1).decode('utf-8', 'ignore')
                    if second_char == 'c':
                        third_char = self._portID.read(1).decode('utf-8', 'ignore')
                        packet = '@c' + third_char
                        packet += self.byte_destuffing()
                        data = packet[4:] 
                        received_crc = ord(packet[-1])
                        data_after_chance_of_int_error = introduce_error(data)
                        corrected_data = cyclic_shift_correction(data_after_chance_of_int_error, int(received_crc))
                        buffer += corrected_data  
                        self._insert_text_to_output(output_textbox, buffer)
                        buffer = ""
                elif packet_started:
                    buffer += decoded_char


    def update_status_window(self, status_label, packet, modified_bytes, packet_number):
        flag = packet[0:2]
        destination_address = ord(packet[2])
        source_address = ord(packet[3]) if packet[3] != '9' else packet[3]
        data = packet[4:-1].replace('\x00', '0')  
        crc = packet[-1]

        status_label.config(state="normal")
        status_label.insert(tk.END, f"Пакет {packet_number}: {flag}{destination_address}{source_address}{data} {crc} ")
        status_label.config(state="disabled")

        status_label.tag_configure("modified", foreground="red")

        for byte in modified_bytes:
            start_index = status_label.search(byte[-2:], "1.0", tk.END)  
            while start_index:
                end_index = f"{start_index}+{len(byte[-2:])}c"  
                status_label.tag_add("modified", start_index, end_index)
                start_index = status_label.search(byte[-2:], end_index, tk.END)

    def _insert_text_to_output(self, output_textbox, text):
        output_textbox.config(state="normal")
        output_textbox.insert(tk.END, text)
        output_textbox.see(tk.END)
        output_textbox.config(state="disabled")

    def _insert_text_to_status(self, status_label, text):
        status_label.config(state="normal")
        status_label.insert(tk.END, text)
        status_label.see(tk.END)
        status_label.config(state="disabled")

    def write_to_port(self, message: str, input_textbox, status_label):
        if not self._init_flag or not (self._portID and self._portID.is_open):
            return
        com_port_number = int(self._port_name[3:])  
        n = 3  
        packets = [message[i:i + n] for i in range(0, len(message), n)]  

        input_textbox.config(state="disabled")
        time.sleep(0.1)
        try:
            for i, packet in enumerate(packets):
                count_of_tries = 10
                collision_count = 0
                while self.collision.listen_channel():
                    pass
                packet_data = self.create_packet(packet, com_port_number)  
                stuffed_packet, modified = self.byte_stuffing(packet_data) 
                self._portID.write(stuffed_packet.encode('utf-8'))
                self.update_status_window(status_label, stuffed_packet, modified, i + 1)
                time.sleep(0.1)
                while self.collision.detect_collision():
                    collision_count += 1
                    if collision_count == count_of_tries:
                        break
                    while self.collision.listen_channel():
                        pass
                    self._insert_text_to_status(status_label, "#")
                    self.collision.delay(collision_count)
                    self._portID.write(stuffed_packet.encode('utf-8'))

                self._insert_text_to_status(status_label, "\n")
        except serial.SerialException as e:
            messagebox.showerror("Ошибка COM порта", f"Ошибка записи в COM порт {self._port_name}: {str(e)}")
        finally:
            input_textbox.config(state="normal")


    def increment_message_count(self, status_label):
        self.messages_received += 1  
        status_label.config(state="normal")
        status_label.insert(tk.END, f"\nКоличество отправленных сообщений: {self.messages_received}\n")
        status_label.config(state="disabled")

class SerialApp:
    def get_ports(self):
        ports = serial.tools.list_ports.comports()
        return ["Не выбран"] + [port.device for port in ports]


    def __init__(self, root):
        self.root = root
        self.root.title("Лабораторная работа №4")

        self.root.geometry("500x700")
        self.root.minsize(500, 700)

        self.root.resizable(False, False)

        self.style = ttk.Style()
        self.style.configure("TButton", padding=6, relief="flat", background="#ccc")
        self.style.configure("TLabel", font=("Arial", 10), background="#f0f0f0")
        self.style.configure("TCombobox", padding=5, font=("Arial", 10))

        self.control_frame = tk.Frame(root, bg="#e0e0e0")
        self.control_frame.pack(padx=10, pady=10, fill=tk.X)

        control_label = ttk.Label(self.control_frame, text="Окно управления", font=("Arial", 12))
        control_label.grid(row=0, columnspan=2, pady=5)

        tx_port_label = ttk.Label(self.control_frame, text="COMПорт для передачи", font=("Arial", 10))
        tx_port_label.grid(row=1, column=0, padx=5, pady=5)

        rx_port_label = ttk.Label(self.control_frame, text="COMПорт для приёма", font=("Arial", 10))
        rx_port_label.grid(row=2, column=0, padx=5, pady=5)

        all_ports = self.get_ports()

        self.tx_port_combo = ttk.Combobox(self.control_frame, values=all_ports, state="readonly", width=20)
        self.tx_port_combo.grid(row=1, column=1, padx=5, pady=5)
        self.tx_port_combo.set("Не выбран")
        self.tx_port_combo.bind("<<ComboboxSelected>>", self.on_port_selected)

        self.rx_port_combo = ttk.Combobox(self.control_frame, values=all_ports, state="readonly", width=20)
        self.rx_port_combo.grid(row=2, column=1, padx=5, pady=5)
        self.rx_port_combo.set("Не выбран")
        self.rx_port_combo.bind("<<ComboboxSelected>>", self.on_port_selected)

        input_label = ttk.Label(root, text="Окно ввода", font=("Arial", 12))
        input_label.pack(pady=5)
        self.input_textbox = tk.Text(root, height=5, font=("Arial", 10))
        self.input_textbox.pack(padx=10, pady=5, fill=tk.X)
        self.input_textbox.bind("<Return>", self.send_message)

        output_label = ttk.Label(root, text="Окно вывода", font=("Arial", 12))
        output_label.pack(pady=5)
        self.output_textbox = tk.Text(root, height=10, font=("Arial", 10))
        self.output_textbox.pack(padx=10, pady=5, fill=tk.X)
        self.output_textbox.config(state="disabled")

        status_label = ttk.Label(root, text="Окно состояния", font=("Arial", 12))
        status_label.pack(pady=5)
        self.status_label = tk.Text(root, height=10, font=("Arial", 10))
        self.status_label.pack(padx= 10, pady=5, fill=tk.X)
        self.status_label.config(state='disabled')

        self.tx_port = None
        self.rx_port = None

        self.available_ports = []
        self.update_ports_periodically()
        self.create_static_status_window()

    def update_ports_periodically(self):
        current_ports = self.get_ports()
        if current_ports != self.available_ports:
            self.available_ports = current_ports
            self.update_combos()
        self.root.after(200, self.update_ports_periodically)

    def create_static_status_window(self):
        static_info = (
            f"Скорость передачи данных: 9600\n"
            f"Кол-во битов в байте: 8\n"
            f"Кол-во стоп-битов: 1\n"
            f"Паритет: не используется\n"
            f"Аппаратный контроль DTR: включён\n"
            f"Аппаратный контроль RTS: включён\n"
        )
        self.status_label.config(state="normal")  
        self.status_label.insert(tk.END, static_info)  
        self.status_label.config(state="disabled")  

    def on_port_selected(self, event):
        tx_port_name = self.tx_port_combo.get()
        rx_port_name = self.rx_port_combo.get()

        if tx_port_name == "Не выбран" and self.tx_port and self.tx_port._init_flag:
            self.tx_port.close_com_port()
            self.tx_port = None

        if rx_port_name == "Не выбран" and self.rx_port and self.rx_port._init_flag:
            self.rx_port.close_com_port()
            self.rx_port = None

        if tx_port_name == rx_port_name and tx_port_name != "Не выбран":
            return

        if self.tx_port and self.tx_port._init_flag:
            self.tx_port.close_com_port()
        if self.rx_port and self.rx_port._init_flag:
            self.rx_port.close_com_port()

        if tx_port_name != "Не выбран":
            self.tx_port = COMPort(port_name=tx_port_name)
            self.tx_port.init_com_port()

        if rx_port_name != "Не выбран":
            self.rx_port = COMPort(port_name=rx_port_name)
            self.rx_port.init_com_port()
            self.rx_port.read_from_port(self.output_textbox, self.status_label)

        self.update_combos()


    def update_combos(self):
        selected_tx_port = self.tx_port_combo.get()
        selected_rx_port = self.rx_port_combo.get()

        available_ports = self.get_ports()

        if selected_tx_port != "Не выбран":
            tx_port_number = int(selected_tx_port[3:])
            if selected_tx_port in available_ports:
                available_ports.remove(selected_tx_port)
            paired_rx_port = f"COM{tx_port_number + 1}"
            if paired_rx_port in available_ports:
                available_ports.remove(paired_rx_port)

        if selected_rx_port != "Не выбран":
            rx_port_number = int(selected_rx_port[3:])
            if selected_rx_port in available_ports:
                available_ports.remove(selected_rx_port)
            paired_tx_port = f"COM{rx_port_number - 1}"
            if paired_tx_port in available_ports:
                available_ports.remove(paired_tx_port)

        self.tx_port_combo.config(values=available_ports)
        self.rx_port_combo.config(values=available_ports)

    def send_message(self, event):
        message = self.input_textbox.get("1.0", tk.END).strip()
        if message and self.tx_port and self.tx_port._init_flag:
        
            def send_in_thread():
                self.tx_port.write_to_port(message, self.input_textbox, self.status_label)
                self.tx_port.increment_message_count(self.status_label)
                self.input_textbox.delete("1.0", tk.END)

            thread = threading.Thread(target=send_in_thread, daemon=True)
            thread.start()
            return "break"
        return None


if __name__ == "__main__":
    root = tk.Tk()
    app = SerialApp(root)
    root.mainloop()