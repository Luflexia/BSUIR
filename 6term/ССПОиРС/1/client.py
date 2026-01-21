import os
import socket
import time
import sys


def enable_keepalive(sock, after_idle_sec=5, interval_sec=2, max_fails=3):

    sock.setsockopt(socket.SOL_SOCKET, socket.SO_KEEPALIVE, 1)  # Включаем SO_KEEPALIVE

    if hasattr(socket, "TCP_KEEPIDLE") and hasattr(socket, "TCP_KEEPINTVL") and hasattr(socket, "TCP_KEEPCNT"):
        # Настройка для Linux
        sock.setsockopt(socket.IPPROTO_TCP, socket.TCP_KEEPIDLE, after_idle_sec)
        sock.setsockopt(socket.IPPROTO_TCP, socket.TCP_KEEPINTVL, interval_sec)
        sock.setsockopt(socket.IPPROTO_TCP, socket.TCP_KEEPCNT, max_fails)



def main():
    global parts
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    enable_keepalive(client_socket, after_idle_sec=5, interval_sec=2, max_fails=1)

    host = input("Enter server IP address: ")
    port = int(input("Enter server port: "))
    server_address = (host, port)

    try:
        client_socket.connect(server_address)
        print(f"Connected to the server {host}:{port}")

        welcome_message = client_socket.recv(1024)
        print(welcome_message.decode('utf-8'))

        while True:
            command = input("Enter a command: ").strip()

            if command.startswith('ECHO') or command.startswith('UPLOAD') or command.startswith('DOWNLOAD'):
                parts = command.split(' ', 1)
                if len(parts) != 2:
                    print(f"Invalid command format. Usage: {parts[0]} <message>")
                    continue

            try:
                client_socket.sendall(command.encode('utf-8'))

                if command.upper() == 'QUIT':
                    break

                elif command.startswith('UPLOAD'):
                    filename = parts[1]
                    if os.path.exists(filename):
                        with open(filename, "rb") as file:
                            file_size = os.path.getsize(filename)
                            client_socket.sendall(str(file_size).encode('utf-8'))

                            sent_size = int(client_socket.recv(1024).decode('utf-8'))
                            if sent_size != 0:
                                print("The upload continues")
                            file.seek(sent_size)

                            start_time = time.time()

                            while True:
                                data = file.read(1024)
                                if not data:
                                    break
                                client_socket.sendall(data)

                            end_time = time.time()
                            elapsed_time = end_time - start_time
                            speed = file_size / elapsed_time


                            if speed < 1024:
                                speed_str = f"{speed:.2f} B/s"
                            elif speed < 1024 * 1024:
                                speed_str = f"{speed / 1024:.2f} KB/s"
                            else:
                                speed_str = f"{speed / (1024 * 1024):.2f} MB/s"

                            print(f"Upload speed: {speed_str}")

                            response = client_socket.recv(1024)
                            print(response.decode('utf-8'))
                    else:
                        print("File not found.")

                elif command.startswith('DOWNLOAD'):
                    filename = parts[1]
                    file_size = int(client_socket.recv(1024).decode('utf-8'))
                    if file_size == 0:
                        print("File not found on the server.")
                        continue

                    received_size = 0

                    if os.path.exists(filename):
                        print(f"The server has continued to send {filename}.")
                        received_size = os.path.getsize(filename)
                        client_socket.sendall(str(received_size).encode('utf-8'))
                    else:
                        client_socket.sendall(str(received_size).encode('utf-8'))

                    start_time = time.time()

                    with open(filename, "ab" if received_size > 0 else "wb") as file:
                        while received_size < file_size:
                            file_data = client_socket.recv(1024)
                            file.write(file_data)
                            received_size += len(file_data)

                    end_time = time.time()
                    elapsed_time = end_time - start_time
                    speed = file_size / elapsed_time


                    if speed < 1024:
                        speed_str = f"{speed:.2f} B/s"
                    elif speed < 1024 * 1024:
                        speed_str = f"{speed / 1024:.2f} KB/s"
                    else:
                        speed_str = f"{speed / (1024 * 1024):.2f} MB/s"

                    print(f"Download speed: {speed_str}")

                    client_socket.sendall("File received".encode('utf-8'))

                    response = client_socket.recv(1024)
                    print(response.decode('utf-8'))

                else:
                    response = client_socket.recv(1024)
                    print(response.decode('utf-8'))

            except BrokenPipeError:
                print("Connection to the server was lost.")
                break
            except ConnectionResetError:
                print("Connection was reset by the server.")
                break
            except Exception as e:
                print(f"An error occurred: {e}")
                break

    except Exception as e:
        print(f"Failed to connect to the server: {e}")
    finally:
        print("\nClosing the connection")
        client_socket.close()


if __name__ == "__main__":
    main()