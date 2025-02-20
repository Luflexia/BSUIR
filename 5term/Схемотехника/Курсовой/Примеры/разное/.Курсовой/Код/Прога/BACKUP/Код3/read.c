#include "read.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>

void read_sysinfo(SysInfo *sysinfo) {
    struct sysinfo info;
    if (sysinfo_mem_info(&info) == 0) {
        sysinfo->total_processes = info.procs;
        sysinfo->running_processes = info.procs_running;
        sysinfo->total_threads = info.totalhigh;
        sysinfo->num_users = info.users;
        sysinfo->load_avg[0] = info.loads[0];
        sysinfo->load_avg[1] = info.loads[1];
        sysinfo->load_avg[2] = info.loads[2];
        sysinfo->cpu_usage_user = info.totalcpu * 100.0 / (1 << SI_CPU_SHIFT);
        sysinfo->cpu_usage_system = info.totalsystemcpu * 100.0 / (1 << SI_CPU_SHIFT);
    } else {
        printf("Ошибка чтения информации о системе\n");
    }
}

int read_processes(Process *processes) {
    DIR *dir;
    struct dirent *entry;
    int count = 0;

    if ((dir = opendir("/proc")) == NULL) {
        perror("Ошибка opendir()");
        return -1;
    }

    while ((entry = readdir(dir)) != NULL && count < MAX_PROC) {
        if (isdigit(*entry->d_name)) {
            char path[256];
            sprintf(path, "/proc/%s/status", entry->d_name);

            FILE *fp;
            if ((fp = fopen(path, "r")) != NULL) {
                char line[256];
                while (fgets(line, sizeof(line), fp)) {
                    if (strncmp(line, "Pid:", 4) == 0) {
                        sscanf(line, "%*s %d", &processes[count].pid);
                    } else if (strncmp(line, "Uid:", 4) == 0) {
                        sscanf(line, "%*s %s", processes[count].user);
                    } else if (strncmp(line, "VmSize:", 7) == 0) {
                        sscanf(line, "%*s %ld", &processes[count].virtual_memory);
                    } else if (strncmp(line, "VmRSS:", 6) == 0) {
                        sscanf(line, "%*s %ld", &processes[count].resident_memory);
                    } else if (strncmp(line, "Cpu(s):", 7) == 0) {
                        sscanf(line, "%*s %lf", &processes[count].cpu_usage);
                    } else if (strncmp(line, "Cmdline:", 8) == 0) {
                        sscanf(line, "%*s %[^\n]", processes[count].command);
                    }
                }
                fclose(fp);
                count++;
            }
        }
    }

    closedir(dir);
    return count;
}

int read_threads(Thread *threads) {
    DIR *dir;
    struct dirent *entry;
    int count = 0;

    if ((dir = opendir("/proc")) == NULL) {
        perror("Ошибка opendir()");
        return -1;
    }

    while ((entry = readdir(dir)) != NULL && count < MAX_THREADS) {
        if (isdigit(*entry->d_name)) {
            char path[256];
            sprintf(path, "/proc/%s/task/%s/status", entry->d_name, entry->d_name);

            FILE *fp;
            if ((fp = fopen(path, "r")) != NULL) {
                char line[256];
                while (fgets(line, sizeof(line), fp)) {
                    if (strncmp(line, "Pid:", 4) == 0) {
                        sscanf(line, "%*s %d", &threads[count].pid);
                    } else if (strncmp(line, "Tid:", 4) == 0) {
                        sscanf(line, "%*s %d", &threads[count].tid);
                    } else if (strncmp(line, "Uid:", 4) == 0) {
                        sscanf(line, "%*s %s", threads[count].user);
                    } else if (strncmp(line, "VmSize:", 7) == 0) {
                        sscanf(line, "%*s %ld", &threads[count].virtual_memory);
                    } else if (strncmp(line, "VmRSS:", 6) == 0) {
                        sscanf(line, "%*s %ld", &threads[count].resident_memory);
                    } else if (strncmp(line, "Cpu(s):", 7) == 0) {
                        sscanf(line, "%*s %lf", &threads[count].cpu_usage);
                    } else if (strncmp(line, "Cmdline:", 8) == 0) {
                        sscanf(line, "%*s %[^\n]", threads[count].command);
                    }
                }
                fclose(fp);
                count++;
            }
        }
    }

    closedir(dir);
    return count;
}
