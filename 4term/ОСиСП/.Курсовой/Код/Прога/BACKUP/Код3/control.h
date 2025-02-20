#ifndef CONTROL_H
#define CONTROL_H

#include "process.h"
#include "threads.h"

void handle_signal(int signal);
void handle_input(Process *processes, int num_processes, Thread *threads, int num_threads);
void switch_color_mode();
void kill_process_by_pid(int pid);
void kill_thread_by_tid(int tid);
void sort_by_criterion();

#endif /* CONTROL_H */