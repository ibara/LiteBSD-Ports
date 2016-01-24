/* html_tbl.c
 * Tables in HTML
 * (c) 2002 Mikulas Patocka
 * This file is a part of the Links program, released under GPL.
 */

#include "links.h"

#ifdef DEBUG
#undef DEBUG
#endif

static void free_table_cache(void);

/*#define DEBUG*/

#define RECT_BOUND_BITS	10	/* --- bound at 1024 pixels */

#define AL_TR		-1

#define VAL_TR		-1
#define VAL_TOP		0
#define VAL_MIDDLE	1
#define VAL_BOTTOM	2

#define W_AUTO		-1
#define W_REL		-2

#define F_VOID		0
#define F_ABOVE		1
#define F_BELOW		2
#define F_HSIDES	3
#define F_LHS		4
#define F_RHS		8
#define F_VSIDES	12
#define F_BOX		15

#define R_NONE		0
#define R_ROWS		1
#define R_COLS		2
#define R_ALL		3
#define R_GROUPS	4

static void get_align(unsigned char *attr, int *a)
{
	unsigned char *al;
	if ((al = get_attr_val(attr, cast_uchar "align"))) {
		if (!(strcasecmp(cast_const_char al, "left"))) *a = AL_LEFT;
		if (!(strcasecmp(cast_const_char al, "right"))) *a = AL_RIGHT;
		if (!(strcasecmp(cast_const_char al, "center"))) *a = AL_CENTER;
		if (!(strcasecmp(cast_const_char al, "justify"))) *a = AL_BLOCK;
		if (!(strcasecmp(cast_const_char al, "char"))) *a = AL_RIGHT; /* NOT IMPLEMENTED */
		mem_free(al);
	}
}

static void get_valign(unsigned char *attr, int *a)
{
	unsigned char *al;
	if ((al = get_attr_val(attr, cast_uchar "valign"))) {
		if (!(strcasecmp(cast_const_char al, "top"))) *a = VAL_TOP;
		if (!(strcasecmp(cast_const_char al, "middle"))) *a = VAL_MIDDLE;
		if (!(strcasecmp(cast_const_char al, "bottom"))) *a = VAL_BOTTOM;
		if (!(strcasecmp(cast_const_char al, "baseline"))) *a = VAL_TOP; /* NOT IMPLEMENTED */
		mem_free(al);
	}
}

static void get_c_width(unsigned char *attr, int *w, int sh)
{
	unsigned char *al;
	if ((al = get_attr_val(attr, cast_uchar "width"))) {
		if (*al && al[strlen(cast_const_char al) - 1] == '*') {
			unsigned char *en;
			unsigned long n;
			al[strlen(cast_const_char al) - 1] = 0;
			n = strtoul(cast_const_char al, (char **)(void *)&en, 10);
			if (n < 10000 && !*en) *w = W_REL - (int)n;
		} else {
			int p = get_width(attr, cast_uchar "width", sh);
			if (p >= 0) *w = p;
		}
		mem_free(al);
	}
}

#define INIT_X		8
#define INIT_Y		8

struct table_cell {
	int used;
	int spanned;
	int mx, my;
	unsigned char *start;
	unsigned char *end;
	int align;
	int valign;
	int b;
	struct rgb bgcolor;
	int group;
	int colspan;
	int rowspan;
	int min_width;
	int max_width;
	int x_width;
	int height;
	int xpos, ypos, xw, yw;
	int link_num;
	unsigned char *tag;
#ifdef G
	unsigned char bgcolor_str[8];
	struct g_object_area *root;
	struct g_object_tag *tag_object;
	struct rect_set *brd;
	int g_width;
	struct rect rect;
	tcount dgen;
#endif
};

struct table_column {
	int group;
	int align;
	int valign;
	int width;
};

struct table {
	struct part *p;
#ifdef G
	struct g_part *gp;
#endif
	int x, y;
	int rx, ry;
	int align;
	int border, cellpd, vcellpd, cellsp;
	int frame, rules, width, wf;
	unsigned char *bordercolor;
	int *min_c, *max_c;
	int *w_c;
	int rw;
	int min_t, max_t;
	struct table_cell *cells;
	int c, rc;
	struct table_column *cols;
	int xc;
	int *xcols;
	int *r_heights;
	int rh;
	int link_num;
	struct rgb bgcolor;
#ifdef G
	struct background *bg;
	struct background *frame_bg;
	struct rect_set **r_bg;
	int nr_bg;
	struct rect_set **r_frame;
	int nr_frame;
	struct table_cell ***r_cells;
	int *w_cells;
	int nr_cells;
#endif
};


#ifdef DEBUG
#define CELL(t, x, y) (((x) < 0 || (x) >= (t)->rx || (y) < 0 || (y) >= (t)->ry) ? (internal("accessing cell out of table (%d,%d) - limit (%d,%d)", (x), (y), (t)->rx, (t)->ry), (t)->cells) : &(t)->cells[(y) * (t)->rx + (x)])
#else
#define CELL(t, x, y) (&(t)->cells[(y) * (t)->rx + (x)])
#endif

static unsigned char frame_table[81] = {
	0x00, 0xb3, 0xba,	0xc4, 0xc0, 0xd3,	0xcd, 0xd4, 0xc8,
	0xc4, 0xd9, 0xbd,	0xc4, 0xc1, 0xd0,	0xcd, 0xd4, 0xc8,
	0xcd, 0xbe, 0xbc,	0xcd, 0xbe, 0xbc,	0xcd, 0xcf, 0xca,

	0xb3, 0xb3, 0xba,	0xda, 0xc3, 0xd3,	0xd5, 0xc6, 0xc8,
	0xbf, 0xb4, 0xbd,	0xc2, 0xc5, 0xd0,	0xd5, 0xc6, 0xc8,
	0xb8, 0xb5, 0xbc,	0xb8, 0xb5, 0xbc,	0xd1, 0xd8, 0xca,

	0xba, 0xba, 0xba,	0xd6, 0xd6, 0xc7,	0xc9, 0xc9, 0xcc,
	0xb7, 0xb7, 0xb6,	0xd2, 0xd2, 0xd7,	0xc9, 0xc9, 0xcc,
	0xbb, 0xbb, 0xb9,	0xbb, 0xbb, 0xb9,	0xcb, 0xcb, 0xce,
};

static unsigned char hline_table[3] = { 0x20, 0xc4, 0xcd };
static unsigned char vline_table[3] = { 0x20, 0xb3, 0xba };

static struct table *new_table(void)
{
	struct table *t;
	t = mem_calloc(sizeof(struct table));
	t->p = NULL;
#ifdef G
	t->gp = NULL;
	t->r_frame = DUMMY;
	t->nr_frame = 0;
	t->r_bg = DUMMY;
	t->nr_bg = 0;
	t->r_cells = DUMMY;
	t->w_cells = DUMMY;
	t->nr_cells = 0;
#endif
	t->x = t->y = 0;
	t->rx = INIT_X;
	t->ry = INIT_Y;
	t->cells = mem_calloc(INIT_X * INIT_Y * sizeof(struct table_cell));
	t->c = 0;
	t->rc = INIT_X;
	t->cols = mem_calloc(INIT_X * sizeof(struct table_column));
	t->xcols = DUMMY;
	t->xc = 0;
	t->r_heights = DUMMY;
	return t;
}

static void free_table(struct table *t)
{
	int i, j;
	for (j = 0; j < t->y; j++) for (i = 0; i < t->x; i++) {
		struct table_cell *c = CELL(t, i, j);
		if (c->tag) mem_free(c->tag);
#ifdef G
		if (c->root) c->root->destruct(c->root);
		if (c->tag_object) c->tag_object->destruct((struct g_object *)c->tag_object);
		if (c->brd) mem_free(c->brd);
#endif
	}
#ifdef G
	if (F) {
		for (i = 0; i < t->nr_frame; i++) mem_free(t->r_frame[i]);
		if (t->r_frame) mem_free(t->r_frame);
		for (i = 0; i < t->nr_bg; i++) mem_free(t->r_bg[i]);
		if (t->r_bg) mem_free(t->r_bg);
		for (i = 0; i < t->nr_cells; i++) mem_free(t->r_cells[i]);
		mem_free(t->r_cells), mem_free(t->w_cells);
		if (t->frame_bg) g_release_background(t->frame_bg);
	}
#endif
	if (t->bordercolor) mem_free(t->bordercolor);
	if (t->min_c) mem_free(t->min_c);
	if (t->max_c) mem_free(t->max_c);
	if (t->w_c) mem_free(t->w_c);
	mem_free(t->r_heights);
	mem_free(t->cols);
	mem_free(t->xcols);
	mem_free(t->cells);
	mem_free(t);
}

static void expand_cells(struct table *t, int x, int y)
{
	int i, j;
	if (x >= t->x) {
		if (t->x) {
			for (i = 0; i < t->y; i++) if (CELL(t, t->x - 1, i)->colspan == -1) {
				for (j = t->x; j <= x; j++) {
					CELL(t, j, i)->used = 1;
					CELL(t, j, i)->spanned = 1;
					CELL(t, j, i)->rowspan = CELL(t, t->x - 1, i)->rowspan;
					CELL(t, j, i)->colspan = -1;
					CELL(t, j, i)->mx = CELL(t, t->x - 1, i)->mx;
					CELL(t, j, i)->my = CELL(t, t->x - 1, i)->my;
				}
			}
		}
		t->x = safe_add(x, 1);
	}
	if (y >= t->y) {
		if (t->y) {
			for (i = 0; i < t->x; i++) if (CELL(t, i, t->y - 1)->rowspan == -1) {
				for (j = t->y; j <= y; j++) {
					CELL(t, i, j)->used = 1;
					CELL(t, i, j)->spanned = 1;
					CELL(t, i, j)->rowspan = -1;
					CELL(t, i, j)->colspan = CELL(t, i, t->y - 1)->colspan;
					CELL(t, i, j)->mx = CELL(t, i, t->y - 1)->mx;
					CELL(t, i, j)->my = CELL(t, i, t->y - 1)->my;
				}
			}
		}
		t->y = safe_add(y, 1);
	}
}

