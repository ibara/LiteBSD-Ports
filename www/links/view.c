/* view.c
 * (c) 2002 Mikulas Patocka
 * This file is a part of the Links program, released under GPL.
 */

#include "links.h"

static void send_open_in_new_xterm(struct terminal *term, int (*open_window)(struct terminal *, unsigned char *, unsigned char *), struct session *ses);

static void init_ctrl(struct form_control *, struct form_state *);

static int c_in_view(struct f_data_c *);

static void set_pos_x(struct f_data_c *, struct link *);
static void set_pos_y(struct f_data_c *, struct link *);
static void find_link(struct f_data_c *, int, int);
static void update_braille_link(struct f_data_c *f);

static int is_active_frame(struct session *ses, struct f_data_c *f);


struct view_state *create_vs(void)
{
	struct view_state *vs;
	vs = mem_calloc(sizeof(struct view_state));
	vs->refcount = 1;
	vs->current_link = -1;
	vs->orig_link = -1;
	vs->frame_pos = -1;
	vs->plain = -1;
	vs->form_info = DUMMY;
	vs->form_info_len = 0;
	return vs;
}

void destroy_vs(struct view_state *vs)
{
	int i;
	if (--vs->refcount) {
		if (vs->refcount < 0) internal("destroy_vs: view_state refcount underflow");
		return;
	}
	for (i = 0; i < vs->form_info_len; i++) if (vs->form_info[i].value) mem_free(vs->form_info[i].value);
	mem_free(vs->form_info);
	mem_free(vs);
}

#if 0
static void copy_vs(struct view_state *dst, struct view_state *src)
{
	int i;
	memcpy(dst, src, sizeof(struct view_state));
	if ((unsigned)src->form_info_len > MAXINT / sizeof(struct form_state)) overalloc();
	dst->form_info = mem_alloc(src->form_info_len * sizeof(struct form_state));
	memcpy(dst->form_info, src->form_info, src->form_info_len * sizeof(struct form_state));
	for (i = 0; i < src->form_info_len; i++) if (src->form_info[i].value) dst->form_info[i].value = stracpy(src->form_info[i].value);
}
#endif

#ifdef JS
void create_js_event_spec(struct js_event_spec **j)
{
	if (*j) return;
	*j = mem_calloc(sizeof(struct js_event_spec));
}

void free_js_event_spec(struct js_event_spec *j)
{
	if (!j) return;
	if (j->move_code) mem_free(j->move_code);
	if (j->over_code) mem_free(j->over_code);
	if (j->out_code) mem_free(j->out_code);
	if (j->down_code) mem_free(j->down_code);
	if (j->up_code) mem_free(j->up_code);
	if (j->click_code) mem_free(j->click_code);
	if (j->dbl_code) mem_free(j->dbl_code);
	if (j->blur_code) mem_free(j->blur_code);
	if (j->focus_code) mem_free(j->focus_code);
	if (j->change_code) mem_free(j->change_code);
	if (j->keypress_code) mem_free(j->keypress_code);
	if (j->keyup_code) mem_free(j->keyup_code);
	if (j->keydown_code) mem_free(j->keydown_code);
	mem_free(j);
}

int compare_js_event_spec(struct js_event_spec *j1, struct js_event_spec *j2)
{
	if (!j1 && !j2) return 0;
	if (!j1 || !j2) return 1;
	return
		xstrcmp(j1->move_code, j2->move_code) ||
		xstrcmp(j1->over_code, j2->over_code) ||
		xstrcmp(j1->out_code, j2->out_code) ||
		xstrcmp(j1->down_code, j2->down_code) ||
		xstrcmp(j1->up_code, j2->up_code) ||
		xstrcmp(j1->click_code, j2->click_code) ||
		xstrcmp(j1->dbl_code, j2->dbl_code) ||
		xstrcmp(j1->blur_code, j2->blur_code) ||
		xstrcmp(j1->focus_code, j2->focus_code) ||
		xstrcmp(j1->change_code, j2->change_code) ||
		xstrcmp(j1->keypress_code, j2->keypress_code) ||
		xstrcmp(j1->keydown_code, j2->keydown_code) ||
		xstrcmp(j1->keyup_code, j2->keyup_code);
}

void copy_js_event_spec(struct js_event_spec **target, struct js_event_spec *source)
{
	struct js_event_spec *t;
	*target = NULL;
	if (!source) return;
	create_js_event_spec(target);
	t = *target;
	t->move_code = stracpy(source->move_code);
	t->over_code = stracpy(source->over_code);
	t->out_code = stracpy(source->out_code);
	t->down_code = stracpy(source->down_code);
	t->up_code = stracpy(source->up_code);
	t->click_code = stracpy(source->click_code);
	t->dbl_code = stracpy(source->dbl_code);
	t->blur_code = stracpy(source->blur_code);
	t->focus_code = stracpy(source->focus_code);
	t->change_code = stracpy(source->change_code);
	t->keypress_code = stracpy(source->keypress_code);
	t->keyup_code = stracpy(source->keyup_code);
	t->keydown_code = stracpy(source->keydown_code);
}

static inline int copy_string(unsigned char **dest, unsigned char *src)
{
	if (!src) return 0;
	if (*dest) {
		if (!strcmp(cast_const_char src, cast_const_char *dest)) return 0;
		mem_free(*dest);
	}
	*dest = stracpy(src);
	return 1;
}

int join_js_event_spec(struct js_event_spec **target, struct js_event_spec *source)
{
	if (!source) return 0;
	create_js_event_spec(target);
	return
	copy_string(&(*target)->move_code, source->move_code) |
	copy_string(&(*target)->over_code, source->over_code) |
	copy_string(&(*target)->out_code, source->out_code) |
	copy_string(&(*target)->down_code, source->down_code) |
	copy_string(&(*target)->up_code, source->up_code) |
	copy_string(&(*target)->click_code, source->click_code) |
	copy_string(&(*target)->dbl_code, source->dbl_code) |
	copy_string(&(*target)->blur_code, source->blur_code) |
	copy_string(&(*target)->focus_code, source->focus_code) |
	copy_string(&(*target)->change_code, source->change_code) |
	copy_string(&(*target)->keypress_code, source->keypress_code) |
	copy_string(&(*target)->keyup_code, source->keyup_code) |
	copy_string(&(*target)->keydown_code, source->keydown_code);
}

static void add_event_desc(unsigned char **str, int *l, unsigned char *fn, unsigned char *desc)
{
	if (!fn) return;
	if (*l) add_to_str(str, l, cast_uchar ", ");
	add_to_str(str, l, desc);
	add_to_str(str, l, cast_uchar ":");
	add_to_str(str, l, fn);
}

unsigned char *print_js_event_spec(struct js_event_spec *j)
{
	unsigned char *str = init_str();
	int l = 0;
	if (!j) return str;
	add_event_desc(&str, &l, j->click_code, cast_uchar "onclick");
	add_event_desc(&str, &l, j->dbl_code, cast_uchar "ondblclick");
	add_event_desc(&str, &l, j->down_code, cast_uchar "onmousedown");
	add_event_desc(&str, &l, j->up_code, cast_uchar "onmouseup");
	add_event_desc(&str, &l, j->over_code, cast_uchar "onmouseover");
	add_event_desc(&str, &l, j->out_code, cast_uchar "onmouseout");
	add_event_desc(&str, &l, j->move_code, cast_uchar "onmousemove");
	add_event_desc(&str, &l, j->focus_code, cast_uchar "onfocus");
	add_event_desc(&str, &l, j->blur_code, cast_uchar "onblur");
	add_event_desc(&str, &l, j->change_code, cast_uchar "onchange");
	add_event_desc(&str, &l, j->keypress_code, cast_uchar "onkeypress");
	add_event_desc(&str, &l, j->keyup_code, cast_uchar "onkeyup");
	add_event_desc(&str, &l, j->keydown_code, cast_uchar "onkeydown");
	return str;
}

#else

void free_js_event_spec(struct js_event_spec *j)
{
}

int compare_js_event_spec(struct js_event_spec *j1, struct js_event_spec *j2)
{
	return 0;
}

void copy_js_event_spec(struct js_event_spec **target, struct js_event_spec *source)
{
}

int join_js_event_spec(struct js_event_spec **target, struct js_event_spec *source)
{
	return 0;
}

unsigned char *print_js_event_spec(struct js_event_spec *j)
{
	return stracpy(cast_uchar "");
}

#endif

void check_vs(struct f_data_c *f)
{
	struct view_state *vs = f->vs;
	int ovx, ovy, ol, obx, oby;
	if (f->f_data->frame_desc) {
		struct f_data_c *ff;
		int n = 0;
		foreach(ff, f->subframes) n++;
		if (vs->frame_pos < 0) vs->frame_pos = 0;
		if (vs->frame_pos >= n) vs->frame_pos = n - 1;
		return;
	}
	ovx = f->vs->orig_view_posx;
	ovy = f->vs->orig_view_pos;
	ol = f->vs->orig_link;
	obx = f->vs->orig_brl_x;
	oby = f->vs->orig_brl_y;
	if (vs->current_link >= f->f_data->nlinks) vs->current_link = f->f_data->nlinks - 1;
	if (!F) {
		if (vs->current_link != -1 && !c_in_view(f)) {
			set_pos_x(f, &f->f_data->links[f->vs->current_link]);
			set_pos_y(f, &f->f_data->links[f->vs->current_link]);
		}
		if (vs->current_link == -1) find_link(f, 1, 0);
		if (f->ses->term->spec->braille) {
			if (vs->brl_x >= f->f_data->x) vs->brl_x = f->f_data->x - 1;
			if (vs->brl_x >= vs->view_posx + f->xw) vs->brl_x = vs->view_posx + f->xw - 1;
			if (vs->brl_x < vs->view_posx) vs->brl_x = vs->view_posx;
			if (vs->brl_y >= f->f_data->y) vs->brl_y = f->f_data->y - 1;
			if (vs->brl_y >= vs->view_pos + f->yw) vs->brl_y = vs->view_pos + f->yw - 1;
			if (vs->brl_y < vs->view_pos) vs->brl_y = vs->view_pos;
			update_braille_link(f);
		}
#ifdef G
	} else {
		/*if (vs->current_link >= 0 && !is_link_in_view(f, vs->current_link)) vs->current_link = -1;*/
#endif
	}
	f->vs->orig_view_posx = ovx;
	f->vs->orig_view_pos = ovy;
	if (!f->ses->term->spec->braille) f->vs->orig_link = ol;
	f->vs->orig_brl_x = obx;
	f->vs->orig_brl_y = oby;
}

static void set_link(struct f_data_c *f)
{
	if (c_in_view(f)) return;
	find_link(f, 1, 0);
}

static int find_tag(struct f_data *f, unsigned char *name)
{
	struct tag *tag;
	unsigned char *tt;
	int ll;
	tt = init_str();
	ll = 0;
	add_conv_str(&tt, &ll, name, (int)strlen(cast_const_char name), -2);
	foreachback(tag, f->tags) if (!strcasecmp(cast_const_char tag->name, cast_const_char tt) || (tag->name[0] == '#' && !strcasecmp(cast_const_char(tag->name + 1), cast_const_char tt))) {
		mem_free(tt);
		return tag->y;
	}
	mem_free(tt);
	return -1;
}

LIBC_CALLBACK static int comp_links(struct link *l1, struct link *l2)
{
	return l1->num - l2->num;
}

void sort_links(struct f_data *f)
{
	int i;
	if (F) return;
	if (f->nlinks) qsort(f->links, f->nlinks, sizeof(struct link), (int(*)(const void *, const void *))comp_links);
	if ((unsigned)f->y > MAXINT / sizeof(struct link *)) overalloc();
	f->lines1 = mem_calloc(f->y * sizeof(struct link *));
	f->lines2 = mem_calloc(f->y * sizeof(struct link *));
	for (i = 0; i < f->nlinks; i++) {
		int p, q, j;
		struct link *link = &f->links[i];
		if (!link->n) {
			if (d_opt->num_links) continue;
			if (link->where) mem_free(link->where);
			if (link->target) mem_free(link->target);
			if (link->where_img) mem_free(link->where_img);
			if (link->img_alt) mem_free(link->img_alt);
			if (link->pos) mem_free(link->pos);
			free_js_event_spec(link->js_event);
			memmove(link, link + 1, (f->nlinks - i - 1) * sizeof(struct link));
			f->nlinks--;
			i--;
			continue;
		}
		p = f->y - 1;
		q = 0;
		for (j = 0; j < link->n; j++) {
			if (link->pos[j].y < p) p = link->pos[j].y;
			if (link->pos[j].y > q) q = link->pos[j].y;
		}
		if (p > q) j = p, p = q, q = j;
		for (j = p; j <= q; j++) {
			if (j >= f->y) {
				internal("link out of screen");
				continue;
			}
			f->lines2[j] = &f->links[i];
			if (!f->lines1[j]) f->lines1[j] = &f->links[i];
		}
	}
}

unsigned char *textptr_add(unsigned char *t, int i, int cp)
{
	if (cp != utf8_table) {
		if ((size_t)i <= strlen(cast_const_char t)) return t + i;
		else return t + strlen(cast_const_char t);
	} else {
		while (i-- && *t) FWD_UTF_8(t);
		return t;
	}
}

int textptr_diff(unsigned char *t2, unsigned char *t1, int cp)
{
	if (cp != utf8_table) return (int)(t2 - t1);
	else {
		int i = 0;
		while (t2 > t1) {
			FWD_UTF_8(t1);
			i++;
		}
		return i;
	}
}

static struct line_info *format_text_uncached(unsigned char *text, int width, int wrap, int cp)
{
	struct line_info *ln = DUMMY;
	int lnn = 0;
	unsigned char *b = text;
	int sk, ps = 0;
	while (*text) {
		unsigned char *s;
		if (*text == '\n') {
			sk = 1;
			put:
			if (!(lnn & (ALLOC_GR-1))) {
				if ((unsigned)lnn > MAXINT / sizeof(struct line_info) - ALLOC_GR) overalloc();
				ln = mem_realloc(ln, (lnn + ALLOC_GR) * sizeof(struct line_info));
			}
			ln[lnn].st = b;
			ln[lnn++].en = text;
			b = text += sk;
			continue;
		}
		if (!wrap || textptr_diff(text, b, cp) < width) {
			if (cp != utf8_table) text++;
			else FWD_UTF_8(text);
			continue;
		}
		for (s = text; s >= b; s--) if (*s == ' ') {
			text = s;
			if (wrap == 2) {
				*s = '\n';
				for (s++; *s; s++) if (*s == '\n') {
					if (s[1] != '\n') *s = ' ';
					break;
				}
			}
			sk = 1;
			goto put;
		}
		sk = 0;
		goto put;
	}
	if (ps < 2) {
		ps++;
		sk = 0;
		goto put;
	}
	ln[lnn - 1].st = ln[lnn - 1].en = NULL;
	return ln;
}

struct format_text_cache_entry {
	struct format_text_cache_entry *next;
	struct format_text_cache_entry *prev;
	unsigned char *text_ptr;
	int text_len;
	int width;
	int wrap;
	int cp;
	struct line_info *ln;
	unsigned char copied_text[1];
};

#define MAX_FORMAT_TEXT_CACHE_ENTRIES		5

static struct list_head format_text_cache = { &format_text_cache, &format_text_cache };
static int format_text_cache_entries = 0;

static void free_format_text_cache_entry(void)
{
	struct format_text_cache_entry *ftce = format_text_cache.prev;
	del_from_list(ftce);
	format_text_cache_entries--;
	mem_free(ftce->ln);
	mem_free(ftce);
}

struct line_info *format_text(unsigned char *text, int width, int wrap, int cp)
{
	struct format_text_cache_entry *ftce;
	int text_len = (int)strlen(cast_const_char text);
	foreach(ftce, format_text_cache) {
		if (ftce->text_len == text_len &&
		    ftce->text_ptr == text &&
		    !memcmp(ftce->copied_text, text, text_len) &&
		    ftce->width == width &&
		    ftce->wrap == wrap &&
		    ftce->cp == cp)
			goto have_it;
	}
	ftce = mem_alloc(sizeof(struct format_text_cache_entry) + text_len);
	memcpy(ftce->copied_text, text, text_len + 1);
	ftce->text_ptr = text;
	ftce->text_len = text_len;
	ftce->width = width;
	ftce->wrap = wrap;
	ftce->cp = cp;
	ftce->ln = format_text_uncached(text, width, wrap, cp);
	add_to_list(format_text_cache, ftce);
	format_text_cache_entries++;
	if (format_text_cache_entries > MAX_FORMAT_TEXT_CACHE_ENTRIES)
		free_format_text_cache_entry();
have_it:
	return ftce->ln;
}

void free_format_text_cache(void)
{
	while (format_text_cache_entries)
		free_format_text_cache_entry();
}

int area_cursor(struct f_data_c *f, struct form_control *form, struct form_state *fs)
{
	struct line_info *ln;
	int q = 0;
	int x, y;
	ln = format_text(fs->value, form->cols, form->wrap, f->f_data->opt.cp);
	for (y = 0; ln[y].st; y++) if (fs->value + fs->state >= ln[y].st && fs->value + fs->state < ln[y].en + (ln[y+1].st != ln[y].en)) {
		x = textptr_diff(fs->value + fs->state, ln[y].st, f->f_data->opt.cp);
		if (form->wrap && x == form->cols) x--;
		if (x >= form->cols + fs->vpos) fs->vpos = x - form->cols + 1;
		if (x < fs->vpos) fs->vpos = x;
		if (y >= form->rows + fs->vypos) fs->vypos = y - form->rows + 1;
		if (y < fs->vypos) fs->vypos = y;
		x -= fs->vpos;
		y -= fs->vypos;
		q = y * form->cols + x;
		break;
	}
	return q;
}

static void draw_link(struct terminal *t, struct f_data_c *scr, int l)
{
	struct link *link = &scr->f_data->links[l];
	int xp = scr->xp;
	int yp = scr->yp;
	int xw = scr->xw;
	int yw = scr->yw;
	int vx, vy;
	struct view_state *vs = scr->vs;
	int f = 0;
	vx = vs->view_posx;
	vy = vs->view_pos;
	if (scr->link_bg) {
		internal("link background not empty");
		mem_free(scr->link_bg);
	}
	if (l == -1) return;
	switch (link->type) {
		int i;
		int q;
		case L_LINK:
		case L_CHECKBOX:
		case L_BUTTON:
		case L_SELECT:
		case L_FIELD:
		case L_AREA:
			q = 0;
			if (link->type == L_FIELD) {
				struct form_state *fs = find_form_state(scr, link->form);
				if (fs) q = textptr_diff(fs->value + fs->state, fs->value + fs->vpos, scr->f_data->opt.cp);
				/*else internal("link has no form control");*/
			} else if (link->type == L_AREA) {
				struct form_state *fs = find_form_state(scr, link->form);
				if (fs) q = area_cursor(scr, link->form, fs);
				/*else internal("link has no form control");*/
			}
			if ((unsigned)link->n > MAXINT / sizeof(struct link_bg)) overalloc();
			scr->link_bg = mem_alloc(link->n * sizeof(struct link_bg));
			scr->link_bg_n = link->n;
			for (i = 0; i < link->n; i++) {
				int x = link->pos[i].x + xp - vx;
				int y = link->pos[i].y + yp - vy;
				if (x >= xp && y >= yp && x < xp+xw && y < yp+yw) {
					chr *co;
					co = get_char(t, x, y);
					scr->link_bg[i].x = x;
					scr->link_bg[i].y = y;
					scr->link_bg[i].c = co->at;
					if (t->spec->braille && !vs->brl_in_field) goto skip_link;
					if (!f || (link->type == L_CHECKBOX && i == 1) || (link->type == L_BUTTON && i == 2) || ((link->type == L_FIELD || link->type == L_AREA) && i == q)) {
						int xx = x, yy = y;
						if (link->type != L_FIELD && link->type != L_AREA) {
							if ((unsigned)(co->at & 0x38) != (link->sel_color & 0x38)) xx = xp + xw - 1, yy = yp + yw - 1;
						}
						set_cursor(t, x, y, xx, yy);
						set_window_ptr(scr->ses->win, x, y);
						f = 1;
					}
					skip_link:;
					set_color(t, x, y, link->sel_color);
				} else {
					scr->link_bg[i].x = scr->link_bg[i].y = -1;
					scr->link_bg[i].c = 0;
				}
			}
			break;
		default: internal("bad link type");
	}
}

static void free_link(struct f_data_c *scr)
{
	if (scr->link_bg) {
		mem_free(scr->link_bg);
		scr->link_bg = NULL;
	}
	scr->link_bg_n = 0;
}

static void clear_link(struct terminal *t, struct f_data_c *scr)
{
	if (scr->link_bg) {
		int i;
		for (i = scr->link_bg_n - 1; i >= 0; i--)
			set_color(t, scr->link_bg[i].x, scr->link_bg[i].y, scr->link_bg[i].c);
		free_link(scr);
	}
}

static struct search *search_lookup(struct f_data *f, int idx)
{
	static struct search sr;
	int result;
#define S_EQUAL(i, id) (f->search_pos[i].idx <= id && f->search_pos[i].idx + f->search_pos[i].co > id)
#define S_ABOVE(i, id) (f->search_pos[i].idx > id)
	BIN_SEARCH(f->nsearch_pos, S_EQUAL, S_ABOVE, idx, result)
	if (result == -1)
		internal("search_lookup: invalid index: %d, %d", idx, f->nsearch_chr);
	if (idx == f->search_pos[result].idx)
		return &f->search_pos[result];
	memcpy(&sr, &f->search_pos[result], sizeof(struct search));
	sr.x += idx - f->search_pos[result].idx;
	return &sr;
}

static int get_range(struct f_data *f, int y, int yw, int l, int *s1, int *s2)
{
	int i;
	*s1 = *s2 = -1;
	for (i = y < 0 ? 0 : y; i < y + yw && i < f->y; i++) {
		if (f->slines1[i] >= 0 && (*s1 < 0 || f->slines1[i] < *s1)) *s1 = f->slines1[i];
		if (f->slines2[i] >= 0 && (*s2 < 0 || f->slines2[i] > *s2)) *s2 = f->slines2[i];
	}

	if (l > f->nsearch_chr) *s1 = *s2 = -1;
	if (*s1 < 0 || *s2 < 0) return -1;

	if (*s1 < l) *s1 = 0;
	else *s1 -= l;

	if (f->nsearch_chr - *s2 < l) *s2 = f->nsearch_chr - l;

	if (*s1 > *s2) *s1 = *s2 = -1;
	if (*s1 < 0 || *s2 < 0) return -1;

	return 0;
}

static int is_in_range(struct f_data *f, int y, int yw, unsigned char *txt, int *min, int *max)
{
#ifdef ENABLE_UTF8
	int utf8 = f->opt.cp == utf8_table;
#else
	const int utf8 = 0;
#endif
	int found = 0;
	int l;
	int s1, s2;
	*min = MAXINT, *max = 0;

	if (!utf8) {
		l = (int)strlen(cast_const_char txt);
	} else {
		l = strlen_utf8(txt);
	}

	if (get_range(f, y, yw, l, &s1, &s2)) return 0;
	for (; s1 <= s2; s1++) {
		int i;
		if (!utf8) {
			if (f->search_chr[s1] != txt[0]) goto cont;
			for (i = 1; i < l; i++) if (f->search_chr[s1 + i] != txt[i]) goto cont;
		} else {
			unsigned char *tt = txt;
			for (i = 0; i < l; i++) {
				unsigned cc;
				GET_UTF_8(tt, cc);
				if (f->search_chr[s1 + i] != cc) goto cont;
			}
		}
		for (i = 0; i < l; i++) {
			struct search *sr = search_lookup(f, s1 + i);
			if (sr->y >= y && sr->y < y + yw && sr->n) goto in_view;
		}
		continue;
		in_view:
		found = 1;
		for (i = 0; i < l; i++) {
			struct search *sr = search_lookup(f, s1 + i);
			if (sr->n) {
				if (sr->x < *min) *min = sr->x;
				if (sr->x + sr->n > *max) *max = sr->x + sr->n;
			}
		}
		cont:;
	}
	return found;
}

