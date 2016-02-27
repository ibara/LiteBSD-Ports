// SC is free software distributed under the MIT license

#include "sc.h"

static struct frange *frame_base;
extern struct frange *lastfr;

void add_frange(struct ent *or_left, struct ent *or_right, struct ent *ir_left, struct ent *ir_right, int toprows, int bottomrows, int leftcols, int rightcols)
{
    struct frange *r;
    int minr, minc, maxr, maxc;

    minr = or_left->row < or_right->row ? or_left->row : or_right->row;
    minc = or_left->col < or_right->col ? or_left->col : or_right->col;
    maxr = or_left->row > or_right->row ? or_left->row : or_right->row;
    maxc = or_left->col > or_right->col ? or_left->col : or_right->col;

    or_left = lookat(minr, minc);
    or_right = lookat(maxr, maxc);

    if (ir_left) {
	minr = ir_left->row < ir_right->row ? ir_left->row : ir_right->row;
	minc = ir_left->col < ir_right->col ? ir_left->col : ir_right->col;
	maxr = ir_left->row > ir_right->row ? ir_left->row : ir_right->row;
	maxc = ir_left->col > ir_right->col ? ir_left->col : ir_right->col;

	ir_left = lookat(minr, minc);
	ir_right = lookat(maxr, maxc);

	if (ir_left->row < or_left->row ||
		ir_left->col < or_left->col ||
		ir_right->row > or_right->row ||
		ir_right->col > or_right->col) {
	    error("Invalid parameters");
	    return;
	}
    }

    if (frame_base) {
	// Has this frange already been created?  If so, any negative
	// parameters mean "don't change this value."
	for (r = frame_base; r; r = r->r_next) {
	    if ((r->or_left == or_left) && (r->or_right == or_right)) {
		if (ir_left) {
		    r->ir_left = ir_left;
		    r->ir_right = ir_right;
		} else {
		    if (toprows < 0)
			toprows = r->ir_left->row - r->or_left->row;
		    if (bottomrows < 0)
			bottomrows = r->or_right->row - r->ir_right->row;
		    if (leftcols < 0)
			leftcols = r->ir_left->col - r->or_left->col;
		    if (rightcols < 0)
			rightcols = r->or_right->col - r->ir_right->col;
		    r->ir_left = lookat(r->or_left->row + toprows,
			    r->or_left->col + leftcols);
		    r->ir_right = lookat(r->or_right->row - bottomrows,
			    r->or_right->col - rightcols);
		}

		// If all frame sides are 0, delete the frange
		if (r->ir_left == r->or_left && r->ir_right == r->or_right) {
		    if (r->r_next)
			r->r_next->r_prev = r->r_prev;
		    if (r->r_prev)
			r->r_prev->r_next = r->r_next;
		    else
			frame_base = r->r_next;
		    scxfree((char *)r);
		    if (lastfr == r) lastfr = NULL;
		}
		modflg++;
		FullUpdate++;
		return;
	    }
	}
	// See if the specified range overlaps any previously created frange.
	for (r = frame_base; r; r = r->r_next) {
	    if (  !(r->or_left->row  > or_right->row ||
		    r->or_right->row < or_left->row  ||
		    r->or_left->col  > or_right->col ||
		    r->or_right->col < or_left->col)) {
		error("Framed ranges may not be nested or overlapping");
		return;
	    }
	}
    }

    if (ir_left != or_left || ir_right != or_right) {
	r = (struct frange *)scxmalloc((unsigned)sizeof(struct frange));
	r->or_left = or_left;
	r->or_right = or_right;

	if (ir_left) {
	    r->ir_left  = ir_left;
	    r->ir_right = ir_right;
	} else {
	    if (toprows    < 0) toprows    = 0;
	    if (bottomrows < 0) bottomrows = 0;
	    if (leftcols   < 0) leftcols   = 0;
	    if (rightcols  < 0) rightcols  = 0;
	    r->ir_left = lookat(r->or_left->row + toprows,
		    r->or_left->col + leftcols);
	    r->ir_right = lookat(r->or_right->row - bottomrows,
		    r->or_right->col - rightcols);
	}

	r->r_next = frame_base;
	r->r_prev = NULL;
	if (frame_base)
	    frame_base->r_prev = r;
	frame_base = r;
	modflg++;
	FullUpdate++;
    }
}

