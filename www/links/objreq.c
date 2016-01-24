/* objreq.c
 * Object Requester
 * (c) 2002 Mikulas Patocka
 * This file is a part of the Links program, released under GPL.
 */

#include "links.h"

static void objreq_end(struct status *, struct object_request *);
static void object_timer(struct object_request *);


static struct list_head requests = {&requests, &requests};
static tcount obj_req_count = 1;

#define LL gf_val(1, G_BFU_FONT_SIZE)

#define MAX_UID_LEN 256

struct auth_dialog {
	unsigned char uid[MAX_UID_LEN];
	unsigned char passwd[MAX_UID_LEN];
	unsigned char *realm;
	int proxy;
	tcount count;
	unsigned char msg[1];
};

static inline struct object_request *find_rq(tcount c)
{
	struct object_request *rq;
	foreach(rq, requests) if (rq->count == c) return rq;
	return NULL;
}

static void auth_fn(struct dialog_data *dlg)
{
	struct terminal *term = dlg->win->term;
	struct auth_dialog *a = dlg->dlg->udata;
	int max = 0, min = 0;
	int w, rw;
	int y = 0;
	max_text_width(term, a->msg, &max, AL_LEFT);
	min_text_width(term, a->msg, &min, AL_LEFT);
	max_text_width(term, TEXT_(T_USERID), &max, AL_LEFT);
	min_text_width(term, TEXT_(T_USERID), &min, AL_LEFT);
	max_text_width(term, TEXT_(T_PASSWORD), &max, AL_LEFT);
	min_text_width(term, TEXT_(T_PASSWORD), &min, AL_LEFT);
	max_buttons_width(term, dlg->items + 2, 2, &max);
	min_buttons_width(term, dlg->items + 2, 2, &min);
	w = term->x * 9 / 10 - 2 * DIALOG_LB;
	if (w > max) w = max;
	if (w < min) w = min;
	rw = w;
	dlg_format_text(dlg, NULL, a->msg, 0, &y, w, &rw, COLOR_DIALOG_TEXT, AL_LEFT);
	y += LL;
	dlg_format_text_and_field(dlg, NULL, TEXT_(T_USERID), dlg->items, 0, &y, w, &rw, COLOR_DIALOG_TEXT, AL_LEFT);
	y += LL;
	dlg_format_text_and_field(dlg, NULL, TEXT_(T_PASSWORD), dlg->items + 1, 0, &y, w, &rw, COLOR_DIALOG_TEXT, AL_LEFT);
	y += LL;
	dlg_format_buttons(dlg, NULL, dlg->items + 2, 2, 0, &y, w, &rw, AL_CENTER);
	w = rw;
	dlg->xw = rw + 2 * DIALOG_LB;
	dlg->yw = y + 2 * DIALOG_TB;
	center_dlg(dlg);
	draw_dlg(dlg);
	y = dlg->y + DIALOG_TB;
	y += LL;
	dlg_format_text(dlg, term, a->msg, dlg->x + DIALOG_LB, &y, w, NULL, COLOR_DIALOG_TEXT, AL_LEFT);
	y += LL;
	dlg_format_text_and_field(dlg, term, TEXT_(T_USERID), dlg->items, dlg->x + DIALOG_LB, &y, w, NULL, COLOR_DIALOG_TEXT, AL_LEFT);
	y += LL;
	dlg_format_text_and_field(dlg, term, TEXT_(T_PASSWORD), dlg->items + 1, dlg->x + DIALOG_LB, &y, w, NULL, COLOR_DIALOG_TEXT, AL_LEFT);
	y += LL;
	dlg_format_buttons(dlg, term, dlg->items + 2, 2, dlg->x + DIALOG_LB, &y, w, NULL, AL_CENTER);
}

static int auth_cancel(struct dialog_data *dlg, struct dialog_item_data *item)
{
	struct auth_dialog *a = dlg->dlg->udata;
	struct object_request *rq = find_rq(a->count);
	if (rq) {
		rq->hold = 0;
		rq->state = O_OK;
		if (rq->timer != NULL) kill_timer(rq->timer);
		rq->timer = install_timer(0, (void (*)(void *))object_timer, rq);
		if (!rq->ce) (rq->ce = rq->ce_internal)->refcount++;
	}
	cancel_dialog(dlg, item);
	return 0;
}

