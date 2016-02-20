/*
** starlanes v1.2.2 (29-Mar-1997) -- a space-age stock trading game
**
** Copyright (C) 1997  Brian "Beej" Hall
** 
** This program is free software; you can redistribute it and/or
** modify it under the terms of the GNU General Public License
** as published by the Free Software Foundation; either version 2
** of the License, or (at your option) any later version.
** 
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
** 
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
**
** For more information, contact "beej@ecst.csuchico.edu".
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <unistd.h>
#include <curses.h>
#include <termios.h>

/* color stuff: */

#define BLUE_ON_BLACK COLOR_PAIR(1)
#define RED_ON_BLACK COLOR_PAIR(2)
#define GREEN_ON_BLACK COLOR_PAIR(3)
#define YELLOW_ON_BLACK COLOR_PAIR(4)
#define MAGENTA_ON_BLACK COLOR_PAIR(5)
#define CYAN_ON_BLACK COLOR_PAIR(6)
#define WHITE_ON_BLACK COLOR_PAIR(7)
#define YELLOW_ON_BLUE COLOR_PAIR(8)
#define WHITE_ON_BLUE COLOR_PAIR(9)
#define BLACK_ON_YELLOW COLOR_PAIR(10)
#define BLACK_ON_WHITE COLOR_PAIR(11)
#define BLACK_ON_RED COLOR_PAIR(12)
#define BLACK_ON_BLUE COLOR_PAIR(13)
#define BLACK_ON_GREEN COLOR_PAIR(14)

#define MAP_TITLE (color?(BLACK_ON_WHITE):(A_REVERSE))
#define MAP_BORDER (color?(WHITE_ON_BLACK|A_BOLD):(A_BOLD))
#define MAP_SPACE (color?(WHITE_ON_BLACK):(A_NORMAL))
#define MAP_STAR (color?(YELLOW_ON_BLACK|A_BOLD):(A_BOLD))
#define MAP_NEWCO (color?(CYAN_ON_BLACK):(A_NORMAL))
#define MAP_BLACKHOLE (A_NORMAL)
#define CO_A (color?(BLUE_ON_BLACK|A_BOLD):(A_NORMAL))
#define CO_B (color?(GREEN_ON_BLACK):(A_NORMAL))
#define CO_C (color?(YELLOW_ON_BLACK):(A_NORMAL))
#define CO_D (color?(RED_ON_BLACK):(A_NORMAL))
#define CO_E (color?(MAGENTA_ON_BLACK):(A_NORMAL))

#define GENERAL_TITLE (color?(WHITE_ON_BLUE|A_BOLD):(A_REVERSE))
#define GENERAL_TITLE_BLINK (color?(WHITE_ON_BLUE|A_BOLD|A_BLINK):(A_REVERSE|A_BLINK))
#define GENERAL_BORDER (color?(BLUE_ON_BLACK):(A_NORMAL))
#define GENERAL_TEXT (A_NORMAL)

#define COINFO_TITLE (color?(BLACK_ON_GREEN):(A_REVERSE))
#define COINFO_TEXT (A_BOLD)

#define STAND_TITLE (color?(YELLOW_ON_BLUE|A_BOLD):(A_REVERSE))
#define STAND_BORDER (color?(BLUE_ON_BLACK):(A_BOLD))
#define STAND_HEADER (color?(WHITE_ON_BLUE):(A_REVERSE))

#define MORE_COINFO_TITLE (color?(YELLOW_ON_BLUE|A_BOLD):(A_REVERSE))
#define MORE_COINFO_BORDER (color?(BLUE_ON_BLACK):(A_BOLD))
#define MORE_COINFO_HEADER (color?(WHITE_ON_BLUE):(A_REVERSE))

#define QUIT_TITLE (color?(BLACK_ON_RED):(A_REVERSE))
#define QUIT_BORDER (color?(RED_ON_BLACK):(A_BOLD))
#define QUIT_TEXT (color?(YELLOW_ON_BLACK|A_BOLD):(A_NORMAL))

/* misc stuff: */

#define SPACE '.'               /* space character                         */
#define NEWCO '+'               /* new company character                   */
#define STAR '*'                /* star character                          */
#define BLACKHOLE '@'           /* black hole character                    */
#define NUMCO 5                 /* number of companies (don't change)      */
#define INIT_CO_COST 100        /* initial company start cost              */
#define INIT_CASH 6000          /* initial player cash                     */
#define SPLIT_PRICE 3000        /* when stocks split 2-1                   */
#define FOUNDER_BONUS 5         /* founder gets this much stock            */
#define STARCOST 500            /* company's price increase near star      */
#define BLACKHOLECOST -500      /* price increase near black hole          */
#define NEWCOCOST 100           /* company's price increase near new co    */
#define NUMMOVES 5              /* number of different moves a player gets */
#define MAXPLAYERS 4            /* total number of players a game can have */
#define END_PERCENT 54          /* end when this much of the map is full   */
#define DEF_LINES 25            /* default number of lines on screen       */
#define DEF_COLUMNS 80          /* default number of columns on screen     */

#define CR 13                   /* various keys */
#define LF 10
#define BS 8
#define DEL 127
#define ESC 27
#define CTRL_L 12
#define CTRL_C 3
#define CTRL_Z 26

/* macros to look at surrounding spaces on the map: */

#define up_obj(move) (((move)-MAPX < 0)?SPACE:map[(move)-MAPX])
#define down_obj(move) (((move)+MAPX >= MAPX*MAPY)?SPACE:map[(move)+MAPX])
#define left_obj(move) (((move)%MAPX)?map[(move)-1]:SPACE)
#define right_obj(move) (((move)%MAPX == MAPX-1)?SPACE:map[(move)+1])
#define iscompany(c) ((c)>='A'&&(c)<='Z')
#define ripe(c) ((c)==STAR||(c)==NEWCO)
#define co_near(move) (iscompany(up_obj(move))||iscompany(down_obj(move))||iscompany(left_obj(move))||iscompany(right_obj(move)))
#define s_or_bh(c) ((c)==SPACE||(c)==BLACKHOLE)

/* player and company structures: */

typedef struct {
    char name[100];
    int holdings[NUMCO];
    int svalue;  /* stock value -- not always accurate!! */
    int cash;
} PLAYER;

