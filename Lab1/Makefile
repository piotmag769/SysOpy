CC=gcc
CFLAGS = -Wall -Wextra -pedantic -Werror

# ask tutor about dependencies

static: clean
	$(CC) -c mylib.c
	ar crs libmylib.a mylib.o
	$(CC) $(CFLAGS) main.c -o main -L. -lmylib

dynamic: clean
	$(CC) $(CFLAGS) -shared -fPIC mylib.c -o mylib.so
	$(CC) $(CFLAGS) main.c -o main mylib.so -L. -Wl,-rpath=.

clean:
	rm -f *.o *.a *.so main
