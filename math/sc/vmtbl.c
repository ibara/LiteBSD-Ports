// SC is free software distributed under the MIT license

#include "sc.h"

// check to see if *rowp && *colp are currently allocated, if not expand the
// current size if we can.
void checkbounds (int *rowp, int *colp)
{
    if (*rowp < 0)
	*rowp = 0;
    else if (*rowp >= maxrows) {
	if (*colp >= maxcols) {
	    if (!growtbl(GROWBOTH, *rowp, *colp)) {
		*rowp = maxrows - 1;
		*colp = maxcols - 1;
	    }
	    return;
	} else {
	    if (!growtbl(GROWROW, *rowp, 0))
		*rowp = maxrows - 1;
	    return;
	}
    }
    if (*colp < 0) 
	*colp = 0;
    else if (*colp >= maxcols) {
	if (!growtbl(GROWCOL, 0, *colp))
	    *colp = maxcols - 1;
    }
}

#define GROWALLOC(newptr, oldptr, nelem, type, msg) \
    oldptr = newptr = (type*) scxrealloc ((char*)oldptr, nelem*sizeof(type));

static const char nolonger[] = "The table can't be any longer";
static const char nowider[] = "The table can't be any wider";

//
// grow the main && auxiliary tables (reset maxrows/maxcols as needed)
// toprow &&/|| topcol tell us a better guess of how big to become.
// we return TRUE if we could grow, FALSE if not....
int growtbl (int rowcol, int toprow UNUSED, int topcol)
{
    int		*fwidth2;
    int		*precision2;
    int		*realfmt2;
    int		newcols;
    struct ent	***tbl2;
    struct ent	** nullit;
    int		cnt;
    char	*col_hidden2;
    char	*row_hidden2;
    int		newrows;
    int		i;

    newrows = maxrows;
    newcols = maxcols;
    if (rowcol == GROWNEW) {
	maxrows = toprow = 0;
	// when we first start up, fill the screen w/ cells
	{
	    int startval;
	    startval = LINES - RESROW;
	    newrows = startval > MINROWS ? startval : MINROWS;
	    startval = ((COLS) - rescol) / DEFWIDTH;
	    newcols = startval > MINCOLS ? startval : MINCOLS;
	}
	maxcols = topcol = 0;
    }
    // set how much to grow
    if (rowcol == GROWROW || rowcol == GROWBOTH) {
	if (toprow > maxrows)
	    newrows = GROWAMT + toprow;
	else
	    newrows += GROWAMT;
    }
    if (rowcol == GROWCOL || rowcol == GROWBOTH) {
	if (rowcol == GROWCOL && (maxcols == ABSMAXCOLS || topcol >= ABSMAXCOLS)) {
	    error(nowider);
	    return (FALSE);
	}
	if (topcol > maxcols)
	    newcols = GROWAMT + topcol;
	else
	    newcols += GROWAMT;
	if (newcols > ABSMAXCOLS)
	    newcols = ABSMAXCOLS;
    }

    if (rowcol == GROWROW || rowcol == GROWBOTH || rowcol == GROWNEW) {
	struct	ent *** lnullit;
	int	lcnt;

	GROWALLOC(row_hidden2, row_hidden, newrows, char, nolonger);
	memset(row_hidden+maxrows, 0, (newrows-maxrows)*sizeof(char));
	// alloc tbl row pointers, per net.lang.c, calloc does not
	// necessarily fill in NULL pointers
	GROWALLOC(tbl2, tbl, newrows, struct ent **, nolonger);
	for (lnullit = tbl+maxrows, lcnt = 0; lcnt < newrows-maxrows;
		lcnt++, lnullit++)
	    *lnullit = (struct ent **)NULL;
    }

    if ((rowcol == GROWCOL) || (rowcol == GROWBOTH) || (rowcol == GROWNEW)) {
	GROWALLOC(fwidth2, fwidth, newcols, int, nowider);
	GROWALLOC(precision2, precision, newcols, int, nowider);
	GROWALLOC(realfmt2, realfmt, newcols, int, nowider);
	GROWALLOC(col_hidden2, col_hidden, newcols, char, nowider);
	memset(col_hidden+maxcols, 0, (newcols-maxcols)*sizeof(char));
	for (i = maxcols; i < newcols; i++) {
	    fwidth[i] = DEFWIDTH;
	    precision[i] = DEFPREC;
	    realfmt[i] = DEFREFMT;
	}

	// [re]alloc the space for each row
	for (i = 0; i < maxrows; i++) {
	    if ((tbl[i] = (struct ent **)scxrealloc((char *)tbl[i],
		(unsigned)(newcols * sizeof(struct ent **)))) == (struct ent **)0) {
	    error(nowider);
	    return(FALSE);
	    }
	for (nullit = ATBL(tbl, i, maxcols), cnt = 0;
		cnt < newcols-maxcols; cnt++, nullit++)
	    *nullit = (struct ent *)NULL;
	}
    }
    else
	i = maxrows;

    // fill in the bottom of the table
    for (; i < newrows; i++) {
	if ((tbl[i] = (struct ent **)scxmalloc((unsigned)(newcols *
		sizeof(struct ent **)))) == (struct ent **)0) {
	    error(nowider);
	    return(FALSE);
	}
	for (nullit = tbl[i], cnt = 0; cnt < newcols; cnt++, nullit++)
	    *nullit = (struct ent *)NULL;
    }

    FullUpdate++;
    maxrows = newrows;

    if (maxrows > 1000) rescol = 5;
    if (maxrows > 10000) rescol = 6;

    maxcols = newcols;
    return (TRUE);
}

// grow the main && auxiliary tables (reset maxrows/maxcols as needed)
// toprow &&/|| topcol tell us a better guess of how big to become.
// we return TRUE if we could grow, FALSE if not....
int psc_growtbl (int rowcol, int topcol)
{
    int newcols = maxcols;
    if (rowcol == GROWNEW) {
	newcols = MINCOLS;
	maxcols = topcol = 0;
    }
    if (rowcol == GROWCOL || rowcol == GROWBOTH) {
	if (rowcol == GROWCOL && (maxcols == ABSMAXCOLS || topcol >= ABSMAXCOLS))
	    return (FALSE);
	if (topcol > maxcols)
	    newcols = GROWAMT + topcol;
	else
	    newcols += GROWAMT;
	if (newcols > ABSMAXCOLS)
	    newcols = ABSMAXCOLS;
    }
    if ((rowcol == GROWCOL) || (rowcol == GROWBOTH) || (rowcol == GROWNEW)) {
	int *fwidth2, *precision2, *realfmt2;
	GROWALLOC(fwidth2, fwidth, newcols, int, nowider);
	GROWALLOC(precision2, precision, newcols, int, nowider);
	GROWALLOC(realfmt2, realfmt, newcols, int, nowider);
	memset(fwidth+maxcols, 0, (newcols-maxcols)*sizeof(int));
	memset(precision+maxcols, 0, (newcols-maxcols)*sizeof(int));
	memset(realfmt+maxcols, 0, (newcols-maxcols)*sizeof(int));
    }
    maxcols = newcols;
    return (TRUE);
}
