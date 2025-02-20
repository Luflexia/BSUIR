#ifndef READ_H
#define READ_H

#include "processes.h"
#include "threads.h"


void get_thread_info(ThreadInfo *thread_info, int pid, int tid);
void get_process_info(ProcessInfo *proc_info, int pid);
#endif // READ_H
