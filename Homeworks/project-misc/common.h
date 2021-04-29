#ifndef __COMMON_H
#define __COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include "timing.h"

#ifndef bool
typedef enum bool {FALSE, TRUE} bool;
#endif

#define BOOL_STR1(b) b?"True":"False"
#define BOOL_STR2(b) b?"Yes":"No"

typedef struct Numbers {
  double *numbers;
  long numSize;
} Numbers;

// repurposed from Prof's code (generateSquareMatrix.c and GetNumbers.c) from Blackboard
void generateNumbers(long size, char fileName[]);
// void writeNumbers(Numbers *n, char fileName[]);
Numbers *getNumbers(char fileName[]);
Numbers *createNumbers(long size);
void copyNumbers(Numbers *destination, Numbers *source, long start, long end);
void destroyNumbers(Numbers *n);

#endif