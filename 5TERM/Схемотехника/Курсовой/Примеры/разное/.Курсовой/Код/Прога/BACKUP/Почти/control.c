#include <curses.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include "display.h"
#include "control.h"

SortCriteria current_sort = SORT_BY_PID; // начальное значение
SortOrder sort_order = SORT_ORDER_ASCENDING; // начальный порядок сортировки

void kill_process_or_thread() {
    char input[10];
    int id = -1;

    echo(); // Включаем отображение вводимых символов
    curs_set(1); // Включаем курсор
    timeout(15000); // Устанавливаем время ожидания ввода 15 секунд

    while (1) {
        // Очищаем строку ввода и выводим приглашение к вводу
        move(1, 0);
        clrtoeol();
        printw("Enter TID or PID to kill (To EXIT press Enter): ");
        refresh();

        // Ввод ID
        if (getnstr(input, sizeof(input) - 1) == ERR) {
            // Если время истекло и пользователь не ввел ID, выходим из функции
            break;
        }
        id = atoi(input);

        // Проверка на выход
        if (id == 0) {
            break;
        }

        // Попытка убить процесс/поток
        move(2, 0); // Перемещаем курсор под строку ввода
        clrtoeol(); // Очищаем строку

        if (id > 0 && kill(id, SIGKILL) == 0) {
            printw("Successfully killed process/thread with ID %d\n", id);
        } else {
            printw("Failed to kill process/thread with ID %d\n", id);
        }
        refresh();
    }
    noecho(); // Отключаем отображение вводимых символов
    curs_set(0); // Выключаем курсор
    clear(); // Очищаем экран после завершения режима
    refresh();
}

int compare_by_pid(const void *a, const void *b) {
    const ProcessData *p1 = (const ProcessData *)a;
    const ProcessData *p2 = (const ProcessData *)b;
    return (sort_order == SORT_ORDER_ASCENDING) ?
           (p1->process_info.pid - p2->process_info.pid) :
           (p2->process_info.pid - p1->process_info.pid);
}

int compare_by_resident_memory(const void *a, const void *b) {
    const ProcessData *p1 = (const ProcessData *)a;
    const ProcessData *p2 = (const ProcessData *)b;
    if (p1->process_info.resident_memory < p2->process_info.resident_memory)
        return (sort_order == SORT_ORDER_ASCENDING) ? -1 : 1;
    if (p1->process_info.resident_memory > p2->process_info.resident_memory)
        return (sort_order == SORT_ORDER_ASCENDING) ? 1 : -1;
    return 0;
}

int compare_by_virtual_memory(const void *a, const void *b) {
    const ProcessData *p1 = (const ProcessData *)a;
    const ProcessData *p2 = (const ProcessData *)b;
    if (p1->process_info.virtual_memory < p2->process_info.virtual_memory)
        return (sort_order == SORT_ORDER_ASCENDING) ? -1 : 1;
    if (p1->process_info.virtual_memory > p2->process_info.virtual_memory)
        return (sort_order == SORT_ORDER_ASCENDING) ? 1 : -1;
    return 0;
}

void handle_user_input(int ch) {
    switch (ch) {
        case 'p':
            if (current_sort == SORT_BY_PID) {
                sort_order = (sort_order == SORT_ORDER_ASCENDING) ? SORT_ORDER_DESCENDING : SORT_ORDER_ASCENDING;
            } else {
                current_sort = SORT_BY_PID;
                sort_order = SORT_ORDER_ASCENDING;
            }
        break;
        case 'r':
            if (current_sort == SORT_BY_RESIDENT_MEMORY) {
                sort_order = (sort_order == SORT_ORDER_ASCENDING) ? SORT_ORDER_DESCENDING : SORT_ORDER_ASCENDING;
            } else {
                current_sort = SORT_BY_RESIDENT_MEMORY;
                sort_order = SORT_ORDER_ASCENDING;
            }
        break;
        case 'v':
            if (current_sort == SORT_BY_VIRTUAL_MEMORY) {
                sort_order = (sort_order == SORT_ORDER_ASCENDING) ? SORT_ORDER_DESCENDING : SORT_ORDER_ASCENDING;
            } else {
                current_sort = SORT_BY_VIRTUAL_MEMORY;
                sort_order = SORT_ORDER_ASCENDING;
            }
        break;
        case 'k':
            kill_process_or_thread();
        break;
        case 'q':
            endwin();
        exit(0);
        default:
            break;
    }
}