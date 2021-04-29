CC=gcc
CFLAGS=-g -Wall -lpthread -lm
DEPS_H=timing.h stackHw.h
DEPS=timing.c stackHw.c GetNumbers.c
all: Homework3_Serial.out Homework3_Parallel.out
Homework3_Serial.out:  $(DEPS_H) $(DEPS) Homework3_Serial.c
	$(CC) -o Homework3_Serial.out $(DEPS) Homework3_Serial.c $(CFLAGS)
Homework3_Parallel.out:  $(DEPS_H) $(DEPS) Homework3_Parallel.c
	$(CC) -o Homework3_Parallel.out $(DEPS) Homework3_Parallel.c $(CFLAGS)
clean:
	$(RM) *.out
