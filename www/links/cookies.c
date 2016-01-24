/* cookies.c
 * Cookies
 * (c) 2002 Mikulas Patocka
 * This file is a part of the Links program, released under GPL
 */

#include "links.h"

#define ACCEPT_NONE	0
#define ACCEPT_ALL	1

static int accept_cookies = ACCEPT_ALL;

struct list_head all_cookies = { &all_cookies, &all_cookies };

struct list_head c_domains = { &c_domains, &c_domains };

struct c_server {
	struct c_server *next;
	struct c_server *prev;
	int accpt;
	unsigned char server[1];
};

static struct list_head c_servers = { &c_servers, &c_servers };

void free_cookie(struct cookie *c)
{
	mem_free(c->name);
	if (c->value) mem_free(c->value);
	if (c->server) mem_free(c->server);
	if (c->path) mem_free(c->path);
	if (c->domain) mem_free(c->domain);
}

static int check_domain_security(unsigned char *server, unsigned char *domain)
{
	size_t i, j, dl;
	int nd;
	char *tld[] = {
		".com",
		".edu",
		".net",
		".org",
		".gov",
		".mil",
		".int",
		NULL };
	if (domain[0] == '.') domain++;
	dl = strlen(cast_const_char domain);
	if (dl > strlen(cast_const_char server)) return 1;
	for (i = strlen(cast_const_char server) - dl, j = 0; server[i]; i++, j++)
		if (upcase(server[i]) != upcase(domain[j])) return 1;
	nd = 2;
	for (i = 0; tld[i]; i++) {
		size_t tl = strlen(cast_const_char tld[i]);
		if (dl > tl && !casecmp(cast_uchar tld[i], &domain[dl - tl], tl)) {
			nd = 1;
			break;
		}
	}
	if (nd == 2) {
		unsigned char *last_dot = cast_uchar strrchr(cast_const_char domain, '.');
		i = 0;
		if (last_dot) {
			while (last_dot > domain) {
				last_dot--;
				if (*last_dot == '.') break;
				i++;
			}
		}
		if (i >= 4) nd = 1;
	}
	for (i = 0; domain[i]; i++) if (domain[i] == '.') if (!--nd) break;
	if (nd > 0) return 1;
	return 0;
}

static void accept_cookie(struct cookie *);

/* sezere 1 cookie z retezce str, na zacatku nesmi byt zadne whitechars
 * na konci muze byt strednik nebo 0
 * cookie musi byt ve tvaru nazev=hodnota, kolem rovnase nesmi byt zadne mezery
 * (respektive mezery se budou pocitat do nazvu a do hodnoty)
 */
int set_cookie(struct terminal *term, unsigned char *url, unsigned char *str)
{
	int noval = 0;
	struct cookie *cookie;
	struct c_server *cs;
	unsigned char *p, *q, *s, *server, *date;
	if (accept_cookies == ACCEPT_NONE) return 0;
	for (p = str; *p != ';' && *p; p++) { /*if (WHITECHAR(*p)) return 0;*/ }
	for (q = str; *q != '='; q++) if (!*q || q >= p) {
		noval = 1;
		break;
	}
	if (str == q || q + 1 == p) return 0;
	cookie = mem_alloc(sizeof(struct cookie));
	server = get_host_name(url);
	cookie->name = memacpy(str, q - str);
	cookie->value = !noval ? memacpy(q + 1, p - q - 1) : NULL;
	cookie->server = stracpy(server);
	date = parse_header_param(str, cast_uchar "expires", 0);
	if (date) {
		cookie->expires = parse_http_date(date);
		/* kdo tohle napsal a proc ?? */
		/*if (! cookie->expires) cookie->expires++;*/ /* no harm and we can use zero then */
		mem_free(date);
	} else
		cookie->expires = 0;
	if (!(cookie->path = parse_header_param(str, cast_uchar "path", 0))) {
		/*unsigned char *w;*/
		cookie->path = stracpy(cast_uchar "/");
		/*
		add_to_strn(&cookie->path, document);
		for (w = cookie->path; *w; w++) if (end_of_dir(cookie->path, *w)) {
			*w = 0;
			break;
		}
		for (w = cookie->path + strlen(cast_const_char cookie->path) - 1; w >= cookie->path; w--)
			if (*w == '/') {
				w[1] = 0;
				break;
			}
		*/
	} else {
		if (cookie->path[0] != '/') {
			add_to_strn(&cookie->path, cast_uchar "x");
			memmove(cookie->path + 1, cookie->path, strlen(cast_const_char cookie->path) - 1);
			cookie->path[0] = '/';
		}
	}
	if (!(cookie->domain = parse_header_param(str, cast_uchar "domain", 0))) cookie->domain = stracpy(server);
	if (cookie->domain[0] == '.') memmove(cookie->domain, cookie->domain + 1, strlen(cast_const_char cookie->domain));
	if ((s = parse_header_param(str, cast_uchar "secure", 0))) {
		cookie->secure = 1;
		mem_free(s);
	} else cookie->secure = 0;
	if (check_domain_security(server, cookie->domain)) {
		mem_free(cookie->domain);
		cookie->domain = stracpy(server);
	}
	foreach (cs, c_servers) if (!strcasecmp(cast_const_char cs->server, cast_const_char server)) {
		if (cs->accpt) goto ok;
		else {
			free_cookie(cookie);
			mem_free(cookie);
			mem_free(server);
			return 0;
		}
	}
	if (accept_cookies != ACCEPT_ALL) {
		free_cookie(cookie);
		mem_free(cookie);
		mem_free(server);
		return 1;
	}
	ok:
	accept_cookie(cookie);
	mem_free(server);
	return 0;
}

