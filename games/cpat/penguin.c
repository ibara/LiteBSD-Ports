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

static void init_vars(GameInfo* g);
static void play(GameInfo* g);

/* Initialise variables for current game */
static void
init_vars(GameInfo* g)
{
    int i,j;

    g->num_cols=7;
    g->num_foun=4;
    g->num_free=7;
    g->num_packs=1;
    g->foun_dir=ASC;

    /* create and shuffle deck */
    init_deck(g);

    /* Set initial column lengths */
    for (j=0; j < g->num_cols; j++)
        g->col_size[j] = 6;

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

    /* Find the beak and move other cards of same rank to foundation. */
    g->foun_start = g->cols[0][0] % SUIT_LENGTH;
    for (i=0;i<g->num_cols;i++)
    {
        for (j=0;j<=g->col_size[i];j++)
        {
            /* Leave actual beak where it is */
            if (i==0 && j==0) 
                continue;
            if (g->cols[i][j] % SUIT_LENGTH == g->foun_start)
            {
                g->foundation[g->cols[i][j] / SUIT_LENGTH]=g->cols[i][j];
                g->foun_size[g->cols[i][j] / SUIT_LENGTH]++;
                g->cols[i][j--] = g->deck[--g->face_down];
            }
        }
    }
    /* The other cards might have been in the leftovers */
    while (g->face_down)
    {
        g->face_down--;
        g->foundation[g->deck[g->face_down]/SUIT_LENGTH]=g->deck[g->face_down];
        g->foun_size[g->deck[g->face_down] / SUIT_LENGTH]++;
    }

    /* No moves yet!! */
    g->moves=0;
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
    int num_freecells;
    int freecells[g->num_free];

    while (g->finished_foundations < g->num_foun)
    {

        if (grab_input(g,&src, &dst, &number))
            break;  /* quit game */

        if (src<=NOCARD) 
            continue;
        else if (src < g->num_cols)
        {
            /* We want to move at least one card */
            if (number==0) number=1; 

            /* Check if stack to move is rbrbr */
            if (dst < g->num_cols || dst >= g->num_cols+g->num_free)
            {
                if (check_sequence(number,src,ASC,IN_SUIT,WRAP,g))
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
            if (dst >= g->num_cols+g->num_free)
                card=g->cols[src][g->col_size[src]];
            else
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
                src=g->num_cols+number-1; /* -1 to translate 1-7 to 0-6 */
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
        else if (dst < g->num_cols) 
        {
            if (g->col_size[dst] >= 0)
            {
                if (check_move(dst,card,DESC,IN_SUIT,WRAP,g))
                    continue;
            }
            /* can only move cards to vacant columns if they are Kings */
            else if (card % SUIT_LENGTH != 
                    (g->foun_start-1+SUIT_LENGTH)%SUIT_LENGTH)
            {
                show_error("Can't move that card to a vacant column.",g->input);
                continue;
            }
        }
        else if (dst < g->num_cols+g->num_free)
        {
            num_freecells=0;
            /* First check for actual freecells */
            for (j=0;j < g->num_free; j++)
                if (g->freepile[j]==NOCARD)
                    freecells[num_freecells++]=j+g->num_cols;
            if (num_freecells < number)
            {
                show_error("Not enough free cells.",g->input);
                continue;
            }
        }
        else if (dst >= g->num_cols+g->num_free)
        {
            if (check_move(dst,card,0,0,NO_WRAP,g))
                continue;
        }

        /* now do move(s) */

        if (dst==g->num_cols)
        {
            dst=freecells[number-1];
            for (j=0;j<number-1;j++)
            {
                move_card(src,freecells[j],1,g);
                nanosleep(&pauselength,&pauseleft);
            }
            /* Now move the base card */
            move_card(src,dst,1,g);
        }
        else if (dst >= g->num_cols+g->num_free)
        {
            for (j=0;j<number-1;j++)
            {
                move_card(src,dst,1,g);
                nanosleep(&pauselength,&pauseleft);
            }
            /* Now move the base card */
            move_card(src,dst,1,g);
        }
        else
            move_card(src,dst,number,g);
    }
}

/* freecell wrapper functions */
void 
penguin(GameInfo* g)
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
/* penguin.c ends here */