static int get_searched(struct f_data_c *scr, struct point **pt, int *pl)
{
#ifdef ENABLE_UTF8
	int utf8 = term_charset(scr->ses->term) == utf8_table;
#else
	const int utf8 = 0;
#endif
	struct f_data *f = scr->f_data;
	int xp = scr->xp;
	int yp = scr->yp;
	int xw = scr->xw;
	int yw = scr->yw;
	int vx = scr->vs->view_posx;
	int vy = scr->vs->view_pos;
	int s1, s2;
	int l;
	unsigned c;
	struct point *points = DUMMY;
	int len = 0;
	unsigned char *ww;
	unsigned char *w = scr->ses->search_word;
	if (!w || !*w) return -1;
	if (get_search_data(f) < 0) {
		mem_free(scr->ses->search_word);
		scr->ses->search_word = NULL;
		return -1;
	}
	if (!utf8) {
		l = (int)strlen(cast_const_char w);
		c = w[0];
	} else {
		l = strlen_utf8(w);
		ww = w;
		GET_UTF_8(ww, c);
	}
	if (get_range(f, scr->vs->view_pos, scr->yw, l, &s1, &s2)) goto ret;
	for (; s1 <= s2; s1++) {
		int i, j;
		if (f->search_chr[s1] != c) {
			c:continue;
		}
		if (!utf8) {
			for (i = 1; i < l; i++) if (f->search_chr[s1 + i] != w[i]) goto c;
		} else {
			ww = w;
			for (i = 0; i < l; i++) {
				unsigned cc;
				GET_UTF_8(ww, cc);
				if (f->search_chr[s1 + i] != cc) goto c;
			}
		}
		for (i = 0; i < l && (!scr->ses->term->spec->braille || i < 1); i++) {
			struct search *sr = search_lookup(f, s1 + i);
			for (j = 0; j < sr->n; j++) {
				int x = sr->x + j + xp - vx;
				int y = sr->y + yp - vy;
				if (x >= xp && y >= yp && x < xp + xw && y < yp + yw) {
					/*unsigned co;
					co = get_char(t, x, y);
					co = ((co >> 3) & 0x0700) | ((co << 3) & 0x3800);
					set_color(t, x, y, co);*/
					if (!(len & (ALLOC_GR - 1))) {
						struct point *points2;
						if ((unsigned)len > MAXINT / sizeof(struct point) - ALLOC_GR) goto ret;
						points2 = mem_realloc_mayfail(points, sizeof(struct point) * (len + ALLOC_GR));
						if (!points2) goto ret;
						points = points2;
					}
					points[len].x = sr->x + j;
					points[len++].y = sr->y;
				}
			}
		}
	}
	ret:
	*pt = points;
	*pl = len;
	return 0;
}

static void draw_searched(struct terminal *t, struct f_data_c *scr)
{
	int xp = scr->xp;
	int yp = scr->yp;
	int vx = scr->vs->view_posx;
	int vy = scr->vs->view_pos;
	struct point *pt;
	int len, i;
	if (get_searched(scr, &pt, &len) < 0) return;
	for (i = 0; i < len; i++) {
		int x = pt[i].x + xp - vx, y = pt[i].y + yp - vy;
		chr *co;
		unsigned char nco;
		co = get_char(t, x, y);
		nco = ((co->at >> 3) & 0x07) | ((co->at << 3) & 0x38);
		set_color(t, x, y, nco);
	}
	mem_free(pt);
}

static void draw_current_link(struct terminal *t, struct f_data_c *scr)
{
	draw_link(t, scr, scr->vs->current_link);
	draw_searched(t, scr);
}

static struct link *get_first_link(struct f_data_c *f)
{
	int i;
	struct link *l = f->f_data->links + f->f_data->nlinks;
	for (i = f->vs->view_pos; i < f->vs->view_pos + f->yw; i++)
		if (i >= 0 && i < f->f_data->y && f->f_data->lines1[i] && f->f_data->lines1[i] < l)
			l = f->f_data->lines1[i];
	if (l == f->f_data->links + f->f_data->nlinks) l = NULL;
	return l;
}

static struct link *get_last_link(struct f_data_c *f)
{
	int i;
	struct link *l = NULL;
	for (i = f->vs->view_pos; i < f->vs->view_pos + f->yw; i++)
		if (i >= 0 && i < f->f_data->y && f->f_data->lines2[i] && (!l || f->f_data->lines2[i] > l))
			l = f->f_data->lines2[i];
	return l;
}

void fixup_select_state(struct form_control *fc, struct form_state *fs)
{
	int inited = 0;
	int i;
	retry:
	if (fs->state >= 0 && fs->state < fc->nvalues && !strcmp(cast_const_char fc->values[fs->state], cast_const_char fs->value)) return;
	for (i = 0; i < fc->nvalues; i++) {
		if (!strcmp(cast_const_char fc->values[i], cast_const_char fs->value)) {
			fs->state = i;
			return;
		}
	}
	if (!inited) {
		init_ctrl(fc, fs);
		inited = 1;
		goto retry;
	}
	fs->state = 0;
	if (fs->value) mem_free(fs->value);
	if (fc->nvalues) fs->value = stracpy(fc->values[0]);
	else fs->value = stracpy(cast_uchar "");
}

static void init_ctrl(struct form_control *form, struct form_state *fs)
{
	if (fs->value) mem_free(fs->value), fs->value = NULL;
	switch (form->type) {
		case FC_TEXT:
		case FC_PASSWORD:
		case FC_TEXTAREA:
			fs->value = stracpy(form->default_value);
			fs->state = (int)strlen(cast_const_char form->default_value);
			fs->vpos = 0;
			break;
		case FC_FILE:
			fs->value = stracpy(cast_uchar "");
			fs->state = 0;
			fs->vpos = 0;
			break;
		case FC_CHECKBOX:
		case FC_RADIO:
			fs->state = form->default_state;
			break;
		case FC_SELECT:
			fs->value = stracpy(form->default_value);
			fs->state = form->default_state;
			fixup_select_state(form, fs);
			break;
	}
}

struct form_state *find_form_state(struct f_data_c *f, struct form_control *form)
{
	struct view_state *vs = f->vs;
	struct form_state *fs;
	int n = form->g_ctrl_num;
	if (n < vs->form_info_len) fs = &vs->form_info[n];
	else {
		if ((unsigned)n > MAXINT / sizeof(struct form_state) - 1) overalloc();
		fs = mem_realloc(vs->form_info, (n + 1) * sizeof(struct form_state));
		vs->form_info = fs;
		memset(fs + vs->form_info_len, 0, (n + 1 - vs->form_info_len) * sizeof(struct form_state));
		vs->form_info_len = n + 1;
		fs = &vs->form_info[n];
	}
	if (fs->form_num == form->form_num && fs->ctrl_num == form->ctrl_num && fs->g_ctrl_num == form->g_ctrl_num && /*fs->position == form->position &&*/ fs->type == form->type) return fs;
	if (fs->value) mem_free(fs->value);
	memset(fs, 0, sizeof(struct form_state));
	fs->form_num = form->form_num;
	fs->ctrl_num = form->ctrl_num;
	fs->g_ctrl_num = form->g_ctrl_num;
	fs->position = form->position;
	fs->type = form->type;
	init_ctrl(form, fs);
	return fs;
}

static void draw_form_entry(struct terminal *t, struct f_data_c *f, struct link *l)
{
	int xp = f->xp;
	int yp = f->yp;
	int xw = f->xw;
	int yw = f->yw;
	struct view_state *vs = f->vs;
	int vx = vs->view_posx;
	int vy = vs->view_pos;
	struct form_state *fs;
	struct form_control *form = l->form;
	int i, x, y;
	if (!form) {
		internal("link %d has no form", (int)(l - f->f_data->links));
		return;
	}
	if (!(fs = find_form_state(f, form))) return;
	switch (form->type) {
		unsigned char *s;
		struct line_info *ln, *lnx;
		int sl;
		case FC_TEXT:
		case FC_PASSWORD:
		case FC_FILE:
				/*
				if (fs->state >= fs->vpos + form->size) fs->vpos = fs->state - form->size + 1;
				if (fs->state < fs->vpos) fs->vpos = fs->state;
				*/
			if ((size_t)fs->vpos > strlen(cast_const_char fs->value)) fs->vpos = (int)strlen(cast_const_char fs->value);
			while ((size_t)fs->vpos < strlen(cast_const_char fs->value) && textptr_diff(fs->value + fs->state, fs->value + fs->vpos, f->f_data->opt.cp) >= form->size) {
				unsigned char *p = fs->value + fs->vpos;
				FWD_UTF_8(p);
				fs->vpos = (int)(p - fs->value);
			}
			while (fs->vpos > fs->state) {
				unsigned char *p = fs->value + fs->vpos;
				BACK_UTF_8(p, fs->value);
				fs->vpos = (int)(p - fs->value);
			}
			if (!l->n) break;
			x = l->pos[0].x + xp - vx; y = l->pos[0].y + yp - vy;
			s = fs->value + fs->vpos;
			for (i = 0; i < form->size; i++, x++) {
				unsigned ch;
				if (!*s) {
					ch = '_';
				} else {
					if (f->f_data->opt.cp != utf8_table) {
						ch = *s++;
					} else {
						GET_UTF_8(s, ch);
					}
					if (form->type == FC_PASSWORD) {
						ch = '*';
					}
				}
				if (x >= xp && y >= yp && x < xp+xw && y < yp+yw) {
					set_only_char(t, x, y, ch, 0);
				}
			}
			break;
		case FC_TEXTAREA:
			if (!l->n) break;
			x = l->pos[0].x + xp - vx; y = l->pos[0].y + yp - vy;
			area_cursor(f, form, fs);
			lnx = format_text(fs->value, form->cols, form->wrap, f->f_data->opt.cp);
			ln = lnx;
			sl = fs->vypos;
			while (ln->st && sl) sl--, ln++;
			for (; ln->st && y < l->pos[0].y + yp - vy + form->rows; ln++, y++) {
				s = textptr_add(ln->st, fs->vpos, f->f_data->opt.cp);
				for (i = 0; i < form->cols; i++) {
					unsigned ch;
					if (s >= ln->en) {
						ch = '_';
					} else {
						if (f->f_data->opt.cp != utf8_table) {
							ch = *s++;
						} else {
							GET_UTF_8(s, ch);
						}
					}
					if (x+i >= xp && y >= yp && x+i < xp+xw && y < yp+yw) {
						set_only_char(t, x+i, y, ch, 0);
					}
				}
			}
			for (; y < l->pos[0].y + yp - vy + form->rows; y++) {
				for (i = 0; i < form->cols; i++) {
					if (x+i >= xp && y >= yp && x+i < xp+xw && y < yp+yw)
						set_only_char(t, x+i, y, '_', 0);
				}
			}

			break;
		case FC_CHECKBOX:
			if (l->n < 2) break;
			x = l->pos[1].x + xp - vx;
			y = l->pos[1].y + yp - vy;
			if (x >= xp && y >= yp && x < xp+xw && y < yp+yw)
				set_only_char(t, x, y, fs->state ? 'X' : ' ', 0);
			break;
		case FC_RADIO:
			if (l->n < 2) break;
			x = l->pos[1].x + xp - vx;
			y = l->pos[1].y + yp - vy;
			if (x >= xp && y >= yp && x < xp+xw && y < yp+yw)
				set_only_char(t, x, y, fs->state ? 'X' : ' ', 0);
			break;
		case FC_SELECT:
			fixup_select_state(form, fs);
			s = fs->state < form->nvalues ? form->labels[fs->state] : NULL;
			if (!s) s = cast_uchar "";
			for (i = 0; i < l->n; i++) {
				unsigned chr;
				if (!*s) {
					chr = '_';
				} else {
#ifdef ENABLE_UTF8
					if (term_charset(t) == utf8_table) {
						GET_UTF_8(s, chr);
					} else
#endif
					chr = *s++;
				}
				x = l->pos[i].x + xp - vx;
				y = l->pos[i].y + yp - vy;
				if (x >= xp && y >= yp && x < xp+xw && y < yp+yw)
					set_only_char(t, x, y, chr, 0);
			}
			break;
		case FC_SUBMIT:
		case FC_IMAGE:
		case FC_RESET:
		case FC_HIDDEN:
		case FC_BUTTON:
			break;
	}
}

struct xdfe {
	struct f_data_c *f;
	struct link *l;
};

static void y_draw_form_entry(struct terminal *t, struct xdfe *x)
{
	draw_form_entry(t, x->f, x->l);
}

static void x_draw_form_entry(struct session *ses, struct f_data_c *f, struct link *l)
{
	struct xdfe x;
	x.f = f, x.l = l;
	draw_to_window(ses->win, (void (*)(struct terminal *, void *))y_draw_form_entry, &x);
}

static void draw_forms(struct terminal *t, struct f_data_c *f)
{
	struct link *l1 = get_first_link(f);
	struct link *l2 = get_last_link(f);
	if (!l1 || !l2) {
		if (l1 || l2) internal("get_first_link == %p, get_last_link == %p", (void *)l1, (void *)l2);
		return;
	}
	do {
		if (l1->type != L_LINK) draw_form_entry(t, f, l1);
	} while (l1++ < l2);
}

/* 0 -> 1 <- 2 v 3 ^ */

static unsigned char fr_trans[2][4] = {{0xb3, 0xc3, 0xb4, 0xc5}, {0xc4, 0xc2, 0xc1, 0xc5}};

static void set_xchar(struct terminal *t, int x, int y, unsigned dir)
{
	chr *co;
	if (x < 0 || x >= t->x || y < 0 || y >= t->y) return;
	co = get_char(t, x, y);
	if (!(co->at & ATTR_FRAME)) return;
	if (co->ch == fr_trans[dir / 2][0]) set_only_char(t, x, y, fr_trans[dir / 2][1 + (dir & 1)], ATTR_FRAME);
	else if (co->ch == fr_trans[dir / 2][2 - (dir & 1)]) set_only_char(t, x, y, fr_trans[dir / 2][3], ATTR_FRAME);
}

static void draw_frame_lines(struct session *ses, struct frameset_desc *fsd, int xp, int yp)
{
	struct terminal *t = ses->term;
	int i, j;
	int x, y;
	if (!fsd) return;
	y = yp - 1;
	for (j = 0; j < fsd->y; j++) {
		int wwy = fsd->f[j * fsd->x].yw;
		x = xp - 1;
		for (i = 0; i < fsd->x; i++) {
			int wwx = fsd->f[i].xw;
			if (i) {
				fill_area(t, x, y + 1, 1, wwy, 179, ATTR_FRAME | get_attribute(ses->ds.t_text_color, ses->ds.t_background_color));
				if (j == fsd->y - 1) set_xchar(t, x, y + wwy + 1, 3);
			} else if (j) set_xchar(t, x, y, 0);
			if (j) {
				fill_area(t, x + 1, y, wwx, 1, 196, ATTR_FRAME | get_attribute(ses->ds.t_text_color, ses->ds.t_background_color));
				if (i == fsd->x - 1) set_xchar(t, x + wwx + 1, y, 1);
			} else if (i) set_xchar(t, x, y, 2);
			if (i && j) set_char(t, x, y, 197, ATTR_FRAME | get_attribute(ses->ds.t_text_color, ses->ds.t_background_color));
			/*if (fsd->f[j * fsd->x + i].subframe) {
				draw_frame_lines(ses, fsd->f[j * fsd->x + i].subframe, x + 1, y + 1);
			}*/
			x += wwx + 1;
		}
		y += wwy + 1;
	}
}

static void set_brl_cursor(struct terminal *t, struct f_data_c *scr)
{
	set_cursor(t, scr->xp + scr->vs->brl_x - scr->vs->view_posx, scr->yp + scr->vs->brl_y - scr->vs->view_pos, scr->xp + scr->vs->brl_x - scr->vs->view_posx, scr->yp + scr->vs->brl_y - scr->vs->view_pos);
	set_window_ptr(scr->ses->win, scr->xp + scr->vs->brl_x - scr->vs->view_posx, scr->yp + scr->vs->brl_y - scr->vs->view_pos);
}

void draw_doc(struct terminal *t, struct f_data_c *scr)
{
	struct session *ses = scr->ses;
	int active = scr->active;
	int y;
	int xp = scr->xp;
	int yp = scr->yp;
	int xw = scr->xw;
	int yw = scr->yw;
	struct view_state *vs;
	int vx, vy;
	if (!scr->vs || !scr->f_data) {
		if (!F) {
			if (active) {
				if (!scr->parent) set_cursor(t, 0, 0, 0, 0);
				else set_cursor(t, xp, yp, xp, yp);
			}
			fill_area(t, xp, yp, xw, yw, ' ', get_attribute(ses->ds.t_text_color, ses->ds.t_background_color));
#ifdef G
		} else {
			long color = dip_get_color_sRGB(ses->ds.g_background_color /* 0x808080 */);
			drv->fill_area(t->dev, xp, yp, xp + xw, yp + yw, color);
#endif
		}
		if (active) set_window_ptr(ses->win, xp, yp);
		return;
	}
	if (active) {
		if (!F) {
			if (!t->spec->braille) {
				set_cursor(t, xp + xw - 1, yp + yw - 1, xp + xw - 1, yp + yw - 1);
				set_window_ptr(ses->win, xp, yp);
			} else {
				set_brl_cursor(t, scr);
			}
		}
	}
	check_vs(scr);
	if (scr->f_data->frame_desc) {
		struct f_data_c *f;
		int n;
		if (!F) {
			fill_area(t, xp, yp, xw, yw, ' ', scr->f_data->y ? scr->f_data->bg : 0);
			draw_frame_lines(ses, scr->f_data->frame_desc, xp, yp);
		}
		n = 0;
		foreach(f, scr->subframes) {
			f->active = active && n++ == scr->vs->frame_pos;
			draw_doc(t, f);
		}
		return;
	}
	vs = scr->vs;
	if (scr->goto_position && (vy = find_tag(scr->f_data, scr->goto_position)) != -1) {
		if (vy > scr->f_data->y) vy = scr->f_data->y - 1;
		if (vy < 0) vy = 0;
		vs->view_pos = vy;
		vs->orig_view_pos = vy;
		vs->view_posx = 0;
		vs->orig_view_posx = 0;
		if (t->spec->braille) {
			vs->brl_y = vy;
			vs->brl_x = 0;
			vs->orig_brl_y = vy;
			vs->orig_brl_x = 0;
		}
		if (!F) set_link(scr);
		if (scr->went_to_position) mem_free(scr->went_to_position);
		scr->went_to_position = scr->goto_position;
		scr->goto_position = NULL;
		if (t->spec->braille) set_brl_cursor(t, scr);
	}
	if (vs->view_pos != vs->orig_view_pos || vs->view_posx != vs->orig_view_posx || vs->current_link != vs->orig_link || (t->spec->braille && (vs->brl_x != vs->orig_brl_x || vs->brl_y != vs->orig_brl_y))) {
		int ol;
		vs->view_pos = vs->orig_view_pos;
		vs->view_posx = vs->orig_view_posx;
		vs->brl_x = vs->orig_brl_x;
		vs->brl_y = vs->orig_brl_y;
		ol = vs->orig_link;
		if (ol < scr->f_data->nlinks) vs->current_link = ol;
		if (!F) {
			while (vs->view_pos >= scr->f_data->y) vs->view_pos -= yw ? yw : 1;
			if (vs->view_pos < 0) vs->view_pos = 0;
		}
		if (!F && !t->spec->braille) set_link(scr);
		check_vs(scr);
		if (!t->spec->braille) {
			vs->orig_link = ol;
		} else {
			vs->orig_link = vs->current_link;
			set_brl_cursor(t, scr);
		}
	}
	if (!F) {
		vx = vs->view_posx;
		vy = vs->view_pos;
		if (scr->xl == vx && scr->yl == vy && scr->xl != -1 && !ses->search_word) {
			clear_link(t, scr);
			draw_forms(t, scr);
			if (active) draw_current_link(t, scr);
			return;
		}
		free_link(scr);
		scr->xl = vx;
		scr->yl = vy;
		fill_area(t, xp, yp, xw, yw, ' ', scr->f_data->y ? scr->f_data->bg : get_attribute(ses->ds.t_text_color, ses->ds.t_background_color));
		if (!scr->f_data->y) return;
		while (vs->view_pos >= scr->f_data->y) vs->view_pos -= yw ? yw : 1;
		if (vs->view_pos < 0) vs->view_pos = 0;
		if (vy != vs->view_pos) vy = vs->view_pos, check_vs(scr);
		for (y = vy <= 0 ? 0 : vy; y < (-vy + scr->f_data->y <= yw ? scr->f_data->y : yw + vy); y++) {
			int st = vx <= 0 ? 0 : vx;
			int en = -vx + scr->f_data->data[y].l <= xw ? scr->f_data->data[y].l : xw + vx;
			set_line(t, xp + st - vx, yp + y - vy, en - st, &scr->f_data->data[y].d[st]);
		}
		draw_forms(t, scr);
		if (active) draw_current_link(t, scr);
		if (ses->search_word) scr->xl = scr->yl = -1;
#ifdef G
	} else {
		draw_graphical_doc(t, scr, active);
#endif
	}
}

static void clr_xl(struct f_data_c *fd)
{
	struct f_data_c *fdd;
	fd->xl = fd->yl = -1;
	foreach(fdd, fd->subframes) clr_xl(fdd);
}

static void draw_doc_c(struct terminal *t, struct f_data_c *scr)
{
	clr_xl(scr);
#ifdef G
	if (F) if (scr == scr->ses->screen) draw_title(scr);
#endif
	draw_doc(t, scr);
}

void draw_formatted(struct session *ses)
{
	/*clr_xl(ses->screen);*/
	ses->screen->active = 1;
	draw_to_window(ses->win, (void (*)(struct terminal *, void *))draw_doc_c, ses->screen);
	change_screen_status(ses);
	print_screen_status(ses);
}

void draw_fd(struct f_data_c *f)
{
	if (f->f_data) f->f_data->time_to_draw = -get_time();
	f->active = is_active_frame(f->ses, f);
	draw_to_window(f->ses->win, (void (*)(struct terminal *, void *))draw_doc_c, f);
	change_screen_status(f->ses);
	print_screen_status(f->ses);
	if (f->f_data) f->f_data->time_to_draw += (uttime)get_time();
}

static void draw_fd_nrd(struct f_data_c *f)
{
	f->active = is_active_frame(f->ses, f);
	draw_to_window(f->ses->win, (void (*)(struct terminal *, void *))draw_doc, f);
	change_screen_status(f->ses);
	print_screen_status(f->ses);
}

#define D_BUF	65536

