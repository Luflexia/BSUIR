#include "process_info.c"
#include "thread_info.c"

void display_process_info(struct ProcessInfo *pinfo) {
    printf("%-10d %-20s %-10s %-10ld %-10.2f\n", pinfo->pid, pinfo->name, pinfo->username, pinfo->memory_usage, pinfo->cpu_usage);
}

void display_process_list(struct ProcessInfo *processes, int count) {
    printf("Список процессов:\n");
    printf("%-10s %-20s\n", "PID", "Имя процесса");
    for (int i = 0; i < count; i++) {
        printf("%-10d %-20s\n", processes[i].pid, processes[i].name);
    }
}

void display_thread_list(pthread_t *threads, size_t count) {
    printf("Список потоков:\n");
    printf("%-10s\n", "ID потока");
    for (size_t i = 0; i < count; i++) {
        printf("%-10lu\n", (unsigned long) threads[i]);
    }
}