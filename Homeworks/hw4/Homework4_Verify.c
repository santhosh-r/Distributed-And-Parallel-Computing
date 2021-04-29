#include "Common.h"

int main(void) {
  FILE *file = fopen("results.txt", "r");
  if (file == NULL) {
    perror("fopen() failed");
    exit(EXIT_FAILURE);
  }
  char *buffer = readFileIntoBuffer(file);
  if (buffer == NULL) {
    exit(EXIT_FAILURE);
  }
  fclose(file);
  int i, wordCount, uniqueCount;
  char **words = convertBufferToStrings(buffer, &wordCount, NULL);
  for (i = 0; i < wordCount; i++) {
    uniqueCount += isUnique(words[i]);
  }
  printf("Number of Words: \x1B[32m%d\x1B[0m\n", wordCount);
  printf("Number of Unique Words: \x1B[32m%d\x1B[0m\n", uniqueCount);
  free(words);
  free(buffer);
  return 0;
}
