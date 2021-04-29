#ifndef __COMMON_H
#define __COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "timing.h"

char *readFileIntoBuffer(FILE *file);
char **convertBufferToStrings(char *buffer, int *stringCount, int **letterIndices);
int isUnique(char *word);

#endif
