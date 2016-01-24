/* url.c
 * (c) 2002 Mikulas Patocka
 * This file is a part of the Links program, released under GPL.
 */

#include "links.h"

static const struct {
	char *prot;
	int port;
	void (*func)(struct connection *);
	void (*nc_func)(struct session *, unsigned char *);
	int free_syntax;
	int need_slashes;
	int need_slash_after_host;
	int allow_post;
	int bypasses_socks;
} protocols[]= {
		{"data", 0, data_func, NULL,		1, 0, 0, 0, 0},
		{"file", 0, file_func, NULL,		1, 1, 0, 0, 1},
		{"https", 443, https_func, NULL,	0, 1, 1, 1, 0},
		{"http", 80, http_func, NULL,		0, 1, 1, 1, 0},
		{"proxy", 3128, proxy_func, NULL,	0, 1, 1, 1, 0},
		{"ftp", 21, ftp_func, NULL,		0, 1, 1, 0, 0},
		{"finger", 79, finger_func, NULL,	0, 1, 1, 0, 0},
#ifndef DISABLE_SMB
		{"smb", 139, smb_func, NULL,		0, 1, 1, 0, 1},
#endif
		{"mailto", 0, NULL, mailto_func,	0, 0, 0, 0, 0},
		{"telnet", 0, NULL, telnet_func,	0, 0, 0, 0, 1},
		{"tn3270", 0, NULL, tn3270_func,	0, 0, 0, 0, 1},
		{"mms", 0, NULL, mms_func,		1, 0, 1, 0, 1},
		{"magnet", 0, NULL, magnet_func,	1, 0, 0, 0, 1},
#ifdef JS
		{"javascript", 0, NULL, javascript_func,1, 0, 0, 0, 0},
#endif
		{NULL, 0, NULL, NULL,			0, 0, 0, 0, 0}
};



static int check_protocol(unsigned char *p, int l)
{
	int i;
	for (i = 0; protocols[i].prot; i++)
		if (!casecmp(cast_uchar protocols[i].prot, p, l) && strlen(cast_const_char protocols[i].prot) == (size_t)l) {
			return i;
		}
	return -1;
}

static int get_prot_info(unsigned char *prot, int *port, void (**func)(struct connection *), void (**nc_func)(struct session *ses, unsigned char *), int *allow_post, int *bypasses_socks)
{
	int i;
	for (i = 0; protocols[i].prot; i++)
		if (!strcasecmp(cast_const_char protocols[i].prot, cast_const_char prot)) {
			if (port) *port = protocols[i].port;
			if (func) *func = protocols[i].func;
			if (nc_func) *nc_func = protocols[i].nc_func;
			if (allow_post) *allow_post = protocols[i].allow_post;
			if (bypasses_socks) *bypasses_socks = protocols[i].bypasses_socks;
			return 0;
		}
	return -1;
}

