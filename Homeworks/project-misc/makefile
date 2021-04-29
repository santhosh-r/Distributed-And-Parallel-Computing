CC=gcc
CFLAGS=-g -Wall -lpthread -lm
DEPS_H=timing.h common.h
DEPS=timing.c common.c
all: mergesort.out
mergesort.out:  $(DEPS_H) $(DEPS) mergesort.c
	$(CC) -o mergesort.out $(DEPS) mergesort.c $(CFLAGS)
clean:
	$(RM) *.out *.txt
