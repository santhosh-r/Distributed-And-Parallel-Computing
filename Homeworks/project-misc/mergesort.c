#include "common.h"

void printNumbers(Numbers *n, char label[]) {
  printf("\n\"%s\"\nNumbers present: %ld\n", label, n->numSize);
  int i = 0;
  while (i < n->numSize) {
    printf("%f\t", n->numbers[i++]);
    if (i % 8 == 0) {
      printf("\n");
    }
  }
}

void __merge(Numbers *a, long p, long q, long r) {
  Numbers *b = createNumbers(q - p + 1);
  Numbers *c = createNumbers(r - q + 1);
  copyNumbers(b, a, p, q);
  copyNumbers(c, a, q, r);
  b->numbers[q - p] = INFINITY;
  c->numbers[r - q] = INFINITY;
  long i = 0, j = 0, k = p;
  while (k < r) {
    if (b->numbers[i] < c->numbers[j]) {
      a->numbers[k++] = b->numbers[i++];
    } else {
      a->numbers[k++] = c->numbers[j++];
    }
  }
  destroyNumbers(b);
  destroyNumbers(c);
}

void __mergeSort(Numbers *n, long p, long r) {
  if ((r - p) < 2) {
    return;
  }
  long q = p + floor((r-p)/2);
  __mergeSort(n, p, q);
  __mergeSort(n, q, r);
  __merge(n, p, q, r);
}

void mergeSort(Numbers *n) {
  __mergeSort(n, 0, n->numSize);
}

bool isSorted(Numbers *n) {
  if (n == NULL || n->numbers == NULL) {
    return TRUE;
  }
  long i;
  for (i = 0; i < n->numSize - 1; i++) {
    if (n->numbers[i] > n->numbers[i+1]) {
      return FALSE;
    }
  }
  return TRUE;
}

int main(void) {
  generateNumbers(1024, "numbers.txt");
  Numbers *n = getNumbers("numbers.txt");
  // printNumbers(n, "Before sorting");
  timing_start();
  mergeSort(n);
  timing_stop();
  // printNumbers(n, "After sorting");
  printf("Numbers present: \x1B[32m%ld\x1B[0m\n", n->numSize);
  printf("Are numbers sorted? \x1B[32m%s\x1B[0m\n", BOOL_STR2(isSorted(n)));
  print_timing();

  destroyNumbers(n);
  return 0;
}