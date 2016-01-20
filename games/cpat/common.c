/*****************************************************************************
 *                                                                           *
 * common.c - common functions used by all the solitaire games.              *
 *                                                                           *
 *            Copyright (C) 2006 Trevor Carey-Smith                          *  
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify it   *
 * under the terms of the GNU General Public License version 2 as published  *
 * by the Free Software Foundation. (See the COPYING file for more info.)    *
 *                                                                           *
 *****************************************************************************/

#include "cpat.h"
#include "common.h"
#include "document.h"

/* Creates empty board windows. Needs to be called once at the
 * beginning of each game */
int 
create_windows(GameInfo* g)
{
    int freepile_y,freepile_x,freepile_h,freepile_w;
    int founpile_y,founpile_h,founpile_w;
    int main_y,main_x,main_h,main_w;
    int input_x,input_y,input_h,input_w;
    int hint_x,hint_y,hint_w;
    int max_x,max_y;

    g->hint_h = (g->debug) ? 13 : 12;  /* initial height of hint board */
    hint_w=14;  /* width of hint board */
    input_h = 5;
    input_w = 15;
 
    founpile_y = PILE_Y;
    founpile_h = (g->num_foun) ? g->num_foun*CARD_HEIGHT+2*BORDER+1 : 0;
    founpile_w = (g->num_foun) ? CARD_WIDTH+2*BORDER : 0;
    main_w = g->num_cols*CARD_WIDTH+2*BORDER;
    main_h = LINES-BOARD_Y-1;
    main_x = PILE_X+founpile_w+(g->num_foun?SPACE:0);
    main_y = BOARD_Y;
    freepile_y = PILE_Y;
    /* horiz coordinate of left side of freeboard (board to the
     * right of the main board */
    freepile_x = main_x+main_w+SPACE;
    /* Height of freepile */
    freepile_h = (g->num_free) ? g->num_free*CARD_HEIGHT+2*BORDER+1 : 0;
    freepile_w = (g->num_free) ? CARD_WIDTH+2*BORDER : 0;

    /* Check if screen size big enough */
    max_y = PILE_Y + ((freepile_h > founpile_h) ? freepile_h : founpile_h);
    max_x = freepile_x+freepile_w;
    /* Try putting freepile below founpile */
    if (max_x > COLS && g->num_foun && g->num_free)
    {
        founpile_y = BOARD_Y;
        freepile_x = PILE_X;
        freepile_y = BOARD_Y + founpile_h + SPACE;
        max_x = main_x+main_w;
        max_y = freepile_y+freepile_h;
    }
    if (max_x > COLS || max_y > LINES)
    {
        clear();
        (void)printw( "Screen size too small. Enlarge to %dx%d.",max_x,max_y);
        refresh();
        return(1);
    }

    hint_x = freepile_x + freepile_w + (g->num_free?SPACE:0);
    hint_y = PILE_Y;
    input_x = freepile_x;
    /* If freepile below founpile, put some hints below mainboard */
    if (freepile_x == PILE_X)
    {
        hint_x = main_x;
        hint_w = main_w;
        g->hint_h = 1;
        main_h -= 2;
        hint_y = main_y+main_h+SPACE;
        /* try putting input at bottom on right of main board */
        input_y = BOARD_Y;
        input_x = main_x+main_w+SPACE;
    }
    /* If no room on right of boards, put hints below the freepile board */
    else if (COLS - hint_x < hint_w)
    {
        hint_x = freepile_x;
        hint_y = PILE_Y+freepile_h+SPACE;
        /* reduce number of hints if not enough room */
        if (LINES-(hint_y+input_h+SPACE) < g->hint_h)
            g->hint_h = LINES-(hint_y+input_h+SPACE);
        if (g->hint_h <= 1 || COLS - hint_x < hint_w)
        {
            /* still no room we put some info below the main board */
            hint_x = main_x;
            hint_w = main_w;
            g->hint_h = 1;
            main_h -= 2;
            hint_y = main_y+main_h+SPACE;
            /* try putting input at bottom on right of main board */
            input_y = PILE_Y+freepile_h+SPACE;
        }
        else
            /* put input below freepile and hint board*/
            input_y = hint_y+g->hint_h+SPACE;
    }
    else
    {
        /* put hint beside freepile */
        /* calculate top of input board */
        if (freepile_h < g->hint_h) 
        {
            /* Line top of input with bottom of hint */
            if (LINES-(hint_y+input_h+SPACE) < g->hint_h)
                g->hint_h = LINES-(hint_y+input_h+SPACE);
            input_y = hint_y+g->hint_h+SPACE;
        }
        else
        {
            /* Line top of input with bottom of freepile */
            input_y = PILE_Y+freepile_h+SPACE;
            if (LINES-input_y < input_h)
            {
                /* try putting input beside freepile, below hints */
                input_y = hint_y+g->hint_h+SPACE;
                input_x = hint_x;
            }
        }
    }

    if (input_w < COLS-input_x-SPACE)
        input_w = COLS-input_x-SPACE;
    /* Check if enough room for input, if not put below main board */
    if (LINES-input_y < input_h || COLS-input_x-SPACE < input_w)
    {
        input_x = main_x;
        input_y = LINES-2;
        input_w = main_w;
        input_h = 2;
        main_h -= 2;
        if (g->hint_h==1)
            hint_y -= 2;
    }
    
    /* Now create windows */
    if (g->num_foun)
        g->found = newwin(founpile_h,founpile_w,founpile_y,PILE_X);
    g->main = newwin(main_h,main_w,main_y,main_x);
    if (g->num_free)
        g->free = newwin(freepile_h,freepile_w,freepile_y,freepile_x);
    g->hint = newwin(g->hint_h,hint_w,hint_y,hint_x);
    g->input = newwin(input_h,input_w,input_y,input_x);

    return(0);
}

