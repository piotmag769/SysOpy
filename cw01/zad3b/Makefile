CC=gcc
CFLAGS = -Wall # -Wextra -pedantic -Werror
OUTPUT_FILE=raport3b.txt
OPTIMIZATION_OPTION=-Os # simply change it from O0 to e.g. Os
# it would be better if i knew how to reassign variables in makefile...

# this rule appends to the file - if u want to empty it use make empty_file
all_tests: tests_static tests_dynamic tests_dll

tests_static: static
	echo '\nSTATIC TESTS:' $(OPTIMIZATION_OPTION) >> $(OUTPUT_FILE)
	make tests
	$(CC) $(OPTIMIZATION_OPTION) $(CFLAGS) -D ADDING_AND_REMOVING main.c -o main -L. -lmylib
	make last_test
	make clean

tests_dynamic: dynamic
	echo '\nDYNAMIC TESTS:' $(OPTIMIZATION_OPTION) >> $(OUTPUT_FILE)
	make tests
	$(CC) $(OPTIMIZATION_OPTION) $(CFLAGS) -D ADDING_AND_REMOVING main.c -o main libmylib.so -L. -Wl,-rpath=.
	make last_test
	make clean

tests_dll: dll
	echo '\nDLL TESTS:' $(OPTIMIZATION_OPTION) >> $(OUTPUT_FILE)
	make tests
	$(CC) $(OPTIMIZATION_OPTION) $(CFLAGS) main.c -o main -L. -ldl -D DLL -D ADDING_AND_REMOVING
	make last_test
	make clean

static: clean
	$(CC) -c mylib.c
	ar crs libmylib.a mylib.o
	$(CC) $(OPTIMIZATION_OPTION) $(CFLAGS) main.c -o main -L. -lmylib

dynamic: clean
	$(CC) $(CFLAGS) -shared -fPIC mylib.c -o libmylib.so
	$(CC) $(OPTIMIZATION_OPTION) $(CFLAGS) main.c -o main libmylib.so -L. -Wl,-rpath=.

dll: clean
	$(CC) $(CFLAGS) -shared -fPIC mylib.c -o libmylib.so
	$(CC) $(OPTIMIZATION_OPTION) $(CFLAGS) main.c -o main -L. -ldl -D DLL

clean:
	rm -f *.o *.a *.so main

empty_file:
	> $(OUTPUT_FILE)

tests:
	./main create_table 3 wc_files 3 large4.txt large5.txt small.txt \
	remove_block 0 >> $(OUTPUT_FILE)

	./main create_table 3 wc_files 5 large4.txt large5.txt small.txt medium.txt gigantic.txt \
	remove_block 0 >> $(OUTPUT_FILE)

	./main create_table 3 wc_files 8 large1.txt large2.txt large3.txt large4.txt large5.txt small.txt medium.txt gigantic.txt \
	remove_block 0 >> $(OUTPUT_FILE)

	./main create_table 3 wc_files 10 gigantic.txt gigantic.txt gigantic.txt gigantic.txt gigantic.txt gigantic.txt gigantic.txt \
    gigantic.txt gigantic.txt gigantic.txt remove_block 0 >> $(OUTPUT_FILE)

	./main create_table 3 wc_files 20 gigantic.txt gigantic.txt gigantic.txt gigantic.txt gigantic.txt gigantic.txt gigantic.txt \
    gigantic.txt gigantic.txt gigantic.txt gigantic.txt gigantic.txt gigantic.txt gigantic.txt gigantic.txt gigantic.txt gigantic.txt gigantic.txt \
  	gigantic.txt gigantic.txt remove_block 0 >> $(OUTPUT_FILE)

	./main create_table 4 wc_files 1 gigantic.txt wc_files 1 large1.txt wc_files 1 medium.txt wc_files 1 small.txt \
	remove_block 0 remove_block 1 remove_block 2 remove_block 3 >> $(OUTPUT_FILE)

# last test is separate bcs it has to have ADDING_AND_REMOVING macro defined
last_test:
	./main create_table 1 wc_files 10 gigantic.txt gigantic.txt gigantic.txt gigantic.txt gigantic.txt gigantic.txt gigantic.txt \
    gigantic.txt gigantic.txt gigantic.txt remove_block 0 wc_files 10 gigantic.txt gigantic.txt gigantic.txt \
    gigantic.txt gigantic.txt gigantic.txt gigantic.txt gigantic.txt gigantic.txt gigantic.txt remove_block 0 \
    wc_files 10 gigantic.txt gigantic.txt gigantic.txt gigantic.txt gigantic.txt gigantic.txt gigantic.txt gigantic.txt gigantic.txt \
    gigantic.txt remove_block 0 >> $(OUTPUT_FILE)
