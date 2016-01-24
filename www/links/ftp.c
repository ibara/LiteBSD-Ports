/* ftp.c
 * ftp:// processing
 * (c) 2002 Mikulas Patocka
 * This file is a part of the Links program, released under GPL.
 */

#include "links.h"

#define FTP_BUF	16384

struct ftp_connection_info {
	int pending_commands;
	int opc;
	int pasv;
	int eprt_epsv;
	int dir;
	int rest_sent;
	int we_are_in_root;
	int conn_st;
	int d;
	int dpos;
	int buf_pos;
	unsigned char ftp_buffer[FTP_BUF];
	unsigned char cmdbuf[1];
};

static void ftp_get_banner(struct connection *);
static void ftp_got_banner(struct connection *, struct read_buffer *);
static void ftp_login(struct connection *);
static void ftp_logged(struct connection *);
static void ftp_sent_passwd(struct connection *);
static void ftp_got_info(struct connection *, struct read_buffer *);
static void ftp_got_user_info(struct connection *, struct read_buffer *);
static void ftp_dummy_info(struct connection *, struct read_buffer *);
static void ftp_pass_info(struct connection *, struct read_buffer *);
static void ftp_send_retr_req(struct connection *, int, int);
static struct ftp_connection_info *add_file_cmd_to_str(struct connection *, int);
static void ftp_retr_1(struct connection *);
static void ftp_retr_file(struct connection *, struct read_buffer *);
static void ftp_got_final_response(struct connection *, struct read_buffer *);
static void created_data_connection(struct connection *);
static void got_something_from_data_connection(struct connection *);
static void ftp_end_request(struct connection *, int);
static int get_ftp_response(struct connection *, struct read_buffer *, int);
static int ftp_process_dirlist(struct cache_entry *, off_t *, int *, unsigned char *, int, int, int, int *);


static int get_ftp_response(struct connection *c, struct read_buffer *rb, int part)
{
	int l;
	set_connection_timeout(c);
	again:
	for (l = 0; l < rb->len; l++) if (rb->data[l] == 10) {
		unsigned char *e;
		long k = strtoul(cast_const_char rb->data, (char **)(void *)&e, 10);
		if (e != rb->data + 3 || k < 100 || k >= 1000) return -1;
		if (*e == '-') {
			int i;
			for (i = 0; i < rb->len - 5; i++) {
				if (rb->data[i] == 10 && !memcmp(rb->data+i+1, rb->data, 3) && rb->data[i+4] == ' ') {
					for (i++; i < rb->len; i++) if (rb->data[i] == 10) goto ok;
					return 0;
				}
			}
			return 0;
			ok:
			l = i;
		}
		if (!part && k >= 100 && k < 200) {
			kill_buffer_data(rb, l + 1);
			goto again;
		}
		if (part == 2) return (int)k;
		kill_buffer_data(rb, l + 1);
		return (int)k;
	}
	return 0;
}

void ftp_func(struct connection *c)
{
	int we_are_in_root;
	/*setcstate(c, S_CONN);*/
	/*set_connection_timeout(c);*/
	if (get_keepalive_socket(c, &we_are_in_root)) {
		int p;
		if ((p = get_port(c->url)) == -1) {
			setcstate(c, S_BAD_URL);
			abort_connection(c);
			return;
		}
		make_connection(c, p, &c->sock1, ftp_options.fast_ftp ? ftp_login : ftp_get_banner);
	} else {
		ftp_send_retr_req(c, S_SENT, we_are_in_root);
	}
}

static void ftp_get_banner(struct connection *c)
{
	struct read_buffer *rb;
	set_connection_timeout(c);
	setcstate(c, S_SENT);
	if (!(rb = alloc_read_buffer(c))) return;
	read_from_socket(c, c->sock1, rb, ftp_got_banner);
}

static void ftp_got_banner(struct connection *c, struct read_buffer *rb)
{
	int g = get_ftp_response(c, rb, 0);
	if (g == -1) { setcstate(c, S_FTP_ERROR); abort_connection(c); return; }
	if (!g) { read_from_socket(c, c->sock1, rb, ftp_got_banner); return; }
	if (g >= 400) { setcstate(c, S_FTP_UNAVAIL); retry_connection(c); return; }
	ftp_login(c);
}

