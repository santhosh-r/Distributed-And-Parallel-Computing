#include "Common.h"

int *row_divisions;
int range_h, range_l, range_n;
int *a, size;

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
    divisions[i] = quotient*i + (i>remainder?(N>P?remainder:0):i);
  }
  
  return divisions;
}

// Find distribution of values for a portion of rows determined by its thread number
void *findDistribution(void *data) {
  long thread_num = (long)data;
  int *values_dist = calloc(range_n, sizeof(*values_dist));
  if (values_dist == NULL) {
    perror("Error");
    return NULL;
  }

  int start = row_divisions[thread_num];
  int end = row_divisions[thread_num+1];
  if (end <= start) {
    return values_dist;
  }
  int i, j;
  for (i = start; i < end; i++) {
    for (j = 0; j < size; j++) {
      int value = a[i*size + j];
      values_dist[value - range_l]++; // Assume values within range given generateIntegerSquareMatrix()
    }
  }
  
  return values_dist;
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("Usage: ./Part_3_parallel.out [N] [P]");
    printf("\n\tN\tThe size of the square matrix to be generated.\n");
    printf("\n\tP\tThe number of threads. (Optional)\n");
    exit(EXIT_FAILURE);
  }
  int N = atoi(argv[1]);
  int P = argc == 2?2:atoi(argv[2]);
  
  range_l = 1;
  range_h = 10;
  range_n = range_h - range_l + 1;
  a = generateIntegerSquareMatrix(N, range_l, range_h);
  size = N;
  
  timing_start(); // Exclude matrix generation in determining runtime
  int **values_dist_n = malloc(sizeof(*values_dist_n) * P);
  if (values_dist_n == NULL) {
    perror("Error");
    exit(EXIT_FAILURE);
  }
  row_divisions = divideWork(N, P);
  if (row_divisions == NULL) {
    printf("Error: Cannot divide work!");
    exit(EXIT_FAILURE);
  }
  pthread_t *threads = malloc(sizeof(*threads) * (P-1));
  
  long i, k; // Use long to avoid different size casting 
  for (i=1; i < P; i++) {
    pthread_create(&threads[i-1], NULL, findDistribution, (void*)i);
  }
  values_dist_n[0] = findDistribution((void*)0); // Make main thread do a portion of the work

  // Join all threads and obtain their distribution values
  for (i=1; i < P; i++) {
    pthread_join(threads[i-1], (void **)&values_dist_n[i]);
  }
  
  // Serialize the individual distribution values
  for (i=1; i < P; i++) {
    for (k = 0; k < range_n; k++) {
      values_dist_n[0][k] += values_dist_n[i][k];
    }
  }
  timing_stop();
  
  printDistribution(range_l, range_h, values_dist_n[0]);
  print_timing();
  
  for (i=0; i < P; i++) {
    free(values_dist_n[i]);
  }
  free(values_dist_n);
  free(threads);
  free(row_divisions);
  free(a);

  return 0;
}