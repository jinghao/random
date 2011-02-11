#include <stdio.h>
#include <stdlib.h>

#define BOARD_SIZE 9
#define MAX_ITERATIONS 20
#define UNUSED_MASK 0x3FE
#define insert(bitmap, index) bitmap |= (1 << index)
#define contains(bitmap, index) (bitmap & (1 << index))

short colset[] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
short rowset[] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
short boxset[] = {0, 0, 0, 0, 0, 0, 0, 0, 0};

// Return a number 0..8
int get_box(int row, int col) {
  return (row / 3) * 3 + (col / 3);
}

void set(int row, int col, char num) {
  if (num) {
    insert(colset[col], num);
    insert(rowset[row], num);
    insert(boxset[get_box(row, col)], num);
  }
}

int find_index(int val) {
  for (int i = 0; i <= 10; ++i) {
    val >>= 1;
    if (val == 0) {
      return i;
    }
  }
  return -1;
}

// Go through the board and set everything that has only one possibility, updating the "possibles" set for rows, columns and boxes
int update_sets(char board[][BOARD_SIZE]) {
  int updated = 0;

  for (int row = 0; row < BOARD_SIZE; ++row) {
    for (int col = 0; col < BOARD_SIZE; ++col) {
      if (!board[row][col]) {
        short unused = UNUSED_MASK & ~(colset[col] | rowset[row] | boxset[get_box(row, col)]);

        // unused is a power of 2
        if (!(unused & (unused - 1))) {
          board[row][col] = find_index(unused);
          set(row, col, board[row][col]);
          ++updated;
        }
      }
    }
  }

  return updated;
}

void print_possibilities(char board[][BOARD_SIZE]) {
  for (int row = 0; row < BOARD_SIZE; ++row) {
    for (int num_y = 0; num_y < 3; ++num_y) {
      for (int col = 0; col < BOARD_SIZE; ++col) {
        char num = board[row][col];
        if (!num) {
          short used = colset[col] | rowset[row] | boxset[get_box(row, col)];
          for (int num_x = 0; num_x < 3; ++num_x) {
            int possibility = num_y * 3 + num_x + 1;
            if (!contains(used, possibility)) {
              printf("%d", possibility);
            } else {
              printf(" ");
            }
          }
        } else {
          printf("   ");
        }

        printf(col % 3 == 2 ? "|" : " ");
      }

      printf("\n");
    }

    if (row % 3 == 2)
      printf("------------------------------------");
    printf("\n");
  }
}

void print_board(char board[][BOARD_SIZE]) {

  for (int i = 0; i < 9; i++) {
    if (i % 3 == 0)
      printf("\n -----------------------");
    printf("\n| ");
    for (int j = 0; j < 9; j++) {
      if (board[i][j]){
        printf("%d ", board[i][j]);
      } else {
        printf("  ");
      }

      if (j % 3 == 2) {
        printf("| ");
      }
    }
  }
  printf("\n -----------------------\n");
}

void solve(char board[][BOARD_SIZE]) {
  for (int row = 0; row < BOARD_SIZE; ++row) {
    for (int col = 0; col < BOARD_SIZE; ++col) {
      set(row, col, board[row][col]);
    }
  }

  for (int update = 0; update < MAX_ITERATIONS; ++update) {
    int updates = update_sets(board);
    printf("\tIteration %d. Updates: %d\n", update, updates);

    // If we've reached steady state, let's exit
    if (!updates) {
      break;
    }
  }
}

int main(void) {
  char board[][BOARD_SIZE] = {
      {1, 0, 5, 9, 4, 0, 0, 2, 0},
      {0, 0, 9, 0, 0, 3, 0, 0, 0},
      {0, 8, 4, 6, 1, 0, 9, 0, 0},
      {0, 9, 0, 0, 0, 0, 0, 8, 0},
      {3, 6, 1, 8, 0, 9, 2, 4, 7},
      {0, 4, 0, 0, 0, 0, 0, 1, 0},
      {0, 0, 3, 0, 9, 5, 1, 6, 0},
      {0, 0, 0, 3, 0, 0, 7, 0, 0},
      {0, 5, 0, 0, 6, 1, 8, 0, 4},
  };

  printf("Input puzzle:\n");
  print_board(board);

  printf("Solving...\n");
  solve(board);

  printf("Done solving. Solution:\n");
  print_board(board);

  return EXIT_SUCCESS;
}
