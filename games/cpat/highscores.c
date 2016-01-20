/*****************************************************************************
 *                                                                           *
 * highscores.c - functions for reading/writing highscores
 *                                                                           *
 *            Copyright (C) 2007 Trevor Carey-Smith                          *  
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify it   *
 * under the terms of the GNU General Public License version 2 as published  *
 * by the Free Software Foundation. (See the COPYING file for more info.)    *
 *                                                                           *
 *****************************************************************************/

#include "cpat.h"
#include "highscores.h"

/* Writes high score information to high score file */
int 
write_hs(void)
{
    FILE *fp;
    int i,j;

    if ((fp = fopen(hs.filename, "w")) == NULL) return 1;

    for (i=0;i<NUM_GAMES;i++)
    {
        for (j=0;j<MAX_VARIATIONS;j++)
        {
            if (hs.total_games[i][j]>0)
            {
                fprintf(fp, "%d %d %d %d %d %d %ld %ld %ld %d\n",
                        i,
                        j,
                        hs.total_games[i][j],
                        hs.finished_games[i][j],
                        hs.lowest_moves[i][j],
                        hs.lowest_deals[i][j],
                        hs.date_first_game[i][j],
                        hs.date_recent_game[i][j],
                        hs.date_best_game[i][j],
                        hs.seed[i][j]);
            }
        }
    }

    fclose(fp);
    return 0;
}

/* Initialises high score information in high score file */
void 
initialise_hs(void)
{
    int i,j;
    for (i=0;i<NUM_GAMES;i++)
    {
        for (j=0;j<MAX_VARIATIONS;j++)
        {
            hs.total_games[i][j]=0;
            hs.finished_games[i][j]=0;
            hs.lowest_moves[i][j]=-1;
            hs.lowest_deals[i][j]=-1;
            hs.date_first_game[i][j]=-1;
            hs.date_recent_game[i][j]=-1;
            hs.date_best_game[i][j]=-1;
            hs.seed[i][j]=-1;
        }
    }
}

/* Reads high score information from high score file */
void 
read_hs(void)
{
    FILE *fp;
    int i,j,items_matched,input,got_something,file_empty;
    int total_games,finished_games,lowest_moves,lowest_deals;
    int date_first_game,date_recent_game,date_best_game,seed;
    char buffer[200];

    if ((fp = fopen(hs.filename, "r")) == NULL)
        /* No file, we assume we haven't played before */
        return;

    got_something=0;
    file_empty=1;

    while( fgets(buffer, sizeof(buffer), fp) != NULL )
    {
        file_empty=0;
        items_matched=sscanf(buffer, "%d %d %d %d %d %d %d %d %d %d", 
                &i,
                &j,
                &total_games,
                &finished_games,
                &lowest_moves,
                &lowest_deals,
                &date_first_game,
                &date_recent_game,
                &date_best_game,
                &seed);
        if (items_matched==10)
        {
            got_something=1;
            hs.total_games[i][j]        = total_games;
            hs.finished_games[i][j]     = finished_games;
            hs.lowest_moves[i][j]       = lowest_moves;
            hs.lowest_deals[i][j]       = lowest_deals;
            hs.date_first_game[i][j]= date_first_game;
            hs.date_recent_game[i][j]= date_recent_game;
            hs.date_best_game[i][j]     = date_best_game;
            hs.seed[i][j]       = seed;
        }
    }
    fclose(fp);

    if (got_something==0 && file_empty==0)
    {
        printf("\nProblem reading cpat score file: %s\n",hs.filename);
        printf("Shall we overwrite this file? (y/N) ");
        input=fgetc(stdin);

        if (input=='y' || input=='Y')
        {
            if (remove(hs.filename))
            {
                /* Could not write */
                printf("Could not overwrite file... continuing without high score support.\n");
                hs.available = FALSE;
            }
        }
        else
        {
            printf("Continuing without high score support.\n");
            hs.available=FALSE;
        }
    }
}

