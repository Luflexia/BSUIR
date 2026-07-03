#ifndef THREADS_H
#define THREADS_H

#define MAX_THREADS 1000

typedef struct {
    int pid;                // Идентификатор процесса
    int tid;                // Идентификатор потока
    char user[50];          // Имя пользователя
    int priority;           // Приоритет потока
    long virtual_memory;    // Потребление виртуальной памяти (KB)
    long resident_memory;   // Потребление физической памяти (KB)
    float cpu_usage;        // % использования CPU
    char command[100];      // Название команды, инициализировавшей процесс
} Thread;

#endif /* THREADS_H */
