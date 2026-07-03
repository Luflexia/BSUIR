import os
import socket
import uuid
from datetime import datetime
import time  


def welcome(client):
    welcome_message = """
    Welcome!
    The list of commands you can use:
    - ECHO <message>: returns "message";
    - TIME: returns the current server time;
    - UPLOAD <filename>: uploads a file to the server;
    - DOWNLOAD <filename>: downloads a file from the server;
    - QUIT: closes the connection.\n"""

    client.sendall(welcome_message.encode('utf-8'))


def echo(message):
    
    command_parts = message.decode('utf-8').split(' ', 1)
    return command_parts[1].encode('utf-8')


def get_time():
    current_time = datetime.now().strftime("%H:%M:%S")
    return current_time.encode('utf-8')


def upload(client, message):
    parts = message.decode('utf-8').split(' ', 1)
    filename = parts[1]
    file_size = int(client.recv(1024).decode('utf-8'))
    received_size = 0

    if os.path.exists(filename + '.temp'):
        print(f"The client has continued to upload {filename}.")
        os.rename(filename + '.temp', filename)
        received_size = os.path.getsize(filename)
        client.sendall(str(received_size).encode('utf-8'))
    else:
        client.sendall(str(received_size).encode('utf-8'))

    start_time = time.time()  

    with open(filename, "ab" if received_size > 0 else "wb") as file:
        while received_size < file_size:
            file_data = client.recv(1024)
            if not file_data:
                print("\nUploading was interrupted.")
                os.rename(filename, filename + '.temp')
                return "File wasn't uploaded.".encode('utf-8')
            file.write(file_data)
            received_size += len(file_data)

    end_time = time.time()  
    elapsed_time = end_time - start_time  
    speed = received_size / elapsed_time  

    if speed < 1024:
        speed_str = f"{speed:.2f} B/s"
    elif speed < 1024 * 1024:
        speed_str = f"{speed / 1024:.2f} KB/s"
    else:
        speed_str = f"{speed / (1024 * 1024):.2f} MB/s"

    print(f"File '{filename}' received")
    print(f"File '{filename}' saved")
    print(f"Upload speed: {speed_str}")

    return "File uploaded successfully".encode('utf-8')


def download(client, message):
    parts = message.decode('utf-8').split(' ', 1)
    filename = parts[1]
    file_size = 0

    if os.path.exists(filename):
        with open(filename, "rb") as file:
            file_size = os.path.getsize(filename)
            client.sendall(str(file_size).encode('utf-8'))

            sent_size = int(client_socket.recv(1024).decode('utf-8'))
            if sent_size != 0:
                print("The download continues")
                file.seek(sent_size)

            start_time = time.time()  

            while True:
                file_data = file.read(1024)
                if not file_data:
                    break
                try:
                    client.sendall(file_data)
                except BrokenPipeError:
                    print("\nDownloading was interrupted.")
                    return "File wasn't downloaded.".encode('utf-8')

            end_time = time.time()  
            elapsed_time = end_time - start_time  
            speed = file_size / elapsed_time  

            if speed < 1024:
                speed_str = f"{speed:.2f} B/s"
            elif speed < 1024 * 1024:
                speed_str = f"{speed / 1024:.2f} KB/s"
            else:
                speed_str = f"{speed / (1024 * 1024):.2f} MB/s"

        print(f"File '{filename}' sent")
        print(f"Download speed: {speed_str}")

        client_response = client.recv(1024).decode('utf-8')
        if client_response == "File received":
            return "File downloaded successfully".encode('utf-8')
        else:
            return "File wasn't downloaded.".encode('utf-8')
    else:
        return str(file_size).encode('utf-8')


def remove_temp_files():
    for filename in os.listdir():
        if filename.endswith('.temp'):
            os.remove(filename)


server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

host = input("Enter server IP address: ")
port = int(input("Enter server port: "))
server_address = (host, port)

session_ids = {}

server_socket.bind(server_address)

server_socket.listen(5)

print(f"Server is listening on {host}:{port}")

while True:
    print("\nWaiting for connection...")
    client_socket, client_address = server_socket.accept()
    print(f"\nConnection accepted from {client_address}")

    try:
        if client_address[0] not in session_ids:
            remove_temp_files()
            print("Temporary files have been removed for a new client")

        session_id = str(uuid.uuid4())
        session_ids[client_address[0]] = session_id

        welcome(client_socket)

        while True:
            data = client_socket.recv(1024)
            print(f"Data received: {data.decode('utf-8')}")

            if not data:
                print("\nConnection is lost.")
                client_socket.close()
                break

            if data.startswith(b'ECHO'):
                response = echo(data)
                client_socket.sendall(response)

            elif data.startswith(b'TIME'):
                response = get_time()
                client_socket.sendall(response)

            elif data.startswith(b'UPLOAD'):
                response = upload(client_socket, data)
                if response == "File wasn't uploaded.".encode('utf-8'):
                    client_socket.close()
                    break
                client_socket.sendall(response)

            elif data.startswith(b'DOWNLOAD'):
                response = download(client_socket, data)
                if response == "File wasn't downloaded.".encode('utf-8'):
                    client_socket.close()
                    break
                client_socket.sendall(response)

            elif data.startswith(b'QUIT'):
                print("Closing the connection")
                client_socket.close()
                break

            else:
                client_socket.sendall(b'Invalid command.\n')

    except Exception as e:
        print(f"\nException occurred: {e}.")
        client_socket.close()

        print("Shutting down the server")
        remove_temp_files()
        server_socket.close()
        break