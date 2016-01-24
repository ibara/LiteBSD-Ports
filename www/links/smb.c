#include "links.h"

#ifndef DISABLE_SMB

#define SMBCLIENT	0
#define SMBC		1
#define N_CLIENTS	2

static int smb_client = 0;

#define CLIENT_NOT_FOUND_STRING	"client not found"

struct smb_connection_info {
	int client;
	int list;
	int cl;
	int ntext;
	unsigned char text[1];
};

static void smb_got_data(struct connection *);
static void smb_got_text(struct connection *);
static void end_smb_connection(struct connection *);

void smb_func(struct connection *c)
{
	int i;
	int po[2];
	int pe[2];
	unsigned char *host, *user, *pass, *port, *data1, *data, *share, *dir;
	int datal;
	unsigned char *p;
	pid_t r;
	int rs;
	struct smb_connection_info *si;
	si = mem_alloc(sizeof(struct smb_connection_info) + 2);
	memset(si, 0, sizeof(struct smb_connection_info));
	c->info = si;
	si->client = smb_client;
	host = get_host_name(c->url);
	if (!host) {
		setcstate(c, S_INTERNAL);
		abort_connection(c);
		return;
	}
	if (!(user = get_user_name(c->url))) user = stracpy(cast_uchar "");
	if (!(pass = get_pass(c->url))) pass = stracpy(cast_uchar "");
	if (!(port = get_port_str(c->url))) port = stracpy(cast_uchar "");
	if (!(data1 = get_url_data(c->url))) data1 = cast_uchar "";
	data = init_str(), datal = 0;
	add_conv_str(&data, &datal, data1, (int)strlen(cast_const_char data1), -2);

	for (i = 0; data[i]; i++) if (data[i] < 32 || data[i] == ';' || (data[i] == '"' && smb_client == SMBCLIENT)) {
/* ';' shouldn't cause security problems but samba doesn't like it */
/* '"' is allowed for smbc */
		mem_free(host);
		mem_free(port);
		mem_free(user);
		mem_free(pass);
		mem_free(data);
		setcstate(c, S_BAD_URL);
		abort_connection(c);
		return;
	}

	if ((p = cast_uchar strchr(cast_const_char data, '/'))) share = memacpy(data, p - data), dir = p + 1;
	else if (*data) {
		if (!c->cache) {
			if (get_cache_entry(c->url, &c->cache)) {
				mem_free(host);
				mem_free(port);
				mem_free(user);
				mem_free(pass);
				mem_free(data);
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
		mem_free(host);
		mem_free(port);
		mem_free(user);
		mem_free(pass);
		mem_free(data);
		setcstate(c, S__OK);
		abort_connection(c);
		return;
	} else share = stracpy(cast_uchar ""), dir = cast_uchar "";
	if (!*share) si->list = 1;
	else if (!*dir || dir[strlen(cast_const_char dir) - 1] == '/' || dir[strlen(cast_const_char dir) - 1] == '\\') si->list = 2;
	if (c_pipe(po)) {
		int err = errno;
		mem_free(host);
		mem_free(port);
		mem_free(user);
		mem_free(pass);
		mem_free(share);
		mem_free(data);
		setcstate(c, get_error_from_errno(err));
		abort_connection(c);
		return;
	}
	if (c_pipe(pe)) {
		int err = errno;
		mem_free(host);
		mem_free(port);
		mem_free(user);
		mem_free(pass);
		mem_free(share);
		mem_free(data);
		EINTRLOOP(rs, close(po[0]));
		EINTRLOOP(rs, close(po[1]));
		setcstate(c, get_error_from_errno(err));
		abort_connection(c);
		return;
	}
	c->from = 0;
	EINTRLOOP(r, fork());
	if (r == -1) {
		int err = errno;
		mem_free(host);
		mem_free(port);
		mem_free(user);
		mem_free(pass);
		mem_free(share);
		mem_free(data);
		EINTRLOOP(rs, close(po[0]));
		EINTRLOOP(rs, close(po[1]));
		EINTRLOOP(rs, close(pe[0]));
		EINTRLOOP(rs, close(pe[1]));
		setcstate(c, get_error_from_errno(err));
		retry_connection(c);
		return;
	}
	if (!r) {
		int n;
		unsigned char *v[32];
		unsigned char *uphp;
		close_fork_tty();
		EINTRLOOP(rs, close(1));
		if (si->list)
			EINTRLOOP(rs, dup2(pe[1], 1));
		else
			EINTRLOOP(rs, dup2(po[1], 1));
		EINTRLOOP(rs, close(2));
		EINTRLOOP(rs, dup2(pe[1], 2));
		EINTRLOOP(rs, close(0));
		EINTRLOOP(rs, open("/dev/null", O_RDONLY));
		EINTRLOOP(rs, close(po[0]));
		EINTRLOOP(rs, close(pe[0]));
		EINTRLOOP(rs, close(po[1]));
		EINTRLOOP(rs, close(pe[1]));
		n = 0;
		switch (si->client) {
		case SMBCLIENT:
			v[n++] = cast_uchar "smbclient";
			if (!*share) {
				v[n++] = cast_uchar "-L";
				v[n++] = host;
			} else {
				unsigned char *s = stracpy(cast_uchar "//");
				add_to_strn(&s, host);
				add_to_strn(&s, cast_uchar "/");
				add_to_strn(&s, share);
				v[n++] = s;
				if (*pass && !*user) {
					v[n++] = pass;
				}
			}
			v[n++] = cast_uchar "-N";
			v[n++] = cast_uchar "-E";
			if (*port) {
				v[n++] = cast_uchar "-p";
				v[n++] = port;
			}
			if (*user) {
				v[n++] = cast_uchar "-U";
				if (!*pass) {
					v[n++] = user;
				} else {
					unsigned char *s = stracpy(user);
					add_to_strn(&s, cast_uchar "%");
					add_to_strn(&s, pass);
					v[n++] = s;
				}
			}
			if (*share) {
				if (!*dir || dir[strlen(cast_const_char dir) - 1] == '/' || dir[strlen(cast_const_char dir) - 1] == '\\') {
					if (*dir) {
						v[n++] = cast_uchar "-D";
						v[n++] = dir;
					}
					v[n++] = cast_uchar "-c";
					v[n++] = cast_uchar "ls";
				} else {
					unsigned char *ss;
					unsigned char *s = stracpy(cast_uchar "get \"");
					add_to_strn(&s, dir);
					add_to_strn(&s, cast_uchar "\" -");
					while ((ss = cast_uchar strchr(cast_const_char s, '/'))) *ss = '\\';
					v[n++] = cast_uchar "-c";
					v[n++] = s;
				}
			}
			break;
		case SMBC:
			v[n++] = cast_uchar "smbc";
			uphp = stracpy(cast_uchar "");
			if (*user) {
				add_to_strn(&uphp, user);
				if (*pass) {
					add_to_strn(&uphp, cast_uchar ":");
					add_to_strn(&uphp, pass);
				}
				add_to_strn(&uphp, cast_uchar "@");
			}
			add_to_strn(&uphp, host);
			if (*port) {
				add_to_strn(&uphp, cast_uchar ":");
				add_to_strn(&uphp, port);
			}
			if (!*share) {
				v[n++] = cast_uchar "-L";
				v[n++] = uphp;
			} else {
				add_to_strn(&uphp, cast_uchar "/");
				add_to_strn(&uphp, share);
				if (!*dir || dir[strlen(cast_const_char dir) - 1] == '/' || dir[strlen(cast_const_char dir) - 1] == '\\') {
					add_to_strn(&uphp, cast_uchar "/");
					add_to_strn(&uphp, dir);
					v[n++] = uphp;
					v[n++] = cast_uchar "-c";
					v[n++] = cast_uchar "ls";
				} else {
					unsigned char *d = init_str();
					int dl = 0;
					unsigned char *dp = dir;
					v[n++] = uphp;
					v[n++] = cast_uchar "-c";
					add_to_str(&d, &dl, cast_uchar "pipe cat ");
					while (*dp) {
						if (*dp <= ' ' || *dp == '\\' || *dp == '"' || *dp == '\'' || *dp == '*' || *dp == '?') add_chr_to_str(&d, &dl, '\\');
						add_chr_to_str(&d, &dl, *dp);
						dp++;
					}
					v[n++] = d;
				}
			}
			break;
		default:
			internal("unsuported smb client");
		}
		v[n++] = NULL;
		EINTRLOOP(rs, execvp(cast_const_char v[0], (void *)v));
		hard_write(2, cast_uchar CLIENT_NOT_FOUND_STRING, (int)strlen(CLIENT_NOT_FOUND_STRING));
		_exit(1);
	}
	c->pid = r;
	mem_free(host);
	mem_free(port);
	mem_free(user);
	mem_free(pass);
	mem_free(share);
	mem_free(data);
	c->sock1 = po[0];
	c->sock2 = pe[0];
	EINTRLOOP(rs, close(po[1]));
	EINTRLOOP(rs, close(pe[1]));
	set_handlers(po[0], (void (*)(void *))smb_got_data, NULL, c);
	set_handlers(pe[0], (void (*)(void *))smb_got_text, NULL, c);
	setcstate(c, S_CONN);
}

static int smbc_get_num(unsigned char *text, int *ptr, off_t *res)
{
	off_t num;
	int dec, dec_order, unit;
	int was_digit;
	int i = *ptr;
	const off_t max_off_t = (((off_t)1 << ((sizeof(off_t) * 8 - 2))) - 1) * 2 + 1;

	while (text[i] == ' ' || text[i] == '\t') i++;
	was_digit = 0;
	num = 0;
	while (text[i] >= '0' && text[i] <= '9') {
		if (num >= max_off_t / 10) return -1;
		num = num * 10 + text[i] - '0';
		i++;
		was_digit = 1;
	}
	dec = 0; dec_order = 1;
	if (text[i] == '.') {
		i++;
		while (text[i] >= '0' && text[i] <= '9') {
			if (dec_order < 1000000) {
				dec = dec * 10 + text[i] - '0';
				dec_order *= 10;
			}
			i++;
			was_digit = 1;
		}
	}
	if (!was_digit) return -1;
	if (upcase(text[i]) == 'B') unit = 1;
	else if (upcase(text[i]) == 'K') unit = 1 << 10;
	else if (upcase(text[i]) == 'M') unit = 1 << 20;
	else if (upcase(text[i]) == 'G') unit = 1 << 30;
	else return -1;
	i++;
	*ptr = i;
	if (num >= max_off_t / unit) return -1;
	*res = num * unit + (off_t)((double)dec * ((double)unit / (double)dec_order));
	return 0;
}

static void smb_read_text(struct connection *c, int sock)
{
	int r;
	struct smb_connection_info *si = c->info;
	if ((unsigned)sizeof(struct smb_connection_info) + si->ntext + page_size + 2 > MAXINT) overalloc();
	si = mem_realloc(si, sizeof(struct smb_connection_info) + si->ntext + page_size + 2);
	c->info = si;
	EINTRLOOP(r, (int)read(sock, si->text + si->ntext, page_size));
	if (r == -1) {
		setcstate(c, get_error_from_errno(errno));
		retry_connection(c);
		return;
	}
	if (r == 0) {
		if (!si->cl) {
			si->cl = 1;
			set_handlers(sock, NULL, NULL, NULL);
			return;
		}
		end_smb_connection(c);
		return;
	}
	si->ntext += r;
	if (!c->from) setcstate(c, S_GETH);
	if (c->from && si->client == SMBC) {
		int lasti = 0;
		int i = 0;
		si->text[si->ntext] = 0;
		for (i = 0; i + 7 < si->ntext; i++) {
			nexti:
			if ((si->text[i] == '\n' || si->text[i] == '\r') && (si->text[i + 1] == ' ' || (si->text[i + 1] >= '0' && si->text[i + 1] <= '9')) && ((si->text[i + 2] == ' ' && si->text[i + 1] == ' ') || (si->text[i + 2] >= '0' && si->text[i + 2] <= '9')) && (si->text[i + 3] >= '0' && si->text[i + 3] <= '9') && si->text[i + 4] == '%' && si->text[i + 5] == ' ' && si->text[i + 6] == '[') {
				off_t position, total = 0; /* against warning */
				i += 7;
				while (si->text[i] != ']') {
					if (!si->text[i] || si->text[i] == '\n' || si->text[i] == '\r') {
						goto nexti;
					}
					i++;
				}
				i++;
				if (smbc_get_num(si->text, &i, &position)) {
					goto nexti;
				}
				while (si->text[i] == ' ' || si->text[i] == '\t') i++;
				if (si->text[i] != '/') {
					goto nexti;
				}
				i++;
				if (smbc_get_num(si->text, &i, &total)) {
					goto nexti;
				}
				if (total < c->from) total = c->from;
				c->est_length = total;
				lasti = i;
			}
		}
		if (lasti) memmove(si->text, si->text + lasti, si->ntext -= lasti);
	}
}

static void smb_got_data(struct connection *c)
{
	struct smb_connection_info *si = c->info;
	unsigned char *buffer = mem_alloc(page_size);
	int r;
	int a;
	if (si->list) {
		smb_read_text(c, c->sock1);
		mem_free(buffer);
		return;
	}
	EINTRLOOP(r, (int)read(c->sock1, buffer, page_size));
	if (r == -1) {
		setcstate(c, get_error_from_errno(errno));
		retry_connection(c);
		mem_free(buffer);
		return;
	}
	if (r == 0) {
		mem_free(buffer);
		if (!si->cl) {
			si->cl = 1;
			set_handlers(c->sock1, NULL, NULL, NULL);
			return;
		}
		end_smb_connection(c);
		return;
	}
	setcstate(c, S_TRANS);
	if (!c->cache) {
		if (get_cache_entry(c->url, &c->cache)) {
			setcstate(c, S_OUT_OF_MEM);
			abort_connection(c);
			mem_free(buffer);
			return;
		}
		c->cache->refcount--;
	}
	if ((off_t)(0UL + c->from + r) < 0) {
		setcstate(c, S_LARGE_FILE);
		abort_connection(c);
		mem_free(buffer);
		return;
	}
	c->received += r;
	a = add_fragment(c->cache, c->from, buffer, r);
	if (a < 0) {
		setcstate(c, a);
		abort_connection(c);
		mem_free(buffer);
		return;
	}
	if (a == 1) c->tries = 0;
	c->from += r;
	mem_free(buffer);
}

static void smb_got_text(struct connection *c)
{
	smb_read_text(c, c->sock2);
}

static void end_smb_connection(struct connection *c)
{
	struct smb_connection_info *si = c->info;
	if (!c->cache) {
		if (get_cache_entry(c->url, &c->cache)) {
			setcstate(c, S_OUT_OF_MEM);
			abort_connection(c);
			return;
		}
		c->cache->refcount--;
	}
	if (!c->from) {
		int sdir;
		if (si->ntext && si->text[si->ntext - 1] != '\n') si->text[si->ntext++] = '\n';
		si->text[si->ntext] = 0;
		if (!strcmp(cast_const_char si->text, CLIENT_NOT_FOUND_STRING "\n")) {
			setcstate(c, S_NO_SMB_CLIENT);
			if (++si->client < N_CLIENTS) {
				if (si->client > smb_client) smb_client = si->client;
				c->tries = -1;
				retry_connection(c);
			} else {
				smb_client = 0;
				abort_connection(c);
			}
			return;
		}
		sdir = 0;
		if (si->client == SMBC) {
			unsigned char *st = si->text;
			if (!memcmp(st, "ServerName", 10) && strchr(cast_const_char st, '\n')) st = cast_uchar strchr(cast_const_char st, '\n') + 1;
			if (!memcmp(st, "Logged", 6) && strchr(cast_const_char st, '\n')) st = cast_uchar strchr(cast_const_char st, '\n') + 1;
			if (!strstr(cast_const_char st, "ERR")) sdir = 1;
		}
		if (!si->list && *c->url &&
		    c->url[strlen(cast_const_char c->url) - 1] != '/' &&
		    c->url[strlen(cast_const_char c->url) - 1] != '\\' &&
		    (strstr(cast_const_char si->text, "NT_STATUS_FILE_IS_A_DIRECTORY") ||
		     strstr(cast_const_char si->text, "NT_STATUS_ACCESS_DENIED") ||
		     strstr(cast_const_char si->text, "ERRbadfile") || sdir)) {
			if (c->cache->redirect) mem_free(c->cache->redirect);
			c->cache->redirect = stracpy(c->url);
			c->cache->redirect_get = 1;
			add_to_strn(&c->cache->redirect, cast_uchar "/");
			c->cache->incomplete = 0;
		} else {
			unsigned char *ls, *le, *le2;
			unsigned char *ud;
			unsigned char *t = init_str();
			int l = 0;
			int type = 0;
			int pos = 0;
			int a;
			add_to_str(&t, &l, cast_uchar "<html><head><title>");
			ud = stracpy(c->url);
			if (strchr(cast_const_char ud, POST_CHAR)) *cast_uchar strchr(cast_const_char ud, POST_CHAR) = 0;
			add_conv_str(&t, &l, ud, (int)strlen(cast_const_char ud), -1);
			mem_free(ud);
			add_to_str(&t, &l, cast_uchar "</title></head><body><pre>");
			if (si->list == 1 && si->client == SMBC) {
/* smbc has a nasty bug that it writes field descriptions to stderr and data to
   stdout. Because of stdout buffer, they'll get mixed in the output. Try to
   demix them. */
#define SERVER	"Server              Comment\n------              -------\n"
#define WORKGR	"Workgroup           Master\n---------           ------\n"
				unsigned char *spos = cast_uchar strstr(cast_const_char si->text, SERVER);
				unsigned char *gpos;
				unsigned char *p, *pp, *ppp;
				if (spos) memmove(spos, spos + strlen(SERVER), strlen(cast_const_char spos) - strlen(SERVER) + 1);
				gpos = cast_uchar strstr(cast_const_char si->text, WORKGR);
				if (gpos) memmove(gpos, gpos + strlen(WORKGR), strlen(cast_const_char gpos) - strlen(WORKGR) + 1);
				if (!spos && !gpos) goto sc;
				pp = NULL, ppp = NULL, p = si->text;
				while ((p = cast_uchar strstr(cast_const_char p, "\n\n"))) ppp = pp, pp = p + 2, p++;
				if (!pp) goto sc;
				if (!spos || !gpos) ppp = NULL;
				if (spos) {
					if (!ppp) ppp = pp, pp = NULL;
					memmove(ppp + strlen(SERVER), ppp, strlen(cast_const_char ppp) + 1);
					memcpy(ppp, SERVER, strlen(SERVER));
					if (pp) pp += strlen(SERVER);
				}
				if (gpos && pp) {
					memmove(pp + strlen(WORKGR), pp, strlen(cast_const_char pp) + 1);
					memcpy(pp, WORKGR, strlen(WORKGR));
				}
				goto sc;
			}
			sc:
			ls = si->text;
			while ((le = cast_uchar strchr(cast_const_char ls, '\n'))) {
				unsigned char *lx;
				unsigned char *st;
				le2 = cast_uchar strchr(cast_const_char ls, '\r');
				if (!le2 || le2 > le) le2 = le;
				lx = memacpy(ls, le2 - ls);
				if (si->list == 1) {
					unsigned char *ll, *lll;
					if (!*lx) type = 0;
					if (strstr(cast_const_char lx, "Sharename") && (st = cast_uchar strstr(cast_const_char lx, "Type"))) {
						pos = (int)(st - lx);
						type = 1;
						goto af;
					}
					if (strstr(cast_const_char lx, "Server") && strstr(cast_const_char lx, "Comment")) {
						type = 2;
						goto af;
					}
					if (strstr(cast_const_char lx, "Workgroup") && (st = cast_uchar strstr(cast_const_char lx, "Master"))) {
						pos = (int)(st - lx);
						type = 3;
						goto af;
					}
					if (!type) goto af;
					for (ll = lx; *ll; ll++) if (!WHITECHAR(*ll) && *ll != '-') goto np;
					goto af;
					np:
					for (ll = lx; *ll; ll++) if (!WHITECHAR(*ll)) break;
					for (lll = ll; *lll /* && lll[1]*/; lll++) if (WHITECHAR(*lll) /*&& WHITECHAR(lll[1])*/) break;
					if (type == 1) {
						unsigned char *llll;
						if (!strstr(cast_const_char lll, "Disk")) goto af;
						if (pos && (size_t)pos < strlen(cast_const_char lx) && WHITECHAR(*(llll = lx + pos - 1)) && llll > ll) {
							while (llll > ll && WHITECHAR(*llll)) llll--;
							if (!WHITECHAR(*llll)) lll = llll + 1;
						}
						add_conv_str(&t, &l, lx, (int)(ll - lx), 0);
						add_to_str(&t, &l, cast_uchar "<a href=\"/");
						add_conv_str(&t, &l, ll, (int)(lll - ll), 1);
						add_to_str(&t, &l, cast_uchar "/\">");
						add_conv_str(&t, &l, ll, (int)(lll - ll), 0);
						add_to_str(&t, &l, cast_uchar "</a>");
						add_conv_str(&t, &l, lll, (int)strlen(cast_const_char lll), 0);
					} else if (type == 2) {
						sss:
						add_conv_str(&t, &l, lx, (int)(ll - lx), 0);
						add_to_str(&t, &l, cast_uchar "<a href=\"smb://");
						add_conv_str(&t, &l, ll, (int)(lll - ll), 1);
						add_to_str(&t, &l, cast_uchar "/\">");
						add_conv_str(&t, &l, ll, (int)(lll - ll), 0);
						add_to_str(&t, &l, cast_uchar "</a>");
						add_conv_str(&t, &l, lll, (int)strlen(cast_const_char lll), 0);
					} else if (type == 3) {
						if ((size_t)pos < strlen(cast_const_char lx) && pos && WHITECHAR(lx[pos - 1]) && !WHITECHAR(lx[pos])) ll = lx + pos;
						else for (ll = lll; *ll; ll++) if (!WHITECHAR(*ll)) break;
						for (lll = ll; *lll; lll++) if (WHITECHAR(*lll)) break;
						goto sss;
					} else goto af;
				} else if (si->list == 2 && si->client == SMBCLIENT) {
					if (strstr(cast_const_char lx, "NT_STATUS")) {
						le[1] = 0;
						goto af;
					}
					if (le2 - ls >= 5 && ls[0] == ' ' && ls[1] == ' ' && ls[2] != ' ') {
						int dir;
						unsigned char *pp;
						unsigned char *p = ls + 3;
						while (p + 2 <= le2) {
							if (p[0] == ' ' && p[1] == ' ') goto o;
							p++;
						}
						goto af;
						o:
						dir = 0;
						pp = p;
						while (pp < le2 && *pp == ' ') pp++;
						while (pp < le2 && *pp != ' ') {
							if (*pp == 'D') {
								dir = 1;
								break;
							}
							pp++;
						}
						add_to_str(&t, &l, cast_uchar "  <a href=\"./");
						add_conv_str(&t, &l, ls + 2, (int)(p - (ls + 2)), 1);
						if (dir) add_chr_to_str(&t, &l, '/');
						add_to_str(&t, &l, cast_uchar "\">");
						add_conv_str(&t, &l, ls + 2, (int)(p - (ls + 2)), 0);
						add_to_str(&t, &l, cast_uchar "</a>");
						add_conv_str(&t, &l, p, (int)(le - p), 0);
					} else goto af;
				} else if (si->list == 2 && si->client == SMBC) {
					unsigned char *d;
					if (le2 - ls <= 17) goto af;
					d = ls + 17;
					smbc_next_chr:
					if (d + 9 >= le2) goto af;
					if (!(d[0] == ':' && d[1] >= '0' && d[1] <= '9' && d[2] >= '0' && d[2] <= '9' && d[3] == ' ' && ((d[4] == '1' && d[5] == '9') || (d[4] == '2' && d[5] >= '0' && d[5] <= '9')) && d[6] >= '0' && d[6] <= '9' && d[7] >= '0' && d[7] <= '9' && d[8] == ' ')) {
						d++;
						goto smbc_next_chr;
					}
					d += 9;
					add_conv_str(&t, &l, ls, (int)(d - ls), 0);
					add_to_str(&t, &l, cast_uchar "<a href=\"./");
					add_conv_str(&t, &l, d, (int)(le2 - d), 1);
					if (ls[4] == 'D') add_chr_to_str(&t, &l, '/');
					add_to_str(&t, &l, cast_uchar "\">");
					add_conv_str(&t, &l, d, (int)(le2 - d), 0);
					add_to_str(&t, &l, cast_uchar "</a>");
				} else af: add_conv_str(&t, &l, ls, (int)(le2 - ls), 0);
				add_chr_to_str(&t, &l, '\n');
				ls = le + 1;
				mem_free(lx);
			}
			/*add_to_str(&t, &l, si->text);*/
			a = add_fragment(c->cache, 0, t, l);
			if (a < 0) {
				mem_free(t);
				setcstate(c, a);
				abort_connection(c);
				return;
			}
			c->from += l;
			truncate_entry(c->cache, l, 1);
			c->cache->incomplete = 0;
			mem_free(t);
			if (!c->cache->head) c->cache->head = stracpy(cast_uchar "\r\n");
			add_to_strn(&c->cache->head, cast_uchar "Content-Type: text/html\r\n");
		}
	} else {
		truncate_entry(c->cache, c->from, 1);
		c->cache->incomplete = 0;
	}
	close_socket(&c->sock1);
	close_socket(&c->sock2);
	setcstate(c, S__OK);
	abort_connection(c);
	return;
}

#endif
