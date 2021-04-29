CC=gcc
CFLAGS=-g -Wall -lpthread
DEPS_H=timing.h Common.h
DEPS=timing.c Common.c
all: Homework4_Serial.out Homework4_Verify.out Homework4_Parallel.out
Homework4_Serial.out:  $(DEPS_H) $(DEPS) Homework4_Serial.c
	$(CC) -o Homework4_Serial.out $(DEPS) Homework4_Serial.c $(CFLAGS)
Homework4_Parallel.out:  $(DEPS_H) $(DEPS) Homework4_Parallel.c
	$(CC) -o Homework4_Parallel.out $(DEPS) Homework4_Parallel.c $(CFLAGS)
Homework4_Verify.out:  $(DEPS_H) $(DEPS) Homework4_Verify.c
	$(CC) -o Homework4_Verify.out $(DEPS) Homework4_Verify.c $(CFLAGS)
clean:
	$(RM) *.out results.txt