/* Kill all board windows. Called at end of each game. */
void 
kill_windows(GameInfo* g)
{
    if (g->num_foun) delwin(g->found);
    delwin(g->main);
    if (g->num_free) delwin(g->free);
    delwin(g->input);
    delwin(g->hint);
    clear();
    refresh();
}

/* creates the deck and shuffles it. */
void
init_deck(GameInfo* g)
{
    int i,j,numswaps,swapnum,temp;
    time_t now;

    /* Clean up all the undo levels */
    clear_undo(g);

    /* Increment number of games started in the hs */
    if (hs.available) 
    {
        hs.total_games[g->game][g->variation]++;
        now = time ( NULL );
        hs.date_recent_game[g->game][g->variation]=now;
        if (hs.date_first_game[g->game][g->variation]==-1)
            hs.date_first_game[g->game][g->variation]=now;
    }

    /* create deck */
    for (i=0; i < PACK_SIZE; i++)
        for (j=0;j < g->num_packs; j++)
            g->deck[i+j*PACK_SIZE]=i;

    /* shuffle the deck */
    numswaps=g->num_packs*PACK_SIZE*10;         /* an arbitrary figure */
    for (swapnum=0;swapnum<numswaps;swapnum++)
    {
        i=rand() % (g->num_packs*PACK_SIZE);
        j=rand() % (g->num_packs*PACK_SIZE);
        temp=g->deck[i];
        g->deck[i]=g->deck[j];
        g->deck[j]=temp;
    }
}

/* This is called when the deck is first dealt onto the tableau.
 * g->col_size needs to be set before this is called.
 */
void
deal_deck(int orient, GameInfo* g)
{
    int k=0,i,j;

    /* Deal the pack into the columns, if FACE_DOWN, then turn the cards
     * over. and pad the columns with empty spaces */
    for (i=0;i < g->num_cols; i++)
        for (j=0;j < MAX_COL_LEN;j++)
            g->cols[i][j] = (j<=g->col_size[i]) ? 
                g->deck[k++]-(j<g->col_size[i] ? orient :0) : CARDSPACE;

    /* Shift remaining cards to start of deck */
    for (i = k;i < g->num_packs*PACK_SIZE;i++)
        g->deck[i-k]=g->deck[i];

    /* Number of cards left in the deck */
    g->face_down = g->num_packs*PACK_SIZE - k;
    /* Number of cards turned over */
    g->face_up = 0;
}

void
turnover_waste(GameInfo* g)
{
    /* This is for if we moved a card off the waste, we need
     * to turn over the card underneath */
    if (g->freepile[1]==NOCARD && g->face_up>0) 
    {
        g->freepile[1]= (--g->face_up>0) 
            ? g->deck[g->num_packs*PACK_SIZE-g->face_up] : NOCARD;
        draw_piles(g->free,g);
        g->undo->type=UNDO_DISCARD;
    }
}

void
foundation_automove(int number,GameInfo* g)
{
    int number_found,j,suit,rank,card;

    number_found=0;
    /* Loop through columns then FreeCells */
    for (j=0;j<g->num_cols+g->num_free;j++)
    {
        if (j<g->num_cols)
        {
            if (g->col_size[j]==NOCARD)
                continue; /* Goto next column */
            else
                card=g->cols[j][g->col_size[j]];
        } 
        else
        {
            if (g->freepile[j-g->num_cols]<=NOCARD)
                continue; /* Goto next column */
            else
                card=g->freepile[j-g->num_cols];
        }

        /* finds what the rank would be if foundation started at ACE */
        /* i.e. the number of cards (minus 1) that would be on the
         * foundation if this card were on the top. */
        rank = (card+SUIT_LENGTH-g->foun_start)%SUIT_LENGTH;
        /* suit can go from 0-7 */
        suit = (g->foun_size[card/SUIT_LENGTH] >
                ((g->foun_dir==DESC)?SUIT_LENGTH-1-rank:rank))
            ? card / SUIT_LENGTH + NUM_SUITS : card / SUIT_LENGTH;
        if (((g->foundation[suit]+SUIT_LENGTH-g->foun_start)%SUIT_LENGTH==rank-1
                    && g->foundation[suit]!=NOCARD)
                || (g->foundation[suit]==NOCARD
                    && card%SUIT_LENGTH==g->foun_start)) {
            if (number_found++) nanosleep(&pauselength,&pauseleft);
            move_card(j,g->num_cols+g->num_free,1,g);
            j=-1;
            turnover_waste(g);
        }
        if (number_found==number) break; /* Goto last column */
    }
    if (number_found==0)
        show_error("No cards to move to foundations.",g->input);
}

void
roll_deckcards(GameInfo* g)
{
    int j;
    char temp_str[30];
    
    if (g->face_down==0) 
    {
        if (g->deals < g->num_deals-1)
        {
            /* Unless there are no cards left */
            if (g->face_up==0) {
                show_error("No cards left in the deck.",g->input);
                return;
            }
            for (j=0;j<g->face_up;j++)
                g->deck[j]=g->deck[g->num_packs*PACK_SIZE-(g->face_up-j)];
            g->face_down=g->face_up;
            g->face_up=0;
            g->freepile[0]=CARDBACK;
            g->freepile[1]=NOCARD;
            draw_piles(g->free,g);
            snprintf(temp_str,30,"Finished deal number %d",++g->deals);
            show_error(temp_str,g->input);
            g->undo = push_items(g->undo,0,0,0,UNDO_ROLLCARDS);
        }
        else
            show_error("No deals left.",g->input);
    }
    else
    {
        /* roll over turn_no cards */
        for (j=0;j<g->turn_no;j++) 
        {
            /* shift card to end of array and adjust counters */
            g->deck[g->num_packs*PACK_SIZE-(++g->face_up)] = 
                g->deck[--g->face_down];
            /* If we reach end of pack, stop */
            if (g->face_down==0)
            {
                g->freepile[0]=CARDSPACE;
                show_error("Finished deck.",g->input);
                j++;
                break;
            }
        }
        /* Transfer card value to freepile[1] so it can be seen */
        g->freepile[1]=g->deck[g->num_packs*PACK_SIZE-g->face_up];
        draw_piles(g->free,g);
        g->undo = push_items(g->undo,0,0,j,UNDO_ROLLCARDS);
    }
}

