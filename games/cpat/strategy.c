/*****************************************************************************
 *                                                                           *
 * strategy.c - functions for initialising and playing strategy.             *
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
    char *queries[1] = {"Choose normal or hard:"};
    char *items[3] = {
        "normal (all cards are visible)",
        "hard (only top card is visible)",
        "return to main menu"
    };

    snprintf(title,40,"Welcome to %s",names[g->game]);

    g->variation=menu(title,queries,1,items,num_items,items,0);

    if (g->variation == 2) return 1;

    g->num_cols=8;
    g->num_foun=4;
    g->num_free=2;
    g->turn_no=1;
    g->num_packs=1;
    g->num_deals=1;
    g->foun_dir=ASC;
    g->foun_start=ACE;

    /* create and shuffle the deck */
    init_deck(g);

    /* Set initial column lengths */
    for (j=0; j < g->num_cols; j++)
        g->col_size[j] = -1;

    /* Take out ACES */
    for (j=i=0;j<PACK_SIZE;j++)
        if (g->deck[j]%SUIT_LENGTH != ACE)
            g->deck[i++]=g->deck[j];

    /* Number of cards left in the deck */
    g->face_down = g->num_packs*PACK_SIZE-4;
    /* Number of cards turned over */
    g->face_up = 0;

    /* Initialise freepile and foundation (with ACES) */
    for (i=0; i < g->num_foun; i++)
    {
        g->foundation[i]=i*SUIT_LENGTH;
        g->foun_size[i]=1;
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

        /* We roll the deck automatically to save an extra command */
        if (g->face_up==0 && g->face_down!=0)
        {
            roll_deckcards(g);
            g->undo=pop_items(g->undo);
        }
        /* This undoes the roll_deckcards above. We do this here as it is
         * an automatic move */
        while (g->face_up>1)
        {
            /* Because we undo the roll_deckcards after we have undone
             * the 'move waste to column' we need to take the second card
             * on the face_up stack!! */
            g->deck[g->face_down++] = 
                g->deck[g->num_packs*PACK_SIZE-(g->face_up--)+1];
            g->deck[g->num_packs*PACK_SIZE-(g->face_up)]=g->freepile[1];
            g->freepile[0]=CARDBACK;
            draw_piles(g->free,g);
        }

        if (grab_input(g,&src, &dst, &number))
            break;

        if (src==NOCARD) 
            continue;
        else if (src < g->num_cols)
        {
            /* Can't move multiple cards*/
            number=1; 
            if (dst == NOCARD)
            {
                dst=src;
                src=g->num_cols+1;
                if (g->freepile[src-g->num_cols] == NOCARD)
                {
                    show_error("No cards left to place.",g->input);
                    continue;
                }
            }
            else if (dst < g->num_cols+g->num_free)
            {
                show_error("Can't move card there.",g->input);
                continue;
            }
            else
                card=g->cols[src][g->col_size[src]-number+1];
        }
        else if (src < g->num_cols+g->num_free)
        { 
            if (g->face_up == 0)
            {
                show_error("No cards left to place.",g->input);
                continue;
            }
            else if (dst>=g->num_cols || dst==NOCARD)
            {
                show_error("Card must be moved to the tableau.",g->input);
                continue;
            }
            /* Can only move one card from deck */
            number=1; 
            /* this is for moving a card off the face up deck */
            src=g->num_cols+1;
        }
        /* Automatically move cards to the foundations */
        else if (src==g->num_cols+g->num_free)
        {
            if (dst!=NOCARD)
                show_error("Can't move cards off the foundations.",g->input);
            else if (g->face_down != 0 || g->face_up != 0)
                show_error("Can't move cards to the foundation until deck is finished.",g->input);
            else
                foundation_automove(number==0?1:number,g);
            continue;
        }

        if (dst==NOCARD) 
            continue;
        /* Next check if card can move there */
        else if (dst < g->num_cols) 
        {
            /* turn card already on that column face down. */
            if (g->variation==1 && g->col_size[dst]>=0)
                    g->cols[dst][g->col_size[dst]]-=FACE_DOWN;
        }
        else if (dst<g->num_cols+g->num_free)
        {
            show_error("You can't move cards there.",g->input);
            continue;
        }
        else if (dst >= g->num_cols+g->num_free)
        {
            if (g->face_down != 0)
            {
                show_error("Can't move cards to the foundation until deck is finished.",g->input);
                continue;
            }
            if (check_move(dst,card,0,0,NO_WRAP,g))
                continue;
        }

        /* now do move(s) */
        move_card(src,dst,number,g);

    }
}

void strategy(GameInfo* g)
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

        snprintf(game_str,60,"You placed %d out of %d cards.",
                g->foun_size[0]+g->foun_size[1]+g->foun_size[2]+g->foun_size[3],
                PACK_SIZE);
        carry_on=game_finished(g,game_str);
    }
}
/* strategy.c ends here */