void clean_frange (void)
{
    struct frange *fr;
    struct frange *nextfr;

    fr = frame_base;
    frame_base = NULL;

    while (fr) {
	nextfr = fr->r_next;
	scxfree((char *)fr);
	fr = nextfr;
    }
    lastfr = NULL;
}

struct frange* find_frange(int row, int col)
{
    if (frame_base)
	for (struct frange* r = frame_base; r; r = r->r_next)
	    if (r->or_left->row <= row && r->or_left->col <= col && r->or_right->row >= row && r->or_right->col >= col)
		return r;
    return 0;
}

void sync_franges (void)
{
    for (struct frange* fr = frame_base; fr; fr = fr->r_next) {
	fr->or_left  = lookat(fr->or_left->row,  fr->or_left->col);
	fr->or_right = lookat(fr->or_right->row, fr->or_right->col);
	fr->ir_left  = lookat(fr->ir_left->row,  fr->ir_left->col);
	fr->ir_right = lookat(fr->ir_right->row, fr->ir_right->col);
    }
}

void write_franges (FILE *f)
{
    struct frange* r = frame_base;
    for (struct frange* nextr = r; nextr; r = nextr, nextr = r->r_next) {}
    for (; r; r = r->r_prev) {
	fprintf(f, "frame %s", v_name(r->or_left->row, r->or_left->col));
	fprintf(f, ":%s", v_name(r->or_right->row, r->or_right->col));
	fprintf(f, " %s", v_name(r->ir_left->row, r->ir_left->col));
	fprintf(f, ":%s\n", v_name(r->ir_right->row, r->ir_right->col));
    }
}

void list_frames (FILE *f)
{
    if (!are_frames()) {
	fprintf(f, "  No frames");
	return;
    }
    fprintf(f, "  %-30s Inner Range\n"
	       "  %-30s -----------\n","Outer Range","-----------");
    struct frange* r = frame_base;
    for (struct frange* nextr = r; nextr; r = nextr, nextr = r->r_next) // */ ;
    for (; r; r = r->r_prev) {
	fprintf(f, "  %-30s", r_name(r->or_left->row, r->or_left->col, r->or_right->row, r->or_right->col));
	fprintf(f, " %s\n", r_name(r->ir_left->row, r->ir_left->col, r->ir_right->row, r->ir_right->col));
    }
}

int are_frames (void)
{
    return (frame_base != 0);
}

void fix_frames (int row1, int col1, int row2, int col2, int delta1, int delta2)
{
    if (!frame_base)
	return;
    struct frange* cfr = find_frange(currow, curcol);
    for (struct frange* fr = frame_base; fr; fr = fr->r_next) {
	int r1 = fr->or_left->row, c1 = fr->or_left->col;
	int r2 = fr->or_right->row, c2 = fr->or_right->col;

	if (!(cfr && (c1 < cfr->or_left->col || c1 > cfr->or_right->col))) {
	    if (r1 >= row1 && r1 <= row2) r1 = row2 - delta1;
	    if (c1 >= col1 && c1 <= col2) c1 = col2 - delta1;
	}

	if (!(cfr && (c2 < cfr->or_left->col || c2 > cfr->or_right->col))) {
	    if (r2 >= row1 && r2 <= row2) r2 = row1 + delta2;
	    if (c2 >= col1 && c2 <= col2) c2 = col1 + delta2;
	}

	fr->or_left = lookat(r1, c1);
	fr->or_right = lookat(r2, c2);

	r1 = fr->ir_left->row;
	c1 = fr->ir_left->col;
	r2 = fr->ir_right->row;
	c2 = fr->ir_right->col;

	if (!(cfr && (c1 < cfr->or_left->col || c1 > cfr->or_right->col))) {
	    if (r1 >= row1 && r1 <= row2) r1 = row2 - delta1;
	    if (c1 >= col1 && c1 <= col2) c1 = col2 - delta1;
	}

	if (!(cfr && (c2 < cfr->or_left->col || c2 > cfr->or_right->col))) {
	    if (r2 >= row1 && r2 <= row2) r2 = row1 + delta2;
	    if (c2 >= col1 && c2 <= col2) c2 = col1 + delta2;
	}

	fr->ir_left = lookat(r1, c1);
	fr->ir_right = lookat(r2, c2);
    }
}
