#include "controlProcess.h"
#include <stdio.h>
#include <signal.h>

// Функция для завершения процесса по PID
int kill_process_by_pid(int pid) {
    if (kill(pid, SIGKILL) == 0) {
        printf("Процесс с PID %d завершен успешно.\n", pid);
        return 0;
    } else {
        perror("Ошибка при завершении процесса");
        return -1;
    }
}
