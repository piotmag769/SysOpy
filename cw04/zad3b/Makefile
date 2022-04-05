CC=gcc
CFLAGS=-Wall #-Wextra -pedantic -Werror

all: clean
	$(CC) $(CFLAGS) catcher.c -o catcher
	$(CC) $(CFLAGS) sender.c -o sender

clean:
	rm -f catcher sender