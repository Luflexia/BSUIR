#ifndef DISPLAY_H
#define DISPLAY_H

#include <ncurses.h>
#include "processes.h"
#include "threads.h"

#define MAX_THREADS_PER_PROCESS 100  // Максимальное количество потоков на процесс

typedef struct {
    ProcessInfo process_info;
    ThreadInfo threads[MAX_THREADS_PER_PROCESS];
    int thread_count;
} ProcessData;

typedef enum {
    SHOW_PROCESSES,
    SHOW_PROCESSES_AND_THREADS
} DisplayMode;

typedef enum {
    COLOR_SCHEME_DEFAULT,
    COLOR_SCHEME_INVERTED
} ColorScheme;

void display_thread_info();
void display_header();
void display_process_info(const ProcessInfo *proc_info);
void update_display(int start_line, int total_lines, ProcessData *process_data, int process_count, DisplayMode mode, ColorScheme color_scheme);

#endif /* DISPLAY_H */