typedef struct {
    char name[100];
    int price;
    int size;
} COMPANY;

/* function prototypes: */

void initialize(void);
void color_setup(void);
void get_num_players(void);
void showmap(void);
void drawmap(int loc, char c);
int get_move(void);
void show_coinfo(void);
void more_coinfo(void);
void do_move(int move);
void do_merge(int *c1, int *c2, int *o1, int *o2);
void holding_bonus(void);
void holding_bonus(void);
void buy_sell(void);
int check_endgame(void);
int count_used_sectors(void);
void calc_cost(int cnum, int move, int n, int s, int w, int e);
void new_co_announce(int newc);
void suck_announce(int conum, int grown);
void merge_announce(int c1, int c2);
void xaction_announce(int c1, int c2);
void split_announce(int conum);
int co_avail(void);
void clear_general(char *s,int blink);
void center(WINDOW *win, int width, int row, char *s);
int my_mvwgetstr(WINDOW *win, int y, int x, int max, int restrict, char *s);
void redraw(void);
void show_standings(char *title);
int order_compare(const void *v1, const void *v2);
void quit_yn(void);
void shutdown(void);
void usage(void);

/* global variables */

char *VERSION = "1.2.2";
char *VERSION_DATE = "29-Mar-1997";
char *ident = "$Id: starlanes.c 1.2.2 29-Mar-1997 beej@ecst.csuchico.edu $";

int MAPX = 12;                     /* x dimension of map */
int MAPY = 10;                     /* y dimension of map */
int LINES;                         /* lines in screen */
int COLUMNS;                       /* columns in screen */
char *map;                         /* pointer to the map data */
PLAYER *pl;                        /* pointer to array of players */
COMPANY *co;                       /* pointer to array of companies */
int numplayers,turn;               /* number of players, whose turn it is */
WINDOW *mapwin,*general,*coinfo;   /* pointers to the windows */
int color;                         /* true if we want color */

int main(int argc, char *argv[])
{
    int done = 0,move, colorforce=0, monoforce=0;

    switch(argc)
    {
        case 1: break;
        case 2: if (argv[1][1] == 'v') {
                    fprintf(stderr,"Starlanes for ncurses v%s Copyright (C) by Brian \"Beej\" Hall %s\n",VERSION,VERSION_DATE);
                    fprintf(stderr,"\nStarlanes comes with ABSOLUTELY NO WARRANTY.  This is free\n");
                    fprintf(stderr,"software, and you are welcome to redistribute it under\n");
                    fprintf(stderr,"certain conditions.  See the file COPYING for details.\n");
                    exit(1);
                }
                else if (argv[1][1] == 'c')
                    colorforce = 1;
                else if (argv[1][1] == 'm')
                    monoforce = 1;
                else
                    usage();
                break;
        default:usage();
    }

    /* initscr */
    initscr();
    start_color();
    
    if (colorforce)
        color = 1;
    else if (monoforce)
        color = 0;
    else
        color = has_colors();

    if (color) color_setup();

    raw();

    /* init map, stocks */
    srand(time(NULL));
    initialize();

    /* num players */
    get_num_players();
    
    clear();
    attron(color?(YELLOW_ON_BLUE|A_BOLD):A_REVERSE);
    mvprintw(0,0," StarLanes ");
    attroff(color?(YELLOW_ON_BLUE|A_BOLD):A_REVERSE);
    attron(color?BLUE_ON_BLACK:A_NORMAL);
    printw("=====================================================================");
    attroff(color?BLUE_ON_BLACK:A_NORMAL);
    wnoutrefresh(stdscr);
    showmap();
    show_coinfo();

    do {
        move = get_move();
        do_move(move);
        holding_bonus();
        if ((done = check_endgame()) != 1) {
            buy_sell();
            turn = (++turn)%numplayers;
        }
    } while (!done);

    shutdown();

    return 0;
}

/*
** initialize() - sets up the map, players, and companies
*/

void initialize(void)
{
    int i,j;
    char *lines, *columns;

    /* get the size of the screen: */

    if ((lines=getenv("LINES"))==NULL || (columns=getenv("COLUMNS"))==NULL) {
        LINES = DEF_LINES;
        COLUMNS = DEF_COLUMNS;
    } else {
        LINES = atoi(lines);
        COLUMNS = atoi(columns);
    }

    /* allocate space for everything: */

    if ((map=malloc(MAPX*MAPY)) == NULL) {
        fprintf(stderr,"starlanes: error mallocing space for map\n");
        exit(1);
    }

    if ((co=calloc(1,NUMCO * sizeof(COMPANY))) == NULL) {
        fprintf(stderr,"starlanes: error mallocing space for companies\n");
        exit(1);
    }

    if ((pl=calloc(1,MAXPLAYERS * sizeof(PLAYER))) == NULL) {
        fprintf(stderr,"starlanes: error mallocing space for players\n");
        exit(1);
    }

    /* set up the map: */

    for(i=0;i<MAPX;i++) { 
        for(j=0;j<MAPY;j++) {
            int n = rand()%60;
            if (n == 0) map[i+j*MAPX] = BLACKHOLE;  /* 1/60 chance */
            else if (n <= 3) map[i+j*MAPX] = STAR;  /* 3/60 chance */
            else map[i+j*MAPX] = SPACE;             /* 56/60 chance */
        }
    }

    /* initialize the companies: */

    for(i=0;i<NUMCO;i++) {
        switch(i) {
            case 0: strcpy(co[i].name, "Altair Starways");break;
            case 1: strcpy(co[i].name, "Beetlejuice, Ltd.");break;
            case 2: strcpy(co[i].name, "Capella Freight Co.");break;
            case 3: strcpy(co[i].name, "Denebola Shippers");break;
            case 4: strcpy(co[i].name, "Eridani Expediters");break;
        }
        co[i].size = co[i].price = 0;
    }

    /* initialize the players: */

    for(i=0;i<MAXPLAYERS;i++) {
        pl[i].name[0] = '\0';
        pl[i].cash = INIT_CASH;
        for(j=0;j<NUMCO;j++)
            pl[i].holdings[j] = 0;
    }

    /* create the windows: */

    if ((mapwin = newwin(MAPY+2,MAPX*3+2,1,1)) == NULL) {
        fprintf(stderr,"starlanes: can't create map window\n");
        exit(1);
    }

    if ((general = newwin(LINES-2-MAPY-2,COLUMNS-2,MAPY+3,1)) == NULL) {
        fprintf(stderr,"starlanes: can't create general window\n");
        exit(1);
    }

    if ((coinfo = newwin(MAPY+2,COLUMNS-2-(MAPX*3+2),1,MAPX*3+3)) == NULL) {
        fprintf(stderr,"starlanes: can't create coinfo window\n");
        exit(1);
    }

    /* set keypad mode so the arrow keys work: */

    keypad(stdscr,1);
    keypad(general,1);
}