static void ftp_login(struct connection *c)
{
	unsigned char *login;
	unsigned char *u;
	int logl = 0;
	set_connection_timeout(c);
	login = init_str();
	add_to_str(&login, &logl, cast_uchar "USER ");
	if ((u = get_user_name(c->url)) && *u) add_to_str(&login, &logl, u);
	else add_to_str(&login, &logl, cast_uchar "anonymous");
	if (u) mem_free(u);
	if (ftp_options.fast_ftp) {
		struct ftp_connection_info *fi;
		add_to_str(&login, &logl, cast_uchar "\r\nPASS ");
		if ((u = get_pass(c->url)) && *u) add_to_str(&login, &logl, u);
		else add_to_str(&login, &logl, ftp_options.anon_pass);
		if (u) mem_free(u);
		add_to_str(&login, &logl, cast_uchar "\r\n");
		if (!(fi = add_file_cmd_to_str(c, 0))) {
			mem_free(login);
			return;
		}
		add_to_str(&login, &logl, fi->cmdbuf);
	} else add_to_str(&login, &logl, cast_uchar "\r\n");
	write_to_socket(c, c->sock1, login, logl, ftp_logged);
	mem_free(login);
	setcstate(c, S_SENT);
}

static void ftp_logged(struct connection *c)
{
	struct read_buffer *rb;
	if (!(rb = alloc_read_buffer(c))) return;
	if (!ftp_options.fast_ftp) {
		ftp_got_user_info(c, rb);
		return;
	}
	read_from_socket(c, c->sock1, rb, ftp_got_info);
}

static void ftp_got_info(struct connection *c, struct read_buffer *rb)
{
	int g = get_ftp_response(c, rb, 0);
	if (g == -1) { setcstate(c, S_FTP_ERROR); abort_connection(c); return; }
	if (!g) { read_from_socket(c, c->sock1, rb, ftp_got_info); return; }
	if (g >= 400) { setcstate(c, S_FTP_UNAVAIL); retry_connection(c); return; }
	ftp_got_user_info(c, rb);
}

static void ftp_got_user_info(struct connection *c, struct read_buffer *rb)
{
	int g = get_ftp_response(c, rb, 0);
	if (g == -1) { setcstate(c, S_FTP_ERROR); abort_connection(c); return; }
	if (!g) { read_from_socket(c, c->sock1, rb, ftp_got_user_info); return; }
	if (g >= 530 && g < 540) { setcstate(c, S_FTP_LOGIN); retry_connection(c); return; }
	if (g >= 400) { setcstate(c, S_FTP_UNAVAIL); retry_connection(c); return; }
	if (g >= 200 && g < 300) {
		if (ftp_options.fast_ftp) ftp_dummy_info(c, rb);
		else ftp_send_retr_req(c, S_GETH, 0);
	} else {
		if (ftp_options.fast_ftp) ftp_pass_info(c, rb);
		else {
			unsigned char *login;
			unsigned char *u;
			int logl = 0;
			login = init_str();
			add_to_str(&login, &logl, cast_uchar "PASS ");
			if ((u = get_pass(c->url)) && *u) add_to_str(&login, &logl, u);
			else add_to_str(&login, &logl, ftp_options.anon_pass);
			if (u) mem_free(u);
			add_to_str(&login, &logl, cast_uchar "\r\n");
			write_to_socket(c, c->sock1, login, logl, ftp_sent_passwd);
			mem_free(login);
			setcstate(c, S_LOGIN);
		}
	}
}

static void ftp_dummy_info(struct connection *c, struct read_buffer *rb)
{
	int g = get_ftp_response(c, rb, 0);
	if (g == -1) { setcstate(c, S_FTP_ERROR); abort_connection(c); return; }
	if (!g) { read_from_socket(c, c->sock1, rb, ftp_dummy_info); return; }
	ftp_retr_file(c, rb);
}

static void ftp_sent_passwd(struct connection *c)
{
	struct read_buffer *rb;
	if (!(rb = alloc_read_buffer(c))) return;
	read_from_socket(c, c->sock1, rb, ftp_pass_info);
}

static void ftp_pass_info(struct connection *c, struct read_buffer *rb)
{
	int g = get_ftp_response(c, rb, 0);
	if (g == -1) { setcstate(c, S_FTP_ERROR); abort_connection(c); return; }
	if (!g) { read_from_socket(c, c->sock1, rb, ftp_pass_info); setcstate(c, S_LOGIN); return; }
	if (g >= 530 && g < 540) { setcstate(c, S_FTP_LOGIN); abort_connection(c); return; }
	if (g >= 400) { setcstate(c, S_FTP_UNAVAIL); abort_connection(c); return; }
	if (ftp_options.fast_ftp) ftp_retr_file(c, rb);
	else ftp_send_retr_req(c, S_GETH, 0);
}

