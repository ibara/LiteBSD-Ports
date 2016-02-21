#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <unistd.h> /* usleep(), to get animation effect while solving. */

static int sudoku[9][9];
static int sudoku_copy[9][9];
static int guess_y, guess_x, guess_number;
static int current_y, current_x;



static void display_update(char *status_msg)
{
  int x, y, offset = 0;

  erase();

  for (y = 0; y < 13; y += 4)
    mvaddstr(y, 0, "|---+---+---|");

  for (y = 0; y < 9; y++) {
    if (y % 3 == 0)
      offset++;
    mvaddch(y + offset, 0, '|');

    for (x = 0; x < 9; x++) {
      if (sudoku[y][x] == 0) 
        addch(' ');
      else
        printw("%d", sudoku[y][x]);

      if (x % 3 == 2)
        addch('|');
    }
  }

  mvaddstr(1, 16, " *** SUDOKU SOLVER ***");
  mvaddstr(3, 16, "Move cursor with arrow keys.");
  mvaddstr(4, 16, "Use numbers 1 to 9 to input numbers.");
  mvaddstr(5, 16, "Clear a number with 0 or space.");
  mvaddstr(6, 16, "Attempt automatic solving by pressing enter.");

  if (status_msg != NULL)
    mvprintw(8, 16, "Status: %s", status_msg);

  /* Place cursor. */
  move(current_y + (current_y / 3) + 1,
       current_x + (current_x / 3) + 1);

  refresh();
}



static int guess_is_not_possible(void)
{
  int ny, nx;

  /* Check 3x3 box. */
  for (ny = (guess_y / 3) * 3; ny < ((guess_y / 3) + 1) * 3; ny++) {
    for (nx = (guess_x / 3) * 3; nx < ((guess_x / 3) + 1) * 3; nx++) {
      if (sudoku[ny][nx] == guess_number)
        return 1;
    }
  }

  /* Check horizontal line. */
  for (nx = 0; nx < 9; nx++) {
    if (sudoku[guess_y][nx] == guess_number)
      return 1;
  }

  /* Check vertical line. */
  for (ny = 0; ny < 9; ny++) {
    if (sudoku[ny][guess_x] == guess_number)
      return 1;
  }

  return 0;
}



static int apply_next_guess(void)
{
  /* Restore to initial state, always. */
  memcpy(sudoku, sudoku_copy, sizeof(sudoku));

  if (guess_number == 0) {
    /* Possibly find inital empty spot. */
    while (1) {
      if (sudoku[guess_y][guess_x] > 0) {
        guess_x++;
        if (guess_x == 9) {
          guess_x = 0;
          guess_y++;
          if (guess_y == 9)
            return 0; /* Exhausted. */
        }
      } else {
        break;
      }
    }
  }

  /* Find next untried number. */
  do {
    guess_number++;
    if (guess_number == 10) {
      guess_number = 1;
      do {
        guess_x++;
        if (guess_x == 9) {
          guess_x = 0;
          guess_y++;
        }
      } while (sudoku[guess_y][guess_x] > 0 && guess_y < 9);
      if (guess_y == 9)
        return 0; /* Exhausted. */
    }
  } while (guess_is_not_possible());

  sudoku[guess_y][guess_x] = guess_number;
  display_update("Guessing...");
  usleep(10000);
  return 1;
}



static int is_blocked_horizontally(int target, int y)
{
  int i;
  for (i = 0; i < 9; i++) {
    if (sudoku[y][i] == target)
      return 1;
  }
  return 0;
}



static int is_blocked_vertically(int target, int x)
{
  int i;
  for (i = 0; i < 9; i++) {
    if (sudoku[i][x] == target)
      return 1;
  }
  return 0;
}



