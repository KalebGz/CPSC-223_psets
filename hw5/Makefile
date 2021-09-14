CC=gcc
CFLAGS= -std=c99 -pedantic -Wall -g3

HWK4= /c/cs223/Hwk4
HWK5= /c/cs223/Hwk5

all:     Nine20

Deque.o:   $(HWK4)/Deque.h

Hash.o:    Hash.h Hash.c

#####
# Instructions to make Nine20
#####

Nine20: Nine20.o $(HWK5)/Deque.o Hash.o
	${CC} ${CFLAGS} -o $@ $^

Nine20.o: $(HWK4)/Deque.h Hash.h

testHash: testHash.o Hash.o 
	${CC} ${CFLAGS} -o $@ $^

testHash.o: testHash.c
	${CC} ${CFLAGS} -c $^

clean:
	rm -f Nine20 testHash *.o