int parse_url(unsigned char *url, int *prlen, unsigned char **user, int *uslen, unsigned char **pass, int *palen, unsigned char **host, int *holen, unsigned char **port, int *polen, unsigned char **data, int *dalen, unsigned char **post)
{
	unsigned char *p, *q;
	unsigned char p_c[2];
	int a;
	if (prlen) *prlen = 0;
	if (user) *user = NULL;
	if (uslen) *uslen = 0;
	if (pass) *pass = NULL;
	if (palen) *palen = 0;
	if (host) *host = NULL;
	if (holen) *holen = 0;
	if (port) *port = NULL;
	if (polen) *polen = 0;
	if (data) *data = NULL;
	if (dalen) *dalen = 0;
	if (post) *post = NULL;
	if (!url || !(p = cast_uchar strchr(cast_const_char url, ':'))) return -1;
	if (prlen) *prlen = (int)(p - url);
	if ((a = check_protocol(url, (int)(p - url))) == -1) return -1;
	if (p[1] != '/' || p[2] != '/') {
		if (protocols[a].need_slashes) return -1;
		p -= 2;
	}
	if (protocols[a].free_syntax) {
		if (data) *data = p + 3;
		if (dalen) *dalen = (int)strlen(cast_const_char(p + 3));
		return 0;
	}
	p += 3;
	q = p + strcspn(cast_const_char p, "@/?");
	if (!*q && protocols[a].need_slash_after_host) return -1;
	if (*q == '@') {
		unsigned char *pp;
		while (strcspn(cast_const_char(q + 1), "@") < strcspn(cast_const_char(q + 1), "/?"))
			q = q + 1 + strcspn(cast_const_char(q + 1), "@");
		pp = cast_uchar strchr(cast_const_char p, ':');
		if (!pp || pp > q) {
			if (user) *user = p;
			if (uslen) *uslen = (int)(q - p);
		} else {
			if (user) *user = p;
			if (uslen) *uslen = (int)(pp - p);
			if (pass) *pass = pp + 1;
			if (palen) *palen = (int)(q - pp - 1);
		}
		p = q + 1;
	}
	if (p[0] == '[') {
		q = cast_uchar strchr(cast_const_char p, ']');
		if (q) {
			q++;
			goto have_host;
		}
	}
	q = p + strcspn(cast_const_char p, ":/?");
	have_host:
	if (!*q && protocols[a].need_slash_after_host) return -1;
	if (host) *host = p;
	if (holen) *holen = (int)(q - p);
	if (*q == ':') {
		unsigned char *pp = q + strcspn(cast_const_char q, "/");
		int cc;
		if (*pp != '/' && protocols[a].need_slash_after_host) return -1;
		if (port) *port = q + 1;
		if (polen) *polen = (int)(pp - q - 1);
		for (cc = 0; cc < pp - q - 1; cc++) if (q[cc+1] < '0' || q[cc+1] > '9') return -1;
		q = pp;
	}
	if (*q && *q != '?') q++;
	p = q;
	p_c[0] = POST_CHAR;
	p_c[1] = 0;
	q = p + strcspn(cast_const_char p, cast_const_char p_c);
	if (data) *data = p;
	if (dalen) *dalen = (int)(q - p);
	if (post) *post = *q ? q + 1 : NULL;
	return 0;
}

unsigned char *get_protocol_name(unsigned char *url)
{
	int l;
	if (parse_url(url, &l, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)) return NULL;
	return memacpy(url, l);
}

unsigned char *get_keepalive_id(unsigned char *url)
{
	unsigned char *h, *p, *k, *d;
	int hl, pl;
	if (parse_url(url, NULL, NULL, NULL, NULL, NULL, &h, &hl, &p, &pl, &d, NULL, NULL)) return NULL;
	if (!casecmp(url, cast_uchar "proxy://", 8) && !casecmp(d, cast_uchar "https://", 8)) {
		if (parse_url(d, NULL, NULL, NULL, NULL, NULL, &h, &hl, &p, &pl, NULL, NULL, NULL)) return NULL;
	}
	k = p ? p + pl : h + hl;
	if (!k) return stracpy(cast_uchar "");
	return memacpy(url, k - url);
}

unsigned char *get_host_name(unsigned char *url)
{
	unsigned char *h;
	int hl;
	if (parse_url(url, NULL, NULL, NULL, NULL, NULL, &h, &hl, NULL, NULL, NULL, NULL, NULL)) return stracpy(cast_uchar "");
	return memacpy(h, hl);
}

unsigned char *get_user_name(unsigned char *url)
{
	unsigned char *h;
	int hl;
	if (parse_url(url, NULL, &h, &hl, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)) return NULL;
	return memacpy(h, hl);
}

unsigned char *get_pass(unsigned char *url)
{
	unsigned char *h;
	int hl;
	if (parse_url(url, NULL,NULL,  NULL, &h, &hl, NULL, NULL, NULL, NULL, NULL, NULL, NULL)) return NULL;
	return memacpy(h, hl);
}

unsigned char *get_port_str(unsigned char *url)
{
	unsigned char *h;
	int hl;
	if (parse_url(url, NULL, NULL, NULL, NULL, NULL, NULL, NULL, &h, &hl, NULL, NULL, NULL)) return NULL;
	return hl ? memacpy(h, hl) : NULL;
}

