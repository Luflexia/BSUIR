#include "readThreads.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>

// Функция для чтения информации о потоках
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
                        sscanf(line, "%*s %d", &threads[count].tid);
                    } else if (strncmp(line, "Uid:", 4) == 0) {
                        sscanf(line, "%*s %s", threads[count].user);
                    } else if (strncmp(line, "VmSize:", 7) == 0) {
                        sscanf(line, "%*s %lu", &threads[count].vm_size);
                    } else if (strncmp(line, "VmRSS:", 6) == 0) {
                        sscanf(line, "%*s %lu", &threads[count].vm_rss);
                    } else if (strncmp(line, "Cpu(s):", 7) == 0) {
                        sscanf(line, "%*s %*s %*s %*s %lf", &threads[count].cpu_usage);
                    } else if (strncmp(line, "Cmdline:", 8) == 0) {
                        sscanf(line, "%*s %s", threads[count].cmd);
                    }
                }
                fclose(fp);
                threads[count].pid = atoi(entry->d_name);
                count++;
            }
        }
    }

    closedir(dir);
    return count;
}
