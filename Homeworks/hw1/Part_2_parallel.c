#include "Common.h"

long number;

void *SumUpto(void *data) {
  long thread_num = (long)data;
  long result = 0;
  int i;
  for (i = 1; i <= number; i++) {
    result += i;
  }
  printf("Sum: %ld (Thread No. %ld)\n", result, thread_num);
  return (void *)result;
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("Usage: ./Part_2_parallel.out [number] [p]");
    printf("\n\tnumber\tThe number upto which to sum upto.");
    printf("\n\tp\tThe number of loops/threads.\n");
    exit(EXIT_FAILURE);
  }
  number = atol(argv[1]);
  int p = argc == 2?2:atoi(argv[2]);
  pthread_t *threads = malloc(sizeof(*threads) * (p-1));
  
  timing_start();
  long i; // Use long to avoid different size casting
  for (i=1; i<p; i++) {
    pthread_create(&threads[i-1], NULL, SumUpto, (void *)i);
  }
  SumUpto(0); // Make main thread do a portion of the work
  for (i=1; i<p; i++) {
    pthread_join(threads[i-1], NULL);
  }
  timing_stop();
  
  print_timing();
  
  free(threads);

  return 0;
}