int get_port(unsigned char *url)
{
	unsigned char *h;
	int hl;
	long n = -1;
	if (parse_url(url, NULL, NULL, NULL, NULL, NULL, NULL, NULL, &h, &hl, NULL, NULL, NULL)) return -1;
	if (h) {
		n = strtol(cast_const_char h, NULL, 10);
		if (n > 0 && n < 65536) return (int)n;
		return -1;
	}
	if ((h = get_protocol_name(url))) {
		int nn = -1;	/* against warning */
		get_prot_info(h, &nn, NULL, NULL, NULL, NULL);
		mem_free(h);
		n = nn;
	}
	return (int)n;
}

void (*get_protocol_handle(unsigned char *url))(struct connection *)
{
	unsigned char *p;
	void (*f)(struct connection *) = NULL;
	int post = 0;
	if (!(p = get_protocol_name(url))) return NULL;
	get_prot_info(p, NULL, &f, NULL, &post, NULL);
	mem_free(p);
	if (!post && strchr(cast_const_char url, POST_CHAR)) return NULL;
	return f;
}

void (*get_external_protocol_function(unsigned char *url))(struct session *, unsigned char *)
{
	unsigned char *p;
	void (*f)(struct session *, unsigned char *) = NULL;
	int post = 0;
	if (!(p = get_protocol_name(url))) return NULL;
	get_prot_info(p, NULL, NULL, &f, &post, NULL);
	mem_free(p);
	if (!post && strchr(cast_const_char url, POST_CHAR)) return NULL;
	return f;
}

int url_bypasses_socks(unsigned char *url)
{
	int ret = 0;	/* against warning */
	unsigned char *p;
	if (!(p = get_protocol_name(url))) return 1;
	get_prot_info(p, NULL, NULL, NULL, NULL, &ret);
	mem_free(p);
	return ret;
}

unsigned char *get_url_data(unsigned char *url)
{
	unsigned char *d;
	if (parse_url(url, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, &d, NULL, NULL)) return NULL;
	return d;
}

#define dsep(x) (lo ? dir_sep(x) : (x) == '/')

static void translate_directories(unsigned char *url)
{
	unsigned char *dd = get_url_data(url);
	unsigned char *s, *d;
	int lo = !casecmp(url, cast_uchar "file://", 7);
	if (!casecmp(url, cast_uchar "javascript:", 11)) return;
	if (!casecmp(url, cast_uchar "magnet:", 7)) return;
	if (!dd || dd == url /*|| *--dd != '/'*/) return;
	if (!dsep(*dd)) {
		dd--;
		if (!dsep(*dd)) {
			dd++;
			memmove(dd + 1, dd, strlen(cast_const_char dd) + 1);
			*dd = '/';
		}
	}
	s = dd;
	d = dd;
	r:
	if (end_of_dir(url, s[0])) {
		memmove(d, s, strlen(cast_const_char s) + 1);
		return;
	}
	if (dsep(s[0]) && s[1] == '.' && dsep(s[2])) {
		/**d++ = s[0];*/
		if (s == dd && !s[3]) goto p;
		s += 2;
		goto r;
	}
	if (dsep(s[0]) && s[1] == '.' && s[2] == '.' && (dsep(s[3]) || !s[3])) {
		while (d > dd) {
			d--;
			if (dsep(*d)) goto b;
		}
		b:
		if (!s[3]) *d++ = *s;
		s += 3;
		goto r;
	}
	p:
	if ((*d++ = *s++)) goto r;
}