/*
** color_setup() -- sets up the color pairs
*/

void color_setup(void)
{
    init_pair(1, COLOR_BLUE, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_BLACK);
    init_pair(3, COLOR_GREEN, COLOR_BLACK);
    init_pair(4, COLOR_YELLOW, COLOR_BLACK);
    init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(6, COLOR_CYAN, COLOR_BLACK);
    init_pair(7, COLOR_WHITE, COLOR_BLACK);
    init_pair(8, COLOR_YELLOW, COLOR_BLUE);
    init_pair(9, COLOR_WHITE, COLOR_BLUE);
    init_pair(10, COLOR_BLACK, COLOR_YELLOW);
    init_pair(11, COLOR_BLACK, COLOR_WHITE);
    init_pair(12, COLOR_BLACK, COLOR_RED);
    init_pair(13, COLOR_BLACK, COLOR_BLUE);
    init_pair(14, COLOR_BLACK, COLOR_GREEN);
}

/*
** get_num_players() -- prompts for the number of players
*/

void get_num_players(void)
{
    char c,s[80];
    int i;

    clear();
    if (color) attron(YELLOW_ON_BLACK|A_BOLD);
    box(stdscr,'$','$');
    sprintf(s," V%s ",VERSION);
    center(stdscr,COLUMNS,LINES-1,s);
    if (color) attroff(YELLOW_ON_BLACK|A_BOLD);

    center(stdscr,COLUMNS,LINES-5, "Starlanes comes with ABSOLUTELY NO WARRANTY.  This is free");
    center(stdscr,COLUMNS,LINES-4, "software, and you are welcome to redistribute it under");
    center(stdscr,COLUMNS,LINES-3, "certain conditions.  See the file COPYING for details.");

    attron(color?YELLOW_ON_BLUE|A_BOLD:A_REVERSE);
    center(stdscr,COLUMNS,6,"* S * T * A * R ** L * A * N * E * S *");
    attroff(color?YELLOW_ON_BLUE|A_BOLD:A_REVERSE);
    sprintf(s,"Please enter the number of players [1-%d]: ",MAXPLAYERS);
    center(stdscr,COLUMNS,9,s);
    refresh();
    noecho();raw();
    do {
        c = getch()-'0';
    } while (c < 1 || c > MAXPLAYERS);
    addch(c+'0');

    numplayers = (int)c;
    srand(getpid());    /* reseed the dumb random number generator */
    turn = rand()%numplayers;

    nl();
    for(i=0;i<numplayers;i++) {
        sprintf(s,"Player %d, enter your name: ",i+1);
        center(stdscr,COLUMNS-8 ,11+i,s);
        refresh();
        my_mvwgetstr(stdscr,i+11,49,20,0,pl[i].name);
        /*getstr(pl[i].name);*/
        if (pl[i].name[0] == '\0') i--;
    }
    nonl();
}

 
/*
** showmap() -- draws the map in the map window
*/

void showmap(void)
{
    int i,j,attrs;

    wattron(mapwin,MAP_BORDER);
    box(mapwin,'|','-');
    wattroff(mapwin,MAP_BORDER);

    wattron(mapwin,MAP_TITLE);
    center(mapwin,MAPX*3+2,0," Map ");
    wattroff(mapwin,MAP_TITLE);

    for(i=0;i<MAPY;i++)
        for(j=0;j<MAPX;j++) {
            switch(map[j+i*MAPX]) {
                case SPACE: attrs = MAP_SPACE;break;
                case STAR:  attrs = MAP_STAR;break;
                case NEWCO: attrs = MAP_NEWCO;break;
                case BLACKHOLE: attrs = MAP_BLACKHOLE;break;
                case 'A':   attrs = CO_A;break;
                case 'B':   attrs = CO_B;break;
                case 'C':   attrs = CO_C;break;
                case 'D':   attrs = CO_D;break;
                case 'E':   attrs = CO_E;break;
                default:    attrs = A_NORMAL;
            }
            wattron(mapwin,attrs);
            mvwaddch(mapwin,i+1,j*3+2,map[j+i*MAPX]);
            wattroff(mapwin,attrs);
        }

    wnoutrefresh(mapwin);
}

/*
** drawmap() -- puts items on the map
*/

void drawmap(int loc, char c)
{
    int attrs;

    switch(c) {
        case SPACE: attrs = MAP_SPACE;break;
        case STAR:  attrs = MAP_STAR;break;
        case NEWCO: attrs = MAP_NEWCO;break;
        case BLACKHOLE: attrs = MAP_BLACKHOLE;break;
        case 'A':   attrs = CO_A;break;
        case 'B':   attrs = CO_B;break;
        case 'C':   attrs = CO_C;break;
        case 'D':   attrs = CO_D;break;
        case 'E':   attrs = CO_E;break;
        default:    attrs = A_NORMAL;
    }
    wattron(mapwin,attrs);
    mvwaddch(mapwin,loc/MAPX+1,(loc%MAPX)*3+2,c);
    wattroff(mapwin,attrs);
}

/*
** show_coinfo() -- draws the company info screen
*/

