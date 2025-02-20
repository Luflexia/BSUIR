#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>

#define BUF_SIZE 512

struct ThreadInfo {
    int tid;
    int pid;
    char name[BUF_SIZE];
};

void get_thread_info(int pid, struct ThreadInfo *tinfo) {
    // ...
}