/* checks for the correct sequence of cards in a given tableau column
 * First checks if enough cards in column to move.
 * number: number of cards to be in sequence.
 * col: column-label (0-num_cols-1)
 * direction: either ASC, DESC or ANY_ORDER, ASC means the rank 
 *          ascends from the bottom
 * of the column to the top.
 * type: IN_SUIT  - checks if cards are the same suit
 *       ALT_COL  - checks if cards alternate red/black/red/...
 *       ANY_SUIT - ignores suit
 * wrap: either WRAP of NO_WRAP. If WRAP sequences can loop from Ace to King.
 * returns 1 for fail.
 */
int
check_sequence(int number,int col,int direction,int type,int wrap,GameInfo* g)
{
    int rank,card,suit,j;

    /* Check if enough cards in column */
    if (g->col_size[col] < number-1 || 
            /* this is to make sure the cards are face up */
            g->cols[col][g->col_size[col]-number+1] < 0)
    {
        show_error("Not enough cards on that pile.",g->input);
        return(1);
    }

    /* Check sequence */
    suit = g->cols[col][g->col_size[col]] / SUIT_LENGTH;
    for (j=1; j < number; j++)
    {
        card = g->cols[col][g->col_size[col]-j];
        rank=g->cols[col][g->col_size[col]-j+1]%SUIT_LENGTH+(direction==ASC?1:-1);
        if (wrap == WRAP)
            rank = rank < ACE ? KING : (rank > KING ? ACE : rank);
        if ((direction!=ANY_ORDER && card % SUIT_LENGTH != rank)
                || (type == IN_SUIT && card/SUIT_LENGTH!=suit)
                || (type == ALT_COL && (card/SUIT_LENGTH)%2!=(suit+j)%2))
        {
            show_error("Stack not in correct sequence.",g->input);
            return(1);
        }
    }
    return(0);
}

/* Checks if a card move is valid.
 * col: id of column to move to (0-num_cols-1). If col is a foundation, it
 *      will check for that (currently ignores direction and type).
 * card: value of card being moved (0-51)
 * direction: ASC or DESC; ASC means its valid to put down a larger card
 * type: IN_SUIT or ALT_COL
 * returns 1 for fail.
 */
int
check_move(int col,int card,int direction,int type,int wrap,GameInfo* g)
{
    int rank,suit;

    if (col >= g->num_cols+g->num_free)
    {
        /* finds what the rank would be if foundation started at ACE */
        /* i.e. the number of cards that would be on the
         * foundation if this card were to go on next. */
        rank = (card+SUIT_LENGTH-g->foun_start)%SUIT_LENGTH;
        /* suit can go from 0-7 */
        suit = (g->foun_size[card/SUIT_LENGTH] >
                ((g->foun_dir==DESC)?SUIT_LENGTH-1-rank:rank))
            ? card / SUIT_LENGTH + NUM_SUITS : card / SUIT_LENGTH;
        if (((g->foundation[suit]+SUIT_LENGTH-g->foun_start)%SUIT_LENGTH!=rank-1
                    || g->foundation[suit]==NOCARD)
                && (g->foundation[suit]!=NOCARD
                    || card%SUIT_LENGTH!=g->foun_start))
        {
            show_error("Bad move.",g->input);
            return(1);
        }
    }
    else
    {
        suit = g->cols[col][g->col_size[col]]/SUIT_LENGTH; //card / SUIT_LENGTH;
        rank = (card % SUIT_LENGTH)+(direction==DESC?1:-1);
        if (wrap == WRAP)
            rank = rank < ACE ? KING : (rank > KING ? ACE : rank);
        /* Check if suit is correct colour and rank */
        if (g->cols[col][g->col_size[col]]%SUIT_LENGTH!=rank
                || (type==ALT_COL && suit%2==(card/SUIT_LENGTH)%2)
                || (type==IN_SUIT && suit!=card/SUIT_LENGTH))
        {
            show_error("Can't move card there.",g->input);
            return(1);
        }
    }
    return(0);
}

/* Finds a valid card move (for some games if n==0 we do this)
 * src: id of column to move from (0-num_cols-1).
 * dst: id of column to move to (0-num_cols-1).
 * direction: ASC or DESC; ASC means its valid to put down a larger card
 * type: IN_SUIT or ALT_COL
 * returns 1 for fail.
 */
int
find_move(int src,int dst,int direction,int type,int wrap,GameInfo* g)
{
    int i,rank,suit;
    int number=0,count=0;

    if (g->col_size[dst] >= 0)
    {
        if (wrap == NO_WRAP)
        {
            if (g->cols[dst][g->col_size[dst]]%SUIT_LENGTH
                    == (direction==ASC?KING:ACE))
            {
                show_error("Can't move a card onto that column.",g->input);
                return 0;
            }
        }
        suit = g->cols[dst][g->col_size[dst]] / SUIT_LENGTH;
        rank = g->cols[dst][g->col_size[dst]]%SUIT_LENGTH+(direction==ASC?1:-1);
        if (wrap == WRAP)
            rank = rank < ACE ? KING : (rank > KING ? ACE : rank);
        /* Check if suit is correct colour and rank */
        for (i=g->col_size[src];i>=0;i--)
        {
            if (g->cols[src][i]%SUIT_LENGTH==rank &&
                    ((type==ALT_COL && (g->cols[src][i]/SUIT_LENGTH)%2!=suit%2)
                     ||(type==IN_SUIT && g->cols[src][i]/SUIT_LENGTH==suit)))
            {
                number = 1+g->col_size[src]-i;
                /* If moving one card is valid do that */
                if (number == 1) break;
                count++;
            }
        }
    }
    else
    {
        for (i=g->col_size[src];i>=0;i--)
        {
            if (wrap == WRAP ||
                    g->cols[src][i]%SUIT_LENGTH==(direction==ASC?ACE:KING))
            {
                number=1+g->col_size[src]-i;
                count++;
            }
        }
    }
    if (number==0)
        show_error("No card can go there.",g->input);
    if (count > 1)
    {
        show_error("Which card do you want to move?",g->input);
        number = 0;
    }
    return number;
}