static struct table_cell *new_cell(struct table *t, int x, int y)
{
	struct table nt;
	int i, j;
	if (x < t->x && y < t->y) goto ret;
	rep:
	if (x < t->rx && y < t->ry) {
		expand_cells(t, x, y);
		goto ret;
	}
	nt.rx = t->rx;
	nt.ry = t->ry;
	while (x >= nt.rx) {
		if ((unsigned)nt.rx > MAXINT / 2) overalloc();
		nt.rx *= 2;
	}
	while (y >= nt.ry) {
		if ((unsigned)nt.ry > MAXINT / 2) overalloc();
		nt.ry *= 2;
	}
	if ((unsigned)nt.rx * (unsigned)nt.ry / (unsigned)nt.rx != (unsigned)nt.ry) overalloc();
	if ((unsigned)nt.rx * (unsigned)nt.ry > MAXINT / sizeof(struct table_cell)) overalloc();
	nt.cells = mem_calloc(nt.rx * nt.ry * sizeof(struct table_cell));
	for (i = 0; i < t->x; i++)
		for (j = 0; j < t->y; j++)
			memcpy(CELL(&nt, i, j), CELL(t, i, j), sizeof(struct table_cell));
	mem_free(t->cells);
	t->cells = nt.cells;
	t->rx = nt.rx;
	t->ry = nt.ry;
	goto rep;

	ret:
	return CELL(t, x, y);
}

static void new_columns(struct table *t, int span, int width, int align, int valign, int group)
{
	if (safe_add(t->c, span) > t->rc) {
		int n = t->rc;
		struct table_column *nc;
		while (t->c + span > n) {
			if ((unsigned)n > MAXINT / 2) overalloc();
			n *= 2;
		}
		if ((unsigned)n > MAXINT / sizeof(struct table_column)) overalloc();
		nc = mem_realloc(t->cols, n * sizeof(struct table_column));
		t->rc = n;
		t->cols = nc;
	}
	while (span--) {
		t->cols[t->c].align = align;
		t->cols[t->c].valign = valign;
		t->cols[t->c].width = width;
		t->cols[t->c++].group = group;
		group = 0;
	}
}

static void set_td_width(struct table *t, int x, int width, int f)
{
	if (x >= t->xc) {
		int n = t->xc ? t->xc : 1;
		int i;
		int *nc;
		while (x >= n) {
			if ((unsigned)n > MAXINT / 2) overalloc();
			n *= 2;
		}
		if ((unsigned)n > MAXINT / sizeof(int)) overalloc();
		nc = mem_realloc(t->xcols, n * sizeof(int));
		for (i = t->xc; i < n; i++) nc[i] = W_AUTO;
		t->xc = n;
		t->xcols = nc;
	}
	if (t->xcols[x] == W_AUTO || f) {
		set:
		t->xcols[x] = width;
		return;
	}
	if (width == W_AUTO) return;
	if (width < 0 && t->xcols[x] >= 0) goto set;
	if (width >= 0 && t->xcols[x] < 0) return;
	t->xcols[x] = safe_add(t->xcols[x], width) / 2;
}

unsigned char *skip_element(unsigned char *html, unsigned char *eof, unsigned char *what, int sub)
{
	int l = (int)strlen(cast_const_char what);
	int level = 1;
	unsigned char *name;
	int namelen;
	r:
	while (html < eof && (*html != '<')) rr:html++;
	if (html + 2 <= eof && (html[1] == '!' || html[1] == '?')) {
		html = skip_comment(html, eof);
		goto r;
	}
	if (html >= eof) return eof;
	if (parse_element(html, eof, &name, &namelen, NULL, &html)) goto rr;
	if (namelen == l && !casecmp(name, what, l) && sub) level = safe_add(level, 1);
	if (namelen == l + 1 && name[0] == '/' && !casecmp(name + 1, what, l)) if (!--level) return html;
	goto r;
}

struct s_e {
	unsigned char *s, *e;
};

static int default_line_align(void)
{
	return par_format.align == AL_NO || par_format.align == AL_NO_BREAKABLE ? par_format.align : AL_LEFT;
}

static struct table *parse_table(unsigned char *html, unsigned char *eof, unsigned char **end, struct rgb *bgcolor, int sh, struct s_e **bad_html, int *bhp)
{
	int qqq;
	struct table *t;
	struct table_cell *cell;
	unsigned char *t_name, *t_attr, *en, *a;
	int t_namelen;
	int x = 0, y = -1;
	int p = 0;
	unsigned char *lbhp = NULL;
	int l_al = default_line_align();
	int l_val = VAL_MIDDLE;
	int csp, rsp;
	int group = 0;
	int i, j, k;
	struct rgb l_col;
	int c_al = AL_TR, c_val = VAL_TR, c_width = W_AUTO, c_span = 0;
	memcpy(&l_col, bgcolor, sizeof(struct rgb));
	*end = html;
	if (bad_html) {
		*bad_html = DUMMY;
		*bhp = 0;
	}
	t = new_table();
	memcpy(&t->bgcolor, bgcolor, sizeof(struct rgb));
	se:
	en = html;
	see:
	html = en;
	if (bad_html && !p && !lbhp) {
		if (!(*bhp & (ALLOC_GR-1))) {
			if ((unsigned)*bhp > MAXINT / sizeof(struct s_e) - ALLOC_GR) overalloc();
			*bad_html = mem_realloc(*bad_html, (*bhp + ALLOC_GR) * sizeof(struct s_e));
		}
		lbhp = (*bad_html)[(*bhp)++].s = html;
	}
	while (html < eof && *html != '<') html++;
	if (html >= eof) {
		if (p) CELL(t, x, y)->end = html;
		if (lbhp) (*bad_html)[*bhp-1].e = html;
		goto scan_done;
	}
	if (html + 2 <= eof && (html[1] == '!' || html[1] == '?')) {
		html = skip_comment(html, eof);
		goto se;
	}
	if (parse_element(html, eof, &t_name, &t_namelen, &t_attr, &en)) {
		html++;
		goto se;
	}
	if (t_namelen == 5 && !casecmp(t_name, cast_uchar "TABLE", 5)) {
		en = skip_element(en, eof, cast_uchar "TABLE", 1);
		goto see;
	}
	if (t_namelen == 6 && !casecmp(t_name, cast_uchar "SCRIPT", 5)) {
		en = skip_element(en, eof, cast_uchar "SCRIPT", 0);
		goto see;
	}
	if (t_namelen == 6 && !casecmp(t_name, cast_uchar "/TABLE", 6)) {
		if (c_span) new_columns(t, c_span, c_width, c_al, c_val, 1);
		if (p) CELL(t, x, y)->end = html;
		if (lbhp) (*bad_html)[*bhp-1].e = html;
		goto scan_done;
	}
	if (t_namelen == 8 && !casecmp(t_name, cast_uchar "COLGROUP", 8)) {
		if (c_span) new_columns(t, c_span, c_width, c_al, c_val, 1);
		if (lbhp) (*bad_html)[*bhp-1].e = html, lbhp = NULL;
		c_al = AL_TR;
		c_val = VAL_TR;
		c_width = W_AUTO;
		get_align(t_attr, &c_al);
		get_valign(t_attr, &c_val);
		get_c_width(t_attr, &c_width, sh);
		if ((c_span = get_num(t_attr, cast_uchar "span")) == -1) c_span = 1;
		goto see;
	}
	if (t_namelen == 9 && !casecmp(t_name, cast_uchar "/COLGROUP", 9)) {
		if (c_span) new_columns(t, c_span, c_width, c_al, c_val, 1);
		if (lbhp) (*bad_html)[*bhp-1].e = html, lbhp = NULL;
		c_span = 0;
		c_al = AL_TR;
		c_val = VAL_TR;
		c_width = W_AUTO;
		goto see;
	}
	if (t_namelen == 3 && !casecmp(t_name, cast_uchar "COL", 3)) {
		int sp, wi, al, val;
		if (lbhp) (*bad_html)[*bhp-1].e = html, lbhp = NULL;
		if ((sp = get_num(t_attr, cast_uchar "span")) == -1) sp = 1;
		wi = c_width;
		al = c_al;
		val = c_val;
		get_align(t_attr, &al);
		get_valign(t_attr, &val);
		get_c_width(t_attr, &wi, sh);
		new_columns(t, sp, wi, al, val, !!c_span);
		c_span = 0;
		goto see;
	}
	if (t_namelen == 3 && (!casecmp(t_name, cast_uchar "/TR", 3) || !casecmp(t_name, cast_uchar "/TD", 3) || !casecmp(t_name, cast_uchar "/TH", 3))) {
		if (c_span) new_columns(t, c_span, c_width, c_al, c_val, 1);
		if (p) CELL(t, x, y)->end = html, p = 0;
		if (lbhp) (*bad_html)[*bhp-1].e = html, lbhp = NULL;
	}
	if (t_namelen == 2 && !casecmp(t_name, cast_uchar "TR", 2)) {
		if (c_span) new_columns(t, c_span, c_width, c_al, c_val, 1);
		if (p) CELL(t, x, y)->end = html, p = 0;
		if (lbhp) (*bad_html)[*bhp-1].e = html, lbhp = NULL;
		if (group) group--;
		l_al = default_line_align();
		l_val = VAL_MIDDLE;
		memcpy(&l_col, bgcolor, sizeof(struct rgb));
		get_align(t_attr, &l_al);
		get_valign(t_attr, &l_val);
		get_bgcolor(t_attr, &l_col);
		y++, x = 0;
		goto see;
	}
	if (t_namelen == 5 && ((!casecmp(t_name, cast_uchar "THEAD", 5)) || (!casecmp(t_name, cast_uchar "TBODY", 5)) || (!casecmp(t_name, cast_uchar "TFOOT", 5)))) {
		if (c_span) new_columns(t, c_span, c_width, c_al, c_val, 1);
		if (lbhp) (*bad_html)[*bhp-1].e = html, lbhp = NULL;
		group = 2;
	}
	if (t_namelen != 2 || (casecmp(t_name, cast_uchar "TD", 2) && casecmp(t_name, cast_uchar "TH", 2))) goto see;
	if (c_span) new_columns(t, c_span, c_width, c_al, c_val, 1);
	if (lbhp) (*bad_html)[*bhp-1].e = html, lbhp = NULL;
	if (p) CELL(t, x, y)->end = html, p = 0;
	if (y == -1) y = 0, x = 0;
	nc:
	if (!(cell = new_cell(t, x, y))) goto see;
	if (cell->used) {
		if (cell->colspan == -1) goto see;
		x++;
		goto nc;
	}
	cell->mx = x;
	cell->my = y;
	cell->used = 1;
	cell->start = en;
	p = 1;
	cell->align = l_al;
	cell->valign = l_val;
	cell->b = 0;
#if 0
	if (upcase(t_name[1]) == 'H') {
		unsigned char *e = en;
		while (e < eof && WHITECHAR(*e)) e++;
		if (eof - e > 6 && !casecmp(e, cast_uchar "<TABLE", 6)) goto no_th; /* hack for www.root.cz */
		cell->b = 1;
		cell->align = AL_CENTER;
		no_th:;
	}
#endif
	if (group == 1) cell->group = 1;
	if (x < t->c) {
		if (t->cols[x].align != AL_TR) cell->align = t->cols[x].align;
		if (t->cols[x].valign != VAL_TR) cell->valign = t->cols[x].valign;
	}
	memcpy(&cell->bgcolor, &l_col, sizeof(struct rgb));
	get_align(t_attr, &cell->align);
	get_valign(t_attr, &cell->valign);
	get_bgcolor(t_attr, &cell->bgcolor);
	if ((a = get_attr_val(t_attr, cast_uchar "id")))
		cell->tag = a;
	if ((a = get_attr_val(t_attr, cast_uchar "class"))) {
		if (!strncmp(cast_const_char a, "blob-code", 9) ||	/* github hack */
		    !strcmp(cast_const_char a, "changelog") ||
		    !strcmp(cast_const_char a, "vc_file_line_text") || /* https://gcc.gnu.org/viewcvs/gcc/trunk/gcc/opts.c?view=markup */
		    0) {
			cell->align = !par_format.implicit_pre_wrap ? AL_NO : AL_NO_BREAKABLE;
		}
		mem_free(a);
	}
#ifdef G
	sprintf(cast_char cell->bgcolor_str, "#%02x%02x%02x", cell->bgcolor.r & 0xff, cell->bgcolor.g & 0xff, cell->bgcolor.b & 0xff);
#endif
	if ((csp = get_num(t_attr, cast_uchar "colspan")) == -1) csp = 1;
	if (!csp) csp = -1;
	if ((rsp = get_num(t_attr, cast_uchar "rowspan")) == -1) rsp = 1;
	if (!rsp) rsp = -1;
	if (csp >= 0 && rsp >= 0 && csp * rsp > 100000) {
		if (csp > 10) csp = -1;
		if (rsp > 10) rsp = -1;
	}
	cell->colspan = csp;
	cell->rowspan = rsp;
	if (csp == 1) {
		int w = W_AUTO;
		get_c_width(t_attr, &w, sh);
		if (w != W_AUTO) set_td_width(t, x, w, 0);
	}
	qqq = t->x;
	for (i = 1; csp != -1 ? i < csp : x + i < qqq; i++) {
		struct table_cell *sc;
		if (!(sc = new_cell(t, x + i, y)) || sc->used) {
			csp = i;
			for (k = 0; k < i; k++) CELL(t, x + k, y)->colspan = csp;
			break;
		}
		sc->used = sc->spanned = 1;
		sc->rowspan = rsp;
		sc->colspan = csp;
		sc->mx = x;
		sc->my = y;
	}
	qqq = t->y;
	for (j = 1; rsp != -1 ? j < rsp : y + j < qqq; j++) {
		for (k = 0; k < i; k++) {
			struct table_cell *sc;
			if (!(sc = new_cell(t, x + k, y + j)) || sc->used) {
				int l, m;
				if (sc->mx == x && sc->my == y) continue;
				/*internal("boo");*/
				for (l = 0; l < k; l++) memset(CELL(t, x + l, y + j), 0, sizeof(struct table_cell));
				rsp = j;
				for (l = 0; l < i; l++) for (m = 0; m < j; m++) CELL(t, x + l, y + m)->rowspan = j;
				goto brk;
			}
			sc->used = sc->spanned = 1;
			sc->rowspan = rsp;
			sc->colspan = csp;
			sc->mx = x;
			sc->my = y;
		}
	}
	brk:
	goto see;

