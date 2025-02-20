#define _POSIX_C_SOURCE 199506L  // Установка стандарта POSIX
#include <errno.h> 
#include <pthread.h>  
#include <semaphore.h>  
#include <signal.h>  
#include <stdbool.h> 
#include <stdint.h>  
#include <stdio.h> 
 #include <stdlib.h>  
#include <string.h> 
#include <sys/ipc.h> 
#include <sys/random.h> 
 #include <sys/types.h>  
#include <unistd.h> 

#define max 10  // Определение максимального количества потоков
#define MAX_RING_SIZE 256  // Определение максимального размера кольцевого буфера

// Определение структуры сообщения
typedef struct _message {
    uint8_t type;  // Тип сообщения
    uint16_t hash;  // Хэш сообщения
    uint88_t size;  // Размер сообщения
    char data[259];  // Данные сообщения
} Message;

// Определение структуры кольцевого буфера
typedef struct _Ring {
    Message messages[MAX_RING_SIZE];  // Массив сообщений
    int head, tail, count, add, get;  // Индексы и счетчики
} Ring;

// Функция для вычисления хэша
uint16_t hash_16(const void* data, size_t len) {
    const uint8_t* bytes = data;
    uint16_t hash = 0xFFFF;
    const uint16_t fnv_prime = 0x0101;

    for (size_t i = 0; i < len; ++i) {
        hash = hash * fnv_prime;  // Обновление хэша
        hash = hash ^ bytes[i];  // XOR с текущим байтом данных
    }

    return hash;  // Возврат вычисленного хэша
}

