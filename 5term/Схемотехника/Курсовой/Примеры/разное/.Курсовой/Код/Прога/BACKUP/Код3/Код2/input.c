#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sort.h"
#include "sortThreads.h"
#include "controlProcess.h"
#include "controlThreads.h"

// Функция для обработки пользовательского ввода
void handle_input(Process *processes, int num_processes, Thread *threads, int num_threads) {
    char input[50];
    printf("Введите команду (например, 'kill pid', 'sort user'):\n");
    fgets(input, sizeof(input), stdin);

    char *token = strtok(input, " ");
    if (token != NULL) {
        if (strcmp(token, "kill") == 0) {
            // Команда для завершения процесса или потока
            token = strtok(NULL, " ");
            int id = atoi(token);
            if (id > 0) {
                if (id < 10000) {
                    // Завершение процесса
                    kill_process_by_pid(id);
                } else {
                    // Завершение потока
                    kill_thread_by_tid(id);
                }
            } else {
                printf("Неверный формат ID.\n");
            }
        } else if (strcmp(token, "sort") == 0) {
            // Команда для сортировки
            token = strtok(NULL, " \n");
            if (strcmp(token, "pid") == 0) {
                sort_processes_by_pid(processes, num_processes);
            } else if (strcmp(token, "user") == 0) {
                sort_processes_by_user(processes, num_processes);
            } else if (strcmp(token, "vm") == 0) {
                sort_processes_by_virtual_memory(processes, num_processes);
            } else if (strcmp(token, "rm") == 0) {
                sort_processes_by_resident_memory(processes, num_processes);
            } else if (strcmp(token, "cpu") == 0) {
                sort_processes_by_cpu_usage(processes, num_processes);
            } else {
                printf("Неверный критерий сортировки.\n");
            }
        } else {
            printf("Неверная команда.\n");
        }
    }
}
