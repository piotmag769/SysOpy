CC=gcc
CFLAGS=-Wall #-Wextra -pedantic -Werror

all: clean
	$(CC) $(CFLAGS) main.c -o main

# quit test if number of processes is n, check manually too
tests: clean
	make all
	./main 128 | wc -l
	./main 256 | wc -l
	./main 512 | wc -l

clean:
	rm -f main
