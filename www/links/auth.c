#include "links.h"

static struct list_head auth = {&auth, &auth};

struct http_auth {
	struct http_auth *next;
	struct http_auth *prev;
	unsigned char *host;
	int port;
	unsigned char *realm;
	unsigned char *user;
	unsigned char *password;
	unsigned char *directory;
	unsigned char *user_password_encoded;
	int proxy;
};

static unsigned char base64_chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

static unsigned char *base64_encode(unsigned char *in)
{
	unsigned char *out, *outstr;
	size_t inlen = strlen(cast_const_char in);
	if (inlen > MAXINT / 4) overalloc();
	outstr = out = mem_alloc(((inlen / 3) + 1) * 4 + 1 );
	while (inlen >= 3) {
		*out++ = base64_chars[ (int)(*in >> 2) ];
		*out++ = base64_chars[ (int)((*in << 4 | *(in + 1) >> 4) & 63) ];
		*out++ = base64_chars[ (int)((*(in + 1) << 2 | *(in + 2) >> 6) & 63) ];
		*out++ = base64_chars[ (int)(*(in + 2) & 63) ];
		inlen -= 3; in += 3;
	}
	if (inlen == 1) {
		*out++ = base64_chars[ (int)(*in >> 2) ];
		*out++ = base64_chars[ (int)(*in << 4 & 63) ];
		*out++ = '=';
		*out++ = '=';
	}
	if (inlen == 2) {
		*out++ = base64_chars[ (int)(*in >> 2) ];
		*out++ = base64_chars[ (int)((*in << 4 | *(in + 1) >> 4) & 63) ];
		*out++ = base64_chars[ (int)((*(in + 1) << 2) & 63) ];
		*out++ = '=';
	}
	*out = 0;
	return outstr;
}

static unsigned char *basic_encode(unsigned char *user, unsigned char *password)
{
	unsigned char *e, *p = mem_alloc(strlen(cast_const_char user) + strlen(cast_const_char password) + 2);
	strcpy(cast_char p, cast_const_char user);
	strcat(cast_char p, ":");
	strcat(cast_char p, cast_const_char password);
	e = base64_encode(p);
	mem_free(p);
	return e;
}

unsigned char *get_auth_realm(unsigned char *url, unsigned char *head, int proxy)
{
	unsigned char *ch = head;
	unsigned char *h, *q, *r;
	int l;
	int unknown = 0;
	int known = 0;
	try_next:
	h = parse_http_header(ch, !proxy ? (unsigned char *)"WWW-Authenticate" : (unsigned char *)"Proxy-Authenticate", &ch);
	if (!h) {
		if (unknown && !known) return NULL;
		if (proxy) {
			unsigned char *p = get_proxy_string(url);
			if (!p) p = cast_uchar "";
			return stracpy(p);
		} else {
			unsigned char *u = get_host_name(url);
			if (u) return u;
			return stracpy(cast_uchar "");
		}
	}
	if (casecmp(h, cast_uchar "Basic", 5)) {
		mem_free(h);
		unknown = 1;
		goto try_next;
	}
	known = 1;
	q = cast_uchar strchr(cast_const_char h, '"');
	if (!q) {
		mem_free(h);
		goto try_next;
	}
	q++;
	r = init_str();
	l = 0;
	while (*q && *q != '"') {
		if (*q == '\\' && !*++q) break;
		add_chr_to_str(&r, &l, *q++);
	}
	mem_free(h);
	return r;
}

static unsigned char *auth_from_url(unsigned char *url, int proxy)
{
	unsigned char *r = NULL;
	int l = 0;
	unsigned char *user, *password;

	user = get_user_name(url);
	password = get_pass(url);
	if (user && *user && password) {
		unsigned char *e = basic_encode(user, password);
		r = init_str();
		if (proxy) add_to_str(&r, &l, cast_uchar "Proxy-");
		add_to_str(&r, &l, cast_uchar "Authorization: Basic ");
		add_to_str(&r, &l, e);
		add_to_str(&r, &l, cast_uchar "\r\n");
		mem_free(e);
		if (user) mem_free(user);
		if (password) mem_free(password);
		return r;
	}
	if (user) mem_free(user);
	if (password) mem_free(password);
	return NULL;
}

