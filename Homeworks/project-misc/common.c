#include "common.h"

double getRand(double min, double max)
{
  double d = (double)rand() / RAND_MAX;
  return min + d * (max - min);
}

void generateNumbers(long size, char fileName[]) {
  FILE *fp;
  long i;
  double min=0.00001, max=100000;
  char comma[2];
  fp = fopen(fileName, "w");
  srand(time(NULL));
  fprintf(fp, "%ld\n", size);
  sprintf(comma, "%s", "");
  for(i=0;i<size;i++)
  {
    fprintf(fp, "%s%f",comma,getRand(min, max));
    sprintf(comma, "%s", ",");
  }
  fprintf(fp, "\n");

  fclose(fp);

}

Numbers *getNumbers(char fileName[])
{
  FILE* fp = fopen(fileName, "rb");
  char * line = NULL;
  size_t len = 0;
  ssize_t read;
  const char delim[2] = ",";
  int lineCount = 0;
  char * token;
  long numbersCount = 0;
  char lineStr[100000];
  if(fp == NULL)
  {
    printf("\nError reading file");
    exit(-1);
  }
  Numbers *n = calloc(1, sizeof(Numbers));
  while ((read = getline(&line, &len, fp)) != -1) 
  {
    sprintf(lineStr, "%s", line);
    if(lineCount == 0)
    {
      n->numSize = atoi(line);
      n->numbers = malloc(n->numSize * sizeof(double));
    }
    else if(lineCount>1)
    {
      break;
    }
    else
    {
    
      token = strtok(lineStr, delim);
  
      while( token != NULL ) 
      {
        n->numbers[numbersCount] = atof(token);
        //printf( " %s\n", token );
        numbersCount++;
        token = strtok(NULL, delim);
      }
      //printf("Retrieved line of length %zu :\n", read);
      //printf("%s\n", line);
    }
    lineCount++;
  }

  fclose(fp);
  if (line!= NULL)
  {
    free(line);
  }

  return n;
}

Numbers *createNumbers(long size) {
  Numbers *n = calloc(1, sizeof(*n));
  n->numSize = size;
  n->numbers = calloc(size, sizeof(*n->numbers));
  return n;
}

void copyNumbers(Numbers *destination, Numbers *source, long start, long end) {
  long i = 0;
  while (start < end) {
    destination->numbers[i++] = source->numbers[start++];
  }
}

void destroyNumbers(Numbers *n) {
  if (n != NULL) {
    if (n->numbers != NULL) {
      free(n->numbers);
    }
    free(n);
  }
}