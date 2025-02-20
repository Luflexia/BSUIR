#define _DEFAULT_SOURCE  // Определение макроса для использования расширений GNU
#define _POSIX_C_SOURCE  // Определение макроса для POSIX
#include <errno.h>  
#include <fcntl.h>  
#include <semaphore.h>  
#include <signal.h>  
#include <stdbool.h> 
 #include <stdint.h>  
#include <stdio.h> 
 #include <stdlib.h>  
#include <sys/mman.h>  
#include <sys/shm.h>  
#include <unistd.h>  

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
    sem_t* ConsumerSem = sem_open("/semconsumer", 0);
    if (ConsumerSem == SEM_FAILED) {  // Проверка на ошибки
        perror("Consumer sem_open");  // Вывод сообщения об ошибке
        return 1;  // Завершение программы с ошибкой
    }
    sem_t* MonoSem = sem_open("/semmono", 0);
    if (MonoSem == SEM_FAILED) {  // Проверка на ошибки
        perror("Mono sem_open");  // Вывод сообщения об ошибке
        return 1;  // Завершение программы с ошибкой
    }

    int fileDescriptor;  // Дескриптор файла для разделяемой памяти
    Ring* ring;  // Указатель на кольцевой буфер

    // Открытие разделяемой памяти
    if ((fileDescriptor = shm_open("/ringmem", O_RDWR, S_IRUSR | S_IWUSR)) == -1)
        perror("shm_open");
    if ((ring = mmap(NULL, sizeof(Ring), PROT_READ | PROT_WRITE, MAP_SHARED, fileDescriptor, 0)) == (void*)-1)
        perror("mmap");

    while (flag) {  // Основной цикл программы
        sem_wait(ConsumerSem);  // Ожидание освобождения семафора потребителя
        sem_wait(MonoSem);  // Ожидание освобождения монопольного семафора

        if (ring->count > 0) {  // Проверка наличия сообщений в буфере
            // Вывод сообщения на консоль
            printf("%s get message: %s\n\n", argv[0], ring->messages[ring->tail].data);
            ring->tail = (ring->tail + 1) % 10;  // Обновление индекса хвоста
            ring->get++;  // Увеличение счетчика полученных сообщений
            ring->count--;  // Уменьшение счетчика сообщений в буфере
        }

        sem_post(MonoSem);  // Освобождение монопольного семафора
        sem_post(ConsumerSem);  // Освобождение семафора потребителя

        sleep(2);  // Задержка на 2 секунды
    }

    munmap(ring, sizeof(Ring));  // Отображение буфера из памяти
    sem_close(ConsumerSem);  // Закрытие семафора потребителя
    sem_close(MonoSem);  // Закрытие монопольного семафора
    return 0;  // Завершение программы
}
