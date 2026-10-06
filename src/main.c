/**
 * ==============================================================================
 * main.c - Testing /proc/[pid]/stat Parsing
 * ==============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include "process.h"

int main(void) {
    DIR *dir = opendir("/proc");
    if (!dir) {
        perror("opendir /proc");
        return 1;
    }

    printf("%5s %5s %-5s %-8s %-10s %s\n", "PID", "PPID", "STATE", "TTY", "TIME(s)", "COMM");
    printf("------------------------------------------------------------\n");

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (is_pid_dir(entry->d_name)) {
            pid_t pid = (pid_t)atoi(entry->d_name);
            ProcessInfo proc;
            if (read_process_info(pid, &proc) == 0) {
                printf("%5d %5d %-5c %-8s %-10lu %s\n",
                       proc.pid, proc.ppid, proc.state, proc.tty, proc.total_time, proc.comm);
                free_process_info(&proc);
            }
        }
    }

    closedir(dir);
    return 0;
}
