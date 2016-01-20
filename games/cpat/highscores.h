/*****************************************************************************
 *                                                                           *
 * document.h - header for document.c                                        *
 *                                                                           *
 *            Copyright (C) 2006 Trevor Carey-Smith                          *  
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify it   *
 * under the terms of the GNU General Public License version 2 as published  *
 * by the Free Software Foundation. (See the COPYING file for more info.)    *
 *                                                                           *
 *****************************************************************************/

#ifndef CPAT_HIGHSCORES_H
#define CPAT_HIGHSCORES_H 1

int write_hs(void);
void initialise_hs(void);
void read_hs(void);
void game_stats(int type);

#endif /* !CPAT_HIGHSCORES_H */
/* document.h ends here */
