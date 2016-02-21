#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <curses.h>

int main(int argc, char *argv[])
{
  int x, y, maxy, maxx;

  srand(time(NULL));

  initscr();
  atexit((void *)endwin);
  noecho();
  timeout(0);

  while (1) {
    if (getch() != ERR)
      return 0; /* Stop on any key press. */

    getmaxyx(stdscr, maxy, maxx);
    for (y = 0; y < maxy; y++) {
      for (x = 0; x < maxx; x++) {
        mvaddch(y, x, (rand() % 0x5E) + 0x21); /* 0x21 -> 0x7E */
      }
    }
    refresh();
    usleep(40000); /* ~24 frames per second. */
  }

  return 0;
}
