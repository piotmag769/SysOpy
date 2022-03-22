CC=gcc
CFLAGS=-Wall #-Wextra -pedantic -Werror

lib: clean
	$(CC) $(CFLAGS) mainlib.c -o main

test: lib
	echo "Library time:\n"
	./main ../

clean:
	rm -f main
