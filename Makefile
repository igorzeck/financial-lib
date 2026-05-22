# - Flags -
CC = gcc
CFLAGS = -g
# -lm includes the math library
INC_FLAGS := -I./include -lm
BIN_DIR := ./bin/
TARGETS := main interactive


# - Variables -
# Conditional deletion command based on OS type
ifeq ($(OS),Windows_NT)
    RM = del /Q /F
	TARGETS := $(foreach wrd,$(TARGETS),$(BIN_DIR)$(wrd).exe)
else
    RM = rm -f
	TARGETS := $(foreach wrd,$(TARGETS),$(BIN_DIR)$(wrd))
endif

# - Targets -
# NOTE: For clarity the files name are explicitly set. May change in the future.

# Cannon test is on 'main' target
all: main test clean

# Default target
main: tests/main.c src/finance.c src/interface.c
	$(CC) $(CFLAGS) $^ $(INC_FLAGS) -o $(BIN_DIR)$@

# For interactive test
interactive: examples/interactive.c src/finance.c src/interface.c
	$(CC) $(CFLAGS) $^ $(INC_FLAGS) -o $(BIN_DIR)$@

# Launch target
launch: interactive
	$(BIN_DIR)$<

# Test target
test: main
	$(BIN_DIR)$<

# Cleaning
clean:
	$(RM) $(TARGETS)