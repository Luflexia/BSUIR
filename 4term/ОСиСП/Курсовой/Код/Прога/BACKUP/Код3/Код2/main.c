#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <termios.h>
#include <fcntl.h>
#include "process.h"
#include "threads.h"
#include "readProcess.h"
#include "readThreads.h"
#include "updateThreads.h"
#include "updateProcess.h" 


// Объявление функции для получения нажатых клавиш без ожидания Enter
int kbhit(void);

// Объявление обработчика сигнала для выхода из программы
void handle_signal(int signal);

int main() {
    Process processes[MAX_PROC];
    Thread threads[MAX_THREADS];

    // Чтение данных при запуске
    int num_processes = read_processes(processes);
    if (num_processes < 0) {
        fprintf(stderr, "Ошибка чтения процессов\n");
        return 1;
    }

    // Установка обработчика сигнала SIGINT (Ctrl+C)
    signal(SIGINT, handle_signal);

    // Бесконечный цикл обновления данных
    while (1) {
    
        // Отображение SysInfo
        displaySystemInfo();

        // Обновление данных о процессах
        update_process_data(processes, num_processes);

        // Обновление данных о потоках
        update_thread_data(threads, MAX_THREADS);

        // Отображение информации о процессах и потоках
        display_process_info(processes, num_processes);
        display_thread_info(threads);

        // Ожидание 1 секунда перед следующим обновлением данных
        sleep(1);

        // Если Ctrl+C, выходим из программы
        if (kbhit()) {
            break;
        }
    }
    return 0;
}

// Функция для получения нажатых клавиш без ожидания Enter
int kbhit(void) {
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if(ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }

    return 0;
}

// Обработчик сигнала для выхода из программы
void handle_signal(int signal) {
    if (signal == SIGINT) {
        printf("Программа завершена.\n");
        exit(0);
    }
}