/* This is called if grab_input() returns 1 or the game is won.
 * game_str should contain game stats to display.
 * The function returns 1 if the user decides not to quit - this is
 * impossible if the game has been won.
 */
int 
game_finished(GameInfo* g,char* game_str)
{
    int num_items[1] = {4};
    int num_phrases = 2;
    time_t now;
    char *queries[1] = {"Choose an option:"};
    char *items[4] = {
        "return to main menu",
        "restart with the same seed",
        "restart with a new seed",
        "continue playing"
    };
    char *phrases[3] = {
        (g->finished_foundations==g->num_foun)
            ? "Congratulations, you finished the game."
            : "A bit hard for you was it??",
        game_str,
        "Well done, you set a new high score!"
    };

    kill_windows(g);

    /* Increment number of games finished in the hs */
    if (g->finished_foundations==g->num_foun && hs.available)
    {
        now = time ( NULL );
        hs.finished_games[g->game][g->variation]++;
        if (g->num_deals)
        {
            if (g->deals<hs.lowest_deals[g->game][g->variation] || hs.lowest_deals[g->game][g->variation]==-1)
            {
                hs.lowest_deals[g->game][g->variation]=g->deals;
                hs.lowest_moves[g->game][g->variation]=g->moves;
                hs.date_best_game[g->game][g->variation]=now;
                hs.seed[g->game][g->variation]=g->seed;
                num_phrases=3;
            }
            else if (g->deals==hs.lowest_deals[g->game][g->variation] && g->moves<hs.lowest_moves[g->game][g->variation])
            {
                hs.lowest_moves[g->game][g->variation]=g->moves;
                hs.date_best_game[g->game][g->variation]=now;
                hs.seed[g->game][g->variation]=g->seed;
                num_phrases=3;
            }
        }
        else if (g->moves<hs.lowest_moves[g->game][g->variation] || hs.lowest_moves[g->game][g->variation]==-1)
        {
            hs.lowest_moves[g->game][g->variation]=g->moves;
            hs.date_best_game[g->game][g->variation]=now;
            hs.seed[g->game][g->variation]=g->seed;
            num_phrases=3;
        }
    }
    
    num_items[0] -= (g->finished_foundations==g->num_foun)?1:0;
    switch(menu(names[g->game],queries,1,items,num_items,phrases,num_phrases))
    {
        case 0: 
            return 0;
        case 1:
            srand(g->seed);
            return 2;
        case 2:
            srand(++g->seed);
            return 2;
        case 3:
            return 1;
    }
    return 1;
}

/* Prints a card! x and y contain the placement of the card relative
 * to the where the top left card on this board is placed. i.e. to 
 * print the top left card x and y should be 0.
 * win should contain the name of the board to print to.
 * This function is low-level, draw_piles() should be called from game
 * functions instead.
 */
void 
printcard(WINDOW *win,int y,int x,int value,GameInfo* g)
{
    wmove(win,y+2,x+2);
    if (value <= CARDBACK)
    {
        wattron(win,A_REVERSE | COLOR_PAIR(BACK_COLOR));
        if (value==CARDBACK)
            (void) wprintw(win,"%3d",g->face_down);
        else if (value==CARDRESERVE)
            (void) wprintw(win,"%3d",g->col_size[g->num_cols]+1);
        else
            (void) waddstr(win,"   ");
    }
    else if (value == NOCARD)
    {
        wattron(win,A_REVERSE | COLOR_PAIR(SPADES_COLOR));
        (void) waddstr(win,"   ");
    }
    else if (value == CARDSPACE)
        (void) waddstr(win,"   ");
    else if (value == CARDSEQR)
    {
        wattron(win,A_REVERSE | COLOR_PAIR(HEARTS_COLOR));
        (void) waddstr(win,"SEQ");
    }
    else if (value == CARDSEQB)
    {
        wattron(win,COLOR_PAIR(SPADES_COLOR));
        (void) waddstr(win,"SEQ");
    }
    else
    {
        wattron(win,A_BOLD | COLOR_PAIR(HEARTS_COLOR+value/SUIT_LENGTH%2));
        if ((value/SUIT_LENGTH)%2) wattron(win,A_REVERSE);
        wprintw(win,"%s%c",ranks[value%SUIT_LENGTH],suits[value/SUIT_LENGTH]);
    }
    wattrset(win,A_NORMAL);
}

/* Draws cards on the three different boards.
 * win: pointer to board window to draw cards on.
 * If win is free or foun board, all the cards are drawn
 * If win is main, columns are drawn if print_col[] is 1.
 * For the main board, cards are usually drawn with a space between 
 * them (vertically), but if there is not enough room, cards are squashed
 * together starting at the top. If there is still no room, cards of the
 * same suit that are in sequence are replaced with SEQ to save space.
 */
