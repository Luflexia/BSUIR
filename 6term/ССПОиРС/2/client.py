import socket
import os
import time
import signal
import select

SERVER_ADDRESS = "172.20.10.5"
SERVER_PORT = 12345
BIND_ADDRESS = "172.20.10.4"
BIND_PORT = 55555
CHECK_TIMER = 5
FRAME_SIZE = 65000
WINDOW_SIZE = 10
MAX_TIMEOUT = 0.12
MIN_TIMEOUT = 0.08

exitFlag = False
validInput = True


def reconnectPrompt():
    print("\nСоединение разорвано.",
          "\nХотите попробовать восстановить соединение? [y/n]")

    answer = input()
    if (answer[0] == "Y") or (answer[0] == "y"):
        return True

    return False


def upload(filePath):
    _, _ = clientSocket.recvfrom(4)
    if not os.path.exists(filePath):
        clientSocket.sendto("0".encode(), (SERVER_ADDRESS, SERVER_PORT))
        clientSocket.sendto(("Файл \"" + filePath + "\" не найден.").encode(), (SERVER_ADDRESS, SERVER_PORT))
        return clientSocket.recvfrom(FRAME_SIZE)[0].decode()

    clientSocket.sendto("1".encode(), (SERVER_ADDRESS, SERVER_PORT))

    fileSize = os.path.getsize(filePath)

    offset = int(clientSocket.recvfrom(FRAME_SIZE)[0].decode())
    clientSocket.sendto(str(fileSize).encode(), (SERVER_ADDRESS, SERVER_PORT))

    with open(filePath, 'rb') as file:
        startTime = time.time()
        uploadFile(file, offset, fileSize)
        endTime = time.time()
        file.close()

    speed = "{:.2f}".format((fileSize - offset) / (endTime - startTime) / 1024)
    print("Скорость загрузки:", speed, "КБ/с")

    return clientSocket.recvfrom(FRAME_SIZE)[0].decode()


def uploadFile(file, offset, fileSize):
    clientFrames = offset // (FRAME_SIZE - 4)
    serverFrames = 0

    timeout = MAX_TIMEOUT

    data = b''
    file.seek(offset, 0)

    while True:
        data = file.read(FRAME_SIZE - 4)
        clientSocket.sendto(clientFrames.to_bytes(4, byteorder='big') + data, (SERVER_ADDRESS, SERVER_PORT))
        clientFrames += 1

        if clientFrames % WINDOW_SIZE == 0 or clientFrames * (FRAME_SIZE - 4) >= fileSize:
            if select.select([clientSocket], [], [], timeout)[0]:
                serverFrames = clientSocket.recvfrom(FRAME_SIZE)[0].decode()
                timeout = timeout - 0.01 if timeout > MIN_TIMEOUT else MIN_TIMEOUT
            else:
                timeout = timeout + 0.005 if timeout < MAX_TIMEOUT else MAX_TIMEOUT

            if int(serverFrames) != clientFrames:
                file.seek((FRAME_SIZE - 4) * int(serverFrames), 0)
                clientFrames = int(serverFrames)

        if clientFrames * (FRAME_SIZE - 4) >= fileSize:
            break


def download(filePath):
    serverHasFile = clientSocket.recvfrom(1)[0].decode()
    if serverHasFile == "0":
        return clientSocket.recvfrom(FRAME_SIZE)[0].decode()

    mode = 'ab' if os.path.exists(filePath) else 'wb+'
    with open(filePath, mode) as file:
        offset = os.path.getsize(filePath)
        clientSocket.sendto(str(offset).encode(), (SERVER_ADDRESS, SERVER_PORT))
        fileSize = int(clientSocket.recvfrom(FRAME_SIZE)[0].decode())
        file.seek(0, os.SEEK_END)

        startTime = time.time()
        downloadFile(file, offset, fileSize)
        endTime = time.time()
        file.close()

    speed = "{:.2f}".format((fileSize - offset) / (endTime - startTime) / 1024)
    print("Скорость загрузки:", speed, "КБ/с")
    consume()

    return "Файл передан успешно."


