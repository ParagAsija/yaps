/**
 * ==============================================================================
 * main.c - Integrating Display Engine
 * ==============================================================================
 */

#include <stdio.h>
#include "process.h"
#include "display.h"

int main(void) {
    DisplayOptions opts;
    opts.show_all = 1;
    opts.all_with_terminal = 0;
    opts.full_format = 0;
    opts.filter_pid = -1;
    opts.filter_user[0] = '\0';

    int current_tty = get_current_terminal_nr();
    ProcessList *list = get_all_processes();

    if (!list) {
        fprintf(stderr, "Failed to load processes.\n");
        return 1;
    }

    print_processes(list, &opts, current_tty);
    free_process_list(list);
    return 0;
}