static int is_blocked_elsewhere(int target, int y, int x)
{
  int ny, nx, skipped = 0;

  /* If number can already be found in the same 3x3 block... */
  for (ny = (y / 3) * 3; ny < ((y / 3) + 1) * 3; ny++) {
    for (nx = (x / 3) * 3; nx < ((x / 3) + 1) * 3; nx++) {
      if (sudoku[ny][nx] == target)
        return 0; /* ...then abort. */
    }
  }

  /* For each other empty field in the block... */
  for (ny = (y / 3) * 3; ny < ((y / 3) + 1) * 3; ny++) {
    for (nx = (x / 3) * 3; nx < ((x / 3) + 1) * 3; nx++) {
      if (sudoku[ny][nx] > 0) {
        skipped++;
        continue; /* ...but only the empty ones... */
      }
      if (ny == y && nx == x) {
        skipped++;
        continue; /* ...not including itself. */
      }

      if (ny != y && is_blocked_horizontally(target, ny)) {
        continue; /* Horizontal path is blocked! */
      } else {
        if (nx != x && is_blocked_vertically(target, nx)) {
          continue; /* Vertical path is blocked! */
        } else {
          return 0; /* No paths blocked. */
        }
      }

    }
  }

  if (skipped == 9)
    return 0; /* Algorithm fails if all fields are occupied! */

  return 1;
}

static int solve_sudoku_algorithm_2(void)
{
  int i, y, x, solutions_found = 0;

  for (y = 0; y < 9; y++) {
    for (x = 0; x < 9; x++) {
      if (sudoku[y][x] > 0)
        continue; /* Already has a number. */

      for (i = 1; i <= 9; i++) {
        if (is_blocked_elsewhere(i, y, x)) {
          sudoku[y][x] = i;
          display_update("Solving...");
          usleep(10000);
          solutions_found++;
          break; /* Solution found, move on to next right away... */
        }
      }

    }
  }

  return solutions_found;
}



static int find_only_solution(int y, int x)
{
  int i, ny, nx, solution;
  char possible[9] = {1,1,1,1,1,1,1,1,1}; /* Tracks possible solutions. */

  if (sudoku[y][x] != 0)
    return -1; /* Already has a number, abort. */

  /* Find numbers on horizontal line, and remove them as solutions. */
  for (nx = 0; nx < 9; nx++) {
    if (sudoku[y][nx] == 0)
      continue;
    possible[sudoku[y][nx] - 1] = 0;
  }

  /* Find numbers on vertical line. */
  for (ny = 0; ny < 9; ny++) {
    if (sudoku[ny][x] == 0)
      continue;
    possible[sudoku[ny][x] - 1] = 0;
  }

  /* Find numbers in same 3x3 box. */
  for (ny = (y / 3) * 3; ny < ((y / 3) + 1) * 3; ny++) {
    for (nx = (x / 3) * 3; nx < ((x / 3) + 1) * 3; nx++) {
      if (sudoku[ny][nx] == 0)
        continue;
      possible[sudoku[ny][nx] - 1] = 0;
    }
  }

  solution = -1; /* Initial "unset" value. */
  for (i = 0; i < 9; i++) {
    if (possible[i]) {
      if (solution == -1)
        solution = i + 1;
      else 
        solution = 0; /* Found more than one solution. */
    }
  }

  if (solution == -1) {
    /* No solutions at all for cell, possibly walked down the wrong road. */
    return -2;
  }

  return solution; /* Will be 0 if multiple solutions are possible. */
}



static int table_full(void)
{
  int x, y;
  for (y = 0; y < 9; y++) {
    for (x = 0; x < 9; x++) {
      if (sudoku[y][x] == 0)
        return 0;
    }
  }
  return 1;
}



static int solve_sudoku_algorithm_1(void)
{
  int x, y, solution, found;

  /* Go through all cells in the table and find a possible "only" solution. */
  /* Do this multiple times, until no more solutions are found or table full. */
  do {
    found = 0; /* Will be overwritten if something is found. */
    for (y = 0; y < 9; y++) {
      for (x = 0; x < 9; x++) {
        if (sudoku[y][x] > 0)
          continue;
        solution = find_only_solution(y, x);
        if (solution > 0) {
          sudoku[y][x] = solution;
          display_update("Solving...");
          usleep(10000);
          found++;
        }
      }
    }
    if (table_full())
      break;
  } while (found > 0);

  if (! table_full()) 
    return -1;

  return 0;
}



