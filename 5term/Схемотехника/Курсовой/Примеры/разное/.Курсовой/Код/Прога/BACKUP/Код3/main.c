#include "control.h"
#include "display.h"
#include "sysinfo.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

int main() {
    // Установка обработчика сигнала SIGINT (Ctrl+C)
    signal(SIGINT, handle_signal);
    
    SysInfo sysinfo;
    
     // Чтение данных о системе
     read_sysinfo(&sysinfo);

    // Чтение данных при запуске
    num_processes = read_processes(processes);
    if (num_processes < 0) {
        fprintf(stderr, "Ошибка чтения процессов\n");
        return 1;
    }

    num_threads = read_threads(threads);
    if (num_threads < 0) {
        fprintf(stderr, "Ошибка чтения потоков\n");
        return 1;
    }

    // Бесконечный цикл обновления данных
    while (1) {
        // Очистка экрана перед обновлением информации
        system("clear");

        // Отображение информации о системе
        display_sysinfo(&sysinfo);
        
        // Чтение данных о процессах и потоках
        num_processes = read_processes(processes);
        if (num_processes < 0) {
            fprintf(stderr, "Ошибка чтения процессов\n");
            return 1;
        }

        num_threads = read_threads(threads);
        if (num_threads < 0) {
            fprintf(stderr, "Ошибка чтения потоков\n");
            return 1;
        }

        // Отображение информации о процессах или потоках в зависимости от выбранного режима
        if (display_mode == 0) {
            display_processes(processes, num_processes);
        } else {
            display_threads(threads, num_threads);
        }

        // Ожидание 1 секунда перед следующим обновлением данных
        sleep(1);

        // Обработка нажатия клавиш
        handle_input(processes, num_processes, threads, num_threads);
    }

    return 0;
}
