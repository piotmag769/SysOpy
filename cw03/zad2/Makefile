CC=gcc
CFLAGS=-Wall #-Wextra -pedantic -Werror
REPORT_FILE=raport2.txt

all: clean
	$(CC) $(CFLAGS) main.c -o main -lm

tests: all
	echo "Precision: 0.000000001:\n" > $(REPORT_FILE)
	echo "\n1 process:\n" >> $(REPORT_FILE)
	(time ./main 0.000000001 1) 2>> $(REPORT_FILE)
	echo "\n10 processes:\n" >> $(REPORT_FILE)
	(time ./main 0.000000001 10) 2>> $(REPORT_FILE)
	echo "\n100 process:\n" >> $(REPORT_FILE)
	(time ./main 0.000000001 100) 2>> $(REPORT_FILE)
	echo "\n500 process:\n" >> $(REPORT_FILE)
	(time ./main 0.000000001 500) 2>> $(REPORT_FILE)

	echo "\n\n\nPrecision: 0.0000002:\n" >> $(REPORT_FILE)
	echo "\n1 process:\n" >> $(REPORT_FILE)
	(time ./main 0.0000002 1) 2>> $(REPORT_FILE)
	echo "\n10 processes:\n" >> $(REPORT_FILE)
	(time ./main 0.0000002 10) 2>> $(REPORT_FILE)
	echo "\n100 process:\n" >> $(REPORT_FILE)
	(time ./main 0.0000002 100) 2>> $(REPORT_FILE)
	echo "\n500 process:\n" >> $(REPORT_FILE)
	(time ./main 0.0000002 500) 2>> $(REPORT_FILE)

	make clean


clean:
	rm -f main w*.txt
