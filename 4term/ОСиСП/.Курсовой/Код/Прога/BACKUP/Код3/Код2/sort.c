#include "sort.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Функция сравнения для сортировки по PID
int compare_pid(const void *a, const void *b) {
    const Process *processA = (const Process *)a;
    const Process *processB = (const Process *)b;
    return processA->pid - processB->pid;
}

// Функция сравнения для сортировки по пользователю
int compare_user(const void *a, const void *b) {
    const Process *processA = (const Process *)a;
    const Process *processB = (const Process *)b;
    return strcmp(processA->user, processB->user);
}

// Функция сравнения для сортировки по потреблению виртуальной памяти
int compare_virtual_memory(const void *a, const void *b) {
    const Process *processA = (const Process *)a;
    const Process *processB = (const Process *)b;
    return processA->virtual_memory - processB->virtual_memory;
}

// Функция сравнения для сортировки по потреблению физической памяти
int compare_resident_memory(const void *a, const void *b) {
    const Process *processA = (const Process *)a;
    const Process *processB = (const Process *)b;
    return processA->resident_memory - processB->resident_memory;
}

// Функция сравнения для сортировки по использованию процессора
int compare_cpu_usage(const void *a, const void *b) {
    const Process *processA = (const Process *)a;
    const Process *processB = (const Process *)b;
    return (int)(processA->cpu_usage - processB->cpu_usage);
}

// Функция для сортировки процессов по PID
void sort_processes_by_pid(Process *processes, int num_processes) {
    qsort(processes, num_processes, sizeof(Process), compare_pid);
}

// Функция для сортировки процессов по пользователю
void sort_processes_by_user(Process *processes, int num_processes) {
    qsort(processes, num_processes, sizeof(Process), compare_user);
}

// Функция для сортировки процессов по потреблению виртуальной памяти
void sort_processes_by_virtual_memory(Process *processes, int num_processes) {
    qsort(processes, num_processes, sizeof(Process), compare_virtual_memory);
}

// Функция для сортировки процессов по потреблению физической памяти
void sort_processes_by_resident_memory(Process *processes, int num_processes) {
    qsort(processes, num_processes, sizeof(Process), compare_resident_memory);
}

// Функция для сортировки процессов по использованию процессора
void sort_processes_by_cpu_usage(Process *processes, int num_processes) {
    qsort(processes, num_processes, sizeof(Process), compare_cpu_usage);
}
