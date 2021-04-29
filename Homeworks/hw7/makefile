CC=gcc
MPICC=mpicc
CFLAGS=-g -Wall
DEPS_H=timing.h Common.h
DEPS=timing.c
all: Homework7_Serial.out Homework7_Distributed.out
Homework7_Serial.out:  $(DEPS_H) $(DEPS) Homework7_Serial.c
	$(CC) -o Homework7_Serial.out $(DEPS) Homework7_Serial.c $(CFLAGS)
Homework7_Distributed.out:  $(DEPS_H) $(DEPS) Homework7_Distributed.c
	$(MPICC) -o Homework7_Distributed.out $(DEPS) Homework7_Distributed.c $(CFLAGS)
clean:
	$(RM) *.out
