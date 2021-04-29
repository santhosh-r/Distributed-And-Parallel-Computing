#include "Common.h"
#include <mpi.h>
#define DEBUG 1
#define DEBUG_EDGES 0

int min(int x, int y) {
  return (x < y)?x:y;
}

int max(int x, int y) {
  return (x > y)?x:y;
}

int xy_to_rank(int x, int y, int p) {
  return x*p + y;
}

void gather_and_print(int rank, int round, int *state_part, int M, int N, int root_p, bool with_edges) {
  int i, j, k, m = M/root_p, n = N/root_p, pad_n = n + 2;
  
  if (with_edges == FALSE) {
    int *state_part_crop = calloc(m*n, sizeof(int));
    for (i = 1; i <= m; i++) {
      for(j = 1; j <= n; j++) {
        state_part_crop[(i-1)*n + (j-1)] = state_part[i*pad_n + j]; 
      }
    }
    state_part = state_part_crop;
  } else {
    m += 2;
    n += 2;
    M += 2*root_p;
    N += 2*root_p;
  }
  
  int *state_gathered, *state;
  if (rank == 0) {
    state_gathered = calloc(M*N, sizeof(int));
  }
  MPI_Gather(state_part, m*n, MPI_INT, state_gathered, m*n, MPI_INT, 0, MPI_COMM_WORLD);
  
  if (rank == 0) {
    state = calloc(M*N, sizeof(int));
    int *s, x, y;
    // Reconstruct stitched grid
    for (i = 0; i < root_p*root_p; i++) {
      x = i/root_p;
      y = i%root_p;
      s = state_gathered + i*m*n;
      for (j = 0; j < m; j++) {
        for (k = 0; k < n; k++) {
          state[x*m*n*root_p + j*n*root_p + y*n + k] = s[j*n + k];
        }
      }
    }
    free(state_gathered);

    if (round < 0) {
      printf("Start");
    } else {
      printf("Round %d", round);
    }
    printf("%s\n---------%s\n", with_edges?" (w/ edges)":"", with_edges?"-----------":"");
    for (i = 0; i < M; i++) {
      for (j = 0; j < N; j++) {
        // printf("%c ", (state[i*N + j]==1?'*':' '));
        printf("%d ", state[i*N + j]);
      }
      printf("\n");
    }
    printf("\n");
    free(state);
  }
  if (with_edges == FALSE) {
    free(state_part);
  }
}

