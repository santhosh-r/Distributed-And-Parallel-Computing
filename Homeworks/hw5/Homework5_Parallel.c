#include "Common.h"
#include <omp.h>
#include <math.h>

double *a, *b, *c;
int size;

// Simple parallel implementation
void matrixMultiplySimple(void) {
  double sum; // temporary variable to store value of C(i,j)
  int i, j, k;
  
  #pragma omp parallel shared(c) private(i, j, k, sum)
  {
    #pragma omp for
    for (j = 0; j < size; j++) {
      for (i = 0; i < size; i++) {
        sum = 0;   
        for (k = 0; k < size; k++) {
          sum += a[i*size + k] * b[k*size + j];
        }
        c[i*size + j] = sum;
      }
    }
  }
}

// Transpose second operand matrix to keep data access contiguous
// Observed to be faster than above
void matrixMultiplyT(void) {
  double sum; // temporary variable to store value of C(i,j)
  double *bTranspose = transposeMatrix(b, size); // convert B into column-major
  int i, j, k, aOffset, bOffset; // store offsets to reduce number of repeated multiplications

  #pragma omp parallel shared(c) private(i, j, k, sum, aOffset, bOffset)
  {
    #pragma omp for schedule(static)
    for (i = 0; i < size; i++) {
      aOffset = i * size;
      for (j = 0; j < size; j++) {
        bOffset = j * size;
        sum = 0;
        for (k = 0; k < size; k++) {
          sum += a[aOffset + k] * bTranspose[bOffset + k];
        }
        c[aOffset + j] = sum;
      }
    }
  }

  free(bTranspose);
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("Usage: ./Homework5_Parallel.out [P]");
    printf("\n\tP\tThe number of threads.\n");
    exit(EXIT_FAILURE);
  }
  int P = atoi(argv[1]);

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
  
  omp_set_num_threads(P);

  timing_start();
  matrixMultiplyT();
  timing_stop();
  
  print_timing();
  writeSquareMatrixToCSV("C_p.csv", c, size);
  
  free(a);
  free(b);
  free(c);

  return 0;
}