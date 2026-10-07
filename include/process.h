/**
 * ==============================================================================
 * process.h - Data Structures and Functions for Reading Linux Process Info
 * ==============================================================================
 */

#ifndef PROCESS_H
#define PROCESS_H

#include <sys/types.h>
#include <stddef.h>

#define MAX_COMM_LEN   64
#define MAX_USER_LEN   32
#define MAX_TTY_LEN    32
#define MAX_PATH_LEN  256

typedef struct {
    pid_t pid;
    pid_t ppid;
    uid_t uid;
    char user[MAX_USER_LEN];
    char comm[MAX_COMM_LEN];
    char *cmdline;
    char state;
    int tty_nr;
    char tty[MAX_TTY_LEN];
    unsigned long utime;
    unsigned long stime;
    unsigned long total_time;
    unsigned long vsize_kb;
    long rss_kb;
} ProcessInfo;

/**
 * ProcessList
 * Dynamic heap-allocated array that automatically resizes.
 */
typedef struct {
    ProcessInfo *items;
    size_t count;
    size_t capacity;
} ProcessList;

int is_pid_dir(const char *name);
int read_process_info(pid_t pid, ProcessInfo *proc);
void free_process_info(ProcessInfo *proc);
ProcessList* get_all_processes(void);
void free_process_list(ProcessList *list);
int get_current_terminal_nr(void);
void format_tty_name(int tty_nr, char *out_buf, size_t buf_len);

#endif /* PROCESS_H */
