/*****************************************************************************
 *                                                                           *
 * fortythieves.c - functions for initialising and playing fortythieves.     *
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

static void init_vars(GameInfo* g);
static void play(GameInfo* g);

/* Initialise variables for current game */
static void
init_vars(GameInfo* g)
{
    int i,j,temp,kings;

    /* game specific variables */
    g->num_cols=13;
    g->num_foun=4;
    g->num_free=0;
    g->num_packs=1;
    g->turn_no=1;
    g->foun_dir=ASC;
    g->foun_start=ACE;

    init_deck(g);

    /* Set initial column lengths */
    for (i=0; i < g->num_cols; i++)
        g->col_size[i] = 3;

    /* create and shuffle deck */
    deal_deck(FACE_UP,g);

    /* move kings to bottom of columns */
    for (i=0;i<g->num_cols;i++)
    {
        kings=0;
        for (j=0;j<=g->col_size[i];j++)
        {
            if (g->cols[i][j]%SUIT_LENGTH == KING)
            {
                if (j>kings)
                {
                    temp=g->cols[i][kings];
                    g->cols[i][kings]=g->cols[i][j];
                    g->cols[i][j]=temp;
                }
                kings++;
            }
        }
    }

    /* Initialise freepile and foundation */
    for (i=0; i < g->num_foun; i++)
    {
        g->foundation[i]=NOCARD;
        g->foun_size[i]=0;
    }

    /* No moves yet!! */
    g->moves=0;
    g->deals=0;
    g->finished_foundations=0;
}

/* Main game loop. Contains rules that describe what moves are allowed and
 * what to do with given move commands. */
static void 
play(GameInfo* g)
{
    int j,number;
    int card;
    int src,dst;

    while (g->finished_foundations < g->num_foun)
    {

        if (grab_input(g,&src, &dst, &number))
            break;  /* quit game */

        if (src<=NOCARD) 
            continue;
        else if (src < g->num_cols)
        {
            /* Can't move multiple cards to normal columns*/
            /* and we want to move at least one card */
            if (dst<g->num_cols || number==0) number=1; 

            /* Check if stack to move is big enough and in suit order */
            if (check_sequence(number,src,ASC,IN_SUIT,NO_WRAP,g))
                    continue;

            card=g->cols[src][g->col_size[src]];
        }
        else if (src < g->num_cols+g->num_free)
            continue;
        /* Move cards automatically to the foundations */
        else if (src>=g->num_cols+g->num_free)
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
        else if (dst < g->num_cols) 
        {
            /* Allowed to move any card to vacant pile */
            if (g->col_size[dst] == NOCARD)
            {
                show_error("Can't move cards to vacant spaces",g->input);
                continue;
            }
            else
            {
                if (check_move(dst,card,DESC,ANY_SUIT,NO_WRAP,g))
                    continue;
            }
        }
        /* Can't move card to waste */
        else if (dst<g->num_cols+g->num_free)
            continue;
        /* and for foundation */
        else if (dst >= g->num_cols+g->num_free)
        {
            if (check_move(dst,card,0,0,NO_WRAP,g))
                continue;
        }

        /* now do move(s) */
        for (j=0;j<number;j++)
        {
            if (j) nanosleep(&pauselength,&pauseleft);
            move_card(src,dst,1,g);
        }

    }
}

/* freecell wrapper functions */
void 
bakersdozen(GameInfo* g)
{
    int carry_on=2;
    char game_str[50];

    while (carry_on)
    {
        if (carry_on==2) init_vars(g);
        if (create_windows(g)) return;
        draw_screen(g);
        play(g);

        snprintf(game_str,50,"You did %d card moves.",g->moves);
        carry_on=game_finished(g,game_str);
    }
}
/* bakersdozen.c ends here */
