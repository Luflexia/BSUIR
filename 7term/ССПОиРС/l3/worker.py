import socket
import numpy as np
import pickle

def receive_full_data(sock):
    """Функция для получения полного сообщения от сокета с учётом длины данных."""
    # Сначала получаем длину данных (8 байт)
    data_length = int.from_bytes(sock.recv(8), 'big')

    # Читаем данные согласно полученной длине
    data = b''
    while len(data) < data_length:
        part = sock.recv(4096)
        if not part:
            raise EOFError("Соединение прервано")
        data += part
    return data

def main():
    host = '0.0.0.0'  # Слушаем все интерфейсы
    port = 65434  # Порт для клиента
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.bind((host, port))
        s.listen()
        print(f"Вычислительный узел запущен на порту {port}...")

        while True:  # Бесконечный цикл для обработки соединений
            conn, addr = s.accept()
            with conn:
                print(f"Подключение от {addr}")
                # Получаем полное сообщение
                data = receive_full_data(conn)
                print(f"Данные получены от {addr}")
                
                try:
                    # Десериализация полученных данных
                    matrix_a, matrix_b = pickle.loads(data)

                    # Перемножение матриц
                    result = np.dot(matrix_a, matrix_b)

                    # Отправляем результат обратно
                    result_data = pickle.dumps(result)

                    # Отправляем сначала длину результата
                    conn.sendall(len(result_data).to_bytes(8, 'big'))

                    # Отправляем сам результат
                    print("Результат отправлен")
                    conn.sendall(result_data)
                except Exception as e:
                    print(f"Ошибка при обработке данных: {e}")

if __name__ == "__main__":
    main()

