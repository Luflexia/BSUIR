import socket
import threading
import time
import struct
from datetime import datetime


class PeerToPeerChat:
    def __init__(self):
        self.udp_port = 12345
        self.multicast_group = "239.255.255.250"
        self.multicast_port = 12346

        self.username = input("Введите ваше имя: ")
        self.running = True
        self.peers = {}
        self.ignored_peers = set()
        self.in_multicast_group = True  

        self.network_info = self.get_network_info()
        self.local_ip = self.network_info['local_ip']

        print(f"\n=== СЕТЕВЫЕ ПАРАМЕТРЫ ===")
        print(f"Ваш IP: {self.local_ip}")
        print(f"Сетевая маска: {self.network_info['netmask']}")
        print(f"Широковещательный адрес: {self.network_info['broadcast_ip']}")
        print(f"Multicast группа: {self.multicast_group}:{self.multicast_port}")

        self.setup_sockets()

        print(f"\nЧат запущен!")
        print("=== РЕЖИМЫ ПЕРЕДАЧИ ===")
        print("UNICAST - личные сообщения (авто)")
        print("MULTICAST - групповые сообщения (/multicast)")
        print("BROADCAST - широковещание (/broadcast)")
        print("\n=== КОМАНДЫ ===")
        print("/list - список участников")
        print("/ignore [IP] - игнорировать участника")
        print("/unignore [IP] - снять игнорирование")
        print("/multicast - отправить в группу")
        print("/broadcast - широковещательное сообщение")
        print("/join - войти в multicast группу")
        print("/leave - выйти из multicast группы")
        print("/quit - выход")
        print("-" * 50)

    def get_network_info(self):
        network_info = {
            'local_ip': '127.0.0.1',
            'netmask': '255.255.255.0',
            'broadcast_ip': '255.255.255.255'
        }

        try:
            with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as s:
                s.connect(("8.8.8.8", 80))
                local_ip = s.getsockname()[0]
                network_info['local_ip'] = local_ip
                if local_ip.startswith('192.168.'):
                    network_info['netmask'] = '255.255.255.0'
                    parts = local_ip.split('.')
                    network_info['broadcast_ip'] = f"{parts[0]}.{parts[1]}.{parts[2]}.255"
                elif local_ip.startswith('10.'):
                    network_info['netmask'] = '255.0.0.0'
                    network_info['broadcast_ip'] = '10.255.255.255'
                elif local_ip.startswith('172.'):
                    network_info['netmask'] = '255.255.0.0'
                    parts = local_ip.split('.')
                    network_info['broadcast_ip'] = f"{parts[0]}.{parts[1]}.255.255"

        except Exception as e:
            print(f"Ошибка определения сетевых параметров: {e}")

        return network_info

    def setup_sockets(self):
        try:
            self.unicast_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
            self.unicast_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
            self.unicast_socket.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, 1)
            self.unicast_socket.bind(('0.0.0.0', self.udp_port))

            self.multicast_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
            self.multicast_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
            self.multicast_socket.bind(('', self.multicast_port))

            self.join_multicast_group()

        except Exception as e:
            print(f"Ошибка настройки сокетов: {e}")
            exit(1)

    def join_multicast_group(self):
        try:
            group = socket.inet_aton(self.multicast_group)
            mreq = struct.pack('4sL', group, socket.INADDR_ANY)
            self.multicast_socket.setsockopt(
                socket.IPPROTO_IP, socket.IP_ADD_MEMBERSHIP, mreq
            )
            self.in_multicast_group = True
            print(f"Присоединились к multicast группе {self.multicast_group}")
        except Exception as e:
            print(f"Ошибка присоединения к multicast: {e}")

    def leave_multicast_group(self):
        try:
            if self.in_multicast_group:
                group = socket.inet_aton(self.multicast_group)
                mreq = struct.pack('4sL', group, socket.INADDR_ANY)
                self.multicast_socket.setsockopt(
                    socket.IPPROTO_IP, socket.IP_DROP_MEMBERSHIP, mreq
                )
                self.in_multicast_group = False
                print(f"Покинули multicast группу {self.multicast_group}")
        except Exception as e:
            print(f"Ошибка выхода из multicast: {e}")

    def send_unicast(self, message, target_ip=None):
        try:
            msg = f"UNICAST:{self.username}:{message}"
            if target_ip:
                self.unicast_socket.sendto(msg.encode('utf-8'), (target_ip, self.udp_port))
            else:
                for ip in self.peers.keys():
                    if ip != self.local_ip and ip not in self.ignored_peers:
                        self.unicast_socket.sendto(msg.encode('utf-8'), (ip, self.udp_port))
        except Exception as e:
            print(f"Ошибка отправки UNICAST: {e}")

    def send_multicast(self, message):
        try:
            if not self.in_multicast_group:
                print("Вы не в multicast группе! Используйте /join")
                return

            msg = f"MULTICAST:{self.username}:{message}"
            self.multicast_socket.sendto(
                msg.encode('utf-8'),
                (self.multicast_group, self.multicast_port)
            )
            print(f"[MULTICAST] Отправлено: {message}")
        except Exception as e:
            print(f"Ошибка отправки MULTICAST: {e}")

    def send_broadcast(self, message):
        try:
            msg = f"BROADCAST:{self.username}:{message}"

            self.unicast_socket.sendto(
                msg.encode('utf-8'),
                ('255.255.255.255', self.udp_port)
            )
            print(f"[BROADCAST] Отправлено: {message}")
        except Exception as e:
            print(f"Ошибка отправки BROADCAST: {e}")

    def send_discovery(self):
        discovery_msg = f"DISCOVERY:{self.username}:{self.local_ip}"
        try:
            self.unicast_socket.sendto(
                discovery_msg.encode('utf-8'),
                (self.network_info['broadcast_ip'], self.udp_port)
            )
            self.unicast_socket.sendto(
                discovery_msg.encode('utf-8'),
                ('255.255.255.255', self.udp_port)
            )
        except Exception as e:
            print(f"Ошибка отправки DISCOVERY: {e}")

    def receive_messages(self):
        while self.running:
            try:
                self.unicast_socket.settimeout(1.0)
                try:
                    data, addr = self.unicast_socket.recvfrom(1024)
                    if addr[0] != self.local_ip:
                        self.process_message(data.decode('utf-8'), addr[0])
                except socket.timeout:
                    pass

                if self.in_multicast_group:
                    self.multicast_socket.settimeout(1.0)
                    try:
                        data, addr = self.multicast_socket.recvfrom(1024)
                        if addr[0] != self.local_ip and addr[0] not in self.ignored_peers:
                            self.process_message(data.decode('utf-8'), addr[0])
                    except socket.timeout:
                        pass

            except Exception as e:
                if self.running:
                    print(f"Ошибка приема: {e}")

    def process_message(self, message, sender_ip):
        if sender_ip in self.ignored_peers:
            return

        try:
            parts = message.split(':', 2)
            if len(parts) < 3:
                return

            msg_type, username, content = parts
            timestamp = datetime.now().strftime("%H:%M:%S")

            if sender_ip != self.local_ip and sender_ip not in self.peers:
                self.peers[sender_ip] = {
                    'username': username,
                    'last_seen': time.time()
                }
                print(f"Обнаружен новый участник: {username} ({sender_ip})")

            if msg_type == "DISCOVERY":
                response = f"DISCOVERY_RESPONSE:{self.username}:{self.local_ip}"
                self.unicast_socket.sendto(response.encode('utf-8'), (sender_ip, self.udp_port))

            elif msg_type == "UNICAST":
                print(f"\n[{timestamp}] {username} ({sender_ip}): {content}")
                print("> ", end="", flush=True)

            elif msg_type == "MULTICAST":
                print(f"\n[{timestamp}] [MULTICAST] {username} ({sender_ip}): {content}")
                print("> ", end="", flush=True)

            elif msg_type == "BROADCAST":
                print(f"\n[{timestamp}] [BROADCAST] {username} ({sender_ip}): {content}")
                print("> ", end="", flush=True)

            if sender_ip in self.peers:
                self.peers[sender_ip]['last_seen'] = time.time()

        except Exception as e:
            print(f"Ошибка обработки сообщения: {e}")

    def cleanup_peers(self):
        while self.running:
            current_time = time.time()
            inactive_peers = []

            for ip, info in self.peers.items():
                if current_time - info['last_seen'] > 30:  
                    inactive_peers.append(ip)

            for ip in inactive_peers:
                print(f"Участник {self.peers[ip]['username']} ({ip}) отключился")
                del self.peers[ip]

            time.sleep(10)

    def list_peers(self):
        print(f"\n=== АКТИВНЫЕ УЧАСТНИКИ ({len(self.peers)}) ===")
        print(f"Вы: {self.username} ({self.local_ip})")

        if self.peers:
            for ip, info in self.peers.items():
                status = "ИГНОРИРУЕТСЯ" if ip in self.ignored_peers else "АКТИВЕН"
                last_seen = time.time() - info['last_seen']
                print(f"{info['username']} ({ip}) - {status} ({int(last_seen)} сек назад)")
        else:
            print("Другие участники не найдены")

        print(f"Multicast группа: {'ПОДКЛЮЧЕН' if self.in_multicast_group else 'ОТКЛЮЧЕН'}")
        print("---")

    def run(self):
        receive_thread = threading.Thread(target=self.receive_messages, daemon=True)
        cleanup_thread = threading.Thread(target=self.cleanup_peers, daemon=True)
        receive_thread.start()
        cleanup_thread.start()

        def discovery_loop():
            while self.running:
                self.send_discovery()
                time.sleep(15)  

        discovery_thread = threading.Thread(target=discovery_loop, daemon=True)
        discovery_thread.start()

        print("Чат готов к работе. Введите сообщение или команду:")

        while self.running:
            try:
                user_input = input("> ").strip()

                if not user_input:
                    continue

                if user_input.startswith('/'):
                    if user_input == '/quit':
                        break
                    elif user_input == '/list':
                        self.list_peers()
                    elif user_input.startswith('/ignore '):
                        ip = user_input.split(' ', 1)[1]
                        if ip in self.peers:
                            self.ignored_peers.add(ip)
                            print(f"Игнорируем участника {ip}")
                        else:
                            print("Участник не найден")
                    elif user_input.startswith('/unignore '):
                        ip = user_input.split(' ', 1)[1]
                        if ip in self.ignored_peers:
                            self.ignored_peers.remove(ip)
                            print(f"Снято игнорирование с {ip}")
                        else:
                            print("Участник не в списке игнорирования")
                    elif user_input == '/multicast':
                        msg = input("MULTICAST сообщение: ")
                        self.send_multicast(msg)
                    elif user_input == '/broadcast':
                        msg = input("BROADCAST сообщение: ")
                        self.send_broadcast(msg)
                    elif user_input == '/join':
                        self.join_multicast_group()
                    elif user_input == '/leave':
                        self.leave_multicast_group()
                    elif user_input == '/discovery':
                        self.send_discovery()
                        print("Запрос обнаружения отправлен")
                    else:
                        print(
                            "Неизвестная команда. Доступные: /list, /ignore, /unignore, /multicast, /broadcast, /join, /leave, /quit")
                else:
                    self.send_unicast(user_input)
                    print(f"[Вы]: {user_input}")

            except KeyboardInterrupt:
                print("\nЗавершение работы...")
                break
            except Exception as e:
                print(f"Ошибка: {e}")

        self.running = False
        self.leave_multicast_group()
        self.unicast_socket.close()
        self.multicast_socket.close()
        print("Чат завершен")


if __name__ == "__main__":
    chat = PeerToPeerChat()
    chat.run()