static int solve_sudoku(void)
{
  int sudoku_copied = 0;

  enum {
    STATE_TRY_ALGO_1,
    STATE_TRY_ALGO_2,
    STATE_TRY_NEXT_GUESS,
    STATE_TABLE_COPY,
    STATE_TABLE_DONE,
    STATE_TABLE_FAILED,
  } state;

  guess_y = guess_x = guess_number = 0;
  state = STATE_TRY_ALGO_1;

  while (state != STATE_TABLE_DONE) {
    switch (state) {
    case STATE_TRY_ALGO_1:
      if (solve_sudoku_algorithm_1() == 0)
        state = STATE_TABLE_DONE;
      else
        state = STATE_TRY_ALGO_2;
      break;

    case STATE_TRY_ALGO_2:
      if (solve_sudoku_algorithm_2() > 0)
        state = STATE_TRY_ALGO_1;
      else {
        if (sudoku_copied)
          state = STATE_TRY_NEXT_GUESS;
        else
          state = STATE_TABLE_COPY;
      }
      break;

    case STATE_TABLE_COPY:
      memcpy(sudoku_copy, sudoku, sizeof(sudoku));
      sudoku_copied = 1;
      state = STATE_TRY_NEXT_GUESS;
      break;

    case STATE_TRY_NEXT_GUESS:
      if (apply_next_guess())
        state = STATE_TRY_ALGO_1;
      else
        state = STATE_TABLE_FAILED;
      break;
    
    case STATE_TABLE_FAILED:
    default:
      if (sudoku_copied)
        memcpy(sudoku, sudoku_copy, sizeof(sudoku));
      return 1;
      break;
    }
  }

  return 0;
}



static int read_from_file(char *filename)
{
  FILE *fh;
  char line[32];
  int n, scan;

  fh = fopen(filename, "r");
  if (fh == NULL)
    return 1;

  while (fgets(line, sizeof(line), fh) != NULL) {
    scan = sscanf(line, "%1d%1d%1d%1d%1d%1d%1d%1d%1d",
      &sudoku[n][0], &sudoku[n][1], &sudoku[n][2],
      &sudoku[n][3], &sudoku[n][4], &sudoku[n][5],
      &sudoku[n][6], &sudoku[n][7], &sudoku[n][8]);
    if (scan == 9) {
      n++;
      if (n == 9)
        break;
    }
  }

  fclose(fh);
  return 0;
}



int main(int argc, char *argv[])
{
  int done;
  char *current_status;

  if (argc == 2)
    read_from_file(argv[1]);

  initscr();
  noecho();
  keypad(stdscr, TRUE);

  current_status = NULL;
  done = 0;
  while (! done) {
    display_update(current_status);

    switch (getch()) {
    case KEY_LEFT:
      current_x--;
      if (current_x < 0)
        current_x = 0;
      break;

    case KEY_RIGHT:
      current_x++;
      if (current_x >= 9)
        current_x = 8;
      break;

    case KEY_UP:
      current_y--;
      if (current_y < 0)
        current_y = 0;
      break;

    case KEY_DOWN:
      current_y++;
      if (current_y >= 9)
        current_y = 8;
      break;

    case '1':
      sudoku[current_y][current_x] = 1;
      break;

    case '2':
      sudoku[current_y][current_x] = 2;
      break;

    case '3':
      sudoku[current_y][current_x] = 3;
      break;

    case '4':
      sudoku[current_y][current_x] = 4;
      break;

    case '5':
      sudoku[current_y][current_x] = 5;
      break;

    case '6':
      sudoku[current_y][current_x] = 6;
      break;

    case '7':
      sudoku[current_y][current_x] = 7;
      break;

    case '8':
      sudoku[current_y][current_x] = 8;
      break;

    case '9':
      sudoku[current_y][current_x] = 9;
      break;

    case '0':
    case ' ':
      sudoku[current_y][current_x] = 0;
      break;

    case KEY_ENTER:
    case '\n':
      if (solve_sudoku())
        current_status = "Unable to solve, enter more numbers.";
      else
        current_status = "Solved!";
      break;

    case 'q':
    case 'Q':
    case '\e':
      done = 1;
      break;

    default:
      break;
    }
  }

  endwin();
  return 0;
}