void show_coinfo(void)
{
    int i,gotco=0,attrs;

    werase(coinfo);
    wattron(coinfo,COINFO_TITLE);
    mvwprintw(coinfo,0,0," %-19s    %5s  %8s ","Company","Price","Holdings");
    wattroff(coinfo,COINFO_TITLE);

    wmove(coinfo,2,0);
    for(i=0;i<NUMCO;i++)
        if (co[i].size) {
            switch(i) {
                case 0: attrs = CO_A;break;
                case 1: attrs = CO_B;break;
                case 2: attrs = CO_C;break;
                case 3: attrs = CO_D;break;
                case 4: attrs = CO_E;break;
            }
            wattron(coinfo,attrs);
            wprintw(coinfo," %-19s    ",co[i].name);
            wattroff(coinfo,attrs);
            wprintw(coinfo,"$%-4d   ",co[i].price);
            /*if (pl[turn].holdings[i] == 0) wattron(coinfo,A_BLINK);*/
            wprintw(coinfo,"%-5d\n\n\r",pl[turn].holdings[i]);
            /*if (pl[turn].holdings[i] == 0) wattroff(coinfo,A_BLINK);*/
            gotco = 1;
        }

    if (!gotco) {
        wattron(coinfo,COINFO_TEXT);
        wprintw(coinfo," no active companies");
        wattroff(coinfo,COINFO_TEXT);
    }

    wnoutrefresh(coinfo);
}

/*
** get_move() -- gets a players move
*/

int get_move(void)
{
    char s[80],c;
    int move[NUMMOVES],i,j,ok;

    wattron(mapwin,A_REVERSE);
    for(i=0;i<NUMMOVES;i++) {
        do {
            ok = 1;
            move[i] = rand()%(MAPX*MAPY);
            if (map[move[i]] != SPACE) ok = 0;
            for(j=0;j<i;j++)
                if (move[j] == move[i]) ok = 0;
            if (co_avail() == -1) {  /* no more companies */
                if ((ripe(up_obj(move[i])) || ripe(down_obj(move[i])) ||
                    ripe(left_obj(move[i])) || ripe(right_obj(move[i]))) &&
                    !co_near(move[i]))
                    ok = 0;
            }
        } while(!ok);
        mvwaddch(mapwin,move[i]/MAPX+1,(move[i]%MAPX)*3+2,i+1+'0');
    }
    wattroff(mapwin,A_REVERSE);
    wnoutrefresh(mapwin);

    sprintf(s," %s (Cash: $%d) ",pl[turn].name,pl[turn].cash);
    clear_general(s,0);
    sprintf(s," %s, enter your move [1-%d]: ",pl[turn].name,NUMMOVES);
    mvwprintw(general,2,1,s);
    show_coinfo();
    noecho();
    do {
        wmove(general,2,1+strlen(s));
        wnoutrefresh(general);
        doupdate();
        c = getch();
        if (c == CTRL_L) {
             redraw();
             continue;
        }
        if (c == CTRL_C || toupper(c) == 'Q') {
            quit_yn();
            continue;
        }
        if (toupper(c) == 'S') {
            show_standings(" Current Standings ");
            continue;
        }
        if (toupper(c) == 'C') {
            more_coinfo();
            continue;
        }

        c -= '0';
    } while(c<1 || c>NUMMOVES);
    echo();

    for(i=0;i<NUMMOVES;i++)
        mvwaddch(mapwin,move[i]/MAPX+1,(move[i]%MAPX)*3+2,SPACE);
    wnoutrefresh(mapwin);

    return move[c-1];
}

/*
** do_move() -- make the move, and follow up on the consequences
**              if it does.
*/

void do_move(int move)
{
    int north,south,west,east,merged = 0,newc;
    char newc_type;

    north = up_obj(move);
    south = down_obj(move);
    west = left_obj(move);
    east = right_obj(move);

    if (s_or_bh(north) && s_or_bh(south) && s_or_bh(east) && s_or_bh(west)) {
        if (north == BLACKHOLE || south == BLACKHOLE || west == BLACKHOLE ||
          east == BLACKHOLE) {
            suck_announce(-1,move);
            return;
        } else {
            map[move] = NEWCO; /* no one around */
            drawmap(move,NEWCO);
            wnoutrefresh(mapwin);
        }
    } else {  /* check for merge */
        if (iscompany(north) && iscompany(south) && north != south) {
            merged = 1;
            do_merge(&north,&south,&west,&east);
        }

        if (iscompany(north) && iscompany(east) && north != east) {
            merged = 1;
            do_merge(&north,&east,&south,&west);
        }

        if (iscompany(north) && iscompany(west) && north != west) {
            merged = 1;
            do_merge(&north,&west,&south,&east);
        }

        if (iscompany(west) && iscompany(east) && west != east) {
            merged = 1;
            do_merge(&west,&east,&north,&south);
        }

        if (iscompany(east) && iscompany(south) && east != south) {
            merged = 1;
            do_merge(&east,&south,&west,&north);
        }

        if (iscompany(west) && iscompany(south) && west != south) {
            merged = 1;
            do_merge(&west,&south,&east,&north);
        }
    }

    if ((ripe(east)||ripe(west)||ripe(north)||ripe(south)) && !co_near(move)) {
        if ((newc=co_avail()) == -1) {
            map[move] = NEWCO;
            drawmap(move,NEWCO);
            wnoutrefresh(mapwin);
        } else {                         /* new company! */
            map[move] = newc+'A';
            drawmap(move,newc+'A');
            co[newc].price = INIT_CO_COST;
            co[newc].size = 1;
            calc_cost(newc,move,north,south,west,east);
            if (co[newc].price <= 0) { /* too close to black hole */
                suck_announce(newc,0);
            } else {                   /* came to life just fine */
                pl[turn].holdings[newc] = FOUNDER_BONUS;
                show_coinfo();
                wnoutrefresh(mapwin);
                new_co_announce(newc);
            }
        }
    } else if (map[move] == SPACE) { /* adding on */
        if (iscompany(west)) newc_type = west;
        if (iscompany(east)) newc_type = east;
        if (iscompany(south)) newc_type = south;
        if (iscompany(north)) newc_type = north;
        map[move] = newc_type;
        drawmap(move,newc_type);
        wnoutrefresh(mapwin);
        co[newc_type-'A'].size++;
        co[newc_type-'A'].price += NEWCOCOST;
        calc_cost(newc_type-'A',move,north,south,west,east);
        if (co[newc_type-'A'].price <= 0)  /* black holed */
            suck_announce(newc_type-'A',1);
        else { 
            if (co[newc_type-'A'].price > SPLIT_PRICE)
                split_announce(newc_type-'A');
            else
                show_coinfo();
        }
    }
}

/*
** do_merge() -- does all the nasty business behind a merge
*/

