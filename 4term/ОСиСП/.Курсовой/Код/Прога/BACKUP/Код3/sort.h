#ifndef SORT_H
#define SORT_H

#include "process.h"
#include "threads.h"

void sort_processes_by_pid(Process *processes, int num_processes);
void sort_processes_by_user(Process *processes, int num_processes);
void sort_processes_by_priority(Process *processes, int num_processes);
void sort_processes_by_virtual_memory(Process *processes, int num_processes);
void sort_processes_by_resident_memory(Process *processes, int num_processes);
void sort_processes_by_cpu_usage(Process *processes, int num_processes);

void sort_threads_by_pid(Thread *threads, int num_threads);
void sort_threads_by_user(Thread *threads, int num_threads);
void sort_threads_by_priority(Thread *threads, int num_threads);
void sort_threads_by_virtual_memory(Thread *threads, int num_threads);
void sort_threads_by_resident_memory(Thread *threads, int num_threads);
void sort_threads_by_cpu_usage(Thread *threads, int num_threads);

#endif /* SORT_H */
