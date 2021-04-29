#ifndef __COMMON_H
#define __COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "timing.h" // Obtained from Blackboard along with generateSquareMatrix.c


double *readSquareMatrixFromCSV(char *fileName, int *mat_size);
void writeSquareMatrixToCSV(char *fileName, double *a, int mat_size);
int *generateIntegerSquareMatrix(int mat_size, int range_l, int range_h);
void writeIntegerSquareMatrixToCSV(char *fileName, int *a, int mat_size);
void printDistribution(int range_l, int range_h, int *values_dist);

#endif