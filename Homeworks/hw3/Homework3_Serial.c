#include "stackHw.h"
#include "timing.h"

extern int numSize;
extern int *numbers;

extern int pushCount;
extern int popCount;

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("Usage: ./Homework3_Serial.out [n]");
    printf("\n\tn\tThe number of loops.\n");
    exit(EXIT_FAILURE);
  }
  int n = atoi(argv[1]), i, j;
  GetNumbers("numbers.txt");
  
  timing_start();
  for (i = 0; i < n; i++) {
    for (j = 0; j < numSize; j++) {
      Push(numbers[j]);
    }
  }
  
  // PrintAll();
  
  while (!IsStackEmpty()) {
    Pop();
  }
  timing_stop();
  
  printf("\n%d number(s) pushed.\n", pushCount);
  printf("%d number(s) popped.\n", popCount);
  printf("stack_count = %lld\n", GetStackCount());
  print_timing();
  
  free(numbers);
  return 0;
}
