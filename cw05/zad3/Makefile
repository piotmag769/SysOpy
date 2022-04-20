CC=gcc
CFLAGS=-Wall #-Wextra -Wpedantic -Werror
FILE=wnioski.txt

# wait up to 30 s for all the test to run (as there is sleep inside producers)
test: all
	echo '' > $(FILE)
	for N in 5 100 5000 ; do \
  		for producers in 1 3 ; do \
  			for consumers in 1 3 ; do \
  				echo "\nN: " $$N "\nproducers: " $$producers "\nconsumers: " $$consumers "\n" >> $(FILE);\
				./main $$N $$producers $$consumers >> $(FILE);\
			done ;\
		done ;\
	done
	make clean

all: clean
	$(CC) $(CLFAGS) main.c -o main
	$(CC) $(CLFAGS) producer.c -o producer
	$(CC) $(CLFAGS) consumer.c -o consumer

clean:
	rm -f producer consumer main FIFO