void 
draw_piles(WINDOW *win, GameInfo* g)
{
    int row,col;
    int sq_rows,in_col;
    int *p,num_rows;
    int maxy,maxx;
    /* Each main board columns is copied to temp_col to facilitate
     * collapsing sequences */
    int temp_col[MAX_COL_LEN];
    int temp_card,seq;

    if (win==g->main)
    {
        for(col=0;col < g->num_cols; col++)
        {
            if (g->print_col[col]==0)
                continue;

            /* Reset print switch */
            g->print_col[col]=0;
            /* Redo the separator line if printing the column beside it */
            if (col+1 < g->num_cols && g->col_size[col+1]==CARDSEPARATOR)
                g->print_col[col+1]=1;

            getmaxyx(g->main,maxy,maxx);
            /* First clear the column (-3 for borders and letter) */
            for (row=0;row<maxy-3;row++)
            {
                printcard(win,row,col*CARD_WIDTH, CARDSPACE,g);
                printcard(win,row,col*CARD_WIDTH+2, CARDSPACE,g);
            }
            if (col==g->num_cols-1)
                (void) mvwvline(win,1,maxx-1,ACS_VLINE,maxy-2);

            if (g->col_size[col]==NOCARD)
                /* No cards to print */
                continue;
            else if (g->col_size[col]==CARDSEPARATOR)
            {
                (void) mvwvline(win,0,col*CARD_WIDTH+1,ACS_URCORNER,1);
                (void) mvwvline(win,1,col*CARD_WIDTH+1,ACS_VLINE,maxy-2);
                (void) mvwvline(win,maxy-1,col*CARD_WIDTH+1,ACS_LRCORNER,1);
                wattron(win,A_REVERSE | COLOR_PAIR(SPADES_COLOR));
                (void) mvwvline(win,0,col*CARD_WIDTH+2,' ',maxy);
                (void) mvwvline(win,0,col*CARD_WIDTH+3,' ',maxy);
                (void) mvwvline(win,0,col*CARD_WIDTH+4,' ',maxy);
                wattrset(win,A_NORMAL);
                (void) mvwvline(win,0,col*CARD_WIDTH+5,ACS_ULCORNER,1);
                (void) mvwvline(win,1,col*CARD_WIDTH+5,ACS_VLINE,maxy-2);
                (void) mvwvline(win,maxy-1,col*CARD_WIDTH+5,ACS_LLCORNER,1);
                continue;
            }
            else if (g->col_size[col]>=maxy-3)
                /* This is if sequences need to be collapsed */
            {
                num_rows=0;
                seq=0;
                temp_card=g->cols[col][0];
                for (row=1;row<=g->col_size[col];row++)
                {
                    if (temp_card > CARDBACK
                            && (g->cols[col][row]/SUIT_LENGTH == 
                                temp_card/SUIT_LENGTH)
                            && (g->cols[col][row]%SUIT_LENGTH == 
                                (temp_card%SUIT_LENGTH)-1))
                    {
                        if (seq==0)
                            temp_col[num_rows++]=temp_card;
                        else if (seq==1)
                            temp_col[num_rows++]=
                                ((temp_card/SUIT_LENGTH)%2)?CARDSEQB:CARDSEQR;
                        seq++;
                    }
                    else
                    {
                        temp_col[num_rows++]=temp_card;
                        seq=0;
                    }
                    temp_card=g->cols[col][row];
                }
                temp_col[num_rows]=temp_card;
                /*
                if (num_rows>=maxy-3)
                    show_error("Too many cards in column.",g->input);
                    */
            }
            else
                /* Don't need to collapse, just copy */
            {
                for (row=0;row<=g->col_size[col];row++)
                    temp_col[row]=g->cols[col][row];
                num_rows=g->col_size[col];
            }

            /* Number of cols to be next to each other */
            sq_rows=2*(((num_rows+1)*2-(maxy-3))/2)+1;
            if (sq_rows<1) sq_rows=1;
            else if (sq_rows>num_rows+1) sq_rows=num_rows+1;

            if (sq_rows > maxy-3)
                in_col=(maxy-3);
            else
                in_col = sq_rows;

            /* Print cards that don't fit in row just to the right of it */
            for (row=in_col;row<sq_rows;row++)
                printcard(win,2*in_col-row-1,col*CARD_WIDTH+2,temp_col[row],g);

            /* Print squashed rows. Always print at least one. */
            for (row=0; row<in_col; row++)
                printcard(win,row,col*CARD_WIDTH,temp_col[row],g);

            /* Print cards for rest of column, (+1 to add space) */
            for (row=sq_rows;row <= num_rows; row++)
                printcard(win,row*CARD_HEIGHT-sq_rows+1,col*CARD_WIDTH,
                        temp_col[row],g);
        }
    }
    else
        /* we are printing one of the side boards */
    {
        p = (win==g->free) ? &g->freepile[0] : &g->foundation[0];
        num_rows = (win==g->free) ? g->num_free : g->num_foun;
        for (row=0; row < num_rows; row++)
            printcard(win,row*CARD_HEIGHT,0,*p++,g);
    }
    wrefresh(win);
}

/* Prints text to the hint board */
void 
init_hints(WINDOW *win,GameInfo* g)
{
    char temp[14];
    int hint_h;

    wclear(win);
    wmove(win,0,0);
    
    /* hint_h==1 is when hints go on a single line. */
    hint_h = (g->hint_h == 1) ? 4 : g->hint_h;

#define P(x,y)    (void)wprintw(win,"%s%s", x,(y==1)?"  ":"\n")
    wattron(win,A_BOLD);
    if (hint_h > 0) P(names[g->game],g->hint_h);
    wattroff(win,A_BOLD);
    if (hint_h > 7)
    {
        P("nsd:move",g->hint_h);
        P(" n=[1-99]",g->hint_h);
        snprintf(temp,14," s=[a-%c,p,o]",'a'+g->num_cols-1);
        P(temp,g->hint_h);
        snprintf(temp,14," d=[a-%c,p,o]",'a'+g->num_cols-1);
        P(temp,g->hint_h);
    }
    if (hint_h > 1)  P("?:help",g->hint_h);
    if (hint_h > 3)  P("r:rules",g->hint_h);
    if (hint_h > 8)  P("u:undo",g->hint_h);
    if (hint_h > 9)  P("t:credits",g->hint_h);
    if (hint_h > 12) P("z:dump var",g->hint_h);
    if (hint_h > 11) P("w:warr/lic",g->hint_h);
    if (hint_h > 10) P("s:refr scrn",g->hint_h);
    if (hint_h > 2)  P("q:quit",g->hint_h);
#undef P
    wrefresh(win);

}

