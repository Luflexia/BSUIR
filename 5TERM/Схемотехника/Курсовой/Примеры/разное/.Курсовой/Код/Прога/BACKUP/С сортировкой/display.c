#include "display.h"

void display_thread_info(const ThreadInfo *thread_info) {
    printw("%-5d %-8s %-5c %-9s %-9s %-4s %-10s -%s\n",
           thread_info->tid, "", thread_info->state, "", "", "", "", thread_info->name);
}

void display_header() {
    printw("%-5s %-8s %-5s %-9s %-9s %-5s %-9s %s\n",
           "PID", "USER", "STATE", "RES_MEM", "VIRT_MEM", "CORES", "START", "COMMAND");
}


void display_process_info(const ProcessInfo *proc_info) {
    // Обрезаем имя пользователя до 8 символов
    printw("%-5d %-8.8s %-5c %-6.1f MB %-5.1f MB %-5d %-9.8s ",
           proc_info->pid, proc_info->user, proc_info->state,
           proc_info->resident_memory, proc_info->virtual_memory,
           proc_info->cpu_cores, proc_info->start_time + 11);

    // Обрезаем имя команды до символа '/'
    char *slash_pos = strchr(proc_info->command, '/');
    if (slash_pos) {
        *slash_pos = '\0'; // Устанавливаем символ '/' как конец строки
    }
    printw("%s\n", proc_info->command);
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
    display_header();
    int y = 1; // Текущая строка для вывода информации о процессе/потоке
    int line_count = 1; // Счетчик строк для вывода (учитываем строку заголовка)

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