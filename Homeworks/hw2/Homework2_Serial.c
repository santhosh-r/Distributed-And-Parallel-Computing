#include "Common.h"

double *a, *b, *c;
int size;

void matrixMultiply(void) {
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
  free(col);
}

// Roughly same running time as above.
void matrixMultiplyT(void) {
  // General idea: keep data access contiguous
  double sum; // temporary variable to store value of C(i,j)
  double *t = transposeMatrix(b, size); // convert B into column-major
  int i, j, k;
  for (i = 0; i < size; i++) {
    for (j = 0; j < size; j++) {
      sum = 0;
      for (k = 0; k < size; k++) {
        sum += a[i*size + k] * t[j*size + k];
      }
      c[i*size + j] = sum;
    }
  }
  free(t);
}

int main(void) {
  int size_a, size_b;
  a = readSquareMatrixFromCSV("A.csv", &size_a);
  b = readSquareMatrixFromCSV("B.csv", &size_b);
  if (a == NULL || b == NULL) {
    printf("Error: Input file couldn't be loaded!\n");
    exit(EXIT_FAILURE);
  }
  if (size_a != size_b) {
    printf("Error: Matrix size mismatch!\n");
    exit(EXIT_FAILURE);
  }

  size = size_a;
  c = calloc(size * size, sizeof(*c));
  if (c == NULL) {
    perror("Error");
    exit(EXIT_FAILURE);
  }

  timing_start();
  matrixMultiply();
  timing_stop();
  
  print_timing();
  writeSquareMatrixToCSV("C_s.csv", c, size);

  free(a);
  free(b);
  free(c);
  
  return 0;
}
