#define _DEFAULT_SOURCE
#define _POSIX_C_SOURCE
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

#define INTERVAL 100000000L

struct pair {
    int x, y;
};

struct pair pair;
int statOl = 0, statOO = 0, statll = 0, statlO = 0;
long counter = 0;
bool output_allowed = true;

// Alarm signal
void alarm_handler(int signo) {
    if (signo != SIGALRM)
        return;

    if (pair.x > pair.y)
        statlO++;
    else if (pair.x < pair.y)
        statOl++;
    else if (pair.x && pair.y)
        statll++;
    else
        statOO++;

    return;
}

// Stop output signal
void sig1_handler(int signo) {
    if (signo != SIGUSR1)
        return;

    output_allowed = false;
    return;
}

// Allow output signal
void sig2_handler(int signo) {
    if (signo != SIGUSR2)
        return;

    output_allowed = true;
    return;
}

int main(int argc, char** argv) {
    // Redifinition of signals
    signal(SIGALRM, alarm_handler);
    signal(SIGUSR1, sig1_handler);
    signal(SIGUSR2, sig2_handler);

    char mes[200] = {'\0'};

    // Set timer
    struct itimerval timerval;
    timerval.it_interval.tv_sec = timerval.it_value.tv_sec = 0;
    timerval.it_interval.tv_usec = timerval.it_value.tv_usec = 500;

    setitimer(ITIMER_REAL, &timerval, NULL);

    while (1) {
        pair.x = pair.y = counter % 2;

        // Check and output
        if (++counter == INTERVAL) {
            if (output_allowed == true) {
                sprintf(mes, "NAME:%s PPID:%d PID:%d 00:%d 01:%d 10:%d 11:%d\n", argv[0], getppid(), getpid(), statOO, statOl, statlO, statll);
                for (size_t i = 0; i < strlen(mes); i++)
                    fputc(mes[i], stdout);
            }
            statOO = statOl = statlO = statll = counter = 0;
        }
    }

    return 0;
}