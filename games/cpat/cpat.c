/*****************************************************************************
 *                                                                           *
 *                              CPat                                         *
 *                         =================                                 *
 *            A curses based implementation of solitaire                     *
 *            Copyright (C) 2006 Trevor Carey-Smith                          *
 *                                                                           *
 *          The program outline and some of the basic functions are          *
 *          taken from bluemoon by T. A. Lister and Eric S. Raymond.         *
 *                     http://www.catb.org/~esr/bluemoon/                    *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify it   *
 * under the terms of the GNU General Public License version 2 as published  *
 * by the Free Software Foundation.                                          *
 *                                                                           *
 * This program is distributed in the hope that it will be useful, but       *
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY*
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License   *
 * for more details.                                                         *
 *                                                                           *
 * You should have received a copy of the GNU General Public License along   *
 * with this program (the COPYING file); if not, write to the Free Software  *
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA *
 *                                                                           *
 *****************************************************************************/

#include "cpat.h"
#include "document.h"
#include "highscores.h"

/* To catch kill signals and exit cleanly */
void
die(int onsig)
{
    if (hs.available) write_hs();
    signal(onsig, SIG_IGN);
    endwin();
    xtermtitle("");
    exit(0);
}

/* Code to provide a stack (linked list) to store moves so they can be 
 * undone...
 */
undo_node *push_items(undo_node *stackTop,int src,int dst,int number,int type)
{
    static undo_node *newItem;
        
    newItem = calloc(sizeof(undo_node),1);
    newItem->src = src;
    newItem->dst = dst;
    newItem->number = number;
    newItem->type = type;
    
    newItem->nextnode = stackTop;
        
    return newItem;
}

undo_node *pop_items(undo_node *stackTop)
{
    static undo_node *temp;
   
    temp = stackTop->nextnode;
    free(stackTop);
        
    return temp;
}

void clear_undo(GameInfo* g)
{
    /* Clean up all the undo levels */
    while (g->undo != NULL) g->undo = pop_items(g->undo);
}
/* Prints given string in xterm title bar. If NULL string given prints
 * value of TERM env variable instead. */
void xtermtitle(char *text)
{
    char *term = getenv("TERM") ? getenv("TERM") : "";
    char *p;
    int l;
    
    if (!strncmp(term,"xterm",5) || !strncmp(term,"Eterm",5) || !strncmp(term,"aterm",5) || !strncmp(term,"rxvt",4) || !strncmp(term,"screen",6))
    {
        l = strlen(text)+30;
        p = (char *) malloc (l);
        snprintf(p,l,"\x1b]1;\x07\x1b]2;%s%s\x07",(l>30?"cpat: ":term),text);
        putp(p);
        free(p);
    }
}

