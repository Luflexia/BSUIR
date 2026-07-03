#ifndef SORT_H
#define SORT_H

#include "process.h"

// Функция для сортировки процессов по PID
void sort_processes_by_pid(Process *processes, int num_processes);

// Функция для сортировки процессов по пользователю
void sort_processes_by_user(Process *processes, int num_processes);

// Функция для сортировки процессов по потреблению виртуальной памяти
void sort_processes_by_virtual_memory(Process *processes, int num_processes);

// Функция для сортировки процессов по потреблению физической памяти
void sort_processes_by_resident_memory(Process *processes, int num_processes);

// Функция для сортировки процессов по использованию процессора
void sort_processes_by_cpu_usage(Process *processes, int num_processes);

#endif /* SORT_H */
