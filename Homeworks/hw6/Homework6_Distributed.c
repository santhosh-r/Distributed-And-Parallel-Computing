#include "Common.h"
#include <mpi.h>

// Try to handle any combination of N and p with assumption N > p
int main(int argc, char *argv[])
{
  int rank;
  int N, p, size_a, size_b;
  double *a, *bT, *c; // Input and product matrices
  double *a_part, *bT_row, *c_part; // Parts of matrix A and C calculated for each rank
  int part_size, remainder;
  double *a_rem, *c_rem; // For remainder left after scatter, handled by master
  int i, j, k;
  double sum;
  
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &p);
  
  if (rank == 0) {
    N = 0;
    a = readSquareMatrixFromCSV("A.csv", &size_a, FALSE);
    bT = readSquareMatrixFromCSV("B.csv", &size_b, TRUE); // read matrix B in column-major order
    if (size_a == size_b && size_a > 0) {
      // Input matrices valid. Ready data for remaining work to be handled by master
      N = size_a;
      remainder = N % p;
      a_rem = malloc(N*remainder * sizeof(*a_rem));
      for (i = 0; i < remainder; i++) {
        for (j = 0; j < N; j++) {
          a_rem[i*N + j] = a[(N - remainder + i)*N + j];
        }
      }
      c_rem = malloc(N*remainder * sizeof(*c_rem));
    }
    timing_start();
  }

  // Broadcast matrix size. Fail all processes if input is invalid
  MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);
  if (N == 0) {
    if (rank == 0) {
      free(a);
      free(bT);
      printf("Invalid input matrix size.");
    }
    MPI_Finalize();
    return 0;
  }
  
  // Scatter matrix A to all processes
  part_size = N * (int)(N / p);
  a_part = malloc(part_size * sizeof(*a_part));
  MPI_Scatter(a, part_size, MPI_DOUBLE, a_part, part_size, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  if (rank == 0) {
    free(a); // Complete matrix A no longer needed
    // matrix B can be loaded here instead if master has insufficient memory for loading A and B together
    // bT = readSquareMatrixFromCSV("B.csv", &size_b, TRUE);
  }
  
  // Broadcast each column of matrix B and compute corresponding element in product matrix C
  bT_row = malloc(N * sizeof(*bT_row));
  c_part = malloc(part_size * sizeof(*c_part));
  for (j = 0; j < N; j++) {
    if (rank == 0) {
       for (i = 0; i < N; i++) {
         bT_row[i] = bT[j*N + i];
       }
    }
    MPI_Bcast(bT_row, N, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    
    for (i = 0; i < N/p; i++) {
      for (k = 0, sum = 0; k < N; k++) {
        sum += a_part[i*N + k] * bT_row[k];
      }
      c_part[i*N + j] = sum;
    }

    // rows of matrix A remaining after scatter handled by master
    if (rank == 0) {
      for (i = 0; i < remainder; i++) {
        for (k = 0, sum = 0; k < N; k++) {
          sum += a_rem[i*N + k] * bT_row[k];
        }
        c_rem[i*N + j] = sum;
      }
    }
  }

  // Gather results into product matrix C
  if (rank == 0) {
    free(bT); // Matrix B no longer needed
    c = calloc(N * N, sizeof(*c));
  }
  MPI_Gather(c_part, part_size, MPI_DOUBLE, c, part_size, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  if (rank == 0) {  // Append results for remaining rows left after Scatter
    for (i = 0; i < remainder; i++) {
      for (j = 0; j < N; j++) {
        c[(N - remainder + i)*N + j] = c_rem[i*N + j];
      }
    }

    timing_stop();
  
    print_timing();
    writeSquareMatrixToCSV("C_p.csv", c, N);

    free(c);
    free(a_rem);
    free(c_rem);
  }

  free(a_part);
  free(bT_row);
  free(c_part);
  
  MPI_Finalize();
  return 0;
}
