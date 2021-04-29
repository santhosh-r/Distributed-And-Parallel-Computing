CC=gcc
CFLAGS=-g -Wall -lpthread
DEPS_H=timing.h Common.h
DEPS=timing.c Common.c
MAT_SIZE=64
all: input Homework2_Serial.out Homework2_Parallel.out
input: generateInput A.csv B.csv
generateInput:
	$(CC) -o generateInput generateSquareMatrix.c
A.csv: generateInput
	./generateInput $(MAT_SIZE)
	mv inputMatrix.csv A.csv
B.csv: generateInput
	./generateInput $(MAT_SIZE)
	mv inputMatrix.csv B.csv
Homework2_Serial.out:  $(DEPS_H) $(DEPS) Homework2_Serial.c
	$(CC) -o Homework2_Serial.out $(DEPS) Homework2_Serial.c $(CFLAGS)
Homework2_Parallel.out:  $(DEPS_H) $(DEPS) Homework2_Parallel.c
	$(CC) -o Homework2_Parallel.out $(DEPS) Homework2_Parallel.c $(CFLAGS)
clean:
	$(RM) *.out *.csv generateInput
