/*****************************************************************************
 *                                                                           *
 * cpat.h - header for cpat.c                                                *
 *                                                                           *
 *            Copyright (C) 2006 Trevor Carey-Smith                          *  
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify it   *
 * under the terms of the GNU General Public License version 2 as published  *
 * by the Free Software Foundation. (See the COPYING file for more info.)    *
 *                                                                           *
 *****************************************************************************/

#ifndef CPAT_CPAT_H
#define CPAT_CPAT_H 1

#if HAVE_CONFIG_H
#   include "config.h"
#endif

#if STDC_HEADERS
#  include <stdlib.h>
#  include <stdio.h>
#  include <string.h>
#elif HAVE_STRINGS_H
#  include <strings.h>
#endif /*STDC_HEADERS*/

#if HAVE_UNISTD_H
#  include <sys/types.h>
#  include <unistd.h>
#endif

#if HAVE_PWD_H
#include <pwd.h>
#endif

#ifdef HAVE_GETOPT_H
#include <getopt.h>
#else
extern char *optarg;
extern int  optind;
#endif

#include <signal.h>
#include <sys/time.h>
#include <time.h>

#if HAVE_NCURSES_H
#  include <ncurses.h>
#elif HAVE_CURSES_H
#  include <curses.h>
#endif

#if !STDC_HEADERS
#  if !HAVE_STRCHR
#    define strchr index
#    define strrchr rindex
#  endif
#endif

/* If system doesn't have nanosleep, we replace with usleep */
#if !HAVE_NANOSLEEP
#  if !HAVE_STRUCT_TIMESPEC
struct timespec { 
    long    ts_nsec; 
};
#  endif
#  if HAVE_USLEEP
#    define nanosleep(req,rem)    ((void) usleep (*req.ts_nsec/1000))
#  else
#    define nanosleep(req,rem)  
#  endif
#endif /* !HAVE_NANOSLEEP */

#if !HAVE_TIME_T
typedef long time_t;
#endif

/* Time to pause when doing auto moves (millisec).
 * This number is passed to nanosleep(). */
#define PAUSETIME       400

/* Define new games here. Need to add its full name to names below */
#define FCELL           0
#define KLOND           1
#define SPIDER          2
#define FORTYTH         3
#define BAKERSD         4
#define SCORP           5
#define PENGUIN         6
#define YUKON           7
#define STRATEGY        8
#define CANFIELD        9
#define DUCHESS        10
#define NUM_GAMES      11

/* This is the maximum number of variations allowed per game */
#define MAX_VARIATIONS  4

/* This produces white space */
#define NOCARD          (-1)
/* This produces a blank black card */
#define CARDSPACE       (-2)
/* These are for collapsed sequences */
#define CARDSEQR        (-3)
#define CARDSEQB        (-4)
/* This is for a deck of cards. The number of cards in the pile 
 * written on a blue card. Cards less than this number will be blue. */
#define CARDBACK        (-5)
#define CARDRESERVE     (-6)
#define CARDSEPARATOR   (-7)

#define SUIT_LENGTH     13
#define NUM_SUITS       4
#define KING            12
#define ACE             0

/* These are for checking card sequences */
#define ASC             0
#define DESC            1
#define ANY_ORDER       2
#define ALT_COL         0
#define IN_SUIT         1
#define ANY_SUIT        2
#define WRAP            0
#define NO_WRAP         1
#define FACE_UP         0
#define FACE_DOWN       100

#define BACK_COLOR      4
#define HEARTS_COLOR    6
#define SPADES_COLOR    7

#define PACK_SIZE       52

#define MAX_NUM_COLS    13
#define MAX_COL_LEN     66   
/* It is not possible to have more than 19 cards in a column */
/* unless playing spider, when technically you could have 66 ! */

#define CARD_WIDTH      5
#define CARD_HEIGHT     2
/* positioning info */
/* How far down the main board is. */
#define BOARD_Y         1
/* How far down the side boards are. */
#define PILE_Y          2
/* How far across the first side board is. */
#define PILE_X          1
/* The size of the board borders */
#define BORDER          1
/* The size of the space between the boards */
#define SPACE           1

