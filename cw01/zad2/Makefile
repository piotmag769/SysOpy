CC=gcc
CFLAGS = -Wall # -Wextra -pedantic -Werror
OUTPUT_FILE=raport2.txt

tests:
	make static

	./main create_table 3 wc_files 3 large4.txt large5.txt small.txt \
	remove_block 0 > $(OUTPUT_FILE)

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

	$(CC) $(CFLAGS) -D ADDING_AND_REMOVING main.c -o main -L. -lmylib

	./main create_table 1 wc_files 10 gigantic.txt gigantic.txt gigantic.txt gigantic.txt gigantic.txt gigantic.txt gigantic.txt \
    gigantic.txt gigantic.txt gigantic.txt remove_block 0 wc_files 10 gigantic.txt gigantic.txt gigantic.txt \
    gigantic.txt gigantic.txt gigantic.txt gigantic.txt gigantic.txt gigantic.txt gigantic.txt remove_block 0 \
    wc_files 10 gigantic.txt gigantic.txt gigantic.txt gigantic.txt gigantic.txt gigantic.txt gigantic.txt gigantic.txt gigantic.txt \
    gigantic.txt remove_block 0 >> $(OUTPUT_FILE)

	make clean

static: clean
	$(CC) -c mylib.c
	ar crs libmylib.a mylib.o
	$(CC) $(CFLAGS) main.c -o main -L. -lmylib


clean:
	rm -f *.o *.a *.so main