	scan_done:
	*end = html;

	for (x = 0; x < t->x; x++) for (y = 0; y < t->y; y++) {
		struct table_cell *c = CELL(t, x, y);
		if (!c->spanned) {
			if (c->colspan == -1) c->colspan = t->x - x;
			if (c->rowspan == -1) c->rowspan = t->y - y;
		}
	}

	if ((unsigned)t->y > MAXINT / sizeof(int)) overalloc();
	t->r_heights = mem_calloc(t->y * sizeof(int));

	for (x = 0; x < t->c; x++) if (t->cols[x].width != W_AUTO) set_td_width(t, x, t->cols[x].width, 1);
	set_td_width(t, t->x, W_AUTO, 0);

	return t;
}

static void get_cell_width(struct table *t, struct table_cell *c, int w, int a, int *min, int *max, int *n_links)
{
	if (min) *min = -1;
	if (max) *max = -1;
	if (n_links) *n_links = c->link_num;
	if (!F) {
		struct part *p;
		if (!(p = format_html_part(c->start, c->end, c->align == AL_NO || c->align == AL_NO_BREAKABLE ? c->align : AL_LEFT, t->cellpd, w, NULL, !!a, !!a, NULL, c->link_num))) return;
		if (min) *min = p->x;
		if (max) *max = p->xmax;
		if (n_links) *n_links = p->link_num;
		mem_free(p);
#ifdef G
	} else {
		struct g_part *gp;
		if (!(gp = g_format_html_part(c->start, c->end, c->align == AL_NO || c->align == AL_NO_BREAKABLE ? c->align : AL_LEFT, 0, w, NULL, c->link_num, NULL, c->bgcolor_str, NULL))) return;
		if (min) *min = gp->x;
		if (max) *max = gp->xmax;
		if (n_links) *n_links = gp->link_num;
		mem_free(gp);
#endif
	}
	/*debug("get_cell_width: %d < %d", *min, *max);*/
	/*if (min && max && *min > *max) internal("get_cell_width: %d > %d", *min, *max);*/
}

#define g_c_w(cc)							\
do {									\
		struct table_cell *c = cc;				\
		if (!c->start) continue;				\
		c->link_num = nl;					\
		get_cell_width(t, c, 0, 0, &c->min_width, &c->max_width, &nl);\
} while (0)

static void get_cell_widths(struct table *t)
{
	int nl = gf_val(t->p->link_num, t->gp->link_num);
	int i, j;
	if (!d_opt->table_order)
		for (j = 0; j < t->y; j++) for (i = 0; i < t->x; i++) g_c_w(CELL(t, i, j));
	else
		for (i = 0; i < t->x; i++) for (j = 0; j < t->y; j++) g_c_w(CELL(t, i, j));
	t->link_num = nl;
}

static void dst_width(int *p, int n, int w, int *lim)
{
	int i, s = 0, d, r;
	for (i = 0; i < n; i++) s = safe_add(s, p[i]);
	if (s >= w) return;
	if (!n) return;
	again:
	d = (w - s) / n;
	r = (w - s) % n;
	w = 0;
	for (i = 0; i < n; i++) {
		p[i] = safe_add(p[i], d + (i < r));
		if (lim && p[i] > lim[i]) w = safe_add(w, p[i] - lim[i]), p[i] = lim[i];
	}
	if (w) {
		/*if (!lim) internal("bug in dst_width");*/
		lim = NULL;
		s = 0;
		goto again;
	}
}

static int get_vline_width(struct table *t, int col)
{			/* return: -1 none, 0, space, 1 line, 2 double */
	int w = 0;
	NO_GFX;
	if (!col) return -1;
	if (t->rules == R_COLS || t->rules == R_ALL) w = t->cellsp;
	else if (t->rules == R_GROUPS) w = col < t->c && t->cols[col].group;
	if (!w && t->cellpd) w = -1;
	return w;
}

static int get_hline_width(struct table *t, int row)
{
	int w = 0;
	NO_GFX;
	if (!row) return -1;
	if (t->rules == R_ROWS || t->rules == R_ALL) {
		x:
		if (t->cellsp || t->vcellpd) return t->cellsp;
		return -1;
	}
	else if (t->rules == R_GROUPS) {
		int q;
		for (q = 0; q < t->x; q++) if (CELL(t, q, row)->group) goto x;
		return t->vcellpd ? 0 : -1;
	}
	if (!w && !t->vcellpd) w = -1;
	return w;
}

#ifdef G
static int g_get_vline_pad(struct table *t, int col, int *plpos, int *plsize)
{
	int pad, lpos, lsize;
	int border;
	NO_TXT;
	if (!col || col == t->x) {
		border = (!col && t->frame & F_LHS) || (col == t->x && t->frame & F_RHS) ? t->border : 0;
		pad = safe_add(safe_add(border, t->cellsp), t->cellpd);
		if (!col) lpos = 0, lsize = border + t->cellsp;
		else lpos = pad - border - t->cellsp, lsize = border + t->cellsp;
	} else {
		border = t->rules == R_COLS || t->rules == R_ALL || (t->rules == R_GROUPS && col < t->c && t->cols[col].group) ? t->border : 0;
		pad = safe_add(safe_add(t->cellpd, t->cellpd), t->cellsp);
		lpos = t->cellpd;
		lsize = t->cellsp;
	}
	if (!border) {
		lsize = 0;
		if (!col) lpos = 0;
		else if (col == t->x) lpos = pad;
		else lpos = pad / 2;
	}
	if (plpos) *plpos = lpos;
	if (plsize) *plsize = lsize;
	return pad;
}

static int g_get_hline_pad(struct table *t, int row, int *plpos, int *plsize)
{
	int pad, lpos, lsize;
	int border;
	NO_TXT;
	if (!row || row == t->y) {
		border = (!row && t->frame & F_ABOVE) || (row == t->y && t->frame & F_BELOW) ? t->border : 0;
		pad = safe_add(safe_add(border, t->cellsp), t->cellpd);
		if (!row) lpos = 0, lsize = border + t->cellsp;
		else lpos = pad - border - t->cellsp, lsize = border + t->cellsp;
	} else {
		border = t->rules == R_ROWS || t->rules == R_ALL ? t->border : 0;
		if (t->rules == R_GROUPS) {
			int q;
			for (q = 0; q < t->x; q++) if (CELL(t, q, row)->group) {
				border = t->border;
				break;
			}
		}
		pad = safe_add(safe_add(t->cellpd, t->cellpd), t->cellsp);
		lpos = t->cellpd;
		lsize = t->cellsp;
	}
	if (!border) {
		lsize = 0;
		if (!row) lpos = 0;
		else if (row == t->y) lpos = pad;
		else lpos = pad / 2;
	}
	if (plpos) *plpos = lpos;
	if (plsize) *plsize = lsize;
	return pad;
}
#endif

