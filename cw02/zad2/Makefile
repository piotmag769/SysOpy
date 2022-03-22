CC=gcc
CFLAGS=-Wall #-Wextra -pedantic -Werror
CHARACTER=o
REPORT_FILE=pomiar_zad_2.txt

lib: clean
	$(CC) $(CFLAGS) mainlib.c -o main

sys: clean
	$(CC) $(CFLAGS) mainsys.c -o main

lib_correctness_test: lib
	./main $(CHARACTER) input.txt

sys_correctness_test: sys
	./main $(CHARACTER) input.txt

time_test:
	make lib
	echo "Library time:\n" > $(REPORT_FILE)
	(time ./main $(CHARACTER) biginput.txt) 2>> $(REPORT_FILE)

	make sys
	echo "\nSystem time:\n" >> $(REPORT_FILE)
	(time ./main $(CHARACTER) biginput.txt) 2>> $(REPORT_FILE)

clean:
	rm -f main
