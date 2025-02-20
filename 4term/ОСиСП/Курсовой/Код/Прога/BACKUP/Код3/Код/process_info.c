#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <unistd.h>
#include <pwd.h>

#define BUF_SIZE 512

struct ProcessInfo {
    int pid;
    char name[BUF_SIZE];
    char username[BUF_SIZE];
    long memory_usage;
    double cpu_usage;
};

void get_process_info(struct ProcessInfo *pinfo, const char *pid) {
    char proc_path[BUF_SIZE];
    char stat_path[BUF_SIZE];
    FILE *stat_file;
    char stat_buf[BUF_SIZE];
    char comm[BUF_SIZE];
    char state;
    int ppid;
    int priority;
    long unsigned int utime, stime;
    unsigned int vsize;
    long int rss;
    long int clock_ticks = sysconf(_SC_CLK_TCK);

    sprintf(proc_path, "/proc/%s/", pid);
    sprintf(stat_path, "/proc/%s/stat", pid);

    FILE *comm_file = fopen(strcat(proc_path, "comm"), "r");
    fgets(pinfo->name, BUF_SIZE, comm_file);
    fclose(comm_file);

    pinfo->pid = atoi(pid);

    stat_file = fopen(stat_path, "r");
    fgets(stat_buf, BUF_SIZE, stat_file);
    fclose(stat_file);
    sscanf(stat_buf, "%*d %s %c %d %*d %*d %*d %*d %*u %*u %*u %*u %*u %lu %lu %*d %*d %*d %*d %*d %*u %*u %u %ld",
        comm, &state, &ppid, &utime, &stime, &vsize, &rss);
    strncpy(pinfo->username, getpwuid(getuid())->pw_name, BUF_SIZE);
    pinfo->memory_usage = rss * getpagesize() / 1024;  // in kilobytes
    pinfo->cpu_usage = (double)(utime + stime) / clock_ticks;
}