unsigned char *translate_hashbang(unsigned char *up)
{
	unsigned char *u, *p, *dp, *data, *post_seq;
	int q;
	unsigned char *r;
	int rl;
	if (!strstr(cast_const_char up, "#!") && !strstr(cast_const_char up, "#%21")) return up;
	u = stracpy(up);
	p = extract_position(u);
	if (!p) {
		free_u_ret_up:
		mem_free(u);
		return up;
	}
	if (p[0] == '!') dp = p + 1;
	else if (!casecmp(p, cast_uchar "%21", 3)) dp = p + 3;
	else {
		mem_free(p);
		goto free_u_ret_up;
	}
	if (!(post_seq = cast_uchar strchr(cast_const_char u, POST_CHAR))) post_seq = cast_uchar strchr(cast_const_char u, 0);
	data = get_url_data(u);
	if (!data) data = u;
	r = init_str();
	rl = 0;
	add_bytes_to_str(&r, &rl, u, post_seq - u);
	q = (int)strlen(cast_const_char data);
	if (q && (data[q - 1] == '&' || data[q - 1] == '?'))
		;
	else if (strchr(cast_const_char data, '?')) add_chr_to_str(&r, &rl, '&');
	else add_chr_to_str(&r, &rl, '?');
	add_to_str(&r, &rl, cast_uchar "_escaped_fragment_=");
	for (; *dp; dp++) {
		unsigned char c = *dp;
		if (c <= 0x20 || c == 0x23 || c == 0x25 || c == 0x26 || c == 0x2b || c >= 0x7f) {
			unsigned char h[4];
			sprintf(cast_char h, "%%%02X", c);
			add_to_str(&r, &rl, h);
		} else {
			add_chr_to_str(&r, &rl, c);
		}
	}
	add_to_str(&r, &rl, post_seq);
	mem_free(u);
	mem_free(p);
	mem_free(up);
	return r;
}

static void insert_wd(unsigned char **up, unsigned char *cwd)
{
	unsigned char *url = *up;
	if (!url || !cwd || !*cwd) return;
	if (casecmp(url, cast_uchar "file://", 7)) return;
	if (dir_sep(url[7])) return;
#ifdef DOS_FS
	if (upcase(url[7]) >= 'A' && upcase(url[7]) <= 'Z' && url[8] == ':' && dir_sep(url[9])) return;
#endif
#ifdef SPAD
	if (_is_absolute(cast_const_char(url + 7)) != _ABS_NO) return;
#endif
	url = mem_alloc(strlen(cast_const_char *up) + strlen(cast_const_char cwd) + 2);
	memcpy(url, *up, 7);
	strcpy(cast_char(url + 7), cast_const_char cwd);
	if (!dir_sep(cwd[strlen(cast_const_char cwd) - 1])) strcat(cast_char url, "/");
	strcat(cast_char url, cast_const_char(*up + 7));
	mem_free(*up);
	*up = url;
}

/*
 * U funkce join_urls musi byt prvni url absolutni (takove, co projde funkci
 * parse_url bez chyby --- pokud neni absolutni, tak to spatne na internal) a
 * druhe url je relativni cesta vuci nemu nebo taky absolutni url. Pokud je
 * druhe url absolutni, vrati se to; pokud je relativni, tak se spoji prvni a
 * druhe url.
 */
