// Find all words which are composed of unique letters
#include "Common.h"

int main(void) {
  FILE *file = fopen("words.txt", "r");
  if (file == NULL) {
    perror("fopen() failed");
    exit(EXIT_FAILURE);
  }
  char *buffer = readFileIntoBuffer(file);
  if (buffer == NULL) {
    exit(EXIT_FAILURE);
  }
  fclose(file);
  int i, wordCount, uniqueCount = 0, unique6Count = 0;
  char **words = convertBufferToStrings(buffer, &wordCount, NULL);
  int *lengths = calloc(wordCount, sizeof(*lengths));

  timing_start();
  for (i = 0; i < wordCount; i++) {
    if (isUnique(words[i])) {
      uniqueCount++;
      lengths[i] = strlen(words[i]);
      if (lengths[i] > 5) {
        unique6Count++;
      }
    }
  }
  timing_stop();
  
  print_timing();
  printf("Number of Unique Words: \x1B[32m%d\x1B[0m\n", uniqueCount);
  printf("Number of Unique Words (length >= 6): \x1B[32m%d\x1B[0m\n", unique6Count);
  file = fopen("results.txt", "w");
  if (file == NULL) {
    perror("fopen() failed");
    exit(EXIT_FAILURE);
  }
  for (i = 0; i < wordCount; i++) {
    if (lengths[i] > 5) {
      fprintf(file, "%s\n", words[i]);
    }
  }
  fclose(file);
  
  free(lengths);
  free(words);
  free(buffer);
  return 0;
}