def consume():
    while select.select([clientSocket], [], [], 1)[0]:
        result, _ = clientSocket.recvfrom(FRAME_SIZE)


def downloadFile(file, offset, fileSize):
    clientFrames = offset // (FRAME_SIZE - 4)
    serverFrames = 0

    data = b''
    i = 0
    buffer = {}

    while fileSize >= offset:
        frame, _ = clientSocket.recvfrom(FRAME_SIZE)

        serverFrames = int.from_bytes(frame[:4], byteorder='big')
        data = frame[4:]
        i += 1

        buffer[serverFrames] = data
        if (i == WINDOW_SIZE or offset + i * (FRAME_SIZE - 4) >= fileSize):
            buffer = dict(buffer.items())
            for _, frameData in buffer.items():
                file.write(frameData)
                clientFrames += 1
                offset += FRAME_SIZE - 4

            buffer = {}
            clientSocket.sendto(str(clientFrames).encode(), (SERVER_ADDRESS, SERVER_PORT))
            i = 0


def _exit():
    global exitFlag
    exitFlag = True


def otherCommand():
    return clientSocket.recvfrom(FRAME_SIZE)[0].decode()


def handleCommand(userInput):
    command, argument = userInput.partition(" ")[::2]

    match (command.lower()):
        case "upload":
            clientSocket.settimeout(None)
            response = upload(argument)
            clientSocket.settimeout(10)

        case "download":
            clientSocket.settimeout(None)
            response = download(argument)
            clientSocket.settimeout(10)

        case _:
            response = otherCommand()

    if (userInput.lower() == "exit"):
        _exit()

    return response


def confirmConnection():
    clientSocket.sendto("CONN".encode(), (SERVER_ADDRESS, SERVER_PORT))

    try:
        clientSocket.settimeout(3)
        data = clientSocket.recvfrom(FRAME_SIZE)[0].decode()

        if (data != "ACK, CONN"):
            raise Exception

    except (Exception, socket.timeout):
        print("Сервер недоступен!\n")
        exit()


def confirmExit():
    clientSocket.sendto("CONN_END".encode(), (SERVER_ADDRESS, SERVER_PORT))


def createSocket():
    clientSocket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    clientSocket.bind((BIND_ADDRESS, BIND_PORT))

    return clientSocket


def _input(str):
    signal.alarm(CHECK_TIMER)
    msg = input(str)
    if msg != "":
        signal.alarm(0)

    return msg


def alarm():
    signal.alarm(0)
    raise Exception


# ------------

try:
    signal.signal(signal.SIGALRM, alarm)
    clientSocket = createSocket()
    confirmConnection()
    print("Соединение установлено.\n")

    while not exitFlag:
        try:
            try:
                if validInput:
                    userInput = _input("> ")
                else:
                    userInput = _input('')
                validInput = True

                if userInput == "":
                    continue
            except Exception:
                validInput = False
                clientSocket.sendto(b'CONN_CHECK', (SERVER_ADDRESS, SERVER_PORT))
                continue

            clientSocket.sendto(userInput.encode(), (SERVER_ADDRESS, SERVER_PORT))
            clientSocket.settimeout(10)
            response = handleCommand(userInput)
            print(response, "\n")

        except (socket.error, socket.timeout):
            clientSocket.close()
            try:
                if (reconnectPrompt()):
                    clientSocket = createSocket()
                    confirmConnection()
                    print("Соединение восстановлено.\n")
                    validInput = True
                else:
                    exitFlag = True
            except socket.error:
                print("Не удалось восстановить подключение.")
                exitFlag = True

except KeyboardInterrupt:
    confirmExit()
    print("\nЗавершение работы.\n")

finally:
    try:
        clientSocket.close()
    except NameError:
        pass