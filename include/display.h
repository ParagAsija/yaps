/**
 * ==============================================================================
 * display.h - Process Table Formatting and CLI Flag Structures
 * ==============================================================================
 */

#ifndef DISPLAY_H
#define DISPLAY_H

#include "process.h"

typedef struct {
    int show_all;             /* -e / -A */
    int all_with_terminal;    /* -a */
    int full_format;          /* -f */
    pid_t filter_pid;         /* -p <pid> */
    char filter_user[32];     /* -u <user> */
} DisplayOptions;

void print_usage(const char *prog_name);
void print_processes(const ProcessList *list, const DisplayOptions *opts, int current_tty_nr);
void format_time(unsigned long total_seconds, char *out_buf, size_t buf_len);

#endif /* DISPLAY_H */
