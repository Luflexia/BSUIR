#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pwd.h>
#include <time.h>
#include "processes.h"
#include "threads.h"

void get_thread_info(ThreadInfo *thread_info, int pid, int tid) {
    char path[256];
    FILE *file;
    thread_info->tid = tid;

    snprintf(path, sizeof(path), "/proc/%d/task/%d/comm", pid, tid);
    file = fopen(path, "r");
    if (file) {
        if (fgets(thread_info->name, sizeof(thread_info->name), file)) {
            thread_info->name[strcspn(thread_info->name, "\n")] = '\0';
        }
        fclose(file);
    }

    snprintf(path, sizeof(path), "/proc/%d/task/%d/status", pid, tid);
    file = fopen(path, "r");
    if (file) {
        char buffer[256];
        while (fgets(buffer, sizeof(buffer), file)) {
            if (strncmp("State:", buffer, 6) == 0) {
                sscanf(buffer, "State: %c", &thread_info->state);
                break;
            }
        }
        fclose(file);
    }
}


// Функция для подсчета количества ядер
int count_cpu_cores(const char *cpu_list) {
    int cores = 1;
    for (const char *p = cpu_list; *p; p++) {
        if (*p == ',') {
            cores++;
        }
    }
    return cores;
}

// Функция для чтения времени запуска системы
time_t get_system_uptime() {
    FILE *file = fopen("/proc/uptime", "r");
    if (!file) {
        perror("Failed to open /proc/uptime");
        return 0;
    }
    double uptime;
    fscanf(file, "%lf", &uptime);
    fclose(file);
    return (time_t)uptime;
}

// Функция для чтения информации о процессе
void get_process_info(ProcessInfo *proc_info, int pid) {
    char path[256];
    char buffer[256];
    FILE *file;
    struct passwd *pw;

    // Заполняем PID
    proc_info->pid = pid;

    // Чтение информации из /proc/[pid]/status для получения имени пользователя, состояния процесса, виртуальной памяти и команды
    snprintf(path, sizeof(path), "/proc/%d/status", pid);
    file = fopen(path, "r");
    if (file) {
        while (fgets(buffer, sizeof(buffer), file)) {
            if (strncmp(buffer, "Uid:", 4) == 0) {
                int uid;
                sscanf(buffer, "Uid: %d", &uid);
                pw = getpwuid(uid);
                if (pw) {
                    strncpy(proc_info->user, pw->pw_name, sizeof(proc_info->user) - 1);
                }
            } else if (strncmp(buffer, "VmSize:", 7) == 0) {
                unsigned long vm_size;
                sscanf(buffer, "VmSize: %lu kB", &vm_size);
                proc_info->virtual_memory = vm_size / 1024.0 / 1024.0; // Конвертируем в MB
            } else if (strncmp(buffer, "State:", 6) == 0) {
                sscanf(buffer, "State: %c", &proc_info->state);
            } else if (strncmp(buffer, "Name:", 5) == 0) {
                sscanf(buffer, "Name: %s", proc_info->command);
            } else if (strncmp(buffer, "Cpus_allowed_list:", 18) == 0) {
                // Здесь считываем информацию о привязке к ядрам
                char cpu_list[256];
                sscanf(buffer, "Cpus_allowed_list: %s", cpu_list);
                proc_info->cpu_cores = count_cpu_cores(cpu_list); // Подсчитываем количество ядер
            } else if (strncmp(buffer, "Threads:", 8) == 0) {
                sscanf(buffer, "Threads: %d", &proc_info->threads);
            }
        }
        fclose(file);
    }

    // Чтение информации из /proc/[pid]/stat для получения потребления физической памяти и времени запуска
    snprintf(path, sizeof(path), "/proc/%d/stat", pid);
    file = fopen(path, "r");
    if (file) {
        long rss;
        unsigned long utime, stime, starttime;
        fscanf(file, "%*d %*s %*c %*d %*d %*d %*d %*d %*d %*u %*u %*u %*u %lu %lu %*d %*d %*d %*d %llu", &utime, &stime, &starttime);
        fclose(file);

        // Получаем текущее время в секундах
        time_t now = time(NULL);

        // Время работы системы в секундах
        time_t uptime = get_system_uptime();

        // Время запуска процесса (текущее время - (время работы системы - время старта процесса))
        time_t start_time = now - (uptime - (starttime / sysconf(_SC_CLK_TCK)));

        struct tm start_tm;
        localtime_r(&start_time, &start_tm);
        strftime(proc_info->start_time, sizeof(proc_info->start_time), "%Y-%m-%d %H:%M:%S", &start_tm);

        snprintf(path, sizeof(path), "/proc/%d/statm", pid);
        file = fopen(path, "r");
        if (file) {
            fscanf(file, "%*d %ld", &rss);
            fclose(file);
            proc_info->resident_memory = rss * (sysconf(_SC_PAGESIZE) / 1024.0 / 1024.0); // Конвертируем в MB


        }
    }
}