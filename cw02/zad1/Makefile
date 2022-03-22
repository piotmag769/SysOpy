CC=gcc
CFLAGS=-Wall #-Wextra -pedantic -Werror
REPORT_FILE=pomiar_zad_1.txt

lib: clean
	$(CC) $(CFLAGS) mainlib.c -o main

sys: clean
	$(CC) $(CFLAGS) mainsys.c -o main

# to check correctness - note that if last line of the fail contains text + newline,
# it will look as if last line is whitespace only while in reality it will be empty (EOF only)
lib_correctness_test: lib
	./main input.txt output.txt

sys_correctness_test: sys
	./main input.txt output.txt

time_test:
	make lib
	echo "Library time:\n" > $(REPORT_FILE)
	(time ./main biginput.txt bigoutput.txt) 2>> $(REPORT_FILE)

	make sys
	echo "\nSystem time:\n" >> $(REPORT_FILE)
	(time ./main biginput.txt bigoutput.txt) 2>> $(REPORT_FILE)

clean:
	rm -f main
