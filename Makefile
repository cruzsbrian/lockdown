CC     = gcc
CFLAGS = -g -Wall -Wstrict-prototypes -ansi -pedantic

othello: main.o
	$(CC) main.o -o othello

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

clean:
	rm -f *.o othello
