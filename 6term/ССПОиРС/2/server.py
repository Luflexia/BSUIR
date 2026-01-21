import socket
import time
import os
import select

BIND_ADDRESS = "172.20.10.5"
BIND_PORT = 12345
CLIENT_ADDRESS = "172.20.10.4"
CLIENT_PORT = 55555
FRAME_SIZE = 65000
WINDOW_SIZE = 10
MAX_TIMEOUT = 0.12
MIN_TIMEOUT = 0.08


def reconnect():
    print("Соединение разорвано.\n")
    global lastClientAddr, connected
    connected = False

    serverSocket.settimeout(None)
    while (not connected):
        connected, clientAddr = confirmConnection()

    if clientAddr[0] == lastClientAddr[0]:
        print("Соединение восстановлено.\n")
    else:
        print("Обнаружено подключение\n", "Адрес:", clientAddr[0], "\n")
        lastClientAddr = clientAddr

    connected = True


def download(fileName):
    if not os.path.exists(fileName):
        serverSocket.sendto("0".encode(), (CLIENT_ADDRESS, CLIENT_PORT))
        return "Файл \"" + fileName + "\" не найден."

    serverSocket.sendto("1".encode(), (CLIENT_ADDRESS, CLIENT_PORT))

    fileSize = os.path.getsize(fileName)

    offset = int(serverSocket.recvfrom(FRAME_SIZE)[0].decode())
    serverSocket.sendto(str(fileSize).encode(), (CLIENT_ADDRESS, CLIENT_PORT))

    with open(fileName, 'rb') as file:
        downloadFile(file, offset, fileSize)
        file.close()

    return "Файл передан успешно."


def downloadFile(file, offset, fileSize):
    serverFrames = offset // (FRAME_SIZE - 4)
    clientFrames = 0

    timeout = MAX_TIMEOUT

    data = b''
    file.seek(offset, 0)

    while True:
        try:
            # Читаем данные из файла
            data = file.read(FRAME_SIZE - 4)
            # Отправляем фрейм
            serverSocket.sendto(serverFrames.to_bytes(4, byteorder='big') + data, (CLIENT_ADDRESS, CLIENT_PORT))
            serverFrames += 1

            if serverFrames % WINDOW_SIZE == 0 or serverFrames * (FRAME_SIZE - 4) >= fileSize:
                # Проверяем ответ от клиента
                ready = select.select([serverSocket], [], [], timeout)[0]

                if ready:
                    clientFrames = serverSocket.recvfrom(FRAME_SIZE)[0].decode()
                    timeout = timeout - 0.01 if timeout > MIN_TIMEOUT else MIN_TIMEOUT
                else:
                    timeout = timeout + 0.005 if timeout < MAX_TIMEOUT else MAX_TIMEOUT

                if int(clientFrames) != serverFrames:
                    file.seek((FRAME_SIZE - 4) * int(clientFrames), 0)
                    serverFrames = int(clientFrames)

            if serverFrames * (FRAME_SIZE - 4) >= fileSize:
                break
        except (socket.error, socket.timeout) as e:
            print(f"Ошибка при передаче данных: {e}")
            print("Попытка повторить передачу...")
            time.sleep(1)  # Задержка перед повторной попыткой
            continue  # Повторить передачу данных
        except Exception as e:
            break  # Если ошибка не связана с сокетом, завершить передачу


def upload(fileName):
    serverSocket.sendto(b"File", (CLIENT_ADDRESS, CLIENT_PORT))
    clientHasFile = serverSocket.recvfrom(1)[0].decode()
    if clientHasFile == "0":
        return serverSocket.recvfrom(FRAME_SIZE)[0].decode()

    mode = 'ab' if os.path.exists(fileName) else 'wb+'
    with open(fileName, mode) as file:
        offset = os.path.getsize(fileName)
        serverSocket.sendto(str(offset).encode(), (CLIENT_ADDRESS, CLIENT_PORT))
        fileSize = int(serverSocket.recvfrom(FRAME_SIZE)[0].decode())
        file.seek(0, os.SEEK_END)

        uploadFile(file, fileSize, offset)
        file.close()
        consume()

    return "Файл загружен успешно."


def consume():
    while select.select([serverSocket], [], [], 1)[0]:
        _, _ = serverSocket.recvfrom(FRAME_SIZE)


def uploadFile(file, fileSize, offset):
    serverFrames = offset // (FRAME_SIZE - 4)
    clientFrames = 0

    data = b''
    i = 0
    buffer = {}

    while fileSize >= offset:
        frame, _ = serverSocket.recvfrom(FRAME_SIZE)

        clientFrames = int.from_bytes(frame[:4], byteorder='big')
        data = frame[4:]
        i += 1

        buffer[clientFrames] = data
        if i == WINDOW_SIZE or offset + i * (FRAME_SIZE - 4) >= fileSize:
            buffer = dict(buffer.items())
            for _, frameData in buffer.items():
                file.write(frameData)
                serverFrames += 1
                offset += FRAME_SIZE - 4

            buffer = {}
            serverSocket.sendto(str(serverFrames).encode(), (CLIENT_ADDRESS, CLIENT_PORT))
            i = 0


def echo(data):
    return data


def _time():
    currentTime = time.asctime(time.localtime())
    return "Время сервера: " + currentTime


def _exit():
    global connected
    connected = False
    return "Отключение от сервера..."


def handleCommand(clientInput):
    command, argument = clientInput.partition(" ")[::2]

    match (command.lower()):
        case "echo":
            response = echo(argument)

        case "time":
            response = _time()

        case "download":
            serverSocket.settimeout(15)
            response = download(argument)
            serverSocket.settimeout(10)

        case "upload":
            serverSocket.settimeout(15)
            response = upload(argument)
            serverSocket.settimeout(10)

        case "exit":
            response = _exit()

        case _:
            response = "Такой команды не существует!"

    return response


def printLog(clientInput, response):
    print("Команда пользователя:", clientInput)
    print("Ответ:", response, "\n")


def confirmConnection():
    data, clientAddress = serverSocket.recvfrom(FRAME_SIZE)
    data = data.decode()
    if (data != "CONN"):
        return (False, None)

    serverSocket.sendto("ACK, CONN".encode(), (CLIENT_ADDRESS, CLIENT_PORT))

    return (True, clientAddress)


# ------------

try:
    connected = False
    lastClientAddr = None

    serverSocket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    serverSocket.bind((BIND_ADDRESS, BIND_PORT))

    print("Сервер начал работу.\n")

    while (True):
        connected, clientAddr = confirmConnection()
        if (not connected):
            continue

        print("Обнаружено подключение\n"
              "Адрес:", clientAddr[0], "\n")

        connected = True
        lastClientAddr = clientAddr

        while (connected):
            try:
                serverSocket.settimeout(10)
                clientInput = serverSocket.recvfrom(FRAME_SIZE)[0].decode()
                if (clientInput == "CONN_END"):
                    print("Соединение закрыто.\n")
                    _exit()
                    break
                if (clientInput == "CONN_CHECK"):
                    continue

                response = handleCommand(clientInput)
                serverSocket.sendto(response.encode(), (CLIENT_ADDRESS, CLIENT_PORT))
                printLog(clientInput, response)

                if (clientInput == "exit"):
                    print("Соединение закрыто.\n")
            except socket.error:
                reconnect()
        serverSocket.settimeout(None)

except KeyboardInterrupt:
    print("\nЗавершение работы.\n")
