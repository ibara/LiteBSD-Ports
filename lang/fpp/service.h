/* @(#)service.h 1.4 96/09/10 */
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

#define S_AL	1
#define S_NM	2
#define S_BL	4

#define is_blank(c)    (!is_special(c) && (chars[(unsigned char)c] & S_BL))
#define is_alpha0(c)   (!is_special(c) && (chars[(unsigned char)c] & S_AL) && ((c) != '$'))
#define is_alpha(c)    (!is_special(c) && (chars[(unsigned char)c] & S_AL)) 
#define is_num(c)      (!is_special(c) && (chars[(unsigned char)c] & S_NM))
#define is_alphanum(c) (!is_special(c) && (chars[(unsigned char)c] & (S_NM | S_AL)))
#define is_special(c)  ((c >> 8) == SYM_CTRL)

#define lowcase(c)     ((c) > 0 && (c) < 255 ? lcase[c] : (c))

/************************************************************* Types */
typedef struct _strlist {
	struct _strlist *next;
	char	*str;
} StrList;

/************************************************************* Variables */
extern unsigned char lcase[];
extern unsigned char chars[];

/************************************************************* Functions */
void	*my_alloc(unsigned, unsigned);
char	*my_strdup(char *s);
void	tolowcase(char *s);
void	tonlowcase(char *s,int n);
int	strtoi(char *s, int *i, int base);
