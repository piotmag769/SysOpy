CC=gcc
CFLAGS=-Wall #-Wextra -pedantic -Werror


# sleeps to ensure correct output sequence
tests: clean all
	for uwu in SA_NOCLDWAIT SA_NOCLDSTOP SA_SIGINFO ; do \
  		echo $$uwu ;\
		./main $$uwu ;\
	done


all:
	$(CC) $(CFLAGS) main.c -o main

clean:
	rm -f main