#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <math.h>
#include <string.h>

#define M 16
#define N 16

// Source: http://graphics.stanford.edu/~seander/bithacks.html
int min(int x, int y) { return y ^ ((x ^ y) & -(x < y)); }
int max(int x, int y) { return x ^ ((x ^ y) & -(x < y)); }

int main(int argc, char *argv[]) {
  int state[M][N] = { 0 }, new_state[M][N] = { 0 };
  int i, j, k, l, ch, done = 0;
  int neighbour_count;

  // srand(time(NULL));
  // for (i = 0; i < M/4; i++) {
  //   for (j = 0; j < N/4; j++) {
  //     state[i][j] = rand() % 2;
  //   }
  // }
  
  //  *
  //   *
  // ***
  state[1][2] = 1;
  state[2][3] = 1;
  state[3][1] = 1;
  state[3][2] = 1;
  state[3][3] = 1;

  while(!done) {
    system("clear");
    for (i = 0; i < M; i++) {
      for (j = 0; j < N; j++) {
        printf("%c ", (state[i][j]==1?'*':' '));
      
        int ks = max(0, i-1), kf = min(M-1, i+1);
        int ls = max(0, j-1), lf = min(N-1, j+1);
        neighbour_count = 0;
        for (k = ks; k <= kf; k++) {
          for (l = ls; l <= lf; l++) {
            neighbour_count += state[k][l];
          }
        }
        neighbour_count -= state[i][j];
        
        if (neighbour_count < 2 || neighbour_count > 3) {
          new_state[i][j] = 0;
        } else if (neighbour_count == 3){
          new_state[i][j] = 1;
        } else {
          new_state[i][j] = state[i][j];
        }
      }
      printf("\n");
    }

    memcpy(state, new_state, sizeof(int)*M*N);

    printf("Press ENTER to continue or 'q' and ENTER to quit...");
    while((ch = getc(stdin)) != '\n') {
      done = (tolower(ch) == 'q');
    }
  }


  return 0;
}