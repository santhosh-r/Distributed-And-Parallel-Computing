// not needed

#include <stdio.h>
#include <string.h>

typedef struct Node {
  char word[46]; // The longest word in the given dataset is "pneumonoultramicroscopicsilicovolcanoconiosis" with length 45.
  struct Node *next;
} Node;

typedef struct Queue {
  Node *current;
  Node *head;
} Queue;

Queue *createQueue() {
  Queue *q = malloc(sizeof(*q));
  q->head = NULL;
  q->current = NULL;
  return q;
}

int destroyQueue(Queue *q) {
  if (q == NULL || q->current->next != NULL) {
    return 0;
  }
  Node *old_node, *current = q->head;
  while (current != NULL) {
    old_node = current;
    current = current->next;
    free(old_node);
  }
  free(q);
  return 1;
}

void enqueue(Queue *q, Node *node) {
  if (q == NULL) {
    return;
  }
  if (q->head == NULL) {
    q->head = q->current = node;
  } else {
    q->current->next = node;
    q->current = node;
  }
}

void readWordsIntoQueue(Queue *q, FILE *file) {

}

char *convertQueueToArray(Queue *q) {
  
}