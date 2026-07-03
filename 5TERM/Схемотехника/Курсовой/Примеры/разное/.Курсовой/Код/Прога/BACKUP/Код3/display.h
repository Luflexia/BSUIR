#ifndef DISPLAY_H
#define DISPLAY_H

#include "process.h"

void display_sysinfo(SysInfo *sysinfo);
void display_processes(Process *processes, int num_processes);
void display_threads(Thread *threads, int num_threads);

#endif /* DISPLAY_H */
