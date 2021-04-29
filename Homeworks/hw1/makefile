CC=gcc
CFLAGS=-I -Wall -lpthread
DEPS_H=timing.h Common.h
DEPS=timing.c Common.c
MAT_SIZE=64
all: Part_1 Part_2 Part_3
Part_1: generateInput A.csv B.csv Part_1_naive.out Part_1_optimized.out
Part_2: Part_2_serial.out Part_2_parallel.out
Part_3: Part_3_serial.out Part_3_parallel.out
generateInput:
	$(CC) -o generateInput generateSquareMatrix.c
A.csv: generateInput
	./generateInput $(MAT_SIZE)
	mv inputMatrix.csv A.csv
B.csv: generateInput
	./generateInput $(MAT_SIZE)
	mv inputMatrix.csv B.csv
Part_1_naive.out: $(DEPS_H) $(DEPS) Part_1_naive.c
	$(CC) -o Part_1_naive.out $(DEPS) Part_1_naive.c $(CFLAGS)
Part_1_optimized.out: $(DEPS_H) $(DEPS) Part_1_optimized.c
	$(CC) -o Part_1_optimized.out $(DEPS) Part_1_optimized.c $(CFLAGS)
Part_2_serial.out:  $(DEPS_H) $(DEPS) Part_2_serial.c
	$(CC) -o Part_2_serial.out $(DEPS) Part_2_serial.c $(CFLAGS)
Part_2_parallel.out:  $(DEPS_H) $(DEPS) Part_2_parallel.c
	$(CC) -o Part_2_parallel.out $(DEPS) Part_2_parallel.c $(CFLAGS)
Part_3_serial.out:  $(DEPS_H) $(DEPS) Part_3_serial.c
	$(CC) -o Part_3_serial.out $(DEPS) Part_3_serial.c $(CFLAGS)
Part_3_parallel.out:  $(DEPS_H) $(DEPS) Part_3_parallel.c
	$(CC) -o Part_3_parallel.out $(DEPS) Part_3_parallel.c $(CFLAGS)
clean:
	$(RM) *.out *.csv generateInput
