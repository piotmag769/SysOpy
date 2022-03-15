CC=gcc
CFLAGS = -Wall # -Wextra -pedantic -Werror

# commented lines are for compiling main file with static and dynamic library

static: clean
	$(CC) -c mylib.c
	ar crs libmylib.a mylib.o
#	$(CC) $(CFLAGS) main.c -o main -L. -lmylib

dynamic: clean
	$(CC) $(CFLAGS) -shared -fPIC mylib.c -o libmylib.so
#	$(CC) $(CFLAGS) main.c -o main libmylib.so -L. -Wl,-rpath=.

clean:
	rm -f *.o *.a *.so main