// Функция для генерации сообщения
Message generateMessage() {
    srand((unsigned)time(NULL));  // Инициализация генератора случайных чисел
    const char letters[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";  // Массив символов для данных сообщения

    Message message;  // Создание сообщения
    {
        uint16_t random;
        do {
            if (getrandom(&random, sizeof(random), 0) < 0) {  // Генерация случайного числа
                perror("getrandom");
                exit(1);
            }
            message.size = random = random % 257;  // Генерация размера сообщения
            if (random != 0)
                break;  // Выход из цикла, если размер сообщения не равен нулю
        } while (1);

        for (int i = 0; i < random; message.data[i] = letters[rand() % 53], i++);  // Генерация случайных данных сообщения
    }

    message.hash = hash_16(message.data, message.size);  // Вычисление хэша сообщения
    message.type = rand() % 256;  // Генерация типа сообщения

    return message;  // Возврат сгенерированного сообщения
}

Ring ring;  // Создание кольцевого буфера
sem_t fill_sem, empty_sem;  // Определение семафоров
pthread_mutex_t ring_mutex;  // Определение мьютекса
int num_prod_threads = 0, num_cons_threads = 0;  // Счетчики потоков производителей и потребителей
pthread_t consumer_threads[max];  // Массив потоков потребителей
pthread_t producer_threads[max];  // Массив потоков производителей
int ring_size = 10;  // Начальный размер кольцевого буфера
char input[3] = {'\0'};  // Переменная для ввода команд
bool wh = true;  // Флаг для завершения программы

// Функция для работы потока производителя
void* producer(void* arg) {
    int thread_id = *(int*)arg;  // Получение идентификатора потока
    while (1) {
        sem_wait(&fill_sem);  // Ожидание семафора заполнения
        pthread_mutex_lock(&ring_mutex);  // Захват мьютекса

        if (ring.count != ring_size) {  // Проверка, есть ли место в буфере
            ring.messages[ring.head] = generateMessage();  // Генерация сообщения

            // Вывод информации о созданном сообщении
            printf("Producer %d: Create message %s\n", thread_id, ring.messages[ring.head].data);

            ring.head = (ring.head + 1) % ring_size;  // Обновление индекса головы
            ring.count++;  // Увеличение счетчика сообщений в буфере
            ring.add++;  // Увеличение счетчика добавленных сообщений
        }

        pthread_mutex_unlock(&ring_mutex);  // Освобождение мьютекса
        sem_post(&empty_sem);  // Освобождение семафора пустоты
        sleep(3);  // Задержка
    }
    return NULL;  // Завершение потока
}

// Функция для работы потока потребителя
void* consumer(void* arg) {
    int thread_id = *(int*)arg;  // Получение идентификатора потока
    while (1) {
        sem_wait(&empty_sem);  // Ожидание семафора пустоты
        pthread_mutex_lock(&ring_mutex);  // Захват мьютекса

        if (ring.count > 0) {  // Проверка, есть ли сообщения в буфере
            Message message = ring.messages[ring.tail];  // Извлечение сообщения

            ring.tail = (ring.tail + 1) % ring_size;  // Обновление индекса хвоста
            ring.count--;  // Уменьшение счетчика сообщений в буфере
            ring.get++;  // Увеличение счетчика полученных сообщений

            // Вывод информации об извлеченном сообщении
            printf("Consumer %d: Extracted message %s\n", thread_id, message.data);
        }

        pthread_mutex_unlock(&ring_mutex);  // Освобождение мьютекса
        sem_post(&fill_sem);  // Освобождение семафора заполнения
        sleep(3);  // Задержка
    }
    return NULL;  // Завершение потока
}

// Функция для увеличения размера кольцевого буфера
void increase_size() {
    if (ring_size < MAX_RING_SIZE) {  // Проверка, можно ли увеличить размер буфера
        ring_size++;  // Увеличение размера буфера
        sem_post(&fill_sem);  // Освобождение семафора заполнения
        printf("Ring size increased to %d\n", ring_size);  // Вывод информации об увеличении размера
    } else {
        printf("Ring size cannot be increased\n");  // Вывод сообщения об ошибке увеличения размера
    }
} 

// Функция для уменьшения размера кольцевого буфера или завершения потока
void decrease_size() {
    if (input[1] == 'p' || input[1] == 'P') {  // Проверка, нужно ли завершить поток производителя
        if (num_prod_threads > 0) {  // Проверка, есть ли активные потоки производителей
            pthread_cancel(producer_threads[num_prod_threads - 1]);  // Завершение последнего созданного потока производителя
            pthread_join(producer_threads[num_prod_threads-- - 1], NULL);  // Ожидание завершения потока
        } else
            puts("Count of producers equals 0");  // Вывод сообщения об отсутствии активных потоков производителей
        return;
    } else if (input[1] == 'c' || input[1] == 'C') {  // Проверка, нужно ли завершить поток потребителя
        if (num_cons_threads > 0) {  // Проверка, есть ли активные потоки потребителей
            pthread_cancel(consumer_threads[num_cons_threads - 1]);  // Завершение последнего созданного потока потребителя
            pthread_join(consumer_threads[num_cons_threads-- - 1], NULL);  // Ожидание завершения потока
        } else
            puts("Count of consumers equals 0");  // Вывод сообщения об отсутствии активных потоков потребителей
        return;
    }

    if (ring_size > 1) {  // Проверка, можно ли уменьшить размер буфера
        ring_size--;  // Уменьшение размера буфера
        sem_wait(&fill_sem);  // Ожидание семафора заполнения
        printf("Ring size decreased to %d\n", ring_size);  // Вывод информации об уменьшении размера
    } else {
        printf("Ring size cannot be decreased\n");  // Вывод сообщения об ошибке уменьшения размера
    }
}

// Функция для создания потока производителя
void create_producer() {
    if (num_prod_threads < max) {  // Проверка, можно ли создать еще один поток производителя
        if (pthread_create(&producer_threads[++num_prod_threads - 1], NULL, producer, &num_prod_threads) != 0)
            perror("Producer_pthread_create error");  // Создание потока производителя и проверка на ошибки
    }
}

// Функция для создания потока потребителя
void create_consumer() {
    if (num_cons_threads < max) {  // Проверка, можно ли создать еще один поток потребителя
        if (pthread_create(&consumer_threads[++num_cons_threads - 1], NULL, consumer, &num_cons_threads) != 0)
            perror("Consumer_pthread_create error");  // Создание потока потребителя и проверка на ошибки
    }
}

// Функция для отображения статистики
void stat() {
    pthread_mutex_lock(&ring_mutex);  // Захват мьютекса
    printf("---Stat---\nRing size: %d\nCount: %d\nAdded: %d\nGetted: %d\nProducers count: %d\nConsumers count: %d\n", ring_size, ring.count, ring.add, ring.get, num_prod_threads, num_cons_threads);  // Вывод статистики
    pthread_mutex_unlock(&ring_mutex);  // Освобождение мьютекса
}

// Функция для завершения всех потоков и завершения программы
void quit() {
    while (num_prod_threads) {  // Завершение всех потоков производителей
        pthread_cancel(producer_threads[num_prod_threads - 1]);
        pthread_join(producer_threads[num_prod_threads-- - 1], NULL);
    };
    while (num_cons_threads) {  // Завершение всех потоков потребителей
        pthread_cancel(consumer_threads[num_cons_threads - 1]);
        pthread_join(consumer_threads[num_cons_threads-- - 1], NULL);
    }
    wh = false;  // Установка флага завершения программы
} 

// Функция инициализации
void init() {
    sem_init(&fill_sem, 0, ring_size);  // Инициализация семафора заполнения
    sem_init(&empty_sem, 0, 0);  // Инициализация семафора пустоты
    pthread_mutex_init(&ring_mutex, NULL);  // Инициализация мьютекса

    ring.head = 0;  // Инициализация головы буфера
    ring.tail = 0;  // Инициализация хвоста буфера
    ring.count = 0;  // Инициализация счетчика сообщений в буфере
    ring.add = 0;  // Инициализация счетчика добавленных сообщений
    ring.get = 0;  // Инициализация счетчика полученных сообщений
}

// Функция освобождения ресурсов
void dest() {
    sem_destroy(&fill_sem);  // Уничтожение семафора заполнения
    sem_destroy(&empty_sem);  // Уничтожение семафора пустоты
    pthread_mutex_destroy(&ring_mutex);  // Уничтожение мьютекса
}

// Главная функция программы
int main() {
    init();  // Инициализация ресурсов

    puts("P - create producer thread\nC - create consumer thread\n-(P/C) - end last created producer/consumer thread\n+/- - increase/decrease ring size\nS - show stat\nQ - end thread and exit\n");  // Вывод инструкции

    while (wh) {  // Основной цикл программы
        strcpy(input, "\0\0\0");  // Очистка ввода
        scanf("%s", input);  // Чтение ввода
        char key = input[0];  // Чтение команды
        if (key == '+')
            increase_size();  // Увеличение размера буфера
        else if (key == '-')
            decrease_size();  // Уменьшение размера буфера или завершение потока
        if (key == 'p')
            create_producer();  // Создание потока производителя
        else if (key == 'c')
            create_consumer();  // Создание потока потребителя
        if (key == 's')
            stat();  // Вывод статистики
        else if (key == 'q')
            quit();  // Завершение всех потоков и завершение программы
    }

    dest();  // Освобождение ресурсов

    return 0;  // Завершение программы
}
