import random
import tkinter as tk
from tkinter import ttk, messagebox
import serial
import serial.tools.list_ports
import threading
import time

class SerialApp:
    def __init__(self, root):
        self.root = root
        self.root.title("Serial ports communication")

        self.ser_in = None
        self.ser_out = None
        self.receive_thread = None
        self.stop_receive_thread = False
        self.sent_portions = 0
        self.collision_window = 16  
        self.slot_time = 0.5 
        self.channel_busy_probability = 0.4  
        self.collision_probability = 0.6  

        self.output_port_label = tk.Label(root, text="Select COM Port For Sending:")
        self.output_port_label.grid(row=0, column=0, padx=10, pady=5, sticky="w")
        self.output_port_combobox = ttk.Combobox(root, values=self.get_com_ports(), state="readonly")
        self.output_port_combobox.grid(row=0, column=1, padx=10, pady=5, sticky="e")
        self.output_port_combobox.bind("<<ComboboxSelected>>", self.open_output_port)

        self.input_port_label = tk.Label(root, text="Select COM Port For Receiving:")
        self.input_port_label.grid(row=1, column=0, padx=10, pady=5, sticky="w")
        self.input_port_combobox = ttk.Combobox(root, values=self.get_com_ports(), state="readonly")
        self.input_port_combobox.grid(row=1, column=1, padx=10, pady=5, sticky="e")
        self.input_port_combobox.bind("<<ComboboxSelected>>", self.open_input_port)

        self.close_ports_button = tk.Button(root, text="Close Ports", command=self.close_serial_ports)
        self.close_ports_button.grid(row=2, column=1, padx=10, pady=10, sticky="e")

        self.output_label = tk.Label(root, text="Output:")
        self.output_label.grid(row=3, column=0, padx=10, pady=5, sticky="w")
        self.output_text = tk.Text(root, width=50, height=6, state='disabled')
        self.output_text.grid(row=4, column=0, columnspan=2, padx=10, pady=5)

        self.input_label = tk.Label(root, text="Input:")
        self.input_label.grid(row=5, column=0, padx=10, pady=5, sticky="w")
        self.input_entry = tk.Entry(root, width=67)
        self.input_entry.grid(row=6, column=0, columnspan=2, padx=10, pady=5, sticky="w")

        self.input_entry.bind("<Return>", self.send_data)

        self.status_label = tk.Label(root, text="Status:")
        self.status_label.grid(row=7, column=0, padx=10, pady=5, sticky="w")
        self.status_text = tk.Text(root, width=50, height=6, state='disabled', relief=tk.SUNKEN)
        self.status_text.grid(row=8, column=0, columnspan=2, padx=10, pady=5)

        self.update_com_ports()

        self.root.protocol("WM_DELETE_WINDOW", self.on_closing)

    def get_com_ports(self):
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

    def update_com_ports(self):
        available_ports = self.get_com_ports()

        if self.ser_in and self.ser_in.is_open:
            corresponding_sending_port = f"COM{int(self.ser_in.portstr[3:]) - 1}"
            if corresponding_sending_port in available_ports:
                available_ports.remove(corresponding_sending_port)

        if self.ser_out and self.ser_out.is_open:
            corresponding_receiving_port = f"COM{int(self.ser_out.portstr[3:]) + 1}"
            if corresponding_receiving_port in available_ports:
                available_ports.remove(corresponding_receiving_port)

        self.input_port_combobox['values'] = available_ports
        self.output_port_combobox['values'] = available_ports

        self.root.after(1000, self.update_com_ports)

    def open_input_port(self, event):
        try:
            selected_port = self.input_port_combobox.get()
            if self.ser_in and self.ser_in.is_open:
                self.ser_in.close()

            self.ser_in = serial.Serial(selected_port, baudrate=9600, timeout=1)
            self.status_text.config(state='normal')
            self.status_text.insert(tk.END, f"Receiving Port is opened: {selected_port}\n")
            self.status_text.insert(tk.END, f"Baudrate: {self.ser_in.baudrate}, Parity: {self.ser_in.parity}, Stopbits: {self.ser_in.stopbits}\n")
            self.status_text.see(tk.END)
            self.status_text.config(state='disabled')

            self.receive_thread = threading.Thread(target=self.receive_data)
            self.receive_thread.start()

        except Exception as e:
            messagebox.showerror("Error", f"Failed to open receiving port: {e}")

    def open_output_port(self, event):
        try:
            selected_port = self.output_port_combobox.get()
            if self.ser_out and self.ser_out.is_open:
                self.ser_out.close()
            self.ser_out = serial.Serial(selected_port, baudrate=9600, timeout=1)
            self.status_text.config(state='normal')
            self.status_text.insert(tk.END, f"Sending Port is opened: {selected_port}\n")
           
            self.status_text.insert(tk.END, f"Baudrate: {self.ser_out.baudrate}, Parity: {self.ser_out.parity}, Stopbits: {self.ser_out.stopbits}\n")
            self.status_text.see(tk.END)
            self.status_text.config(state='disabled')
        except Exception as e:
            messagebox.showerror("Error", f"Choose the sending COM port: {e}")

    def close_serial_ports(self):
        self.stop_receive_thread = True

        if self.receive_thread and self.receive_thread.is_alive():
            self.receive_thread.join()

        if self.ser_out and self.ser_out.is_open:
            self.ser_out.close()

        if self.ser_in and self.ser_in.is_open:
            self.ser_in.close()

        self.output_port_combobox.set("")
        self.input_port_combobox.set("")

        self.status_text.config(state='normal')
        self.status_text.insert(tk.END, "Ports are closed.\n")
        self.status_text.see(tk.END)
        self.status_text.config(state='disabled')

        self.receive_thread = None
        self.stop_receive_thread = False

    def byte_stuffing(self, data):
        stuffed_data = data.replace(b'\x40\x77', b'\x40\x03')
        return stuffed_data

    def unstuffing(self, data):    
        unstuffed_data = data.replace(b'\x40\x03', b'\x40\x77')
        return unstuffed_data

    def send_data(self, event=None):
        if self.ser_out and self.ser_out.is_open:
            threading.Thread(target=self._send_data_thread, daemon=True).start()
        else:
            messagebox.showwarning("Port Error", "Choose the sending COM port.")

    def _send_data_thread(self):
        try:
            message = self.input_entry.get().encode()
            message += b'\n'
            while message:
                portion = message[:23]
                message = message[23:]
                frame = self.create_frame(portion)

                highlighted_frame_hex = ""
                i = 0
                while i < len(frame) - 1:
                    if frame[i:i+2] == b'\x40\x03':
                        highlighted_frame_hex += f"[0x{frame[i]:02X} 0x{frame[i+1]:02X}] "
                        i += 2
                    else:
                        highlighted_frame_hex += f"0x{frame[i]:02X} "
                        i += 1
                highlighted_frame_hex += f"   0x{frame[len(frame)-1]:02X}"
            
                self.root.after(0, self.update_status, f"Frame sent: {highlighted_frame_hex}")
            
                success = self.csma_cd(frame)
                
                self.status_text.config(state='normal')
                self.status_text.insert(tk.END, f"\n")
                self.status_text.see(tk.END)
                self.status_text.config(state='disabled')

                if not success:
                    self.root.after(0, self.update_status, "Transmission failed due to excessive collisions.\n")
                    break

            self.sent_portions += 1
            self.root.after(0, self.update_status, f"Portions sent: {self.sent_portions}\n")
            self.root.after(0, self.input_entry.delete, 0, tk.END)
        except Exception as e:
            self.root.after(0, messagebox.showerror, "Send Error", str(e))

    def update_status(self, message):
        self.status_text.config(state='normal')
        self.status_text.insert(tk.END, message)
        self.status_text.see(tk.END)
        self.status_text.config(state='disabled')
            
    def csma_cd(self, frame):
        collision_count = 0

        while collision_count < self.collision_window:
            if random.random() < self.channel_busy_probability:
                time.sleep(self.slot_time)
                continue
            self.ser_out.write(frame)
            if random.random() < self.collision_probability:
                collision_count += 1
                self.status_text.config(state='normal')
                self.status_text.insert(tk.END, f" #")
                self.status_text.see(tk.END)
                self.status_text.config(state='disabled')
                r = random.randint(0, collision_count)
                delay = (2 ** r - 1) * self.slot_time
                time.sleep(delay)
                continue 
            else:
                
                return True

        return False

            
    def calculate_fcs(self, data):
           
            polynomial = 0x107
            fcs = 0
        
            for byte in data:
                fcs ^= byte
                for _ in range(8):
                    if fcs & 0x80:
                        fcs = (fcs << 1) ^ polynomial
                    else:
                        fcs <<= 1
                fcs &= 0xFF 
            return fcs
    
    def check_fcs(self, data, received_fcs):
        
        calculated_fcs = self.calculate_fcs(data)
        return calculated_fcs == received_fcs

            
    def create_frame(self, data):
            flag = b'@w'
            destination_address = b'\x00'
            source_address = bytes([int(self.ser_out.portstr[3:])])
            data = data.ljust(23, b'\x00')  

            fcs = bytes([self.calculate_fcs(data)])

            stuffed_data = self.byte_stuffing(data)

            frame = flag + destination_address + source_address + stuffed_data + fcs
            return frame


    def receive_data(self):
        buffer = b''
        expected_frame_length = 28

        while not self.stop_receive_thread:
            if self.ser_in and self.ser_in.is_open:
                try:
                    if self.ser_in.in_waiting >= expected_frame_length:
                        buffer = self.ser_in.read(self.ser_in.in_waiting)
                        if buffer.startswith(b'@w'):
                            flag_start = buffer[0:2]
                            destination_address = buffer[2]
                            source_address = buffer[3]
                            data_with_fcs = buffer[4:]
                            data = data_with_fcs[:-1]
                            received_fcs = data_with_fcs[-1]
                            unstuffed_data = self.unstuffing(data)
                            if self.check_fcs(unstuffed_data, received_fcs):
                                self.output_text.config(state='normal')
                                self.output_text.insert(tk.END, f"{unstuffed_data.decode(errors='ignore')}")
                                self.output_text.see(tk.END)
                                self.output_text.config(state='disabled')
                            else:
                                self.status_text.config(state='normal')
                                self.status_text.insert(tk.END, "FCS mismatch detected\n")
                                self.status_text.see(tk.END)
                                self.status_text.config(state='disabled')

                except Exception as e:
                    messagebox.showerror("Receive Error", str(e))
            else:
                break



    def on_closing(self):
        self.close_serial_ports()
        self.root.destroy()


root = tk.Tk()
root.resizable(False, False)
app = SerialApp(root)
root.mainloop()

