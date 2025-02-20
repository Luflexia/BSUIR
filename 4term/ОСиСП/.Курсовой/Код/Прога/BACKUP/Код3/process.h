#ifndef PROCESS_H
#define PROCESS_H

#define MAX_PROC 1000

typedef struct {
    int pid;                // Идентификатор процесса
    char user[50];          // Имя пользователя
    int priority;           // Приоритет процесса
    long virtual_memory;    // Потребление виртуальной памяти (KB)
    long resident_memory;   // Потребление физической памяти (KB)
    float cpu_usage;        // % использования CPU
    char command[100];      // Название команды, инициализировавшей процесс
} Process;

#endif /* PROCESS_H */