/* These are for the undo type */
#define UNDO_NORMAL     0
#define UNDO_FACE_DOWN  1
#define UNDO_DISCARD    2
#define UNDO_ROLLCARDS  3
#define UNDO_DEAL       4
#define UNDO_RESERVE    5
#define UNDO_FOUN_START 6
#define UNDO_IMPOSSIBLE 7

typedef struct undo_node {
    int number,src,dst;
    int type;
    struct undo_node * nextnode;
} undo_node;

typedef struct {
    WINDOW *main,*free,*found,*input,*hint;
    int hint_h;
    int game;
    /* The columns on the main board */
    int cols[MAX_NUM_COLS][MAX_COL_LEN]; 
    /* Keeps track of how many cards in each column (0 means 1 card) */
    int col_size[MAX_NUM_COLS];
    undo_node * undo;
    int deck[2*PACK_SIZE];
    int face_down,face_up; /* Number of cards in deck and discard (KLON,SPID)*/
    int turn_no; /* this is the number of cards to turn over (Klondike) */
    int freepile[8]; /* The cards to show on the free board */
    int foundation[8]; /* The cards to show on the foundations */
    int foun_size[8]; /* Number of cards in each foundation */
    int foun_dir;
    int num_cols;
    int num_foun;
    int num_free;
    int num_packs;
    int num_deals; /* number of times allowed to deal through deck */
    int foun_start;
    int allow_undo;
    int variation; /* records what variation of the game is being played */
    int seq_moves; /* Whether to allow sequence moves or not */

    /* To notify draw_piles() which columns to draw */
    int print_col[MAX_NUM_COLS]; 

    /* Game stats */
    int seed,moves,deals,finished_foundations;

    int debug;
} GameInfo;

typedef struct {
    int total_games[NUM_GAMES][MAX_VARIATIONS];
    int finished_games[NUM_GAMES][MAX_VARIATIONS];
    int lowest_moves[NUM_GAMES][MAX_VARIATIONS];
    int lowest_deals[NUM_GAMES][MAX_VARIATIONS];
    time_t date_first_game[NUM_GAMES][MAX_VARIATIONS];
    time_t date_recent_game[NUM_GAMES][MAX_VARIATIONS];
    time_t date_best_game[NUM_GAMES][MAX_VARIATIONS];
    int seed[NUM_GAMES][MAX_VARIATIONS];
    char filename[100];
    bool available;
} HighScores;
    
struct timespec pauselength;
struct timespec pauseleft;

HighScores hs;

/* This is the background color of the boards */
static chtype boardbkgd = ' ' | COLOR_PAIR(SPADES_COLOR);

/* Game names */
static char *names[NUM_GAMES] =
    {"FreeCell","Klondike","Spider","40 Thieves","Baker's Dozen","Scorpion",
    "Penguin","Yukon","Strategy","Canfield","Duchess"};

/* Game variations */
static char *variations[NUM_GAMES][MAX_VARIATIONS] =
    {
        {"3 freecells","4 freecells","5 freecells",""},
        {"3 cards/deal","1 card/deal","",""},
        {"","","",""},
        {"normal","easy","",""},
        {"","","",""},
        {"","","",""},
        {"","","",""},
        {"","","",""},
        {"easy","hard","",""},
        {"","","",""},
        {"","","",""}
    };

undo_node *pop_items(undo_node *stackTop);
undo_node *push_items(undo_node *stackTop,int src,int dst,int number,int type);
void clear_undo(GameInfo* g);

void pager(char *title,char* text,int num_phrases, char **phrases);
int menu(char *title,char **queries,int num_queries,
        char **items,int *num_items, char **phrases,int num_phrases);

void freecell(GameInfo* g);
void klondike(GameInfo* g);
void spider(GameInfo* g);
void fortythieves(GameInfo* g);
void bakersdozen(GameInfo* g);
void scorpion(GameInfo* g);
void penguin(GameInfo* g);
void yukon(GameInfo* g);
void strategy(GameInfo* g);
void canfield(GameInfo* g);
void duchess(GameInfo* g);

void die(int onsig);
void xtermtitle(char *text);

#endif /* !CPAT_CPAT_H */
/* cpat.h ends here */
