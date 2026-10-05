/**
 * ==============================================================================
 * process.c - Linux /proc Virtual Filesystem Scanner (Prototype)
 * ==============================================================================
 */

#define _GNU_SOURCE
#include "process.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>

int is_pid_dir(const char *name) {
    if (name == NULL || *name == '\0') {
        return 0;
    }

    for (size_t i = 0; name[i] != '\0'; i++) {
        if (!isdigit((unsigned char)name[i])) {
            return 0;
        }
    }

    return 1;
}

void scan_pids(void) {
    DIR *dir = opendir("/proc");
    if (!dir) {
        perror("Error opening /proc");
        return;
    }

    printf("Scanning /proc for active processes...\n");
    int count = 0;
    struct dirent *entry;

    while ((entry = readdir(dir)) != NULL) {
        if (is_pid_dir(entry->d_name)) {
            printf("Found PID: %s\n", entry->d_name);
            count++;
        }
    }

    closedir(dir);
    printf("Total active processes discovered: %d\n", count);
}
