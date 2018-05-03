CC=gcc
CFLAGS=-std=c99 -pedantic -Wall -Wextra

ac: main.o
	$(CC) -o ac main.o

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

clean:
	rm -f *.o main