static int get_column_widths(struct table *t)
{
	int i, j, s, ns;
	if ((unsigned)t->x > MAXINT / sizeof(int)) overalloc();
	if (!t->min_c) t->min_c = mem_alloc(t->x * sizeof(int));
	if (!t->max_c) t->max_c = mem_alloc(t->x * sizeof(int));
	if (!t->w_c) t->w_c = mem_alloc(t->x * sizeof(int));
	memset(t->min_c, 0, t->x * sizeof(int));
	memset(t->max_c, 0, t->x * sizeof(int));
	s = 1;
	do {
		ns = MAXINT;
		for (i = 0; i < t->x; i++) for (j = 0; j < t->y; j++) {
			struct table_cell *c = CELL(t, i, j);
			if (c->spanned || !c->used) continue;
			if (c->colspan + i > t->x) {
				/*internal("colspan out of table");
				return -1;*/
				continue;
			}
			if (c->colspan == s) {
				int k, p = 0;
				/*int pp = t->max_c[i];*/
				int m = 0;
				for (k = 1; k < s; k++) {
					if (!F) {
						if (get_vline_width(t, i + k) >= 0)
							p = safe_add(p, 1);
					}
#ifdef G
					else {
						int qw = g_get_vline_pad(t, i + k, NULL, NULL);
						p = safe_add(p, qw);
					}
#endif
				}
				dst_width(t->min_c + i, s, c->min_width - p, t->max_c + i);
				dst_width(t->max_c + i, s, c->max_width - p - m, NULL);
				for (k = 0; k < s; k++) if (t->min_c[i + k] > t->max_c[i + k]) t->max_c[i + k] = t->min_c[i + k];
			} else if (c->colspan > s && c->colspan < ns) ns = c->colspan;
		}
	} while ((s = ns) != MAXINT);
	return 0;
}

static void get_table_width(struct table *t)
{
	int i, vl;
	int min = 0, max = 0;
	for (i = 0; i < t->x; i++) {
		if (!F) vl = get_vline_width(t, i) >= 0;
#ifdef G
		else if (i) {
			vl = g_get_vline_pad(t, i, NULL, NULL);
		} else vl = 0;
#endif
		min = safe_add(min, vl), max = safe_add(max, vl);
		min = safe_add(min, t->min_c[i]);
		if (t->xcols[i] > t->max_c[i]) max = safe_add(max, t->xcols[i]);
		max = safe_add(max, t->max_c[i]);
	}
	if (!F) {
		vl = (!!(t->frame & F_LHS) + !!(t->frame & F_RHS)) * !!t->border;
		min = safe_add(min, vl), max = safe_add(max, vl);
#ifdef G
	} else {
		vl = g_get_vline_pad(t, 0, NULL, NULL);
		min = safe_add(min, vl), max = safe_add(max, vl);
		vl = g_get_vline_pad(t, t->x, NULL, NULL);
		min = safe_add(min, vl), max = safe_add(max, vl);
#endif
	}
	t->min_t = min;
	t->max_t = max;
	/*if (min > max) internal("min(%d) > max(%d)", min, max);*/
}

static void distribute_widths(struct table *t, int width)
{
	int i;
	int d = width - t->min_t;
	int om = 0;
	unsigned char *u;
	int *w, *mx;
	int mmax_c = 0;
	t->rw = 0;
	if (!t->x) return;
	if (d < 0) {
		/*internal("too small width %d, required %d", width, t->min_t);*/
		return;
	}
	for (i = 0; i < t->x; i++) if (t->max_c[i] > mmax_c) mmax_c = t->max_c[i];
	memcpy(t->w_c, t->min_c, t->x * sizeof(int));
	t->rw = width;
	if ((unsigned)t->x > MAXINT / sizeof(int)) overalloc();
	u = mem_alloc(t->x);
	w = mem_alloc(t->x * sizeof(int));
	mx = mem_alloc(t->x * sizeof(int));
	while (d) {
		int mss, mii;
		int p = 0;
		int wq;
		int dd;
		memset(w, 0, t->x * sizeof(int));
		memset(mx, 0, t->x * sizeof(int));
		for (i = 0; i < t->x; i++) {
			switch (om) {
				case 0:
					if (t->w_c[i] < t->xcols[i]) {
						w[i] = 1, mx[i] = (t->xcols[i] > t->max_c[i] ? t->max_c[i] : t->xcols[i]) - t->w_c[i];
						if (mx[i] <= 0) w[i] = 0;
					}
					break;
				case 1:
					if (t->xcols[i] < -1 && t->xcols[i] != -2) {
						w[i] = t->xcols[i] <= -2 ? -2 - t->xcols[i] : 1;
						mx[i] = t->max_c[i] - t->w_c[i];
						if (mx[i] <= 0) w[i] = 0;
					}
					break;
				case 2:
				case 3:
					if (t->w_c[i] < t->max_c[i] && (om == 3 || t->xcols[i] == W_AUTO)) {
						mx[i] = t->max_c[i] - t->w_c[i];
						if (mmax_c) w[i] = safe_add(gf_val(5, 5 * HTML_CHAR_WIDTH), t->max_c[i] * 10 / mmax_c);
						else w[i] = 1;
					}
					break;
				case 4:
					if (t->xcols[i] >= 0) {
						w[i] = 1, mx[i] = t->xcols[i] - t->w_c[i];
						if (mx[i] <= 0) w[i] = 0;
					}
					break;
				case 5:
					if (t->xcols[i] < 0) w[i] = t->xcols[i] <= -2 ? -2 - t->xcols[i] : 1, mx[i] = MAXINT;
					break;
				case 6:
					w[i] = 1, mx[i] = MAXINT;
					break;
				default:
					/*internal("could not expand table");*/
					goto end2;
			}
			p = safe_add(p, w[i]);
		}
		if (!p) {
			om++;
			continue;
		}
		wq = 0;
		if (u) memset(u, 0, t->x);
		dd = d;
		a:
		mss = 0; mii = -1;
		for (i = 0; i < t->x; i++) if (w[i]) {
			int ss;
			if (u && u[i]) continue;
			if (!(ss = dd * w[i] / p)) ss = 1;
			if (ss > mx[i]) ss = mx[i];
			if (ss > mss) mss = ss, mii = i;
		}
		if (mii != -1) {
			int q = t->w_c[mii];
			if (u) u[mii] = 1;
			t->w_c[mii] = safe_add(t->w_c[mii], mss);
			d -= t->w_c[mii] - q;
			while (d < 0) t->w_c[mii]--, d++;
			if (t->w_c[mii] < q) {
				/*internal("shrinking cell");*/
				t->w_c[mii] = q;
			}
			wq = 1;
			if (d) goto a;
		} else if (!wq) om++;
	}
	end2:
	mem_free(mx);
	mem_free(w);
	if (u) mem_free(u);
}

#ifdef HTML_TABLE_2ND_PASS
static void check_table_widths(struct table *t)
{
	int *w;
	int i, j;
	int s, ns;
	int m, mi = 0; /* go away, warning! */
	if ((unsigned)t->x > MAXINT / sizeof(int)) overalloc();
	w = mem_calloc(t->x * sizeof(int));
	for (j = 0; j < t->y; j++) for (i = 0; i < t->x; i++) {
		struct table_cell *c = CELL(t, i, j);
		int k, p = 0;
		if (!c->start) continue;
		for (k = 1; k < c->colspan; k++) if (get_vline_width(t, i + k) >= 0) p = safe_add(p, 1);
		for (k = 0; k < c->colspan; k++) p = safe_add(p, t->w_c[i + k]);
		get_cell_width(t, c, p, 1, &c->x_width, NULL, NULL);
		if (c->x_width > p) {
			/*int min, max;
			get_cell_width(t, c, 0, 0, &min, &max, NULL);
			internal("cell is now wider (%d > %d) min = %d, max = %d, now_min = %d, now_max = %d", c->x_width, p, t->min_c[i], t->max_c[i], min, max);*/
			/* sbohem, internale. chytl jsi mi spoustu chyb v tabulkovaci, ale ted je proste cas jit ... ;-( */
			c->x_width = p;
		}
	}
	s = 1;
	do {
		ns = MAXINT;
		for (i = 0; i < t->x; i++) for (j = 0; j < t->y; j++) {
			struct table_cell *c = CELL(t, i, j);
			if (!c->start) continue;
			if (c->colspan + i > t->x) {
				/*internal("colspan out of table");*/
				mem_free(w);
				return;
			}
			if (c->colspan == s) {
				int k, p = 0;
				for (k = 1; k < s; k++) if (get_vline_width(t, i + k) >= 0) p = safe_add(p, 1);
				dst_width(w + i, s, c->x_width - p, t->max_c + i);
				/*for (k = i; k < i + s; k++) if (w[k] > t->w_c[k]) {
					int l;
					int c;
					ag:
					c = 0;
					for (l = i; l < i + s; l++) if (w[l] < t->w_c[k]) w[l] = safe_add(w[l], 1), w[k]--, c = 1;
					if (w[k] > t->w_c[k]) {
						if (!c) internal("can't shrink cell");
						else goto ag;
					}
				}*/
			} else if (c->colspan > s && c->colspan < ns) ns = c->colspan;
		}
	} while ((s = ns) != MAXINT);

	s = 0; ns = 0;
	for (i = 0; i < t->x; i++) {
		s = safe_add(s, t->w_c[i]), ns = safe_add(ns, w[i]);
		/*if (w[i] > t->w_c[i]) {
			int k;
			for (k = 0; k < t->x; k++) debug("%d, %d", t->w_c[k], w[k]);
			debug("column %d: new width(%d) is larger than previous(%d)", i, w[i], t->w_c[i]);
		}*/
	}
	if (ns > s) {
		/*internal("new width(%d) is larger than previous(%d)", ns, s);*/
		mem_free(w);
		return;
	}
	m = -1;
	for (i = 0; i < t->x; i++) {
		/*if (table_level == 1) debug("%d: %d %d %d %d", i, t->max_c[i], t->min_c[i], t->w_c[i], w[i]);*/
		if (t->max_c[i] > m) m = t->max_c[i], mi = i;
	}
	/*if (table_level == 1) debug("%d %d", mi, s - ns);*/
	if (m != -1) {
		w[mi] = safe_add(w[mi], s - ns);
		if (w[mi] <= t->max_c[mi]) {
			mem_free(t->w_c);
			t->w_c = w;
			return;
		}
	}
	mem_free(w);
}
#endif

