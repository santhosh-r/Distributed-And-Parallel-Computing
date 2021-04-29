#include "Common.h"

int main(int argc, char *argv[]) {
  if (argc > 2) {
    printf("Usage: ./Homework7_Serial.out [n]");
    printf("\n\tn\tThe number of randomly generated points to be used to calculate PI.\n");
    exit(EXIT_FAILURE);
  }
  long i, c = 0, n = (argc == 2) ? atol(argv[1]):31415926;
  double x, y;

  timing_start();

  srand(time(NULL));
  for (i = 0; i < n; i++) {
    x = (double)rand()/RAND_MAX;
    y = (double)rand()/RAND_MAX;
    if (x*x + y*y <= 1.0) {
      c++;
    }
  }
  
  timing_stop();

  print_timing();
  printf("The calculated value of PI using Monte Carlo method for %ld points is %f", n, 4.0*c/n);
  int digit_cpi, digit_mpi, digits_correct = 0;
  double t_cpi = 4.0*c/n, t_mpi = M_PI;
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

  return 0;
}