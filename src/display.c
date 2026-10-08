/**
 * ==============================================================================
 * display.c - Process Table Formatting and Terminal Rendering
 * ==============================================================================
 */

#include "display.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void format_time(unsigned long total_seconds, char *out_buf, size_t buf_len) {
    unsigned long hours = total_seconds / 3600;
    unsigned long minutes = (total_seconds % 3600) / 60;
    unsigned long seconds = total_seconds % 60;

    snprintf(out_buf, buf_len, "%02lu:%02lu:%02lu", hours, minutes, seconds);
}

static int compare_process_by_pid(const void *a, const void *b) {
    const ProcessInfo *proc_a = (const ProcessInfo *)a;
    const ProcessInfo *proc_b = (const ProcessInfo *)b;

    if (proc_a->pid < proc_b->pid) return -1;
    if (proc_a->pid > proc_b->pid) return 1;
    return 0;
}

void print_processes(const ProcessList *list, const DisplayOptions *opts, int current_tty_nr) {
    (void)current_tty_nr; /* Will be used in Day 5 for terminal filtering */
    if (!list || list->count == 0) {
        printf("No processes found.\n");
        return;
    }

    qsort(list->items, list->count, sizeof(ProcessInfo), compare_process_by_pid);

    char time_str[32];

    if (opts->full_format) {
        printf("%-10s %5s %5s %-8s %-8s %s\n",
               "UID", "PID", "PPID", "TTY", "TIME", "CMD");
    } else {
        printf("%5s %-8s %-8s %s\n",
               "PID", "TTY", "TIME", "CMD");
    }

    for (size_t i = 0; i < list->count; i++) {
        const ProcessInfo *proc = &list->items[i];
        format_time(proc->total_time, time_str, sizeof(time_str));

        const char *cmd_display = opts->full_format ?
                                  (proc->cmdline ? proc->cmdline : proc->comm) :
                                  proc->comm;

        if (opts->full_format) {
            printf("%-10s %5d %5d %-8s %-8s %s\n",
                   proc->user, proc->pid, proc->ppid, proc->tty, time_str, cmd_display);
        } else {
            printf("%5d %-8s %-8s %s\n",
                   proc->pid, proc->tty, time_str, cmd_display);
        }
    }
}

void print_usage(const char *prog_name) {
    printf("myps - A lightweight, educational process status utility for Linux\n\n");
    printf("Usage:\n  %s [options]\n", prog_name);
}
