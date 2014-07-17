CC = gcc -g
CFLAGS = -O3 -Wall -Wextra -mmmx -msse -DINTEL_SSE -msse2 -DINTEL_SSE2 -msse3 -DINTEL_SSE3 -mssse3 -msse4.1 -DINTEL_SSE4 -msse4.2 -DINTEL_SSE4  -fPIC

SHELL=/bin/bash

ALL = clean $(OBJS) cleantests

OBJS = common.o mat.o mcoding.o region_xor.o zcode.o

all:$(ALL)

clean:
	rm -f core tags *.o $(ALL)

cleantests:
	rm -f tests/*.test

cleanall:
	rm -f */*.test

.SUFFIXES: .c .o
.c.o:
	$(CC) $(CFLAGS) -c $*.c
