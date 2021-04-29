#include "Common.h"

double *a, *b, *c;
int size;
int *row_divisions;

// Return an array of indices of how the work consisting of N portions
// is to be split between P threads
int *divideWork(int N, int P) {
  int *divisions = calloc(P+1, sizeof(*divisions));
  if (divisions == NULL) {
    perror("Error");
    return NULL;
  }

  int remainder = N%P, quotient = N/P, i;
  for (i = 0; i < P+1; i++) {
    divisions[i] = quotient*i;
    if (i <= remainder) {
      divisions[i] += i; // spread leftover work evenly
    } else if (N > P) {
      divisions[i] += remainder; // shift indices for remaining threads
    } else {
      divisions[i] += 0; // no work left for remaining threads
    }
}
  
  return divisions;
}

// Calculate a portion of product rows determined by its thread number
void *matrixMultiply(void *data) {
  long thread_num = (long)data;
  int start = row_divisions[thread_num];
  int end = row_divisions[thread_num+1];
  
  // General idea: keep data access contiguous
  double sum; // temporary variable to store value of C(i,j)
  double *col = malloc(sizeof(*col) * size); // temporary array to collect values in B's kth column
  int i, j, k;
  for (j = 0; j < size; j++) {
    for (k = 0; k < size; k++) {
      col[k] = b[k*size + j];
    }
    for (i = start; i < end; i++) {
      sum = 0;   
      for (k = 0; k < size; k++) {
        sum += a[i*size + k] * col[k];
      }
      c[i*size + j] = sum;
    }
  }

  free(col);
  return NULL;
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("Usage: ./Homework2_Parallel.out [P]");
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

  timing_start();
  row_divisions = divideWork(size, P);
  if (row_divisions == NULL) {
    printf("Error: Cannot divide work!");
    exit(EXIT_FAILURE);
  }
  pthread_t *threads = malloc(sizeof(*threads) * (P-1));
  
  long i; // Use long to avoid different size casting 
  for (i=1; i < P; i++) {
    if (row_divisions[i+1] == 0) {  // Avoid creating threads with no work assigned
      P = i;
      break;
    }
    pthread_create(&threads[i-1], NULL, matrixMultiply, (void*)i);
  }
  matrixMultiply(0); // Make main thread do a portion of the work

  // Join all threads
  for (i=1; i < P; i++) {
    pthread_join(threads[i-1], NULL);
  }
  
  timing_stop();
  
  print_timing();
  writeSquareMatrixToCSV("C_p.csv", c, size);
  
  free(threads);
  free(row_divisions);
  free(a);
  free(b);
  free(c);

  return 0;
}