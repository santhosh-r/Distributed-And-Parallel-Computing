CC=gcc
MPICC=mpicc
CFLAGS=-g -Wall -lm
DEPS_H=timing.h Common.h
DEPS=timing.c
all: Homework8_Serial.out Homework8_Distributed.out
Homework8_Serial.out:  $(DEPS_H) $(DEPS) Homework8_Serial.c
	$(CC) -o Homework8_Serial.out $(DEPS) Homework8_Serial.c $(CFLAGS)
Homework8_Distributed.out:  $(DEPS_H) $(DEPS) Homework8_Distributed.c
	$(MPICC) -o Homework8_Distributed.out $(DEPS) Homework8_Distributed.c $(CFLAGS)
clean:
	$(RM) *.out