static void get_table_heights(struct table *t)
{
	int s, ns;
	int i, j;
	for (j = 0; j < t->y; j++) {
		for (i = 0; i < t->x; i++) {
			struct table_cell *cell = CELL(t, i, j);
			struct part *p;
#ifdef G
			struct g_part *gp;
#endif
			int xw = 0, sp;
			if (!cell->used || cell->spanned) continue;
			/*
			fprintf(stderr, "i==%d, w_c[i]==%d, min_c[i]==%d, max_c[i]==%d\n", i, t->w_c[i], t->min_c[i], t->max_c[i]);
			*/
			for (sp = 0; sp < cell->colspan; sp++) {
				xw = safe_add(xw, t->w_c[i + sp]);
				if (sp < cell->colspan - 1) {
					if (!F) {
						if (get_vline_width(t, i + sp + 1) >= 0)
							xw = safe_add(xw, 1);
					}
#ifdef G
					else {
						int qw = g_get_vline_pad(t, i + sp + 1, NULL, NULL);
						xw = safe_add(xw, qw);
					}
#endif
				}
			}
			if (!F) {
				if (!(p = format_html_part(cell->start, cell->end, cell->align, t->cellpd, xw, NULL, 2, 2, NULL, cell->link_num))) return;
				cell->height = p->y;
				mem_free(p);
#ifdef G
			} else {
				if (!(gp = g_format_html_part(cell->start, cell->end, cell->align, 0, xw, NULL, cell->link_num, NULL /* FIX: background image */, cell->bgcolor_str, t->gp->data))) return;
				/*if (gp->root->xw > xw) internal("vono to neumi formatovat... buuuu (%d > %d)", gp->root->xw, xw);*/
				cell->root = gp->root;
				gp->root = NULL;
				cell->height = gp->y;
				cell->g_width = xw;
				g_release_part(gp);
				mem_free(gp);
#endif
			}
				/*debug("%d, %d.",xw, cell->height);*/
		}
	}
	s = 1;
	do {
		ns = MAXINT;
		for (j = 0; j < t->y; j++) {
			for (i = 0; i < t->x; i++) {
				struct table_cell *cell = CELL(t, i, j);
				if (!cell->used || cell->spanned) continue;
				if (cell->rowspan == s) {
					int k, p = 0;
					for (k = 1; k < s; k++) {
						if (!F) {
							if (get_hline_width(t, j + k) >= 0)
								p = safe_add(p, 1);
						}
#ifdef G
						else {
							int qw = g_get_hline_pad(t, j + k, NULL, NULL);
							p = safe_add(p, qw);
						}
#endif
					}
					dst_width(t->r_heights + j, s, cell->height - p, NULL);
				} else if (cell->rowspan > s && cell->rowspan < ns) ns = cell->rowspan;
			}
		}
	} while ((s = ns) != MAXINT);
	if (!F) {
		t->rh = (!!(t->frame & F_ABOVE) + !!(t->frame & F_BELOW)) * !!t->border;
		for (j = 0; j < t->y; j++) {
			t->rh = safe_add(t->rh, t->r_heights[j]);
			if (j && get_hline_width(t, j) >= 0) t->rh = safe_add(t->rh, 1);
		}
#ifdef G
	} else {
		t->rh = 0;
		for (j = 0; j <= t->y; j++) {
			int qw = g_get_hline_pad(t, j, NULL, NULL);
			t->rh = safe_add(t->rh, qw);
			if (j < t->y) t->rh = safe_add(t->rh, t->r_heights[j]);
		}
#endif
	}
}

static void display_complicated_table(struct table *t, int x, int y, int *yy)
{
	int i, j;
	struct f_data *f = t->p->data;
	int yp, xp = safe_add(x, (t->frame & F_LHS) && t->border);
	for (i = 0; i < t->x; i++) {
		yp = safe_add(y, (t->frame & F_ABOVE) && t->border);
		for (j = 0; j < t->y; j++) {
			struct table_cell *cell = CELL(t, i, j);
			if (cell->start) {
				int yt;
				struct part *p = NULL;
				int xw = 0, yw = 0, s;
				for (s = 0; s < cell->colspan; s++) {
					xw = safe_add(xw, t->w_c[i + s]);
					if (s < cell->colspan - 1) {
						if (get_vline_width(t, i + s + 1) >= 0)
							xw = safe_add(xw, 1);
					}
				}
				for (s = 0; s < cell->rowspan; s++) {
					yw += t->r_heights[j + s];
					if (s < cell->rowspan - 1) {
						if (get_hline_width(t, j + s + 1) >= 0)
							yw = safe_add(yw, 1);
					}
				}
				html_stack_dup();
				html_top.dontkill = 1;
				if (cell->b) format_.attr |= AT_BOLD;
				memcpy(&format_.bg, &cell->bgcolor, sizeof(struct rgb));
				memcpy(&par_format.bgcolor, &cell->bgcolor, sizeof(struct rgb));
				if (cell->tag) {
					if (f) html_tag(f, cell->tag, safe_add(t->p->xp, xp), safe_add(t->p->yp, yp));
				}
				p = format_html_part(cell->start, cell->end, cell->align, t->cellpd, xw, f, safe_add(t->p->xp, xp), safe_add(safe_add(t->p->yp, yp), cell->valign != VAL_MIDDLE && cell->valign != VAL_BOTTOM ? 0 : (yw - cell->height) / (cell->valign == VAL_MIDDLE ? 2 : 1)), NULL, cell->link_num);
				cell->xpos = xp;
				cell->ypos = yp;
				cell->xw = xw;
				cell->yw = yw;
				for (yt = 0; yt < p->y; yt++) {
					xxpand_lines(t->p, safe_add(yp, yt));
					xxpand_line(t->p, yp + yt, safe_add(xp, t->w_c[i]));
				}
				kill_html_stack_item(&html_top);
				mem_free(p);
			}
			cell->xpos = xp;
			cell->ypos = yp;
			cell->xw = t->w_c[i];
			yp = safe_add(yp, t->r_heights[j]);
			if (j < t->y - 1) if (get_hline_width(t, j + 1) >= 0) yp = safe_add(yp, 1);
		}
		if (i < t->x - 1) {
			if (get_vline_width(t, i + 1) >= 0)
				xp = safe_add(xp, 1);
			xp = safe_add(xp, t->w_c[i]);
		}
	}
	yp = y;
	for (j = 0; j < t->y; j++) {
		yp = safe_add(yp, t->r_heights[j]);
		if (j < t->y - 1) if (get_hline_width(t, j + 1) >= 0) yp = safe_add(yp, 1);
	}
	*yy = safe_add(yp, (!!(t->frame & F_ABOVE) + !!(t->frame & F_BELOW)) * !!t->border);
}

static unsigned char AF;

#define draw_frame_point(xx, yy, ii, jj)	\
if (H_LINE_X((ii-1), (jj)) >= 0 || H_LINE_X((ii), (jj)) >= 0 || V_LINE_X((ii), (jj-1)) >= 0 || V_LINE_X((ii), (jj)) >= 0) xset_hchar(t->p, (xx), (yy), frame_table[V_LINE((ii),(jj)-1)+3*H_LINE((ii),(jj))+9*H_LINE((ii)-1,(jj))+27*V_LINE((ii),(jj))], AF)

#define draw_frame_hline(xx, yy, ll, ii, jj)	\
if (H_LINE_X((ii), (jj)) >= 0) xset_hchars(t->p, (xx), (yy), (ll), hline_table[H_LINE((ii), (jj))], AF)

#define draw_frame_vline(xx, yy, ll, ii, jj)	\
{						\
	int qq;					\
	if (V_LINE_X((ii), (jj)) >= 0) for (qq = 0; qq < (ll); qq++) xset_hchar(t->p, (xx), safe_add((yy), qq), vline_table[V_LINE((ii), (jj))], AF); }

#ifndef DEBUG
#define H_LINE_X(xx, yy) fh[(xx) + 1 + (t->x + 2) * (yy)]
#define V_LINE_X(xx, yy) fv[(yy) + 1 + (t->y + 2) * (xx)]
#else
#define H_LINE_X(xx, yy) (*((xx) < -1 || (xx) > t->x + 1 || (yy) < 0 || (yy) > t->y ? (short *)NULL : &fh[(xx) + 1 + (t->x + 2) * (yy)]))
#define V_LINE_X(xx, yy) (*((xx) < 0 || (xx) > t->x || (yy) < -1 || (yy) > t->y + 1 ? (short *)NULL : &fv[(yy) + 1 + (t->y + 2) * (xx)]))
#endif
#define H_LINE(xx, yy) (H_LINE_X((xx), (yy)) < 0 ? 0 : H_LINE_X((xx), (yy)))
#define V_LINE(xx, yy) (V_LINE_X((xx), (yy)) < 0 ? 0 : V_LINE_X((xx), (yy)))

static void get_table_frame(struct table *t, short *fv, short *fh)
{
	int i, j;
	memset(fh, -1, (t->x + 2) * (t->y + 1) * sizeof(short));
	memset(fv, -1, (t->x + 1) * (t->y + 2) * sizeof(short));
	for (j = 0; j < t->y; j++) for (i = 0; i < t->x; i++) {
		int x, y;
		int xsp, ysp;
		struct table_cell *cell = CELL(t, i, j);
		if (!cell->used || cell->spanned) continue;
		if ((xsp = cell->colspan) == 0) xsp = t->x - i;
		if ((ysp = cell->rowspan) == 0) ysp = t->y - j;
		if (t->rules != R_NONE && t->rules != R_COLS) for (x = 0; x < xsp; x++) {H_LINE_X(i + x, j) = t->cellsp; H_LINE_X(i + x, j + ysp) = t->cellsp;}
		if (t->rules != R_NONE && t->rules != R_ROWS) for (y = 0; y < ysp; y++) {V_LINE_X(i, j + y) = t->cellsp; V_LINE_X(i + xsp, j + y) = t->cellsp;}
		if (F) {
			for (x = 0; x < xsp; x++) for (y = 1; y < ysp; y++)
				H_LINE_X(i + x, j + y) = -2;
			for (x = 1; x < xsp; x++) for (y = 0; y < ysp; y++)
				V_LINE_X(i + x, j + y) = -2;
		}
	}
	if (t->rules == R_GROUPS) {
		for (i = 1; i < t->x; i++) {
			if (/*i < t->xc &&*/ t->xcols[i]) continue;
			for (j = 0; j < t->y; j++) V_LINE_X(i, j) = 0;
		}
		for (j = 1; j < t->y; j++) {
			for (i = 0; i < t->x; i++) if (CELL(t, i, j)->group) goto c;
			for (i = 0; i < t->x; i++) H_LINE_X(i, j) = 0;
			c:;
		}
	}
	for (i = 0; i < t->x; i++) {
		H_LINE_X(i, 0) = t->border * !!(t->frame & F_ABOVE);
		H_LINE_X(i, t->y) = t->border * !!(t->frame & F_BELOW);
	}
	for (j = 0; j < t->y; j++) {
		V_LINE_X(0, j) = t->border * !!(t->frame & F_LHS);
		V_LINE_X(t->x, j) = t->border * !!(t->frame & F_RHS);
	}
}

