#include "Common.h"

int main(void) {
  int size_a, size_b;
  double *a = readSquareMatrixFromCSV("A.csv", &size_a);
  double *b = readSquareMatrixFromCSV("B.csv", &size_b);
  if (a == NULL || b == NULL) {
    printf("Error: Input file couldn't be loaded!\n");
    exit(EXIT_FAILURE);
  }
  if (size_a != size_b) {
    printf("Error: Matrix size mismatch!\n");
    exit(EXIT_FAILURE);
  }

  int size = size_a;
  double *c = calloc(size * size, sizeof(*c));
  if (c == NULL) {
    perror("Error");
    exit(EXIT_FAILURE);
  }

  timing_start();
  // General idea: keep data access contiguous
  double sum; // temporary variable to store value of C(i,j)
  double *col = malloc(sizeof(*col) * size); // temporary array to collect values in B's kth column
  int i, j, k;
  for (j = 0; j < size; j++) {
    for (k = 0; k < size; k++) {
      col[k] = b[k*size + j];
    }
    for (i = 0; i < size; i++) {
      sum = 0;   
      for (k = 0; k < size; k++) {
        sum += a[i*size + k] * col[k];
      }
      c[i*size + j] = sum;
    }
  }
  timing_stop();
  
  print_timing();
  writeSquareMatrixToCSV("C.csv", c, size);

  free(col);
  free(a);
  free(b);
  free(c);
  
  return 0;
}