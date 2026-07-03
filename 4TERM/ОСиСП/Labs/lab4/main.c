#define _POSIX_C_SOURCE  // Определение макроса для POSIX
#define _DEFAULT_SOURCE  // Определение макроса для использования расширений GNU
#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>  
#include <unistd.h>  
#include <sys/mman.h> 
#include <fcntl.h> 
#include <semaphore.h> 
#include <sys/stat.h> 
#include <sys/shm.h> 
#include <signal.h>  

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

int main() {
    // Инициализация дескрипторов процессов производителей и потребителей
    pid_t prod[10], consumer[10];
    size_t prod_count = 0, consumer_count = 0;  // Счетчики процессов

    // Создание и инициализация семафоров
    sem_t* ProducerSem = sem_open("/semproducer", O_CREAT, 0666, 1);
    sem_t* ConsumerSem = sem_open("/semconsumer", O_CREAT, 0666, 1);
    sem_t* MonoSem = sem_open("/semmono", O_CREAT, 0666, 1);

    // Проверка на ошибки при создании семафоров
    if (ProducerSem == SEM_FAILED || ConsumerSem == SEM_FAILED || MonoSem == SEM_FAILED) {
        perror("sem_open");
        return 1;
    }

    int fd = shm_open("/ringmem", O_CREAT | O_RDWR, 0666);  // Создание разделяемой памяти
    ftruncate(fd, sizeof(Ring));  // Установка размера разделяемой памяти
    Ring* ring = mmap(NULL, sizeof(Ring), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);  // Отображение разделяемой памяти

    memset(ring, 0, sizeof(Ring));  // Инициализация буфера

    char command;  // Переменная для хранения команды

    // Основной цикл программы
    while (1) {
        command = getchar();  // Чтение команды

        // Обработка команды
        switch (command) {
        case 'P':
        case 'p':
            // Создание процесса производителя
            prod[prod_count++] = fork();
            if (prod[prod_count - 1] == -1)
                perror("Producer fork");
            else if (prod[prod_count - 1] == 0) {
                char* child_argv[2];
                child_argv[0] = (char*)malloc(sizeof(char) * 13);
                child_argv[1] = NULL;
                sprintf(child_argv[0], "Producer_%ld", prod_count);
                if (execve("./producer", child_argv, NULL) == -1) {
                    perror("Producer execve");
                    free(child_argv[0]);
                    return 1;
                }
            }
            break;
        
        case 'C':
        case 'c':
            // Создание процесса потребителя
            consumer[consumer_count++] = fork();
            if (consumer[consumer_count - 1] == -1)
                perror("Consumer fork");
            else if (consumer[consumer_count - 1] == 0) {
                char* child_argv[2];
                child_argv[0] = (char*)malloc(sizeof(char) * 13);
                child_argv[1] = NULL;
                sprintf(child_argv[0], "Consumer_%ld", consumer_count);
                if (execve("./consumer", child_argv, NULL) == -1) {
                    perror("Consumer execve");
                    free(child_argv[0]);
                    return 1;
                }
            }
            break;
        
        case 'S':
        case 's':
            // Вывод статистики
            sem_wait(MonoSem);
            printf("---Stat---\nMax count: 10\nCount: %ld\nAdded: %ld\nGetted: %ld\nProducers count: %ld\nConsumers count: %ld\n",
                   ring->count, ring->add, ring->get, prod_count, consumer_count);
            sem_post(MonoSem);
            break;

        case 'Q':
        case 'q':
            // Завершение работы
            while (prod_count > 0) kill(prod[--prod_count], SIGUSR1);
            while (consumer_count > 0) kill(consumer[--consumer_count], SIGUSR1);

            munmap(ring, sizeof(Ring));  // Отображение буфера из памяти
            close(fd);  // Закрытие дескриптора файла
            shm_unlink("/ringmem");  // Удаление разделяемой памяти
            sem_close(ProducerSem);  // Закрытие семафоров
            sem_close(ConsumerSem);
            sem_close(MonoSem);
            sem_unlink("/semmono");  // Удаление семафоров
            sem_unlink("/semproducer");
            sem_unlink("/semconsumer");
            return 0;
            break;

        default:
            break;
        }
    }

    return 0;  // Завершение программы
}
