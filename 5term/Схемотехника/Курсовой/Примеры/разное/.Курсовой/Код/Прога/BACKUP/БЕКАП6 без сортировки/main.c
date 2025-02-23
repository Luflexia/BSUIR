#include <curses.h>
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include "processes.h"
#include "display.h"
#include "threads.h"

#define MAX_PROCESSES 1000  // Максимальное количество процессов для отображения
#define REFRESH_INTERVAL 3000  // Интервал обновления в миллисекундах
#define SCROLL_LINES 20  // Количество строк для прокрутки по стрелкам

int main() {
    initscr();         // Инициализация ncurses
    start_color();     // Включение поддержки цвета
    noecho();          // Отключение эха вводимых символов
    cbreak();          // Включение режима cbreak
    keypad(stdscr, TRUE); // Включение поддержки клавиш
    mousemask(ALL_MOUSE_EVENTS, NULL); // Включение обработки всех событий мыши

    // Инициализация цветовых пар
    init_pair(1, COLOR_WHITE, COLOR_BLACK); // Стандартная цветовая схема (белый текст на черном фоне)
    init_pair(2, COLOR_BLACK, COLOR_WHITE); // Инвертированная цветовая схема (черный текст на белом фоне)

    int start_line = 0;  // Первая видимая строка
    int total_lines = 0; // Общее количество строк для отображения
    DisplayMode mode = SHOW_PROCESSES; // Режим отображения по умолчанию
    ColorScheme color_scheme = COLOR_SCHEME_DEFAULT; // Цветовая схема по умолчанию

    while (1) {
        DIR *dir;
        struct dirent *entry;

        dir = opendir("/proc");
        if (!dir) {
            perror("opendir(/proc)");
            endwin();
            return 1;
        }

        ProcessData process_data[MAX_PROCESSES]; // Сохранение информации о процессах и потоках
        int process_count = 0; // Обнуляем количество процессов
        total_lines = 1; // Обнуляем количество строк (учитываем строку заголовка)

        while ((entry = readdir(dir)) != NULL) {
            if (entry->d_type == DT_DIR) {
                int pid = atoi(entry->d_name);
                if (pid > 0 && process_count < MAX_PROCESSES) {
                    ProcessInfo proc_info;
                    get_process_info(&proc_info, pid);
                    process_data[process_count].process_info = proc_info;
                    process_data[process_count].thread_count = 0;
                    total_lines++;

                    if (mode == SHOW_PROCESSES_AND_THREADS) {
                        char path[256];
                        DIR *task_dir;
                        struct dirent *task_entry;
                        snprintf(path, sizeof(path), "/proc/%d/task", proc_info.pid);
                        task_dir = opendir(path);
                        if (task_dir) {
                            while ((task_entry = readdir(task_dir)) != NULL) {
                                if (task_entry->d_type == DT_DIR && task_entry->d_name[0] != '.' && process_data[process_count].thread_count < MAX_THREADS_PER_PROCESS) {
                                    int tid = atoi(task_entry->d_name);
                                    ThreadInfo thread_info;
                                    get_thread_info(&thread_info, proc_info.pid, tid);
                                    process_data[process_count].threads[process_data[process_count].thread_count] = thread_info;
                                    process_data[process_count].thread_count++;
                                    total_lines++;
                                }
                            }
                            closedir(task_dir);
                        }
                    }
                    process_count++;
                }
            }
        }
        closedir(dir);
        update_display(start_line, total_lines, process_data, process_count, mode, color_scheme);

        timeout(REFRESH_INTERVAL);  // Устанавливаем таймаут ожидания ввода
        int ch = getch();
        if (ch == KEY_MOUSE) {
            MEVENT event;
            if (getmouse(&event) == OK) {
                if (event.bstate & BUTTON4_PRESSED) { // Прокрутка вверх
                    if (start_line > 0) start_line--;
                } else if (event.bstate & BUTTON5_PRESSED) { // Прокрутка вниз
                    if (start_line < total_lines - (LINES - 1)) start_line++;
                }
                update_display(start_line, total_lines, process_data, process_count, mode, color_scheme);
            }
        } else if (ch == KEY_UP) {
            if (start_line > SCROLL_LINES) {
                start_line -= SCROLL_LINES;
            } else {
                start_line = 0;
            }
            update_display(start_line, total_lines, process_data, process_count, mode, color_scheme);
        } else if (ch == KEY_DOWN) {
            if (start_line + SCROLL_LINES < total_lines - (LINES - 1)) {
                start_line += SCROLL_LINES;
            } else {
                start_line = total_lines - (LINES - 1);
                if (start_line < 0) start_line = 0;
            }
            update_display(start_line, total_lines, process_data, process_count, mode, color_scheme);
        } else if (ch == 't') {
            if (mode == SHOW_PROCESSES) {
                mode = SHOW_PROCESSES_AND_THREADS;
            } else {
                mode = SHOW_PROCESSES;
            }
            start_line = 0;  // Сбрасываем начало строки при переключении режима
            update_display(start_line, total_lines, process_data, process_count, mode, color_scheme);
        } else if (ch == 'z') {
            if (color_scheme == COLOR_SCHEME_DEFAULT) {
                // Включение инвертированной цветовой схемы для всего окна
                wbkgd(stdscr, COLOR_PAIR(2));
                color_scheme = COLOR_SCHEME_INVERTED;
            } else {
                // Включение стандартной цветовой схемы для всего окна
                wbkgd(stdscr, COLOR_PAIR(1));
                color_scheme = COLOR_SCHEME_DEFAULT;
            }
            start_line = 0;  // Сбрасываем начало строки при переключении цветовой схемы
            update_display(start_line, total_lines, process_data, process_count, mode, color_scheme);
        } else if (ch == 'q') {
            break; // Завершение программы при нажатии 'q'
        } else if (ch == ERR) {
            // Таймаут достигнут, обновляем экран
            update_display(start_line, total_lines, process_data, process_count, mode, color_scheme);
        }
    }

    endwin(); // Завершение работы ncurses
    return 0;
}