static int auth_ok(struct dialog_data *dlg, struct dialog_item_data *item)
{
	struct auth_dialog *a = dlg->dlg->udata;
	struct object_request *rq = find_rq(a->count);
	if (rq) {
		struct session *ses;
		struct conv_table *ct;
		int net_cp;
		unsigned char *uid, *passwd;
		get_dialog_data(dlg);
		ses = ((struct window *)dlg->win->term->windows.prev)->data;
		get_convert_table(rq->ce_internal->head, term_charset(dlg->win->term), ses->ds.assume_cp, &net_cp, NULL, ses->ds.hard_assume);
		ct = get_translation_table(term_charset(dlg->win->term), net_cp);
		uid = convert_string(ct, a->uid, (int)strlen(cast_const_char a->uid), NULL);
		passwd = convert_string(ct, a->passwd, (int)strlen(cast_const_char a->passwd), NULL);
		add_auth(rq->url, a->realm, uid, passwd, a->proxy);
		mem_free(uid);
		mem_free(passwd);
		rq->hold = 0;
		change_connection(&rq->stat, NULL, PRI_CANCEL);
		load_url(rq->url, rq->prev_url, &rq->stat, rq->pri, NC_RELOAD, 0, 0, 0);
	}
	cancel_dialog(dlg, item);
	return 0;
}

static int auth_window(struct object_request *rq, unsigned char *realm)
{
	unsigned char *host, *port;
	struct dialog *d;
	struct auth_dialog *a;
	struct terminal *term;
	struct conv_table *ct;
	unsigned char *urealm;
	struct session *ses;
	foreach(term, terminals) if (rq->term == term->count) goto ok;
	return -1;
	ok:
	ses = ((struct window *)term->windows.prev)->data;
	ct = get_convert_table(rq->ce_internal->head, term_charset(term), ses->ds.assume_cp, NULL, NULL, ses->ds.hard_assume);
	if (rq->ce_internal->http_code == 407) {
		host = get_proxy_string(rq->url);
		if (!host) host = cast_uchar "";
		host = stracpy(host);
	} else {
		host = get_host_name(rq->url);
		if (!host) return -1;
		if ((port = get_port_str(rq->url))) {
			add_to_strn(&host, cast_uchar ":");
			add_to_strn(&host, port);
			mem_free(port);
		}
	}
	urealm = convert_string(ct, realm, (int)strlen(cast_const_char realm), NULL);
	d = mem_alloc(sizeof(struct dialog) + 5 * sizeof(struct dialog_item) + sizeof(struct auth_dialog) + strlen(cast_const_char _(TEXT_(T_ENTER_USERNAME), term)) + strlen(cast_const_char urealm) + 1 + strlen(cast_const_char _(TEXT_(T_AT), term)) + strlen(cast_const_char host));
	memset(d, 0, sizeof(struct dialog) + 5 * sizeof(struct dialog_item) + sizeof(struct auth_dialog));
	a = (struct auth_dialog *)((unsigned char *)d + sizeof(struct dialog) + 5 * sizeof(struct dialog_item));
	strcpy(cast_char a->msg, cast_const_char _(TEXT_(T_ENTER_USERNAME), term));
	strcat(cast_char a->msg, cast_const_char urealm);
	if (*host) {
		strcat(cast_char a->msg, "\n");
		strcat(cast_char a->msg, cast_const_char _(TEXT_(T_AT), term));
		strcat(cast_char a->msg, cast_const_char host);
	}
	mem_free(host);
	mem_free(urealm);
	a->proxy = rq->ce_internal->http_code == 407;
	a->realm = stracpy(realm);
	a->count = rq->count;
	d->udata = a;
	if (rq->ce_internal->http_code == 401) d->title = TEXT_(T_AUTHORIZATION_REQUIRED);
	else d->title = TEXT_(T_PROXY_AUTHORIZATION_REQUIRED);
	d->fn = auth_fn;
	d->items[0].type = D_FIELD;
	d->items[0].dlen = MAX_UID_LEN;
	d->items[0].data = a->uid;

	d->items[1].type = D_FIELD_PASS;
	d->items[1].dlen = MAX_UID_LEN;
	d->items[1].data = a->passwd;

	d->items[2].type = D_BUTTON;
	d->items[2].gid = B_ENTER;
	d->items[2].fn = auth_ok;
	d->items[2].text = TEXT_(T_OK);

	d->items[3].type = D_BUTTON;
	d->items[3].gid = B_ESC;
	d->items[3].fn = auth_cancel;
	d->items[3].text = TEXT_(T_CANCEL);

	do_dialog(term, d, getml(d, a->realm, NULL));
	return 0;
}

#ifdef HAVE_SSL_CERTIFICATES

struct cert_dialog {
	tcount count;
	unsigned char *host;
};

static void cert_yes(void *data)
{
	struct cert_dialog *cs = data;
	struct object_request *rq = find_rq(cs->count);
	rq->hold = 0;
	if (rq->stat.state == S_INVALID_CERTIFICATE)
		add_blacklist_entry(cs->host, BL_IGNORE_CERTIFICATE);
	else
		add_blacklist_entry(cs->host, BL_IGNORE_CIPHER);
	change_connection(&rq->stat, NULL, PRI_CANCEL);
	load_url(rq->url, rq->prev_url, &rq->stat, rq->pri, NC_CACHE, 0, 0, 0);
}

