#include "controlThreads.h"
#include <stdio.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <signal.h>

// Функция для завершения потока по TID
int kill_thread_by_tid(int tid) {
    if (syscall(SYS_tkill, tid, SIGKILL) == 0) {
        printf("Поток с TID %d завершен успешно.\n", tid);
        return 0;
    } else {
        perror("Ошибка при завершении потока");
        return -1;
    }
}
