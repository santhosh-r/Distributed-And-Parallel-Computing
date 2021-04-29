#include "Common.h"
#include <time.h>

double *readSquareMatrixFromCSV(char *fileName, int *mat_size) {
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
          a[i*size + j++] = atof(token); //row-major
          // a[j++*size + i] = atof(token); //column-major
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

int *generateIntegerSquareMatrix(int mat_size, int range_l, int range_h) {
  int *a = malloc(sizeof(*a) * mat_size * mat_size);
  if (a == NULL) {
    perror("Error");
    return NULL;
  }
  int range_n = range_h - range_l + 1;
  srand(time(NULL)); // Comment this line to check if serial and parallel have the same output
  int i, j;
  for (i = 0; i < mat_size; i++) {
    for (j = 0; j < mat_size; j++) {
      a[i*mat_size + j] = rand()%range_n + range_l;
    }
  }
  return a;
}

void writeIntegerSquareMatrixToCSV(char *fileName, int *a, int mat_size) {
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
      fprintf(file, "%d,", a[i*mat_size + j]);
    }
    fprintf(file, "%d\n", a[i*mat_size + j]);
  }
  fclose(file);
}

void printDistribution(int range_l, int range_h, int *values_dist) {
  int range_n = range_h - range_l + 1, i;
  
  printf("Distribution of values [%d, %d] in matrix:\n", range_l, range_h);
  printf("%d: %d", range_l, values_dist[0]);
  for (i = 1; i < range_n; i++) {
    printf(", %d: %d", i + range_l, values_dist[i]);
  }
  printf("\n");
}
