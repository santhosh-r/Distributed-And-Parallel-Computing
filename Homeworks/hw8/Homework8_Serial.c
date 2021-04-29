#include "Common.h"

int min(int x, int y) {
  return (x < y)?x:y;
}

int max(int x, int y) {
  return (x > y)?x:y;
}

void print_state(int *state, int round, int M, int N) {
  int i, j;
  if (round < 0) {
    printf("Start");
  } else {
    printf("Round %d", round);
  }
  printf("\n---------\n");
  for (i = 0; i < M; i++) {
    for (j = 0; j < N; j++) {
      // printf("%c ", (state[i*N + j]==1?'*':' '));
      printf("%d ", state[i*N + j]);
    }
    printf("\n");
  }
  printf("\n");
}

int main(int argc, char *argv[]) {
  if (argc < 4) {
    printf("Usage: ./Homework8_Serial.out [M] [N] [K]");
    printf("\n\tM, N\tThe dimensions of board for Game of Life.\n");
    printf("\tK\tThe number of iterations of Game of Life to be executed.\n");
    exit(EXIT_FAILURE);
  }
  int M = atoi(argv[1]), N = atoi(argv[2]), K = atoi(argv[3]);
  // int M = 6, N = 4, K = 3;
  int i, j, k, p, q; // ch;
  int neighbour_count;
  int *state = calloc(M*N, sizeof(int)); // current board state
  // int state[] = { 0,0,0,1,0,0,0,1,0,1,0,0,1,1,1,0,0,1,0,0,0,1,0,0 };
  int *next_state = calloc(M*N, sizeof(int)); // next state calculated from previous state
  
  // Random initial state
  srand(time(NULL));
  for (i = 0; i < M; i++) {
    for (j = 0; j < N; j++) {
      state[i*N + j] = rand() % 2;
    }
  }

  // Shape instead of random values above
  //  *
  //   *
  // ***
  // Executes as expected when reproduction limited to 3 live neighbors.
  // state[1*N + 2] = 1;
  // state[2*N + 3] = 1;
  // state[3*N + 1] = 1;
  // state[3*N + 2] = 1;
  // state[3*N + 3] = 1;

  memcpy(next_state, state, sizeof(int)*M*N);
  print_state(next_state, -1, M, N);
  
  timing_start();
  for (k = 0; k < K; k++) { // Run K iterations of Game of Life
    for (i = 0; i < M; i++) {
      for (j = 0; j < N; j++) {
        int ps = max(0, i-1), pf = min(M-1, i+1);
        int qs = max(0, j-1), qf = min(N-1, j+1);
        neighbour_count = 0;
        for (p = ps; p <= pf; p++) {
          for (q = qs; q <= qf; q++) {
            neighbour_count += state[p*N + q];
          }
        }
        neighbour_count -= state[i*N + j];
        
        if (neighbour_count < 2 || neighbour_count > 3) {
          next_state[i*N + j] = 0;
        } else {
        // } else if (neighbour_count == 3) { // Execute similar to other definitions of Conway's Game of Life
          next_state[i*N + j] = 1;
        }
      }
    }

    print_state(next_state, k, M, N);
    
    // Copy next state to current state
    memcpy(state, next_state, sizeof(int)*M*N);

    // printf("Press ENTER to continue or 'q' and ENTER to quit...");
    // while((ch = getc(stdin)) != '\n') {
    //   if (tolower(ch) == 'q') {
    //     exit(EXIT_SUCCESS);
    //   }
    // }
  }
  timing_stop();

  print_timing();

  free(state);
  free(next_state);
  return 0;
}