int dump_to_file(struct f_data *fd, int h)
{
	int x, y;
	unsigned char *buf;
	int bptr = 0;
	buf = mem_alloc(D_BUF);
	for (y = 0; y < fd->y; y++) for (x = 0; x <= fd->data[y].l; x++) {
		unsigned c;
		if (x == fd->data[y].l) c = '\n';
		else {
			c = fd->data[y].d[x].ch;
			if (c == 1) c = ' ';
			if (fd->data[y].d[x].at & ATTR_FRAME && c >= 176 && c < 224) c = frame_dumb[c - 176];
		}
#ifdef ENABLE_UTF8
		if (fd->opt.cp == utf8_table && c >= 0x80) {
			unsigned char *enc = encode_utf_8(c);
			strcpy(cast_char(buf + bptr), cast_const_char enc);
			bptr += (int)strlen(cast_const_char enc);
		} else
#endif
		{
			buf[bptr++] = (unsigned char)c;
		}
		if (bptr >= D_BUF - 7) {
			if (hard_write(h, buf, bptr) != bptr) goto fail;
			bptr = 0;
		}
	}
	if (hard_write(h, buf, bptr) != bptr) {
		fail:
		mem_free(buf);
		return -1;
	}
	mem_free(buf);
	if (fd->opt.num_links && fd->nlinks) {
		static unsigned char head[] = "\nLinks:\n";
		int i;
		if ((int)hard_write(h, head, (int)strlen(cast_const_char head)) != (int)strlen(cast_const_char head)) return -1;
		for (i = 0; i < fd->nlinks; i++) {
			struct link *lnk = &fd->links[i];
			unsigned char *s = init_str();
			int l = 0;
			add_num_to_str(&s, &l, i + 1);
			add_to_str(&s, &l, cast_uchar ". ");
			if (lnk->where) {
				add_to_str(&s, &l, lnk->where);
			} else if (lnk->where_img) {
				add_to_str(&s, &l, cast_uchar "Image: ");
				add_to_str(&s, &l, lnk->where_img);
			} else if (lnk->type == L_BUTTON) {
				struct form_control *fc = lnk->form;
				if (fc->type == FC_RESET) add_to_str(&s, &l, cast_uchar "Reset form");
				else if (fc->type == FC_BUTTON || !fc->action) add_to_str(&s, &l, cast_uchar "Button");
				else {
					if (fc->method == FM_GET) add_to_str(&s, &l, cast_uchar "Submit form: ");
					else add_to_str(&s, &l, cast_uchar "Post form: ");
					add_to_str(&s, &l, fc->action);
				}
			} else if (lnk->type == L_CHECKBOX || lnk->type == L_SELECT || lnk->type == L_FIELD || lnk->type == L_AREA) {
				struct form_control *fc = lnk->form;
				if (fc->type == FC_RADIO) add_to_str(&s, &l, cast_uchar "Radio button");
				else if (fc->type == FC_CHECKBOX) add_to_str(&s, &l, cast_uchar "Checkbox");
				else if (fc->type == FC_SELECT) add_to_str(&s, &l, cast_uchar "Select field");
				else if (fc->type == FC_TEXT) add_to_str(&s, &l, cast_uchar "Text field");
				else if (fc->type == FC_TEXTAREA) add_to_str(&s, &l, cast_uchar "Text area");
				else if (fc->type == FC_FILE) add_to_str(&s, &l, cast_uchar "File upload");
				else if (fc->type == FC_PASSWORD) add_to_str(&s, &l, cast_uchar "Password field");
				else goto unknown;
				if (fc->name && fc->name[0]) add_to_str(&s, &l, cast_uchar ", Name "), add_to_str(&s, &l, fc->name);
				if ((fc->type == FC_CHECKBOX || fc->type == FC_RADIO) && fc->default_value && fc->default_value[0]) add_to_str(&s, &l, cast_uchar ", Value "), add_to_str(&s, &l, fc->default_value);
			}
			unknown:
			add_to_str(&s, &l, cast_uchar "\n");
			if (hard_write(h, s, l) != l) {
				mem_free(s);
				return -1;
			}
			mem_free(s);
		}
	}
	return 0;
}

static int in_viewx(struct f_data_c *f, struct link *l)
{
	int i;
	for (i = 0; i < l->n; i++) {
		if (l->pos[i].x >= f->vs->view_posx && l->pos[i].x < f->vs->view_posx + f->xw)
			return 1;
	}
	return 0;
}

static int in_viewy(struct f_data_c *f, struct link *l)
{
	int i;
	for (i = 0; i < l->n; i++) {
		if (l->pos[i].y >= f->vs->view_pos && l->pos[i].y < f->vs->view_pos + f->yw)
			return 1;
	}
	return 0;
}

static int in_view(struct f_data_c *f, struct link *l)
{
	return in_viewy(f, l) && in_viewx(f, l);
}

static int c_in_view(struct f_data_c *f)
{
	return f->vs->current_link != -1 && in_view(f, &f->f_data->links[f->vs->current_link]);
}

static int next_in_view(struct f_data_c *f, int p, int d, int (*fn)(struct f_data_c *, struct link *), void (*cntr)(struct f_data_c *, struct link *))
{
	int p1 = f->f_data->nlinks - 1;
	int p2 = 0;
	int y;
	int yl = f->vs->view_pos + f->yw;
	if (yl > f->f_data->y) yl = f->f_data->y;
	for (y = f->vs->view_pos < 0 ? 0 : f->vs->view_pos; y < yl; y++) {
		if (f->f_data->lines1[y] && f->f_data->lines1[y] - f->f_data->links < p1) p1 = (int)(f->f_data->lines1[y] - f->f_data->links);
		if (f->f_data->lines2[y] && f->f_data->lines2[y] - f->f_data->links > p2) p2 = (int)(f->f_data->lines2[y] - f->f_data->links);
	}
	/*while (p >= 0 && p < f->f_data->nlinks) {*/
	while (p >= p1 && p <= p2) {
		if (fn(f, &f->f_data->links[p])) {
			f->vs->current_link = p;
			f->vs->orig_link = f->vs->current_link;
			if (cntr) cntr(f, &f->f_data->links[p]);
			return 1;
		}
		p += d;
	}
	f->vs->current_link = -1;
	f->vs->orig_link = f->vs->current_link;
	return 0;
}

static void set_pos_x(struct f_data_c *f, struct link *l)
{
	int i;
	int xm = 0;
	int xl = MAXINT;
	for (i = 0; i < l->n; i++) {
		if (l->pos[i].y >= f->vs->view_pos && l->pos[i].y < f->vs->view_pos + f->yw) {
			if (l->pos[i].x >= xm) xm = l->pos[i].x + 1;
			if (l->pos[i].x < xl) xl = l->pos[i].x;
		}
	}
	if (xl == MAXINT) return;
	/*if ((f->vs->view_posx = xm - f->xw) > xl) f->vs->view_posx = xl;*/
	if (f->vs->view_posx + f->xw < xm) f->vs->view_posx = xm - f->xw;
	if (f->vs->view_posx > xl) f->vs->view_posx = xl;
	f->vs->orig_view_posx = f->vs->view_posx;
}

static void set_pos_y(struct f_data_c *f, struct link *l)
{
	int i;
	int ym = 0;
	int yl = f->f_data->y;
	for (i = 0; i < l->n; i++) {
		if (l->pos[i].y >= ym) ym = l->pos[i].y + 1;
		if (l->pos[i].y < yl) yl = l->pos[i].y;
	}
	if ((f->vs->view_pos = (ym + yl) / 2 - f->yw / 2) > f->f_data->y - f->yw) f->vs->view_pos = f->f_data->y - f->yw;
	if (f->vs->view_pos < 0) f->vs->view_pos = 0;
	f->vs->orig_view_pos = f->vs->view_pos;
}

static void update_braille_link(struct f_data_c *f)
{
	int i;
	struct link *l1, *l2;
	struct view_state *vs = f->vs;
	struct f_data *f_data = f->f_data;
	if (vs->brl_x >= f->f_data->x && f->f_data->x) vs->brl_x = f->f_data->x - 1;
	if (vs->brl_x >= vs->view_posx + f->xw) vs->view_posx = vs->brl_x - f->xw + 1;
	if (vs->brl_x < vs->view_posx) vs->view_posx = vs->brl_x;
	if (vs->brl_y >= f_data->y && f_data->y) vs->brl_y = f->f_data->y - 1;
	if (vs->brl_y >= vs->view_pos + f->yw) vs->view_pos = vs->brl_y - f->yw + 1;
	if (vs->brl_y < vs->view_pos) vs->view_pos = vs->brl_y;
	vs->orig_brl_x = vs->brl_x;
	vs->orig_brl_y = vs->brl_y;
	vs->orig_view_pos = vs->view_pos;
	vs->orig_view_posx = vs->view_posx;
	if (vs->brl_y >= f_data->y) goto no_link;
	l1 = f_data->lines1[vs->brl_y];
	l2 = f_data->lines2[vs->brl_y];
	if (!l1 || !l2) goto no_link;
	for (; l1 <= l2; l1++) {
		for (i = 0; i < l1->n; i++) if (l1->pos[i].x == vs->brl_x && l1->pos[i].y == vs->brl_y) {
			if (l1 - f_data->links != vs->current_link) vs->brl_in_field = 0;
			vs->current_link = (int)(l1 - f_data->links);
			vs->orig_link = vs->current_link;
			return;
		}
	}
	no_link:
	vs->brl_in_field = 0;
	vs->current_link = -1;
	vs->orig_link = vs->current_link;
}

static void find_link(struct f_data_c *f, int p, int s)
{ /* p=1 - top, p=-1 - bottom, s=0 - pgdn, s=1 - down */
	int y;
	int l;
	struct link *link;
	struct link **line;
	if (f->ses->term->spec->braille) {
		update_braille_link(f);
		return;
	}
	line = p == -1 ? f->f_data->lines2 : f->f_data->lines1;
	if (p == -1) {
		y = f->vs->view_pos + f->yw - 1;
		if (y >= f->f_data->y) y = f->f_data->y - 1;
	} else {
		y = f->vs->view_pos;
		if (y < 0) y = 0;
	}
	if (y < 0 || y >= f->f_data->y) goto nolink;
	link = NULL;
	do {
		if (line[y] && (!link || (p > 0 ? line[y] < link : line[y] > link))) link = line[y];
		y += p;
	} while (!(y < 0 || y < f->vs->view_pos || y >= f->vs->view_pos + f->yw || y >= f->f_data->y));
	if (!link) goto nolink;
	l = (int)(link - f->f_data->links);
	if (s == 0) {
		next_in_view(f, l, p, in_view, NULL);
		return;
	}
	f->vs->current_link = l;
	f->vs->orig_link = f->vs->current_link;
	set_pos_x(f, link);
	return;
	nolink:
	f->vs->current_link = -1;
	f->vs->orig_link = f->vs->current_link;
}

static void page_down(struct session *ses, struct f_data_c *f, int a)
{
	if (f->vs->view_pos + f->yw < f->f_data->y) {
		f->vs->view_pos += f->yw;
		f->vs->orig_view_pos = f->vs->view_pos;
		if (!ses->term->spec->braille) find_link(f, 1, a);
	} else {
		if (!ses->term->spec->braille) find_link(f, -1, a);
		else if (f->f_data->y) f->vs->brl_y = f->f_data->y - 1;
	}
	if (ses->term->spec->braille) {
		if (f->vs->view_pos > f->vs->brl_y) f->vs->brl_y = f->vs->view_pos;
		f->vs->orig_brl_y = f->vs->brl_y;
		update_braille_link(f);
	}
}

static void page_up(struct session *ses, struct f_data_c *f, int a)
{
	f->vs->view_pos -= f->yw;
	if (ses->term->spec->braille) {
		if (f->vs->view_pos + f->yw <= f->vs->brl_y) f->vs->brl_y = f->vs->view_pos + f->yw - 1;
	} else find_link(f, -1, a);
	if (f->vs->view_pos < 0) {
		f->vs->view_pos = 0;
	}
	f->vs->orig_view_pos = f->vs->view_pos;
	if (ses->term->spec->braille) {
		if (f->vs->brl_y < 0) f->vs->brl_y = 0;
		f->vs->orig_brl_y = f->vs->brl_y;
		update_braille_link(f);
	}
}

static void down(struct session *ses, struct f_data_c *f, int a)
{
	int l;
	if (ses->term->spec->braille) {
		if (f->vs->brl_y < f->f_data->y - 1) f->vs->brl_y++;
		else if (f->f_data->y) f->vs->brl_y = f->f_data->y - 1;
		else f->vs->brl_y = 0;
		f->vs->orig_brl_y = f->vs->brl_y;
		if (f->vs->brl_y >= f->vs->view_pos + f->yw) {
			page_down(ses, f, 1);
			return;
		}
		update_braille_link(f);
		return;
	}
	l = f->vs->current_link;
	/*if (f->vs->current_link >= f->nlinks - 1) return;*/
	if (f->vs->current_link == -1 || !next_in_view(f, f->vs->current_link+1, 1, in_viewy, set_pos_x)) page_down(ses, f, 1);
	if (l != f->vs->current_link) set_textarea(ses, f, KBD_UP);
}

static void up(struct session *ses, struct f_data_c *f, int a)
{
	int l;
	if (ses->term->spec->braille) {
		if (f->vs->brl_y > 0) f->vs->brl_y--;
		else f->vs->brl_y = 0;
		f->vs->orig_brl_y = f->vs->brl_y;
		if (f->vs->brl_y < f->vs->view_pos) {
			page_up(ses, f, 0);
			return;
		}
		update_braille_link(f);
		return;
	}
	l = f->vs->current_link;
	if (f->vs->current_link == -1 || !next_in_view(f, f->vs->current_link-1, -1, in_viewy, set_pos_x)) page_up(ses, f, 1);
	if (l != f->vs->current_link) set_textarea(ses, f, KBD_DOWN);
}

static void scroll(struct session *ses, struct f_data_c *f, int a)
{
	if (f->vs->view_pos + f->yw >= f->f_data->y && a > 0) return;
	f->vs->view_pos += a;
	if (f->vs->view_pos > f->f_data->y - f->yw && a > 0) f->vs->view_pos = f->f_data->y - f->yw;
	if (f->vs->view_pos < 0) f->vs->view_pos = 0;
	f->vs->orig_view_pos = f->vs->view_pos;
	if (ses->term->spec->braille) {
		if (f->vs->view_pos + f->yw <= f->vs->brl_y) f->vs->brl_y = f->vs->view_pos + f->yw - 1;
		if (f->vs->view_pos > f->vs->brl_y) f->vs->brl_y = f->vs->view_pos;
		f->vs->orig_brl_y = f->vs->brl_y;
		update_braille_link(f);
		return;
	}
	if (c_in_view(f)) return;
	find_link(f, a < 0 ? -1 : 1, 0);
}

static void hscroll(struct session *ses, struct f_data_c *f, int a)
{
	f->vs->view_posx += a;
	if (f->vs->view_posx >= f->f_data->x) f->vs->view_posx = f->f_data->x - 1;
	if (f->vs->view_posx < 0) f->vs->view_posx = 0;
	f->vs->orig_view_posx = f->vs->view_posx;
	if (ses->term->spec->braille) {
		if (f->vs->view_posx + f->xw <= f->vs->brl_x) f->vs->brl_x = f->vs->view_posx + f->xw - 1;
		if (f->vs->view_posx > f->vs->brl_x) f->vs->brl_x = f->vs->view_posx;
		f->vs->orig_brl_x = f->vs->brl_x;
		update_braille_link(f);
		return;
	}
	if (c_in_view(f)) return;
	find_link(f, 1, 0);
	/* !!! FIXME: check right margin */
}

static void right(struct session *ses, struct f_data_c *f, int a)
{
	if (ses->term->spec->braille) {
		if (f->vs->brl_x < f->f_data->x - 1) f->vs->brl_x++;
		else if (f->f_data->x) f->vs->brl_x = f->f_data->x - 1;
		else f->vs->brl_x = 0;
		f->vs->orig_brl_x = f->vs->brl_x;
		if (f->vs->brl_x >= f->vs->view_posx + f->xw) {
			hscroll(ses, f, 1);
			return;
		}
		update_braille_link(f);
		return;
	}
}

static void left(struct session *ses, struct f_data_c *f, int a)
{
	if (ses->term->spec->braille) {
		if (f->vs->brl_x > 0) f->vs->brl_x--;
		else f->vs->brl_x = 0;
		f->vs->orig_brl_x = f->vs->brl_x;
		if (f->vs->brl_x < f->vs->view_posx) {
			hscroll(ses, f, -1);
			return;
		}
		update_braille_link(f);
		return;
	}
}

static int get_at_pos(struct f_data *f, int x, int y)
{
	chr *ch;
	struct line *ln;
	if (y < 0 || y >= f->y) return -1;
	ln = &f->data[y];
	if (x < 0 || x >= ln->l) return 0;
	ch = &ln->d[x];
	if (ch->at & ATTR_FRAME) return 0;
	return ch->ch != 0 && ch->ch != 1 && ch->ch != ' ' && ch->ch != '~';
}

static void cursor_word(struct session *ses, struct f_data_c *f, int a)
{
	if (ses->term->spec->braille) {
		int p = 1;
		int q;
		int x = f->vs->brl_x, y = f->vs->brl_y;
		while (1) {
			q = get_at_pos(f->f_data, x, y);
			if (q == -1) return;
			if (!p && q) {
				f->vs->brl_x = x;
				f->vs->brl_y = y;
				f->vs->orig_brl_x = f->vs->brl_x;
				f->vs->orig_brl_y = f->vs->brl_y;
				update_braille_link(f);
				return;
			}
			x++;
			if (x >= f->f_data->x) x = 0, y++;
			p = q;
		}
	}
}

static void cursor_word_back(struct session *ses, struct f_data_c *f, int a)
{
	if (ses->term->spec->braille) {
		int p = 0;
		int q;
		int x = f->vs->brl_x, y = f->vs->brl_y;
		int px, py;
		while (1) {
			px = x, py = y;
			x--;
			if (x < 0) x = f->f_data->x - 1, y--;
			if (x < 0) x = 0;
			q = get_at_pos(f->f_data, x, y);
			if (q == -1) return;
			if (p && !q) {
				f->vs->brl_x = px;
				f->vs->brl_y = py;
				f->vs->orig_brl_x = f->vs->brl_x;
				f->vs->orig_brl_y = f->vs->brl_y;
				update_braille_link(f);
				return;
			}
			p = q;
		}
	}
}

static void cursor_home(struct session *ses, struct f_data_c *f, int a)
{
	if (ses->term->spec->braille) {
		f->vs->brl_x = 0;
		f->vs->orig_brl_x = f->vs->brl_x;
		update_braille_link(f);
		return;
	}
}

static void cursor_end(struct session *ses, struct f_data_c *f, int a)
{
	if (ses->term->spec->braille) {
		if (f->f_data->x) f->vs->brl_x = f->f_data->x - 1;
		else f->vs->brl_x = 0;
		f->vs->orig_brl_x = f->vs->brl_x;
		update_braille_link(f);
		return;
	}
}


static void br_next_link(struct session *ses, struct f_data_c *f, int a)
{
	if (ses->term->spec->braille) {
		int y;
		struct link *l, *ol, *cl;
		struct view_state *vs = f->vs;
		struct f_data *f_data = f->f_data;
		if (vs->brl_y >= f_data->y) return;
		for (y = vs->brl_y; y < f_data->y; y++) if (f_data->lines1[y]) goto o;
		return;
		o:
		cl = NULL, ol = NULL;
		for (l = f_data->lines1[y]; l && l < f_data->links + f_data->nlinks && (!cl || l <= cl); l++) {
			if (!l->n) continue;
			if (a && !l->form) continue;
			if (l->pos[0].y > vs->brl_y || (l->pos[0].y == vs->brl_y && l->pos[0].x > vs->brl_x)) if (vs->current_link == -1 || l != f_data->links + vs->current_link) {
				if (!ol || l->pos[0].y < ol->pos[0].y || (l->pos[0].y == ol->pos[0].y && l->pos[0].x < ol->pos[0].x)) {
					ol = l;
					cl = f_data->lines2[ol->pos[0].y];
				}
			}
		}
		if (!ol) return;
		vs->brl_x = ol->pos[0].x;
		vs->brl_y = ol->pos[0].y;
		while (vs->brl_y >= vs->view_pos + f->yw) {
			vs->view_pos += f->yw ? f->yw : 1;
			if (vs->view_pos >= f_data->y) vs->view_pos = f_data->y - !!f_data->y;
			vs->orig_view_pos = vs->view_pos;
		}
		vs->orig_brl_x = vs->brl_x;
		vs->orig_brl_y = vs->brl_y;
		set_pos_x(f, ol);
		update_braille_link(f);
	}
}

static void br_prev_link(struct session *ses, struct f_data_c *f, int a)
{
	if (ses->term->spec->braille) {
		int y;
		struct link *l, *ol, *cl;
		struct view_state *vs = f->vs;
		struct f_data *f_data = f->f_data;
		if (vs->brl_y >= f_data->y) return;
		for (y = vs->brl_y; y >= 0; y--) if (f_data->lines2[y]) goto o;
		return;
		o:
		cl = NULL, ol = NULL;
		for (l = f_data->lines2[y]; l && l >= f_data->links && (!cl || l >= cl); l--) {
			if (!l->n) goto cont;
			if (l->pos[0].y < vs->brl_y || (l->pos[0].y == vs->brl_y && l->pos[0].x < vs->brl_x)) if (vs->current_link == -1 || l != f_data->links + vs->current_link) {
				if (!ol || l->pos[0].y > ol->pos[0].y || (l->pos[0].y == ol->pos[0].y && l->pos[0].x > ol->pos[0].x)) {
					ol = l;
					cl = f_data->lines1[ol->pos[0].y];
				}
			}
			cont:
			if (l == f_data->links) break;
		}
		if (!ol) return;
		vs->brl_x = ol->pos[0].x;
		vs->brl_y = ol->pos[0].y;
		while (vs->brl_y < vs->view_pos) {
			vs->view_pos -= f->yw ? f->yw : 1;
			if (vs->view_pos < 0) vs->view_pos = 0;
			vs->orig_view_pos = vs->view_pos;
		}
		vs->orig_brl_x = vs->brl_x;
		vs->orig_brl_y = vs->brl_y;
		set_pos_x(f, ol);
		update_braille_link(f);
	}
}

static void home(struct session *ses, struct f_data_c *f, int a)
{
	f->vs->view_pos = f->vs->view_posx = 0;
	f->vs->orig_view_pos = f->vs->view_pos;
	f->vs->orig_view_posx = f->vs->view_posx;
	if (ses->term->spec->braille) {
		f->vs->brl_x = f->vs->brl_y = 0;
		f->vs->orig_brl_x = f->vs->brl_x;
		f->vs->orig_brl_y = f->vs->brl_y;
		update_braille_link(f);
		return;
	}
	find_link(f, 1, 0);
}

static void x_end(struct session *ses, struct f_data_c *f, int a)
{
	f->vs->view_posx = 0;
	if (f->vs->view_pos < f->f_data->y - f->yw) f->vs->view_pos = f->f_data->y - f->yw;
	if (f->vs->view_pos < 0) f->vs->view_pos = 0;
	f->vs->orig_view_pos = f->vs->view_pos;
	f->vs->orig_view_posx = f->vs->view_posx;
	if (ses->term->spec->braille) {
		if (f->f_data->y) f->vs->brl_y = f->f_data->y - 1;
		else f->vs->brl_y = 0;
		f->vs->brl_x = 0;
		f->vs->orig_brl_x = f->vs->brl_x;
		f->vs->orig_brl_y = f->vs->brl_y;
		update_braille_link(f);
		return;
	}
	find_link(f, -1, 0);
}

