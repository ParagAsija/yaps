/**
 * ==============================================================================
 * main.c - Entry Point for the 'myps' Process Status Utility
 * ==============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "process.h"
#include "display.h"

int main(int argc, char *argv[]) {
    DisplayOptions opts;
    opts.show_all = 0;
    opts.all_with_terminal = 0;
    opts.full_format = 0;
    opts.filter_pid = -1;
    opts.filter_user[0] = '\0';

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--help") == 0) {
            print_usage(argv[0]);
            return 0;
        }
    }

    int opt;
    while ((opt = getopt(argc, argv, "eAafp:u:h")) != -1) {
        switch (opt) {
            case 'e':
            case 'A':
                opts.show_all = 1;
                break;

            case 'a':
                opts.all_with_terminal = 1;
                break;

            case 'f':
                opts.full_format = 1;
                break;

            case 'p':
                opts.filter_pid = (pid_t)atoi(optarg);
                if (opts.filter_pid <= 0) {
                    fprintf(stderr, "Error: Invalid process ID '%s'. PID must be a positive integer.\n", optarg);
                    return 1;
                }
                break;

            case 'u':
                strncpy(opts.filter_user, optarg, sizeof(opts.filter_user) - 1);
                opts.filter_user[sizeof(opts.filter_user) - 1] = '\0';
                break;

            case 'h':
                print_usage(argv[0]);
                return 0;

            case '?':
                fprintf(stderr, "Run '%s -h' or '%s --help' for usage instructions.\n", argv[0], argv[0]);
                return 1;

            default:
                break;
        }
    }

    int current_tty = get_current_terminal_nr();

    ProcessList *list = get_all_processes();
    if (!list) {
        fprintf(stderr, "Failed to inspect processes from /proc.\n");
        return 1;
    }

    print_processes(list, &opts, current_tty);
    free_process_list(list);

    return 0;
}
