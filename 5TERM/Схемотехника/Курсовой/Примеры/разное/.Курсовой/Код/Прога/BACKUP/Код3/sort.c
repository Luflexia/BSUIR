#include "sort.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int compare_processes_by_pid(const void *a, const void *b) {
    const Process *process_a = a;
    const Process *process_b = b;
    return process_a->pid - process_b->pid;
}

int compare_processes_by_user(const void *a, const void *b) {
    const Process *process_a = a;
    const Process *process_b = b;
    return strcmp(process_a->user, process_b->user);
}

int compare_processes_by_priority(const void *a, const void *b) {
    const Process *process_a = a;
    const Process *process_b = b;
    return process_a->priority - process_b->priority;
}

int compare_processes_by_virtual_memory(const void *a, const void *b) {
    const Process *process_a = a;
    const Process *process_b = b;
    return process_a->virtual_memory - process_b->virtual_memory;
}

int compare_processes_by_resident_memory(const void *a, const void *b) {
    const Process *process_a = a;
    const Process *process_b = b;
    return process_a->resident_memory - process_b->resident_memory;
}

int compare_processes_by_cpu_usage(const void *a, const void *b) {
    const Process *process_a = a;
    const Process *process_b = b;
    return (int)(process_a->cpu_usage - process_b->cpu_usage);
}

void sort_processes_by_pid(Process *processes, int num_processes) {
    qsort(processes, num_processes, sizeof(Process), compare_processes_by_pid);
}

void sort_processes_by_user(Process *processes, int num_processes) {
    qsort(processes, num_processes, sizeof(Process), compare_processes_by_user);
}

void sort_processes_by_priority(Process *processes, int num_processes) {
    qsort(processes, num_processes, sizeof(Process), compare_processes_by_priority);
}

void sort_processes_by_virtual_memory(Process *processes, int num_processes) {
    qsort(processes, num_processes, sizeof(Process), compare_processes_by_virtual_memory);
}

void sort_processes_by_resident_memory(Process *processes, int num_processes) {
    qsort(processes, num_processes, sizeof(Process), compare_processes_by_resident_memory);
}

void sort_processes_by_cpu_usage(Process *processes, int num_processes) {
    qsort(processes, num_processes, sizeof(Process), compare_processes_by_cpu_usage);
}

int compare_threads_by_pid(const void *a, const void *b) {
    const Thread *thread_a = a;
    const Thread *thread_b = b;
    return thread_a->pid - thread_b->pid;
}

int compare_threads_by_user(const void *a, const void *b) {
    const Thread *thread_a = a;
    const Thread *thread_b = b;
    return strcmp(thread_a->user, thread_b->user);
}

int compare_threads_by_priority(const void *a, const void *b) {
    const Thread *thread_a = a;
    const Thread *thread_b = b;
    return thread_a->priority - thread_b->priority;
}

int compare_threads_by_virtual_memory(const void *a, const void *b) {
    const Thread *thread_a = a;
    const Thread *thread_b = b;
    return thread_a->virtual_memory - thread_b->virtual_memory;
}

int compare_threads_by_resident_memory(const void *a, const void *b) {
    const Thread *thread_a = a;
    const Thread *thread_b = b;
    return thread_a->resident_memory - thread_b->resident_memory;
}

int compare_threads_by_cpu_usage(const void *a, const void *b) {
    const Thread *thread_a = a;
    const Thread *thread_b = b;
    return (int)(thread_a->cpu_usage - thread_b->cpu_usage);
}

void sort_threads_by_pid(Thread *threads, int num_threads) {
    qsort(threads, num_threads, sizeof(Thread), compare_threads_by_pid);
}

void sort_threads_by_user(Thread *threads, int num_threads) {
    qsort(threads, num_threads, sizeof(Thread), compare_threads_by_user);
}

void sort_threads_by_priority(Thread *threads, int num_threads) {
    qsort(threads, num_threads, sizeof(Thread), compare_threads_by_priority);
}

void sort_threads_by_virtual_memory(Thread *threads, int num_threads) {
    qsort(threads, num_threads, sizeof(Thread), compare_threads_by_virtual_memory);
}

void sort_threads_by_resident_memory(Thread *threads, int num_threads) {
    qsort(threads, num_threads, sizeof(Thread), compare_threads_by_resident_memory);
}

void sort_threads_by_cpu_usage(Thread *threads, int num_threads) {
    qsort(threads, num_threads, sizeof(Thread), compare_threads_by_cpu_usage);
}