static int has_form_submit(struct f_data *f, struct form_control *form)
{
	struct form_control *i;
	int q = 0;
	/*if (F) return 0;*/
	foreach (i, f->forms) if (i->form_num == form->form_num) {
		if ((i->type == FC_SUBMIT || i->type == FC_IMAGE)) return 1;
		q = 1;
	}
	if (!q) internal("form is not on list");
	return 0;
}

struct submitted_value {
	struct submitted_value *next;
	struct submitted_value *prev;
	int type;
	unsigned char *name;
	unsigned char *value;
	void *file_content;
	int fc_len;
	int position;
};

static void free_succesful_controls(struct list_head *submit)
{
	struct submitted_value *v;
	foreach(v, *submit) {
		if (v->name) mem_free(v->name);
		if (v->value) mem_free(v->value);
		if (v->file_content) mem_free(v->file_content);
	}
	free_list(*submit);
}

static unsigned char *encode_textarea(unsigned char *t)
{
	int len = 0;
	unsigned char *o = init_str();
	for (; *t; t++) {
		if (*t != '\n') add_chr_to_str(&o, &len, *t);
		else add_to_str(&o, &len, cast_uchar "\r\n");
	}
	return o;
}

static int compare_submitted(struct submitted_value *sub1, struct submitted_value *sub2)
{
	/*int c = (sub1->type == FC_IMAGE) - (sub2->type == FC_IMAGE);
	if (c) return c;*/
	return sub1->position - sub2->position;
}

static void get_succesful_controls(struct f_data_c *f, struct form_control *fc, struct list_head *subm)
{
	int ch;
	struct form_control *form;
	init_list(*subm);
	foreach(form, f->f_data->forms) {
		if (form->form_num == fc->form_num && ((form->type != FC_SUBMIT && form->type != FC_IMAGE && form->type != FC_RESET && form->type != FC_BUTTON) || form == fc) && form->name && form->name[0] && form->ro != 2) {
			struct submitted_value *sub;
			struct form_state *fs;
			int fi = form->type == FC_IMAGE && form->default_value && *form->default_value ? -1 : 0;
			int svl;
			if (!(fs = find_form_state(f, form))) continue;
			if ((form->type == FC_CHECKBOX || form->type == FC_RADIO) && !fs->state) continue;
			if (form->type == FC_BUTTON) continue;
			if (form->type == FC_SELECT && !form->nvalues) continue;
			fi_rep:
			sub = mem_calloc(sizeof(struct submitted_value));
			sub->type = form->type;
			sub->name = stracpy(form->name);
			switch (form->type) {
				case FC_TEXT:
				case FC_PASSWORD:
				case FC_FILE:
					sub->value = stracpy(fs->value);
					break;
				case FC_TEXTAREA:
					sub->value = encode_textarea(fs->value);
					break;
				case FC_CHECKBOX:
				case FC_RADIO:
				case FC_SUBMIT:
				case FC_HIDDEN:
					sub->value = encode_textarea(form->default_value);
					break;
				case FC_SELECT:
					fixup_select_state(form, fs);
					sub->value = encode_textarea(fs->value);
					break;
				case FC_IMAGE:
					if (fi == -1) {
						sub->value = encode_textarea(form->default_value);
						break;
					}
					add_to_strn(&sub->name, fi ? cast_uchar ".x" : cast_uchar ".y");
					/*sub->value = stracpy("0");*/
					sub->value = init_str();
					svl = 0;
					add_num_to_str(&sub->value, &svl, fi ? ismap_x : ismap_y);
					break;
				default:
					internal("bad form control type");
					mem_free(sub);
					continue;
			}
			sub->position = form->form_num + form->ctrl_num;
			add_to_list(*subm, sub);
			if (form->type == FC_IMAGE && fi < 1) {
				fi++;
				goto fi_rep;
			}
		}
	}
	do {
		struct submitted_value *sub, *nx;
		ch = 0;
		foreach(sub, *subm) if (sub->next != (void *)subm)
			if (compare_submitted(sub->next, sub) < 0) {
				nx = sub->next;
				del_from_list(sub);
				add_at_pos(nx, sub);
				sub = nx;
				ch = 1;
			}
		foreachback(sub, *subm) if (sub->next != (void *)subm)
			if (compare_submitted(sub->next, sub) < 0) {
				nx = sub->next;
				del_from_list(sub);
				add_at_pos(nx, sub);
				sub = nx;
				ch = 1;
			}
	} while (ch);
}

static unsigned char *strip_file_name(unsigned char *f)
{
	unsigned char *n;
	unsigned char *l = f - 1;
	for (n = f; *n; n++) if (dir_sep(*n)) l = n;
	return l + 1;
}

