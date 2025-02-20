#include "control.h"
#include "display.h"
#include "input.h"
#include "sort.h"
#include "read.h"
#include "sysinfo.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <termios.h>
#include <fcntl.h>
#include "process.h"
#include "threads.h"

#define MAX_PROC 1000
#define MAX_THREADS 1000

Process processes[MAX_PROC];
Thread threads[MAX_THREADS];

int num_processes = 0;
int num_threads = 0;

int display_mode = 0; // 0 - процессы, 1 - потоки

void handle_signal(int signal) {
    if (signal == SIGINT) {
        printf("Программа завершена.\n");
        exit(0);
    }
}

void handle_input(Process *processes, int num_processes, Thread *threads, int num_threads) {
    if (kbhit()) {
        char c = getchar();
        switch (c) {
            case 'z':
                switch_color_mode();
                break;
            case 't':
                display_mode = 1;
                break;
            case 'p':
                display_mode = 0;
                break;
            case 'k': {
                int id;
                char prompt[50];
                if (display_mode == 0) {
                    sprintf(prompt, "Введите PID процесса для удаления: ");
                } else {
                    sprintf(prompt, "Введите TID потока для удаления: ");
                }
                printf("%s", prompt);
                scanf("%d", &id);
                if (display_mode == 0) {
                    kill_process_by_pid(processes, num_processes, id);
                } else {
                    kill_thread_by_tid(threads, num_threads, id);
                }
                break;
            }
            case 'q':
                handle_signal(SIGINT);
                break;
            case 's': {
                char criterion[50];
                printf("Введите критерий для сортировки (pid/tid, user, priority, virtual_memory, resident_memory, cpu_usage): ");
                scanf("%s", criterion);
                if (display_mode == 0) {
                    if (strcmp(criterion, "pid") == 0) {
                        sort_processes_by_pid(processes, num_processes);
                    } else if (strcmp(criterion, "user") == 0) {
                        sort_processes_by_user(processes, num_processes);
                    } else if (strcmp(criterion, "priority") == 0) {
                        sort_processes_by_priority(processes, num_processes);
                    } else if (strcmp(criterion, "virtual_memory") == 0) {
                        sort_processes_by_virtual_memory(processes, num_processes);
                    } else if (strcmp(criterion, "resident_memory") == 0) {
                        sort_processes_by_resident_memory(processes, num_processes);
                    } else if (strcmp(criterion, "cpu_usage") == 0) {
                        sort_processes_by_cpu_usage(processes, num_processes);
                    } else {
                        printf("Неверный критерий сортировки.\n");
                    }
                } else {
                    if (strcmp(criterion, "tid") == 0) {
                        sort_threads_by_pid(threads, num_threads);
                    } else if (strcmp(criterion, "user") == 0) {
                        sort_threads_by_user(threads, num_threads);
                    } else if (strcmp(criterion, "priority") == 0) {
                        sort_threads_by_priority(threads, num_threads);
                    } else if (strcmp(criterion, "virtual_memory") == 0) {
                        sort_threads_by_virtual_memory(threads, num_threads);
                    } else if (strcmp(criterion, "resident_memory") == 0) {
                        sort_threads_by_resident_memory(threads, num_threads);
                    } else if (strcmp(criterion, "cpu_usage") == 0) {
                        sort_threads_by_cpu_usage(threads, num_threads);
                    } else {
                        printf("Неверный критерий сортировки.\n");
                    }
                }
                break;
            }
            default:
                break;
        }
    }
}

void switch_color_mode() {
    static int color_mode = 0;
    color_mode = !color_mode;
    if (color_mode) {
        printf("\033[1;37m"); // white on black
    } else {
        printf("\033[0;30m"); // black on white
    }
    printf("\n");
}

void kill_process_by_pid(Process *processes, int num_processes, int pid) {
    // find process with given PID and kill it
    for (int i = 0; i < num_processes; i++) {
        if (processes[i].pid == pid) {
            kill(pid, SIGKILL);
            return;
        }
    }
    printf("Процесс с указанным PID не найден.\n");
}

void kill_thread_by_tid(Thread *threads, int num_threads, int tid) {
    // find thread with given TID and kill it
    for (int i = 0; i < num_threads; i++) {
        if (threads[i].tid == tid) {
            pthread_cancel(threads[i].tid);
            return;
        }
    }
    printf("Поток с указанным TID не найден.\n");
}
