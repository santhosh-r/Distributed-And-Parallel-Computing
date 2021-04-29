#include "Common.h"

long SumUpto(long number) {
  long result = 0;
  int i;
  for (i = 1; i <= number; i++) {
    result += i;
  }
  printf("Sum: %ld\n", result);
  return result;
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("Usage: ./Part_2_serial.out [number] [p]");
    printf("\n\tnumber\tThe number upto which to sum upto.");
    printf("\n\tp\tThe number of loops.\n");
    exit(EXIT_FAILURE);
  }
  long number = atol(argv[1]);
  int p = argc == 2?2:atoi(argv[2]), i;
  
  timing_start();
  for (i=0; i<p; i++) {
    SumUpto(number);
  }
  timing_stop();
  
  print_timing();
  
  return 0;
}