static inline int safe_char(unsigned char c)
{
	return (c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c== '.' || c == '-' || c == '_';
}

static void encode_string(unsigned char *name, unsigned char **data, int *len)
{
	for (; *name; name++) {
		if (*name == ' ') add_chr_to_str(data, len, '+');
		else if (safe_char(*name)) add_chr_to_str(data, len, *name);
		else {
			unsigned char n[4];
			sprintf(cast_char n, "%%%02X", *name);
			add_to_str(data, len, n);
		}
	}
}

static void encode_controls(struct list_head *l, unsigned char **data, int *len,
		     int cp_from, int cp_to)
{
	struct submitted_value *sv;
	int lst = 0;
	unsigned char *p2;
	struct conv_table *convert_table = get_translation_table(cp_from, cp_to);
	*len = 0;
	*data = init_str();
	foreach(sv, *l) {
		unsigned char *p = sv->value;
		if (lst) add_to_str(data, len, cast_uchar "&"); else lst = 1;
		encode_string(sv->name, data, len);
		add_to_str(data, len, cast_uchar "=");
		if (sv->type == FC_TEXT || sv->type == FC_PASSWORD || sv->type == FC_TEXTAREA)
			p2 = convert_string(convert_table, p, (int)strlen(cast_const_char p), NULL);
		else p2 = stracpy(p);
		encode_string(p2, data, len);
		mem_free(p2);
	}
}

#define BL	56
#define BL1	27

static void encode_multipart(struct session *ses, struct list_head *l, unsigned char **data, int *len,
		      unsigned char *bound, int cp_from, int cp_to)
{
	int errn;
	int *bound_ptrs = DUMMY;
	int nbound_ptrs = 0;
	unsigned char *m1, *m2;
	struct submitted_value *sv;
	int i, j;
	int flg = 0;
	unsigned char *p;
	int rs;
	struct conv_table *convert_table = get_translation_table(cp_from, cp_to);
	memset(bound, 'x', BL);
	*len = 0;
	*data = init_str();
	foreach(sv, *l) {
		unsigned char *ct;
		bnd:
		add_to_str(data, len, cast_uchar "--");
		if (!(nbound_ptrs & (ALLOC_GR-1))) {
			if ((unsigned)nbound_ptrs > MAXINT / sizeof(int) - ALLOC_GR) overalloc();
			bound_ptrs = mem_realloc(bound_ptrs, (nbound_ptrs + ALLOC_GR) * sizeof(int));
		}
		bound_ptrs[nbound_ptrs++] = *len;
		add_bytes_to_str(data, len, bound, BL);
		if (flg) break;
		add_to_str(data, len, cast_uchar "\r\nContent-Disposition: form-data; name=\"");
		add_to_str(data, len, sv->name);
		add_to_str(data, len, cast_uchar "\"");
		if (sv->type == FC_FILE) {
			add_to_str(data, len, cast_uchar "; filename=\"");
			add_to_str(data, len, strip_file_name(sv->value));
				/* It sends bad data if the file name contains ", but
				   Netscape does the same */
			add_to_str(data, len, cast_uchar "\"");
			if (*sv->value) if ((ct = get_content_type(NULL, sv->value))) {
				add_to_str(data, len, cast_uchar "\r\nContent-Type: ");
				add_to_str(data, len, ct);
				if (strlen(cast_const_char ct) >= 4 && !casecmp(ct, cast_uchar "text", 4)) {
					add_to_str(data, len, cast_uchar "; charset=");
					if (!F) add_to_str(data, len, get_cp_mime_name(term_charset(ses->term)));
#ifdef G
					else add_to_str(data, len, get_cp_mime_name(ses->ds.assume_cp));
#endif
				}
				mem_free(ct);
			}
		}
		add_to_str(data, len, cast_uchar "\r\n\r\n");
		if (sv->type != FC_FILE) {
			if (sv->type == FC_TEXT || sv->type == FC_PASSWORD || sv->type == FC_TEXTAREA)
				p = convert_string(convert_table, sv->value, (int)strlen(cast_const_char sv->value), NULL);
			else p = stracpy(sv->value);
			add_to_str(data, len, p);
			mem_free(p);
		} else {
			int fh, rd;
#define F_BUFLEN 1024
			unsigned char buffer[F_BUFLEN];
			/*if (!check_file_name(sv->value)) {
				errn = errno;
				err = cast_uchar "File access forbidden";
				goto error;
			}*/
			if (*sv->value) {
				unsigned char *wd;
				if (anonymous) {
					goto not_allowed;
				}
				wd = get_cwd();
				set_cwd(ses->term->cwd);
				fh = c_open(sv->value, O_RDONLY | O_NOCTTY);
				if (fh == -1) {
					errn = errno;
					if (wd) set_cwd(wd), mem_free(wd);
					goto error;
				}
				if (wd) set_cwd(wd), mem_free(wd);
				do {
					if ((rd = hard_read(fh, buffer, F_BUFLEN)) == -1) {
						errn = errno;
						EINTRLOOP(rs, close(fh));
						goto error;
					}
					if (rd) add_bytes_to_str(data, len, buffer, rd);
				} while (rd);
				EINTRLOOP(rs, close(fh));
			}
		}
		add_to_str(data, len, cast_uchar "\r\n");
	}
	if (!flg) {
		flg = 1;
		goto bnd;
	}
	add_to_str(data, len, cast_uchar "--\r\n");
	memset(bound, '-', BL1);
	memset(bound + BL1, '0', BL - BL1);
	again:
	for (i = 0; i <= *len - BL; i++) {
		for (j = 0; j < BL; j++) if ((*data)[i + j] != bound[j]) goto nb;
		for (j = BL - 1; j >= 0; j--) {
			if (bound[j] < '0') bound[j] = '0' - 1;
			if (bound[j]++ >= '9') bound[j] = '0';
			else goto again;
		}
		internal("Could not assign boundary");
		nb:;
	}
	for (i = 0; i < nbound_ptrs; i++) memcpy(*data + bound_ptrs[i], bound, BL);
	mem_free(bound_ptrs);
	return;

	error:
	mem_free(bound_ptrs);
	mem_free(*data);
	*data = NULL;
	m1 = stracpy(sv->value);
	m2 = stracpy(cast_uchar strerror(errn));
	msg_box(ses->term, getml(m1, m2, NULL), TEXT_(T_ERROR_WHILE_POSTING_FORM), AL_CENTER | AL_EXTD_TEXT, TEXT_(T_COULD_NOT_GET_FILE), cast_uchar " ", m1, cast_uchar ": ", m2, NULL, ses, 1, TEXT_(T_CANCEL), NULL, B_ENTER | B_ESC);
	return;

	not_allowed:
	mem_free(bound_ptrs);
	mem_free(*data);
	*data = NULL;
	msg_box(ses->term, NULL, TEXT_(T_ERROR_WHILE_POSTING_FORM), AL_CENTER | AL_EXTD_TEXT, TEXT_(T_READING_FILES_IS_NOT_ALLOWED), NULL, ses, 1, TEXT_(T_CANCEL), NULL, B_ENTER | B_ESC);
}

void reset_form(struct f_data_c *f, int form_num)
{
	struct form_control *form;
	foreach(form, f->f_data->forms) if (form->form_num == form_num) {
		struct form_state *fs;
		if ((fs = find_form_state(f, form))) init_ctrl(form, fs);
	}
}

unsigned char *get_form_url(struct session *ses, struct f_data_c *f, struct form_control *form, int *onsubmit)
{
	struct list_head submit;
	unsigned char *data;
	unsigned char bound[BL];
	int len;
	unsigned char *go = NULL;
	int cp_from, cp_to;
	if (!form) return NULL;
	if (form->type == FC_RESET) {
		reset_form(f, form->form_num);
#ifdef G
		if (F) draw_fd(f);
#endif
		return NULL;
	}
	if (onsubmit)*onsubmit=0;
#ifdef JS
	if (form->onsubmit)
	{
		jsint_execute_code(f,form->onsubmit,strlen(cast_const_char form->onsubmit),-1,form->form_num,form->form_num, NULL);
		if (onsubmit)*onsubmit=1;
	}
#endif
	if (!form->action) return NULL;
	get_succesful_controls(f, form, &submit);
	cp_from = term_charset(ses->term);
	cp_to = f->f_data->cp;
	if (form->method == FM_GET || form->method == FM_POST)
		encode_controls(&submit, &data, &len, cp_from, cp_to);
	else
		encode_multipart(ses, &submit, &data, &len, bound, cp_from, cp_to);
	if (!data) goto ff;
	if (!strncasecmp(cast_const_char form->action,"javascript:",11))
	{
		go=stracpy(form->action);
		goto x;
	}
	if (form->method == FM_GET) {
		unsigned char *pos, *da;
		int q;
		if (strlen(cast_const_char form->action) + 2 + len < (unsigned)len) overalloc();
		go = mem_alloc(strlen(cast_const_char form->action) + 1 + len + 1);
		strcpy(cast_char go, cast_const_char form->action);
		pos = extract_position(go);
		if (!(da = get_url_data(go))) da = go;
		q = (int)strlen(cast_const_char da);
		if (q && (da[q - 1] == '&' || da[q - 1] == '?'))
			;
		else if (strchr(cast_const_char da, '?')) strcat(cast_char go, "&");
		else strcat(cast_char go, "?");
		strcat(cast_char go, cast_const_char data);
		if (pos) strcat(cast_char go, cast_const_char pos), mem_free(pos);
	} else {
		int l = 0;
		int i;
		go = init_str();
		add_to_str(&go, &l, form->action);
		add_chr_to_str(&go, &l, POST_CHAR);
		if (form->method == FM_POST) add_to_str(&go, &l, cast_uchar "application/x-www-form-urlencoded\n");
		else {
			add_to_str(&go, &l, cast_uchar "multipart/form-data; boundary=");
			add_bytes_to_str(&go, &l, bound, BL);
			add_to_str(&go, &l, cast_uchar "\n");
		}
		for (i = 0; i < len; i++) {
			unsigned char p[3];
			sprintf(cast_char p, "%02x", (int)data[i]);
			add_to_str(&go, &l, p);
		}
	}
	x:
	mem_free(data);
	ff:
	free_succesful_controls(&submit);
	return go;
}

int ismap_link = 0, ismap_x = 1, ismap_y = 1;

/* if onsubmit is not NULL it will contain 1 if link is submit and the form has an onsubmit handler */
static unsigned char *get_link_url(struct session *ses, struct f_data_c *f, struct link *l, int *onsubmit)
{
	if (l->type == L_LINK) {
		if (!l->where) {
			if (l->where_img && (!F || (!f->f_data->opt.display_images && f->f_data->opt.plain != 2))) return stracpy(l->where_img);
			return NULL;
		}
		if (ismap_link && strlen(cast_const_char l->where) >= 4 && !strcmp(cast_const_char(l->where + strlen(cast_const_char l->where) - 4), "?0,0")) {
			unsigned char *nu = init_str();
			int ll = 0;
			add_bytes_to_str(&nu, &ll, l->where, strlen(cast_const_char l->where) - 3);
			add_num_to_str(&nu, &ll, ismap_x);
			add_chr_to_str(&nu, &ll, ',');
			add_num_to_str(&nu, &ll, ismap_y);
			return nu;
		}
		return stracpy(l->where);
	}
	if (l->type != L_BUTTON && l->type != L_FIELD) return NULL;
	return get_form_url(ses, f, l->form, onsubmit);
}

static struct menu_item *clone_select_menu(struct menu_item *m)
{
	struct menu_item *n = DUMMY;
	int i = 0;
	do {
		if ((unsigned)i > MAXINT / sizeof(struct menu_item) - 1) overalloc();
		n = mem_realloc(n, (i + 1) * sizeof(struct menu_item));
		n[i].text = stracpy(m->text);
		n[i].rtext = stracpy(m->rtext);
		n[i].hotkey = stracpy(m->hotkey);
		n[i].in_m = m->in_m;
		n[i].free_i = 0;
		if ((n[i].func = m->func) != MENU_FUNC do_select_submenu) {
			n[i].data = m->data;
		} else n[i].data = clone_select_menu(m->data);
		i++;
	} while (m++->text);
	return n;
}

static void free_select_menu(struct menu_item *m)
{
	struct menu_item *om = m;
	do {
		if (m->text) mem_free(m->text);
		if (m->rtext) mem_free(m->rtext);
		if (m->hotkey) mem_free(m->hotkey);
		if (m->func == MENU_FUNC do_select_submenu) free_select_menu(m->data);
	} while (m++->text);
	mem_free(om);
}

void set_frame(struct session *ses, struct f_data_c *f, int a)
{
	if (f == ses->screen) return;
	if (!f->loc->url) return;
	goto_url_not_from_dialog(ses, f->loc->url, ses->screen);
}

static struct link *get_current_link(struct f_data_c *f)
{
	if (!f || !f->f_data || !f->vs) return NULL;
	if (f->vs->current_link >= 0 && f->vs->current_link < f->f_data->nlinks)
		return &f->f_data->links[f->vs->current_link];
	if (F && f->f_data->opt.plain == 2 && f->f_data->nlinks == 1)
		return &f->f_data->links[0];
	return NULL;
}

/* pokud je a==1, tak se nebude submitovat formular, kdyz kliknu na input field a formular nema submit */
int enter(struct session *ses, struct f_data_c *f, int a)
{
	struct link *link;
	unsigned char *u;
	link = get_current_link(f);
	if (!link) return 1;
#ifdef JS
	if (link->js_event&&link->js_event->click_code)
		jsint_execute_code(f,link->js_event->click_code,strlen(cast_const_char link->js_event->click_code),-1,(link->type==L_BUTTON&&link->form&&link->form->type==FC_SUBMIT)?link->form->form_num:-1,-1, NULL);
#endif
	if (link->type == L_LINK || link->type == L_BUTTON) {
		int has_onsubmit;
		if (link->type==L_BUTTON&&link->form->type==FC_BUTTON)return 1;
		submit:
		if ((u = get_link_url(ses, f, link, &has_onsubmit))) {
#ifdef JS
			struct js_event_spec *s=link->js_event;
#endif
			if (strlen(cast_const_char u) >= 4 && !casecmp(u, cast_uchar "MAP@", 4)) {
				goto_imgmap(ses, f, u + 4, stracpy(u + 4), stracpy(link->target));
			} else if (ses->ds.target_in_new_window && link->target && *link->target && !find_frame(ses, link->target, f) && can_open_in_new(ses->term)) {	/* open in new window */
				if (ses->wtd_target) mem_free(ses->wtd_target);
				ses->wtd_target = stracpy(link->target);
				open_in_new_window(ses->term, send_open_in_new_xterm, ses);
				mem_free(ses->wtd_target), ses->wtd_target=NULL;
			} else {
				goto_url_f(
				ses,
				NULL,
				u,
				link->target,
				f,
				(link->type==L_BUTTON&&link->form&&link->form->type==FC_SUBMIT)?link->form->form_num:-1,
#ifdef JS
				(s&&(/*s->keyup_code||s->keydown_code||s->keypress_code||s->change_code||s->blur_code||s->focus_code||s->move_code||s->over_code||s->out_code||*/s->down_code||s->up_code||s->click_code||s->dbl_code))||has_onsubmit
#else
				0
#endif
				,0,0
				);
			}
			mem_free(u);
			return 2;
		}
		return 1;
	}
	if (link->type == L_CHECKBOX) {
		struct form_state *fs = find_form_state(f, link->form);
		if (link->form->ro) return 1;
		if (link->form->type == FC_CHECKBOX) fs->state = !fs->state;
		else {
			struct form_control *fc;
#ifdef G
			int re = 0;
#endif
			foreach(fc, f->f_data->forms)
				if (fc->form_num == link->form->form_num && fc->type == FC_RADIO && !xstrcmp(fc->name, link->form->name)) {
					struct form_state *fffs = find_form_state(f, fc);
					if (fffs) fffs->state = 0;
#ifdef G
					re = 1;
#endif
				}
			fs = find_form_state(f, link->form);
			fs->state = 1;
#ifdef G
			if (F && re) draw_fd(f);
#endif
		}
		return 1;
	}
	if (link->type == L_SELECT) {
		struct menu_item *m;
		if (link->form->ro) return 1;
		m = clone_select_menu(link->form->menu);
		if (!m) return 1;
		/* execute onfocus code of the select object */
#ifdef JS
		if (link->js_event&&link->js_event->focus_code)
		{
			jsint_execute_code(f,link->js_event->focus_code,strlen(cast_const_char link->js_event->focus_code),-1,-1,-1, NULL);
		}
#endif
		add_empty_window(ses->term, (void (*)(void *))free_select_menu, m);
		do_select_submenu(ses->term, m, ses);
		return 1;
	}
	if (link->type == L_FIELD || link->type == L_AREA) {
		/* pri enteru v textovem policku se bude posilat vzdycky       -- Brain */
		if (!has_form_submit(f->f_data, link->form) && (!a || !F)) goto submit;
#ifdef JS
		/* process onfocus handler */
		if (
#ifdef G
		    !ses->locked_link&&
#endif
		    f->vs&&f->f_data&&f->vs->current_link>=0&&f->vs->current_link<f->f_data->nlinks)
		{
			struct link *lnk=&(f->f_data->links[f->vs->current_link]);
			if (lnk->js_event&&lnk->js_event->focus_code)
				jsint_execute_code(f,lnk->js_event->focus_code,strlen(cast_const_char lnk->js_event->focus_code),-1,-1,-1, NULL);
		}
#endif
#ifdef G
		if (F && a) {
			ses->locked_link = 1;
			return 2;
		}
#endif
		if (!F) {
			if (!ses->term->spec->braille) {
				down(ses, f, 0);
			} else {
				if (f->vs->current_link < f->f_data->nlinks - 1) {
					f->vs->current_link++;
					if (f->f_data->links[f->vs->current_link].n) {
						f->vs->brl_x = f->f_data->links[f->vs->current_link].pos[0].x;
						f->vs->brl_y = f->f_data->links[f->vs->current_link].pos[0].y;
						f->vs->orig_brl_x = f->vs->brl_x;
						f->vs->orig_brl_y = f->vs->brl_y;
					}
				}
			}
		}
#ifdef G
		else g_next_link(f, 1);
#endif
		return 1;
	}
	internal("bad link type %d", link->type);
	return 1;
}

void toggle(struct session *ses, struct f_data_c *f, int a)
{
	if (!f || !f->vs) {
		msg_box(ses->term, NULL, TEXT_(T_TOGGLE_HTML_PLAIN), AL_LEFT, TEXT_(T_YOU_ARE_NOWHERE), NULL, 1, TEXT_(T_OK), NULL, B_ENTER | B_ESC);
		return;
	}
	if (f->vs->plain == -1) f->vs->plain = 1;
	else f->vs->plain = f->vs->plain ^ 1;
	html_interpret_recursive(f);
	draw_formatted(ses);
}

void selected_item(struct terminal *term, void *pitem, struct session *ses)
{
	int item = (int)(my_intptr_t)pitem;
#ifdef JS
	int old_item=item;
#endif
	struct f_data_c *f = current_frame(ses);
	struct link *l;
	struct form_state *fs;
	l = get_current_link(f);
	if (!l) return;
	if (l->type != L_SELECT) return;
	if ((fs = find_form_state(f, l->form))) {
		struct form_control *form= l->form;
		if (item >= 0 && item < form->nvalues) {
#ifdef JS
			old_item=fs->state;
#endif
			fs->state = item;
			if (fs->value) mem_free(fs->value);
			fs->value = stracpy(form->values[item]);
		}
		fixup_select_state(form, fs);
	}
	f->active = 1;
#ifdef G
	if (F) {
		f->xl = -1;
		f->yl = -1;
	}
#endif
	/* execute onchange handler */
#ifdef JS
	if (old_item!=item&&l->js_event&&l->js_event->change_code)
		jsint_execute_code(f,l->js_event->change_code,strlen(cast_const_char l->js_event->change_code),-1,-1,-1, NULL);
#endif
	/* execute onblur handler */
#ifdef JS
	if (l->js_event&&l->js_event->blur_code)
		jsint_execute_code(f,l->js_event->blur_code,strlen(cast_const_char l->js_event->blur_code),-1,-1,-1, NULL);
#endif
	draw_to_window(ses->win, (void (*)(struct terminal *, void *))draw_doc, f);
	change_screen_status(ses);
	print_screen_status(ses);
	/*if (!has_form_submit(f->f_data, l->form)) {
		goto_form(ses, f, l->form, l->target);
	}*/
}

int get_current_state(struct session *ses)
{
	struct f_data_c *f = current_frame(ses);
	struct link *l;
	struct form_state *fs;
	l = get_current_link(f);
	if (!l) return -1;
	if (l->type != L_SELECT) return -1;
	if ((fs = find_form_state(f, l->form))) return fs->state;
	return -1;
}

static int find_pos_in_link(struct f_data_c *fd,struct link *l,struct links_event *ev,int *xx,int *yy);

static void set_form_position(struct f_data_c *fd, struct link *l, struct links_event *ev)
{
	struct form_state *fs;
	/* if links is a field, set cursor position */
	if (l->form&&(l->type==L_AREA||l->type==L_FIELD)&&(fs=find_form_state(fd,l->form)))
	{
		int xx = 0, yy = 0; /* against uninitialized warning */

		if (l->type==L_AREA) {
			struct line_info *ln;

			if (!find_pos_in_link(fd,l,ev,&xx,&yy)) {

				int a;
				xx += fs->vpos;
				yy += fs->vypos;
				ln = format_text(fs->value, l->form->cols, l->form->wrap, fd->f_data->opt.cp);
				for (a = 0; ln[a].st; a++) if (a == yy) {
					unsigned char *ptr;
					fs->state = (int)(ln[a].st - fs->value);
					ptr = textptr_add(fs->value + fs->state, xx, fd->f_data->opt.cp);
					if (ptr > ln[a].en) ptr = ln[a].en;
					fs->state = (int)(ptr - fs->value);
					goto br;
				}
				fs->state = (int)strlen(cast_const_char fs->value);
				br:;
			}
		} else if (l->type==L_FIELD) {
			if (!find_pos_in_link(fd,l,ev,&xx,&yy)) {
				unsigned char *ptr;
				ptr = textptr_add(fs->value + fs->vpos, xx, fd->f_data->opt.cp);
				fs->state = (int)(ptr - fs->value);
			}
		}
	}
}

static int textarea_adjust_viewport(struct f_data_c *fd, struct link *l)
{
	struct form_control *fc = l->form;
	struct view_state *vs = fd->vs;
	int r = 0;
	if (l->pos[0].x + fc->cols > fd->xw + vs->view_posx)
		vs->view_posx = l->pos[0].x + fc->cols - fd->xw, r = 1;
	if (l->pos[0].x < vs->view_posx)
		vs->view_posx = l->pos[0].x, r = 1;
	if (l->pos[0].y + fc->rows > fd->yw + vs->view_pos)
		vs->view_pos = l->pos[0].y + fc->rows - fd->yw, r = 1;
	if (l->pos[0].y < vs->view_pos)
		vs->view_pos = l->pos[0].y, r = 1;
	vs->orig_view_pos = vs->view_pos;
	vs->orig_view_posx = vs->view_posx;
	return r;
}

static void set_br_pos(struct f_data_c *fd, struct link *l)
{
	struct links_event ev;
	if (!fd->ses->term->spec->braille || fd->vs->brl_in_field) return;
	ev.ev = EV_MOUSE;
	ev.x = fd->ses->term->cx - fd->xp;
	ev.y = fd->ses->term->cy - fd->yp;
	ev.b = 0;
	set_form_position(fd, l, &ev);
}

#ifdef JS
/* executes onkey-press/up/down handler */
static void field_op_changed(struct f_data_c *f, struct link *lnk)
{
	/*
	if (lnk->js_event&&lnk->js_event->keydown_code)
		jsint_execute_code(f,lnk->js_event->keydown_code,strlen(cast_const_char lnk->js_event->keydown_code),-1,-1,-1, NULL);
	if (lnk->js_event&&lnk->js_event->keypress_code)
		jsint_execute_code(f,lnk->js_event->keypress_code,strlen(cast_const_char lnk->js_event->keypress_code),-1,-1,-1, NULL);
	if (lnk->js_event&&lnk->js_event->keyup_code)
		jsint_execute_code(f,lnk->js_event->keyup_code,strlen(cast_const_char lnk->js_event->keyup_code),-1,-1,-1, NULL);
	*/
}
#endif


int field_op(struct session *ses, struct f_data_c *f, struct link *l, struct links_event *ev, int rep)
{
	struct form_control *form = l->form;
	struct form_state *fs;
	int x = 1;

	if (!form) {
		internal("link has no form control");
		return 0;
	}
	if (l->form->ro == 2) return 0;
	if (!(fs = find_form_state(f, form))) return 0;
	if (!fs->value) return 0;
	if (ev->ev == EV_KBD) {
		if (ev->x == KBD_LEFT && (!ses->term->spec->braille || f->vs->brl_in_field)) {
			if (f->f_data->opt.cp != utf8_table) fs->state = fs->state ? fs->state - 1 : 0;
			else {
				unsigned char *p = fs->value + fs->state;
				BACK_UTF_8(p, fs->value);
				fs->state = (int)(p - fs->value);
			}
		} else if (ev->x == KBD_RIGHT && (!ses->term->spec->braille || f->vs->brl_in_field)) {
			if ((size_t)fs->state < strlen(cast_const_char fs->value)) {
				if (f->f_data->opt.cp != utf8_table) fs->state = fs->state + 1;
				else {
					unsigned char *p = fs->value + fs->state;
					FWD_UTF_8(p);
					fs->state = (int)(p - fs->value);
				}
			} else fs->state = (int)strlen(cast_const_char fs->value);
		} else if ((ev->x == KBD_HOME || (upcase(ev->x) == 'A' && ev->y & KBD_CTRL)) /*&& (!ses->term->spec->braille || f->vs->brl_in_field)*/) {
			if (form->type == FC_TEXTAREA) {
				struct line_info *ln;
				int y;
				ln = format_text(fs->value, form->cols, form->wrap, f->f_data->opt.cp);
				for (y = 0; ln[y].st; y++) if (fs->value + fs->state >= ln[y].st && fs->value + fs->state < ln[y].en + (ln[y+1].st != ln[y].en)) {
					fs->state = (int)(ln[y].st - fs->value);
					goto x;
				}
				fs->state = 0;
				x:;
			} else fs->state = 0;
		} else if (ev->x == KBD_UP && (!ses->term->spec->braille || f->vs->brl_in_field)) {
			if (form->type == FC_TEXTAREA) {
				struct line_info *ln;
				int y;
				ln = format_text(fs->value, form->cols, form->wrap, f->f_data->opt.cp);
				rep1:
				for (y = 0; ln[y].st; y++) if (fs->value + fs->state >= ln[y].st && fs->value + fs->state < ln[y].en + (ln[y+1].st != ln[y].en)) {
					if (!y) {
						/*if (F) goto xx;*/
						goto b;
					}
					/*fs->state -= ln[y].st - ln[y-1].st;*/
					fs->state = (int)(textptr_add(ln[y-1].st, textptr_diff(fs->value + fs->state, ln[y].st, f->f_data->opt.cp), f->f_data->opt.cp) - fs->value);
					if (fs->value + fs->state > ln[y-1].en) fs->state = (int)(ln[y-1].en - fs->value);
					goto xx;
				}
				goto b;
				xx:
				if (rep) goto rep1;
			} else x = 0, f->vs->brl_in_field = 0;
		} else if (ev->x == KBD_DOWN && (!ses->term->spec->braille || f->vs->brl_in_field)) {
			if (form->type == FC_TEXTAREA) {
				struct line_info *ln;
				int y;
				ln = format_text(fs->value, form->cols, form->wrap, f->f_data->opt.cp);
				rep2:
				for (y = 0; ln[y].st; y++) if (fs->value + fs->state >= ln[y].st && fs->value + fs->state < ln[y].en + (ln[y+1].st != ln[y].en)) {
					if (!ln[y+1].st) {
						/*if (F) goto yy;*/
						goto b;
					}
					/*fs->state += ln[y+1].st - ln[y].st;*/
					fs->state = (int)(textptr_add(ln[y+1].st, textptr_diff(fs->value + fs->state, ln[y].st, f->f_data->opt.cp), f->f_data->opt.cp) - fs->value);
					if (fs->value + fs->state > ln[y+1].en) fs->state = (int)(ln[y+1].en - fs->value);
					goto yy;
				}
				goto b;
				yy:
				if (rep) goto rep2;

			} else x = 0, f->vs->brl_in_field = 0;
		} else if ((ev->x == KBD_END || (upcase(ev->x) == 'E' && ev->y & KBD_CTRL)) /*&& (!ses->term->spec->braille || f->vs->brl_in_field)*/) {
			if (form->type == FC_TEXTAREA) {
				struct line_info *ln;
				int y;
				ln = format_text(fs->value, form->cols, form->wrap, f->f_data->opt.cp);
				for (y = 0; ln[y].st; y++) if (fs->value + fs->state >= ln[y].st && fs->value + fs->state < ln[y].en + (ln[y+1].st != ln[y].en)) {
					fs->state = (int)(ln[y].en - fs->value);
					if (fs->state && (size_t)fs->state < strlen(cast_const_char fs->value) && ln[y+1].st == ln[y].en) fs->state--;
					goto yyyy;
				}
				fs->state = (int)strlen(cast_const_char fs->value);
				yyyy:;
			} else fs->state = (int)strlen(cast_const_char fs->value);
		} else if (!(ev->y & (KBD_CTRL | KBD_ALT)) && (ev->x >= 32 && ev->x < MAXINT && gf_val(cp2u(ev->x, term_charset(ses->term)) != -1, 1))) {
			set_br_pos(f, l);
			if (!form->ro && cp_len(term_charset(ses->term), fs->value) < form->maxlength) {
				unsigned char *v;
				unsigned char a_[2];
				unsigned char *nw;
				int ll;
				v = mem_realloc(fs->value, strlen(cast_const_char fs->value) + 12);
				if (f->f_data->opt.cp != utf8_table) {
					nw = a_;
					a_[0] = (unsigned char)ev->x;
					a_[1] = 0;
				} else {
					nw = encode_utf_8(ev->x);
				}
				ll = (int)strlen(cast_const_char nw);
				if (ll > 10) goto bad;
				fs->value = v;
				memmove(v + fs->state + ll, v + fs->state, strlen(cast_const_char(v + fs->state)) + 1);
				memcpy(&v[fs->state], nw, ll);
				fs->state += ll;
#ifdef JS
				fs->changed=1;
				field_op_changed(f,l);
#endif
				bad:;
			}
		} else if ((ev->x == KBD_INS && ev->y & KBD_CTRL) || (upcase(ev->x) == 'B' && ev->y & KBD_CTRL)) {
			set_br_pos(f, l);
			set_clipboard_text(ses->term, fs->value);
		} else if ((ev->x == KBD_DEL && ev->y & KBD_SHIFT) || (upcase(ev->x) == 'X' && ev->y & KBD_CTRL)) {
			set_br_pos(f, l);
			set_clipboard_text(ses->term, fs->value);
			if (!form->ro) fs->value[0] = 0;
			fs->state = 0;
#ifdef JS
			fs->changed=1;
			field_op_changed(f,l);
#endif
		} else if ((ev->x == KBD_INS && ev->y & KBD_SHIFT) || (upcase(ev->x) == 'V' && ev->y & KBD_CTRL)) {
			unsigned char *clipboard;
			set_br_pos(f, l);
			clipboard = get_clipboard_text(ses->term);
			if (!clipboard) goto brk;
			if (form->type != FC_TEXTAREA) {
				unsigned char *nl = clipboard;
				while ((nl = cast_uchar strchr(cast_const_char nl, '\n'))) *nl = ' ';
			}
			if (!form->ro && cp_len(term_charset(ses->term), fs->value) + cp_len(term_charset(ses->term), clipboard) <= form->maxlength) {
				unsigned char *v;
				v = mem_realloc(fs->value, strlen(cast_const_char fs->value) + strlen(cast_const_char clipboard) +1);
				fs->value = v;
				memmove(v + fs->state + strlen(cast_const_char clipboard), v + fs->state, strlen(cast_const_char v) - fs->state + 1);
				memcpy(v + fs->state, clipboard, strlen(cast_const_char clipboard));
				fs->state += (int)strlen(cast_const_char clipboard);
			}
			mem_free(clipboard);
#ifdef JS
			fs->changed=1;
			field_op_changed(f,l);
#endif
			brk:;
		} else if (ev->x == KBD_ENTER && form->type == FC_TEXTAREA && (!ses->term->spec->braille || f->vs->brl_in_field)) {
			if (!form->ro && strlen(cast_const_char fs->value) < (size_t)form->maxlength) {
				unsigned char *v;
				v = mem_realloc(fs->value, strlen(cast_const_char fs->value) + 2);
				fs->value = v;
				memmove(v + fs->state + 1, v + fs->state, strlen(cast_const_char(v + fs->state)) + 1);
				v[fs->state++] = '\n';
#ifdef JS
				fs->changed=1;
				field_op_changed(f,l);
#endif
			}
		} else if (ev->x == KBD_ENTER) {
			x = 0;
		} else if (ev->x == KBD_BS) {
			set_br_pos(f, l);
			if (!form->ro && fs->state) {
				int ll = 1;
				if (f->f_data->opt.cp == utf8_table) {
					unsigned char *p = fs->value + fs->state;
					BACK_UTF_8(p, fs->value);
					ll = (int)(fs->value + fs->state - p);
				}
				memmove(fs->value + fs->state - ll, fs->value + fs->state, strlen(cast_const_char(fs->value + fs->state)) + 1), fs->state -= ll
#ifdef JS
				, fs->changed=1, field_op_changed(f,l)
#endif
				;
			}
		} else if (ev->x == KBD_DEL || (upcase(ev->x) == 'D' && ev->y & KBD_CTRL)) {
			int ll = 1;
			set_br_pos(f, l);
			if (f->f_data->opt.cp == utf8_table) {
				unsigned char *p = fs->value + fs->state;
				FWD_UTF_8(p);
				ll = (int)(p - (fs->value + fs->state));
			}
			if (!form->ro && (size_t)fs->state < strlen(cast_const_char fs->value)) memmove(fs->value + fs->state, fs->value + fs->state + ll, strlen(cast_const_char(fs->value + fs->state + ll)) + 1)
#ifdef JS
				, fs->changed=1, field_op_changed(f,l)
#endif
				;
		} else if (upcase(ev->x) == 'U' && ev->y & KBD_CTRL) {
			set_br_pos(f, l);
			if (!form->ro) {
				unsigned char *a = memacpy(fs->value, fs->state);
				if (a) {
					set_clipboard_text(ses->term, a);
					mem_free(a);
				}
				memmove(fs->value, fs->value + fs->state, strlen(cast_const_char(fs->value + fs->state)) + 1);
			}
			fs->state = 0;
#ifdef JS
			fs->changed=1;
			field_op_changed(f,l);
#endif
		} else if (upcase(ev->x) == 'K' && ev->y & KBD_CTRL) {
			set_br_pos(f, l);
			if (!form->ro) {
				if (form->type == FC_TEXTAREA) {
					struct line_info *ln, *lnx;
					lnx = format_text(fs->value, form->cols, form->wrap, f->f_data->opt.cp);
					for (ln = lnx; ln->st; ln++) {
						if (!(ln + 1)->st || (ln + 1)->st > fs->value + fs->state) {
							unsigned l;
							unsigned char *cp = memacpy(ln->st, ln->en - ln->st);
							set_clipboard_text(ses->term, cp);
							mem_free(cp);
							l = (int)(ln->en - ln->st + ((ln + 1)->st && (ln + 1)->st > ln->en));
							memmove(ln->st, ln->st + l, strlen(cast_const_char(ln->st + l)) + 1);
							fs->state = (int)(ln->st - fs->value);
							break;
						}
					}
				} else {
					set_clipboard_text(ses->term, fs->state + fs->value);
					fs->value[fs->state] = 0;
				}
			}
#ifdef JS
			fs->changed=1;
			field_op_changed(f,l);
#endif
		} else {
			b:
			f->vs->brl_in_field = 0;
			x = 0;
		}
	} else x = 0;
	if (!F && x) {
		if (((ev->x != KBD_UP && ev->x != KBD_DOWN) || (ses->term->spec->braille)) && form->type == FC_TEXTAREA && textarea_adjust_viewport(f, l))
			;
		else x_draw_form_entry(ses, f, l);
	}
	if (!x && ses->term->spec->braille) {
		f->vs->brl_x = ses->term->cx - f->xp + f->vs->view_posx;
		f->vs->brl_y = ses->term->cy - f->yp + f->vs->view_pos;
		f->vs->orig_brl_x = f->vs->brl_x;
		f->vs->orig_brl_y = f->vs->brl_y;
	}
	return x;
}

void set_textarea(struct session *ses, struct f_data_c *f, int kbd)
{
	struct link *l = get_current_link(f);
	if (l && l->type == L_AREA) {
		struct links_event ev = { EV_KBD, 0, 0, 0 };
		ev.x = kbd;
		field_op(ses, f, l, &ev, 1);
	}
}

void search_for_back(struct session *ses, unsigned char *str)
{
	struct f_data_c *f = current_frame(ses);
	if (!f || !str || !str[0]) return;
	if (ses->search_word) mem_free(ses->search_word);
	ses->search_word = stracpy(str);
	clr_spaces(ses->search_word, 0);
	charset_upcase_string(&ses->search_word, term_charset(ses->term));
	if (ses->last_search_word) mem_free(ses->last_search_word);
	ses->last_search_word = stracpy(ses->search_word);
	ses->search_direction = -1;
	find_next(ses, f, 1);
}

void search_for(struct session *ses, unsigned char *str)
{
	struct f_data_c *f = current_frame(ses);
	if (!f || !f->vs || !f->f_data || !str || !str[0]) return;
	if (ses->search_word) mem_free(ses->search_word);
	ses->search_word = stracpy(str);
	clr_spaces(ses->search_word, 0);
	charset_upcase_string(&ses->search_word, term_charset(ses->term));
	if (ses->last_search_word) mem_free(ses->last_search_word);
	ses->last_search_word = stracpy(ses->search_word);
	ses->search_direction = 1;
	find_next(ses, f, 1);
}

#define HASH_SIZE	4096

#define HASH(p) (((p.y << 6) + p.x) & (HASH_SIZE - 1))

static int point_intersect(struct point *p1, int l1, struct point *p2, int l2)
{
	int i, j;
	static unsigned char hash[HASH_SIZE];
	static unsigned char init = 0;
	if (!init) memset(hash, 0, HASH_SIZE), init = 1;
	for (i = 0; i < l1; i++) hash[HASH(p1[i])] = 1;
	for (j = 0; j < l2; j++) if (hash[HASH(p2[j])]) {
		for (i = 0; i < l1; i++) if (p1[i].x == p2[j].x && p1[i].y == p2[j].y) {
			for (i = 0; i < l1; i++) hash[HASH(p1[i])] = 0;
			return 1;
		}
	}
	for (i = 0; i < l1; i++) hash[HASH(p1[i])] = 0;
	return 0;
}

static int find_next_link_in_search(struct f_data_c *f, int d)
{
	struct point *pt;
	int len;
	struct link *link;
	if (f->ses->term->spec->braille) {
		int i, opt;
		if (get_searched(f, &pt, &len))
			return 1;
		if (!len) {
			mem_free(pt);
			return 1;
		}
		opt = -1;
		for (i = 0; i < len; i++) {
			if (d > 0) {
				if ((d == 2 || pt[i].y > f->vs->brl_y || (pt[i].y == f->vs->brl_y && pt[i].x > f->vs->brl_x)) && (opt == -1 || pt[i].y < pt[opt].y || (pt[i].y == pt[opt].y && pt[i].x < pt[opt].x))) opt = i;
			}
			if (d < 0) {
				if ((d == -2 || pt[i].y < f->vs->brl_y || (pt[i].y == f->vs->brl_y && pt[i].x < f->vs->brl_x)) && (opt == -1 || pt[i].y > pt[opt].y || (pt[i].y == pt[opt].y && pt[i].x > pt[opt].x))) opt = i;
			}
		}
		if (opt == -1) {
			mem_free(pt);
			return 1;
		}
		f->vs->brl_x = pt[opt].x;
		f->vs->brl_y = pt[opt].y;
		f->vs->orig_brl_x = f->vs->brl_x;
		f->vs->orig_brl_y = f->vs->brl_y;
		update_braille_link(f);
		mem_free(pt);
		return 0;
	}
	if (d == -2 || d == 2) {
		d /= 2;
		find_link(f, d, 0);
		if (f->vs->current_link == -1) return 1;
	} else nx:if (f->vs->current_link == -1 || !(next_in_view(f, f->vs->current_link + d, d, in_view, NULL))) {
		find_link(f, d, 0);
		return 1;
	}
	link = &f->f_data->links[f->vs->current_link];
	if (get_searched(f, &pt, &len) < 0)
		return 1;
	if (point_intersect(pt, len, link->pos, link->n)) {
		mem_free(pt);
		return 0;
	}
	mem_free(pt);
	goto nx;
}

void find_next(struct session *ses, struct f_data_c *f, int a)
{
	int min, max;
	int c = 0;
	int p;
	if (!f->f_data || !f->vs) {
		msg_box(ses->term, NULL, TEXT_(T_SEARCH), AL_CENTER, TEXT_(T_YOU_ARE_NOWHERE), NULL, 1, TEXT_(T_CANCEL), NULL, B_ENTER | B_ESC);
		return;
	}
	p = f->vs->view_pos;
	if (!F && !a && ses->search_word) {
		if (!(find_next_link_in_search(f, ses->search_direction))) return;
		p += ses->search_direction * f->yw;
	}
	if (!ses->search_word) {
		if (!ses->last_search_word) {
			msg_box(ses->term, NULL, TEXT_(T_SEARCH), AL_CENTER, TEXT_(T_NO_PREVIOUS_SEARCH), NULL, 1, TEXT_(T_CANCEL), NULL, B_ENTER | B_ESC);
			return;
		}
		ses->search_word = stracpy(ses->last_search_word);
	}
#ifdef G
	if (F) {
		g_find_next(f, a);
		return;
	}
#endif
	if (get_search_data(f->f_data) < 0) {
		mem_free(ses->search_word);
		ses->search_word = NULL;
		msg_box(ses->term, NULL, TEXT_(T_SEARCH), AL_CENTER, TEXT_(T_OUT_OF_MEMORY), NULL, 1, TEXT_(T_CANCEL), NULL, B_ENTER | B_ESC);
		return;
	}
	do {
		if (is_in_range(f->f_data, p, f->yw, ses->search_word, &min, &max)) {
			f->vs->view_pos = p;
			if (max >= min) {
				if (max > f->vs->view_posx + f->xw) f->vs->view_posx = max - f->xw;
				if (min < f->vs->view_posx) f->vs->view_posx = min;
			}
			f->vs->orig_view_pos = f->vs->view_pos;
			f->vs->orig_view_posx = f->vs->view_posx;
			if (!ses->term->spec->braille) set_link(f);
			find_next_link_in_search(f, ses->search_direction * 2);
			return;
		}
		if ((p += ses->search_direction * f->yw) > f->f_data->y) p = 0;
		if (p < 0) {
			p = 0;
			while (p < f->f_data->y) p += f->yw ? f->yw : 1;
			p -= f->yw;
		}
	} while ((c += f->yw ? f->yw : 1) < f->f_data->y + f->yw);
	msg_box(ses->term, NULL, TEXT_(T_SEARCH), AL_CENTER, TEXT_(T_SEARCH_STRING_NOT_FOUND), NULL, 1, TEXT_(T_CANCEL), NULL, B_ENTER | B_ESC);
}

void find_next_back(struct session *ses, struct f_data_c *f, int a)
{
	ses->search_direction = - ses->search_direction;
	find_next(ses, f, a);
	ses->search_direction = - ses->search_direction;
}

static void rep_ev(struct session *ses, struct f_data_c *fd, void (*f)(struct session *, struct f_data_c *, int), int a)
{
	int i = ses->kbdprefix.rep ? ses->kbdprefix.rep_num : 1;
	while (i--) f(ses, fd, a);
}

static struct link *choose_mouse_link(struct f_data_c *f, struct links_event *ev)
{
	return get_link_at_location(f->f_data, ev->x + f->vs->view_posx, ev->y + f->vs->view_pos);
}

static void goto_link_number(struct session *ses, unsigned char *num)
{
	int n = atoi(cast_const_char num);
	struct f_data_c *f = current_frame(ses);
	struct link *link;
	if (!f || !f->vs) return;
	if (n < 0 || n > f->f_data->nlinks) return;
	f->vs->current_link = n - 1;
	f->vs->orig_link = f->vs->current_link;
	link = &f->f_data->links[f->vs->current_link];
	if (ses->term->spec->braille) {
		if (link->n) {
			f->vs->brl_x = link->pos[0].x;
			f->vs->brl_y = link->pos[0].y;
			f->vs->orig_brl_x = f->vs->brl_x;
			f->vs->orig_brl_y = f->vs->brl_y;
		}
	}
	check_vs(f);
	f->vs->orig_view_pos = f->vs->view_pos;
	f->vs->orig_view_posx = f->vs->view_posx;
	if (link->type != L_AREA && link->type != L_FIELD) enter(ses, f, 0);
}


/* l must be a valid link, ev must be a mouse event */
static int find_pos_in_link(struct f_data_c *fd,struct link *l,struct links_event *ev,int *xx,int *yy)
{
	int a;
	int minx,miny;
	int found=0;

	if (!l->n)return 1;
	minx=l->pos[0].x;miny=l->pos[0].y;
	for (a=0;a<l->n;a++)
	{
		if (l->pos[a].x<minx)minx=l->pos[a].x;
		if (l->pos[a].y<miny)miny=l->pos[a].y;
		if (l->pos[a].x-fd->vs->view_posx==ev->x && l->pos[a].y-fd->vs->view_pos==ev->y)(*xx=l->pos[a].x),(*yy=l->pos[a].y),found=1;
	}
	if (!found)return 1;
	*xx-=minx;
	*yy-=miny;
	return 0;
}


static int frame_ev(struct session *ses, struct f_data_c *fd, struct links_event *ev)
{
	int x = 1;

	if (!fd || !fd->vs || !fd->f_data) return 0;
	if (fd->vs->current_link >= 0 && (fd->f_data->links[fd->vs->current_link].type == L_FIELD || fd->f_data->links[fd->vs->current_link].type == L_AREA))
		if (field_op(ses, fd, &fd->f_data->links[fd->vs->current_link], ev, 0)) {
			fd->vs->brl_in_field = 1;
			return 1;
		}
	if (ev->ev == EV_KBD && ev->x >= '0'+!ses->kbdprefix.rep && ev->x <= '9' && (!fd->f_data->opt.num_links || (ev->y & (KBD_CTRL | KBD_ALT)))) {
		if (!ses->kbdprefix.rep) ses->kbdprefix.rep_num = 0;
		if ((ses->kbdprefix.rep_num = ses->kbdprefix.rep_num * 10 + ev->x - '0') > 65536) ses->kbdprefix.rep_num = 65536;
		ses->kbdprefix.rep = 1;
		return 1;
	}
	if (ev->ev == EV_KBD) {
		if (ev->x == KBD_PAGE_DOWN || (ev->x == ' ' && (!(ev->y & KBD_ALT))) || (upcase(ev->x) == 'F' && ev->y & KBD_CTRL)) rep_ev(ses, fd, page_down, 0);
		else if (ev->x == KBD_PAGE_UP || (upcase(ev->x) == 'B' && (!(ev->y & KBD_ALT)))) rep_ev(ses, fd, page_up, 0);
		else if (ev->x == KBD_DOWN) rep_ev(ses, fd, down, 0);
		else if (ev->x == KBD_UP) rep_ev(ses, fd, up, 0);
		else if (ev->x == KBD_LEFT && ses->term->spec->braille) rep_ev(ses, fd, left, 0);
		else if (ev->x == KBD_RIGHT && ses->term->spec->braille) rep_ev(ses, fd, right, 0);
		else if (ev->x == '{' && ses->term->spec->braille) rep_ev(ses, fd, cursor_home, 0);
		else if (ev->x == '}' && ses->term->spec->braille) rep_ev(ses, fd, cursor_end, 0);
		else if (upcase(ev->x) == 'Y' && !(ev->y & (KBD_CTRL | KBD_ALT)) && ses->term->spec->braille) rep_ev(ses, fd, cursor_word, 0);
		else if (upcase(ev->x) == 'T' && !(ev->y & (KBD_CTRL | KBD_ALT)) && ses->term->spec->braille) rep_ev(ses, fd, cursor_word_back, 0);
		else if (((ev->x == KBD_TAB && !ev->y && fd == ses->screen) || (upcase(ev->x) == 'Y' && ev->y & KBD_CTRL)) && ses->term->spec->braille) rep_ev(ses, fd, br_next_link, 0);
		else if (((ev->x == KBD_TAB && ev->y && fd == ses->screen) || (upcase(ev->x) == 'T' && ev->y & KBD_CTRL)) && ses->term->spec->braille) rep_ev(ses, fd, br_prev_link, 0);
		else if (upcase(ev->x) == 'O' && ev->y & KBD_CTRL && ses->term->spec->braille) rep_ev(ses, fd, br_next_link, 1);
		/* Copy current link to clipboard */
		else if ((ev->x == KBD_INS && ev->y & KBD_CTRL) || (upcase(ev->x) == 'C' && ev->y & KBD_CTRL)) {
			unsigned char *current_link = print_current_link(ses);
			if (current_link) {
				set_clipboard_text(ses->term, current_link);
				mem_free(current_link);
			}
		}
		else if (ev->x == KBD_INS || (upcase(ev->x) == 'P' && ev->y & KBD_CTRL)) rep_ev(ses, fd, scroll, -1 - !ses->kbdprefix.rep);
		else if (ev->x == KBD_DEL || (upcase(ev->x) == 'N' && ev->y & KBD_CTRL)) rep_ev(ses, fd, scroll, 1 + !ses->kbdprefix.rep);
		else if (ev->x == '[') rep_ev(ses, fd, hscroll, -1 - 7 * !ses->kbdprefix.rep);
		else if (ev->x == ']') rep_ev(ses, fd, hscroll, 1 + 7 * !ses->kbdprefix.rep);
		/*else if (upcase(ev->x) == 'Y' && ev->y & KBD_CTRL) rep_ev(ses, fd, scroll, -1);
		else if (upcase(ev->x) == 'E' && ev->y & KBD_CTRL) rep_ev(ses, fd, scroll, 1);*/
		else if (ev->x == KBD_HOME || (upcase(ev->x) == 'A' && ev->y & KBD_CTRL)) rep_ev(ses, fd, home, 0);
		else if (ev->x == KBD_END || (upcase(ev->x) == 'E' && ev->y & KBD_CTRL)) rep_ev(ses, fd, x_end, 0);
		else if ((ev->x == KBD_RIGHT && !ses->term->spec->braille) || ev->x == KBD_ENTER) {
			x = enter(ses, fd, 0);
		} else if (ev->x == '*') {
			ses->ds.images ^= 1;
			html_interpret_recursive(ses->screen);
			draw_formatted(ses);
		} else if (ev->x == 'i' && !(ev->y & KBD_ALT)) {
			if (!F || fd->f_data->opt.plain != 2) frm_view_image(ses, fd);
		} else if (ev->x == 'I' && !(ev->y & KBD_ALT)) {
			if (!anonymous) frm_download_image(ses, fd);
		} else if (upcase(ev->x) == 'D' && !(ev->y & KBD_ALT)) {
			if (!anonymous) frm_download(ses, fd);
		} else if (ev->x == '/') search_dlg(ses, fd, 0);
		else if (ev->x == '?') search_back_dlg(ses, fd, 0);
		else if (ev->x == 'n' && !(ev->y & KBD_ALT)) find_next(ses, fd, 0);
		else if (ev->x == 'N' && !(ev->y & KBD_ALT)) find_next_back(ses, fd, 0);
		else if (upcase(ev->x) == 'F' && !(ev->y & (KBD_ALT | KBD_CTRL))) set_frame(ses, fd, 0);
		else if (ev->x >= '1' && ev->x <= '9' && !(ev->y & (KBD_CTRL | KBD_ALT))) {
			struct f_data *f_data = fd->f_data;
			int nl, lnl;
			unsigned char d[2];
			d[0] = (unsigned char)ev->x;
			d[1] = 0;
			nl = f_data->nlinks, lnl = 1;
			while (nl) nl /= 10, lnl++;
			if (lnl > 1) input_field(ses->term, NULL, TEXT_(T_GO_TO_LINK), TEXT_(T_ENTER_LINK_NUMBER), ses, NULL, lnl, d, 1, f_data->nlinks, check_number, TEXT_(T_OK), (void (*)(void *, unsigned char *)) goto_link_number, TEXT_(T_CANCEL), NULL, NULL);
		}
		else x = 0;
	} else if (ev->ev == EV_MOUSE) {
		struct link *l = choose_mouse_link(fd, ev);
		if (l) {
			x = 1;
			fd->vs->current_link = (int)(l - fd->f_data->links);
			fd->vs->orig_link = fd->vs->current_link;
			if (l->type == L_LINK || l->type == L_BUTTON || l->type == L_CHECKBOX || l->type == L_SELECT) if ((ev->b & BM_ACT) == B_UP) {
				fd->active = 1;
				draw_to_window(ses->win, (void (*)(struct terminal *, void *))draw_doc_c, fd);
				change_screen_status(ses);
				print_screen_status(ses);
				if ((ev->b & BM_BUTT) < B_MIDDLE) x = enter(ses, fd, 0);
				else link_menu(ses->term, NULL, ses);
			}

			set_form_position(fd, l, ev);
		}
	} else x = 0;
	ses->kbdprefix.rep = 0;
	return x;
}

struct f_data_c *current_frame(struct session *ses)
{
	struct f_data_c *fd, *fdd;
	fd = ses->screen;
	while (!list_empty(fd->subframes)) {
		int n = fd->vs->frame_pos;
		if (n == -1) break;
		foreach(fdd, fd->subframes) if (!n--) {
			fd = fdd;
			goto r;
		}
		fd = fd->subframes.next;
		r:;
	}
	return fd;
}

static int is_active_frame(struct session *ses, struct f_data_c *f)
{
	struct f_data_c *fd, *fdd;
	fd = ses->screen;
	if (f == fd) return 1;
	while (!list_empty(fd->subframes)) {
		int n = fd->vs->frame_pos;
		if (n == -1) break;
		foreach(fdd, fd->subframes) if (!n--) {
			fd = fdd;
			goto r;
		}
		fd = fd->subframes.next;
		r:;
		if (f == fd) return 1;
	}
	return 0;
}

#ifdef JS
static int event_catchable(struct links_event *ev)
{
	if (!(ev->ev == EV_KBD)) return 0;
	if (ev->x == KBD_TAB || ev->x == KBD_ESC || ev->x == KBD_CTRL_C || ev->x == KBD_CLOSE) return 0;
	return 1;
}

static int call_keyboard_event(struct f_data_c *fd, unsigned char *code, struct links_event *ev)
{
	int keycode;
	unsigned char *shiftkey, *ctrlkey, *altkey;
	unsigned char *nc;
	int nl;
	shiftkey = ev->y & KBD_SHIFT ? "true" : "false";
	ctrlkey = ev->y & KBD_CTRL ? "true" : "false";
	altkey = ev->y & KBD_ALT ? "true" : "false";
	if (ev->x >= 0) {
		if (ev->x < 0x80 || term_charset(fd->ses->term) == utf8_table) keycode = ev->x;
		else keycode = cp2u(ev->x, term_charset(fd->ses->term));
	}
	else if (ev->x == KBD_ENTER) keycode = 13;
	else if (ev->x == KBD_BS) keycode = 8;
	else if (ev->x == KBD_TAB) keycode = 9;
	else if (ev->x == KBD_ESC) keycode = 27;
	else if (ev->x == KBD_INS) keycode = 45;
	else if (ev->x == KBD_DEL) keycode = 46;
	else if (ev->x == KBD_PAGE_UP) keycode = 33;
	else if (ev->x == KBD_PAGE_DOWN) keycode = 34;
	else if (ev->x == KBD_END) keycode = 35;
	else if (ev->x == KBD_HOME) keycode = 36;
	else if (ev->x == KBD_LEFT) keycode = 37;
	else if (ev->x == KBD_UP) keycode = 38;
	else if (ev->x == KBD_RIGHT) keycode = 39;
	else if (ev->x == KBD_DOWN) keycode = 40;
	else if (ev->x == KBD_F1) keycode = 112;
	else if (ev->x == KBD_F2) keycode = 113;
	else if (ev->x == KBD_F3) keycode = 114;
	else if (ev->x == KBD_F4) keycode = 115;
	else if (ev->x == KBD_F5) keycode = 116;
	else if (ev->x == KBD_F6) keycode = 117;
	else if (ev->x == KBD_F7) keycode = 118;
	else if (ev->x == KBD_F8) keycode = 119;
	else if (ev->x == KBD_F9) keycode = 120;
	else if (ev->x == KBD_F10) keycode = 121;
	else if (ev->x == KBD_F11) keycode = 122;
	else if (ev->x == KBD_F12) keycode = 123;
	else return -1;
	nc = init_str();
	nl = 0;
	add_to_str(&nc, &nl, cast_uchar "event = new Object(); event.keyCode = ");
	add_num_to_str(&nc, &nl, keycode);
	add_to_str(&nc, &nl, cast_uchar "; event.shiftKey = ");
	add_to_str(&nc, &nl, shiftkey);
	add_to_str(&nc, &nl, cast_uchar "; event.ctrlKey = ");
	add_to_str(&nc, &nl, ctrlkey);
	add_to_str(&nc, &nl, cast_uchar "; event.altKey = ");
	add_to_str(&nc, &nl, altkey);
	add_to_str(&nc, &nl, cast_uchar "; ");
	add_to_str(&nc, &nl, code);
	jsint_execute_code(fd, nc, nl, -1, -1, -1, ev);
	mem_free(nc);
	return 0;
}
#endif

static int send_to_frame(struct session *ses, struct links_event *ev)
{
	int r;
	struct f_data_c *fd;
#ifdef JS
	int previous_link;
#endif
	fd = current_frame(ses);
	if (!fd) {
		/*internal("document not formatted");*/
		return 0;
	}

#ifdef JS
	previous_link=fd->vs ? fd->vs->current_link : -1;
	if (!event_catchable(ev) || !fd->f_data || !fd->vs) goto dont_catch;
	if (fd->vs->current_link >= 0 && fd->vs->current_link < fd->f_data->nlinks) {
		struct link *l = &fd->f_data->links[fd->vs->current_link];
		if (ev->b < EVH_LINK_KEYDOWN_PROCESSED && l->js_event && l->js_event->keydown_code) {
			ev->b = EVH_LINK_KEYDOWN_PROCESSED;
			if (!(call_keyboard_event(fd, l->js_event->keydown_code, ev))) return 1;
		}
		if (ev->b < EVH_LINK_KEYPRESS_PROCESSED && l->js_event && l->js_event->keypress_code) {
			ev->b = EVH_LINK_KEYPRESS_PROCESSED;
			if (!(call_keyboard_event(fd, l->js_event->keypress_code, ev))) return 1;
		}
	}
	if (ev->b < EVH_DOCUMENT_KEYDOWN_PROCESSED && fd->f_data->js_event && fd->f_data->js_event->keydown_code) {
		ev->b = EVH_DOCUMENT_KEYDOWN_PROCESSED;
		if (!(call_keyboard_event(fd, fd->f_data->js_event->keydown_code, ev))) return 1;
	}
	if (ev->b < EVH_DOCUMENT_KEYPRESS_PROCESSED && fd->f_data->js_event && fd->f_data->js_event->keypress_code) {
		ev->b = EVH_DOCUMENT_KEYPRESS_PROCESSED;
		if (!(call_keyboard_event(fd, fd->f_data->js_event->keypress_code, ev))) return 1;
	}
	dont_catch:
#endif

	if (!F) r = frame_ev(ses, fd, ev);
#ifdef G
	else r = g_frame_ev(ses, fd, ev);
#endif
	if (r == 1) {
		fd->active = 1;
		draw_to_window(ses->win, (void (*)(struct terminal *, void *))draw_doc_c, fd);
		change_screen_status(ses);
		print_screen_status(ses);
	}
	if (r == 3) draw_fd_nrd(fd);
	if (!F && fd->vs) {
#ifdef JS
		if (previous_link!=fd->vs->current_link&&fd->f_data&&previous_link>=0&&previous_link<fd->f_data->nlinks) /* link has changed */
		{
			struct link *l=&(fd->f_data->links[previous_link]);
			struct form_state *fs;

			/* process onchange code, if previous link was a textarea or a textfield and has changed */
			if ((l->type==L_FIELD||l->type==L_AREA) && (fs=find_form_state(fd,l->form)) && fs->changed && l->js_event && l->js_event->change_code)
				fs->changed=0,jsint_execute_code(fd,l->js_event->change_code,strlen(cast_const_char l->js_event->change_code),-1,-1,-1, NULL);

			/* process blur and mouse-out handlers */
			if (l->js_event&&l->js_event->blur_code)
				jsint_execute_code(fd,l->js_event->blur_code,strlen(cast_const_char l->js_event->blur_code),-1,-1,-1, NULL);
			if (l->js_event&&l->js_event->out_code)
				jsint_execute_code(fd,l->js_event->out_code,strlen(cast_const_char l->js_event->out_code),-1,-1,-1, NULL);
		}
		if (previous_link!=fd->vs->current_link&&fd->f_data&&fd->vs->current_link>=0&&fd->vs->current_link<fd->f_data->nlinks)
		{
			struct link *l=&(fd->f_data->links[fd->vs->current_link]);

			/* process focus and mouse-over handlers */
			if (l->js_event&&l->js_event->focus_code)
				jsint_execute_code(fd,l->js_event->focus_code,strlen(cast_const_char l->js_event->focus_code),-1,-1,-1, NULL);
			if (l->js_event&&l->js_event->over_code)
				jsint_execute_code(fd,l->js_event->over_code,strlen(cast_const_char l->js_event->over_code),-1,-1,-1, NULL);
		}
#endif
	}
	return r;
}

void next_frame(struct session *ses, int p)
{
	int n;
	struct view_state *vs;
	struct f_data_c *fd, *fdd;

	if (!(fd = current_frame(ses))) return;
#ifdef G
	ses->locked_link = 0;
#endif
	while ((fd = fd->parent)) {
		n = 0;
		foreach(fdd, fd->subframes) n++;
		vs = fd->vs;
		vs->frame_pos += p;
		if (vs->frame_pos < -!fd->f_data->frame_desc) { vs->frame_pos = n - 1; continue; }
		if (vs->frame_pos >= n) { vs->frame_pos = -!fd->f_data->frame_desc; continue; }
		break;
	}
	if (!fd) fd = ses->screen;
	vs = fd->vs;
	n = 0;
	foreach(fdd, fd->subframes) if (n++ == vs->frame_pos) {
		fd = fdd;
		next_sub:
		if (list_empty(fd->subframes)) break;
		fd = p < 0 ? fd->subframes.prev : fd->subframes.next;
		vs = fd->vs;
		vs->frame_pos = -1;
		if (!fd->f_data || (!fd->f_data->frame_desc && p > 0)) break;
		if (p < 0) foreach(fdd, fd->subframes) vs->frame_pos++;
		else vs->frame_pos = 0;
		goto next_sub;
	}
#ifdef G
	if (F && (fd = current_frame(ses)) && fd->vs && fd->f_data) {
		if (fd->vs->current_link >= 0 && fd->vs->current_link < fd->f_data->nlinks) {
			/*fd->vs->g_display_link = 1;*/
			if (fd->vs->g_display_link && (fd->f_data->links[fd->vs->current_link].type == L_FIELD || fd->f_data->links[fd->vs->current_link].type == L_AREA)) {
				if ((fd->f_data->locked_on = fd->f_data->links[fd->vs->current_link].obj)) fd->ses->locked_link = 1;
			}
		}
	}
#endif
}

void do_for_frame(struct session *ses, void (*f)(struct session *, struct f_data_c *, int), int a)
{
	struct f_data_c *fd = current_frame(ses);
	if (!fd) {
		/*internal("document not formatted");*/
		return;
	}
	f(ses, fd, a);
	if (!F) {
		fd->active = 1;
		draw_to_window(ses->win, (void (*)(struct terminal *, void *))draw_doc_c, fd);
		change_screen_status(ses);
		print_screen_status(ses);
	}
}

static void do_mouse_event(struct session *ses, struct links_event *ev)
{
	struct links_event evv;
	struct f_data_c *fdd, *fd = current_frame(ses);
	if (!fd) return;
	if (ev->x >= fd->xp && ev->x < fd->xp + fd->xw &&
	    ev->y >= fd->yp && ev->y < fd->yp + fd->yw) goto ok;
#ifdef G
	if (ses->scrolling) goto ok;
#endif
	r:
	next_frame(ses, 1);
	fdd = current_frame(ses);
	/*o = &fdd->f_data->opt;*/
	if (ev->x >= fdd->xp && ev->x < fdd->xp + fdd->xw &&
	    ev->y >= fdd->yp && ev->y < fdd->yp + fdd->yw) {
		draw_formatted(ses);
		fd = fdd;
		goto ok;
	}
	if (fdd != fd) goto r;
	return;
	ok:
	memcpy(&evv, ev, sizeof(struct links_event));
	evv.x -= fd->xp;
	evv.y -= fd->yp;
	send_to_frame(ses, &evv);
}

void send_event(struct session *ses, struct links_event *ev)
{
	if (ses->brl_cursor_mode) {
		ses->brl_cursor_mode = 0;
		print_screen_status(ses);
	}
	if (ev->ev == EV_KBD) {
		if (send_to_frame(ses, ev)) return;
		if (ev->y & KBD_ALT && ev->x != KBD_TAB) {
			struct window *m;
			ev->y &= ~KBD_ALT;
			activate_bfu_technology(ses, -1);
			m = ses->term->windows.next;
			m->handler(m, ev, 0);
			if (ses->term->windows.next == m) {
				delete_window(m);
			} else goto x;
			ev->y |= KBD_ALT;
		}
		if (ev->x == KBD_F1) {
			activate_keys(ses);
			goto x;
		}
		if (ev->x == KBD_ESC || ev->x == KBD_F9) {
			activate_bfu_technology(ses, -1);
			goto x;
		}
		if (ev->x == KBD_F10) {
			activate_bfu_technology(ses, 0);
			goto x;
		}
		if (ev->x == KBD_TAB) {
			next_frame(ses, ev->y ? -1 : 1);
			draw_formatted(ses);
		}
		if (ev->x == KBD_LEFT && !ses->term->spec->braille) {
			go_back(ses, 1);
			goto x;
		}
		if (upcase(ev->x) == 'Z' && !(ev->y & (KBD_CTRL | KBD_ALT))) {
			go_back(ses, 1);
			goto x;
		}
		if (ev->x == '\'') {
			go_back(ses, -1);
			goto x;
		}
		if (upcase(ev->x) == 'X' && !(ev->y & (KBD_CTRL | KBD_ALT))) {
			go_back(ses, -1);
			goto x;
		}
		if (upcase(ev->x) == 'A' && ses->term->spec->braille) {
			ses->brl_cursor_mode = 2;
			print_screen_status(ses);
			goto x;
		}
		if (upcase(ev->x) == 'W' && ses->term->spec->braille) {
			ses->brl_cursor_mode = 1;
			print_screen_status(ses);
			goto x;
		}
		if (ev->x == KBD_BS) {
			go_back(ses, 1);
			goto x;
		}
		if (upcase(ev->x) == 'R' && ev->y & KBD_CTRL) {
			reload(ses, -1);
			goto x;
		}
		if (ev->x == 'g' && !(ev->y & (KBD_CTRL | KBD_ALT))) {
			quak:
			dialog_goto_url(ses, cast_uchar "");
			goto x;
		}
		if (ev->x == 'G' && !(ev->y & (KBD_CTRL | KBD_ALT))) {
			unsigned char *s;
			if (list_empty(ses->history)) goto quak;
			s = stracpy(ses->screen->rq->url);
			if (!s) goto quak;
			if (strchr(cast_const_char s, POST_CHAR)) *cast_uchar strchr(cast_const_char s, POST_CHAR) = 0;
			dialog_goto_url(ses, s);
			mem_free(s);
			goto x;
		}
		if (upcase(ev->x) == 'G' && ev->y & KBD_CTRL) {
			struct f_data_c *fd = current_frame(ses);
			if (!fd->vs || !fd->f_data || fd->vs->current_link < 0 || fd->vs->current_link >= fd->f_data->nlinks) goto quak;
			dialog_goto_url(ses, fd->f_data->links[fd->vs->current_link].where);
			goto x;
		}
		/*
		if (upcase(ev->x) == 'A' && !(ev->y & (KBD_CTRL | KBD_ALT))) {
			if (!anonymous) menu_bookmark_manager(ses->term, NULL, ses);
			goto x;
		}
		*/
		if (upcase(ev->x) == 'S' && !(ev->y & (KBD_CTRL | KBD_ALT))) {
			if (!anonymous) menu_bookmark_manager(ses->term, NULL, ses);
			goto x;
		}
		if ((upcase(ev->x) == 'Q' && !(ev->y & (KBD_CTRL | KBD_ALT))) || ev->x == KBD_CTRL_C) {
			exit_prog(ses->term, (void *)(my_intptr_t)(ev->x == KBD_CTRL_C || ev->x == 'Q'), ses);
			goto x;
		}
		if (ev->x == KBD_CLOSE) {
			really_exit_prog(ses);
			goto x;
		}
		if (ev->x == '=') {
			state_msg(ses);
			goto x;
		}
		if (ev->x == '|') {
			head_msg(ses);
			goto x;
		}
		if (ev->x == '\\') {
			toggle(ses, ses->screen, 0);
			goto x;
		}
	}
	if (ev->ev == EV_MOUSE) {
		if (ev->b == (B_DOWN | B_FOURTH)) {
			go_back(ses, 1);
			goto x;
		}
		if (ev->b == (B_DOWN | B_FIFTH)) {
			go_back(ses, -1);
			goto x;
		}
#ifdef G
		if (ses->locked_link) {
			if ((ev->b & BM_ACT) != B_MOVE) {
				ses->locked_link = 0;
#ifdef JS
				/* process onblur handler of current link */
				if (ses->screen&&ses->screen->vs&&ses->screen->f_data&&ses->screen->vs->current_link>=0&&ses->screen->vs->current_link<ses->screen->f_data->nlinks)
				{
					struct link *lnk=&(ses->screen->f_data->links[ses->screen->vs->current_link]);
					struct form_state *fs;
					/* select se dela jinde */
					if (lnk->type!=L_SELECT&&lnk->js_event&&lnk->js_event->blur_code)
						jsint_execute_code(current_frame(ses),lnk->js_event->blur_code,strlen(cast_const_char lnk->js_event->blur_code),-1,-1,-1, NULL);

					/* execute onchange handler of text field/area */
					if ((lnk->type==L_AREA||lnk->type==L_FIELD)&&lnk->js_event&&lnk->js_event->change_code&&(fs=find_form_state(ses->screen,lnk->form))&&fs->changed)
						fs->changed=0,jsint_execute_code(current_frame(ses),lnk->js_event->change_code,strlen(cast_const_char lnk->js_event->change_code),-1,-1,-1, NULL);
				}
#endif
				clr_xl(ses->screen);
				draw_formatted(ses);
			} else return;
		}
#endif
		if (ev->y >= 0 && ev->y < gf_val(1, G_BFU_FONT_SIZE) && ev->x >=0 && ev->x < ses->term->x && (ev->b & BM_ACT) == B_DOWN) {
#ifdef G
			if (F && ev->x < ses->back_size) {
				go_back(ses, 1);
				goto x;
			} else
#endif
			{
				struct window *m;
				activate_bfu_technology(ses, -1);
				m = ses->term->windows.next;
				m->handler(m, ev, 0);
				goto x;
			}
		}
		do_mouse_event(ses, ev);
	}
	return;
	x:
	ses->kbdprefix.rep = 0;
}

static void send_enter(struct terminal *term, void *xxx, struct session *ses)
{
	struct links_event ev = { EV_KBD, KBD_ENTER, 0, 0 };
	send_event(ses, &ev);
}

void frm_download(struct session *ses, struct f_data_c *fd)
{
	struct link *link = get_current_link(fd);
	if (!link) return;
	if (ses->dn_url) mem_free(ses->dn_url), ses->dn_url = NULL;
	if (link->type != L_LINK && link->type != L_BUTTON) return;
	if ((ses->dn_url = get_link_url(ses, fd, link, NULL))) {
		ses->dn_allow_flags = f_data_c_allow_flags(fd);
		if (!casecmp(ses->dn_url, cast_uchar "MAP@", 4)) {
			mem_free(ses->dn_url);
			ses->dn_url = NULL;
			return;
		}
		query_file(ses, ses->dn_url, NULL, start_download, NULL, DOWNLOAD_CONTINUE);
	}
}

void frm_view_image(struct session *ses, struct f_data_c *fd)
{
	struct link *link = get_current_link(fd);
	if (!link) return;
	if (link->type != L_LINK && link->type != L_BUTTON) return;
	if (!link->where_img) return;
	goto_url_not_from_dialog(ses, link->where_img, fd);
}

void frm_download_image(struct session *ses, struct f_data_c *fd)
{
	struct link *link = get_current_link(fd);
	if (!link) return;
	if (ses->dn_url) mem_free(ses->dn_url), ses->dn_url = NULL;
	if (link->type != L_LINK && link->type != L_BUTTON) return;
	if (!link->where_img) return;
	if ((ses->dn_url = stracpy(link->where_img))) {
		ses->dn_allow_flags = f_data_c_allow_flags(fd);
		if (!casecmp(ses->dn_url, cast_uchar "MAP@", 4)) {
			mem_free(ses->dn_url);
			ses->dn_url = NULL;
			return;
		}
		query_file(ses, ses->dn_url, NULL, start_download, NULL, DOWNLOAD_CONTINUE);
	}
}

static void send_download_image(struct terminal *term, void *xxx, struct session *ses)
{
	struct f_data_c *fd = current_frame(ses);
	struct link *link = get_current_link(fd);
	if (!link) return;
	if (ses->dn_url) mem_free(ses->dn_url);
	if ((ses->dn_url = stracpy(link->where_img))) {
		ses->dn_allow_flags = f_data_c_allow_flags(fd);
		query_file(ses, ses->dn_url, NULL, start_download, NULL, DOWNLOAD_CONTINUE);
	}
}

#ifdef G
static void send_block_image(struct terminal *term, void *xxx, struct session *ses)
{
	struct f_data_c *fd = current_frame(ses);
	struct link *link = get_current_link(fd);
	if (!link) return;
	if (!link->where_img) return;
	block_url_query(ses, link->where_img);
}
#endif

static void send_download(struct terminal *term, void *xxx, struct session *ses)
{
	struct f_data_c *fd = current_frame(ses);
	struct link *link = get_current_link(fd);
	if (!link) return;
	if (ses->dn_url) mem_free(ses->dn_url);
	if ((ses->dn_url = get_link_url(ses, fd, link, NULL))) {
		ses->dn_allow_flags = f_data_c_allow_flags(fd);
		query_file(ses, ses->dn_url, NULL, start_download, NULL, DOWNLOAD_CONTINUE);
	}
}

static void send_submit(struct terminal *term, void *xxx, struct session *ses)
{
	int has_onsubmit;
	struct form_control *form;
	struct f_data_c *fd = current_frame(ses);
	struct link *link = get_current_link(fd);
	unsigned char *u;

	if (!link) return;
	if (!(form=link->form)) return;
	u=get_form_url(ses,fd,form,&has_onsubmit);
	if (u) {
		goto_url_f(fd->ses,NULL,u,NULL,fd,form->form_num, has_onsubmit,0,0);
		mem_free(u);
	}
	draw_fd(fd);
}

static void send_reset(struct terminal *term, void *xxx, struct session *ses)
{
	struct form_control *form;
	struct f_data_c *fd = current_frame(ses);
	struct link *link = get_current_link(fd);

	if (!link) return;
	if (!(form=link->form)) return;
	reset_form(fd,form->form_num);
	draw_fd(fd);
}

static void copy_link_location(struct terminal *term, void *xxx, struct session *ses)
{
	unsigned char *current_link = print_current_link(ses);

	if (current_link) {
		set_clipboard_text(term, current_link);
		mem_free(current_link);
	}

}

void copy_url_location(struct terminal *term, void *xxx, struct session *ses)
{
	unsigned char *url;
	struct location *current_location;

	if (list_empty(ses->history)) return;

	if ((current_location = cur_loc(ses)) && (url = stracpy(current_location->url))) {
		if (strchr(cast_const_char url, POST_CHAR)) *cast_uchar strchr(cast_const_char url, POST_CHAR) = 0;
		set_clipboard_text(term, url);
		mem_free(url);
	}
}

static void cant_open_new_window(struct terminal *term)
{
	msg_box(term, NULL, TEXT_(T_NEW_WINDOW), AL_CENTER, TEXT_(T_UNABLE_TO_OPEN_NEW_WINDOW), NULL, 1, TEXT_(T_CANCEL), NULL, B_ENTER | B_ESC);
}

/* open a link in a new xterm, pass target frame name */
static void send_open_in_new_xterm(struct terminal *term, int (*open_window)(struct terminal *, unsigned char *, unsigned char *), struct session *ses)
{
	struct f_data_c *fd = current_frame(ses);
	if (!fd) return;
	if (fd->vs->current_link == -1) return;
	if (ses->dn_url) mem_free(ses->dn_url);
	if ((ses->dn_url = get_link_url(ses, fd, &fd->f_data->links[fd->vs->current_link], NULL))) {
		unsigned char *p;
		int pl;
		unsigned char *enc_url;
		unsigned char *path;

		ses->dn_allow_flags = f_data_c_allow_flags(fd);
		if (disallow_url(ses->dn_url, ses->dn_allow_flags)) {
			mem_free(ses->dn_url);
			ses->dn_url = NULL;
			return;
		}

		p = init_str();
		pl = 0;

		add_to_str(&p, &pl, cast_uchar "-base-session ");
		add_num_to_str(&p, &pl, ses->id);
		add_chr_to_str(&p, &pl, ' ');

		if (ses->wtd_target && *ses->wtd_target) {
			unsigned char *tgt = stracpy(ses->wtd_target);

			check_shell_security(&tgt);
			add_to_str(&p, &pl, cast_uchar "-target ");
			add_to_str(&p, &pl, tgt);
			add_chr_to_str(&p, &pl, ' ');
			mem_free(tgt);
		}
		enc_url = encode_url(ses->dn_url);
		add_to_str(&p, &pl, enc_url);
		mem_free(enc_url);
		path = escape_path(path_to_exe);
		if (open_window(term, path, p))
			cant_open_new_window(term);
		mem_free(p);
		mem_free(path);
	}
}

void send_open_new_xterm(struct terminal *term, int (*open_window)(struct terminal *, unsigned char *, unsigned char *), struct session *ses)
{
	unsigned char *p = init_str();
	int pl = 0;
	unsigned char *path;
	add_to_str(&p, &pl, cast_uchar "-base-session ");
	add_num_to_str(&p, &pl, ses->id);
	path = escape_path(path_to_exe);
	if (open_window(term, path, p))
		cant_open_new_window(term);
	mem_free(path);
	mem_free(p);
}

void open_in_new_window(struct terminal *term, void (*xxx)(struct terminal *, int (*)(struct terminal *, unsigned char *, unsigned char *), struct session *ses), struct session *ses)
{
	/*int e = term->environment;*/
	struct menu_item *mi;
	struct open_in_new *oin, *oi;
	if (!(oin = get_open_in_new(term->environment))) return;
	if (!oin[1].text) {
		xxx(term, oin[0].open_window_fn, ses);
		mem_free(oin);
		return;
	}
	mi = new_menu(1);
	for (oi = oin; oi->text; oi++) add_to_menu(&mi, oi->text, cast_uchar "", oi->hk, MENU_FUNC xxx, (void *)oi->open_window_fn, 0, -1);
	mem_free(oin);
	do_menu(term, mi, ses);
}

int can_open_in_new(struct terminal *term)
{
	struct open_in_new *oin = get_open_in_new(term->environment);
	if (!oin) return 0;
	if (!oin[1].text) {
		mem_free(oin);
		return 1;
	}
	mem_free(oin);
	return 2;
}

void save_url(struct session *ses, unsigned char *url)
{
	unsigned char *u;
	if (!(u = translate_url(url, ses->term->cwd))) {
		struct status stat = { NULL, NULL, NULL, NULL, S_BAD_URL, PRI_CANCEL, 0, NULL, NULL, NULL };
		print_error_dialog(ses, &stat, TEXT_(T_ERROR));
		return;
	}
	if (ses->dn_url) mem_free(ses->dn_url);
	ses->dn_url = u;
	ses->dn_allow_flags = ALLOW_ALL;
	query_file(ses, ses->dn_url, NULL, start_download, NULL, DOWNLOAD_CONTINUE);
}

static void send_image(struct terminal *term, void *xxx, struct session *ses)
{
	unsigned char *u;
	struct f_data_c *fd = current_frame(ses);
	if (!fd) return;
	if (fd->vs->current_link == -1) return;
	if (!(u = fd->f_data->links[fd->vs->current_link].where_img)) return;
	goto_url_not_from_dialog(ses, u, fd);
}

#ifdef G

static void send_scale(struct terminal *term, void *xxx, struct session *ses)
{
	ses->ds.porn_enable ^= 1;
	html_interpret_recursive(ses->screen);
	draw_formatted(ses);
}

#endif

void save_as(struct terminal *term, void *xxx, struct session *ses)
{
	unsigned char *head;
	if (list_empty(ses->history)) return;
	if (ses->dn_url) mem_free(ses->dn_url);
	ses->dn_url = stracpy(ses->screen->rq->url);
	ses->dn_allow_flags = ALLOW_ALL;
	if (!ses->dn_url) return;
	head = stracpy(ses->screen->rq->ce ? ses->screen->rq->ce->head : NULL);
	if (head) {
		unsigned char *p, *q;
		/* remove Content-Encoding from the header */
		q = parse_http_header(head, cast_uchar "Content-Encoding", &p);
		if (q) {
			mem_free(q);
			if (p > head && p < (unsigned char *)strchr(cast_const_char head, 0)) {
				for (q = p - 1; q > head && *q != 10; q--)
					;
				q[1] = 'X';
			}
		}
	}
	query_file(ses, ses->dn_url, head, start_download, NULL, DOWNLOAD_CONTINUE);
	if (head)
		mem_free(head);
}

static void save_formatted(struct session *ses, unsigned char *file, int mode)
{
	int h;
	int rs;
	struct f_data_c *f;
	int download_mode = mode == DOWNLOAD_DEFAULT ? CDF_EXCL : 0;
	if (!(f = current_frame(ses)) || !f->f_data) return;
	if ((h = create_download_file(ses, ses->term->cwd, file, download_mode, 0)) < 0) return;
	if (dump_to_file(f->f_data, h))
		msg_box(ses->term, NULL, TEXT_(T_SAVE_ERROR), AL_CENTER, TEXT_(T_ERROR_WRITING_TO_FILE), NULL, 1, TEXT_(T_CANCEL), NULL, B_ENTER | B_ESC);
	EINTRLOOP(rs, close(h));
}

void menu_save_formatted(struct terminal *term, void *xxx, struct session *ses)
{
	struct f_data_c *f;
	if (!(f = current_frame(ses)) || !f->f_data) return;
	query_file(ses, f->rq->url, NULL, save_formatted, NULL, DOWNLOAD_OVERWRITE);
}

void link_menu(struct terminal *term, void *xxx, struct session *ses)
{
	struct f_data_c *f = current_frame(ses);
	struct link *link;
	struct menu_item *mi;
	if (ses->wtd_target) mem_free(ses->wtd_target), ses->wtd_target = NULL;
	mi = new_menu(1);
	link = get_current_link(f);
	if (!link) goto no_l;
	if (link->type == L_LINK && link->where) {
		if (strlen(cast_const_char link->where) >= 4 && !casecmp(link->where, cast_uchar "MAP@", 4)) {
			if (!F) {
				add_to_menu(&mi, TEXT_(T_DISPLAY_USEMAP), cast_uchar ">", TEXT_(T_HK_DISPLAY_USEMAP), MENU_FUNC send_enter, NULL, 1, -1);
			}
		} else {
			int c = can_open_in_new(term);
			add_to_menu(&mi, TEXT_(T_FOLLOW_LINK), cast_uchar "Enter", TEXT_(T_HK_FOLLOW_LINK), MENU_FUNC send_enter, NULL, 0, -1);
			if (c) add_to_menu(&mi, TEXT_(T_OPEN_IN_NEW_WINDOW), c - 1 ? cast_uchar ">" : cast_uchar "", TEXT_(T_HK_OPEN_IN_NEW_WINDOW), MENU_FUNC open_in_new_window, (void *)send_open_in_new_xterm, c - 1, -1);
			if (!anonymous) add_to_menu(&mi, TEXT_(T_DOWNLOAD_LINK), cast_uchar "d", TEXT_(T_HK_DOWNLOAD_LINK), MENU_FUNC send_download, NULL, 0, -1);
			if (clipboard_support(term))
				add_to_menu(&mi, TEXT_(T_COPY_LINK_LOCATION), cast_uchar "", TEXT_(T_HK_COPY_LINK_LOCATION), MENU_FUNC copy_link_location, NULL, 0, -1);
			/*add_to_menu(&mi, TEXT_(T_ADD_BOOKMARK), cast_uchar "A", TEXT_(T_HK_ADD_BOOKMARK), MENU_FUNC menu_bookmark_manager, NULL, 0);*/

		}
	}
	if ((link->type == L_CHECKBOX || link->type == L_SELECT || link->type == L_FIELD || link->type == L_AREA) && link->form){
		int c = can_open_in_new(term);
		add_to_menu(&mi, TEXT_(T_SUBMIT_FORM), cast_uchar "", TEXT_(T_HK_SUBMIT_FORM), MENU_FUNC send_submit, NULL, 0, -1);
		if (c && link->form->method == FM_GET) add_to_menu(&mi, TEXT_(T_SUBMIT_FORM_AND_OPEN_IN_NEW_WINDOW), c - 1 ? cast_uchar ">" : cast_uchar "", TEXT_(T_HK_SUBMIT_FORM_AND_OPEN_IN_NEW_WINDOW), MENU_FUNC open_in_new_window, (void *)send_open_in_new_xterm, c - 1, -1);
		/*if (!anonymous) add_to_menu(&mi, TEXT_(T_SUBMIT_FORM_AND_DOWNLOAD), cast_uchar "d", TEXT_(T_HK_SUBMIT_FORM_AND_DOWNLOAD), MENU_FUNC send_download, NULL, 0, -1);*/
		add_to_menu(&mi, TEXT_(T_RESET_FORM), cast_uchar "", TEXT_(T_HK_RESET_FORM), MENU_FUNC send_reset, NULL, 0, -1);
	}
	if (link->type == L_BUTTON && link->form) {
		if (link->form->type == FC_RESET) add_to_menu(&mi, TEXT_(T_RESET_FORM), cast_uchar "", TEXT_(T_HK_RESET_FORM), MENU_FUNC send_enter, NULL, 0, -1);
		else if (link->form->type==FC_BUTTON)
			;
		else if (link->form->type == FC_SUBMIT || link->form->type == FC_IMAGE) {
			int c = can_open_in_new(term);
			add_to_menu(&mi, TEXT_(T_SUBMIT_FORM), cast_uchar "", TEXT_(T_HK_SUBMIT_FORM), MENU_FUNC send_enter, NULL, 0, -1);
			if (c && link->form->method == FM_GET) add_to_menu(&mi, TEXT_(T_SUBMIT_FORM_AND_OPEN_IN_NEW_WINDOW), c - 1 ? cast_uchar ">" : cast_uchar "", TEXT_(T_HK_SUBMIT_FORM_AND_OPEN_IN_NEW_WINDOW), MENU_FUNC open_in_new_window, (void *)send_open_in_new_xterm, c - 1, -1);
			if (!anonymous) add_to_menu(&mi, TEXT_(T_SUBMIT_FORM_AND_DOWNLOAD), cast_uchar "d", TEXT_(T_HK_SUBMIT_FORM_AND_DOWNLOAD), MENU_FUNC send_download, NULL, 0, -1);
		}
	}
	if (link->where_img) {
		if (!F || f->f_data->opt.plain != 2) add_to_menu(&mi, TEXT_(T_VIEW_IMAGE), cast_uchar "i", TEXT_(T_HK_VIEW_IMAGE), MENU_FUNC send_image, NULL, 0, -1);
#ifdef G
		else add_to_menu(&mi, TEXT_(T_SCALE_IMAGE_TO_FULL_SCREEN), cast_uchar "Enter", TEXT_(T_HK_SCALE_IMAGE_TO_FULL_SCREEN), MENU_FUNC send_scale, NULL, 0, -1);
#endif
		if (!anonymous) add_to_menu(&mi, TEXT_(T_DOWNLOAD_IMAGE), cast_uchar "I", TEXT_(T_HK_DOWNLOAD_IMAGE), MENU_FUNC send_download_image, NULL, 0, -1);
#ifdef G
		if (F && !anonymous) add_to_menu(&mi, TEXT_(T_BLOCK_URL), cast_uchar "", TEXT_(T_HK_BLOCK_URL), MENU_FUNC send_block_image, NULL, 0, -1);
#endif
	}
	no_l:
	if (!mi->text) add_to_menu(&mi, TEXT_(T_NO_LINK_SELECTED), cast_uchar "", M_BAR, NULL, NULL, 0, -1);
	do_menu(term, mi, ses);
}

static unsigned char *print_current_titlex(struct f_data_c *fd, int w)
{
	int mul, pul;
	int ml = 0, pl = 0;
	unsigned char *m, *p;
	if (!fd || !fd->vs || !fd->f_data) return NULL;
	w -= 1;
	p = init_str();
	if (fd->yw < fd->f_data->y) {
		int pp, pe;
		if (fd->yw) {
			pp = (fd->vs->view_pos + fd->yw / 2) / fd->yw + 1;
			pe = (fd->f_data->y + fd->yw - 1) / fd->yw;
		} else pp = pe = 1;
		if (pp > pe) pp = pe;
		if (fd->vs->view_pos + fd->yw >= fd->f_data->y) pp = pe;
		if (fd->f_data->title && !fd->ses->term->spec->braille) add_chr_to_str(&p, &pl, ' ');
		add_to_str(&p, &pl, _(TEXT_(T_PAGE_P), fd->ses->term));
		add_num_to_str(&p, &pl, pp);
		add_to_str(&p, &pl, _(TEXT_(T_PAGE_OF), fd->ses->term));
		add_num_to_str(&p, &pl, pe);
		add_to_str(&p, &pl, _(TEXT_(T_PAGE_CL), fd->ses->term));
		if (fd->f_data->title && fd->ses->term->spec->braille) add_chr_to_str(&p, &pl, ' ');
	}
	if (!fd->f_data->title) return p;
	if (fd->ses->term->spec->braille) {
		add_to_str(&p, &pl, fd->f_data->title);
		return p;
	}
	m = init_str();
	add_to_str(&m, &ml, fd->f_data->title);
	mul = cp_len(term_charset(fd->ses->term), m);
	pul = cp_len(term_charset(fd->ses->term), p);
	if (mul + pul > w) {
		unsigned char *mm;
		if ((mul = w - pul) < 0) mul = 0;
		for (mm = m; mul--; GET_TERM_CHAR(fd->ses->term, &mm))
			;
		ml = (int)(mm - m);
	}
	add_to_str(&m, &ml, p);
	mem_free(p);
	return m;
}

static unsigned char *print_current_linkx(struct f_data_c *fd, struct terminal *term)
{
	int ll = 0;
	struct link *l;
	unsigned char *m = NULL /* shut up warning */;
	if (!fd || !fd->vs || !fd->f_data) return NULL;
	if (fd->vs->current_link == -1 || fd->vs->current_link >= fd->f_data->nlinks || fd->f_data->frame_desc) return NULL;
	l = &fd->f_data->links[fd->vs->current_link];
	if (l->type == L_LINK) {
		if (!l->where && l->where_img) {
			m = init_str();
			ll = 0;
			if (l->img_alt)
			{
				unsigned char *txt;
				struct conv_table* ct;

				ct=get_translation_table(fd->f_data->cp,fd->f_data->opt.cp);
				txt = convert_string(ct, l->img_alt, (int)strlen(cast_const_char l->img_alt), &fd->f_data->opt);
				add_to_str(&m, &ll, txt);
				mem_free(txt);
			}
			else
			{
				add_to_str(&m, &ll, _(TEXT_(T_IMAGE), term));
				add_to_str(&m, &ll, cast_uchar " ");
				add_to_str(&m, &ll, l->where_img);
			}
			goto p;
		}
		if (l->where && strlen(cast_const_char l->where) >= 4 && !casecmp(l->where, cast_uchar "MAP@", 4)) {
			m = init_str();
			ll = 0;
			add_to_str(&m, &ll, _(TEXT_(T_USEMAP), term));
			add_to_str(&m, &ll, cast_uchar " ");
			add_to_str(&m, &ll, l->where + 4);
			goto p;
		}
		if (l->where) {
			m = stracpy(l->where);
			goto p;
		}
		m = print_js_event_spec(l->js_event);
		goto p;
	}
	if (!l->form) return NULL;
	if (l->type == L_BUTTON) {
		if (l->form->type == FC_BUTTON) {
			unsigned char *n;
			unsigned char *txt;
			m = init_str();
			ll = 0;
			add_to_str(&m, &ll, _(TEXT_(T_BUTTON), term));
			if (!l->js_event) goto p;
			add_to_str(&m, &ll, cast_uchar " ");
			n=print_js_event_spec(l->js_event);
			if (fd->f_data)
			{
				struct conv_table* ct;

				ct=get_translation_table(fd->f_data->cp,fd->f_data->opt.cp);
				txt=convert_string(ct,n,(int)strlen(cast_const_char n),NULL);
				mem_free(n);
			}
			else
				txt=n;
			add_to_str(&m, &ll, txt);
			mem_free(txt);
			goto p;
		}
		if (l->form->type == FC_RESET) {
			m = stracpy(_(TEXT_(T_RESET_FORM), term));
			goto p;
		}
		if (!l->form->action) return NULL;
		m = init_str();
		ll = 0;
		if (l->form->method == FM_GET) add_to_str(&m, &ll, _(TEXT_(T_SUBMIT_FORM_TO), term));
		else add_to_str(&m, &ll, _(TEXT_(T_POST_FORM_TO), term));
		add_to_str(&m, &ll, cast_uchar " ");
		add_to_str(&m, &ll, l->form->action);
		goto p;
	}
	if (l->type == L_CHECKBOX || l->type == L_SELECT || l->type == L_FIELD || l->type == L_AREA) {
		m = init_str();
		ll = 0;
		if (l->form->type == FC_RADIO) add_to_str(&m, &ll, _(TEXT_(T_RADIO_BUTTON), term));
		else if (l->form->type == FC_CHECKBOX) add_to_str(&m, &ll, _(TEXT_(T_CHECKBOX), term));
		else if (l->form->type == FC_SELECT) add_to_str(&m, &ll, _(TEXT_(T_SELECT_FIELD), term));
		else if (l->form->type == FC_TEXT) add_to_str(&m, &ll, _(TEXT_(T_TEXT_FIELD), term));
		else if (l->form->type == FC_TEXTAREA) add_to_str(&m, &ll, _(TEXT_(T_TEXT_AREA), term));
		else if (l->form->type == FC_FILE) add_to_str(&m, &ll, _(TEXT_(T_FILE_UPLOAD), term));
		else if (l->form->type == FC_PASSWORD) add_to_str(&m, &ll, _(TEXT_(T_PASSWORD_FIELD), term));
		else {
			mem_free(m);
			return NULL;
		}
		if (l->form->name && l->form->name[0]) add_to_str(&m, &ll, cast_uchar ", "), add_to_str(&m, &ll, _(TEXT_(T_NAME), term)), add_to_str(&m, &ll, cast_uchar " "), add_to_str(&m, &ll, l->form->name);
		if ((l->form->type == FC_CHECKBOX || l->form->type == FC_RADIO) && l->form->default_value && l->form->default_value[0]) add_to_str(&m, &ll, cast_uchar ", "), add_to_str(&m, &ll, _(TEXT_(T_VALUE), term)), add_to_str(&m, &ll, cast_uchar " "), add_to_str(&m, &ll, l->form->default_value);
				       /* pri enteru se bude posilat vzdycky   -- Brain */
		if (l->type == L_FIELD && !has_form_submit(fd->f_data, l->form)  && l->form->action) {
			add_to_str(&m, &ll, cast_uchar ", ");
			add_to_str(&m, &ll, _(TEXT_(T_HIT_ENTER_TO), term));
			add_to_str(&m, &ll, cast_uchar " ");
			if (l->form->method == FM_GET) add_to_str(&m, &ll, _(TEXT_(T_SUBMIT_TO), term));
			else add_to_str(&m, &ll, _(TEXT_(T_POST_TO), term));
			add_to_str(&m, &ll, cast_uchar " ");
			add_to_str(&m, &ll, l->form->action);
		}
		goto p;
	}
	p:
	return m;
}

/* jako print_current_linkx, ale vypisuje vice informaci o obrazku
   pouziva se v informacich o dokumentu

   Ach jo, to Brain kopiroval kod, snad to nedela i v ty firme,
   kde ted pracuje... -- mikulas
 */
static unsigned char *print_current_linkx_plus(struct f_data_c *fd, struct terminal *term)
{
	int ll = 0;
	struct link *l;
	unsigned char *m = NULL /* shut up warning */;
	if (!fd || !fd->vs || !fd->f_data) return NULL;
	if (fd->vs->current_link == -1 || fd->vs->current_link >= fd->f_data->nlinks || fd->f_data->frame_desc) return NULL;
	l = &fd->f_data->links[fd->vs->current_link];
	if (l->type == L_LINK) {
		unsigned char *spc;
		m = init_str();
		ll = 0;
		if (l->where && strlen(cast_const_char l->where) >= 4 && !casecmp(l->where, cast_uchar "MAP@", 4)) {
			add_to_str(&m, &ll, _(TEXT_(T_USEMAP), term));
			add_to_str(&m, &ll, cast_uchar " ");
			add_to_str(&m, &ll, l->where + 4);
		}
		else if (l->where) {
			add_to_str(&m, &ll, l->where);
		}
		spc = print_js_event_spec(l->js_event);
		if (spc&&*spc)
		{
			add_to_str(&m, &ll, cast_uchar "\n");
			add_to_str(&m, &ll, _(TEXT_(T_JAVASCRIPT), term));
			add_to_str(&m, &ll, cast_uchar ": ");
			add_to_str(&m, &ll, spc);
		}
		if (spc) mem_free(spc);
		if (l->where_img) {
			add_to_str(&m, &ll, cast_uchar "\n");
			add_to_str(&m, &ll, _(TEXT_(T_IMAGE), term));
			add_to_str(&m, &ll, cast_uchar ": src='");
			add_to_str(&m, &ll, l->where_img);
			add_to_str(&m, &ll, cast_uchar "'");

			if (l->img_alt)
			{
				unsigned char *txt;
				struct conv_table* ct;

				add_to_str(&m, &ll, cast_uchar " alt='");
				ct=get_translation_table(fd->f_data->cp,fd->f_data->opt.cp);
				txt = convert_string(ct, l->img_alt, (int)strlen(cast_const_char l->img_alt), &fd->f_data->opt);
				add_to_str(&m, &ll, txt);
				add_to_str(&m, &ll, cast_uchar "'");
				mem_free(txt);
			}
#ifdef G
			if (F&&l->obj)
			{
				add_to_str(&m, &ll, cast_uchar " size='");
				add_num_to_str(&m, &ll, l->obj->xw);
				add_to_str(&m, &ll, cast_uchar "x");
				add_num_to_str(&m, &ll, l->obj->yw);
				add_to_str(&m, &ll, cast_uchar "'");
			}
#endif
			goto p;
		}
		goto p;
	}
	if (!l->form) return NULL;
	if (l->type == L_BUTTON) {
		if (l->form->type == FC_BUTTON) {
			unsigned char *n;
			unsigned char *txt;
			m = init_str();
			ll = 0;
			add_to_str(&m, &ll, _(TEXT_(T_BUTTON), term));
			if (!l->js_event) goto p;
			add_to_str(&m, &ll, cast_uchar " ");
			n=print_js_event_spec(l->js_event);
			if (fd->f_data)
			{
				struct conv_table* ct;

				ct=get_translation_table(fd->f_data->cp,fd->f_data->opt.cp);
				txt=convert_string(ct,n,(int)strlen(cast_const_char n),NULL);
				mem_free(n);
			}
			else
				txt=n;
			add_to_str(&m, &ll, txt);
			mem_free(txt);
			goto p;
		}
		if (l->form->type == FC_RESET) {
			m = stracpy(_(TEXT_(T_RESET_FORM), term));
			goto p;
		}
		if (!l->form->action) return NULL;
		m = init_str();
		ll = 0;
		if (l->form->method == FM_GET) add_to_str(&m, &ll, _(TEXT_(T_SUBMIT_FORM_TO), term));
		else add_to_str(&m, &ll, _(TEXT_(T_POST_FORM_TO), term));
		add_to_str(&m, &ll, cast_uchar " ");
		add_to_str(&m, &ll, l->form->action);
		goto p;
	}
	if (l->type == L_CHECKBOX || l->type == L_SELECT || l->type == L_FIELD || l->type == L_AREA) {
		m = init_str();
		ll = 0;
		if (l->form->type == FC_RADIO) add_to_str(&m, &ll, _(TEXT_(T_RADIO_BUTTON), term));
		else if (l->form->type == FC_CHECKBOX) add_to_str(&m, &ll, _(TEXT_(T_CHECKBOX), term));
		else if (l->form->type == FC_SELECT) add_to_str(&m, &ll, _(TEXT_(T_SELECT_FIELD), term));
		else if (l->form->type == FC_TEXT) add_to_str(&m, &ll, _(TEXT_(T_TEXT_FIELD), term));
		else if (l->form->type == FC_TEXTAREA) add_to_str(&m, &ll, _(TEXT_(T_TEXT_AREA), term));
		else if (l->form->type == FC_FILE) add_to_str(&m, &ll, _(TEXT_(T_FILE_UPLOAD), term));
		else if (l->form->type == FC_PASSWORD) add_to_str(&m, &ll, _(TEXT_(T_PASSWORD_FIELD), term));
		else {
			mem_free(m);
			return NULL;
		}
		if (l->form->name && l->form->name[0]) add_to_str(&m, &ll, cast_uchar ", "), add_to_str(&m, &ll, _(TEXT_(T_NAME), term)), add_to_str(&m, &ll, cast_uchar " "), add_to_str(&m, &ll, l->form->name);
		if ((l->form->type == FC_CHECKBOX || l->form->type == FC_RADIO) && l->form->default_value && l->form->default_value[0]) add_to_str(&m, &ll, cast_uchar ", "), add_to_str(&m, &ll, _(TEXT_(T_VALUE), term)), add_to_str(&m, &ll, cast_uchar " "), add_to_str(&m, &ll, l->form->default_value);
				       /* pri enteru se bude posilat vzdycky   -- Brain */
		if (l->type == L_FIELD && !has_form_submit(fd->f_data, l->form)  && l->form->action) {
			add_to_str(&m, &ll, cast_uchar ", ");
			add_to_str(&m, &ll, _(TEXT_(T_HIT_ENTER_TO), term));
			add_to_str(&m, &ll, cast_uchar " ");
			if (l->form->method == FM_GET) add_to_str(&m, &ll, _(TEXT_(T_SUBMIT_TO), term));
			else add_to_str(&m, &ll, _(TEXT_(T_POST_TO), term));
			add_to_str(&m, &ll, cast_uchar " ");
			add_to_str(&m, &ll, l->form->action);
		}
		goto p;
	}
	p:
	return m;
}

unsigned char *print_current_link(struct session *ses)
{
	return print_current_linkx(current_frame(ses), ses->term);
}

unsigned char *print_current_title(struct session *ses)
{
	return print_current_titlex(current_frame(ses), ses->term->x);
}

void loc_msg(struct terminal *term, struct location *lo, struct f_data_c *frame)
{
	struct cache_entry *ce;
	unsigned char *s;
	int l = 0;
	unsigned char *a;
	if (!lo || !frame || !frame->vs || !frame->f_data) {
		msg_box(term, NULL, TEXT_(T_INFO), AL_LEFT, TEXT_(T_YOU_ARE_NOWHERE), NULL, 1, TEXT_(T_OK), NULL, B_ENTER | B_ESC);
		return;
	}
	s = init_str();
	add_to_str(&s, &l, _(TEXT_(T_URL), term));
	add_to_str(&s, &l, cast_uchar ": ");
	if (strchr(cast_const_char lo->url, POST_CHAR)) add_bytes_to_str(&s, &l, lo->url, (unsigned char *)strchr(cast_const_char lo->url, POST_CHAR) - lo->url);
	else add_to_str(&s, &l, lo->url);
	if (!find_in_cache(lo->url, &ce)) {
		unsigned char *start, *end;
		add_to_str(&s, &l, cast_uchar "\n");
		add_to_str(&s, &l, _(TEXT_(T_SIZE), term));
		add_to_str(&s, &l, cast_uchar ": ");
		get_file_by_term(NULL, ce, &start, &end, NULL);
		if (ce->decompressed) {
			unsigned char *enc;
			add_num_to_str(&s, &l, end - start);
			enc = get_content_encoding(ce->head, ce->url);
			if (enc) {
				add_to_str(&s, &l, cast_uchar " (");
				add_num_to_str(&s, &l, ce->length);
				add_to_str(&s, &l, cast_uchar " ");
				add_to_str(&s, &l, _(TEXT_(T_COMPRESSED_WITH), term));
				add_to_str(&s, &l, cast_uchar " ");
				add_to_str(&s, &l, enc);
				add_to_str(&s, &l, cast_uchar ")");
				mem_free(enc);
			}
		} else {
			add_num_to_str(&s, &l, ce->length);
		}
		if (ce->incomplete) {
			add_to_str(&s, &l, cast_uchar " (");
			add_to_str(&s, &l, _(TEXT_(T_INCOMPLETE), term));
			add_to_str(&s, &l, cast_uchar ")");
		}
		add_to_str(&s, &l, cast_uchar "\n");
		add_to_str(&s, &l, _(TEXT_(T_CODEPAGE), term));
		add_to_str(&s, &l, cast_uchar ": ");
		add_to_str(&s, &l, get_cp_name(frame->f_data->cp));
		if (frame->f_data->ass == 1) add_to_str(&s, &l, cast_uchar " ("), add_to_str(&s, &l, _(TEXT_(T_ASSUMED), term)), add_to_str(&s, &l, cast_uchar ")");
		if (frame->f_data->ass == 2) add_to_str(&s, &l, cast_uchar " ("), add_to_str(&s, &l, _(TEXT_(T_IGNORING_SERVER_SETTING), term)), add_to_str(&s, &l, cast_uchar ")");
		if (ce->head && ce->head[0] != '\n' && ce->head[0] != '\r' && (a = parse_http_header(ce->head, cast_uchar "Content-Type", NULL))) {
			add_to_str(&s, &l, cast_uchar "\n");
			add_to_str(&s, &l, _(TEXT_(T_CONTENT_TYPE), term));
			add_to_str(&s, &l, cast_uchar ": ");
			add_to_str(&s, &l, a);
			mem_free(a);
		}
		if ((a = parse_http_header(ce->head, cast_uchar "Server", NULL))) {
			add_to_str(&s, &l, cast_uchar "\n");
			add_to_str(&s, &l, _(TEXT_(T_SERVER), term));
			add_to_str(&s, &l, cast_uchar ": ");
			add_to_str(&s, &l, a);
			mem_free(a);
		}
		if ((a = parse_http_header(ce->head, cast_uchar "Date", NULL))) {
			add_to_str(&s, &l, cast_uchar "\n");
			add_to_str(&s, &l, _(TEXT_(T_DATE), term));
			add_to_str(&s, &l, cast_uchar ": ");
			add_to_str(&s, &l, a);
			mem_free(a);
		}
		if ((a = parse_http_header(ce->head, cast_uchar "Last-Modified", NULL))) {
			add_to_str(&s, &l, cast_uchar "\n");
			add_to_str(&s, &l, _(TEXT_(T_LAST_MODIFIED), term));
			add_to_str(&s, &l, cast_uchar ": ");
			add_to_str(&s, &l, a);
			mem_free(a);
		}
#ifdef HAVE_SSL
		if (ce->ssl_info) {
			add_to_str(&s, &l, cast_uchar "\n");
			add_to_str(&s, &l, _(TEXT_(T_SSL_CIPHER), term));
			add_to_str(&s, &l, cast_uchar ": ");
			add_to_str(&s, &l, ce->ssl_info);
		}
#endif
		ce->refcount--;
	}
	if ((a = print_current_linkx_plus(frame, term))) {
		add_to_str(&s, &l, cast_uchar "\n\n");
		if (*a != '\n') {
			add_to_str(&s, &l, _(TEXT_(T_LINK), term));
			add_to_str(&s, &l, cast_uchar ": ");
			add_to_str(&s, &l, a);
		} else {
			add_to_str(&s, &l, a + 1);
		}
		mem_free(a);
	}
	msg_box(term, getml(s, NULL), TEXT_(T_INFO), AL_LEFT, s, NULL, 1, TEXT_(T_OK), NULL, B_ENTER | B_ESC);
}

void state_msg(struct session *ses)
{
	if (list_empty(ses->history)) loc_msg(ses->term, NULL, NULL);
	else loc_msg(ses->term, cur_loc(ses), current_frame(ses));
}

void head_msg(struct session *ses)
{
	struct cache_entry *ce;
	unsigned char *s, *ss;
	int len;
	if (list_empty(ses->history)) {
		msg_box(ses->term, NULL, TEXT_(T_HEADER_INFO), AL_LEFT, TEXT_(T_YOU_ARE_NOWHERE), NULL, 1, TEXT_(T_OK), NULL, B_ENTER | B_ESC);
		return;
	}
	if (!find_in_cache(cur_loc(ses)->url, &ce)) {
		if (ce->head) s = stracpy(ce->head);
		else s = stracpy(cast_uchar "");
		len = (int)strlen(cast_const_char s) - 1;
		if (len > 0) {
			while ((ss = cast_uchar strstr(cast_const_char s, "\r\n"))) memmove(ss, ss + 1, strlen(cast_const_char ss));
			while (*s && s[strlen(cast_const_char s) - 1] == '\n') s[strlen(cast_const_char s) - 1] = 0;
		}
		if (*s && *s != '\n') {
			msg_box(ses->term, getml(s, NULL), TEXT_(T_HEADER_INFO), AL_LEFT, s, NULL, 1, TEXT_(T_OK), NULL, B_ENTER | B_ESC);
		} else {
			msg_box(ses->term, getml(s, NULL), TEXT_(T_HEADER_INFO), AL_CENTER, TEXT_(T_NO_HEADER), NULL, 1, TEXT_(T_OK), NULL, B_ENTER | B_ESC);
		}
		ce->refcount--;
	}
}