void do_merge(int *c1, int *c2, int *o1, int *o2)
{
    int t,i,cb,cs,doswap=0;

    cb = *c1 - 'A';
    cs = *c2 - 'A';

    if (co[cs].size == co[cb].size) {  /* if same size, check prices */
        int pb=0,ps=0;
        for(i=0;i<numplayers;i++) {    /* calculate worth of companies */
            pb += co[cb].price * pl[i].holdings[cb];
            ps += co[cs].price * pl[i].holdings[cs];
        }

        if (ps > pb)  /* if smaller co has higher worth, swap 'em */
            doswap = 1;
        else if (ps == pb) /* if same price, choose rand */
            doswap = rand()%2;
    }

    if (co[cs].size > co[cb].size || doswap) { /* cb = merger, cs = mergee */
        t = cs;
        cs = cb;
        cb = t;
    }

    for(i=0;i<MAPX*MAPY;i++) 
        if (map[i] == cs+'A') {
            map[i] = cb+'A';
            drawmap(i,cb+'A');
        }

    *c1 = *c2 = cb + 'A';              /* convert to new co */
    if (*o1 == cs + 'A') *o1 = cb + 'A';
    if (*o2 == cs + 'A') *o2 = cb + 'A';

    co[cb].size += co[cs].size;
    co[cb].price += co[cs].price;
    co[cs].size = 0;
    
    wnoutrefresh(mapwin);  /* show the players what's up */

    merge_announce(cb,cs);
    xaction_announce(cb,cs);
    if (co[cb].price > SPLIT_PRICE)
        split_announce(cb);
}

/*
** calc_cost() -- adds value to a company based on surroundings and converts
**                NEWCOs to the company
*/

void calc_cost(int cnum, int move, int n, int s, int w, int e)
{
    if (n == STAR) co[cnum].price += STARCOST;  /* stars */
    if (s == STAR) co[cnum].price += STARCOST;
    if (w == STAR) co[cnum].price += STARCOST;
    if (e == STAR) co[cnum].price += STARCOST;

    if (n == BLACKHOLE) co[cnum].price += BLACKHOLECOST; /* black holes */
    if (s == BLACKHOLE) co[cnum].price += BLACKHOLECOST;
    if (w == BLACKHOLE) co[cnum].price += BLACKHOLECOST;
    if (e == BLACKHOLE) co[cnum].price += BLACKHOLECOST;
        
    if (n == NEWCO) {                          /* starter companies */
        map[move-MAPX] = cnum + 'A';
        drawmap(move-MAPX,cnum+'A');
        co[cnum].size++;
        co[cnum].price += NEWCOCOST;
    }
    if (s == NEWCO) {
        map[move+MAPX] = cnum + 'A';
        drawmap(move+MAPX,cnum+'A');
        co[cnum].size++;
        co[cnum].price += NEWCOCOST;
    }
    if (w == NEWCO) {
        map[move-1] = cnum + 'A';
        drawmap(move-1,cnum+'A');
        co[cnum].size++;
        co[cnum].price += NEWCOCOST;
    }
    if (e == NEWCO) {
        map[move+1] = cnum + 'A';
        drawmap(move+1,cnum+'A');
        co[cnum].size++;
        co[cnum].price += NEWCOCOST;
    }
    wnoutrefresh(mapwin);
}

/*
** new_co_announce() -- announce the coming of a new company
*/

void new_co_announce(int newc)
{
    char s[80];

    clear_general(" Special Announcement! ",1);
    wattron(general,A_BOLD);
    center(general,COLUMNS-2,2,"A new shipping company has been formed!");
    sprintf(s,"Its name is %s",co[newc].name);
    center(general,COLUMNS-2,4,s);
    wattroff(general,A_BOLD);
    center(general,COLUMNS-2,7,"Press any key to continue...");
    wnoutrefresh(general);
    noecho();raw();
    doupdate();
    while (getch() == CTRL_L) redraw();
}

/*
** suck_announce() -- when a company gets drawn into a black hole (value < 0)
*/

void suck_announce(int conum, int grown)
{
    int i;

    if (conum >= 0) {
        for(i=0;i<MAPX*MAPY;i++) {  /* clear the company from the map */
            if (map[i] == conum+'A') {
                map[i] = SPACE;
                drawmap(i,SPACE);
            }
        }
        wnoutrefresh(mapwin);
        
        for(i=0;i<numplayers;i++)   /* ditch all player holdings */
            pl[i].holdings[conum] = 0;

        co[conum].size = co[conum].price = 0; /* eliminate the company */
    } else {
        map[grown] = SPACE; /* grown contains the opposite of the site */
        drawmap(grown,SPACE);
        wnoutrefresh(mapwin);
    }

    clear_general(" Special Announcement! ",1);
    wattron(general,A_BOLD);
    if (conum >= 0 && grown == 1) {  /* already existed */
        center(general,COLUMNS-2,2,"The company named");
        center(general,COLUMNS-2,3,co[conum].name);
        center(general,COLUMNS-2,4,"has been sucked into a black hole!");
        center(general,COLUMNS-2,6,"All players' holdings lost.");
        show_coinfo(); /* show change */
    } else if (conum >= 0 && grown == 0) {      /* was trying to start up */
        center(general,COLUMNS-2,2,"The company that would have been named");
        center(general,COLUMNS-2,3,co[conum].name);
        center(general,COLUMNS-2,4,"has been sucked into a black hole!");
    } else { /* was only a starter company, not a real one yet */
        center(general,COLUMNS-2,2,"The new company site just placed");
        center(general,COLUMNS-2,3,"has been sucked into a black hole!");
    }
    wattroff(general,A_BOLD);
    center(general,COLUMNS-2,8,"Press any key to continue...");
    wnoutrefresh(general);
    noecho();raw();
    doupdate();
    while (getch() == CTRL_L) redraw();
}

/*
** merge_announce() -- announce a merger
*/

void merge_announce(int c1, int c2)
{
    clear_general(" Special Announcement! ",1);
    wattron(general,A_BOLD);
    center(general,COLUMNS-2,2,co[c2].name);
    wattroff(general,A_BOLD);
    center(general,COLUMNS-2,3,"has just been merged into");
    wattron(general,A_BOLD);
    center(general,COLUMNS-2,4,co[c1].name);
    wattroff(general,A_BOLD);
    center(general,COLUMNS-2,7,"Press any key to continue...");
    wnoutrefresh(general);
    noecho();raw();
    doupdate();
    while (getch() == CTRL_L) redraw();
}