static void add_port_pasv(unsigned char **s, int *l, struct ftp_connection_info *inf, unsigned char *port_string)
{
	if (!inf->pasv) {
		if (inf->eprt_epsv) {
			add_to_str(s, l, cast_uchar "EPRT ");
			add_to_str(s, l, port_string);
		} else {
			add_to_str(s, l, cast_uchar "PORT ");
			add_to_str(s, l, port_string);
		}
	} else {
		if (inf->eprt_epsv) {
			add_to_str(s, l, cast_uchar "EPSV");
		} else {
			add_to_str(s, l, cast_uchar "PASV");
		}
	}
	add_to_str(s, l, cast_uchar "\r\n");
}

static struct ftp_connection_info *add_file_cmd_to_str(struct connection *c, int we_are_in_root)
{
	unsigned char *d = get_url_data(c->url);
	unsigned char *dd, *de;
	int del;
	unsigned char port_string[50];
	struct ftp_connection_info *inf, *inf2;
	unsigned char *s;
	int l;
	if (!d) {
		internal("get_url_data failed");
		setcstate(c, S_INTERNAL);
		abort_connection(c);
		return NULL;
	}

	de = init_str(), del = 0;
	add_conv_str(&de, &del, d, (int)strlen(cast_const_char d), -2);
	d = de;
	inf = mem_alloc(sizeof(struct ftp_connection_info));
	memset(inf, 0, sizeof(struct ftp_connection_info));
	l = 0;
	s = init_str();
	inf->we_are_in_root = we_are_in_root;
	inf->pasv = ftp_options.passive_ftp;
#ifdef LINKS_2
	if (*c->socks_proxy) inf->pasv = 1;
	if (ftp_options.eprt_epsv || is_ipv6(c->sock1)) inf->eprt_epsv = 1;
#endif
	c->info = inf;

	if (!inf->pasv) {
		int ps;
#ifdef SUPPORT_IPV6
		if (is_ipv6(c->sock1)) {
			ps = get_pasv_socket_ipv6(c, c->sock1, &c->sock2, port_string);
			if (ps) {
				mem_free(d);
				mem_free(s);
				return NULL;
			}
		} else
#endif
		{
			unsigned char pc[6];
			ps = get_pasv_socket(c, c->sock1, &c->sock2, pc);
			if (ps) {
				mem_free(d);
				mem_free(s);
				return NULL;
			}
			if (inf->eprt_epsv)
				sprintf(cast_char port_string, "|1|%d.%d.%d.%d|%d|", pc[0], pc[1], pc[2], pc[3], (pc[4] << 8) | pc[5]);
			else
				sprintf(cast_char port_string, "%d,%d,%d,%d,%d,%d", pc[0], pc[1], pc[2], pc[3], pc[4], pc[5]);
		}
		if (strlen(cast_const_char port_string) >= sizeof(port_string))
			internal("buffer overflow in get_pasv_socket_ipv6: %d > %d", (int)strlen(cast_const_char port_string), (int)sizeof(port_string));
	}
#ifdef HAVE_IPTOS
	if (ftp_options.set_tos) {
		int rx;
		int on = IPTOS_THROUGHPUT;
		EINTRLOOP(rx, setsockopt(c->sock2, IPPROTO_IP, IP_TOS, (char *)&on, sizeof(int)));
	}
#endif
	dd = d;
	while (*dd == '/') dd++;
	if (!(de = cast_uchar strchr(cast_const_char dd, POST_CHAR))) de = cast_uchar strchr(cast_const_char dd, 0);
	if (dd == de || de[-1] == '/') {
		inf->dir = 1;
		inf->pending_commands = 3;
		add_to_str(&s, &l, cast_uchar "TYPE A\r\n");
		add_port_pasv(&s, &l, inf, port_string);
		if (!inf->we_are_in_root) {
			add_to_str(&s, &l, cast_uchar "CWD /\r\n");
			inf->we_are_in_root = 1;
			inf->pending_commands++;
		}
		if (dd != de) {
			add_to_str(&s, &l, cast_uchar "CWD ");
			add_bytes_to_str(&s, &l, dd, de - 1 - dd);
			add_to_str(&s, &l, cast_uchar "\r\n");
			inf->we_are_in_root = 0;
			inf->pending_commands++;
		}
		add_to_str(&s, &l, cast_uchar "LIST\r\n");
		c->from = 0;
	} else {
		inf->dir = 0;
		inf->pending_commands = 3;
		add_to_str(&s, &l, cast_uchar "TYPE I\r\n");
		add_port_pasv(&s, &l, inf, port_string);
		if (!inf->we_are_in_root) {
			add_to_str(&s, &l, cast_uchar "CWD /\r\n");
			inf->we_are_in_root = 1;
			inf->pending_commands++;
		}
		if (c->from && c->no_cache < NC_IF_MOD) {
			add_to_str(&s, &l, cast_uchar "REST ");
			add_num_to_str(&s, &l, c->from);
			add_to_str(&s, &l, cast_uchar "\r\n");
			inf->rest_sent = 1;
			inf->pending_commands++;
		} else {
			c->from = 0;
		}
		add_to_str(&s, &l, cast_uchar "RETR ");
		add_bytes_to_str(&s, &l, dd, de - dd);
		add_to_str(&s, &l, cast_uchar "\r\n");
	}
	inf->opc = inf->pending_commands;
	if ((unsigned)l > MAXINT - sizeof(struct ftp_connection_info) - 1) overalloc();
	inf2 = mem_realloc(inf, sizeof(struct ftp_connection_info) + l + 1);
	strcpy(cast_char (inf = inf2)->cmdbuf, cast_const_char s);
	mem_free(s);
	c->info = inf;
	mem_free(d);
	return inf;
}


