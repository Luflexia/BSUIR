#include "readProcess.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>

// Функция для чтения информации о процессах
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
