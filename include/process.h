/**
 * ==============================================================================
 * process.h - Data Structures and Functions for Reading Linux Process Info
 * ==============================================================================
 */

#ifndef PROCESS_H
#define PROCESS_H

#include <sys/types.h>  /* pid_t */
#include <stddef.h>     /* size_t */

/**
 * Checks whether a given directory entry name is purely numeric.
 * In Linux /proc, process folders are named with their numeric PID.
 *
 * @param name Directory name string (e.g., "1234" vs "cpuinfo")
 * @return 1 if name consists solely of digits, 0 otherwise.
 */
int is_pid_dir(const char *name);

/**
 * Prototype scanner: scans /proc and prints all discovered process IDs.
 */
void scan_pids(void);

#endif /* PROCESS_H */