static void ftp_send_retr_req(struct connection *c, int state, int we_are_in_root)
{
	struct ftp_connection_info *fi;
	unsigned char *login;
	int logl = 0;
	set_connection_timeout(c);
	login = init_str();
	if (!c->info && !(fi = add_file_cmd_to_str(c, we_are_in_root))) {
		mem_free(login);
		return;
	} else {
		fi = c->info;
	}
	if (ftp_options.fast_ftp) {
		a:add_to_str(&login, &logl, fi->cmdbuf);
	} else {
		unsigned char *nl = cast_uchar strchr(cast_const_char fi->cmdbuf, '\n');
		if (!nl) goto a;
		nl++;
		add_bytes_to_str(&login, &logl, fi->cmdbuf, nl - fi->cmdbuf);
		memmove(fi->cmdbuf, nl, strlen(cast_const_char nl) + 1);
	}
	write_to_socket(c, c->sock1, login, logl, ftp_retr_1);
	mem_free(login);
	setcstate(c, state);
}

static void ftp_retr_1(struct connection *c)
{
	struct read_buffer *rb;
	if (!(rb = alloc_read_buffer(c))) return;
	read_from_socket(c, c->sock1, rb, ftp_retr_file);
}

static void ftp_retr_file(struct connection *c, struct read_buffer *rb)
{
	int g;
	struct ftp_connection_info *inf = c->info;
	if (0) {
		rep:
		if (!ftp_options.fast_ftp) {
			ftp_send_retr_req(c, S_GETH, inf->we_are_in_root);
			return;
		}
	}
	if (inf->pending_commands > 1) {
		unsigned char pc[6] = { 0, 0, 0, 0, 0, 0 }; /* against warning */
		if (inf->pasv && inf->opc - (inf->pending_commands - 1) == 2) {
			int i, j;
			i = 3;
			if (!inf->eprt_epsv) while (i < rb->len) {
				if (rb->data[i] >= '0' && rb->data[i] <= '9') {
					for (j = 0; j < 6; j++) {
						int n = 0;
						while (rb->data[i] >= '0' && rb->data[i] <= '9') {
							n = n * 10 + rb->data[i] - '0';
							if (n >= 256) goto no_pasv;
							if (++i >= rb->len) goto no_pasv;
						}
						pc[j] = (unsigned char)n;
						if (j != 5) {
							if (rb->data[i] != ',') goto xa;
							if (++i >= rb->len) goto xa;
							if (rb->data[i] < '0' || rb->data[i] > '9') {
								xa:
								if (j != 1) goto no_pasv;
								pc[4] = pc[0];
								pc[5] = pc[1];
								pc[0] = pc[1] = pc[2] = pc[3] = 0;
								goto pasv_ok;
							}
						}
					}
					goto pasv_ok;
				}
				i++;
			}
			no_pasv:
			i = 3;
			while (i < rb->len - 5) {
				if (rb->data[i] == '(' && (rb->data[i + 1] < '0' || rb->data[i + 1] > '9') && rb->data[i + 1] == rb->data[i + 2] && rb->data[i + 2] == rb->data[i + 3]) {
					unsigned char delim = rb->data[i + 1];
					int n = 0;
					i += 4;
					while (rb->data[i] >= '0' && rb->data[i] <= '9') {
						n = n * 10 + rb->data[i] - '0';
						if (n >= 65536) goto no_epsv;
						if (++i >= rb->len) goto no_epsv;
					}
					if (rb->data[i] != delim) goto no_epsv;
					pc[4] = n >> 8;
					pc[5] = n & 0xff;
					pc[0] = pc[1] = pc[2] = pc[3] = 0;
					goto pasv_ok;
				}
				i++;
			}
			no_epsv:
			memset(pc, 0, sizeof pc);
			pasv_ok:;
		}
		g = get_ftp_response(c, rb, 0);
		if (g == -1) { setcstate(c, S_FTP_ERROR); abort_connection(c); return; }
		if (!g) { read_from_socket(c, c->sock1, rb, ftp_retr_file); setcstate(c, S_GETH); return; }
		inf->pending_commands--;
		switch (inf->opc - inf->pending_commands) {
			case 1:		/* TYPE */
				goto rep;
			case 2:		/* PORT */
				if (g >= 400) { setcstate(c, S_FTP_PORT); abort_connection(c); return; }
				if (inf->pasv) {
					if (!pc[4] && !pc[5]) {
						setcstate(c, S_FTP_ERROR);
						retry_connection(c);
						return;
					}
					make_connection(c, (pc[4] << 8) + pc[5], &c->sock2, created_data_connection);
				}
				goto rep;
			case 3:		/* REST / CWD */
			case 4:
				if (g >= 400) {
					if (!inf->dir && c->from && inf->pending_commands == 1) c->from = 0;
					else { setcstate(c, S_FTP_NO_FILE); abort_connection(c); return; }
				}
				goto rep;
		}
		internal("WHAT???");
	}
	g = get_ftp_response(c, rb, 2);
	if (!g) { read_from_socket(c, c->sock1, rb, ftp_retr_file); setcstate(c, S_GETH); return; }
	if (g >= 100 && g < 200) {
		unsigned char *d = rb->data;
		int i, p = 0;
		for (i = 0; i < rb->len && d[i] != 10; i++) if (d[i] == '(') p = i;
		if (!p || p == rb->len - 1) goto nol;
		p++;
		if (d[p] < '0' || d[p] > '9') goto nol;
		for (i = p; i < rb->len; i++) if (d[i] < '0' || d[i] > '9') goto quak;
		goto nol;
		quak:
		for (; i < rb->len; i++) if (d[i] != ' ') break;
		if (i + 4 > rb->len) goto nol;
		if (casecmp(&d[i], cast_uchar "byte", 4)) goto nol;
		/* when resuming file transfer, some servers return total size,
		   others return the number of remaining bytes. So it is not
		   reliable to guess file size in this case */
		if (c->from) goto nol;
		{
			my_strtoll_t est = my_strtoll(&d[p], NULL);
			if (est < 0 || (off_t)est < 0 || (off_t)est != est) est = 0;
			if (est) c->est_length = est + c->from;
		}
		nol:;
	}
	if (!inf->pasv)
		set_handlers(c->sock2, (void (*)(void *))got_something_from_data_connection, NULL, c);
	/*read_from_socket(c, c->sock1, rb, ftp_got_final_response);*/
	ftp_got_final_response(c, rb);
}

