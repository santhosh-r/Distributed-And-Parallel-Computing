// Find all words which are composed of unique letters
#include "Common.h"

int *tasks;
int *lengths;
char **words;
int *uniqueCounts, *unique6Counts;
pthread_mutex_t mutex;
int tasksStarted = 0, taskCount = 26;

void *countUnique(void *data) {
  int i, index, threadNumber = (long)data;
  while (1) {
    pthread_mutex_lock(&mutex);
    index = tasksStarted++;
    pthread_mutex_unlock(&mutex);
    if (index >= taskCount) {
      break;
    }

    for (i = tasks[index]; i < tasks[index + 1]; i++) {
      if (isUnique(words[i])) {
        uniqueCounts[threadNumber]++;
        lengths[i] = strlen(words[i]);
        if (lengths[i] > 5) {
          unique6Counts[threadNumber]++;
        }
      }
    }
  }
  printf("Worker \x1B[32m#%02d\x1B[0m finished. Found \x1B[32m%d\x1B[0m unique word(s).\n",
    threadNumber, uniqueCounts[threadNumber]);
  return NULL;
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("Usage: ./Homework4_Parallel.out [w]");
    printf("\n\tw\tThe number of threads.\n");
    exit(EXIT_FAILURE);
  }
  int w = atoi(argv[1]);
  w = w > 25 ? 25 : w; // limiting to max of 25 threads (originally 26, changed due to 'y')
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
  int wordCount, uniqueCount = 0, unique6Count = 0;

  // The longest word in the given dataset is "pneumonoultramicroscopicsilicovolcanoconiosis" with a length of
  // 45 and so converting buffer into array by simply replacing newline character with null character instead
  // of creating an array of strings with a fixed size of 25.
  words = convertBufferToStrings(buffer, &wordCount, &tasks);
  // i and y appears to be throughly mixed in words.txt, no entries for y after x
  // using a quick fix to avoid changing words.txt by skipping index for y
  // in a normally ordered dataset, x and y are grouped into a single task so nothing lost
  tasks[24] = tasks[25];
  tasks[25] = tasks[26];
  taskCount--;
  
  lengths = calloc(wordCount, sizeof(*lengths));
  uniqueCounts = calloc(w, sizeof(*uniqueCounts));
  unique6Counts = calloc(w, sizeof(*unique6Counts));
  pthread_t *threads = malloc(sizeof(*threads) * w);
  if (pthread_mutex_init(&mutex, NULL) < 0) {
    printf("pthread_mutex_init() failed");
    exit(EXIT_FAILURE);
  }
  
  timing_start();
  long i;
  for (i = 0; i < w; i++) {
    pthread_create(&threads[i], NULL, countUnique, (void *)i);
  }
  for (i = 0; i < w; i++) {
    pthread_join(threads[i], NULL);
    uniqueCount += uniqueCounts[i];
    unique6Count += unique6Counts[i];
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
  
  pthread_mutex_destroy(&mutex);
  free(threads);
  free(uniqueCounts);
  free(unique6Counts);
  free(lengths);
  free(tasks);
  free(words);
  free(buffer);
  return 0;
}
