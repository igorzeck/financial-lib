# Conditional deletion command based on OS type
ifeq ($(OS),Windows_NT)
    RM = del /Q /F
    TARGETS = out.exe interactive.exe
else
    RM = rm -f
    TARGETS = out interactive
endif

# Default target
main: tests/main.c src/finance.c src/interface.c
	gcc -g tests/main.c src/finance.c src/interface.c -I./include -lm -o out

# For interactive test
interactive: examples/interactive.c src/finance.c src/interface.c
	gcc -g examples/interactive.c src/finance.c src/interface.c -I./include -lm -o interactive

# Launch target
launch: out
	./out

# Cleaning
clean:
	$(RM) $(TARGETS)