static void ftp_got_final_response(struct connection *c, struct read_buffer *rb)
{
	struct ftp_connection_info *inf = c->info;
	int g = get_ftp_response(c, rb, 0);
	if (g == -1) { setcstate(c, S_FTP_ERROR); abort_connection(c); return; }
	if (!g) { read_from_socket(c, c->sock1, rb, ftp_got_final_response); if (c->state != S_TRANS) setcstate(c, S_GETH); return; }
	if (g == 425 || g == 450 || g == 500 || g == 501 || g == 550) {
		if (c->url[strlen(cast_const_char c->url) - 1] == '/') goto skip_redir;
		if (!c->cache) {
			if (get_cache_entry(c->url, &c->cache)) {
				setcstate(c, S_OUT_OF_MEM);
				abort_connection(c);
				return;
			}
			c->cache->refcount--;
		}
		if (c->cache->redirect) mem_free(c->cache->redirect);
		c->cache->redirect = stracpy(c->url);
		c->cache->redirect_get = 1;
		add_to_strn(&c->cache->redirect, cast_uchar "/");
		c->cache->incomplete = 0;
		/*setcstate(c, S_FTP_NO_FILE);*/
		setcstate(c, S__OK);
		abort_connection(c);
		return;
	}
	skip_redir:
	if (g >= 400) { setcstate(c, S_FTP_FILE_ERROR); abort_connection(c); return; }
	if (inf->conn_st == 2) {
		ftp_end_request(c, S__OK);
	} else {
		inf->conn_st = 1;
		if (c->state != S_TRANS) setcstate(c, S_GETH);
	}
}

