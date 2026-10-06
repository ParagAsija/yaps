/**
 * ==============================================================================
 * process.h - Data Structures and Functions for Reading Linux Process Info
 * ==============================================================================
 */

#ifndef PROCESS_H
#define PROCESS_H

#include <sys/types.h>  /* pid_t, uid_t */
#include <stddef.h>     /* size_t */

#define MAX_COMM_LEN   64
#define MAX_USER_LEN   32
#define MAX_TTY_LEN    32
#define MAX_PATH_LEN  256

/**
 * ProcessInfo
 * Represents collected metadata for a single running process.
 */
typedef struct {
    pid_t pid;                 /* Process ID */
    pid_t ppid;                /* Parent Process ID */
    uid_t uid;                 /* User ID */
    char user[MAX_USER_LEN];   /* Username */
    char comm[MAX_COMM_LEN];   /* Short executable name */
    char *cmdline;             /* Full command line */
    char state;                /* Process state ('R', 'S', 'Z', etc.) */
    int tty_nr;                /* Controlling terminal number */
    char tty[MAX_TTY_LEN];     /* Decoded TTY name (e.g. "pts/1" or "?") */
    unsigned long utime;       /* User-space CPU ticks */
    unsigned long stime;       /* Kernel-space CPU ticks */
    unsigned long total_time;  /* Total CPU time in seconds */
    unsigned long vsize_kb;    /* Virtual memory size (KB) */
    long rss_kb;               /* Resident Set Size (KB) */
} ProcessInfo;

int is_pid_dir(const char *name);
int read_process_info(pid_t pid, ProcessInfo *proc);
void free_process_info(ProcessInfo *proc);
void format_tty_name(int tty_nr, char *out_buf, size_t buf_len);

#endif /* PROCESS_H */
