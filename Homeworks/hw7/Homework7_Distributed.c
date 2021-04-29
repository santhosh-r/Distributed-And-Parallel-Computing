#include "Common.h"
#include <mpi.h>

int main(int argc, char *argv[]) {
  int rank, p;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &p);
  if (argc > 2) {
    if (rank == 0) {
      printf("Usage: ./Homework7_Distributed.out [n]");
      printf("\n\tn\tThe number of randomly generated points to be used to calculate PI.\n");
    }
    MPI_Finalize();
    return 0;
  }
  
  if (rank == 0) {
    timing_start();
  }
  
  long n = (argc == 2) ? atol(argv[1]):31415926;
  long i, c = 0, n_by_p = n/p;
  double x, y;
  
  srand(time(NULL));
  
  for (i = 0; i < n_by_p; i++) {
    x = (double)rand()/RAND_MAX;
    y = (double)rand()/RAND_MAX;
    if (x*x + y*y <= 1.0) {
      c++;
    }
  }

  long total_c;
  MPI_Reduce(&c, &total_c, 1, MPI_LONG, MPI_SUM, 0, MPI_COMM_WORLD);
  if (rank == 0) {
    timing_stop();

    // sleep(1);  // delay final output to print at the end
    double calc_pi = 4.0*total_c/n;
    printf("The calculated value of PI using Monte Carlo method for %ld points is %f\n",
      n, calc_pi);
    double error = fabs(M_PI - calc_pi)*100.0/M_PI;
    printf("%f%% error from actual PI value.\n", error);
    int digit_cpi, digit_mpi, digits_correct = 0;
    double t_cpi = calc_pi, t_mpi = M_PI;
    while (1) {
      digit_cpi = t_cpi;
      digit_mpi = t_mpi;
      if (digit_cpi != digit_mpi) {
        break;
      }
      digits_correct++;
      t_cpi = (t_cpi - digit_cpi) * 10;
      t_mpi = (t_mpi - digit_mpi) * 10;
    }
    printf("\nCalculated PI is correct upto %d digit(s).\n", digits_correct);
    print_timing();
  }
  MPI_Finalize();
  return 0;
}