static void display_table_frames(struct table *t, int x, int y)
{
	short *fh, *fv;
	int i, j;
	int cx, cy;
	if ((unsigned)t->x > MAXINT) overalloc();
	if ((unsigned)t->y > MAXINT) overalloc();
	if (((unsigned)t->x + 2) * ((unsigned)t->y + 2) / ((unsigned)t->x + 2) != ((unsigned)t->y + 2)) overalloc();
	if (((unsigned)t->x + 2) * ((unsigned)t->y + 2) > MAXINT) overalloc();
	fh = mem_alloc((t->x + 2) * (t->y + 1) * sizeof(short));
	fv = mem_alloc((t->x + 1) * (t->y + 2) * sizeof(short));
	get_table_frame(t, fv, fh);

	cy = y;
	for (j = 0; j <= t->y; j++) {
		cx = x;
		if ((j > 0 && j < t->y && get_hline_width(t, j) >= 0) || (j == 0 && t->border && (t->frame & F_ABOVE)) || (j == t->y && t->border && (t->frame & F_BELOW))) {
			for (i = 0; i < t->x; i++) {
				int w;
				if (i > 0) w = get_vline_width(t, i);
				else w = t->border && (t->frame & F_LHS) ? t->border : -1;
				if (w >= 0) {
					draw_frame_point(cx, cy, i, j);
					if (j < t->y) draw_frame_vline(cx, safe_add(cy, 1), t->r_heights[j], i, j);
					cx = safe_add(cx, 1);
				}
				w = t->w_c[i];
				draw_frame_hline(cx, cy, w, i, j);
				cx = safe_add(cx, w);
			}
			if (t->border && (t->frame & F_RHS)) {
				draw_frame_point(cx, cy, i, j);
				if (j < t->y) draw_frame_vline(cx, safe_add(cy, 1), t->r_heights[j], i, j);
				cx = safe_add(cx, 1);
			}
			cy = safe_add(cy, 1);
		} else if (j < t->y) {
			for (i = 0; i <= t->x; i++) {
				if ((i > 0 && i < t->x && get_vline_width(t, i) >= 0) || (i == 0 && t->border && (t->frame & F_LHS)) || (i == t->x && t->border && (t->frame & F_RHS))) {
					draw_frame_vline(cx, cy, t->r_heights[j], i, j);
					cx = safe_add(cx, 1);
				}
				if (i < t->x) cx = safe_add(cx, t->w_c[i]);
			}
		}
		if (j < t->y) cy = safe_add(cy, t->r_heights[j]);
		/*for (cyy = cy1; cyy < cy; cyy++) xxpand_line(t->p, cyy, cx - 1);*/
	}
	mem_free(fh);
	mem_free(fv);
}

#ifdef G
static void process_g_table(struct g_part *gp, struct table *t);
#endif

void format_table(unsigned char *attr, unsigned char *html, unsigned char *eof, unsigned char **end, void *f)
{
	struct part *p = !F ? f : NULL;
#ifdef G
	struct g_part *gp = F ? f : NULL;
#endif
	int border, cellsp, vcellpd, cellpd, align;
	int frame, rules, width, wf;
	struct rgb bgcolor;
	struct table *t;
	unsigned char *al;
	int cye;
	int x;
	int i;
	struct s_e *bad_html = NULL;
	int bad_html_n;
	struct node *n, *nn;
	int cpd_pass, cpd_width, cpd_last;
	unsigned char AF_SAVE = AF;
	table_level++;
	memcpy(&bgcolor, &par_format.bgcolor, sizeof(struct rgb));
	get_bgcolor(attr, &bgcolor);
	if (!F) {
		int bg = find_nearest_color(&bgcolor, 8);
		int fg = find_nearest_color(&d_opt->default_fg, 16);
		/*fg = fg_color(fg, bg);*/
		AF = ATTR_FRAME | get_attribute(fg, bg);
	}
	html_stack_dup();
	html_top.dontkill = 1;
	par_format.align = AL_LEFT;
	if ((border = get_num(attr, cast_uchar "border")) == -1) border = has_attr(attr, cast_uchar "border") || has_attr(attr, cast_uchar "rules") || has_attr(attr, cast_uchar "frame");
	/*if (!border) border = 1;*/
	if ((cellsp = get_num(attr, cast_uchar "cellspacing")) == -1) cellsp = gf_val(1, 2);
	if ((cellpd = get_num(attr, cast_uchar "cellpadding")) == -1) {
		vcellpd = gf_val(0, 1);
		cellpd = gf_val(!!border, 1);
	} else {
		if (!F) {
			vcellpd = cellpd >= HTML_CHAR_HEIGHT / 2 + 1;
			cellpd = cellpd >= HTML_CHAR_WIDTH / 2 + 1;
		} else vcellpd = cellpd;
	}
	if (!F && !border) cellsp = 0;
	else if (!F && !cellsp) cellsp = 1;
	/* Sparc gcc-2.7.2.1 miscompiles this */
	do_not_optimize_here(&cellsp);
	if (!F && border > 2) border = 2;
	if (!F && cellsp > 2) cellsp = 2;
#ifdef G
	if (F && !cellsp && border) cellsp = 1;
#endif
	align = par_format.align;
	if (align == AL_NO || align == AL_NO_BREAKABLE || align == AL_BLOCK) align = AL_LEFT;
	if ((al = get_attr_val(attr, cast_uchar "summary"))) {
		if (!strcmp(cast_const_char al, "diff")) {
			mem_free(al);
			if ((al = get_attr_val(attr, cast_uchar "class"))) {
				if (!strcmp(cast_const_char al, "diff")) {
					format_.attr |= AT_FIXED;
					par_format.align = AL_NO;
				}
				mem_free(al);
			}
		} else {
			mem_free(al);
		}
	}
	if ((al = get_attr_val(attr, cast_uchar "align"))) {
		if (!strcasecmp(cast_const_char al, "left")) align = AL_LEFT;
		if (!strcasecmp(cast_const_char al, "center")) align = AL_CENTER;
		if (!strcasecmp(cast_const_char al, "right")) align = AL_RIGHT;
		mem_free(al);
	}
	frame = F_BOX;
	if ((al = get_attr_val(attr, cast_uchar "frame"))) {
		if (!strcasecmp(cast_const_char al, "void")) frame = F_VOID;
		if (!strcasecmp(cast_const_char al, "above")) frame = F_ABOVE;
		if (!strcasecmp(cast_const_char al, "below")) frame = F_BELOW;
		if (!strcasecmp(cast_const_char al, "hsides")) frame = F_HSIDES;
		if (!strcasecmp(cast_const_char al, "vsides")) frame = F_VSIDES;
		if (!strcasecmp(cast_const_char al, "lhs")) frame = F_LHS;
		if (!strcasecmp(cast_const_char al, "rhs")) frame = F_RHS;
		if (!strcasecmp(cast_const_char al, "box")) frame = F_BOX;
		if (!strcasecmp(cast_const_char al, "border")) frame = F_BOX;
		mem_free(al);
	}
	rules = border ? R_ALL : R_NONE;
	if ((al = get_attr_val(attr, cast_uchar "rules"))) {
		if (!strcasecmp(cast_const_char al, "none")) rules = R_NONE;
		if (!strcasecmp(cast_const_char al, "groups")) rules = R_GROUPS;
		if (!strcasecmp(cast_const_char al, "rows")) rules = R_ROWS;
		if (!strcasecmp(cast_const_char al, "cols")) rules = R_COLS;
		if (!strcasecmp(cast_const_char al, "all")) rules = R_ALL;
		mem_free(al);
	}
	if (!border) frame = F_VOID;
	wf = 0;
	if ((width = get_width(attr, cast_uchar "width", gf_val(p->data || p->xp, !!gp->data))) == -1) {
		width = par_format.width - safe_add(par_format.leftmargin, par_format.rightmargin) * gf_val(1, G_HTML_MARGIN);
		if (width < 0) width = 0;
		wf = 1;
	}
	t = parse_table(html, eof, end, &bgcolor, gf_val(p->data || p->xp, !!gp->data), &bad_html, &bad_html_n);
	for (i = 0; i < bad_html_n; i++) {
		while (bad_html[i].s < bad_html[i].e && WHITECHAR(*bad_html[i].s)) bad_html[i].s++;
		while (bad_html[i].s < bad_html[i].e && WHITECHAR(bad_html[i].e[-1])) bad_html[i].e--;
		if (bad_html[i].s < bad_html[i].e) parse_html(bad_html[i].s, bad_html[i].e, put_chars_f, line_break_f, special_f, gf_val((void *)p, (void *)gp), NULL);
	}
	mem_free(bad_html);
#ifdef G
	if (F) {
		t->gp = gp;
	} else
#endif
	{
		t->p = p;
	}
	t->bordercolor = get_attr_val(attr, cast_uchar "bordercolor");
	t->align = align;
	t->border = border;
	t->cellpd = cellpd;
	t->vcellpd = vcellpd;
	t->cellsp = cellsp;
	t->frame = frame;
	t->rules = rules;
	t->width = width;
	t->wf = wf;
	cpd_pass = 0;
	cpd_last = t->cellpd;
	cpd_width = 0;	/* not needed, but let the warning go away */
	again:
	get_cell_widths(t);
	if (get_column_widths(t)) goto ret2;
	get_table_width(t);
	if (gf_val(!p->data && !p->xp, !gp->data)) {
		if (!wf && t->max_t > width) t->max_t = width;
		if (t->max_t < t->min_t) t->max_t = t->min_t;
		if (safe_add(t->max_t, safe_add(par_format.leftmargin, par_format.rightmargin) * gf_val(1, G_HTML_MARGIN)) > gf_val(p->xmax, gp->xmax)) *gf_val(&p->xmax, &gp->xmax) = t->max_t + (par_format.leftmargin + par_format.rightmargin) * gf_val(1, G_HTML_MARGIN);
		if (safe_add(t->min_t, safe_add(par_format.leftmargin, par_format.rightmargin) * gf_val(1, G_HTML_MARGIN)) > gf_val(p->x, gp->x)) *gf_val(&p->x, &gp->x) = t->min_t + (par_format.leftmargin + par_format.rightmargin) * gf_val(1, G_HTML_MARGIN);
		goto ret2;
	}
	if (!F && !cpd_pass && t->min_t > width && t->cellpd) {
		t->cellpd = 0;
		cpd_pass = 1;
		cpd_width = t->min_t;
		goto again;
	}
	if (cpd_pass == 1 && t->min_t > cpd_width) {
		t->cellpd = cpd_last;
		cpd_pass = 2;
		goto again;
	}
	/*debug("%d %d %d", t->min_t, t->max_t, width);*/
	if (t->min_t >= width) distribute_widths(t, t->min_t);
	else if (t->max_t < width && wf) distribute_widths(t, t->max_t);
	else distribute_widths(t, width);
	if (!F && !p->data && p->xp == 1) {
		int ww = safe_add(t->rw, safe_add(par_format.leftmargin, par_format.rightmargin));
		if (ww > par_format.width) ww = par_format.width;
		if (ww < t->rw) ww = t->rw;
		if (ww > p->x) p->x = ww;
		p->cy = safe_add(p->cy, t->rh);
		goto ret2;
	}
#ifdef HTML_TABLE_2ND_PASS
	if (!F) check_table_widths(t);
#endif
	get_table_heights(t);
#ifdef G
	if (F) {
		gp->link_num = t->link_num;
		process_g_table(gp, t);
		t = NULL;
		goto ret3;
	}
#endif

	x = par_format.leftmargin;
	if (align == AL_CENTER) x = (safe_add(par_format.width, par_format.leftmargin) - par_format.rightmargin - t->rw) / 2;
	if (align == AL_RIGHT) x = par_format.width - par_format.rightmargin - t->rw;
	if (safe_add(x, t->rw) > par_format.width) x = par_format.width - t->rw;
	if (x < 0) x = 0;
	/*display_table(t, x, p->cy, &cye);*/
	if (!p->data) {
		if (safe_add(t->rw, safe_add(par_format.leftmargin, par_format.rightmargin)) > p->x) p->x = t->rw + par_format.leftmargin + par_format.rightmargin;
		p->cy = safe_add(p->cy, t->rh);
		goto ret2;
	}

	n = p->data->nodes.next;
	n->yw = p->yp - n->y + p->cy;
	display_complicated_table(t, x, p->cy, &cye);
	display_table_frames(t, x, p->cy);
	nn = mem_alloc(sizeof(struct node));
	nn->x = n->x;
	nn->y = safe_add(p->yp, cye);
	nn->xw = n->xw;
	add_to_list(p->data->nodes, nn);
	/*if (p->cy + t->rh != cye) internal("size does not match; 1:%d, 2:%d", p->cy + t->rh, cye);*/
	p->cy = cye;

	ret2:
	*gf_val(&p->link_num, &gp->link_num) = t->link_num;
#ifdef G
	ret3:
#endif
	if (!F) if (p->cy > p->y) p->y = p->cy;
	if (t) free_table(t);
	kill_html_stack_item(&html_top);
	table_level--;
	if (!table_level) {
		free_table_cache();
	}
	AF = AF_SAVE;
}

