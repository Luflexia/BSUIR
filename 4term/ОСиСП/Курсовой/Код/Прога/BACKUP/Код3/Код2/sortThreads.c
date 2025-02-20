#include "sortThreads.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Функция сравнения для сортировки потоков по PID
int compare_thread_pid(const void *a, const void *b) {
    const Thread *threadA = (const Thread *)a;
    const Thread *threadB = (const Thread *)b;
    return threadA->pid - threadB->pid;
}

// Функция сравнения для сортировки потоков по пользователю
int compare_thread_user(const void *a, const void *b) {
    const Thread *threadA = (const Thread *)a;
    const Thread *threadB = (const Thread *)b;
    return strcmp(threadA->user, threadB->user);
}

// Функция сравнения для сортировки потоков по потреблению виртуальной памяти
int compare_thread_virtual_memory(const void *a, const void *b) {
    const Thread *threadA = (const Thread *)a;
    const Thread *threadB = (const Thread *)b;
    return threadA->virtual_memory - threadB->virtual_memory;
}

// Функция сравнения для сортировки потоков по потреблению физической памяти
int compare_thread_resident_memory(const void *a, const void *b) {
    const Thread *threadA = (const Thread *)a;
    const Thread *threadB = (const Thread *)b;
    return threadA->resident_memory - threadB->resident_memory;
}

// Функция сравнения для сортировки потоков по использованию процессора
int compare_thread_cpu_usage(const void *a, const void *b) {
    const Thread *threadA = (const Thread *)a;
    const Thread *threadB = (const Thread *)b;
    return (int)(threadA->cpu_usage - threadB->cpu_usage);
}

// Функция для сортировки потоков по PID
void sort_threads_by_pid(Thread *threads, int num_threads) {
    qsort(threads, num_threads, sizeof(Thread), compare_thread_pid);
}

// Функция для сортировки потоков по пользователю
void sort_threads_by_user(Thread *threads, int num_threads) {
    qsort(threads, num_threads, sizeof(Thread), compare_thread_user);
}

// Функция для сортировки потоков по потреблению виртуальной памяти
void sort_threads_by_virtual_memory(Thread *threads, int num_threads) {
    qsort(threads, num_threads, sizeof(Thread), compare_thread_virtual_memory);
}

// Функция для сортировки потоков по потреблению физической памяти
void sort_threads_by_resident_memory(Thread *threads, int num_threads) {
    qsort(threads, num_threads, sizeof(Thread), compare_thread_resident_memory);
}

// Функция для сортировки потоков по использованию процессора
void sort_threads_by_cpu_usage(Thread *threads, int num_threads) {
    qsort(threads, num_threads, sizeof(Thread), compare_thread_cpu_usage);
}
