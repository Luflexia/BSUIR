#include <stdio.h>
#include <time.h>
#include <sys/sysinfo.h>

void readString1() {
    // Получение текущего времени
    time_t current_time = time(NULL);
    struct tm *tm_info = localtime(&current_time);
    char time_str[9]; // Для формата HH:MM:SS
    strftime(time_str, sizeof(time_str), "%H:%M:%S", tm_info);

    printf("Bruh - (%s), ", time_str);

    // Получение информации о системе
    struct sysinfo info;
    if (sysinfo(&info) != 0) {
        perror("Ошибка при получении информации о системе");
        return;
    }

    printf("(%ld days, %ld:%02ld), ", 
           info.uptime / (60 * 60 * 24), (info.uptime % (60 * 60 * 24)) / (60 * 60), (info.uptime % (60 * 60)) / 60);
    printf("(%d users), ", info.procs);
    printf("(%.2f, %.2f, %.2f - за 1 минуту, 5 минут и 15 минут).\n", 
           (float)info.loads[0] / (1 << SI_LOAD_SHIFT), (float)info.loads[1] / (1 << SI_LOAD_SHIFT), 
           (float)info.loads[2] / (1 << SI_LOAD_SHIFT));
}

void readString2() {
    // Открываем файл /proc/stat для чтения
    FILE *file = fopen("/proc/stat", "r");
    if (file == NULL) {
        perror("Ошибка при открытии файла /proc/stat");
        return;
    }

    // Объявляем переменные для хранения данных
    int total_processes = 0, running_processes = 0, sleeping_processes = 0, stopped_processes = 0, zombie_processes = 0;
    char line[256];

    // Считываем данные из файла
    while (fgets(line, sizeof(line), file) != NULL) {
        // Проверяем строки, начинающиеся с "procs_"
        if (strstr(line, "procs_") != NULL) {
            // Считываем количество процессов из каждой категории
            if (strstr(line, "total") != NULL) {
                sscanf(line, "processes %d", &total_processes);
            } else if (strstr(line, "running") != NULL) {
                sscanf(line, "procs_running %d", &running_processes);
            } else if (strstr(line, "blocked") != NULL) {
                sscanf(line, "procs_blocked %d", &sleeping_processes);
            } else if (strstr(line, "stopped") != NULL) {
                sscanf(line, "procs_blocked %d", &stopped_processes);
            } else if (strstr(line, "zombie") != NULL) {
                sscanf(line, "procs_blocked %d", &zombie_processes);
            }
        }
    }

    // Закрываем файл
    fclose(file);

    // Выводим считанные значения
    printf("Tasks - total: %d, running: %d, sleeping: %d, stopped: %d, zombie: %d\n", total_processes, running_processes, sleeping_processes, stopped_processes, zombie_processes);
}

void readString3() {
    // Открываем файл /proc/stat для чтения информации о процессоре
    FILE *file = fopen("/proc/stat", "r");
    if (file == NULL) {
        perror("Ошибка при открытии файла /proc/stat");
        return;
    }

    // Объявляем переменные для хранения значений процентов
    float user, system, nice, idle, iowait, irq, softirq, steal;

    // Считываем данные из файла
    fscanf(file, "cpu %f %f %f %f %f %f %f %f", 
           &user, &nice, &system, &idle, &iowait, &irq, &softirq, &steal);

    // Закрываем файл
    fclose(file);

    // Выводим считанные значения
    printf("%%Cpu(s) - user: %.1f%%, system: %.1f%%, nice: %.1f%%, idle: %.1f%%, iowait: %.1f%%, irq: %.1f%%, softirq: %.1f%%, steal: %.1f%%\n", user, system, nice, idle, iowait, irq, softirq, steal);
}

void displaySystemInfo() {
    readString1();
    readString2();
    readString3();
}
