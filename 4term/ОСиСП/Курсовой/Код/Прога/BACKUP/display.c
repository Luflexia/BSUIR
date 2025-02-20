#include "display.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ncurses.h>
#include <unistd.h>
#include "processes.h"
#include "threads.h"
#include "control.h"

void display_thread_info(const ThreadInfo *thread_info) {
    printw("%-5d %-8s %-5c %-9s %-9s %-4s %-10s -%s\n",
           thread_info->tid, "", thread_info->state, "", "", "", "", thread_info->name);
}

void display_header(ColorScheme color_scheme) {
    int max_y, max_x; // Объявляем переменные для хранения размеров экрана
    getmaxyx(stdscr, max_y, max_x); // Получаем размеры окна
    char datetime[50];
    time_t rawtime;
    struct tm *timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(datetime, 50, "%Y-%m-%d %H:%M:%S", timeinfo); // Форматируем дату и время

    int datetime_length = strlen(datetime);
    const char *prefix = "Kustrica ";
    int prefix_length = strlen(prefix);
    int total_length = prefix_length + datetime_length;
    int padding = (max_x - total_length) / 2;

    // Выводим текст "Kustrica " и дату/время по центру первой строки без цветовой пары
    mvprintw(0, padding, "%s%s", prefix, datetime);

    // Устанавливаем цветовую пару для второй строки в зависимости от схемы
    if (color_scheme == COLOR_SCHEME_INVERTED) {
        attron(COLOR_PAIR(1)); // Черный текст на белом фоне
    } else {
        attron(COLOR_PAIR(2)); // Белый текст на черном фоне
    }

    mvprintw(1, 0, "%*s", max_x, ""); // Заполняем всю вторую строку пробелами с цветовой парой
    mvprintw(1, 0, "%-5s %-8s %-5s %-9s %-9s %-5s %-9s %s",
             "PID", "USER", "STATE", "RES_MEM", "VIRT_MEM", "CORES", "START", "COMMAND");

    // Отключаем цветовую пару после второй строки
    if (color_scheme == COLOR_SCHEME_INVERTED) {
        attroff(COLOR_PAIR(1));
    } else {
        attroff(COLOR_PAIR(2));
    }
}

void display_process_info(const ProcessInfo *proc_info) {
    // Обрезаем имя пользователя до 8 символов
    printw("%-5d %-8.8s %-5c %-6.1f MB %-6.1f MB %-5d %-9.8s ",
           proc_info->pid, proc_info->user, proc_info->state,
           proc_info->resident_memory, proc_info->virtual_memory,
           proc_info->cpu_cores, proc_info->start_time);

    // Обрезаем имя команды до последнего компонента пути, если есть '/'
    const char *command = strrchr(proc_info->command, '/');
    if (command) {
        command++; // Переходим к символу после '/'
    } else {
        command = proc_info->command; // Если '/' нет, используем всю строку
    }

    printw("%s\n", command);
}


void update_display(int start_line, int total_lines, ProcessData *process_data, int process_count, DisplayMode mode, ColorScheme color_scheme) {
    // Сортировка данных перед отображением
    switch (current_sort) {
        case SORT_BY_PID:
            qsort(process_data, process_count, sizeof(ProcessData), compare_by_pid);
        break;
        case SORT_BY_RESIDENT_MEMORY:
            qsort(process_data, process_count, sizeof(ProcessData), compare_by_resident_memory);
        break;
        case SORT_BY_VIRTUAL_MEMORY:
            qsort(process_data, process_count, sizeof(ProcessData), compare_by_virtual_memory);
        break;
    }

    clear(); // Очистка окна перед новым выводом
    display_header(color_scheme);
    int y = 2; // Текущая строка для вывода информации о процессе/потоке
    int line_count = 2; // Счетчик строк для вывода (учитываем строку заголовка)

    for (int i = 0; i < process_count && line_count < total_lines; i++) {
        if (line_count >= start_line && y < LINES) {
            move(y, 0);
            if (color_scheme == COLOR_SCHEME_INVERTED) {
                attron(COLOR_PAIR(2));
            } else {
                attron(COLOR_PAIR(1));
            }
            display_process_info(&process_data[i].process_info);
            y++;
        }
        line_count++;

        if (mode == SHOW_PROCESSES_AND_THREADS) {
            for (int j = 0; j < process_data[i].thread_count && line_count < total_lines; j++) {
                if (line_count >= start_line && y < LINES) {
                    move(y, 0);
                    if (color_scheme == COLOR_SCHEME_INVERTED) {
                        attron(COLOR_PAIR(2));
                    } else {
                        attron(COLOR_PAIR(1));
                    }
                    display_thread_info(&process_data[i].threads[j]);
                    y++;
                }
                line_count++;
            }
        }
    }
    refresh();
}

void display_help(ColorScheme color_scheme) {
    clear(); // Очищаем окно перед выводом справки

    // Определяем символы для заполнения пространства вокруг текста справки
    char fill_char = ' ';
    if (color_scheme == COLOR_SCHEME_INVERTED) {
        fill_char = '#'; // Для инвертированной схемы используем пробел
    } else {
        fill_char = '#'; // Для стандартной схемы используем решетку
    }

    // Определяем цвет заполнения в зависимости от выбранной темы
    int fill_color_pair = 1;
    if (color_scheme == COLOR_SCHEME_INVERTED) {
        fill_color_pair = 2; // В светлой теме цвет черный
    }

    // Получаем размеры окна
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);

    // Определяем координаты вывода для каждой строки
    int y_center = (max_y / 2)-2; // Центральная координата по вертикали
    int x_cetner = max_x / 2; // Центральная координата по горизонтали

    // Вывод справки
    mvprintw(y_center - 3, x_cetner - 6, "=== Help ===");
    mvprintw(y_center - 1, x_cetner - 20, "Press 'p' to sort by PID (process ID)");
    mvprintw(y_center, x_cetner - 20, "Press 'r' to sort by resident memory");
    mvprintw(y_center + 1, x_cetner - 20, "Press 'v' to sort by virtual memory");
    mvprintw(y_center + 2, x_cetner - 20, "Press 'k' to kill a process or thread");
    mvprintw(y_center + 3, x_cetner - 20, "Press 'q' to quit");

    // Добавленные строки
    mvprintw(y_center + 4, x_cetner - 20, "Press 't' to show threads");
    mvprintw(y_center + 5, x_cetner - 20, "Press 'key down' to go 20 lines down");
    mvprintw(y_center + 6, x_cetner - 20, "Press 'key up' to go 20 lines up");
    mvprintw(y_center + 7, x_cetner - 20, "Press 'z' to change color scheme");

    // Пустая строка
    mvprintw(y_center + 8, x_cetner - 20, "");

    // Заполняем пространство вокруг текста справки
    attron(COLOR_PAIR(fill_color_pair)); // Устанавливаем цвет для заполнения
    for (int i = 0; i < max_y; i++) {
        for (int j = 0; j < max_x; j++) {
            if (i < y_center - 4 || i > y_center + 8 || j < x_cetner - 30 || j > x_cetner + 30) {
                mvaddch(i, j, fill_char);
            }
        }
    }
    attroff(COLOR_PAIR(fill_color_pair)); // Отключаем цвет заполнения

    refresh();

    // Ожидаем нажатия любой клавиши или истечения времени
    timeout(30000); // Ожидание 30 секунд
    getch(); // Ожидаем нажатия клавиши
    clear(); // Очищаем экран после завершения отображения справки
    refresh();
}