unsigned char *join_urls(unsigned char *base, unsigned char *rel)
{
	unsigned char *p, *n, *pp, *ch;
	int l;
	int lo = !casecmp(base, cast_uchar "file://", 7);
	int data = !casecmp(base, cast_uchar "data:", 5);
	if (rel[0] == '#' || !rel[0]) {
		n = stracpy(base);
		for (p = n; *p && *p != POST_CHAR && *p != '#'; p++)
			;
		*p = 0;
		add_to_strn(&n, rel);
		goto return_n;
	}
	if (rel[0] == '?' || rel[0] == '&') {
		unsigned char rj[3];
		unsigned char *d = get_url_data(base);
		if (!d) goto bad_base;
		rj[0] = rel[0];
		rj[1] = POST_CHAR;
		rj[2] = 0;
		d += strcspn(cast_const_char d, cast_const_char rj);
		n = memacpy(base, d - base);
		add_to_strn(&n, rel);
		goto return_n;
	}
	if (rel[0] == '/' && rel[1] == '/' && !data) {
		unsigned char *s;
		if (!(s = cast_uchar strstr(cast_const_char base, "//"))) {
			if (!(s = cast_uchar strchr(cast_const_char base, ':'))) {
				bad_base:
				internal("bad base url: %s", base);
				return NULL;
			}
			s++;
		}
		n = memacpy(base, s - base);
		add_to_strn(&n, rel);
		if (!parse_url(n, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)) goto return_n;
		mem_free(n);
	}
	if (!casecmp(cast_uchar "proxy://", rel, 8)) goto prx;
	if (!parse_url(rel, &l, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)) {
		n = stracpy(rel);
		goto return_n;
	}
	n = stracpy(rel);
	while (n[0] && n[strlen(cast_const_char n) - 1] <= ' ') n[strlen(cast_const_char n) - 1] = 0;
	extend_str(&n, 1);
	ch = cast_uchar strrchr(cast_const_char n, '#');
	if (!ch || strchr(cast_const_char ch, '/')) ch = n + strlen(cast_const_char n);
	memmove(ch + 1, ch, strlen(cast_const_char ch) + 1);
	*ch = '/';
	if (!parse_url(n, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)) goto return_n;
	mem_free(n);
	prx:
	if (parse_url(base, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, &p, NULL, NULL) || !p) {
		goto bad_base;
	}
	if (!dsep(*p)) p--;
	if (!data) {
		if (end_of_dir(base, rel[0])) for (; *p; p++) {
			if (end_of_dir(base, *p)) break;
		} else if (!dsep(rel[0])) for (pp = p; *pp; pp++) {
			if (end_of_dir(base, *pp)) break;
			if (dsep(*pp)) p = pp + 1;
		}
	}
	n = memacpy(base, p - base);
	add_to_strn(&n, rel);
	goto return_n;

	return_n:
	extend_str(&n, 1);
	translate_directories(n);
	return n;
}

