#ifndef SORTTHREADS_H
#define SORTTHREADS_H

#include "process.h"

// Функция для сортировки потоков по PID
void sort_threads_by_pid(Thread *threads, int num_threads);

// Функция для сортировки потоков по пользователю
void sort_threads_by_user(Thread *threads, int num_threads);

// Функция для сортировки потоков по потреблению виртуальной памяти
void sort_threads_by_virtual_memory(Thread *threads, int num_threads);

// Функция для сортировки потоков по потреблению физической памяти
void sort_threads_by_resident_memory(Thread *threads, int num_threads);

// Функция для сортировки потоков по использованию процессора
void sort_threads_by_cpu_usage(Thread *threads, int num_threads);

#endif /* SORTTHREADS_H */
