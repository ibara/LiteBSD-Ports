/* @(#)lex.h 1.4 96/06/28 */
/*
 * ====================================================
 * Copyright (C) 1995 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunSoft, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice
 * is preserved.
 * ====================================================
 */

Token *get_token(unsigned);
Token *get_token_nl(unsigned);
Token *get_cur_tok();