/* initialises board windows:
 * win: the board to initialise.
 * This routine clears the board and sets the background colour. Also
 * prints the letters that correspond to the move commands.
 */
void 
init_board(WINDOW *win,GameInfo* g)
{
    int i;

    wbkgdset(win, boardbkgd);
    wclear(win);
    box(win, 0, 0);
    if (win==g->main)
        for (i=0;i<g->num_cols;g->print_col[i++]=1)
            mvwaddch(win,1,i*CARD_WIDTH+3,i+'a');
    else if (win==g->free)
        mvwaddch(win,g->num_free*2+1,3,'o');
    else if (win==g->found)
        mvwaddch(win,g->num_foun*2+1,3,'p');
    draw_piles(win,g);
}

/* Completely (re)draws all the boards - called at the start of each game
 * after create_windows(). Also called when a refresh screen is required.
 */
void 
draw_screen(GameInfo* g)
{
    clearok(stdscr, TRUE);
    clear();
    attron(A_BOLD);
    (void)printw( "%s\n", PACKAGE_STRING);
    attroff(A_BOLD);
    refresh();
    xtermtitle(names[g->game]);

    init_hints(g->hint,g);
    if (g->num_free) init_board(g->free,g);
    if (g->num_foun) init_board(g->found,g);
    init_board(g->main,g);
}

/* This should be called for normal card moves. It will move a card from
 * one pile (src) to another pile (dst) and the call draw_piles() to 
 * refresh the piles that require it.
 * Normally number should be set to 1. If number > 1 a stack of piles will
 * be moved. This works from a main board column to main board column. It 
 * also works from main board column to foundation (only relevant for 
 * spider at the moment). The cards will be moved all at once and counted
 * as only one move (i.e. not good for Freecell).
 * For definitions of src and dst, see grab_input() above.
 * move_card() always does what its told. Validity of moves should be 
 * checked by game functions.
 */
void 
move_card(int src,int dst,int number,GameInfo* g)
{
    int card[number];
    int suit,rank;
    int undo_type=UNDO_NORMAL;
    int j;

    if (src < g->num_cols) 
    {
        for (j=0;j<number;j++)
        {
            card[j]=g->cols[src][g->col_size[src]];
            g->cols[src][g->col_size[src]--]=CARDSPACE;
        }
        /* Next bit turns over a face down card in a Klondike/Spider column */
        if (g->col_size[src]>=0 && g->cols[src][g->col_size[src]] < 0) 
        {
            g->cols[src][g->col_size[src]] += FACE_DOWN;
            undo_type=UNDO_FACE_DOWN;
        }
        g->print_col[src]=1;
        if (dst >= g->num_cols)
            draw_piles(g->main,g);
    }
    else if (src < g->num_cols+g->num_free)
    {
        card[0]=g->freepile[src-g->num_cols];
        g->freepile[src-g->num_cols]= NOCARD;
        draw_piles(g->free,g);
    }
    else
    {
        for (j=0;j<number;j++)
        {
            card[j]=g->foundation[src-g->num_cols-g->num_free];
            if (g->foun_size[src-g->num_cols-g->num_free]==SUIT_LENGTH)
                g->finished_foundations--;

            if (--g->foun_size[src-g->num_cols-g->num_free]==0)
            {
                g->foundation[src-g->num_cols-g->num_free]=NOCARD;
                if (j<number-1)
                {
                    endwin();
                    puts ("CPat Error: move_card(): tryed to move too many cards off a foundation pile.");
                    exit (1);
                }
            }
            else
            {
                suit=g->foundation[src-g->num_cols-g->num_free]/SUIT_LENGTH;
                g->foundation[src-g->num_cols-g->num_free] +=
                        (g->foun_dir==DESC)?1:-1;
                if (g->foundation[src-g->num_cols-g->num_free] < 0)
                    g->foundation[src-g->num_cols-g->num_free] += SUIT_LENGTH;
                if (g->foundation[src-g->num_cols-g->num_free]/SUIT_LENGTH!=suit)
                    g->foundation[src-g->num_cols-g->num_free] += 
                        SUIT_LENGTH*((g->foun_dir==DESC)?-1:1);
            }
        }
        draw_piles(g->found,g);
    }

    if (dst < g->num_cols)
    {
        for (j=number-1;j>=0;j--)
            g->cols[dst][++g->col_size[dst]]=card[j];
        g->print_col[dst]=1;
        draw_piles(g->main,g);
    }
    else if (dst < g->num_cols+g->num_free)
    {
        g->freepile[dst-g->num_cols]=card[0];
        draw_piles(g->free,g);
    }
    else
    {
        for (j=number-1;j>=0;j--)
        {
            /* finds what the rank would be if foundation started at ACE */
            /* i.e. the number of cards (minus 1) that would be on the
             * foundation if this card were on the top. */
            rank = (card[j]+SUIT_LENGTH-g->foun_start)%SUIT_LENGTH;
            /* suit can go from 0-7 */
            suit = (g->foun_size[card[j]/SUIT_LENGTH] > 
                    ((g->foun_dir==DESC)?SUIT_LENGTH-1-rank:rank)) 
                ? card[j] / SUIT_LENGTH + NUM_SUITS : card[j] / SUIT_LENGTH;
            g->foundation[suit]=card[j];
            if (++g->foun_size[suit]==SUIT_LENGTH)
                g->finished_foundations++;
        }
        draw_piles(g->found,g);
        dst=g->num_cols+g->num_free+suit;
    }
    g->undo = push_items(g->undo,src,dst,number,undo_type);
    g->moves++;
}

