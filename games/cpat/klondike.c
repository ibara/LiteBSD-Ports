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

static int init_vars(GameInfo* g);
static void play(GameInfo* g);

static int init_vars(GameInfo* g)
{
    int i,j;
    int num_items[1] = {3};
    char title[40];
    char *queries[1] = {"Choose normal or easy:"};
    char *items[3] = {
        "normal (cards dealt 3 at a time)",
        "easy (cards dealt 1 at a time)",
        "return to main menu"
    };

    snprintf(title,40,"Welcome to %s",names[g->game]);

    g->variation=menu(title,queries,1,items,num_items,items,0);
    switch(g->variation) 
    {
        case 0: 
            g->turn_no=3;
            break;
        case 1:
            g->turn_no=1;
            break;
        case 2:
            return 1;
    }

    g->num_cols=7;
    g->num_foun=4;
    g->num_free=2;
    g->num_packs=1;
    g->num_deals=1000;
    g->foun_dir=ASC;
    g->foun_start=ACE;

    /* create and shuffle the deck */
    init_deck(g);

    /* Set initial column lengths */
    for (j=0; j < g->num_cols; j++)
        g->col_size[j] = g->num_cols-j-1;

    deal_deck(FACE_DOWN,g);

    /* Initialise freepile and foundation */
    for (i=0; i < g->num_foun; i++)
    {
        g->foundation[i]=NOCARD;
        g->foun_size[i]=0;
    }
        
    g->freepile[0]=CARDBACK;    /* This is the back of the deck to deal */
    g->freepile[1]=NOCARD;      /* No card has been delt yet */

    /* No moves yet!! */
    g->moves=0;
    g->deals=0;
    g->finished_foundations=0;

    return 0;
}

static void play(GameInfo* g)
{
    int number,card;
    int src,dst;

    while (g->finished_foundations < g->num_foun)
    {

        turnover_waste(g);

        if (grab_input(g,&src, &dst, &number))
            break;

        if (src==NOCARD) 
            continue;
        else if (src < g->num_cols)
        {
            if (dst==NOCARD)
            {
                show_error("Excuse me?",g->input);
                continue;
            }
            /* Can't move multiple cards to foundation*/
            if (dst>=g->num_cols) number=1; 

            /* If no number is given we search the column for any possible
             * moves and set number accordingly */
            if (number==0)
                number = find_move(src,dst,DESC,ALT_COL,NO_WRAP,g);
            if (number==0)
                continue;

            if (check_sequence(number,src,ASC,ALT_COL,NO_WRAP,g))
                continue;
            card=g->cols[src][g->col_size[src]-number+1];
        }
        else if (src < g->num_cols+g->num_free)
        { 
            /* this is turning deck cards over */
            if (dst==g->num_cols || dst==NOCARD)
            {
                roll_deckcards(g);
                continue;
            }
            /* this is for moving a card off the face up deck */
            else
            {
                /* Can only move one card from deck */
                number=1; 

                src=g->num_cols+1;
                if (g->freepile[src-g->num_cols] == NOCARD)
                {
                    show_error("No cards on the waste.",g->input);
                    continue;
                }
                card=g->freepile[src-g->num_cols];
            }
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
            show_error("You can't move cards there.",g->input);
            continue;
        }
        else if (dst >= g->num_cols+g->num_free)
            if (check_move(dst,card,0,0,NO_WRAP,g))
                continue;

        /* now do move(s) */
        move_card(src,dst,number,g);

    }
}

void klondike(GameInfo* g)
{
    int carry_on=2;
    char game_str[60];
    
    while (carry_on)
    {
        if (carry_on==2)
            if (init_vars(g)) return;
        if (create_windows(g)) return;
        draw_screen(g);
        play(g);

        snprintf(game_str,60,"You did %d card moves and %d redeals.",
                g->moves,g->deals);
        carry_on=game_finished(g,game_str);
    }
}
/* klondike.c ends here */