/*
** xaction_announce() -- announce transactions after a merger
*/

void xaction_announce(int c1, int c2)
{
    int i,totalshares=0,newshares,bonus,totalholdings,percentage;

    clear_general(" Stock Transactions ",0);
    center(general,COLUMNS-2,2,co[c2].name);
    wattron(general,color?YELLOW_ON_BLUE|A_BOLD:A_REVERSE);
    mvwprintw(general,2,4,"=Player===============Old Stock===New Stock===Total Holdings===Bonus=");
    wattroff(general,color?YELLOW_ON_BLUE|A_BOLD:A_REVERSE);

    for(i=0;i<numplayers;i++)
        totalshares += pl[i].holdings[c2];
    if (totalshares == 0) totalshares = 1;  /* prevent divide by zero */

    for(i=0;i<numplayers;i++) {
        newshares = (int)(((float)(pl[i].holdings[c2]) / 2.0) + 0.5);
        totalholdings = pl[i].holdings[c1] + newshares;
        percentage = 100*pl[i].holdings[c2]/totalshares;
        bonus = 10 * co[c2].price * pl[i].holdings[c2] / totalshares;
        wattron(general,A_BOLD);
        mvwprintw(general,3+i,4," %-20s   ", pl[i].name);
        wattroff(general,A_BOLD);
        wprintw(general,"%-5d       %-5d     %5d            ",
                pl[i].holdings[c2],newshares,totalholdings);
        wattron(general,A_BOLD);
        wprintw(general,"$%-5d",bonus);
        wattroff(general,A_BOLD);

        pl[i].holdings[c1] = totalholdings;
        pl[i].holdings[c2] = 0;
        pl[i].cash += bonus;
    }

    center(general,COLUMNS-2,8,"Press any key to continue...");
    wnoutrefresh(general);
    noecho();raw();
    doupdate();
    while (getch() == CTRL_L) redraw();
}

/*
** split_announce() -- happens when a company splits 2 for 1
*/

void split_announce(int conum)
{
    char s[80];
    int i;

    clear_general(" Special Announcement! ",1);
    wattron(general,A_BOLD);
    sprintf(s,"The stock of %s",co[conum].name);
    center(general,COLUMNS-2,2,s);
    center(general,COLUMNS-2,3,"has split two-for-one!");
    wattroff(general,A_BOLD);
    center(general,COLUMNS-2,6,"Press any key to continue...");

    co[conum].price = (int)(((float)(co[conum].price)/2.0) + 0.5);
    for(i=0;i<numplayers;i++) 
        pl[i].holdings[conum] *= 2;

    show_coinfo();
    wnoutrefresh(general);
    noecho();raw();
    doupdate();
    while (getch() == CTRL_L) redraw();
}

/*
** holding_bonus() -- gives bonus to player based on current holdings
*/

void holding_bonus(void)
{
    int i;

    for(i=0;i<NUMCO;i++) {
        if (co[i].size) {
            pl[turn].cash += (int)(0.05 * ((float)pl[turn].holdings[i]) * ((float)co[i].price));
        }
    }
}

/*
** buy_sell() -- screen that allows the user to buy and sell stocks
*/

void buy_sell(void)
{
    int i,cos[NUMCO],cocount=0,cursor=0,newcur,done=0,amt,max,min,pos1,pos2;
    int attrs;
    char s[80],amtstr[15];

    for(i=0;i<NUMCO;i++)          /* make a list of active companies */
        if (co[i].size)
            cos[cocount++] = i;

    if (cocount == 0) return;     /* no companies yet */

    sprintf(s," %s (Cash: $%d) ",pl[turn].name,pl[turn].cash);
    clear_general(s,0);

    center(general,COLUMNS-2,1,"Arrow keys to select a company, return to trade, escape when done:");

    for(i=0;i<cocount;i++) {
        switch(cos[i]) {
            case 0:     attrs = CO_A;break;
            case 1:     attrs = CO_B;break;
            case 2:     attrs = CO_C;break;
            case 3:     attrs = CO_D;break;
            case 4:     attrs = CO_E;break;
        }
        if (i == cursor) wattron(general,A_REVERSE);
        else wattron(general,attrs);
        mvwprintw(general,i+3,20,co[cos[i]].name);
        if (i == cursor) wattroff(general,A_REVERSE);
        else wattroff(general,attrs);
    }
    wnoutrefresh(general);

    do {
        newcur = cursor;
        raw();noecho();
        doupdate();
        switch(getch()) {
            case CTRL_L:redraw();
                        break;
            case 's':
            case 'S':   show_standings(" Current Standings ");
                        wmove(general,cursor+3,strlen(co[cos[cursor]].name)+20);
                        wnoutrefresh(general);
                        break;
            case 'c':
            case 'C':   more_coinfo();
                        wmove(general,cursor+3,strlen(co[cos[cursor]].name)+20);
                        wnoutrefresh(general);
                        break;
            case 'q':
            case 'Q':
            case CTRL_C:quit_yn();
                        wmove(general,cursor+3,strlen(co[cos[cursor]].name)+20);
                        wnoutrefresh(general);
                        break;
            case KEY_UP:
            case '8':
            case 'k':
            case 'K':   newcur = cursor?cursor-1:cocount-1;break;
            case KEY_DOWN:
            case '5':
            case 'j':
            case 'J':   newcur = cursor<cocount-1?cursor+1:0;break;
            case KEY_RIGHT:
            case 6:
            case CR :
            case LF :   max = pl[turn].cash / co[cos[cursor]].price;
                        min = -pl[turn].holdings[cos[cursor]];
                        sprintf(s,"Amount (%d to %d): ",min,max);
                        mvwprintw(general,cursor+3,40,s);
                        wnoutrefresh(general);
                        my_mvwgetstr(general,cursor+3,40+strlen(s),9,1,amtstr);
                        if (tolower(amtstr[0]) == 'm')
                            amt = max;
                        else if (tolower(amtstr[0]) == 'n')
                            amt = min;
                        else
                            amt = atoi(amtstr);
                        if (amt >= min && amt <= max) {
                            pl[turn].cash += (-amt * co[cos[cursor]].price);
                            pl[turn].holdings[cos[cursor]] += amt;
                        } else {
                            mvwprintw(general,cursor+3,40,"Invalid amount!                      ");
                            wmove(general,cursor+3,55);
                            wnoutrefresh(general);
                            doupdate();
                            sleep(1);
                        }
                        mvwprintw(general,cursor+3,40,"                                     ");
                        sprintf(s," %s (Cash: $%d) ",pl[turn].name,pl[turn].cash);

                        if (amt) show_coinfo();

                        pos1 = ((COLUMNS-2)-strlen(s))/2;
                        pos2 = pos1 + strlen(s);
                        wattron(general,GENERAL_TITLE);
                        center(general,COLUMNS-2,0,s);
                        wattroff(general,GENERAL_TITLE);
                        wattron(general,GENERAL_BORDER);
                        mvwaddstr(general,0,pos1-4,"////");
                        mvwaddstr(general,0,pos2,"////");
                        wattroff(general,GENERAL_BORDER);
                        wmove(general,cursor+3,strlen(co[cos[cursor]].name)+20);
                        wnoutrefresh(general);

                        break;

            case ESC:   done = 1;
        } /* switch */

        if (cursor != newcur) {  /* move cursor */
            switch(cos[cursor]) {
                case 0:     attrs = CO_A;break;
                case 1:     attrs = CO_B;break;
                case 2:     attrs = CO_C;break;
                case 3:     attrs = CO_D;break;
                case 4:     attrs = CO_E;break;
            }

            wattron(general,attrs);
            mvwprintw(general,cursor+3,20,co[cos[cursor]].name);
            wattroff(general,attrs);

            wattron(general,color?BLACK_ON_WHITE:A_REVERSE);
            mvwprintw(general, newcur+3, 20, co[cos[newcur]].name);
            wattroff(general,color?BLACK_ON_WHITE:A_REVERSE);

            wnoutrefresh(general);
            cursor = newcur;
        }
    } while(!done);
}


