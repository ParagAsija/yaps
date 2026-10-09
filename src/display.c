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

static int should_display(const ProcessInfo *proc, const DisplayOptions *opts, int current_tty_nr) {
    if (opts->filter_pid != -1) {
        return (proc->pid == opts->filter_pid);
    }

    if (opts->filter_user[0] != '\0') {
        if (strcmp(proc->user, opts->filter_user) != 0) {
            return 0;
        }
    }

    if (opts->show_all) {
        return 1;
    }

    if (opts->all_with_terminal) {
        return (proc->tty_nr > 0);
    }

    if (current_tty_nr > 0) {
        return (proc->tty_nr == current_tty_nr);
    }

    return 1;
}

void print_processes(const ProcessList *list, const DisplayOptions *opts, int current_tty_nr) {
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

        if (!should_display(proc, opts, current_tty_nr)) {
            continue;
        }

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
    printf("Usage:\n");
    printf("  %s [options]\n\n", prog_name);
    printf("Options:\n");
    printf("  -e, -A          Select all processes across the entire system\n");
    printf("  -a              Select all processes attached to any terminal\n");
    printf("  -f              Full-format listing (shows UID, PID, PPID, etc.)\n");
    printf("  -u <username>   Select processes owned by a specific username\n");
    printf("  -p <pid>        Select a single process by its numerical PID\n");
    printf("  -h, --help      Display this help manual\n\n");
    printf("Examples:\n");
    printf("  %s              # Show processes attached to the current terminal\n", prog_name);
    printf("  %s -e           # Show all active processes on the system\n", prog_name);
    printf("  %s -ef          # Show all processes with detailed columns\n", prog_name);
    printf("  %s -u root      # Show all processes owned by user 'root'\n", prog_name);
    printf("  %s -p 1         # Show details for init/systemd (PID 1)\n", prog_name);
}