/* Creates windows to write stuff not in a game */
void
pager(char *title,char* text,int num_phrases, char **phrases)
{
    WINDOW *outer,*input;
    int win_w,win_h;
    int i,line,max_line,prev_line;
    char *char_p,**start_line,**end_line;
    char *pagerhelp = "Pager Help:\n\n\
<Up>,<BackSpace>,k   Scroll back one line\n\
<Down>,<Enter>,j     Scroll forward one line\n\
<PageUp>,p           Scroll back one page\n\
<PageDown>,n,<Space> Scroll forward one page\n\
u                    Scroll back half a page\n\
d                    Scroll forward half a page\n\
<Home>               Jump to start of text\n\
<End>                Jump to end of text\n\
?,h                  Show this help\n\
q                    Exit pager";


    /* Values for outer window */
    win_w = COLS-8;
    win_h = LINES-4;
    outer = newwin(win_h,win_w,2,4);
    wbkgdset(outer, boardbkgd);
    wclear(outer);
    box(outer,0,0);

    /* Add title and header lines */
    wattron(outer,A_UNDERLINE);
    mvwprintw(outer,2,4,title);
    xtermtitle(title);
    wattroff(outer,A_UNDERLINE);
    for (i = 0;i < num_phrases;i++) mvwprintw(outer,4+i,4,phrases[i]);
    wrefresh(outer);

    /* Values for inner window */
    win_w = win_w-7;
    win_h = win_h-5-num_phrases;
    input = newwin(win_h,win_w,6+num_phrases,8);
    wbkgdset(input, boardbkgd);
    keypad(input,TRUE);

    max_line = win_h;
    start_line = (char**) malloc (sizeof(char *)*max_line);
    end_line = (char**) malloc (sizeof(char *)*max_line);
    i = 0;
    char_p = &text[0]+strlen(text);
    start_line[i] = &text[0];
    while (start_line[i] < char_p)
    {
        if (i >= max_line-1)
        {
            max_line += win_h;
            start_line = (char**) realloc(start_line,sizeof(char *)*max_line);
            end_line = (char**) realloc(end_line,sizeof(char *)*max_line);
        }
        end_line[i] = start_line[i]-1;
        /* largest possible start of next line */
        start_line[i+1] = start_line[i]+win_w-1;
        if (start_line[i+1] > char_p) start_line[i+1] = char_p;

        /* search from start of current line for newline chr */
        while (*++end_line[i] != '\n' && end_line[i] <= start_line[i+1]);

        if (end_line[i] > char_p) end_line[i] = char_p;
        else if (end_line[i] > start_line[i+1])
            /* If no newline found */
            /* search for the space closest to the end of current line*/
            while (*--end_line[i] != ' ' && end_line[i] > start_line[i]);
        
        if (end_line[i] == start_line[i] && *end_line[i] != '\n')
            /* This means there's no space on the line, we will
             * break the long line up */
            end_line[i] = start_line[i+1];
        else
            /* +1 means we don't print space */
            start_line[i+1] = end_line[i]+1;
        i++;
    }
    max_line = i-1;

    char_p = (char *) malloc (win_w);
    prev_line = line = 0;
    while (prev_line > -2)
    {
        wclear(input);
        wmove(input,0,0);
        for (i = line;i < line+win_h-2;)
        {
            strncpy(char_p,start_line[i],(int)(end_line[i]-start_line[i]));
            char_p[(int)(end_line[i]-start_line[i])] = '\0';
            wprintw(input,"%s\n",char_p);
            if (i++ == max_line) break;
        }
        wattron(input,A_REVERSE);
        mvwprintw(input,win_h-1,0," Lines %d-%d/%d ",line+1,i,max_line+1);
        mvwprintw(input,win_h-1,win_w-15," q:exit ?:help");
        wattroff(input,A_REVERSE);
        wrefresh(input);

        prev_line = line;
        while (prev_line == line)
        {
            switch (wgetch(input))
            {
                case KEY_HOME:
                    line = 0;
                    break;
                case KEY_END:
                    line = max_line-win_h+3;
                    break;
                case KEY_DOWN: case 'j': case KEY_ENTER: case 13:
                    line++;
                    break;
                case KEY_UP: case 'k': case KEY_BACKSPACE: case 127:
                    line--;
                    break;
                case KEY_PPAGE: case 'p':
                    line -= win_h-2;
                    break;
                case KEY_NPAGE: case ' ': case 'n':
                    line += win_h-2;
                    break;
                case 'u':
                    line -= (win_h-2)/2;
                    break;
                case 'd':
                    line += (win_h-2)/2;
                    break;
                case 'q':
                    prev_line = -2; /* force page refresh and quit */
                    break;
                case '?': case 'h':
                    wclear(input);
                    mvwprintw(input,0,0,pagerhelp);
                    wattron(input,A_REVERSE);
                    mvwprintw(input,win_h-1,win_w-18," Press any key...");
                    wattroff(input,A_REVERSE);
                    wrefresh(input);
                    wgetch(input);
                    prev_line = -1; /* force page refresh */
                    break;
            }
            if (line > max_line-win_h+3) line = max_line-win_h+3;
            if (line < 0) line = 0;
        }
    }
    free(start_line);
    free(end_line);
    free(char_p);
    /* Now delete windows */
    delwin(input);
    delwin(outer);
    clear();
    refresh();
}

