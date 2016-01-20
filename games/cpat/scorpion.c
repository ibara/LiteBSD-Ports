/*****************************************************************************
 *                                                                           *
 * spider.c - functions for initialising and playing spider.                 *
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
    g->num_foun=0;
    g->num_free=1;
    g->num_packs=1;
    g->num_deals=1;
    g->foun_dir=DESC;
    g->foun_start=ACE;

    init_deck(g);

    /* Set initial column lengths (54 cards are dealt)*/
    for (j=0; j < g->num_cols; j++)
        g->col_size[j] = 6;

    deal_deck(FACE_UP,g);

    /* Turn face down the first 3 cards of the first 4 columns */
    for (i=0;i<4;i++)
        for (j=0;j<3;j++)
            g->cols[i][j] -= FACE_DOWN;

    /* Initialise freepile and foundation */
    g->freepile[0]=CARDBACK;    /* This is the back of the deck to deal */

    /* No moves yet!! */
    g->moves=0;
    g->deals=0; /* number of deals left */
    g->finished_foundations=-1;
}


static void play(GameInfo* g)
{
    int i,j,number,card;
    int number_found; /* number of cards moved to foundation automatically */
    int src,dst;
    char temp_str[20];

    while (g->finished_foundations < g->num_foun)
    {

        number_found=0;

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
            /* Here we search to see if there are any cards that can
             * be moved to dst and if so set number so that the stack
             * is moved */
            if (number==0)
                number = find_move(src,dst,DESC,IN_SUIT,NO_WRAP,g);
            if (number==0)
                continue;

            /* Check if enough cards in stack */
            if (check_sequence(number,src,ANY_ORDER,ANY_SUIT,NO_WRAP,g))
                continue;
            card=g->cols[src][g->col_size[src]-number+1];
        }
        /* Deal out remaining 3 cards */
        else if (src<g->num_cols+g->num_free)
        {
            if (g->deals==g->num_deals)
            {
                show_error("No deals left.",g->input);
                continue;
            }
            /* Now deal the cards */
            for (j=0;j<3;j++)
            {
                g->cols[j][++g->col_size[j]]=g->deck[--g->face_down];
                g->print_col[j]=1;
                g->undo = push_items(g->undo,g->num_cols,j,j+1,UNDO_DEAL);
            }
            if (++g->deals==g->num_deals)
                g->freepile[0]=CARDSPACE;
            draw_piles(g->free,g);
            draw_piles(g->main,g);
            snprintf(temp_str,20,"%d deals left.",g->num_deals-g->deals);
            show_error(temp_str,g->input);
            continue;
        }
        /* Automatically move cards to the foundations */
        else if (src>=g->num_cols+g->num_free)
            continue;

        /* Next check if card can move there */
        if (dst==NOCARD) 
            continue;
        else if (dst < g->num_cols) 
        {
            if (g->col_size[dst] >= 0)
            {
                if (check_move(dst,card,DESC,IN_SUIT,NO_WRAP,g))
                    continue;
            }
            /* can only move cards to vacant columns if they are Kings */
            else if (card % SUIT_LENGTH != KING)
            {
                show_error("Only Kings to vacant columns.",g->input);
                continue;
            }
        }
        else if (dst < g->num_cols+g->num_free)
        {
            show_error("You can't move cards there.",g->input);
            continue;
        }
        else if (dst>=g->num_cols+g->num_free)
            continue;

        /* now do move(s) */
        move_card(src,dst,number,g);

        /* Check to see if sequences complete */
        number_found=0;
        for (i=0;i<g->num_cols;i++)
        {
            if (!check_sequence(SUIT_LENGTH,i,ASC,IN_SUIT,NO_WRAP,g))
                number_found++;
        }
        /* Removes error produced by check_sequence above */
        show_error(" ",g->input);
        if (number_found==4)
            g->finished_foundations=0;
    }
}

void scorpion(GameInfo* g)
{
    int carry_on=2;
    char game_str[60];

    while (carry_on)
    {
        if (carry_on==2) init_vars(g);
        if (create_windows(g)) return;
        draw_screen(g);
        play(g);

        snprintf(game_str,60,"You did %d card moves and had %d %s left.",
                g->moves,g->num_deals-g->deals,(g->num_deals-g->deals)==1?"deal":"deals");
        carry_on=game_finished(g,game_str);
    }
}
/* scorpion.c ends here */
