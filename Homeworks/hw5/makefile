CC=gcc
CFLAGS=-g -Wall -lpthread -fopenmp
DEPS_H=timing.h Common.h
DEPS=timing.c Common.c
MAT_SIZE=64
all: input Homework5_Serial.out Homework5_Parallel.out
input: generateInput A.csv B.csv
generateInput:
	$(CC) -o generateInput generateSquareMatrix.c
A.csv: generateInput
	./generateInput $(MAT_SIZE)
	mv inputMatrix.csv A.csv
B.csv: generateInput
	./generateInput $(MAT_SIZE)
	mv inputMatrix.csv B.csv
Homework5_Serial.out:  $(DEPS_H) $(DEPS) Homework5_Serial.c
	$(CC) -o Homework5_Serial.out $(DEPS) Homework5_Serial.c $(CFLAGS)
Homework5_Parallel.out:  $(DEPS_H) $(DEPS) Homework5_Parallel.c
	$(CC) -o Homework5_Parallel.out $(DEPS) Homework5_Parallel.c $(CFLAGS)
clean:
	$(RM) *.out *.csv generateInput
