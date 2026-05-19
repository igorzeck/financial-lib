# Default target
out: tests/main.c src/finance.c
	gcc tests/main.c src/finance.c -I./include -lm -o out

# Cleaning
clean:
	rm -f out