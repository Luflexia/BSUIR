import socket
import struct
import threading
import tkinter as tk
from tkinter import scrolledtext
from tkinter import simpledialog  # Исправленный импорт
import psutil
import time
import json

MCAST_GRP = '224.0.0.69'
MCAST_PORT = 3000
BUFFER_SIZE = 1024

class ChatApp:
    def __init__(self, root):
        self.root = root
        self.root.title("Chat Room")
        self.username = None
        self.prompt_username()

        # Список игнорируемых IP-адресов
        self.ignored_hosts = set()
        self.active_ips = set()  # Список активных IP-адресов
        self.running = True  # Флаг для управления потоком

        # Автоматическое определение сетевых параметров
        try:
            self.MY_PRIVATE_IP, self.BROADCAST_ADDR = self.get_network_params()
        except Exception as e:
            self.MY_PRIVATE_IP, self.BROADCAST_ADDR = '127.0.0.1', '255.255.255.255'
            self.display_network_info(f"Error getting network params: {e}. Using defaults.")

        # Основная сетка приложения
        self.main_frame = tk.Frame(root)
        self.main_frame.pack(padx=10, pady=10)

        # Область для отображения сообщений
        self.text_area = scrolledtext.ScrolledText(self.main_frame, wrap=tk.WORD, height=15, width=50, state='disabled')
        self.text_area.grid(row=0, column=0, padx=10, pady=10)

        # Область для вывода сетевых параметров и активных IP
        self.network_info_area = scrolledtext.ScrolledText(self.main_frame, wrap=tk.WORD, height=15, width=50, state='disabled')
        self.network_info_area.grid(row=0, column=1, padx=10, pady=10)

        # Поле ввода для multicast сообщений
        self.multicast_entry = tk.Entry(self.main_frame, width=50)
        self.multicast_entry.grid(row=1, column=0, columnspan=2, padx=10, pady=5)
        self.multicast_entry.bind("<Return>", self.send_multicast_message)
        self.multicast_entry.bind("<KP_Enter>", self.send_multicast_message)

        # Метка для multicast поля
        self.multicast_label = tk.Label(self.main_frame, text="Multicast Message Input")
        self.multicast_label.grid(row=2, column=0, columnspan=2, padx=10, pady=2)

        # Поле ввода для broadcast сообщений
        self.broadcast_entry = tk.Entry(self.main_frame, width=50)
        self.broadcast_entry.grid(row=3, column=0, columnspan=2, padx=10, pady=5)
        self.broadcast_entry.bind("<Return>", self.send_broadcast_message)
        self.broadcast_entry.bind("<KP_Enter>", self.send_broadcast_message)

        # Метка для broadcast поля
        self.broadcast_label = tk.Label(self.main_frame, text="Broadcast Message Input")
        self.broadcast_label.grid(row=4, column=0, columnspan=2, padx=10, pady=2)

        # Поле для ввода игнорируемого хоста
        self.ignore_entry = tk.Entry(self.main_frame, width=50)
        self.ignore_entry.grid(row=5, column=0, columnspan=2, padx=10, pady=5)
        self.ignore_entry.bind("<Return>", self.ignore_host)
        self.ignore_entry.bind("<KP_Enter>", self.ignore_host)

        # Метка для игнорируемого хоста
        self.ignore_label = tk.Label(self.main_frame, text="Enter Host IP to Ignore/Unignore")
        self.ignore_label.grid(row=6, column=0, columnspan=2, padx=10, pady=2)

        # Кнопки
        self.leave_button = tk.Button(self.main_frame, text="Leave Multicast Group", command=self.leave_multicast_group)
        self.leave_button.grid(row=7, column=0, padx=5, pady=5, sticky="ew")

        self.join_button = tk.Button(self.main_frame, text="Join Multicast Group", command=self.join_multicast_group)
        self.join_button.grid(row=7, column=1, padx=5, pady=5, sticky="ew")

        self.ignore_button = tk.Button(self.main_frame, text="Ignore Host", command=self.ignore_host)
        self.ignore_button.grid(row=8, column=0, padx=5, pady=5, sticky="ew")

        self.unignore_button = tk.Button(self.main_frame, text="Unignore Host", command=self.unignore_host)
        self.unignore_button.grid(row=8, column=1, padx=5, pady=5, sticky="ew")

        self.list_ips_button = tk.Button(self.main_frame, text="List Active IPs", command=self.list_active_ips)
        self.list_ips_button.grid(row=9, column=0, columnspan=2, padx=5, pady=5, sticky="ew")

        self.network_info_button = tk.Button(self.main_frame, text="Get Network Info", command=self.get_network_info)
        self.network_info_button.grid(row=10, column=0, columnspan=2, padx=5, pady=5, sticky="ew")

        # Сетевой сокет для multicast
        try:
            self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, socket.IPPROTO_UDP)
            self.sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
            self.sock.settimeout(0.1)
            self.sock.bind(('0.0.0.0', MCAST_PORT))
            self.mreq = struct.pack("4s4s", socket.inet_aton(MCAST_GRP), socket.inet_aton(self.MY_PRIVATE_IP))
            self.sock.setsockopt(socket.IPPROTO_IP, socket.IP_ADD_MEMBERSHIP, self.mreq)
        except Exception as e:
            self.display_network_info(f"Error initializing socket: {e}")
            self.MY_PRIVATE_IP = '127.0.0.1'
            self.BROADCAST_ADDR = '255.255.255.255'

        # Запуск потока для приема сообщений
        self.recv_thread = threading.Thread(target=self.receive_messages, daemon=True)
        self.recv_thread.start()

        # Обработка закрытия окна
        self.root.protocol("WM_DELETE_WINDOW", self.on_closing)

    def prompt_username(self):
        """Запрос имени пользователя при запуске"""
        try:
            self.username = simpledialog.askstring("Username", "Enter your username:", parent=self.root)
            if not self.username:
                self.username = f"User_{int(time.time())}"
        except Exception as e:
            self.username = f"User_{int(time.time())}"
            self.display_network_info(f"Error prompting username: {e}")

    def get_network_params(self):
        """Автоматическое определение IP, маски и широковещательного адреса"""
        interfaces = psutil.net_if_addrs()
        for interface, addrs in interfaces.items():
            for addr in addrs:
                if addr.family == socket.AF_INET and not addr.address.startswith('127.'):
                    ip_address = addr.address
                    netmask = addr.netmask
                    broadcast = addr.broadcast if addr.broadcast else self.calculate_broadcast(ip_address, netmask)
                    return ip_address, broadcast
        raise Exception("No suitable network interface found")

    def calculate_broadcast(self, ip, netmask):
        """Вычисление широковещательного адреса"""
        ip_int = struct.unpack('!I', socket.inet_aton(ip))[0]
        mask_int = struct.unpack('!I', socket.inet_aton(netmask))[0]
        broadcast_int = ip_int | (~mask_int & 0xffffffff)
        return socket.inet_ntoa(struct.pack('!I', broadcast_int))

    def get_network_info(self):
        """Вывод информации о сетевых интерфейсах"""
        self.network_info_area.config(state='normal')
        self.network_info_area.delete(1.0, tk.END)
        interfaces = psutil.net_if_addrs()
        for interface, addrs in interfaces.items():
            for addr in addrs:
                if addr.family == socket.AF_INET:
                    self.network_info_area.insert(tk.END, f"Interface: {interface}\n")
                    self.network_info_area.insert(tk.END, f"IP Address: {addr.address}\n")
                    self.network_info_area.insert(tk.END, f"Netmask: {addr.netmask}\n")
                    self.network_info_area.insert(tk.END, f"Broadcast: {addr.broadcast or 'N/A'}\n\n")
        self.network_info_area.config(state='disabled')
        self.network_info_area.yview(tk.END)

    def send_multicast_message(self, event=None):
        """Отправка мультикастового сообщения"""
        message = self.multicast_entry.get().strip()
        if message:
            try:
                msg_data = {
                    'type': 'message',
                    'username': self.username,
                    'timestamp': time.strftime('%H:%M:%S'),
                    'content': message
                }
                self.sock.sendto(json.dumps(msg_data).encode('utf-8'), (MCAST_GRP, MCAST_PORT))
                self.display_message(f"[{msg_data['timestamp']}] {msg_data['username']} (me): {msg_data['content']}")
                self.multicast_entry.delete(0, tk.END)
            except Exception as e:
                self.display_network_info(f"Error sending multicast message: {e}")

    def send_broadcast_message(self, event=None):
        """Отправка широковещательного сообщения"""
        message = self.broadcast_entry.get().strip()
        if message:
            try:
                broadcast_sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
                broadcast_sock.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, 1)
                broadcast_sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
                msg_data = {
                    'type': 'message',
                    'username': self.username,
                    'timestamp': time.strftime('%H:%M:%S'),
                    'content': message
                }
                broadcast_sock.sendto(json.dumps(msg_data).encode('utf-8'), (self.BROADCAST_ADDR, MCAST_PORT))
                self.display_message(f"[{msg_data['timestamp']}] {msg_data['username']} (me): {msg_data['content']}")
                self.broadcast_entry.delete(0, tk.END)
            except Exception as e:
                self.display_network_info(f"Error sending broadcast message: {e}")
            finally:
                broadcast_sock.close()

    def list_active_ips(self):
        """Отправка запроса на проверку активных IP"""
        try:
            self.active_ips.clear()
            self.network_info_area.config(state='normal')
            self.network_info_area.delete(1.0, tk.END)
            msg_data = {'type': 'check_active', 'content': 'Check active applications'}
            broadcast_sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
            broadcast_sock.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, 1)
            broadcast_sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
            broadcast_sock.sendto(json.dumps(msg_data).encode('utf-8'), (self.BROADCAST_ADDR, MCAST_PORT))
            broadcast_sock.close()
            self.root.after(2000, self.display_active_ips)
        except Exception as e:
            self.display_network_info(f"Error listing active IPs: {e}")

    def display_active_ips(self):
        """Отображение списка активных IP"""
        self.network_info_area.config(state='normal')
        self.network_info_area.delete(1.0, tk.END)
        if self.active_ips:
            self.network_info_area.insert(tk.END, "Active IPs:\n")
            for ip in sorted(self.active_ips):
                self.network_info_area.insert(tk.END, f"{ip}\n")
        else:
            self.network_info_area.insert(tk.END, "No active IPs found.\n")
        self.network_info_area.config(state='disabled')
        self.network_info_area.yview(tk.END)

    def receive_messages(self):
        """Прием сообщений"""
        while self.running:
            try:
                msg, addr = self.sock.recvfrom(BUFFER_SIZE)
                if addr[0] == self.MY_PRIVATE_IP:
                    continue
                try:
                    msg_data = json.loads(msg.decode('utf-8'))
                    msg_type = msg_data.get('type', 'message')
                    if msg_type == 'message' and addr[0] not in self.ignored_hosts:
                        self.display_message(f"[{msg_data['timestamp']}] {msg_data['username']} ({addr[0]}): {msg_data['content']}")
                    elif msg_type == 'check_active':
                        response = {'type': 'health_ok', 'content': 'Health Okey', 'username': self.username}
                        broadcast_sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
                        broadcast_sock.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, 1)
                        broadcast_sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
                        broadcast_sock.sendto(json.dumps(response).encode('utf-8'), (self.BROADCAST_ADDR, MCAST_PORT))
                        broadcast_sock.close()
                    elif msg_type == 'health_ok':
                        self.active_ips.add(addr[0])
                        self.display_network_info(f"Active host: {addr[0]}")
                    elif msg_type == 'ignore_host' and addr[0] not in self.ignored_hosts:
                        target_ip = msg_data.get('content')
                        if target_ip and target_ip != self.MY_PRIVATE_IP:
                            self.ignored_hosts.add(target_ip)
                            self.display_network_info(f"Ignored host {target_ip} by request from {addr[0]}")
                except json.JSONDecodeError:
                    self.display_network_info(f"Invalid message format from {addr[0]}")
            except socket.timeout:
                continue
            except Exception as e:
                if self.running:
                    self.display_network_info(f"Receive error: {e}")

    def display_message(self, message):
        """Отображение сообщения в чате"""
        self.text_area.config(state='normal')
        self.text_area.insert(tk.END, message + '\n')
        self.text_area.config(state='disabled')
        self.text_area.yview(tk.END)

    def display_network_info(self, info):
        """Отображение сетевой информации"""
        self.network_info_area.config(state='normal')
        self.network_info_area.insert(tk.END, f"[{time.strftime('%H:%M:%S')}] {info}\n")
        self.network_info_area.config(state='disabled')
        self.network_info_area.yview(tk.END)

    def leave_multicast_group(self):
        """Выход из мультикастовой группы"""
        try:
            self.sock.setsockopt(socket.IPPROTO_IP, socket.IP_DROP_MEMBERSHIP, self.mreq)
            self.display_network_info("Left the multicast group.")
        except Exception as e:
            self.display_network_info(f"Error leaving multicast group: {e}")

    def join_multicast_group(self):
        """Вход в мультикастовую группу"""
        try:
            self.sock.setsockopt(socket.IPPROTO_IP, socket.IP_ADD_MEMBERSHIP, self.mreq)
            self.display_network_info("Joined the multicast group.")
        except Exception as e:
            self.display_network_info(f"Error joining multicast group: {e}")

    def ignore_host(self, event=None):
        """Игнорирование хоста"""
        host = self.ignore_entry.get().strip()
        if host and host != self.MY_PRIVATE_IP:
            self.ignored_hosts.add(host)
            self.ignore_entry.delete(0, tk.END)
            self.display_network_info(f"Ignored host: {host}")
            try:
                msg_data = {'type': 'ignore_host', 'content': host, 'username': self.username}
                broadcast_sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
                broadcast_sock.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, 1)
                broadcast_sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
                broadcast_sock.sendto(json.dumps(msg_data).encode('utf-8'), (self.BROADCAST_ADDR, MCAST_PORT))
                broadcast_sock.close()
            except Exception as e:
                self.display_network_info(f"Error broadcasting ignore request: {e}")

    def unignore_host(self):
        """Снятие игнорирования хоста"""
        host = self.ignore_entry.get().strip()
        if host in self.ignored_hosts:
            self.ignored_hosts.remove(host)
            self.ignore_entry.delete(0, tk.END)
            self.display_network_info(f"Unignored host: {host}")

    def on_closing(self):
        """Обработка закрытия окна"""
        self.running = False
        try:
            self.sock.close()
        except:
            pass
        self.root.destroy()

if __name__ == "__main__":
    root = tk.Tk()
    app = ChatApp(root)
    root.mainloop()
