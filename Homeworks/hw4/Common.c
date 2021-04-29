#include "Common.h"

char *readFileIntoBuffer(FILE *file) {
  fseek(file, 0, SEEK_END);
  long fileSize = ftell(file);
  if (fileSize == -1) {
    perror("ftell() failed");
    return NULL;
  }
  rewind(file);
  char *buffer = malloc(fileSize + 1);
  if (buffer == NULL) {
    perror("malloc() failed");
    return NULL;
  }
  fread(buffer, 1, fileSize, file);
  buffer[fileSize] = '\0';
  return buffer;
}

char **convertBufferToStrings(char *buffer, int *stringCount, int **letterIndices) {
  char *c = buffer;
  int *indices = calloc(27, sizeof(*indices));
  int count = 1; // make sure to count the last null terminated line if present
  while (*c != '\0') {
    if (*c++ == '\n') {
      count++;
    }
  }
  char **strings = malloc(sizeof(*strings) * count);
  strings[0] = c = buffer;
  count = 1;
  while (*c != '\0') {
    if (*c++ == '\n') {
      *(c - 1) = '\0';
      strings[count++] = c;
      // set starting indices for each small letter
      if (*c >= 'a' && *c <= 'z') {
        if (indices[*c - 'a'] == 0) {
          indices[*c - 'a'] = count - 1;
        }
      }
    }
  }
  while (*--c == '\0') { // ignore empty lines at the end
    count--;
  }
  indices[26] = *stringCount = count;
  if (letterIndices == NULL) {
    free(indices);
  } else {
    *letterIndices = indices;
  }
  return strings;
}

int isUnique(char *word) {
  char *c;
  int index, alphabetCount[26] = { 0 };
  for (c = word; *c != '\0'; c++) {
    if (*c >= 'a' && *c <= 'z') {
      index = (int)(*c - 'a');
    } else {
      return 0; // Either not a word or not one with all small letters
    }
    if (++alphabetCount[index] > 1) {
      return 0;
    }
  }
  return (c != word); // avoid identifying empty strings as unique words
} 