void
undo_move(GameInfo* g)
{
    int j;

    if (g->undo == NULL)
        show_error("No more undo's left.",g->input);
    else
    {
        if (g->allow_undo == 0 && g->undo->type != UNDO_NORMAL
                && g->undo->type != UNDO_FOUN_START)
        {
            show_error("Not allowed to undo that move.",g->input);
            return;
        }
        else if (g->undo->type == UNDO_IMPOSSIBLE)
        {
            show_error("Can't undo that move.",g->input);
            return;
        }
        else if (g->undo->type == UNDO_FOUN_START)
            g->foun_start = -1;
        else if (g->undo->type == UNDO_FACE_DOWN)
            g->cols[g->undo->src][g->col_size[g->undo->src]]-=FACE_DOWN;
        else if (g->undo->type == UNDO_DISCARD)
            g->deck[g->num_packs*PACK_SIZE-(++g->face_up)] = 
                (g->undo->dst < g->num_cols)
                    ? g->cols[g->undo->dst][g->col_size[g->undo->dst]]
                    : g->foundation[g->undo->dst-(g->num_cols+g->num_free)];
        else if (g->undo->type == UNDO_DEAL)
        {
            for (j=g->undo->number;j>0;j--)
            {
                g->deck[g->face_down++] =
                    g->cols[g->undo->dst][g->col_size[g->undo->dst]--];
                g->freepile[0]=CARDBACK;
                g->print_col[g->undo->dst]=1;
                g->undo=pop_items(g->undo);
            }
            g->deals--;
            draw_piles(g->free,g);
            draw_piles(g->main,g);
            return;
        }
        else if (g->undo->type == UNDO_RESERVE)
        {
            g->cols[g->undo->src][++g->col_size[g->undo->src]] = 
                g->cols[g->undo->dst][g->col_size[g->undo->dst]--];
            draw_piles(g->free,g);
            g->undo = pop_items(g->undo);
            undo_move(g);
            return;
        }
        else if (g->undo->type == UNDO_ROLLCARDS)
        {
            if (g->undo->number==0)
            {
                for (j=0;j<g->face_down;j++)
                    g->deck[g->num_packs*PACK_SIZE-j]=g->deck[g->face_down-j];
                g->face_up=g->face_down;
                g->face_down=0;
                g->freepile[0]=CARDSPACE;
                g->freepile[1]=g->deck[g->face_down];
                g->deals--;
            }
            else
            {
                for (j=0;j<g->undo->number;j++) 
                {
                    /* shift card to end of array and adjust counters */
                    g->deck[g->face_down++] = 
                        g->deck[g->num_packs*PACK_SIZE-(g->face_up--)];
                }
                /* Transfer card value to freepile[1] so it can be seen */
                if (g->face_up==0)
                    g->freepile[1]=NOCARD;
                else
                    g->freepile[1]=g->deck[g->num_packs*PACK_SIZE-g->face_up];
                g->freepile[0]=CARDBACK;
            }
            draw_piles(g->free,g);

            g->undo = pop_items(g->undo);
            return;
        }
        move_card(g->undo->dst,g->undo->src,g->undo->number,g);
        /* Two pops: one to remove what the above move_card adds!! */
        g->undo = pop_items(g->undo);
        g->undo = pop_items(g->undo);
        /* subtract two: one to remove what the above move_card adds */
        g->moves-=2;
    }
}

/* Reads move command and sets the source and destination piles.
 * Integers pointed to by src, dst and number will contain:
 * src, dst: integer corresponding to piles:
 *      0 to num_cols-1 = main board columns
 *      num_cols to num_cols+num_free = piles on right hand board
 *      num_cols+num_free to num_cols+num_free+num_foun = foundation piles
 * number: number of cards to be moved
 * If src=NOCARD, no game move was made (i.e. it was a bad command or 
 * some other command: help, license, refresh, credits
 * Only exception is the returned integer: if quit is chosen, the function
 * returns 1, otherwise it returns 0.
 */
int 
grab_input(GameInfo* g, int* src, int* dst, int* number)
{
    int inp;
    int *p;

    *number=0;
    *src=NOCARD;
    *dst=NOCARD;

    mvwprintw(g->input,0,0,"Move:     ");
    wmove(g->input,0,6);
    while (*dst==NOCARD)
    {
        wrefresh(g->input);
        inp = getch();

        switch (inp)
        {
            case KEY_ENTER:
            case 13:
                return(0);
            case KEY_BACKSPACE:
            case 127:
                *number=0;
                *src=NOCARD;
                mvwprintw(g->input,0,0,"Move:     ");
                wmove(g->input,0,6);
                break;
            case 'u':
                *src=NOCARD;
                if (*number==0) *number=1;
                if (g->game==STRATEGY) *number=1;
                while ((*number)--) undo_move(g);
                return(0);
            case 's':
                *src=NOCARD;
                kill_windows(g);
                if (create_windows(g)) return 1;
                draw_screen(g);
                return(0);
            case 'w':
                *src=NOCARD;
                kill_windows(g);
                license();      /* Display license */
                if (create_windows(g)) return 1;
                draw_screen(g);
                return(0);
            case 't':
                *src=NOCARD;
                kill_windows(g);
                credits();      /* Display credits */
                if (create_windows(g)) return 1;
                draw_screen(g);
                return(0);
            case 'r':
                *src=NOCARD;
                kill_windows(g);
                rules(g);       /* Display games rules */
                if (create_windows(g)) return 1;
                draw_screen(g);
                return(0);
            case '?':
                *src=NOCARD;
                kill_windows(g);
                help(); /* Display general help */
                if (create_windows(g)) return 1;
                draw_screen(g);
                return(0);
            case 'z':
                *src=NOCARD;
                if (g->debug)
                {
                    kill_windows(g);
                    dump_vars(g);       
                    if (create_windows(g)) return 1;
                    draw_screen(g);
                }
                return(0);
//          case 'x':
//              die(0);
            case 'q':
                *src=NOCARD;
                return(1);
            case 'P':
                *src=*dst=g->num_cols+g->num_free;
                *number=g->num_packs*PACK_SIZE;
                break;
            default:
                p = (*src==NOCARD) ? src : dst;
                if (inp>='0'&&inp<='9')
                    *number= (*number) ? *number*10+inp-'0' : inp-'0';
                else if (inp=='o')
                    *p = g->num_cols;
                else if (inp=='p')
                    *p = g->num_cols+g->num_free;
                else if (inp>='a'&&inp<'a'+g->num_cols)
                    *p = inp-'a';
                else
                    break;
                waddch(g->input,inp);
        }
    }
    /* This clears the previous error */
    show_error(" ",g->input);
    if (*src==*dst) *dst=NOCARD;
    return(0);
}

