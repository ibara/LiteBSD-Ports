/* Free Software under the terms of GNU GPL */
/* Raffael Himmelreich <raffi@raffi.at>     */
/* Clifford Wolf <clifford@clifford.at>     */

#include <curses.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[]) {
	int px, py, opx, opy, ipx, ipy, x, y, c=0, p=0, op, dir;
	int last_lv = argc>=2 ? argc-1 : 11, level=1, lifes=5; FILE *f;
	char ch, field[25][81], tmp[96], *lastword = "Bye.";

	if ( argc > 1 && *argv[1] == '-' ) {
		printf("Usage: %s [aop-level-01.txt [..] ]\n", argv[0]);
		return 1;
	}
	initscr();   cbreak();   noecho();
	curs_set(0); keypad(stdscr, TRUE);
	nodelay(stdscr, 1); start_color();
	init_pair(1, COLOR_RED, COLOR_BLACK);

start:	memset(field, ' ', 25*81);
	snprintf(tmp, 96, "/usr/local/share/aop/aop-level-%02d.txt", level);
	f = fopen(argc>=2 ? argv[level] : tmp, "r");
	if (!f) { endwin(); printf("Can't open level file.\n"); return 1; }
	opx=ipx=px=3, opy=ipy=py=2; op=p; p += 700000 + level*373737;
	for(dir=y=x=0; y <= 25; x=(x+1)%81, y+=x==0)
		if ( fread(&ch, 1, 1, f) <= 0 ) break;
		else if( ch == '%'  ) { opx=ipx=px=x; opy=ipy=py=y; }
			else field[y][x] = ch;
	fclose(f);
	do {
		if ( c == KEY_UP    ) { dir = 8; p-=77; }
		if ( c == KEY_DOWN  ) { dir = 6; p-=77; }
		if ( c == KEY_LEFT  ) { dir = 4; p-=77; }
		if ( c == KEY_RIGHT ) { dir = 2; p-=77; }
		if ( c != ERR ) continue;
		py += dir==8 ? -1 : (dir==6 ? +1 : 0);
		px += dir==4 ? -1 : (dir==2 ? +1 : 0);
		if (field[py][px] == '@') {
			if(level++ != last_lv) goto start;
			lastword="Well done!"; break;
		} else if (field[py][px] == '0') lifes++;
		else if (field[py][px] != ' ' && dir) {
			field[opy][opx] = '0'; sleep(1); flushinp();
			if(--lifes == 0) { p=op; lastword="Sucker!"; break; }
			opx=px=ipx; opy=py=ipy; c=dir=0; continue;
		}
		field[opy][opx] = ' '; field[opy=py][opx=px] = 'O';
		for(y=x=0; y < 25; x=(x+1)%80, y+=x==0) {
			if (!((y^py)|(x^px))) attron(COLOR_PAIR(1));
		 	mvaddch(y, x, field[y][x]); attroff(COLOR_PAIR(1));
		}
		attron(COLOR_PAIR(1));
		mvprintw(0, 0, "Lifes: %d, Points: %d ",
				lifes, p=p-(dir < 5 ? 1 : 2));
		refresh(); usleep(dir < 5 ? 50000 : 100000);
	} while( (c=getch()) != 'q' );
	endwin(); printf("%s (%d points)\n", lastword, p);
	return strcmp(lastword, "Sucker!") == 0;
}

/* Yup - This are 64 lines of C code.  ;-) */
