CC=gcc
MPICC=mpicc
CFLAGS=-g -Wall
DEPS_H=timing.h Common.h
DEPS=timing.c Common.c
MAT_SIZE=64
all: input Homework6_Serial.out Homework6_Distributed.out
input: generateInput A.csv B.csv
generateInput:
	$(CC) -o generateInput generateSquareMatrix.c
A.csv: generateInput
	./generateInput $(MAT_SIZE)
	mv inputMatrix.csv A.csv
B.csv: generateInput
	./generateInput $(MAT_SIZE)
	mv inputMatrix.csv B.csv
Homework6_Serial.out:  $(DEPS_H) $(DEPS) Homework6_Serial.c
	$(CC) -o Homework6_Serial.out $(DEPS) Homework6_Serial.c $(CFLAGS)
Homework6_Distributed.out:  $(DEPS_H) $(DEPS) Homework6_Distributed.c
	$(MPICC) -o Homework6_Distributed.out $(DEPS) Homework6_Distributed.c
clean:
	$(RM) *.out *.csv generateInput
