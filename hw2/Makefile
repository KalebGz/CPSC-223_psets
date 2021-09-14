CC = gcc
CFLAGS = -g3 -std=c99 -pedantic -Wall

all:    Binpack nLines

Binpack:  Binpack.o
	${CC} ${CFLAGS} $^ -o $@

nLines:
	/c/cs223/bin/xLines < Binpack.c

clean:
	rm -f Binpack *.o
