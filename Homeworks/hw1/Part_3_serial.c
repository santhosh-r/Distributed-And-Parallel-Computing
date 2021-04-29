#include "Common.h"

// Find distribution of values in a given matrix
int *findDistribution(int range_l, int range_h, int *a, int size) {
  int range_n = range_h - range_l + 1;
  int *values_dist = calloc(range_n, sizeof(*values_dist));
  if (values_dist == NULL) {
    perror("Error");
    return NULL;
  }
  
  int i, j;
  for (i = 0; i < size; i++) {
    for (j = 0; j < size; j++) {
      int value = a[i*size + j];
      values_dist[value - range_l]++; // Assume values within range given generateIntegerSquareMatrix()
    }
  }
  
  return values_dist;
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("Usage: ./Part_3_serial.out [N]");
    printf("\n\tN\tThe size of the square matrix to be generated.\n");
    exit(EXIT_FAILURE);
  }
  int N = atoi(argv[1]);
  
  int range_l = 1, range_h = 10;
  int *a = generateIntegerSquareMatrix(N, range_l, range_h);
  
  timing_start(); // Exclude matrix generation in determining runtime
  int *values_dist = findDistribution(range_l, range_h, a, N);
  if (values_dist == NULL) {
    printf("Error: Cannot find distribution of values!\n");
    exit(EXIT_FAILURE);
  }
  timing_stop();
  
  printDistribution(range_l, range_h, values_dist);
  print_timing();
  
  free(values_dist);
  free(a);

  return 0;
}