# Conditional deletion command based on OS type
ifeq ($(OS),Windows_NT)
    RM = del /Q /F
    TARGETS = bin/out.exe bin/interactive.exe
else
    RM = rm -f
    TARGETS = bin/out bin/interactive
endif

# Default target
main: tests/main.c src/finance.c src/interface.c
	gcc -g tests/main.c src/finance.c src/interface.c -I./include -lm -o bin/out

# For interactive test
interactive: examples/interactive.c src/finance.c src/interface.c
	gcc -g examples/interactive.c src/finance.c src/interface.c -I./include -lm -o bin/interactive

# Launch target
launch: interactive
	./bin/interactive

test: out
	./bin/out

# Cleaning
clean:
	$(RM) $(TARGETS)