unsigned char *translate_url(unsigned char *url, unsigned char *cwd)
{
	unsigned char *ch;
	unsigned char *nu, *da;
	unsigned char *prefix;
	int sl;
	while (*url == ' ') url++;
	if (*url && url[strlen(cast_const_char url) - 1] == ' ') {
		nu = stracpy(url);
		while (*nu && nu[strlen(cast_const_char nu) - 1] == ' ') nu[strlen(cast_const_char nu) - 1] = 0;
		ch = translate_url(nu, cwd);
		mem_free(nu);
		return ch;
	}
	if (!casecmp(cast_uchar "proxy://", url, 8)) return NULL;
	if (!parse_url(url, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, &da, NULL, NULL)) {
		nu = stracpy(url);
		goto return_nu;
	}
	if (strchr(cast_const_char url, POST_CHAR)) return NULL;
	if (strstr(cast_const_char url, "://")) {
		nu = stracpy(url);
		extend_str(&nu, 1);
		ch = cast_uchar strrchr(cast_const_char nu, '#');
		if (!ch || strchr(cast_const_char ch, '/')) ch = nu + strlen(cast_const_char nu);
		memmove(ch + 1, ch, strlen(cast_const_char ch) + 1);
		*ch = '/';
		if (!parse_url(nu, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)) goto return_nu;
		mem_free(nu);
	}
	prefix = cast_uchar "file://";
	if (url[0] == '[' && strchr(cast_const_char url, ']')) {
		ch = url;
		goto http;
	}
	ch = url + strcspn(cast_const_char url, ".:/@");
	sl = 0;
#ifdef SPAD
	if (strchr(cast_const_char url, ':') && _is_local(cast_const_char url)) goto set_prefix;
#endif
	if (*ch != ':' || *(url + strcspn(cast_const_char url, "/@")) == '@') {
		if (*url != '.' && *ch == '.') {
			unsigned char *f, *e;
			int i;
			for (e = ch + 1; *(f = e + strcspn(cast_const_char e, ".:/")) == '.'; e = f + 1)
				;
			for (i = 0; i < f - e; i++) if (e[i] < '0' || e[i] > '9') goto noip;
			goto http;
			noip:
			if (f - e == 2 && casecmp(e, cast_uchar "gz", 2)) {
				http:
				prefix = cast_uchar "http://", sl = 1;
			} else {
				const char * const tld[] = {
					"com",
					"edu",
					"net",
					"org",
					"gov",
					"mil",
					"int",
					"arpa",
					"abogado",
					"academy",
					"accountant",
					"accountants",
					"active",
					"actor",
					"adult",
					"aero",
					"agency",
					"airforce",
					"allfinanz",
					"alsace",
					"amsterdam",
					"android",
					"apartments",
					"app",
					"aquarelle",
					"archi",
					"army",
					"arpa",
					"asia",
					"associates",
					"attorney",
					"auction",
					"audio",
					"autos",
					"axa",
					"band",
					"bank",
					"bar",
					"barclaycard",
					"barclays",
					"bargains",
					"bayern",
					"beer",
					"berlin",
					"best",
					"bid",
					"bike",
					"bingo",
					"bio",
					"biz",
					"black",
					"blackfriday",
					"blog",
					"bloomberg",
					"blue",
					"bmw",
					"bnpparibas",
					"boo",
					"boutique",
					"brussels",
					"budapest",
					"build",
					"builders",
					"business",
					"buzz",
					"bzh",
					"cab",
					"cafe",
					"cal",
					"camera",
					"camp",
					"cancerresearch",
					"capetown",
					"capital",
					"caravan",
					"cards",
					"care",
					"career",
					"careers",
					"casa",
					"cash",
					"casino",
					"cat",
					"catering",
					"center",
					"ceo",
					"cern",
					"channel",
					"chat",
					"cheap",
					"christmas",
					"chrome",
					"church",
					"citic",
					"city",
					"claims",
					"cleaning",
					"click",
					"clinic",
					"clothing",
					"club",
					"coach",
					"codes",
					"coffee",
					"college",
					"cologne",
					"com",
					"community",
					"company",
					"computer",
					"condos",
					"construction",
					"consulting",
					"contractors",
					"cooking",
					"cool",
					"coop",
					"country",
					"coupons",
					"credit",
					"creditcard",
					"cricket",
					"crs",
					"cruises",
					"cuisinella",
					"cymru",
					"dad",
					"dance",
					"date",
					"dating",
					"day",
					"deals",
					"degree",
					"delivery",
					"democrat",
					"dental",
					"dentist",
					"desi",
					"design",
					"diamonds",
					"diet",
					"digital",
					"direct",
					"directory",
					"discount",
					"dnp",
					"dog",
					"domains",
					"download",
					"durban",
					"dvag",
					"eat",
					"edu",
					"education",
					"email",
					"emerck",
					"energy",
					"engineer",
					"engineering",
					"enterprises",
					"equipment",
					"esq",
					"estate",
					"eus",
					"events",
					"everbank",
					"exchange",
					"expert",
					"exposed",
					"express",
					"fail",
					"faith",
					"farm",
					"fashion",
					"feedback",
					"finance",
					"financial",
					"firmdale",
					"fish",
					"fishing",
					"fit",
					"fitness",
					"flights",
					"florist",
					"flowers",
					"flsmidth",
					"fly",
					"foo",
					"football",
					"forsale",
					"foundation",
					"frl",
					"frogans",
					"fund",
					"furniture",
					"futbol",
					"fyi",
					"gal",
					"gallery",
					"garden",
					"gbiz",
					"gent",
					"gift",
					"gifts",
					"gives",
					"glass",
					"gle",
					"global",
					"globo",
					"gmail",
					"gmo",
					"gmx",
					"gold",
					"golf",
					"google",
					"gop",
					"gov",
					"graphics",
					"gratis",
					"green",
					"gripe",
					"guide",
					"guitars",
					"guru",
					"hamburg",
					"haus",
					"healthcare",
					"help",
					"here",
					"hiphop",
					"hiv",
					"hockey",
					"holdings",
					"holiday",
					"homes",
					"horse",
					"host",
					"hosting",
					"house",
					"how",
					"hsbc",
					"ibm",
					"immo",
					"immobilien",
					"industries",
					"info",
					"ing",
					"ink",
					"institute",
					"insure",
					"int",
					"international",
					"investments",
					"irish",
					"jetzt",
					"jewelry",
					"jobs",
					"joburg",
					"juegos",
					"kaufen",
					"kim",
					"kitchen",
					"kiwi",
					"koeln",
					"krd",
					"kred",
					"lacaixa",
					"land",
					"lat",
					"latrobe",
					"lawyer",
					"lds",
					"lease",
					"legal",
					"lgbt",
					"life",
					"lighting",
					"limited",
					"limo",
					"link",
					"loan",
					"loans",
					"lol",
					"london",
					"lotto",
					"love",
					"ltda",
					"luxe",
					"luxury",
					"madrid",
					"maison",
					"management",
					"mango",
					"market",
					"marketing",
					"markets",
					"mba",
					"media",
					"meet",
					"melbourne",
					"meme",
					"memorial",
					"men",
					"menu",
					"miami",
					"mil",
					"mini",
					"mobi",
					"moda",
					"moe",
					"monash",
					"money",
					"mormon",
					"mortgage",
					"moscow",
					"motorcycles",
					"mov",
					"movie",
					"museum",
					"nagoya",
					"name",
					"nato",
					"navy",
					"net",
					"network",
					"neustar",
					"new",
					"news",
					"nexus",
					"ngo",
					"nhk",
					"ninja",
					"nra",
					"nrw",
					"nyc",
					"okinawa",
					"one",
					"ong",
					"onl",
					"onlajn",
					"online",
					"ooo",
					"org",
					"organic",
					"otsuka",
					"ovh",
					"paris",
					"partners",
					"parts",
					"party",
					"pharmacy",
					"photo",
					"photography",
					"photos",
					"physio",
					"piaget",
					"pics",
					"pictures",
					"pink",
					"pizza",
					"place",
					"plumbing",
					"plus",
					"pohl",
					"poker",
					"porn",
					"post",
					"praxi",
					"press",
					"pro",
					"prod",
					"productions",
					"prof",
					"properties",
					"property",
					"pub",
					"qpon",
					"quebec",
					"racing",
					"realtor",
					"recipes",
					"red",
					"rehab",
					"reise",
					"reisen",
					"reit",
					"ren",
					"rentals",
					"repair",
					"report",
					"republican",
					"rest",
					"restaurant",
					"review",
					"reviews",
					"rich",
					"rio",
					"rip",
					"rocks",
					"rodeo",
					"rsvp",
					"ruhr",
					"run",
					"ryukyu",
					"saarland",
					"sajt",
					"sale",
					"sarl",
					"sca",
					"scb",
					"schmidt",
					"school",
					"schule",
					"science",
					"scot",
					"services",
					"sexy",
					"shiksha",
					"shoes",
					"show",
					"singles",
					"site",
					"soccer",
					"social",
					"software",
					"sohu",
					"solar",
					"solutions",
					"soy",
					"space",
					"spiegel",
					"style",
					"sucks",
					"supplies",
					"supply",
					"support",
					"surf",
					"surgery",
					"suzuki",
					"sydney",
					"systems",
					"taipei",
					"tatar",
					"tattoo",
					"tax",
					"taxi",
					"team",
					"technology",
					"tel",
					"tennis",
					"theater",
					"tienda",
					"tips",
					"tires",
					"tirol",
					"today",
					"tokyo",
					"tools",
					"top",
					"tours",
					"town",
					"toys",
					"trade",
					"training",
					"travel",
					"tui",
					"university",
					"uno",
					"uol",
					"vacations",
					"vegas",
					"ventures",
					"versicherung",
					"vet",
					"viajes",
					"video",
					"villas",
					"vision",
					"vlaanderen",
					"vodka",
					"vote",
					"voting",
					"voto",
					"voyage",
					"wales",
					"wang",
					"watch",
					"webcam",
					"website",
					"wed",
					"wedding",
					"whoswho",
					"wien",
					"wiki",
					"williamhill",
					"win",
					"wme",
					"work",
					"works",
					"world",
					"wtc",
					"wtf",
					"xxx",
					"xyz",
					"yandex",
					"yoga",
					"yokohama",
					"youtube",
					"zone",
					"zuerich",
					"onion",
					"exit",
					NULL };
				for (i = 0; tld[i]; i++) if ((size_t)(f - e) == strlen(tld[i]) && !casecmp(cast_uchar tld[i], e, f - e)) goto http;
			}
		}
		if (*ch == '@' || *ch == ':' || !cmpbeg(url, cast_uchar "ftp.")) prefix = cast_uchar "ftp://", sl = 1;
		goto set_prefix;
		set_prefix:
		nu = stracpy(prefix);
		add_to_strn(&nu, url);
		if (sl && !strchr(cast_const_char url, '/')) add_to_strn(&nu, cast_uchar "/");
		if (parse_url(nu, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)) {
			mem_free(nu);
			return NULL;
		}
		goto return_nu;
	}
#ifdef DOS_FS
	if (ch == url + 1) goto set_prefix;
#endif
	if (!(nu = memacpy(url, ch - url + 1))) return NULL;
	add_to_strn(&nu, cast_uchar "//");
	add_to_strn(&nu, ch + 1);
	if (!parse_url(nu, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)) goto return_nu;
	add_to_strn(&nu, cast_uchar "/");
	if (!parse_url(nu, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)) goto return_nu;
	mem_free(nu);
	return NULL;

	return_nu:
	insert_wd(&nu, cwd);
	extend_str(&nu, 1);
	translate_directories(nu);
	nu = translate_hashbang(nu);
	return nu;
}

