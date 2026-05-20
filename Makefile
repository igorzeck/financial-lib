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
	rm -f out interactive