/* Displays a message after a move. Usually an error for a bad move, but
 * also used for info (e.g. number of deals left). The message should
 * disappear after the next move (cleared by grab_input()).
 */
void 
show_error(char* error_str,WINDOW* win)
{
    int maxy,maxx;
    int startx=0,starty=2;
    char *start_p,*end_p,*final_p;
    char dest[100]; /* this will contain each line before we print it */

    werase(win);
    getmaxyx(win,maxy,maxx);
    if (maxy <= 2)
    {
        starty = 0;
        startx = maxx-strlen(error_str);
        if (startx < 10)
        {
            startx = 10;
            maxx -= startx;
        }
    }

    /* points to last character of text */
    final_p=&error_str[0]+strlen(error_str);

    /* will point to last character of current line */
    end_p = &error_str[0];
    /* will point to first character of current line */
    start_p = &error_str[0];
    /* do this for all of text */
    while (end_p < final_p)
    {
        end_p = start_p+maxx;

        if (start_p + maxx >= final_p)
            end_p=final_p;
        else
            /* search for the space closest to the end of current line*/
            while (*end_p != ' ' && end_p>start_p) end_p--;

        if (end_p==start_p)
        {
            /* This means there's no space on the line, we will
             * break the long line up */
            end_p = start_p + maxx;
            (void) strncpy(dest,start_p,(int)(end_p-start_p));
            dest[(int)(end_p-start_p)]='\0';
            start_p=end_p;
        }
        else
        {
            (void) strncpy(dest,start_p,(int)(end_p-start_p));
            dest[(int)(end_p-start_p)]='\0';
            start_p=end_p+1;
        }
        wmove(win,starty++,startx);
        wprintw(win,"%s",dest);
    }
}

void 
dump_vars(GameInfo* g)
{
    char string[50*200]; /* this will contain the text*/
    char temp[200];
    int i;

    snprintf(string,200,"game: %d - %s\n",g->game,names[g->game]);
    snprintf(temp,200,"variation: %d - %s\n",g->variation,variations[g->game][g->variation]);
    strncat(string,temp,200);
    snprintf(temp,200,"hs.filename: %s\n",hs.filename);
    strncat(string,temp,200);
    snprintf(temp,200,"hs.available: %d\n",hs.available);
    strncat(string,temp,200);
    snprintf(temp,200,"hs.total_games: %d\n",hs.total_games[g->game][g->variation]);
    strncat(string,temp,200);
    snprintf(temp,200,"hs.finished_games: %d\n",
            hs.finished_games[g->game][g->variation]);
    strncat(string,temp,200);
    snprintf(temp,200,"hs.lowest_moves: %d\n",
            hs.lowest_moves[g->game][g->variation]);
    strncat(string,temp,200);
    snprintf(temp,200,"hs.lowest_deals: %d\n",
            hs.lowest_deals[g->game][g->variation]);
    strncat(string,temp,200);
    snprintf(temp,200,"hs.date_first_game: %ld\n",
            hs.date_first_game[g->game][g->variation]);
    strncat(string,temp,200);
    snprintf(temp,200,"hs.date_best_game: %ld\n",
            hs.date_best_game[g->game][g->variation]);
    strncat(string,temp,200);
    snprintf(temp,200,"hs.seed: %d\n",
            hs.seed[g->game][g->variation]);
    strncat(string,temp,200);
    snprintf(temp,200,"col_size: ");
    strncat(string,temp,200);
    for (i=0;i<g->num_cols;i++)
    {
        snprintf(temp,200,"%d ",g->col_size[i]);
        strncat(string,temp,200);
    }
    snprintf(temp,200,"\nface_up: %d  face_down: %d\n",g->face_up,g->face_down);
    strncat(string,temp,200);
    snprintf(temp,200,"foun_size: ");
    strncat(string,temp,200);
    for (i=0;i<g->num_foun;i++)
    {
        snprintf(temp,200,"%d ",g->foun_size[i]);
        strncat(string,temp,200);
    }
    snprintf(temp,200,"\nfinished_foundations: %d\n",g->finished_foundations);
    strncat(string,temp,200);
    snprintf(temp,200,"deals: %d  moves: %d\n",g->deals,g->moves);
    strncat(string,temp,200);
    snprintf(temp,200,"seed: %d\n",g->seed);
    strncat(string,temp,200);
    if (g->undo == NULL) 
    {
        snprintf(temp,200,"undo: There are no undo's yet\n");
        strncat(string,temp,200);
    }
    else 
    {
        snprintf(temp,200,"undo - number: %d\n",g->undo->number);
        strncat(string,temp,200);
        snprintf(temp,200,"undo - src: %d\n",g->undo->src);
        strncat(string,temp,200);
        snprintf(temp,200,"undo - dst: %d\n",g->undo->dst);
        strncat(string,temp,200);
        snprintf(temp,200,"undo - type: %d\n",g->undo->type);
        strncat(string,temp,200);
    }

    pager("Debug Output",string,0,NULL);
}
/* common.c ends here */
