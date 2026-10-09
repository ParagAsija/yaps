# yaps — Lightweight Linux Process Status Utility (`myps`)

A modular, high-performance implementation of the standard Unix `ps` utility written in pure C99 for Linux.

`yaps` (`myps`) inspects the Linux `/proc` virtual pseudo-filesystem directly, parsing kernel data structures without external dependencies to deliver process monitoring and inspection.

---

## Features

- **Direct `/proc` Virtual Filesystem Parsing**: Directly reads `/proc/[pid]/stat`, `/proc/[pid]/status`, and `/proc/[pid]/cmdline`.
- **Robust Parenthesis Boundary Matching**: Accurately handles edge cases where process names contain spaces or nested parentheses (e.g., `(cat (1))` or `(Web Content)`).
- **TTY Number Decoding**: Converts Linux kernel terminal device numbers (major/minor encoding) into readable paths (`pts/0`, `tty1`, `console`, `?`).
- **User Resolution**: Maps numeric process `UID` values to system usernames using `getpwuid()`.
- **Dynamic Array Architecture**: Safely reallocates heap memory (`malloc`/`realloc`) to dynamically scale with hundreds of running processes.
- **POSIX Argument Parsing**: Full support for standard POSIX command flags (`-e`, `-A`, `-a`, `-f`, `-u`, `-p`, `-h`).
- **Memory Safe**: Zero memory leaks, validated with explicit cleanup routines.

---

## Directory Structure

```text
.
├── Makefile            # Automated compilation and linking recipes
├── LICENSE             # MIT Open Source License
├── README.md           # Documentation and architecture guide
├── include/
│   ├── process.h       # Kernel data structures and /proc parser prototypes
│   └── display.h       # Terminal formatting and CLI flag prototypes
└── src/
    ├── main.c          # Entry point, POSIX getopt() argument handling
    ├── process.c       # /proc parser, stat/status reader, dynamic ProcessList
    └── display.c       # Table rendering, time formatting, and qsort() comparator
```

---

## Compilation & Installation

Compile using GCC and GNU Make:

```bash
# Build the project
make

# Clean build artifacts
make clean

# Build and execute immediately
make run
```

---

## Usage Examples

```bash
# Show processes in the current terminal (default behavior)
./myps

# Show all active processes on the system
./myps -e

# Full format listing (UID, PID, PPID, TTY, TIME, full command line)
./myps -ef

# Show all processes attached to any terminal
./myps -a

# Filter processes owned by a specific user
./myps -u root

# Filter a single process by its PID
./myps -p 1

# Display usage instructions and help
./myps -h
```