unsigned char *extract_position(unsigned char *url)
{
	unsigned char *u, *uu, *r;
	if ((u = get_url_data(url))) url = u;
	if (!(u = cast_uchar strchr(cast_const_char url, POST_CHAR))) u = cast_uchar strchr(cast_const_char url, 0);
	if (!(uu = memchr(url, '#', u - url))) return NULL;
	r = mem_alloc(u - uu);
	memcpy(r, uu + 1, u - uu - 1);
	r[u - uu - 1] = 0;
	memmove(uu, u, strlen(cast_const_char u) + 1);
	return r;
}

int url_not_saveable(unsigned char *url)
{
	int p, palen;
	unsigned char *u = translate_url(url, cast_uchar "/");
	if (!u)
		return 1;
	p = parse_url(u, NULL, NULL, NULL, NULL, &palen, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	mem_free(u);
	return p || palen;
}

#define accept_char(x)	((x) != '"' && (x) != '\'' && (x) != '&' && (x) != '<' && (x) != '>')
#define special_char(x)	((x) == '%' || (x) == '#')

void add_conv_str(unsigned char **s, int *l, unsigned char *b, int ll, int encode_special)
{
	for (; ll > 0; ll--, b++) {
		if ((unsigned char)*b < ' ') continue;
		if (special_char(*b) && encode_special == 1) {
			unsigned char h[4];
			sprintf(cast_char h, "%%%02X", (unsigned)*b & 0xff);
			add_to_str(s, l, h);
		} else if (*b == '%' && encode_special <= -1 && ll > 2 && ((b[1] >= '0' && b[1] <= '9') || (b[1] >= 'A' && b[1] <= 'F') || (b[1] >= 'a' && b[1] <= 'f'))) {
			unsigned char h = 0;
			int i;
			for (i = 1; i < 3; i++) {
				if (b[i] >= '0' && b[i] <= '9') h = h * 16 + b[i] - '0';
				if (b[i] >= 'A' && b[i] <= 'F') h = h * 16 + b[i] - 'A' + 10;
				if (b[i] >= 'a' && b[i] <= 'f') h = h * 16 + b[i] - 'a' + 10;
			}
			if (h >= ' ') add_chr_to_str(s, l, h);
			ll -= 2;
			b += 2;
		} else if (*b == ' ' && (!encode_special || encode_special == -1)) {
			add_to_str(s, l, cast_uchar "&nbsp;");
		} else if (accept_char(*b) || encode_special == -2) {
			add_chr_to_str(s, l, *b);
		} else {
			add_to_str(s, l, cast_uchar "&#");
			add_num_to_str(s, l, (int)*b);
			add_chr_to_str(s, l, ';');
		}
	}
}

