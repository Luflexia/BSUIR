#ifndef PROCESSES_H
#define PROCESSES_H
#include "threads.h"

#define MAX_THREADS_PER_PROCESS 100

typedef struct {
    int pid;                  // ID процесса
    char user[50];            // Имя пользователя вызвавшего процесс
    char state;               // Состояние процесса
    double resident_memory;   // Потребление физической памяти (MB)
    double virtual_memory;    // Виртуальная память, используемая процессом (MB)
    int cpu_cores;            // Количество ядер процессора, используемых процессом
    int threads;              // Количество потоков
    char start_time[20];      // Дата и время запуска
    char command[100];        // Имя команды, запустившей процесс
} ProcessInfo;

typedef struct {
    ProcessInfo process_info;                    // Информация о процессе
    ThreadInfo threads[MAX_THREADS_PER_PROCESS]; // Информация о потоках процесса
    int thread_count;                            // Количество потоков процесса
} ProcessData;                                   // Структура для хранения информации о процессе и его потоках

int compare_by_pid(const void *a, const void *b);
int compare_by_resident_memory(const void *a, const void *b);
int compare_by_virtual_memory(const void *a, const void *b);

#endif // PROCESSES_H
