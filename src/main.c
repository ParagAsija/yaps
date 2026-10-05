/**
 * ==============================================================================
 * main.c - Entry Point for the 'myps' Process Status Utility
 * ==============================================================================
 */

#include <stdio.h>
#include "process.h"

int main(void) {
    printf("myps - Lightweight Process Status Monitor (v0.1.0-alpha)\n");
    printf("========================================================\n");
    scan_pids();
    return 0;
}