#ifdef G

static void add_to_rect_sets(struct rect_set ***s, int *n, struct rect *r)
{
	int i, j;
	for (i = r->y1 >> RECT_BOUND_BITS; i <= (r->y2 - 1) >> RECT_BOUND_BITS; i++) {
		if (i >= *n) {
			struct rect_set **ns;
			if ((unsigned)i > MAXINT / sizeof(struct rect_set *) - 1) overalloc();
			ns = mem_realloc(*s, (i + 1) * sizeof(struct rect_set *));
			for (j = *n; j < i + 1; j++) ns[j] = init_rect_set();
			*s = ns;
			*n = i + 1;
		}
		add_to_rect_set(&(*s)[i], r);
	}
}

static void add_to_cell_sets(struct table_cell ****s, int **nn, int *n, struct rect *r, struct table_cell *c)
{
	int i, j;
	if (r->y1 < 0 || r->y2 < 0)
		internal("add_to_cell_sets: integer overflow: %d, %d", r->y1, r->y2);
	for (i = r->y1 >> RECT_BOUND_BITS; i <= (r->y2 - 1) >> RECT_BOUND_BITS; i++) {
		if (i >= *n) {
			struct table_cell ***ns;
			int *nnn;
			if ((unsigned)i > MAXINT / sizeof(struct table_cell ***) - 1) overalloc();
			if ((unsigned)i > MAXINT / sizeof(int *) - 1) overalloc();
			ns = mem_realloc(*s, (i + 1) * sizeof(struct table_cell ***));
			nnn = mem_realloc(*nn, (i + 1) * sizeof(int *));
			for (j = *n; j < i + 1; j++) ns[j] = DUMMY, nnn[j] = 0;
			*s = ns;
			*nn = nnn;
			*n = i + 1;
		}
		{
			struct table_cell **nc;
			if ((unsigned)(*nn)[i]  > MAXINT / sizeof(struct table_cell *) - 1) overalloc();
			nc = mem_realloc((*s)[i], ((*nn)[i] + 1) * sizeof(struct table_cell *));
			nc[(*nn)[i]] = c;
			(*s)[i] = nc;
			(*nn)[i]++;
		}
	}
}

static void table_mouse_event(struct f_data_c *fd, struct g_object_table *o, int x, int y, int b)
{
	struct table *t = o->t;
	int i, j;
	for (j = 0; j < t->y; j++) for (i = 0; i < t->x; i++) {
		struct table_cell *c = CELL(t, i, j);
		if (c->root) if (!g_forward_mouse(fd, (struct g_object *)c->root, x, y, b)) return;
	}
}

static void draw_rect_set(struct graphics_device *dev, struct background *bg, struct rect_set *rs, int x, int y)
{
	int i;
	for (i = 0; i < rs->m; i++) {
		struct rect *r = &rs->r[i];
		if (is_rect_valid(r))
			g_draw_background(dev, bg, x + r->x1, y + r->y1, r->x2 - r->x1, r->y2 - r->y1);
	}
}

static void draw_rect_sets(struct graphics_device *dev, struct background *bg, struct rect_set **rs, int nrs, int x, int y)
{
	int i;
	for (i = (dev->clip.y1 - y) >> RECT_BOUND_BITS; i <= (dev->clip.y2 - y - 1) >> RECT_BOUND_BITS; i++) if (i >= 0 && i < nrs) {
		draw_rect_set(dev, bg, rs[i], x, y);
	}
}

static void table_draw(struct f_data_c *fd, struct g_object_table *o, int x, int y)
{
	static tcount dgen = 0;
	tcount my_dgen = ++dgen;
	int i, j;
	struct table *t = o->t;
	struct graphics_device *dev = fd->ses->term->dev;
	/*
	for (j = 0; j < t->y; j++) for (i = 0; i < t->x; i++) {
		struct table_cell *c = CELL(t, i, j);*/
	/*
	fprintf(stderr, "Y: %d %d\n", x, y);
	fprintf(stderr, "bounds: %d %d\n", dev->clip.y1 + y, dev->clip.y2 + y);
	*/
	for (i = (dev->clip.y1 - y) >> RECT_BOUND_BITS; i <= (dev->clip.y2 - y - 1) >> RECT_BOUND_BITS; i++) if (i >= 0 && i < t->nr_cells) for (j = 0; j < t->w_cells[i]; j++) {
		struct table_cell *c = t->r_cells[i][j];
		/*fprintf(stderr, "draw: %d %d\n", i, j);*/
		if (c->root && c->dgen != my_dgen) {
			struct rect clip;
			c->dgen = my_dgen;
			memcpy(&clip, &c->rect, sizeof(struct rect));
			clip.x1 = clip.x1 + x;
			clip.x2 = clip.x2 + x;
			clip.y1 = clip.y1 + y;
			clip.y2 = clip.y2 + y;
			if (!do_rects_intersect(&clip, &dev->clip)) continue;
			draw_rect_set(dev, c->root->bg, c->brd, x, y);
			restrict_clip_area(dev, &clip, x + c->root->x, y + c->root->y, x + c->root->x + c->root->xw /*c->g_width*/, y + c->root->y + c->root->yw);
			c->root->draw(fd, c->root, x + c->root->x, y + c->root->y);
			drv->set_clip_area(dev, &clip);
		}
	}
	draw_rect_sets(dev, t->bg, t->r_bg, t->nr_bg, x, y);
	draw_rect_sets(dev, t->frame_bg, t->r_frame, t->nr_frame, x, y);
}

static void table_destruct(struct g_object_table *o)
{
	free_table(o->t);
	mem_free(o);
}

static void table_get_list(struct g_object_table *o, void (*fn)(struct g_object *parent, struct g_object *child))
{
	struct table *t = o->t;
	int i, j;
	for (j = 0; j < t->y; j++) for (i = 0; i < t->x; i++) {
		struct table_cell *c = CELL(t, i, j);
		if (c->tag_object)
			fn((struct g_object *)o, (struct g_object *)c->tag_object);
		if (c->root)
			fn((struct g_object *)o, (struct g_object *)c->root);
	}
}

void table_bg(struct text_attrib *ta, unsigned char bgstr[8])
{
	if (ta->bg.r + ta->bg.g * 3 + ta->bg.b * 5 >= 9 * 128) strcpy(cast_char bgstr, "#000000");
	else if (ta->fg.r > G_HTML_TABLE_FRAME_COLOR && ta->fg.g > G_HTML_TABLE_FRAME_COLOR && ta->fg.b > G_HTML_TABLE_FRAME_COLOR) {
		unsigned char max = ta->fg.r;
		if (ta->fg.g > max) max = ta->fg.g;
		if (ta->fg.b > max) max = ta->fg.b;
		max &= 0xff;
		sprintf(cast_char bgstr, "#%02x%02x%02x", max, max, max);
	} else sprintf(cast_char bgstr, "#%02x%02x%02x", G_HTML_TABLE_FRAME_COLOR, G_HTML_TABLE_FRAME_COLOR, G_HTML_TABLE_FRAME_COLOR);
}

