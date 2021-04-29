#include "stackHw.h"
#include "timing.h"
#include <math.h>
#include <pthread.h>

extern int numSize;
extern int *numbers;
extern pthread_mutex_t mutex;
extern int pushCount;
extern int popCount;
int n, p, n_by_p;

void *pushAndPop(void *data) {
  long thread_num = (long)data, i, j;
  long start = thread_num * n_by_p;
  long end = (thread_num + 1) * n_by_p;
  if (start >= n) {
    return NULL;
  }
  if (end > n) {
    end = n;
  }
  for (i = start; i < end; i++) {
    for (j = 0; j < numSize; j++) {
      Push(numbers[j]);
    }
  }
  
  end = numSize * (end - start);
  for (i = 0; i < end; i++) {
    // printf("#[%03ld]%03ld: %d\t", thread_num, i, Pop());
    Pop();  // comment if printing output
    // if (popCount % 5 == 0) {
    //   printf("\n");
    // }
  }
  return NULL;
}

int main(int argc, char *argv[]) {
  if (argc < 3) {
    printf("Usage: ./Homework3_Parallel.out [n] [p]");
    printf("\n\tn\tThe number of loops.");
    printf("\n\tp\tThe number of threads.\n");
    exit(EXIT_FAILURE);
  }
  n = atoi(argv[1]);
  p = atoi(argv[2]);
  n_by_p = (int)ceil((double)n/p);
  GetNumbers("numbers.txt");
  
  timing_start();
  pthread_t *threads = malloc(sizeof(*threads) * p);
  pthread_mutex_init(&mutex, NULL);
  long i;
  for (i = 0; i < p; i++) {
    pthread_create(&threads[i], NULL, pushAndPop, (void*)i);
  }
  for (i = 0; i < p; i++) {
    pthread_join(threads[i], NULL);
  }
  timing_stop();
  
  printf("\n%d number(s) pushed.\n", pushCount);
  printf("%d number(s) popped.\n", popCount);
  print_timing();
  
  pthread_mutex_destroy(&mutex);
  free(numbers);
  free(threads);
  return 0;
}
