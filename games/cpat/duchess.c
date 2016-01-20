/*****************************************************************************
 *                                                                           *
 * duchess.c - functions for initialising and playing duchess.               *
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
    int i;

    g->num_cols=9;
    g->num_foun=4;
    g->num_free=2;
    g->num_packs=1;
    g->num_deals=2;
    g->foun_dir=ASC;
    g->turn_no=1;
    g->foun_start = -1; /* until foundation base is chosen in first move */

    /* create and shuffle the deck */
    init_deck(g);

    /* Set initial column lengths */
    for (i=0; i < g->num_cols; i++)
        g->col_size[i] = 2*(i/5);
    g->col_size[4] = CARDSEPARATOR; /* This is just a spacer column */

    /* Deal the pack into the columns */
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

static void play(GameInfo* g)
{
    int number,card,i,count;
    int spacer=4;
    int src,dst;

    while (g->finished_foundations < g->num_foun)
    {

        turnover_waste(g);

        if (grab_input(g,&src, &dst, &number))
            break;

        if (src == spacer || dst == spacer)
        {
            show_error("That column is just a spacer",g->input);
            continue;
        }

        if (src==NOCARD) 
            continue;
        else if (g->foun_start < 0 && (src < spacer || src >= g->num_cols || dst < g->num_cols+g->num_free))
        {
            show_error("Must move card from reserve to foundation",g->input);
            continue;
        }
        else if (src < spacer)
        {
            if (dst==NOCARD)
            {
                show_error("Excuse me?",g->input);
                continue;
            }
            else if (dst < spacer && g->col_size[dst] < 0)
            {
                count=0;
                for (i=spacer+1;i<g->num_cols;i++)
                    if (g->col_size[i]>=0)
                        count=1;
                if (count)
                {
                    show_error("You must move reserve cards to empty columns.",g->input);
                    continue;
                }
            }
            /* Can't move multiple cards to foundation*/
            if (dst>=g->num_cols) number=1; 

            if (number==0)
                number = find_move(src,dst,DESC,ALT_COL,WRAP,g);
            if (number==0)
                continue;

            if (check_sequence(number,src,ASC,ALT_COL,WRAP,g))
                continue;
            card=g->cols[src][g->col_size[src]-number+1];
        }
        else if (src < g->num_cols)
        {
            if (dst==NOCARD)
            {
                show_error("Excuse me?",g->input);
                continue;
            }
            /* Can't move multiple reserve cards */
            number=1; 

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
                foundation_automove(number==0?1:number,g);
            continue;
        }

        if (dst==NOCARD) 
            continue;
        /* Next check if card can move there */
        if (dst < spacer)
        {
            if (g->col_size[dst] < 0)
            {
                if (src >= g->num_cols)
                {
                    show_error("You can't move discards to empty columns.",g->input);
                    continue;
                }
            }
            else if (src >= spacer && src < g->num_cols)
            {
                show_error("Reserve cards must be moved to empty columns.",g->input);
                continue;
            }
            else if (check_move(dst,card,DESC,ALT_COL,WRAP,g))
                continue;
        }
        else if (dst<g->num_cols+g->num_free)
        {
            show_error("You can't move cards there.",g->input);
            continue;
        }
        else if (dst >= g->num_cols+g->num_free)
            if (g->foun_start >= 0 && check_move(dst,card,0,0,NO_WRAP,g))
                    continue;

        /* now do move(s) */
        move_card(src,dst,number,g);
        if (g->foun_start < 0) 
        {
            g->foun_start = card%SUIT_LENGTH;
            g->undo->type = UNDO_FOUN_START;
        }
    }
}

void duchess(GameInfo* g)
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

        snprintf(game_str,80,"You did %d card moves and %d redeals.",
                g->moves,g->deals);
        carry_on=game_finished(g,game_str);
    }
}
/* duchess.c ends here */