unsigned char *get_auth_string(unsigned char *url, int proxy)
{
	struct http_auth *a;
	unsigned char *host;
	int port;
	unsigned char *r = NULL;
	int l = 0;
	if (proxy && upcase(url[0]) != 'P') return NULL;
	if (!(host = get_host_name(url))) return NULL;
	port = get_port(url);

	if (!proxy && (r = auth_from_url(url, proxy))) goto have_passwd;

	foreach(a, auth) if (a->proxy == proxy && !strcasecmp(cast_const_char a->host, cast_const_char host) && a->port == port) {
		unsigned char *d, *data;
		if (proxy) goto skip_dir_check;
		data = get_url_data(url);
		d = cast_uchar strrchr(cast_const_char data, '/');
		if (!d) d = data;
		else d++;
		if ((size_t)(d - data) >= strlen(cast_const_char a->directory) && !memcmp(data, a->directory, strlen(cast_const_char a->directory))) {
			skip_dir_check:
			r = init_str();
			if (proxy) add_to_str(&r, &l, cast_uchar "Proxy-");
			add_to_str(&r, &l, cast_uchar "Authorization: Basic ");
			add_to_str(&r, &l, a->user_password_encoded);
			add_to_str(&r, &l, cast_uchar "\r\n");
			goto have_passwd;
		}
	}

	if (proxy && (r = auth_from_url(url, proxy))) goto have_passwd;

	have_passwd:
	mem_free(host);
	return r;
}

static void free_auth_entry(struct http_auth *a)
{
	mem_free(a->host);
	mem_free(a->realm);
	mem_free(a->user);
	mem_free(a->password);
	if (a->directory) mem_free(a->directory);
	mem_free(a->user_password_encoded);
	del_from_list(a);
	mem_free(a);
}

void cleanup_auth(void)
{
	while (!list_empty(auth)) free_auth_entry(auth.next);
}

void add_auth(unsigned char *url, unsigned char *realm, unsigned char *user, unsigned char *password, int proxy)
{
	struct http_auth *a;
	unsigned char *host = NULL;
	int port = 0	/* against warning */;
	if (!proxy) {
		host = get_host_name(url);
		port = get_port(url);
	} else {
		unsigned char *p = get_proxy(url);
		if (strcmp(cast_const_char p, cast_const_char url)) {
			host = get_host_name(p);
			port = get_port(p);
		}
		mem_free(p);
	}
	if (!host) return;
	foreach(a, auth) if (a->proxy == proxy && !strcasecmp(cast_const_char a->host, cast_const_char host) && a->port == port && !strcmp(cast_const_char a->realm, cast_const_char realm)) {
		a = a->prev;
		free_auth_entry(a->next);
	}
	a = mem_alloc(sizeof(struct http_auth));
	a->host = host;
	a->port = port;
	a->realm = stracpy(realm);
	a->user = stracpy(user);
	a->password = stracpy(password);
	if (!proxy) {
		unsigned char *data = stracpy(get_url_data(url));
		unsigned char *d = cast_uchar strrchr(cast_const_char data, '/');
		if (d) d[1] = 0;
		else data[0] = 0;
		a->directory = data;
	} else a->directory = NULL;
	a->proxy = proxy;
	a->user_password_encoded = basic_encode(a->user, a->password);
	add_to_list(auth, a);
}

int find_auth(unsigned char *url, unsigned char *realm)
{
	struct http_auth *a;
	unsigned char *data, *d;
	unsigned char *host = get_host_name(url);
	int port = get_port(url);
	if (!host) return -1;
	data = stracpy(get_url_data(url));
	d = cast_uchar strrchr(cast_const_char data, '/');
	if (d) d[1] = 0;
	foreach(a, auth) if (!a->proxy && !strcasecmp(cast_const_char a->host, cast_const_char host) && a->port == port && !strcmp(cast_const_char a->realm, cast_const_char realm) && strcmp(cast_const_char a->directory, cast_const_char data)) {
		mem_free(a->directory);
		a->directory = data;
		mem_free(host);
		del_from_list(a);
		add_to_list(auth, a);
		return 0;
	}
	mem_free(host);
	mem_free(data);
	return -1;
}