// Works only when both row M and N col which are divisible 
// by square root of p the no of processes which is an exact square
int main(int argc, char *argv[]) {
  int rank, p;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &p);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  
  // Check if arguments are valid
  if (argc < 4) {
    if (rank == 0) {
      printf("Usage: ./Homework8_Distributed.out [M] [N] [K]");
      printf("\n\tM, N\tThe dimensions of grid for Game of Life.\n");
      printf("\tK\tThe number of iterations of Game of Life to be executed.\n");
    }
    MPI_Finalize();
    return 0;
  }
  int M = atoi(argv[1]), N = atoi(argv[2]), K = atoi(argv[3]);
  int root_p = sqrt(p);
  bool is_exact_square = (sqrt(p) - root_p) == 0.0;
  if (is_exact_square == FALSE || M % root_p != 0 || N % root_p != 0) {
    if (rank == 0) {
      printf("Board dimensions %dx%d not divisible by number of processes p=%d.\n", M, N, p);
      if (M % root_p == 0 && N % root_p == 0) {
        printf("Try 'mpirun -n %d Homework8_Distributed.out'\n", root_p*root_p);
      }
    }
    MPI_Finalize();
    return 0;
  }

  // Data for each process
  int m = M/root_p, n = N/root_p;
  int pad_m = m + 2, pad_n = n + 2;
  int *state_part = calloc(pad_m*pad_n, sizeof(int));
  int *next_state_part = calloc(pad_m*pad_n, sizeof(int));
  int *edge_top = calloc(n, sizeof(int));
  int *edge_bottom = calloc(n, sizeof(int));
  int *edge_left = calloc(m, sizeof(int));
  int *edge_right = calloc(m, sizeof(int));
  int corners[4] = { 0 };
  int i, j, k, r, s;
  
  // Random initial state
  srand(time(NULL) * (rank+1));
  for (i = 1; i <= m; i++) {
    for (j = 1; j <= n; j++) { 
      state_part[i*pad_n + j] = rand() % 2;
    }
  }
  memcpy(next_state_part, state_part, sizeof(int)*pad_m*pad_n);
  if (DEBUG) {
    if (DEBUG_EDGES) {
      gather_and_print(rank, -1, state_part, M, N, root_p, TRUE);
    }
    gather_and_print(rank, -1, state_part, M, N, root_p, FALSE);
  }

  if (rank == 0) {
    timing_start();
  }
  
  // Run K iterations of Game of Life
  for(k = 0; k < K; k++) {
    if (p > 1) { // No need to exchange data for serial execution
      // Load data to be exchanged between processes
      memcpy(edge_top, state_part + pad_n + 1, n*sizeof(int));
      memcpy(edge_bottom, state_part + m*pad_n + 1, n*sizeof(int));
      for (i = 1; i <= m; i++) {
        edge_left[i-1] = state_part[i*pad_n + 1];
        edge_right[i-1] = state_part[i*pad_n + n];
      }
      corners[0] = edge_left[0];
      corners[1] = edge_right[0];
      corners[2] = edge_right[m - 1];
      corners[3] = edge_left[m - 1];

      // Exchange data
      int x = rank/root_p, y = rank%root_p;
      int top = xy_to_rank(max(0, x-1), y, root_p);
      int bottom = xy_to_rank(min(root_p-1, x+1), y, root_p);
      int left = xy_to_rank(x, max(0, y-1), root_p);
      int right = xy_to_rank(x, min(root_p-1, y+1), root_p);
      MPI_Status status, rstatus;
      MPI_Request request;
      
      if (top != rank) {
        MPI_Isend(edge_top, n, MPI_INT, top, top, MPI_COMM_WORLD, &request);
        MPI_Recv(edge_top, n, MPI_INT, top, rank, MPI_COMM_WORLD, &rstatus);
        MPI_Wait(&request, &status);
      }
      if (bottom != rank) {
        MPI_Isend(edge_bottom, n, MPI_INT, bottom, bottom, MPI_COMM_WORLD, &request);
        MPI_Recv(edge_bottom, n, MPI_INT, bottom, rank, MPI_COMM_WORLD, &rstatus);
        MPI_Wait(&request, &status);
      }
      if (left != rank) {
        MPI_Isend(edge_left, m, MPI_INT, left, left, MPI_COMM_WORLD, &request);
        MPI_Recv(edge_left, m, MPI_INT, left, rank, MPI_COMM_WORLD, &rstatus);
        MPI_Wait(&request, &status);
      }
      if (right != rank) {
        MPI_Isend(edge_right, m, MPI_INT, right, right, MPI_COMM_WORLD, &request);
        MPI_Recv(edge_right, m, MPI_INT, right, rank, MPI_COMM_WORLD, &rstatus);
        MPI_Wait(&request, &status);
      }
      
      if (top != rank && left != rank) {
        int top_left = xy_to_rank(x-1, y-1, root_p);
        MPI_Isend(&corners[0], 1, MPI_INT, top_left, top_left, MPI_COMM_WORLD, &request);
        MPI_Recv(&corners[0], 1, MPI_INT, top_left, rank, MPI_COMM_WORLD, &rstatus);
        MPI_Wait(&request, &status);
      }
      if (bottom != rank && right != rank) {
        int bottom_right = xy_to_rank(x+1, y+1, root_p);
        MPI_Isend(&corners[2], 1, MPI_INT, bottom_right, bottom_right, MPI_COMM_WORLD, &request);
        MPI_Recv(&corners[2], 1, MPI_INT, bottom_right, rank, MPI_COMM_WORLD, &rstatus);
        MPI_Wait(&request, &status);
      }
      if (top != rank && right != rank) {
        int top_right = xy_to_rank(x-1, y+1, root_p);
        MPI_Isend(&corners[1], 1, MPI_INT, top_right, top_right, MPI_COMM_WORLD, &request);
        MPI_Recv(&corners[1], 1, MPI_INT, top_right, rank, MPI_COMM_WORLD, &rstatus);
        MPI_Wait(&request, &status);
      }
      if (bottom != rank && left != rank) {
        int bottom_left = xy_to_rank(x+1, y-1, root_p);
        MPI_Isend(&corners[3], 1, MPI_INT, bottom_left, bottom_left, MPI_COMM_WORLD, &request);
        MPI_Recv(&corners[3], 1, MPI_INT, bottom_left, rank, MPI_COMM_WORLD, &rstatus);
        MPI_Wait(&request, &status);
      }

      // Load data into padded area
      if (top != rank) {
        memcpy(state_part + 1, edge_top, n*sizeof(int));
      }
      if (bottom != rank) {
        memcpy(state_part + (m+1)*pad_n + 1, edge_bottom, n*sizeof(int));
      }
      if (left != rank) {
        for (i = 1; i <= m; i++) {
          state_part[i*pad_n] = edge_left[i-1];
        }
      }
      if (right != rank) {
        for (i = 1; i <= m; i++) {
          state_part[i*pad_n + (n+1)] = edge_right[i-1];
        }
      }
      
      if (top != rank && left != rank) {
        state_part[0] = corners[0];
      }
      if (bottom != rank && right != rank) {
        state_part[(m+1)*pad_n + n+1] = corners[2];
      }
      if (top != rank && right != rank) {
        state_part[n+1] = corners[1];
      }
      if (bottom != rank && left != rank) {
        state_part[(m+1)*pad_n] = corners[3];
      }
    }

    // Calculate next state
    for (i = 1; i <= m; i++) {
      for (j = 1; j <= n; j++) {
        int neighbour_count = 0;
        for (r = i - 1; r <= i + 1; r++) {
          for (s = j - 1; s <= j + 1; s++) {
            neighbour_count += state_part[r*pad_n + s];
          }
        }
        neighbour_count -= state_part[i*pad_n + j];
        
        if (neighbour_count < 2 || neighbour_count > 3) {
          next_state_part[i*pad_n + j] = 0;
        } else {
          next_state_part[i*pad_n + j] = 1;
        }
      }
    }

    // Print debug output
    if (DEBUG) {
      if (DEBUG_EDGES) {  // After exchange before computation
        gather_and_print(rank, k, state_part, M, N, root_p, TRUE);
      }
      gather_and_print(rank, k, next_state_part, M, N, root_p, FALSE);
    }

    // Copy next state to current state
    memcpy(state_part, next_state_part, sizeof(int)*pad_m*pad_n);
  }

  if (rank == 0) {
    timing_stop();

    print_timing();
  }

  free(edge_right);
  free(edge_left);
  free(edge_bottom);
  free(edge_top);
  free(next_state_part);
  free(state_part);
  
  MPI_Finalize();
  return 0;
}