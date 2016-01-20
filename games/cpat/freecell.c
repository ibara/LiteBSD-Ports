/*****************************************************************************
 *                                                                           *
 * freecell.c - functions for initialising and playing freecell.             *
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

static int init_vars(GameInfo* g);
static void play(GameInfo* g);

/* Initialise variables for current game */
static int
init_vars(GameInfo* g)
{
    int i,j;

    int num_items[1] = {4};
    char title[40];
    char *queries[1] = {"Choose hard, normal or easy:"};
    char *items[4] = {
        "hard (three freecells)",
        "normal (four freecells)",
        "easy (five freecells)",
        "return to main menu"
    };

    snprintf(title,40,"Welcome to %s",names[g->game]);

    g->variation=menu(title,queries,1,items,num_items,items,0);

    if (g->variation==3) return 1;

    g->num_free=g->variation+3;
    g->num_cols=8;
    g->num_foun=4;
    g->num_packs=1;
    g->foun_dir=ASC;
    g->foun_start=ACE;

    /* create and shuffle deck */
    init_deck(g);

    /* Set initial column lengths */
    for (j=0; j < g->num_cols; j++)
        g->col_size[j] = PACK_SIZE/g->num_cols - (1&&j/(PACK_SIZE%g->num_cols));

    /* Deal the pack into the columns */
    deal_deck(FACE_UP,g);

    /* Initialise freepile and foundation */
    for (i=0; i < g->num_foun; i++)
    {
        g->foundation[i]=NOCARD;
        g->foun_size[i]=0;
    }
    for (i=0; i < g->num_free; i++)
        g->freepile[i]=NOCARD;

    /* No moves yet!! */
    g->moves=0;
    g->finished_foundations=0;

    return 0;
}

/* Main game loop. Contains rules that describe what moves are allowed and
 * what to do with given move commands. */
static void 
play(GameInfo* g)
{
    int j,number;
    int card;
    int src,dst;
    int num_freecells;
    int freecells[g->num_cols+g->num_free];

    while (g->finished_foundations < g->num_foun)
    {

        if (grab_input(g,&src, &dst, &number))
            break;  /* quit game */

        if (src<=NOCARD) 
            continue;
        else if (src < g->num_cols)
        {
            /* Can't move multiple cards to foundation*/
            /* and we want to move at least one card */
            if (dst==g->num_cols+g->num_free || number==0) number=1; 

            /* Check if stack to move is rbrbr */
            if (dst < g->num_cols)
            {
                if (check_sequence(number,src,ASC,ALT_COL,NO_WRAP,g))
                    continue;
            }
            else
            {
                /* Check if enough cards in column. This is for
                 * moving multiple cards to Freecells.*/
                if (g->col_size[src] < number-1)
                {
                    show_error("Not enough cards on that pile.",g->input);
                    continue;
                }
            }
            card=g->cols[src][g->col_size[src]-number+1];
        }
        else if (src<g->num_cols+g->num_free)
        {
            if (number==0)
            {
                /* No number was chosen. If there is only one */
                /* card on the free piles, that is our src. */
                for (j=0;j<g->num_free;j++)
                {
                    if (g->freepile[j]!=NOCARD)
                    {
                        if (number==0) 
                            number=j+1;
                        else
                        {
                            show_error("Which card do you want to move",
                                    g->input);
                            number=-1;
                            break;      /* quit looking at freecells */
                        }
                    }
                }
                if (number==0)
                    show_error("No free cards to move.",g->input);
            }
            else if (g->freepile[number-1]==NOCARD) 
            {
                show_error("No card in that FreeCell.",g->input);
                number=0;
            }
            if (number>0)
                src=g->num_cols+number-1; /* -1 to translate 1-4 to 0-3 */
            else
                continue; /* Must have been an error in this section 
                           * goto next move */
            number=1; /* don't interfere with multi moves */
            card=g->freepile[src-g->num_cols];
        }
        /* Automatically moving cards to the foundations */
        else if (src==g->num_cols+g->num_free)
        {
            if (dst!=NOCARD)
                show_error("Can't move cards off the foundations.",g->input);
            else
                foundation_automove(number==0?1:number,g);
            continue;   /* Goto next move */
        }

        if (dst==NOCARD) 
            continue;
        /* Next check if card can move there */
        /* check for freecells below */
        else if (dst < g->num_cols && g->col_size[dst] >= 0) 
        {
            if (check_move(dst,card,DESC,ALT_COL,NO_WRAP,g))
                continue;
        }
        else if (dst >= g->num_cols+g->num_free)
        {
            if (check_move(dst,card,0,0,NO_WRAP,g))
                continue;
        }

        /* Check if enough free cells */
        if (number>1 || dst==g->num_cols)
        {
            num_freecells=0;
            /* First check for actual freecells */
            for (j=0;j < g->num_free; j++)
                if (g->freepile[j]==NOCARD)
                    freecells[num_freecells++]=j+g->num_cols;
            /* Now for empty columns, unless its the column */
            /* we're moving to.  */
            if (dst<g->num_cols) /* Only if moving to a column */
            {
                for (j=0;j < g->num_cols; j++)
                    if (g->col_size[j]==NOCARD && j!=dst)
                        freecells[num_freecells++]=j;
                num_freecells++; /*because actual card doesn't need
                                   a free cell */
            }
            if (num_freecells < number)
            {
                show_error("Not enough free cells.",g->input);
                continue;
            }
            if (dst==g->num_cols)
                dst=freecells[number-1];
        }

        /* now do move(s) */

        /* First move cards to free cells */
        for (j=0;j<number-1;j++)
        {
            move_card(src,freecells[j],1,g);
            nanosleep(&pauselength,&pauseleft);
        }
        /* Now move the base card */
        move_card(src,dst,1,g);
        /* Now move cards from free cells onto dst */
        /* unless we just moved cards to freecells */
        if (dst<g->num_cols)
        {
            for (j=number-2; j >= 0;j--)
            {
                nanosleep(&pauselength,&pauseleft);
                move_card(freecells[j],dst,1,g);
            }
        }
    }
}

/* freecell wrapper functions */
void 
freecell(GameInfo* g)
{
    int carry_on=2;
    char game_str[50];

    while (carry_on)
    {
        if (carry_on==2) 
            if (init_vars(g)) return;
        if (create_windows(g)) return;
        draw_screen(g);
        play(g);

        snprintf(game_str,50,"You did %d card moves.",g->moves);
        carry_on=game_finished(g,game_str);
    }
}
/* freecell.c ends here */
