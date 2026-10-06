/**
 * ==============================================================================
 * process.c - Linux /proc Virtual Filesystem Parser
 * ==============================================================================
 */

#define _GNU_SOURCE
#include "process.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>
#include <unistd.h>

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

void format_tty_name(int tty_nr, char *out_buf, size_t buf_len) {
    if (tty_nr <= 0) {
        snprintf(out_buf, buf_len, "?");
        return;
    }

    int major = (tty_nr >> 8) & 0xff;
    int minor = (tty_nr & 0xff) | ((tty_nr >> 12) & 0xfff00);

    if (major >= 136 && major <= 143) {
        snprintf(out_buf, buf_len, "pts/%d", minor);
    } else if (major == 4) {
        if (minor < 64) {
            snprintf(out_buf, buf_len, "tty%d", minor);
        } else {
            snprintf(out_buf, buf_len, "ttyS%d", minor - 64);
        }
    } else if (major == 5) {
        if (minor == 0) {
            snprintf(out_buf, buf_len, "tty");
        } else if (minor == 1) {
            snprintf(out_buf, buf_len, "console");
        } else {
            snprintf(out_buf, buf_len, "?");
        }
    } else if (major == 229) {
        snprintf(out_buf, buf_len, "hvc%d", minor);
    } else {
        snprintf(out_buf, buf_len, "?");
    }
}

int read_process_info(pid_t pid, ProcessInfo *proc) {
    if (!proc) {
        return -1;
    }

    memset(proc, 0, sizeof(ProcessInfo));
    proc->pid = pid;

    char stat_path[MAX_PATH_LEN];
    snprintf(stat_path, sizeof(stat_path), "/proc/%d/stat", pid);

    FILE *fp = fopen(stat_path, "r");
    if (!fp) {
        return -1;
    }

    char buffer[2048];
    if (!fgets(buffer, sizeof(buffer), fp)) {
        fclose(fp);
        return -1;
    }
    fclose(fp);

    /* Locate opening and closing parentheses for command name */
    char *open_paren = strchr(buffer, '(');
    char *close_paren = strrchr(buffer, ')');

    if (!open_paren || !close_paren || open_paren >= close_paren) {
        return -1;
    }

    size_t comm_len = (size_t)(close_paren - open_paren - 1);
    if (comm_len >= sizeof(proc->comm)) {
        comm_len = sizeof(proc->comm) - 1;
    }
    memcpy(proc->comm, open_paren + 1, comm_len);
    proc->comm[comm_len] = '\0';

    char *rest = close_paren + 1;

    int pgrp, session, tpgid;
    unsigned int flags;
    unsigned long minflt, cminflt, majflt, cmajflt;
    long cutime, cstime, priority, nice, num_threads, itrealvalue;
    unsigned long long starttime;
    unsigned long vsize = 0;
    long rss = 0;

    int parsed = sscanf(rest,
        " %c %d %d %d %d %d %u %lu %lu %lu %lu %lu %lu %ld %ld %ld %ld %ld %ld %llu %lu %ld",
        &proc->state,
        &proc->ppid,
        &pgrp,
        &session,
        &proc->tty_nr,
        &tpgid,
        &flags,
        &minflt, &cminflt, &majflt, &cmajflt,
        &proc->utime,
        &proc->stime,
        &cutime, &cstime,
        &priority,
        &nice,
        &num_threads,
        &itrealvalue,
        &starttime,
        &vsize,
        &rss
    );

    if (parsed < 5) {
        return -1;
    }

    format_tty_name(proc->tty_nr, proc->tty, sizeof(proc->tty));

    long ticks_per_sec = sysconf(_SC_CLK_TCK);
    if (ticks_per_sec <= 0) {
        ticks_per_sec = 100;
    }
    proc->total_time = (proc->utime + proc->stime) / (unsigned long)ticks_per_sec;

    proc->vsize_kb = vsize / 1024;
    long page_size = sysconf(_SC_PAGESIZE);
    if (page_size <= 0) {
        page_size = 4096;
    }
    proc->rss_kb = (rss * page_size) / 1024;

    return 0;
}

void free_process_info(ProcessInfo *proc) {
    if (proc && proc->cmdline) {
        free(proc->cmdline);
        proc->cmdline = NULL;
    }
}