static void accept_cookie(struct cookie *c)
{
	struct c_domain *cd;
	struct cookie *d, *e;
	foreach(d, all_cookies) if (!strcasecmp(cast_const_char d->name, cast_const_char c->name) && !strcasecmp(cast_const_char d->domain, cast_const_char c->domain)) {
		e = d;
		d = d->prev;
		del_from_list(e);
		free_cookie(e);
		mem_free(e);
	}
	if (c->value && !strcasecmp(cast_const_char c->value, "deleted")) {
		free_cookie(c);
		mem_free(c);
		return;
	}
	add_to_list(all_cookies, c);
	foreach(cd, c_domains) if (!strcasecmp(cast_const_char cd->domain, cast_const_char c->domain)) return;
	cd = mem_alloc(sizeof(struct c_domain) + strlen(cast_const_char c->domain) + 1);
	strcpy(cast_char cd->domain, cast_const_char c->domain);
	add_to_list(c_domains, cd);
}

int is_in_domain(unsigned char *d, unsigned char *s)
{
	int dl = (int)strlen(cast_const_char d);
	int sl = (int)strlen(cast_const_char s);
	if (dl > sl) return 0;
	if (dl == sl) return !strcasecmp(cast_const_char d, cast_const_char s);
	if (s[sl - dl - 1] != '.') return 0;
	return !casecmp(d, s + sl - dl, dl);
}

int is_path_prefix(unsigned char *d, unsigned char *s)
{
	int dl = (int)strlen(cast_const_char d);
	int sl = (int)strlen(cast_const_char s);
	if (!dl) return 1;
	if (dl > sl) return 0;
	if (memcmp(d, s, dl)) return 0;
	return d[dl - 1] == '/' || !s[dl] || s[dl] == '/' || s[dl] == POST_CHAR || s[dl] == '?' || s[dl] == '&';
}

int cookie_expired(struct cookie *c)	/* parse_http_date is broken */
{
	time_t t;
	errno = 0;
	EINTRLOOPX(t, time(NULL), (time_t)-1);
	return 0 && (c->expires && c->expires < t);
}

void add_cookies(unsigned char **s, int *l, unsigned char *url)
{
	int nc = 0;
	struct c_domain *cd;
	struct cookie *c, *d;
	unsigned char *server = get_host_name(url);
	unsigned char *data = get_url_data(url);
	if (data > url) data--;
	foreach (cd, c_domains) if (is_in_domain(cd->domain, server)) goto ok;
	mem_free(server);
	return;
	ok:
	foreachback (c, all_cookies) if (is_in_domain(c->domain, server)) if (is_path_prefix(c->path, data)) {
		if (cookie_expired(c)) {
			d = c;
			c = c->prev;
			del_from_list(d);
			free_cookie(d);
			mem_free(d);
			continue;
		}
		if (c->secure && casecmp(url, cast_uchar "https://", 8)) continue;
		if (!nc) add_to_str(s, l, cast_uchar "Cookie: "), nc = 1;
		else add_to_str(s, l, cast_uchar "; ");
		add_to_str(s, l, c->name);
		if (c->value) {
			add_to_str(s, l, cast_uchar "=");
			add_to_str(s, l, c->value);
		}
	}
	if (nc) add_to_str(s, l, cast_uchar "\r\n");
	mem_free(server);
}

void free_cookies(void)
{
	struct cookie *c;
	free_list(c_domains);
	/* !!! FIXME: save cookies */
	foreach (c, all_cookies) free_cookie(c);
	free_list(all_cookies);
}

void init_cookies(void)
{
	/* !!! FIXME: read cookies */
}

void cleanup_cookies(void)
{
	free_cookies();
}
