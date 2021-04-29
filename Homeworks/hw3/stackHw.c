#include "stackHw.h"
#include <pthread.h>
#include <unistd.h>

typedef struct Node {
  int number;
  struct Node *next;
} Node;

// Creating a structure containing below global variables will allow creation of multiple concurrent stacks
Node *top = NULL;
long long stack_count = 0;
pthread_mutex_t mutex;
// separate counters to check if the no of pops and pushes are equal (in which case stack_count = 0)
int pushCount = 0;
int popCount = 0;

long long GetStackCount() {
  return stack_count;
}

int Pop() {
  // Critical Region:
  // n x m pops for n x m pushes
  // Two threads calling pop don't pop the same node or assign freed next to top
  // Another thread calling push doesn't connect to old top which is freed here
  // Increment/Decrement operations are not atomic in C/C++
  pthread_mutex_lock(&mutex);
  if (top == NULL) {
    return 0;
  }
  Node *old_top = top;
  top = old_top->next;
  stack_count--;
  popCount++;
  pthread_mutex_unlock(&mutex);
  
  int popped = old_top->number;
  free(old_top);

  // My current implementation benefits execution time only if there are some
  // operations to be done on the popped element as in dfs using stack or using io
  // // pretend calculation
  // long i;
  // for (i = 0; i < 100000; i++);
  // // pretend i/o delay
  // usleep(10);
  
  return popped;
}

void Push(int number) {
  Node *node = malloc(sizeof(*node));
  node->number = number;
  
  // Critical Region:
  // New element is not lost when two threads push
  // New node is not connected to freed old top node
  // Increment/Decrement operations are not atomic in C/C++
  pthread_mutex_lock(&mutex);
  node->next = top;
  top = node;
  stack_count++;
  pushCount++;
  pthread_mutex_unlock(&mutex);
}

void PrintAll() {
  long long i = stack_count;
  Node *current = top;
  while (current != NULL) {
    printf("#%05lld: %d\t", --i, current->number);
    if ((stack_count - i) % 3 == 0) {
      printf("\n");
    }
    current = current->next;
  }
  printf("\n");
}

int IsStackEmpty(void) {
  return (top == NULL);
}