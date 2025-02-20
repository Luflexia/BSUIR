#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include "processes.h"
#include "threads.h"
#include "display.h"
#include "processes.h"
#include "threads.h"

#define MAX_THREADS_PER_PROCESS 100  // Максимальное количество потоков на процесс



typedef enum {
    SORT_ORDER_ASCENDING,
    SORT_ORDER_DESCENDING
} SortOrder;

typedef enum {
    SORT_BY_PID,
    SORT_BY_RESIDENT_MEMORY,
    SORT_BY_VIRTUAL_MEMORY
} SortCriteria;

extern SortCriteria current_sort;

typedef enum {
    SHOW_PROCESSES,
    SHOW_PROCESSES_AND_THREADS
} DisplayMode;

typedef enum {
    COLOR_SCHEME_DEFAULT,
    COLOR_SCHEME_INVERTED
} ColorScheme;

extern SortCriteria current_sort;
void display_thread_info();
void display_header();
void display_process_info(const ProcessInfo *proc_info);
void update_display(int start_line, int total_lines, ProcessData *process_data, int process_count, DisplayMode mode, ColorScheme color_scheme);

#endif /* DISPLAY_H */