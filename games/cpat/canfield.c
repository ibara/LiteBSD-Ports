/*****************************************************************************
 *                                                                           *
 * canfield.c - functions for initialising and playing canfield.             *
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
static void flip_reserve(GameInfo* g);
static void play(GameInfo* g);

static int init_vars(GameInfo* g)
{
    int i;

    g->num_cols=4;
    g->num_foun=4;
    g->num_free=4;
    g->num_packs=1;
    g->num_deals=1000;
    g->foun_dir=ASC;
    g->turn_no=3;

    /* create and shuffle the deck */
    init_deck(g);

    /* Set initial column lengths */
    for (i=0; i < g->num_cols; i++)
        g->col_size[i] = 0;

    /* Deal the pack into the columns */
    deal_deck(FACE_DOWN,g);

    /* Initialise freepile and foundation */
    for (i=0; i < g->num_foun; i++)
    {
        g->foundation[i]=NOCARD;
        g->foun_size[i]=0;
    }
    g->freepile[0]=CARDBACK;    /* This is the back of the deck to deal */
    g->freepile[1]=NOCARD;      /* No card has been delt yet */
    g->freepile[2]=CARDSPACE;   /* This is just a gap */
    g->freepile[3]=CARDRESERVE; /* This is the back of the reserve pile */

    /* Create 13 card reserve + 1 for the first foundation*/
    g->col_size[g->num_cols] = 13;
    for (i=0; i <= g->col_size[g->num_cols]; i++)
        g->cols[g->num_cols][i] = g->deck[--g->face_down];

    /* The first reserve will be foundation */
    g->foun_start = g->cols[g->num_cols][g->col_size[g->num_cols]--];
    g->foundation[g->foun_start/SUIT_LENGTH] = g->foun_start;
    g->foun_size[g->foun_start/SUIT_LENGTH]++;
    g->foun_start %= SUIT_LENGTH;

    /* No moves yet!! */
    g->moves=0;
    g->deals=0;
    g->finished_foundations=0;

    return 0;
}

static void flip_reserve(GameInfo* g)
{
    int i;

    for (i=0; i < g->num_cols; i++)
    {
        /* Turn over reserve if space on tableau */
        if (g->col_size[i] < 0 && g->col_size[g->num_cols] >= 0)
        {
            g->cols[i][++g->col_size[i]] = 
                g->cols[g->num_cols][g->col_size[g->num_cols]--];
            g->print_col[i]=1;
            draw_piles(g->main,g);
            draw_piles(g->free,g);
            g->undo = push_items(g->undo,g->num_cols,i,1,UNDO_RESERVE);
        }
    }
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

            /* Move at least one card */
            if (number==0)
                number = find_move(src,dst,DESC,ALT_COL,WRAP,g);
            if (number==0)
                continue;

            if (number > 1 && number != g->col_size[src]+1)
            {
                show_error("Must move full column",g->input);
                continue;
            }

            if (check_sequence(number,src,ASC,ALT_COL,WRAP,g))
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
            {
                foundation_automove(number==0?1:number,g);
                flip_reserve(g);
            }
            continue;
        }

        if (dst==NOCARD) 
            continue;
        /* Next check if card can move there */
        if (dst < g->num_cols) 
        {
            if (g->col_size[dst] >= 0)
                if (check_move(dst,card,DESC,ALT_COL,WRAP,g))
                    continue;
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
        flip_reserve(g);
    }
}

void canfield(GameInfo* g)
{
    int carry_on=2;
    char game_str[80];
    
    while (carry_on)
    {
        if (carry_on==2)
            if (init_vars(g)) return;
        if (create_windows(g)) return;
        draw_screen(g);
        play(g);

        snprintf(game_str,80,"You did %d card moves, %d redeals and had %d reserve cards left.",
                g->moves,g->deals,g->col_size[g->num_cols]+1);
        carry_on=game_finished(g,game_str);
    }
}
/* canfield.c ends here */
