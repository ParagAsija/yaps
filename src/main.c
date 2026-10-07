/**
 * ==============================================================================
 * main.c - Testing Dynamic ProcessList and Full Metadata
 * ==============================================================================
 */

#include <stdio.h>
#include "process.h"

int main(void) {
    int current_tty = get_current_terminal_nr();
    char current_tty_name[32];
    format_tty_name(current_tty, current_tty_name, sizeof(current_tty_name));
    printf("Current terminal: %s (device nr: %d)\n\n", current_tty_name, current_tty);

    ProcessList *list = get_all_processes();
    if (!list) {
        fprintf(stderr, "Failed to inspect processes.\n");
        return 1;
    }

    printf("%-10s %5s %5s %-8s %-8s %s\n", "USER", "PID", "PPID", "TTY", "TIME", "CMDLINE");
    printf("----------------------------------------------------------------------\n");

    for (size_t i = 0; i < list->count && i < 25; i++) {
        ProcessInfo *p = &list->items[i];
        printf("%-10s %5d %5d %-8s %-8lu %s\n",
               p->user, p->pid, p->ppid, p->tty, p->total_time,
               p->cmdline ? p->cmdline : p->comm);
    }

    printf("\nCollected %zu total processes into heap memory.\n", list->count);

    free_process_list(list);
    return 0;
}
