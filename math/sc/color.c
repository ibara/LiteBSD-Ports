// SC is free software distributed under the MIT license

#include "sc.h"

struct colorpair cpairs [CPAIRS];
static struct crange* color_base;

void initcolor (unsigned colornum)
{
    static const unsigned char c_DefaultColors [2*ArraySize(cpairs)] = {
	COLOR_WHITE,	COLOR_BLUE,
	COLOR_RED,	COLOR_WHITE,	// default for negative numbers
	COLOR_WHITE,	COLOR_RED,	// default for cells with errors
	COLOR_BLACK,	COLOR_YELLOW,	// default for '*' marking cells with attached notes
	COLOR_BLACK,	COLOR_CYAN,
	COLOR_RED,	COLOR_CYAN,
	COLOR_DEFAULT,	COLOR_DEFAULT,	// default colors
	COLOR_RED,	COLOR_DEFAULT
    };
    for (unsigned i = 0; i < ArraySize(cpairs); ++i) {
	if (!colornum || colornum == i+1) {
	    cpairs[i].fg = c_DefaultColors[2*i+0];
	    cpairs[i].bg = c_DefaultColors[2*i+1];
	    cpairs[i].expr = NULL;
	    init_pair(i+1, cpairs[i].fg, cpairs[i].bg);
	}
    }
}

void change_color (unsigned pair, struct enode *e)
{
    if (--pair >= ArraySize(cpairs)) {
	error("Invalid color number");
	return;
    }
    unsigned v = (int) eval(e);
    cpairs[pair].fg = v & 7;
    cpairs[pair].bg = (v >> 3) & 7;
    cpairs[pair].expr = e;
    if (color && has_colors())
	init_pair (pair+1, cpairs[pair].fg, cpairs[pair].bg);
    ++modflg;
    ++FullUpdate;
}

void add_crange (struct ent *r_left, struct ent *r_right, unsigned pair)
{
    struct crange *r;
    int minr, minc, maxr, maxc;

    minr = r_left->row < r_right->row ? r_left->row : r_right->row;
    minc = r_left->col < r_right->col ? r_left->col : r_right->col;
    maxr = r_left->row > r_right->row ? r_left->row : r_right->row;
    maxc = r_left->col > r_right->col ? r_left->col : r_right->col;

    if (!pair) {
	if (color_base) {
	    for (r = color_base; r; r = r->r_next) {
		if (    (r->r_left->row == r_left->row) &&
			(r->r_left->col == r_left->col) &&
			(r->r_right->row == r_right->row) &&
			(r->r_right->col == r_right->col)) {
		    if (r->r_next)
			r->r_next->r_prev = r->r_prev;
		    if (r->r_prev)
			r->r_prev->r_next = r->r_next;
		    else
			color_base = r->r_next;
		    scxfree((char *)r);
		    modflg++;
		    FullUpdate++;
		    return;
		}
	    }
	}
	error("Color range not defined");
	return;
    }

    r = (struct crange*) scxmalloc (sizeof(struct crange));
    r->r_left = lookat(minr, minc);
    r->r_right = lookat(maxr, maxc);
    r->r_color = pair;

    r->r_next = color_base;
    r->r_prev = (struct crange *)0;
    if (color_base)
	color_base->r_prev = r;
    color_base = r;

    modflg++;
    FullUpdate++;
}

void clean_crange (void)
{
    struct crange* cr = color_base;
    color_base = NULL;

    while (cr) {
	struct crange* nextcr = cr->r_next;
	scxfree((char *)cr);
	cr = nextcr;
    }
}

struct crange* find_crange (int row, int col)
{
    struct crange *r;
    if (color_base)
	for (r = color_base; r; r = r->r_next)
	    if (r->r_left->row <= row && r->r_left->col <= col && r->r_right->row >= row && r->r_right->col >= col)
		return r;
    return 0;
}

void sync_cranges (void)
{
    for (struct crange* cr = color_base; cr; cr = cr->r_next) {
	cr->r_left = lookat(cr->r_left->row, cr->r_left->col);
	cr->r_right = lookat(cr->r_right->row, cr->r_right->col);
    }
}

void write_cranges (FILE *f)
{
    struct crange* r = color_base;
    for (struct crange* nextr = r; nextr; r = nextr, nextr = r->r_next) {}
    for (; r; r = r->r_prev) {
	fprintf(f, "color %s", v_name(r->r_left->row, r->r_left->col));
	fprintf(f, ":%s", v_name(r->r_right->row, r->r_right->col));
	fprintf(f, " %d\n", r->r_color);
    }
}

void write_colors (FILE *f, int indent)
{
    int c = 0;
    for (unsigned i = 0; i < ArraySize(cpairs); i++) {
	if (cpairs[i].expr) {
	    sprintf(line, "color %d = ", i + 1);
	    linelim = strlen(line);
	    decompile(cpairs[i].expr, 0);
	    line[linelim] = '\0';
	    fprintf(f, "%*s%s\n", indent, "", line);
	    ++c;
	}
    }
    if (indent && c)
	fprintf(f, "\n");
}

void list_colors (FILE *f)
{
    struct crange *r;
    struct crange *nextr;

    write_colors(f, 2);
    linelim = -1;

    if (!are_colors()) {
	fprintf(f, "  No color ranges");
	return;
    }

    fprintf(f, "  %-30s Color\n"
	       "  %-30s -----\n","Range","-----");
    for (r = nextr = color_base; nextr; r = nextr, nextr = r->r_next) {}
    while (r) {
	fprintf(f, "  %-32s %d\n", r_name(r->r_left->row, r->r_left->col,
		r->r_right->row, r->r_right->col), r->r_color);
	r = r->r_prev;
    }
}

int are_colors (void)
{
    return (color_base != 0);
}

void fix_colors (int row1, int col1, int row2, int col2, int delta1, int delta2)
{
    if (!color_base)
	return;
    struct frange* fr = find_frange (currow, curcol);
    for (struct crange* cr = color_base; cr; cr = cr->r_next) {
	int r1 = cr->r_left->row, c1 = cr->r_left->col;
	int r2 = cr->r_right->row, c2 = cr->r_right->col;
	if (!(fr && (c1 < fr->or_left->col || c1 > fr->or_right->col))) {
	    if (r1 != r2 && r1 >= row1 && r1 <= row2) r1 = row2 - delta1;
	    if (c1 != c2 && c1 >= col1 && c1 <= col2) c1 = col2 - delta1;
	}
	if (!(fr && (c2 < fr->or_left->col || c2 > fr->or_right->col))) {
	    if (r1 != r2 && r2 >= row1 && r2 <= row2) r2 = row1 + delta2;
	    if (c1 != c2 && c2 >= col1 && c2 <= col2) c2 = col1 + delta2;
	}
	if (r1 > r2 || c1 > c2)	// the 0 means delete color range
	    add_crange(cr->r_left, cr->r_right, 0);
	else {
	    cr->r_left = lookat(r1, c1);
	    cr->r_right = lookat(r2, c2);
	}
    }
}