/*
** check_endgame() -- returns true if the game is over
*/

int check_endgame(void)
{
    int sum=0,i,maptotal;

    for(i=0;i<NUMCO;i++)
        sum += co[i].size;

    maptotal = count_used_sectors();

    return (sum*100)/(MAPX*MAPY) >= END_PERCENT && maptotal <= MAPX*MAPY-4;
}

/*
** count_used_sectors() -- counts the number of non-empty sectors
*/

int count_used_sectors(void)
{
    int maptotal, i;

    for(i=maptotal=0;i<MAPX*MAPY;i++)  /* must be enough room to move */
        if (map[i] != SPACE)
            maptotal++;
    
    return maptotal;
}

/*
** co_avail() - returns the index of the next available company, or -1
**              if all companies exist.
*/

int co_avail(void)
{
    int i;
    for(i=0;i<NUMCO;i++)
        if (!co[i].size)
            return i;
    return -1;
}

/*
** clear_general() -- clears and titles the general window
*/

void clear_general(char *s, int blink)
{
    werase(general);
    wattron(general,GENERAL_BORDER);
    box(general,'|','/');
    wattroff(general,GENERAL_BORDER);
    wattron(general,blink?GENERAL_TITLE_BLINK:GENERAL_TITLE);
    center(general,COLUMNS-2,0,s);
    wattroff(general,blink?GENERAL_TITLE_BLINK:GENERAL_TITLE);
}

/*
** center() -- centers a piece of text on a window on the specified row
*/

void center(WINDOW *win, int width, int row, char *s)
{
    mvwaddstr(win, row, (width-strlen(s))/2, s);
}

/*
** my_mvwgetstr() -- does it my way
*/

int my_mvwgetstr(WINDOW *win, int y, int x, int max, int restrict, char *s)
{
    int cur=0,c,done=0;

    s[0] = '\0';

    nl();noecho();raw();keypad(stdscr,0);
    while(doupdate(), c=getch(), !done && (c != LF && c != CR)) {
        if (c == CTRL_L) {
            redraw();
            continue;
        }
        if (c == erasechar() || c == BS || c == DEL) {
            if (cur) {
                cur--;
                mvwaddch(win,y,x+cur,' ');
                s[cur] = '\0';
                wmove(win,y,x+cur);
                wnoutrefresh(win);
            }
        } else {
            if (restrict == 1) {  /* only numbers and '-' allowed */
                if (!isdigit(c) && c!='-' && tolower(c)!='m' && tolower(c)!='n')
                    continue;
                if (tolower(c) == 'm' || tolower(c) == 'n')
                    if (cur == 0)
                        done = 1;
                    else continue;
                if (c == '-' && cur != 0) continue;
            }
            if (cur < max) {
                mvwaddch(win,y,x+cur,c);
                s[cur] = c; s[cur+1] = '\0';
                cur++;
                wnoutrefresh(win);
            }
        }
    }

    nonl();
    keypad(stdscr,1);
    return strlen(s);
}

/*
** redraw() -- redraws the screen
*/

void redraw(void)
{
    touchwin(stdscr);
    touchwin(mapwin);
    touchwin(coinfo);
    touchwin(general);
    refresh();
    wrefresh(mapwin);
    wrefresh(coinfo);
    wrefresh(general);
}

/*
** show_standings() -- pops up a window with the current standings
*/