static void cert_no(void *data)
{
	struct cert_dialog *cs = data;
	struct object_request *rq = find_rq(cs->count);
	rq->hold = 0;
	rq->dont_print_error = 1;
	rq->state = O_FAILED;
	if (rq->timer != NULL) kill_timer(rq->timer);
	rq->timer = install_timer(0, (void (*)(void *))object_timer, rq);
}

static int cert_window(struct object_request *rq)
{
	struct terminal *term;
	unsigned char *host;
	struct cert_dialog *cs;
	struct memory_list *ml;
	foreach(term, terminals) if (rq->term == term->count) goto ok;
	return -1;
	ok:
	host = get_host_name(rq->url);
	cs = mem_alloc(sizeof(struct cert_dialog));
	cs->count = rq->count;
	cs->host = host;
	ml = getml(cs, host, NULL);
	msg_box(term, ml, rq->stat.state == S_INVALID_CERTIFICATE ? TEXT_(T_INVALID_CERTIFICATE) : TEXT_(T_INSECURE_CIPHER), AL_CENTER | AL_EXTD_TEXT, TEXT_(T_THE_SERVER_), host, rq->stat.state == S_INVALID_CERTIFICATE ? TEXT_(T_DOESNT_HAVE_A_VALID_CERTIFICATE) : TEXT_(T_USES_INSECURE_CIPHER), NULL, cs, 2, TEXT_(T_NO), cert_no, B_ESC, TEXT_(T_YES), cert_yes, B_ENTER);
	return 0;
}

#endif

/* prev_url is a pointer to previous url or NULL */
/* prev_url will NOT be deallocated */
void request_object(struct terminal *term, unsigned char *url, unsigned char *prev_url, int pri, int cache, int allow_flags, void (*upcall)(struct object_request *, void *), void *data, struct object_request **rqp)
{
	struct object_request *rq;
	rq = mem_calloc(sizeof(struct object_request));
	rq->state = O_WAITING;
	rq->refcount = 1;
	rq->term = term ? term->count : 0;
	rq->stat.end = (void (*)(struct status *, void *))objreq_end;
	rq->stat.data = rq;
	rq->orig_url = stracpy(url);
	rq->url = stracpy(url);
	rq->pri = pri;
	rq->cache = cache;
	rq->upcall = upcall;
	rq->data = data;
	rq->timer = NULL;
	rq->z = (uttime)get_time() - STAT_UPDATE_MAX;
	rq->last_update = rq->z;
	if (rq->prev_url) mem_free(rq->prev_url);
	rq->prev_url = stracpy(prev_url);
	if (rqp) *rqp = rq;
	rq->count = obj_req_count++;
	add_to_list(requests, rq);
	load_url(url, prev_url, &rq->stat, pri, cache, 0, allow_flags, 0);
}

static void set_ce_internal(struct object_request *rq)
{
	if (rq->stat.ce != rq->ce_internal) {
		if (!rq->stat.ce) {
			rq->ce_internal->refcount--;
			rq->ce_internal = NULL;
		} else {
			if (rq->ce_internal)
				rq->ce_internal->refcount--;
			rq->ce_internal = rq->stat.ce;
			rq->ce_internal->refcount++;
		}
	}
}