/* Creates a blue menu type screen */
int
menu(char *title,char **queries,int num_queries,
        char **items,int *num_items,
        char **phrases,int num_phrases)
{
    WINDOW *outer,*input;
    char inp;
    int title_y;
    int phrases_y;
    int inner_w,inner_h,inner_x,inner_y;
    int outer_w,outer_h,outer_x,outer_y;
    int lines[num_queries],total_lines,i,j,total_items;
    int name_len = 0;

    /* Initial values for outer window */
    outer_w = COLS-8;
    outer_h = LINES-4;
    outer_x = 4;
    outer_y = 2;
    title_y = 2;
    phrases_y = 4;
    
    total_items = 0;
    for (i = 0;i < num_queries;i++)
    {
        total_items += num_items[i];
        lines[i] = (num_items[i]+1)/2; /* two column view */
    }

    for (i = 0;i < total_items;i++)
        if ((int)strlen(items[i])>name_len)
            name_len = strlen(items[i]);

    /* First make adjustments to width of windows */
    inner_w = 2*(name_len+10);
    if (total_items < 6 || inner_w > outer_w)
    {
        /* Outer win way too thin, change to one column view */
        inner_w = (name_len+14);
        if (inner_w < 37) inner_w = 37;
        if (inner_w > COLS-14) inner_w = COLS-14;
        for (i = 0;i < num_queries;i++)
            lines[i] = num_items[i]; /* one column view */
    }
    else if (inner_w > outer_w-4)
    {
        /* Outer win only just to thin, increase it by 4 */
        outer_w = COLS-4;
        outer_x = 2;
    }

    total_lines = 0;
    for (i = 0;i < num_queries;i++)
        total_lines += lines[i];

    inner_x = outer_x+(outer_w-inner_w)/2;
    inner_h = total_lines+num_queries+4;
    inner_y = outer_y+num_phrases+(num_phrases?5:4);

    if (inner_h > outer_h-num_phrases-1)
    {
        endwin();
        puts ("CPat Error: your screen is too small.");
        exit (1);
    }

    if (inner_h > outer_h-num_phrases-(num_phrases?8:7))
    {
        outer_y--;
        inner_y--;
        outer_h += 2;
        total_lines = outer_h-inner_h-num_phrases-1;
        if (total_lines < 5) inner_y -= (num_phrases?5:4)-total_lines;
        if (total_lines < 4)
        {
            title_y--;
            phrases_y -= 4-total_lines;
        }
    }

    outer = newwin(outer_h,outer_w,outer_y,outer_x);

    wbkgdset(outer, boardbkgd);
    wclear(outer);
    box(outer, 0, 0);

    wattron(outer,A_UNDERLINE);
    mvwprintw(outer,title_y,4,title);
    xtermtitle(title);
    wattroff(outer,A_UNDERLINE);
    for (i = 0;i < num_phrases;i++) 
        mvwprintw(outer,phrases_y+i,6,phrases[i]);
    wrefresh(outer);

    input = newwin(inner_h,inner_w,inner_y,inner_x);
    wattron(input,A_REVERSE | COLOR_PAIR(BACK_COLOR) | A_BOLD);
    wbkgdset(input, COLOR_PAIR(BACK_COLOR) | A_REVERSE | A_BOLD);
    wclear(input);

    total_lines = 1;
    total_items = 0;
    for (j = 0;j < num_queries;j++)
    {
        mvwaddstr(input, total_lines++,2,queries[j]);
        for (i = 0;i < num_items[j];i++)
        {
            mvwprintw(input,total_lines+i%lines[j],i<lines[j]?4:name_len+12,"[%c] - %s",'a'+total_items,items[total_items]);
            total_items++;
        }
        total_lines += lines[j];
    }

    mvwaddstr(input,++total_lines,2,"Enter your choice ('q' to quit): ");
    while (!(inp = wgetch(input)) || 
            (inp != 'q' && (inp < 'a'||inp >= 'a'+total_items)));

    if (inp == 'q') die(0);

    delwin(input);
    delwin(outer);
    clear();
    refresh();
    return (inp-'a');
}