static int is_date(unsigned char *data)	/* can touch at most data[-4] --- "n 12 "<--if fed with this --- if you change it, fix the caller */
{
	/* fix for ftp://ftp.su.se/ */
	if (*data == ' ') data--;
	if (data[0] >= '0' && data[0] <= '9' && data[-1] >= '0' && data[-1] <= '9') data -= 2;
	else if (data[0] >= '1' && data[0] <= '9' && data[-1] == ' ') data -= 1 + (data[-2] == ' ');
	else return 0;
	if (data[0] == ':') return 1;
	if (data[0] != ' ') return 0;
	if ((data[-1] < 'a' || data[-1] > 'z') && (data[-1] < 'A' || data[-1] > 'Z')) return 0;
	return 1;
}

#define PARSE_MODE_VMS		-1

static int ftp_process_dirlist(struct cache_entry *ce, off_t *pos, int *d, unsigned char *bf, int ln, int fin, int root, int *tr)
{
	unsigned char *str, *buf;
	int sl;
	int ret = 0;
	int p;
	int len;
	int f;
	int a;
	int dir;
	int sp;
	int ee;
	again:
	buf = bf + ret;
	len = ln - ret;
	for (p = 0; p < len; p++) if (buf[p] == '\n') goto lb;
	if (p && (fin || len >= FTP_BUF)) {
		ret += p;
		goto pl;
	}
	return ret;
	lb:
	ret += p + 1;
	if (p && buf[p - 1] == '\r') p--;
	pl:
	str = init_str();
	sl = 0;
	/*add_to_str(&str, &sl, cast_uchar "   ");*/

	dir = 0;
	if (!*d || *d == PARSE_MODE_VMS) for (ee = 0; ee + 1 < p && !WHITECHAR(buf[ee]); ee++) {
		if (buf[ee] == ';' && buf[ee + 1] >= '1' && buf[ee + 1] <= '9') {
			if (!ee) goto skip_vms;
			if (ee >= 4 && buf[ee - 4] == '.' && buf[ee - 3] == 'D' && buf[ee - 2] == 'I' && buf[ee - 1] == 'R') {
				if (ee == 4) goto raw;
				dir = 1, ee -= 4;
			} else {
				/*if (buf[ee + 1] == '1' && (ee + 2 == p || buf[ee + 2] < '0' || buf[ee + 2] > '9')) goto no_version;
				ee += 2;
				while (ee < p && buf[ee] >= '0' && buf[ee] < '9') ee++;
				no_version:;*/
			}
			if (*d != PARSE_MODE_VMS && !root) {
				add_to_str(&str, &sl, cast_uchar "<a href=\"../\">..</a>\n");
			}
			sp = 0;
			*d = PARSE_MODE_VMS;
			goto put_entry;
		}
	}
	skip_vms:

	if (*d < 0) goto raw;

	f = *d;
	if (*d && *d < p && WHITECHAR(buf[*d - 1])) {
		sp = *d;
		ppp:
		for (ee = sp; ee <= p - 4; ee++)
			if (!memcmp(buf + ee, cast_uchar " -> ", 4)) goto syml;
		ee = p;
		syml:
		if (!f && !root) {
			if ((ee - sp != 1 || buf[sp] != '.') &&
			    (ee - sp != 2 || buf[sp] != '.' || buf[sp + 1] != '.')) {
				int i;
				for (i = 0; i < sp; i++) add_chr_to_str(&str, &sl, ' ');
				add_to_str(&str, &sl, cast_uchar "<a href=\"../\">..</a>\n");
			}
		}
		dir = buf[0] == 'd';
		if (!dir) {
			unsigned char *p = memacpy(buf, sp);
			if (strstr(cast_const_char p, "<DIR>")) dir = 1;
			mem_free(p);
		};
		put_entry:
		add_conv_str(&str, &sl, buf, sp, 0);
		add_to_str(&str, &sl, cast_uchar "<a href=\"./");
		add_conv_str(&str, &sl, buf + sp, ee - sp, 1);
		if (dir) add_chr_to_str(&str, &sl, '/');
		add_to_str(&str, &sl, cast_uchar "\">");
		add_conv_str(&str, &sl, buf + sp, ee - sp, 0);
		add_to_str(&str, &sl, cast_uchar "</a>");
		add_conv_str(&str, &sl, buf + ee, p - ee, 0);
	} else {
		int pp, ppos;
		int bp, bn;
		if (p > 5 && !casecmp(buf, cast_uchar "total", 5)) goto raw;
		if (p > 10 && !memcmp(buf, cast_uchar "Directory ", 10)) goto raw;
		for (pp = p - 1; pp >= 0; pp--) if (!WHITECHAR(buf[pp])) break;
		if (pp < 0) goto raw;
		if (pp < p - 1) pp++;
		ppos = -1;
		for (; pp >= 10; pp--) if (WHITECHAR(buf[pp])) {
			if (is_date(&buf[pp - 6]) &&
			    buf[pp - 5] == ' ' &&
			    ((buf[pp - 4] == '2' && buf[pp - 3] == '0') ||
			     (buf[pp - 4] == '1' && buf[pp - 3] == '9')) &&
			    buf[pp - 2] >= '0' && buf[pp - 2] <= '9' &&
			    buf[pp - 1] >= '0' && buf[pp - 1] <= '9') {
				if (pp < p - 2 && buf[pp + 1] == ' ' && buf[pp + 2] != ' ') ppos = pp + 1;
				else ppos = pp;
			}
			if (buf[pp - 6] == ' ' &&
			    ((buf[pp - 5] >= '0' && buf[pp - 5] <= '2') || buf[pp - 5] == ' ') &&
			    buf[pp - 4] >= '0' && buf[pp - 4] <= '9' &&
			    buf[pp - 3] == ':' &&
			    buf[pp - 2] >= '0' && buf[pp - 2] <= '5' &&
			    buf[pp - 1] >= '0' && buf[pp - 1] <= '9') {
				ppos = pp;
				if (pp + 2 < p && buf[pp + 1] == ' ' && buf[pp + 2] != ' ')
					ppos++;
			}
		}
		if (ppos != -1) {
			pp = ppos;
			goto done;
		}

		for (pp = 0; pp + 5 <= p; pp++)
			if (!casecmp(&buf[pp], cast_uchar "<DIR>", 5)) {
				pp += 4;
				while (pp + 1 < p && WHITECHAR(buf[pp + 1])) pp++;
				if (pp + 1 < p) goto done;
			}

		bn = -1;
		bp = 0;		/* warning, go away */
		for (pp = 0; pp < p; ) {
			if (buf[pp] >= '0' && buf[pp] <= '9') {
				int i;
				for (i = pp; i < p; i++)
					if (buf[i] < '0' || buf[i] > '9') break;
				if (i < p && WHITECHAR(buf[i])) {
					if (i - pp > bn) {
						bn = i - pp;
						bp = pp;
					}
				}
				pp = i;
			}
			while (pp < p && !WHITECHAR(buf[pp])) pp++;
			while (pp < p && WHITECHAR(buf[pp])) pp++;
		}
		if (bn >= 0) {
			pp = bp + bn;
			while (pp + 1 < p && WHITECHAR(buf[pp + 1])) pp++;
			if (pp + 1 < p) goto done;
		}

		for (pp = p - 1; pp >= 0; pp--) if (!WHITECHAR(buf[pp])) break;
		if (pp < 0) goto raw;
		for (; pp >= 0; pp--) if (WHITECHAR(buf[pp]) && (pp < 3 || memcmp(buf + pp - 3, cast_uchar " -> ", 4)) && (pp > p - 4 || memcmp(buf + pp, cast_uchar " -> ", 4))) break;
		done:
		sp = *d = pp + 1;
		goto ppp;
		raw:
		add_conv_str(&str, &sl, buf, p, 0);
	}
	add_chr_to_str(&str, &sl, '\n');
	a = add_fragment(ce, *pos, str, sl);
	if (a < 0) return a;
	if (a == 1) *tr = 0;
	*pos += sl;
	mem_free(str);
	goto again;
}

