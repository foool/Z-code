CC = gcc -g
CFLAGS =  -O3 -Wall -msse4.1 -DINTEL_SSE4 -msse4.2 -DINTEL_SSE4 -fPIC

CFILES = $(wildcard *.c)
PROGS =	$(patsubst %.c,%.test,$(CFILES))

all: $(PROGS)

%.test: %.o
		$(CC) $(CFLAGS) -o $@ $< ../*.o

.c.o:
		$(CC) $(CFLAGS) -c $*.c

clean:
		rm -f *.o *.test