static void objreq_end(struct status *stat, struct object_request *rq)
{
	set_ce_internal(rq);

	if (stat->state < 0) {
#ifdef HAVE_SSL_CERTIFICATES
		if (!stat->ce && rq->state == O_WAITING && (stat->state == S_INVALID_CERTIFICATE || stat->state == S_INSECURE_CIPHER) && ssl_options.certificates == SSL_WARN_ON_INVALID_CERTIFICATE) {
			if (!cert_window(rq)) {
				rq->hold = 1;
				rq->redirect_cnt = 0;
				goto tm;
			}
		}
#endif
		if (stat->ce && rq->state == O_WAITING && stat->ce->redirect) {
			if (rq->redirect_cnt++ < MAX_REDIRECTS) {
				int cache, allow_flags;
				unsigned char *u, *p, *pos;
				change_connection(stat, NULL, PRI_CANCEL);
				u = join_urls(rq->url, stat->ce->redirect);
				u = translate_hashbang(u);
				if ((pos = extract_position(u))) {
					if (rq->goto_position) mem_free(rq->goto_position);
					rq->goto_position = pos;
				}
				if (!http_options.bug_302_redirect && !stat->ce->redirect_get && (p = cast_uchar strchr(cast_const_char u, POST_CHAR))) add_to_strn(&u, p);
				cache = rq->cache;
				if (cache < NC_RELOAD && (!strcmp(cast_const_char u, cast_const_char rq->url) || !strcmp(cast_const_char u, cast_const_char rq->orig_url) || rq->redirect_cnt >= MAX_CACHED_REDIRECTS)) cache = NC_RELOAD;
				allow_flags = get_allow_flags(rq->url);
				mem_free(rq->url);
				rq->url = u;
				load_url(u, rq->prev_url, &rq->stat, rq->pri, cache, 0, allow_flags, 0);
				return;
			} else {
				maxrd:
				rq->stat.state = S_CYCLIC_REDIRECT;
			}
		}
		if (stat->ce && rq->state == O_WAITING && (stat->ce->http_code == 401 || stat->ce->http_code == 407)) {
			unsigned char *realm = get_auth_realm(rq->url, stat->ce->head, stat->ce->http_code == 407);
			unsigned char *user;
			if (!realm) goto xx;
			if (stat->ce->http_code == 401 && !find_auth(rq->url, realm)) {
				mem_free(realm);
				if (rq->redirect_cnt++ >= MAX_REDIRECTS) goto maxrd;
				change_connection(stat, NULL, PRI_CANCEL);
				load_url(rq->url, rq->prev_url, &rq->stat, rq->pri, NC_RELOAD, 0, 0, 0);
				return;
			}
			user = get_user_name(rq->url);
			if (stat->ce->http_code == 401 && user && *user) {
				mem_free(user);
				mem_free(realm);
				goto xx;
			}
			mem_free(user);
			if (!auth_window(rq, realm)) {
				rq->hold = 1;
				rq->redirect_cnt = 0;
				mem_free(realm);
				goto tm;
			}
			mem_free(realm);
			goto xx;
		}
	}
	if ((stat->state < 0 || stat->state == S_TRANS) && stat->ce && !stat->ce->redirect && stat->ce->http_code != 401 && stat->ce->http_code != 407) {
		rq->state = O_LOADING;
		if (0) {
			xx:
			rq->state = O_OK;
		}
		if (!rq->ce) (rq->ce = stat->ce)->refcount++;
	}
	tm:
	if (rq->timer != NULL) kill_timer(rq->timer);
	rq->timer = install_timer(0, (void (*)(void *))object_timer, rq);
}

static void object_timer(struct object_request *rq)
{
	off_t last;

	set_ce_internal(rq);

	last = rq->last_bytes;
	if (rq->ce) rq->last_bytes = rq->ce->length;
	rq->timer = NULL;
	if (rq->stat.state < 0 && !rq->hold && (!rq->ce_internal || !rq->ce_internal->redirect || rq->stat.state == S_CYCLIC_REDIRECT)) {
		if (rq->ce_internal && rq->stat.state != S_CYCLIC_REDIRECT) {
			rq->state = rq->stat.state != S__OK ? O_INCOMPLETE : O_OK;
		} else rq->state = O_FAILED;
	}
	if (rq->stat.state != S_TRANS) {
		if (rq->stat.state >= 0)
			rq->timer = install_timer(STAT_UPDATE_MAX, (void (*)(void *))object_timer, rq);
		rq->last_update = rq->z;
		if (rq->upcall) rq->upcall(rq, rq->data);
	} else {
		ttime ct = get_time();
		ttime t = (uttime)ct - (uttime)rq->last_update;
		rq->timer = install_timer(STAT_UPDATE_MIN, (void (*)(void *))object_timer, rq);
		if (t >= STAT_UPDATE_MAX || (t >= STAT_UPDATE_MIN && rq->ce && rq->last_bytes > last)) {
			rq->last_update = ct;
			if (rq->upcall) rq->upcall(rq, rq->data);
		}
	}
}

void release_object_get_stat(struct object_request **rqq, struct status *news, int pri)
{
	struct object_request *rq = *rqq;
	if (!rq) return;
	*rqq = NULL;
	if (--rq->refcount) return;
	change_connection(&rq->stat, news, pri);
	if (rq->timer != NULL) kill_timer(rq->timer);
	if (rq->ce_internal) rq->ce_internal->refcount--;
	if (rq->ce) rq->ce->refcount--;
	mem_free(rq->orig_url);
	mem_free(rq->url);
	if (rq->prev_url) mem_free(rq->prev_url);
	if (rq->goto_position) mem_free(rq->goto_position);
	del_from_list(rq);
	mem_free(rq);
}

void release_object(struct object_request **rqq)
{
	release_object_get_stat(rqq, NULL, PRI_CANCEL);
}

void detach_object_connection(struct object_request *rq, off_t pos)
{
	if (rq->state == O_WAITING || rq->state == O_FAILED) {
		internal("detach_object_connection: no data received");
		return;
	}
	if (rq->refcount == 1) detach_connection(&rq->stat, pos);
}

void clone_object(struct object_request *rq, struct object_request **rqq)
{
	(*rqq = rq)->refcount++;
}