static void created_data_connection(struct connection *c)
{
	struct ftp_connection_info *inf = c->info;
#ifdef HAVE_IPTOS
	if (ftp_options.set_tos) {
		int rx;
		int on = IPTOS_THROUGHPUT;
		EINTRLOOP(rx, setsockopt(c->sock2, IPPROTO_IP, IP_TOS, (char *)&on, sizeof(int)));
	}
#endif
	inf->d = 1;
	set_handlers(c->sock2, (void (*)(void *))got_something_from_data_connection, NULL, c);
}

static void got_something_from_data_connection(struct connection *c)
{
	struct ftp_connection_info *inf = c->info;
	int l;
	int m;
	int rs;
	set_connection_timeout(c);
	if (!inf->d) {
		int ns;
		inf->d = 1;
		set_handlers(c->sock2, NULL, NULL, NULL);
		ns = c_accept(c->sock2, NULL, NULL);
		if (ns == -1) goto e;
		set_nonblock(ns);
		EINTRLOOP(rs, close(c->sock2));
		c->sock2 = ns;
		set_handlers(ns, (void (*)(void *))got_something_from_data_connection, NULL, c);
		return;
	}
	if (!c->cache) {
		if (get_cache_entry(c->url, &c->cache)) {
			setcstate(c, S_OUT_OF_MEM);
			abort_connection(c);
			return;
		}
		c->cache->refcount--;
	}
	if (inf->dir && !c->from) {
		unsigned char *ud;
		unsigned char *s0;
		int s0l;
		int err = 0;
		static unsigned char ftp_head[] = "<html><head><title>/";
		static unsigned char ftp_head2[] = "</title></head><body><h2>Directory /";
		static unsigned char ftp_head3[] = "</h2><pre>";
#define A(s)							\
do {								\
	m = add_fragment(c->cache, c->from, s, strlen(cast_const_char s));	\
	if (m < 0 && !err) err = m;				\
	c->from += strlen(cast_const_char s);					\
} while (0)
		A(ftp_head);
		ud = stracpy(get_url_data(c->url));
		if (strchr(cast_const_char ud, POST_CHAR)) *strchr(cast_const_char ud, POST_CHAR) = 0;
		s0 = init_str();
		s0l = 0;
		add_conv_str(&s0, &s0l, ud, (int)strlen(cast_const_char ud), -1);
		mem_free(ud);
		A(s0);
		A(ftp_head2);
		A(s0);
		A(ftp_head3);
		mem_free(s0);
		if (!c->cache->head) c->cache->head = stracpy(cast_uchar "\r\n");
		add_to_strn(&c->cache->head, cast_uchar "Content-Type: text/html\r\n");
		if (err) {
			setcstate(c, err);
			abort_connection(c);
			return;
		}
#undef A
	}
	EINTRLOOP(l, (int)read(c->sock2, inf->ftp_buffer + inf->buf_pos, FTP_BUF - inf->buf_pos));
	if (l == -1) {
		e:
		if (inf->conn_st != 1 && !inf->dir && !c->from) {
			close_socket(&c->sock2);
			inf->conn_st = 2;
			return;
		}
		setcstate(c, get_error_from_errno(errno));
		retry_connection(c);
		return;
	}
	if (l > 0) {
		if (!inf->dir) {
			if ((off_t)(0UL + c->from + l) < 0) {
				setcstate(c, S_LARGE_FILE);
				abort_connection(c);
				return;
			}
			c->received += l;
			m = add_fragment(c->cache, c->from, inf->ftp_buffer, l);
			if (m < 0) {
				setcstate(c, m);
				abort_connection(c);
				return;
			}
			if (m == 1) c->tries = 0;
			c->from += l;
		} else {
			c->received += l;
			m = ftp_process_dirlist(c->cache, &c->from, &inf->dpos, inf->ftp_buffer, l + inf->buf_pos, 0, inf->we_are_in_root, &c->tries);
			if (m < 0) {
				setcstate(c, m);
				abort_connection(c);
				return;
			}
			memmove(inf->ftp_buffer, inf->ftp_buffer + m, inf->buf_pos + l - m);
			inf->buf_pos += l - m;
		}
		setcstate(c, S_TRANS);
		return;
	}
	m = ftp_process_dirlist(c->cache, &c->from, &inf->dpos, inf->ftp_buffer, inf->buf_pos, 1, inf->we_are_in_root, &c->tries);
	if (m < 0) {
		setcstate(c, m);
		abort_connection(c);
		return;
	}
	close_socket(&c->sock2);
	if (inf->conn_st == 1) {
		ftp_end_request(c, S__OK);
	} else {
		inf->conn_st = 2;
	}
}

static void ftp_end_request(struct connection *c, int state)
{
	struct ftp_connection_info *inf = c->info;
	if (state == S__OK) {
		if (c->cache) {
			truncate_entry(c->cache, c->from, 1);
			c->cache->incomplete = 0;
		}
	}
	setcstate(c, state);
	add_keepalive_socket(c, FTP_KEEPALIVE_TIMEOUT, inf->we_are_in_root);
}