void show_standings(char *title)
{
    WINDOW *stand;
    char s[80];
    int i, j, order[MAXPLAYERS], togo, sum;

    if ((stand = newwin(9+numplayers,60,5,(COLUMNS-60)/2)) == NULL) {
        fprintf(stderr,"starlanes: couldn't create standings window\n");
        exit(1);
    }

    wattron(stand,STAND_BORDER);
    box(stand,'|','=');
    wattroff(stand,STAND_BORDER);

    wattron(stand,STAND_TITLE);
    sprintf(s," %s ",title);
    center(stand,60,0,s);
    wattroff(stand,STAND_TITLE);

    wattron(stand,STAND_HEADER);
    mvwaddstr(stand,2,2,"=Player================Stock Value===Cash=====Net Worth=");
    wattroff(stand,STAND_HEADER);

    for(i=0;i<numplayers;i++) {
        order[i] = i;
        pl[i].svalue = 0;   /* calculate total player stock value */
        for(j=0;j<NUMCO;j++)
            if (co[j].size)
                pl[i].svalue += co[j].price * pl[i].holdings[j];
    }

    qsort(order, numplayers, sizeof(int), order_compare); /* sort */

    for(i=0;i<numplayers;i++) {
        wattron(stand,A_BOLD);
        mvwprintw(stand,4+i,3,"%-19s   ",pl[order[i]].name);
        wattroff(stand,A_BOLD);

        wprintw(stand,"$%-8d   $%-8d  ",pl[order[i]].svalue,pl[order[i]].cash);

        wattron(stand,A_BOLD);
        wprintw(stand,"$%-8d",pl[order[i]].cash+pl[order[i]].svalue);
        wattroff(stand,A_BOLD);
    }

    for(i=sum=0;i<NUMCO;i++)   /* get company size */
        sum += co[i].size;

    togo = (MAPX*MAPY * END_PERCENT / 100) - sum + 1;

    sprintf(s,"Available company sectors remaining: %d", togo);
    center(stand,60,5+numplayers, s);
    center(stand,60,7+numplayers,"Press any key to continue...");
    wnoutrefresh(stand);
    doupdate();
    getch();
    delwin(stand);
    touchwin(mapwin);
    touchwin(general);
    touchwin(coinfo);
    wnoutrefresh(mapwin);
    wnoutrefresh(general);
    wnoutrefresh(coinfo);
}

/*
** order_compare() -- for sorting for show_standings
*/

int order_compare(const void *v1, const void *v2)
{
    int nw1,nw2,p1,p2;

    p1 = *((int *)v1);
    p2 = *((int *)v2);
    
    nw1 = pl[p1].svalue + pl[p1].cash;
    nw2 = pl[p2].svalue + pl[p2].cash;

    if (nw1 < nw2) return 1;
    if (nw1 > nw2) return -1;
    return 0;
}

/*
** more_coinfo() -- shows detailed company information
*/

void more_coinfo(void)
{
    WINDOW *more_coinfo;
    int numco=0,cos[NUMCO],i,j,attrs,total;

    for(i=0;i<NUMCO;i++) {  /* get list of active companies */
        if (co[i].size)
            cos[numco++] = i;
    }

    if (numco == 0) return;  /* don't bother if there are no active co's */

    if ((more_coinfo = newwin(7+numco,52,5,(COLUMNS-52)/2)) == NULL) {
        fprintf(stderr,"starlanes: couldn't create more_coinfo window\n");
        exit(1);
    }

    wattron(more_coinfo,MORE_COINFO_BORDER);
    box(more_coinfo,'|','=');
    wattroff(more_coinfo,MORE_COINFO_BORDER);

    wattron(more_coinfo,MORE_COINFO_TITLE);
    center(more_coinfo,52,0," Detailed Company Information ");
    wattroff(more_coinfo,MORE_COINFO_TITLE);

    wattron(more_coinfo,MORE_COINFO_HEADER);
    mvwaddstr(more_coinfo,2,1," Company name          Price   Size   Total Worth ");
    wattroff(more_coinfo,MORE_COINFO_HEADER);


    for(i=0;i<numco;i++) {
        switch(cos[i]) {
            case 0: attrs = CO_A;break;
            case 1: attrs = CO_B;break;
            case 2: attrs = CO_C;break;
            case 3: attrs = CO_D;break;
            case 4: attrs = CO_E;break;
        }

        wattron(more_coinfo,attrs);
        mvwprintw(more_coinfo,i+4,2,"%-20s  ",co[cos[i]].name);
        wattroff(more_coinfo,attrs);
        wprintw(more_coinfo,"$%-4d    ",co[cos[i]].price);
        wprintw(more_coinfo,"%-3d    ",co[cos[i]].size);

        for(j=total=0;j<numplayers;j++)
            total += co[cos[i]].price * pl[j].holdings[cos[i]];

        wattron(more_coinfo,A_BOLD);
        wprintw(more_coinfo,"$%-9d",total);
        wattroff(more_coinfo,A_BOLD);
    }

    center(more_coinfo,52,5+numco,"Press any key to continue...");
    wnoutrefresh(more_coinfo);
    doupdate();
    getch();
    delwin(more_coinfo);
    touchwin(mapwin);
    touchwin(general);
    touchwin(coinfo);
    wnoutrefresh(mapwin);
    wnoutrefresh(general);
    wnoutrefresh(coinfo);
}

/*
** quit_yn() -- asks the users if they're sure they want to quit
*/

void quit_yn(void)
{
    WINDOW *yn;

    if ((yn = newwin(5,42,5,(COLUMNS-42)/2)) == NULL) {
        fprintf(stderr,"starlanes: couldn't open window for y/n prompt\n");
        exit(1);
    }
    
    wattron(yn,QUIT_BORDER);
    box(yn,'|','=');
    wattroff(yn,QUIT_BORDER);
    wattron(yn,QUIT_TITLE);
    center(yn,42,0," Really Quit? ");
    wattroff(yn,QUIT_TITLE);
    wattron(yn,QUIT_TEXT);
    mvwaddstr(yn,2,2,"Are you sure you want to quit (y/n)? ");
    wattroff(yn,QUIT_TEXT);
    wnoutrefresh(yn);
    doupdate();
    if (toupper(getch()) == 'Y') {
        delwin(yn);
        shutdown();
        exit(2);
    }
    delwin(yn);
    touchwin(mapwin);touchwin(coinfo);
    wnoutrefresh(mapwin);wnoutrefresh(coinfo);
}

/*
** shutdown() -- cleans up
*/

void shutdown(void)
{
    show_standings(" GAME OVER - Final Standings ");
    delwin(mapwin);delwin(coinfo);delwin(general);
    noraw();echo();clear();refresh();
    endwin();
    printf("Starlanes for ncurses v%s Copyright by Brian \"Beej\" Hall %s\n",VERSION,VERSION_DATE);
}

/*
** usage() -- prints a stderr usage message
*/

void usage(void)
{
    fprintf(stderr,"usage: starlanes [-v|c|m]\n");
    exit(1);
}

