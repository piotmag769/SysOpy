CC=gcc
CFLAGS=-Wall #-Wextra -pedantic -Werror


# sleeps to ensure correct output sequence
tests: clean all process
	for uwu in ignore handler mask pending ; do \
		for owo in fork exec; do \
		  	echo $$uwu $$owo ;\
		  	sleep 0.5 ; \
			./main $$uwu $$owo; \
			sleep 0.5 ; \
		done ; \
	done


all:
	$(CC) $(CFLAGS) main.c -o main

process:
	$(CC) $(CFLAGS) process.c -o process

clean:
	rm -f main process