#ifndef __COMMON_H
#define __COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "timing.h" // Obtained from Blackboard along with generateSquareMatrix.c

double *transposeMatrix(double *a, int size);
double *readSquareMatrixFromCSV(char *fileName, int *mat_size);
void writeSquareMatrixToCSV(char *fileName, double *a, int mat_size);

#endif