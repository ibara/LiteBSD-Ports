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

static int init_vars(GameInfo* g);
static void play(GameInfo* g);

/* Initialise variables for current game */
static int
init_vars(GameInfo* g)
{
    int i;
    int num_items[1] = {3};
    char title[40];
    char *queries[1] = {"Choose normal or easy:"};
    char *items[3] = {
        "normal",
        "easy (sequence moves allowed)",
        "return to main menu"
    };

    snprintf(title,40,"Welcome to %s",names[g->game]);

    g->variation=menu(title,queries,1,items,num_items,items,0);
    switch(g->variation)
    {
        case 0:
            g->seq_moves=0;
            break;
        case 1:
            g->seq_moves=1;
            break;
        case 2:
            return 1;
    }

    /* game specific variables */
    g->num_cols=10;
    g->num_foun=8;
    g->num_free=2;
    g->num_packs=2;
    g->turn_no=1;
    g->num_deals=1;
    g->foun_dir=ASC;
    g->foun_start=ACE;

    init_deck(g);

    /* Set initial column lengths */
    for (i=0; i < g->num_cols; i++)
        g->col_size[i] = 3;

    /* create and shuffle deck */
    deal_deck(FACE_UP,g);

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

        /* This is for if we moved a card off the waste, we need
         * to turn over the card underneath */
        turnover_waste(g);

        if (grab_input(g,&src, &dst, &number))
            break;  /* quit game */

        if (src<=NOCARD)
            continue;
        else if (src < g->num_cols)
        {
            /* We want to move at least one card */
            if (number==0) number=1;
            /* Can't move multiple cards to normal columns*/
            if (g->seq_moves == 0 && dst < g->num_cols) number=1;

            /* Check if stack to move is big enough and in suit order */
            if (check_sequence(number,src,ASC,IN_SUIT,NO_WRAP,g))
                continue;

            if (dst < g->num_cols)
                card=g->cols[src][g->col_size[src]-number+1];
            else
                card=g->cols[src][g->col_size[src]];
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
                src=g->num_cols+1;
                if (g->freepile[1] == NOCARD)
                {
                    show_error("No cards on that pile.",g->input);
                    continue;
                }
                card=g->freepile[1];
            }
            number=1; /* don't interfere with multi moves */
        }
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
            if (g->col_size[dst] >= 0)
                if (check_move(dst,card,DESC,IN_SUIT,NO_WRAP,g))
                    continue;
        }
        /* Can't move card to waste */
        else if (dst<g->num_cols+g->num_free)
        {
            show_error("You can't move cards there.",g->input);
            continue;
        }
        /* and for foundation */
        else if (dst >= g->num_cols+g->num_free)
        {
            if (check_move(dst,card,0,0,NO_WRAP,g))
                continue;
        }

        /* now do move(s) */
        if (dst < g->num_cols+g->num_free)
            move_card(src,dst,number,g);
        else
        {
            for (j=0;j<number;j++)
            {
                if (j) nanosleep(&pauselength,&pauseleft);
                move_card(src,dst,1,g);
            }
        }
    }
}

/* freecell wrapper functions */
void
fortythieves(GameInfo* g)
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
/* fortythieves.c ends here */