int
main(int argc, char **argv, char *envp[])
{
#ifdef HAVE_GETOPT_LONG
    struct option  long_options [] = {
        { "seed", required_argument, 0, 's' },
        { "no-record", no_argument, 0, 'R' },
        { "help", no_argument, 0, 'h' },
        { "fast", no_argument, 0, 'f' },
        { "cheat", no_argument, 0, 'c' },
        { "debug", no_argument, 0, 'd' },
        { "version", no_argument, 0, 'V' },
        { NULL, 0, NULL, 0}
    };
#endif
    char title[40];
    char *short_options="s:RdfchV";
    char *home;
    int i;
    int  help_flag = 0;
    int  version_flag = 0;
    int  error_flag = 0;
    int  fast_flag = 1;
    const char *prog_name;

    char *items[NUM_GAMES+5] = {
        "Help","Credits","License","Game Stats","High Scores"
    };
    char *queries[2] = {"Choose a game","or an option:"};
    int num_items[2] = {NUM_GAMES,5};

    GameInfo g;

    hs.available=TRUE;

    g.debug = 0;
    g.allow_undo = 0;
    g.undo = NULL;
    
    g.seed = (int)time((time_t *)0);

    /* `basename' seems to be non-standard.  So we avoid it.  */
    prog_name = strrchr (argv[0], '/');
    prog_name = strdup (prog_name ? prog_name+1 : argv[0]);


    (void) signal(SIGINT, die);

    while (! error_flag) {
        int  c;
#ifdef HAVE_GETOPT_LONG
        int  ind;
        c = getopt_long (argc, argv, short_options, long_options, &ind);
#else
        c = getopt (argc, argv, short_options);
#endif
        if (c == -1)  break;
        switch (c) {
            case 's':
                g.seed = atoi(optarg);
                break;
            case 'R':
                hs.available = FALSE;
                break;
            case 'd':
                g.debug = 1;
                break;
            case 'c':
                g.allow_undo = 1;
                break;
            case 'h':
                help_flag = 1;
                break;
            case 'f':
                fast_flag*=2;
                break;
            case 'V':
                version_flag = 1;
                break;
            default:
                error_flag = 1;
        }
    }
    if (argc != optind) {
        fputs ("too many arguments\n", stderr);
        error_flag = 1;
    }
    if (version_flag) {
        puts (PACKAGE_STRING);
        if (! error_flag)  exit (0);
    }
    if (error_flag || help_flag) {
#ifdef HAVE_GETOPT_LONG
#  define P(both,short,long) "  " both ", " long "    "
#else
#  define P(both,short,long) "  " both " " short "    "
#endif
        FILE *out = error_flag ? stderr : stdout;
        fprintf (out, "usage: %s [OPTION...]\n", prog_name);
        fprintf (out, 
                "%s - a curses based solitaire collection\n\n", PACKAGE_NAME);
        fputs (P("-h","    ","--help     ") "print this message\n", out);
        fputs (P("-f","    ","--fast     ") "for faster auto-moves\n", out);
        fputs (P("-s","SEED","--seed=SEED") "seed for the deck shuffle\n", out);
        fputs (P("-R","    ","--no-record") "do not record game stats\n", out);
        fputs (P("-c","    ","--cheat    ") "allow undo for all moves\n", out);
        fputs (P("-V","    ","--version  ") "print the version number\n", out);
        fputs ("\nPlease report bugs to " PACKAGE_BUGREPORT ".\n", out);
        exit (error_flag);
#undef P
    }

    home = getenv("HOME");
    if (home == NULL) 
    {
#if HAVE_PWD_H
        home = getpwuid(getuid())->pw_dir; 
#else
        hs.available=FALSE;
#endif
    }
    if (hs.available) 
    {
        (void)strncpy(hs.filename,home,100);
        (void)strncat(hs.filename,"/.cpat_scores",13);

        initialise_hs();
        read_hs();
    }

    pauselength.tv_nsec= PAUSETIME*1000000/fast_flag;

    srand(g.seed);

    initscr();
    start_color();
    init_pair(  HEARTS_COLOR,   COLOR_WHITE,    COLOR_RED);
    init_pair(  SPADES_COLOR,   COLOR_BLACK,    COLOR_WHITE);
    init_pair(  BACK_COLOR,     COLOR_BLUE,     COLOR_WHITE);

    cbreak();
    nonl();
    noecho();
//    intrflush(stdscr,FALSE);

    for (i = 0;i < 5;i++) items[i+NUM_GAMES] = items[i];
    for (i = 0;i < NUM_GAMES;i++) items[i] = names[i];

    while (1)
    {
        /* g.num_deals=0 so that hs ignores numdeals unless this is set */
        g.num_deals = 0;
        g.variation = 0;
        snprintf(title,40,"Welcome to CPat %s",VERSION);
        g.game=menu(title,queries,2,items,num_items,items,0);
        switch (g.game) {
            case FCELL:
                freecell(&g);
                break;
            case KLOND:
                klondike(&g);
                break;
            case SPIDER:
                spider(&g);
                break;
            case FORTYTH:
                fortythieves(&g);
                break;
            case BAKERSD:
                bakersdozen(&g);
                break;
            case SCORP:
                scorpion(&g);
                break;
            case PENGUIN:
                penguin(&g);
                break;
            case YUKON:
                yukon(&g);
                break;
            case STRATEGY:
                strategy(&g);
                break;
            case CANFIELD:
                canfield(&g);
                break;
            case DUCHESS:
                duchess(&g);
                break;
            case NUM_GAMES:
                help();
                break;
            case NUM_GAMES+1:
                credits();
                break;
            case NUM_GAMES+2:
                license();
                break;
            case NUM_GAMES+3:
                game_stats(0);
                break;
            case NUM_GAMES+4:
                game_stats(1);
                break;
        }
        srand(++g.seed);
    }

    clear_undo(&g);
    die(0);
}
/* cpat.c ends here */
