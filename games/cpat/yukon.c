/*****************************************************************************
 *                                                                           *
 * klondike.c - functions for initialising and playing klondike.             *
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

static void init_vars(GameInfo* g)
{
    int i,j;

    g->num_cols=7;
    g->num_foun=4;
    g->num_free=0;
    g->num_packs=1;
    g->foun_dir=ASC;
    g->foun_start=ACE;

    /* create and shuffle the deck */
    init_deck(g);

    /* Set initial column lengths */
    for (j=0; j < g->num_cols; j++)
        g->col_size[j] = g->num_cols-j-1;

    deal_deck(FACE_DOWN,g);

    /* Deal out rest of cards to first 6 columns */
    for (j=0; j < g->num_cols-1; j++)
        for (i=0;i<4;i++)
            g->cols[j][++g->col_size[j]] = g->deck[--g->face_down];

    /* Initialise freepile and foundation */
    for (i=0; i < g->num_foun; i++)
    {
        g->foundation[i]=NOCARD;
        g->foun_size[i]=0;
    }
        
    /* No moves yet!! */
    g->moves=0;
    g->finished_foundations=0;
}

static void play(GameInfo* g)
{
    int number;
    int card;
    int src,dst;

    while (g->finished_foundations < g->num_foun)
    {
        if (grab_input(g,&src, &dst, &number))
            break;

        if (src==NOCARD) 
            continue;
        else if (src < g->num_cols)
        {
            /* Can't move multiple cards to foundation*/
            /* and we want to move at least one card */
            if (dst>=g->num_cols || number==0) number=1; 

            if (check_sequence(number,src,ANY_ORDER,ANY_SUIT,NO_WRAP,g))
                continue;
            card=g->cols[src][g->col_size[src]-number+1];
        }
        else if (src < g->num_cols+g->num_free)
        { 
            show_error("There is no other board.",g->input);
            continue;
        }
        /* Automatically move cards to the foundations */
        else if (src==g->num_cols+g->num_free)
        {
            if (dst!=NOCARD)
                show_error("Can't move cards off the foundations.",g->input);
            else
                foundation_automove(number==0?1:number,g);
            continue;
        }

        if (dst==NOCARD) 
            continue;
        /* Next check if card can move there */
        if (dst < g->num_cols) 
        {
            if (g->col_size[dst] >= 0)
            {
                if (check_move(dst,card,DESC,ALT_COL,NO_WRAP,g))
                    continue;
            }
            /* can only move cards to vacant columns if they are Kings */
            else if (card % SUIT_LENGTH != KING)
            {
                show_error("Only Kings to vacant columns.",g->input);
                continue;
            }
        }
        else if (dst<g->num_cols+g->num_free)
        {
            show_error("There is no other board.",g->input);
            continue;
        }
        else if (dst >= g->num_cols+g->num_free)
            if (check_move(dst,card,0,0,NO_WRAP,g))
                continue;

        /* now do move(s) */
        move_card(src,dst,number,g);

    }
}

void yukon(GameInfo* g)
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
/* klondike.c ends here */
