#include "displayThreads.h"
#include <stdio.h>

// Функция для отображения информации о потоках
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