static void process_g_table(struct g_part *gp, struct table *t)
{
	int i, j;
	int x, y;
	struct g_object_table *o;
	short *fv, *fh;
	unsigned char bgstr[8];
	struct text_attrib *ta;
	struct rgb dummy;
	y = 0;
	for (j = 0; j < t->y; j++) {
		int qw = g_get_hline_pad(t, j, NULL, NULL);
		x = 0;
		y = safe_add(y, qw);
		for (i = 0; i < t->x; i++) {
			struct table_cell *c;
			int we = g_get_vline_pad(t, i, NULL, NULL);
			x = safe_add(x, we);
			c = CELL(t, i, j);
			if (c->root) {
				int s;
				int yw = 0;
				for (s = 0; s < c->rowspan; s++) {
					yw = safe_add(yw, t->r_heights[j + s]);
					if (s < c->rowspan - 1) {
						int er = g_get_hline_pad(t, j + s + 1, NULL, NULL);
						yw = safe_add(yw, er);
					}
				}
				c->root->x = x, c->root->y = y;
				c->root->y = safe_add(c->root->y, c->valign != VAL_MIDDLE && c->valign != VAL_BOTTOM ? 0 : (yw - c->root->yw) / (c->valign == VAL_MIDDLE ? 2 : 1));
			}
			x = safe_add(x, t->w_c[i]);
		}
		y = safe_add(y, t->r_heights[j]);
	}

	if (html_top.next != (struct html_element *)(void *)&html_stack) ta = &html_top.next->attr;
	else ta = &format_;

	if (t->bordercolor && !decode_color(t->bordercolor, &dummy)) {
		if (!(t->frame_bg = get_background(NULL, t->bordercolor))) {
			free_table(t);
			return;
		}
	} else {
		table_bg(ta, bgstr);
		if (!(t->frame_bg = get_background(NULL, bgstr))) {
			free_table(t);
			return;
		}
	}

	if ((unsigned)t->x > MAXINT) overalloc();
	if ((unsigned)t->y > MAXINT) overalloc();
	if (((unsigned)t->x + 2) * ((unsigned)t->y + 2) / ((unsigned)t->x + 2) != ((unsigned)t->y + 2)) overalloc();
	if (((unsigned)t->x + 2) * ((unsigned)t->y + 2) > MAXINT) overalloc();
	fh = mem_alloc((t->x + 2) * (t->y + 1) * sizeof(short));
	fv = mem_alloc((t->x + 1) * (t->y + 2) * sizeof(short));
	get_table_frame(t, fv, fh);
	y = 0;
	for (j = 0; j <= t->y; j++) {
		int ypad, ypos, ysize;
		ypad = g_get_hline_pad(t, j, &ypos, &ysize);
		x = 0;
		for (i = 0; i <= t->x; i++) {
			struct rect r;
			int xpad, xpos, xsize;
			xpad = g_get_vline_pad(t, i, &xpos, &xsize);
			if (i < t->x && j < t->y) {
				CELL(t, i, j)->xpos = safe_add(safe_add(x, xpos), xsize);
				CELL(t, i, j)->ypos = safe_add(safe_add(y, ypos), ysize);
			}
			if (i > 0 && j > 0) {
				struct table_cell *c = CELL(t, i - 1, j - 1);
				c->xw = x + xpos - c->xpos;
				c->yw = y + ypos - c->ypos;
				/*debug("C: %d %d %d %d", c->xpos, c->ypos, c->xw, c->yw);*/
				/*debug("%d %d %d", y, ypos, c->ypos);*/
				if (!c->used && !c->spanned) {
					r.x1 = c->xpos, r.x2 = c->xpos + c->xw;
					r.y1 = c->ypos, r.y2 = c->ypos + c->yw;
					add_to_rect_sets(&t->r_bg, &t->nr_bg, &r);
				}
			}
			r.x1 = x + xpos, r.x2 = x + xpos + xsize;
			r.y1 = y + ypos, r.y2 = y + ypos + ysize;
			if (H_LINE(i-1,j) || H_LINE(i,j) || V_LINE(i,j-1) || V_LINE(i,j))
				add_to_rect_sets(&t->r_frame, &t->nr_frame, &r);
			else if (H_LINE_X(i-1,j) != -2 || H_LINE_X(i,j) != -2 || V_LINE_X(i,j-1) != -2 || V_LINE_X(i,j) != -2) add_to_rect_sets(&t->r_bg, &t->nr_bg, &r);
			if (i < t->x) {
				int l;
				int b;
				g_get_vline_pad(t, i + 1, &b, NULL);
				r.x1 = r.x2;
				r.x2 = safe_add(safe_add(safe_add(x, xpad), t->w_c[i]), b);
				l = H_LINE_X(i,j);
				if (l == -2)
					;
				else if (l > 0) add_to_rect_sets(&t->r_frame, &t->nr_frame, &r);
				else add_to_rect_sets(&t->r_bg, &t->nr_bg, &r);
			}
			r.x1 = x + xpos, r.x2 = x + xpos + xsize;
			if (j < t->y) {
				int l;
				int b;
				g_get_hline_pad(t, j + 1, &b, NULL);
				r.y1 = r.y2;
				r.y2 = safe_add(safe_add(safe_add(y, ypad), t->r_heights[j]), b);
				l = V_LINE_X(i,j);
				if (l == -2)
					;
				else if (l > 0) add_to_rect_sets(&t->r_frame, &t->nr_frame, &r);
				else add_to_rect_sets(&t->r_bg, &t->nr_bg, &r);
			}
			if (i < t->x) x = safe_add(x, safe_add(xpad, t->w_c[i]));
		}
		if (j < t->y) y = safe_add(y, safe_add(ypad, t->r_heights[j]));
	}

	for (j = 0; j < t->y; j++) for (i = 0; i < t->x; i++) {
		struct table_cell *c = CELL(t, i, j);
		if (c->used && !c->spanned && c->root) {
			struct table_cell *d = CELL(t, i + c->colspan - 1, j + c->rowspan - 1);
			struct rect r;
			r.x1 = c->xpos;
			r.y1 = c->ypos;
			r.x2 = safe_add(d->xpos, d->xw);
			r.y2 = safe_add(d->ypos, d->yw);
			add_to_cell_sets(&t->r_cells, &t->w_cells, &t->nr_cells, &r, c);
			memcpy(&c->rect, &r, sizeof(struct rect));
			c->brd = init_rect_set();
			/*debug("%d,%d %d,%d", r.x1, r.y1, r.x2, r.y2);*/
			add_to_rect_set(&c->brd, &r);
			r.x1 = c->root->x;
			r.y1 = c->root->y;
			r.x2 = safe_add(c->root->x, c->root->xw);
			r.y2 = safe_add(c->root->y, c->root->yw);
			exclude_rect_from_set(&c->brd, &r);
			/*debug("%d,%d %d,%d", r.x1, r.y1, r.x2, r.y2);*/
		}
	}

	mem_free(fh);
	mem_free(fv);

	o = mem_calloc(sizeof(struct g_object_table));
	o->mouse_event = table_mouse_event;
	o->draw = table_draw;
	o->destruct = table_destruct;
	o->get_list = table_get_list;
	o->xw = t->rw;
	o->yw = t->rh;
	o->t = t;
	t->bg = gp->root->bg;
	flush_pending_text_to_line(gp);
	flush_pending_line_to_obj(gp, 0);
	gp->cx = -1;
	gp->cx_w = 0;
	add_object_to_line(gp, &gp->line, (struct g_object *)o);
	flush_pending_text_to_line(gp);
	par_format.align = t->align;
	flush_pending_line_to_obj(gp, 0);
	gp->cx = -1;
	gp->cx_w = 0;

	for (j = 0; j < t->y; j++) {
		for (i = 0; i < t->x; i++) {
			struct table_cell *c;
			c = CELL(t, i, j);
			if (c->root && c->tag) {
				struct g_object_tag *tag = mem_calloc(sizeof(struct g_object_tag) + strlen(cast_const_char c->tag));
				tag->mouse_event = g_dummy_mouse;
				tag->draw = g_dummy_draw;
				tag->destruct = g_tag_destruct;
				strcpy(cast_char tag->name, cast_const_char c->tag);
				tag->x = c->root->x;
				tag->y = c->root->y + o->yw;
				c->tag_object = tag;
			}
		}
	}

}

#endif

struct table_cache_entry {
	struct table_cache_entry *next;
	struct table_cache_entry *prev;
	struct table_cache_entry *hash_next;
	unsigned char *start;
	unsigned char *end;
	int align;
	int m;
	int width;
	int xs;
	int link_num;
	union {
		struct part p;
#ifdef G
		struct g_part gp;
#endif
	} u;
};

static struct list_head table_cache = { &table_cache, &table_cache };

#define TC_HASH_SIZE	8192

static struct table_cache_entry *table_cache_hash[TC_HASH_SIZE] = { NULL };

static inline int make_hash(unsigned char *start, int xs)
{
	return ((int)(unsigned long)start + xs) & (TC_HASH_SIZE - 1);
}

void *find_table_cache_entry(unsigned char *start, unsigned char *end, int align, int m, int width, int xs, int link_num)
{
	int hash = make_hash(start, xs);
	struct table_cache_entry *tce;
	for (tce = table_cache_hash[hash]; tce; tce = tce->hash_next) {
		if (tce->start == start && tce->end == end && tce->align == align && tce->m == m && tce->width == width && tce->xs == xs && tce->link_num == link_num) {
			if (!F) {
				struct part *p = mem_alloc(sizeof(struct part));
				memcpy(p, &tce->u.p, sizeof(struct part));
				return p;
#ifdef G
			} else {
				struct g_part *gp = mem_alloc(sizeof(struct g_part));
				memcpy(gp, &tce->u.gp, sizeof(struct g_part));
				return gp;
#endif
			}
		}
	}
	return NULL;
}

void add_table_cache_entry(unsigned char *start, unsigned char *end, int align, int m, int width, int xs, int link_num, void *p)
{
	int hash;
	struct table_cache_entry *tce = mem_alloc(sizeof(struct table_cache_entry));
	tce->start = start;
	tce->end = end;
	tce->align = align;
	tce->m = m;
	tce->width = width;
	tce->xs = xs;
	tce->link_num = link_num;
	if (!F) memcpy(&tce->u.p, p, sizeof(struct part));
#ifdef G
	else memcpy(&tce->u.gp, p, sizeof(struct g_part));
#endif
	hash = make_hash(start, xs);
	tce->hash_next = table_cache_hash[hash];
	table_cache_hash[hash] = tce;
	add_to_list(table_cache, tce);
}

static void free_table_cache(void)
{
	struct table_cache_entry *tce;
	foreach(tce, table_cache) {
		int hash = make_hash(tce->start, tce->xs);
		table_cache_hash[hash] = NULL;
	}
	free_list(table_cache);
}