void 
game_stats(int type)
{
//    char string[NUM_GAMES*MAX_VARIATIONS*200]; /* this will contain the scores*/
    char *string;
    char *header[2];
    char temp[200];
    char name_dashes[30],var_dashes[30];
    char time1[11],time2[11];
    int name_len=0;
    int num_lines=0;
    int variation_len=0;
    int line_len;
    int i,j;
    int firsttime=0;

    for (i=0;i<NUM_GAMES;i++)
    {
        for (j=0;j<MAX_VARIATIONS;j++)
        {
            if ((type==0 && hs.total_games[i][j]>0) || 
                    (type==1 && hs.finished_games[i][j]>0))
                num_lines++;
            if ((int)strlen(variations[i][j])>variation_len)
                variation_len=strlen(variations[i][j]);
        }
        if ((int)strlen(names[i])>name_len)
            name_len=strlen(names[i]);
    }

    snprintf(name_dashes,30,"---------------------------");
    snprintf(var_dashes,30,"---------------------------");
    name_dashes[name_len]='\0';
    var_dashes[variation_len]='\0';

    line_len=name_len+variation_len+40;
    
    header[0] = (char *)malloc (line_len);
    header[1] = (char *)malloc (line_len);
    if (type==0)
    {
        snprintf(header[0],200,"%-*s %-*s %-4s %-4s %-10s %-11s",
                name_len,"Game",variation_len,"Variation",
                "Trys","Wins","First Game","Latest Game");
        snprintf(header[1],200,"%-*s %-*s %-4s %-4s %-10s %-11s",
                name_len,name_dashes,variation_len,var_dashes,
                "----","----","----------","-----------");
    }
    else
    {
        snprintf(header[0],200,"%-*s %-*s %-5s %-5s %-10s %-10s",
                name_len,"Game",variation_len,"Variation",
                "Moves","Deals","Date","Seed");
        snprintf(header[1],200,"%-*s %-*s %-5s %5s %-10s %-10s",
                name_len,name_dashes,variation_len,var_dashes,
                "-----", "-----", "----------", "----------");
    }
    
    /* num_lines+1 so that its never 0 */
    string = (char *)malloc ((strlen(header[0])+1)*(num_lines+1));

    snprintf(string,200,"No stats available.");
    for (i=0;i<NUM_GAMES;i++)
    {
        for (j=0;j<MAX_VARIATIONS;j++)
        {
            if (type==0 && hs.total_games[i][j]>0)
            {
                if (firsttime++) 
                    strncat(string,"\n",1);
                else 
                    string[0]='\0';
                strftime(time1,11,"%F",localtime(&hs.date_first_game[i][j]));
                strftime(time2,11,"%F",localtime(&hs.date_recent_game[i][j]));
                snprintf(temp,200,"%-*s %-*s %4d %4d %10s %11s",
                        name_len,names[i],
                        variation_len,variations[i][j],
                        hs.total_games[i][j],
                        hs.finished_games[i][j],
                        time1,time2);
                strncat(string,temp,200);
            }
            else if (type==1 && hs.finished_games[i][j]>0)
            {
                if (firsttime++) 
                    strncat(string,"\n",1);
                else 
                    string[0]='\0';
                strftime(time1,11,"%F",localtime(&hs.date_best_game[i][j]));
                snprintf(temp,200,"%-*s %-*s %5d %5d %10s %10d",
                        name_len,names[i],
                        variation_len,variations[i][j],
                        hs.lowest_moves[i][j],
                        hs.lowest_deals[i][j]==-1?0:hs.lowest_deals[i][j],
                        time1,hs.seed[i][j]);
                strncat(string,temp,200);
            }
        }
    }

    /* Print highscore table for all games */
    pager(type==0?"Game Statistics":"High Scores",string,2,header);
    free(header[0]);
    free(header[1]);
    free(string);
}

/* highscores.c ends here */
