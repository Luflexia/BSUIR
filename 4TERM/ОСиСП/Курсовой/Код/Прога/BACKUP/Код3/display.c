#include "display.h"
#include "sysinfo.h"
#include <stdio.h>

void display_sysinfo(SysInfo *sysinfo) {
    printf("Общее количество процессов: %d\n", sysinfo->total_processes);
    printf("Количество активных процессов: %d\n", sysinfo->running_processes);
    printf("Общее количество потоков: %d\n", sysinfo->total_threads);
    printf("Количество пользователей: %d\n", sysinfo->num_users);
    printf("Средняя нагрузка на систему (за последние 1, 5 и 15 минут): %.2f, %.2f, %.2f\n", sysinfo->load_avg[0], sysinfo->load_avg[1], sysinfo->load_avg[2]);
    printf("Использование ЦП пользователем: %.2f%%\n", sysinfo->cpu_usage_user);
    printf("Использование ЦП системой: %.2f%%\n", sysinfo->cpu_usage_system);
}

void display_processes(Process *processes, int num_processes) {
    printf("%-8s %-15s %-8s %-12s %-12s %-8s %-8s %s\n",
           "PID", "USER", "PRIORITY", "VM(KB)", "RM(KB)", "CPU(%)", "COMMAND");

    for (int i = 0; i < num_processes; ++i) {
        printf("%-8d %-15s %-8d %-12ld %-12ld %-8.2f %-8s\n",
               processes[i].pid, processes[i].user, processes[i].priority,
               processes[i].virtual_memory, processes[i].resident_memory,
               processes[i].cpu_usage, processes[i].command);
    }
}

void display_threads(Thread *threads, int num_threads) {
    printf("%-8s %-15s %-8s %-12s %-12s %-8s %-8s %s\n",
           "PID", "USER", "TID", "VM(KB)", "RM(KB)", "CPU(%)", "COMMAND");

    for (int i = 0; i < num_threads; ++i) {
        printf("%-8d %-15s %-8d %-12ld %-12ld %-8.2f %-8s\n",
               threads[i].pid, threads[i].user, threads[i].tid,
               threads[i].virtual_memory, threads[i].resident_memory,
               threads[i].cpu_usage, threads[i].command);
    }
}
