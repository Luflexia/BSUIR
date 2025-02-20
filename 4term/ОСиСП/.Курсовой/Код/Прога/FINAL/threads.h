#ifndef THREADS_H
#define THREADS_H

typedef struct {
    int tid;         // ID потока
    char state;      // Сстояние потока
    char name[16];   // Имя потока
} ThreadInfo;

#endif //THREADS_H
