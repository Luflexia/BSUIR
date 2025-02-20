#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>

struct ProcessInfo {
    int pid;
    char name[256];
};

void get_process_list(struct ProcessInfo **processes, int *count) {
    DIR *dir;
    struct dirent *ent;
    int num_processes = 0;

    // Открываем директорию /proc
    dir = opendir("/proc");
    if (dir == NULL) {
        perror("Ошибка при открытии директории /proc");
        exit(EXIT_FAILURE);
    }

    // Считываем информацию о процессах
    while ((ent = readdir(dir)) != NULL) {
        if (ent->d_type == DT_DIR) {
            // Проверяем, является ли имя директории числом (идентификатором процесса)
            int pid = atoi(ent->d_name);
            if (pid != 0) {
                // Получаем имя процесса
                char stat_path[256];
                char stat_buf[1024];
                FILE *stat_file;

                sprintf(stat_path, "/proc/%d/stat", pid);
                stat_file = fopen(stat_path, "r");
                if (stat_file == NULL) {
                    continue;  // Пропускаем процесс, если не удалось открыть stat-файл
                }

                // Извлекаем имя процесса из stat-файла
                fgets(stat_buf, sizeof(stat_buf), stat_file);
                fclose(stat_file);

                // Извлекаем имя процесса из строки
                char *start = strchr(stat_buf, '(') + 1;
                char *end = strchr(stat_buf, ')');
                size_t name_length = end - start;
                strncpy((*processes)[num_processes].name, start, name_length);
                (*processes)[num_processes].name[name_length] = '\0';
                (*processes)[num_processes].pid = pid;

                num_processes++;
            }
        }
    }

    closedir(dir);

    *count = num_processes;
}