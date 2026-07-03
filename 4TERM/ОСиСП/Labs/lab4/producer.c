#define _POSIX_C_SOURCE  
#define _DEFAULT_SOURCE  
#include <stdio.h>  
#include <stdlib.h>  
#include <stdint.h>  
#include <sys/mman.h>  
#include <sys/shm.h>  
#include <fcntl.h>  
#include <unistd.h>  
#include <semaphore.h>  
#include <signal.h> 
#include <stdbool.h> 
#include <sys/random.h>  
#include <time.h>  

// Определение структуры сообщения
typedef struct _message {
    uint8_t type;  // Тип сообщения
    uint16_t hash;  // Хэш сообщения
    uint8_t size;  // Размер сообщения
    char data[259];  // Данные сообщения
} Message;

// Определение структуры кольцевого буфера
typedef struct _Ring {
    Message messages[10];  // Массив сообщений
    size_t head, tail, count, add, get;  // Индексы и счетчики
} Ring;

// Функция для вычисления хэша
uint16_t hash_16(const void *data, size_t len) {
    const uint8_t *bytes = data;
    uint16_t hash = 0xFFFF;
    const uint16_t fnv_prime = 0x0101;

    for (size_t i = 0; i < len; ++i) {
        hash = hash * fnv_prime;
        hash = hash ^ bytes[i];
    }
    return hash;
}

// Функция для генерации сообщения
Message generateMessage() {
    srand((unsigned)time(NULL));
    const char letters[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    Message message;

    uint16_t random;
    do {
        if (getrandom(&random, sizeof(random), 0) < 0) {  // Генерация случайного числа
            perror("getrandom");
            exit(1);
        }
        message.size = random = random % 257;  // Генерация размера сообщения
    } while (random == 0);

    for (int i = 0; i < random; i++) {  // Генерация случайных данных сообщения
        message.data[i] = letters[rand() % 52];
    }

    message.hash = hash_16(message.data, message.size);  // Вычисление хэша сообщения
    message.type = rand() % 256;  // Генерация типа сообщения
    return message;
}

bool flag = true;  // Флаг для завершения работы

// Обработчик сигнала SIGUSR1
void sig1_handler(int signo) {
    if (signo != SIGUSR1)  // Проверка типа сигнала
        return;
    flag = false;  // Установка флага завершения работы
}

int main(int argc, char** argv) {
    signal(SIGUSR1, sig1_handler);  // Установка обработчика сигнала

    // Открытие семафоров
    sem_t* ProducerSem = sem_open("/semproducer", 0);
    if (ProducerSem == SEM_FAILED) {  // Проверка на ошибки
        perror("Producer sem_open");
        return 1;
    }

    sem_t* MonoSem = sem_open("/semmono", 0);
    if (MonoSem == SEM_FAILED) {  // Проверка на ошибки
        perror("Mono sem_open");
        return 1;
    }

    int fileDescriptor;  // Дескриптор файла для разделяемой памяти
    Ring* ring;  // Указатель на кольцевой буфер

    // Открытие разделяемой памяти для создания общего кольцевого буфера, к которому имеют доступ и производители, и потребители сообщений
    
    if ((fileDescriptor = shm_open("/ringmem", O_RDWR, S_IRUSR | S_IWUSR)) == -1)
        perror("shm_open");

    if ((ring = mmap(NULL, sizeof(Ring), PROT_READ | PROT_WRITE, MAP_SHARED, fileDescriptor, 0)) == (void*)-1)
        perror("mmap");

    while (flag) {  // Основной цикл программы
        sem_wait(ProducerSem);  // Ожидание освобождения семафора производителя
        sem_wait(MonoSem);  // Ожидание освобождения монопольного семафора

        if (ring->count < 10) {  // Проверка наличия свободного места в буфере
            Message message = generateMessage();  // Генерация нового сообщения

            // Запись сообщения в буфер
            ring->messages[ring->head] = message;
            ring->head = (ring->head + 1) % 10;  // Обновление индекса головы
            ring->add++;  // Увеличение счетчика добавленных сообщений
            ring->count++;  // Увеличение счетчика сообщений в буфере

            // Вывод информации о добавленном сообщении на консоль
            printf("%s put message: %s\n\n", argv[0], message.data);
        }

        sem_post(MonoSem);  // Освобождение монопольного семафора
        sem_post(ProducerSem);  // Освобождение семафора производителя

        sleep(2);  // Задержка на 2 секунды
    }

    munmap(ring, sizeof(Ring));  // Отображение буфера из памяти
    sem_close(ProducerSem);  // Закрытие семафора производителя
    sem_close(MonoSem);  // Закрытие монопольного семафора
    return 0;  // Завершение программы
}
