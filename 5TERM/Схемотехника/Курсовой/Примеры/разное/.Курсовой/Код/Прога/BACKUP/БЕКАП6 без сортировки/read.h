#ifndef READ_H
#define READ_H

#include "processes.h"
#include "threads.h"

void get_process_info(int pid, ProcessInfo *proc_info);
void get_thread_info(ThreadInfo *thread_info, int pid);

#endif // READ_H
