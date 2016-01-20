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

    g->num_cols=10;
    g->num_foun=8;
    g->num_free=1;
    g->num_packs=2;
    g->foun_dir=DESC;
    g->foun_start=ACE;
    g->num_deals=5;

    init_deck(g);

    /* Set initial column lengths (54 cards are dealt)*/
    for (j=0; j < g->num_cols; j++)
        g->col_size[j] = 54/g->num_cols - (1&&j/(54%g->num_cols));

    deal_deck(FACE_DOWN,g);

    /* Initialise freepile and foundation */
    for (i=0; i < g->num_foun; i++)
    {
        g->foundation[i]=NOCARD;
        g->foun_size[i]=0;
    }
        
    g->freepile[0]=CARDBACK;    /* This is the back of the deck to deal */

    /* No moves yet!! */
    g->moves=0;
    g->deals=0; /* number of deals left */
    g->finished_foundations=0;
}


static void play(GameInfo* g)
{
    int j,number,card;
    int failed=0;
    int number_found; /* number of cards moved to foundation automatically */
    int src,dst;
    char temp_str[20];

    while (g->finished_foundations < g->num_foun)
    {

        failed=0;
        number_found=0;

        if (grab_input(g,&src, &dst, &number))
            break;

        if (src==NOCARD)
            continue;
        else if (src < g->num_cols)
        {
            /* Can't move single cards to foundation and */
            /* setting number to 13 essentially checks for foundation move */
            if (dst>=g->num_cols+g->num_free) 
                number = SUIT_LENGTH; 
            /* Need to move at least one card */
            else if (number==0) 
                number=1;

            /* Check if stack to move is one suit and in order */
            if (check_sequence(number,src,ASC,IN_SUIT,NO_WRAP,g))
                continue;
            card=g->cols[src][g->col_size[src]-number+1];
        }
        /* Deal out 10 new cards */
        else if (src<g->num_cols+g->num_free)
        {
            if (g->deals==g->num_deals)
            {
                show_error("No deals left.",g->input);
                continue;
            }
            /* Check for any empty columns */
            for (j=0;j<g->num_cols;j++)
            {
                if (g->col_size[j]==NOCARD)
                {
                    show_error("Can't deal if empty columns.",g->input);
                    failed=1;
                    break;
                }
            }
            if (failed) continue;
            /* Now deal the cards */
            for (j=0;j<g->num_cols;j++)
            {
                g->cols[j][++g->col_size[j]]=g->deck[--g->face_down];
                g->print_col[j]=1;
                g->undo = push_items(g->undo,g->num_cols,j,j+1,UNDO_DEAL);
            }
            if (++g->deals==g->num_deals)
                g->freepile[0]=CARDSPACE;
            draw_piles(g->free,g);
            draw_piles(g->main,g);
            snprintf(temp_str,20,"%d %s left.",
                    g->num_deals-g->deals,
                    (g->num_deals-g->deals)==1?"deal":"deals");
            show_error(temp_str,g->input);
            continue;
        }
        /* Automatically move cards to the foundations */
        else if (src>=g->num_cols+g->num_free)
        {
            if (dst!=NOCARD)
            {
                show_error("Can't move cards off the foundations.",g->input);
                continue;
            }
            if (number==0)
                number = 1;
            number_found=0;
            /* Loop through columns */
            for (j=0;j<g->num_cols;j++)
            {
                if (check_sequence(SUIT_LENGTH,j,ASC,IN_SUIT,NO_WRAP,g))
                {
                    show_error(" ",g->input); // remove non-errors
                    continue;
                }
                if (number_found++)
                    nanosleep(&pauselength,&pauseleft);
                /* move 13 cards to foundation */
                move_card(j,g->num_cols+g->num_free,SUIT_LENGTH,g);
                j=-1; /* start at first column again */
                if (number_found==number)
                    break;
            }
            if (number_found==0)
                show_error("No piles to move to foundations.",g->input);
            continue;
        }

        /* Next check if card can move there */
        if (dst==NOCARD) 
            continue;
        else if (dst < g->num_cols) 
        {
            if (g->col_size[dst] >= 0)
                if (check_move(dst,card,DESC,ANY_SUIT,NO_WRAP,g))
                    continue;
        }
        else if (dst < g->num_cols+g->num_free)
        {
            show_error("You can't move cards there.",g->input);
            continue;
        }

        /* now do move(s) */
        move_card(src,dst,number,g);
    }
}

void spider(GameInfo* g)
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
/* spider.c ends here */
