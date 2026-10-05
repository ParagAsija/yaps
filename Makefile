# ==============================================================================
# Makefile for 'myps' Process Status Utility
# ==============================================================================

CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=gnu99 -Iinclude -O2

SRCDIR = src
INCDIR = include
OBJDIR = obj
BINDIR = bin

TARGET = $(BINDIR)/myps

SRCS = $(wildcard $(SRCDIR)/*.c)
OBJS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRCS))
HEADERS = $(wildcard $(INCDIR)/*.h)

.PHONY: all clean run help

all: $(TARGET)
	@ln -sf $(TARGET) myps
	@echo "Build complete! Executable available at ./myps and $(TARGET)"

$(TARGET): $(OBJS) | $(BINDIR)
	@echo "[LINK] $@"
	$(CC) $(CFLAGS) $^ -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(HEADERS) | $(OBJDIR)
	@echo "[CC]   $<"
	$(CC) $(CFLAGS) -c $< -o $@

$(BINDIR):
	@mkdir -p $(BINDIR)

$(OBJDIR):
	@mkdir -p $(OBJDIR)

clean:
	@echo "Cleaning build artifacts..."
	rm -rf $(OBJDIR) $(BINDIR) myps
	@echo "Clean complete."

run: all
	@echo "Running ./myps:"
	./myps

help:
	@echo "myps Makefile targets:"
	@echo "  make         - Compiles the project and produces ./myps"
	@echo "  make clean   - Deletes compiled object files and binaries"
	@echo "  make run     - Compiles and runs ./myps"
	@echo "  make help    - Displays this help message"
