#include "Common.h"
#include <time.h>

double *transposeMatrix(double *a, int size) {
  double *t = calloc(size * size, sizeof(*t));
  if (t == NULL) {
    perror("Error");
    exit(EXIT_FAILURE);
  }
  int i, j;
  for (i = 0; i < size; i++) {
    for (j = 0; j < size; j++) {
      t[j*size + i] = a[i*size + j];
    }
  }
  return t;
}

double *readSquareMatrixFromCSV(char *fileName, int *mat_size, bool isTransposed) {
  *mat_size = 0;
  FILE *file = fopen(fileName, "r");
  if (file == NULL) {
    perror("Error");
    return NULL;
  }

  int size;
  double *a = NULL;
  fscanf(file, "%d", &size);
  if (size > 0) {
    int i, j;
    char *line, *token, *saveptr;
    size_t line_size = 256 * sizeof(*line);
    line = malloc(line_size);
    a = malloc(sizeof(*a) * size * size);
    if (a == NULL) {
      perror("Error");
      return NULL;
    }
    
    getline(&line, &line_size, file);
    for (i = 0; getline(&line, &line_size, file) != -1; i++) {
      j = 0;
      token = strtok_r(line, ",", &saveptr);
      do {
          if (isTransposed) {
            a[j++*size + i] = atof(token); //column-major
          } else {
            a[i*size + j++] = atof(token); //row-major
          }
      } while ((token = strtok_r(NULL, ",", &saveptr)) != NULL);
    }

    free(line);
    if (i != size || j != size) {
      free(a);
      return NULL;
    }
    *mat_size = size;
  }
  fclose(file);
  return a;
}

void writeSquareMatrixToCSV(char *fileName, double *a, int mat_size) {
  if (mat_size < 1) {
    printf("Error: Invalid matrix size!\n");
    return;
  }
  FILE *file = fopen(fileName, "w");
  if (file == NULL) {
    perror("Error");
    return;
  }
  fprintf(file, "%d\n", mat_size);
  int i, j;
  for (i = 0; i < mat_size; i++) {
    for (j = 0; j < mat_size-1; j++) {
      fprintf(file, "%f,", a[i*mat_size + j]);
    }
    fprintf(file, "%f\n", a[i*mat_size + j]);
  }
  fclose(file);
}
