/* file.c
 * (c) 2002 Mikulas Patocka
 * This file is a part of the Links program, released under GPL.
 */

#include "links.h"

#ifdef FS_UNIX_RIGHTS
static void setrwx(int m, unsigned char *p)
{
	if (m & S_IRUSR) p[0] = 'r';
	if (m & S_IWUSR) p[1] = 'w';
	if (m & S_IXUSR) p[2] = 'x';
}

static void setst(int m, unsigned char *p)
{
#ifdef S_ISUID
	if (m & S_ISUID) {
		p[2] = 'S';
		if (m & S_IXUSR) p[2] = 's';
	}
#endif
#ifdef S_ISGID
	if (m & S_ISGID) {
		p[5] = 'S';
		if (m & S_IXGRP) p[5] = 's';
	}
#endif
#ifdef S_ISVTX
	if (m & S_ISVTX) {
		p[8] = 'T';
		if (m & S_IXOTH) p[8] = 't';
	}
#endif
}
#endif

static void stat_mode(unsigned char **p, int *l, struct stat *stp)
{
	unsigned char c = '?';
	if (stp) {
		if (0) {}
#ifdef S_ISBLK
		else if (S_ISBLK(stp->st_mode)) c = 'b';
#endif
#ifdef S_ISCHR
		else if (S_ISCHR(stp->st_mode)) c = 'c';
#endif
		else if (S_ISDIR(stp->st_mode)) c = 'd';
		else if (S_ISREG(stp->st_mode)) c = '-';
#ifdef S_ISFIFO
		else if (S_ISFIFO(stp->st_mode)) c = 'p';
#endif
#ifdef S_ISLNK
		else if (S_ISLNK(stp->st_mode)) c = 'l';
#endif
#ifdef S_ISSOCK
		else if (S_ISSOCK(stp->st_mode)) c = 's';
#endif
#ifdef S_ISNWK
		else if (S_ISNWK(stp->st_mode)) c = 'n';
#endif
	}
	add_chr_to_str(p, l, c);
#ifdef FS_UNIX_RIGHTS
	{
		unsigned char rwx[10] = "---------";
		if (stp) {
			int mode = stp->st_mode;
			setrwx(mode << 0, &rwx[0]);
			setrwx(mode << 3, &rwx[3]);
			setrwx(mode << 6, &rwx[6]);
			setst(mode, rwx);
		}
		add_to_str(p, l, rwx);
	}
#endif
	add_chr_to_str(p, l, ' ');
}


static void stat_links(unsigned char **p, int *l, struct stat *stp)
{
#ifdef FS_UNIX_HARDLINKS
	unsigned char lnk[64];
	if (!stp) add_to_str(p, l, cast_uchar "    ");
	else {
		sprintf(cast_char lnk, "%3ld ", (unsigned long)stp->st_nlink);
		add_to_str(p, l, lnk);
	}
#endif
}

#ifdef FS_UNIX_USERS
static int last_uid = -1;
static unsigned char last_user[64];

static int last_gid = -1;
static unsigned char last_group[64];
#endif

static void stat_user(unsigned char **p, int *l, struct stat *stp, int g)
{
#ifdef FS_UNIX_USERS
	struct passwd *pwd;
	struct group *grp;
	int id;
	unsigned char *pp;
	int i;
	if (!stp) {
		add_to_str(p, l, cast_uchar "         ");
		return;
	}
	id = !g ? stp->st_uid : stp->st_gid;
	pp = !g ? last_user : last_group;
	if (!g && id == last_uid && last_uid != -1) goto a;
	if (g && id == last_gid && last_gid != -1) goto a;
	if (!g) {
		ENULLLOOP(pwd, getpwuid(id));
		if (!pwd || !pwd->pw_name) sprintf(cast_char pp, "%d", id);
		else sprintf(cast_char pp, "%.8s", pwd->pw_name);
		last_uid = id;
	} else {
		ENULLLOOP(grp, getgrgid(id));
		if (!grp || !grp->gr_name) sprintf(cast_char pp, "%d", id);
		else sprintf(cast_char pp, "%.8s", grp->gr_name);
		last_gid = id;
	}
	a:
	add_to_str(p, l, pp);
	for (i = (int)strlen(cast_const_char pp); i < 8; i++) add_chr_to_str(p, l, ' ');
	add_chr_to_str(p, l, ' ');
#endif
}

static void stat_size(unsigned char **p, int *l, struct stat *stp)
{
	unsigned char num[64];
	const int digits = 8;
	int i;
	if (!stp) {
		num[0] = 0;
	} else {
		snzprint(num, sizeof num, stp->st_size);
	}
	for (i = (int)strlen(cast_const_char num); i < digits; i++)
		add_chr_to_str(p, l, ' ');
	add_to_str(p, l, num);
	add_chr_to_str(p, l, ' ');
}

static void stat_date(unsigned char **p, int *l, struct stat *stp)
{
	time_t current_time;
	time_t when;
	struct tm *when_local;
	unsigned char *fmt;
	unsigned char str[13];
	static unsigned char fmt1[] = "%b %e  %Y";
	static unsigned char fmt2[] = "%b %e %H:%M";
	int wr;
	EINTRLOOPX(current_time, time(NULL), (time_t)-1);
	if (!stp) {
		wr = 0;
		goto set_empty;
	}
	when = stp->st_mtime;
	when_local = localtime(&when);
	if ((ulonglong)current_time > (ulonglong)when + 6L * 30L * 24L * 60L * 60L ||
	    (ulonglong)current_time < (ulonglong)when - 60L * 60L) fmt = fmt1;
	else fmt = fmt2;
#ifdef HAVE_STRFTIME
	wr = (int)strftime(cast_char str, 13, cast_const_char fmt, when_local);
#else
	wr = 0;
#endif
	set_empty:
	while (wr < 12) str[wr++] = ' ';
	str[12] = 0;
	add_to_str(p, l, str);
	add_chr_to_str(p, l, ' ');
}

static unsigned char *get_filename(unsigned char *url)
{
	unsigned char *p, *m;
	int ml;
#ifdef DOS_FS
	if (url[7] == '/' && strchr(cast_const_char(url + 8), ':')) url++;
#endif
	for (p = url + 7; *p && *p != POST_CHAR; p++)
		;
	m = init_str(), ml = 0;
	add_conv_str(&m, &ml, url + 7, (int)(p - url - 7), -2);
	return m;
}

struct dirs {
	unsigned char *s;
	unsigned char *f;
};

LIBC_CALLBACK static int comp_de(struct dirs *d1, struct dirs *d2)
{
	if (d1->f[0] == '.' && d1->f[1] == '.' && !d1->f[2]) return -1;
	if (d2->f[0] == '.' && d2->f[1] == '.' && !d2->f[2]) return 1;
	if (d1->s[0] == 'd' && d2->s[0] != 'd') return -1;
	if (d1->s[0] != 'd' && d2->s[0] == 'd') return 1;
	return strcmp(cast_const_char d1->f, cast_const_char d2->f);
}

void file_func(struct connection *c)
{
	struct cache_entry *e;
	unsigned char *file, *name, *head = NULL; /* against warning */
	int fl;
	DIR *d;
	int h, r;
	struct stat stt;
	int rs;
	if (anonymous) {
		setcstate(c, S_BAD_URL);
		abort_connection(c);
		return;
	}
	if (!(name = get_filename(c->url))) {
		setcstate(c, S_OUT_OF_MEM); abort_connection(c); return;
	}
	EINTRLOOP(rs, stat(cast_const_char name, &stt));
	if (rs) {
		mem_free(name);
		setcstate(c, get_error_from_errno(errno)); abort_connection(c); return;
	}
	if (!S_ISDIR(stt.st_mode) && !S_ISREG(stt.st_mode)) {
		mem_free(name);
		setcstate(c, S_FILE_TYPE); abort_connection(c); return;
	}
	h = c_open(name, O_RDONLY | O_NOCTTY);
	if (h == -1) {
		int er = errno;
		d = c_opendir(name);
		if (d) goto dir;
		mem_free(name);
		setcstate(c, get_error_from_errno(er));
		abort_connection(c);
		return;
	}
	if (S_ISDIR(stt.st_mode)) {
		struct dirs *dir;
		int dirl;
		int i;
		int er;
		struct dirent *de;
		d = c_opendir(name);
		er = errno;
		EINTRLOOP(rs, close(h));
		if (!d) {
			mem_free(name);
			setcstate(c, get_error_from_errno(er)); abort_connection(c); return;
		}
		dir:
		dir = DUMMY, dirl = 0;
		if (name[0] && !dir_sep(name[strlen(cast_const_char name) - 1])) {
			if (!c->cache) {
				if (get_cache_entry(c->url, &c->cache)) {
					mem_free(name);
					closedir(d);
					setcstate(c, S_OUT_OF_MEM); abort_connection(c); return;
				}
				c->cache->refcount--;
			}
			e = c->cache;
			if (e->redirect) mem_free(e->redirect);
			e->redirect = stracpy(c->url);
			e->redirect_get = 1;
			add_to_strn(&e->redirect, cast_uchar "/");
			mem_free(name);
			closedir(d);
			goto end;
		}
#ifdef FS_UNIX_USERS
		last_uid = -1;
		last_gid = -1;
#endif
		file = init_str();
		fl = 0;
		add_to_str(&file, &fl, cast_uchar "<html><head><title>");
		add_conv_str(&file, &fl, name, (int)strlen(cast_const_char name), -1);
		add_to_str(&file, &fl, cast_uchar "</title></head><body><h2>Directory ");
		add_conv_str(&file, &fl, name, (int)strlen(cast_const_char name), -1);
		add_to_str(&file, &fl, cast_uchar "</h2>\n<pre>");
		while (1) {
			struct stat stt, *stp;
			unsigned char **p;
			int l;
			unsigned char *n;
			ENULLLOOP(de, (void *)readdir(d));
			if (!de) break;
			if (!strcmp(cast_const_char de->d_name, ".")) continue;
			if (!strcmp(cast_const_char de->d_name, "..")) {
				unsigned char *n = name;
#if defined(DOS_FS) || defined(SPAD)
				unsigned char *nn = cast_uchar strchr(cast_const_char n, ':');
				if (nn) n = nn + 1;
#endif
				if (strspn(cast_const_char n, dir_sep('\\') ? "/\\" : "/") == strlen(cast_const_char n))
					continue;
			}
			if ((unsigned)dirl > MAXINT / sizeof(struct dirs) - 1) overalloc();
			dir = mem_realloc(dir, (dirl + 1) * sizeof(struct dirs));
			dir[dirl].f = stracpy(cast_uchar de->d_name);
			*(p = &dir[dirl++].s) = init_str();
			l = 0;
			n = stracpy(name);
			add_to_strn(&n, cast_uchar de->d_name);
#ifdef FS_UNIX_SOFTLINKS
			EINTRLOOP(rs, lstat(cast_const_char n, &stt));
#else
			EINTRLOOP(rs, stat(cast_const_char n, &stt));
#endif
			if (rs) stp = NULL;
			else stp = &stt;
			mem_free(n);
			stat_mode(p, &l, stp);
			stat_links(p, &l, stp);
			stat_user(p, &l, stp, 0);
			stat_user(p, &l, stp, 1);
			stat_size(p, &l, stp);
			stat_date(p, &l, stp);
		}
		closedir(d);
		if (dirl) qsort(dir, dirl, sizeof(struct dirs), (int(*)(const void *, const void *))comp_de);
		for (i = 0; i < dirl; i++) {
			unsigned char *lnk = NULL;
#ifdef FS_UNIX_SOFTLINKS
			if (dir[i].s[0] == 'l') {
				unsigned char *buf = NULL;
				int size = 0;
				int r;
				unsigned char *n = stracpy(name);
				add_to_strn(&n, dir[i].f);
				do {
					if (buf) mem_free(buf);
					size += ALLOC_GR;
					if ((unsigned)size > MAXINT) overalloc();
					buf = mem_alloc(size);
					EINTRLOOP(r, (int)readlink(cast_const_char n, cast_char buf, size));
				} while (r == size);
				if (r == -1) goto yyy;
				buf[r] = 0;
				lnk = buf;
				goto xxx;
				yyy:
				mem_free(buf);
				xxx:
				mem_free(n);
			}
#endif
			/*add_to_str(&file, &fl, cast_uchar "   ");*/
			add_to_str(&file, &fl, dir[i].s);
			add_to_str(&file, &fl, cast_uchar "<a href=\"./");
			add_conv_str(&file, &fl, dir[i].f, (int)strlen(cast_const_char dir[i].f), 1);
			if (dir[i].s[0] == 'd') add_to_str(&file, &fl, cast_uchar "/");
			else if (lnk) {
				struct stat st;
				unsigned char *n = stracpy(name);
				add_to_strn(&n, dir[i].f);
				EINTRLOOP(rs, stat(cast_const_char n, &st));
				if (!rs) if (S_ISDIR(st.st_mode)) add_to_str(&file, &fl, cast_uchar "/");
				mem_free(n);
			}
			add_to_str(&file, &fl, cast_uchar "\">");
			/*if (dir[i].s[0] == 'd') add_to_str(&file, &fl, cast_uchar "<font color=\"yellow\">");*/
			add_conv_str(&file, &fl, dir[i].f, (int)strlen(cast_const_char dir[i].f), 0);
			/*if (dir[i].s[0] == 'd') add_to_str(&file, &fl, cast_uchar "</font>");*/
			add_to_str(&file, &fl, cast_uchar "</a>");
			if (lnk) {
				add_to_str(&file, &fl, cast_uchar " -> ");
				add_to_str(&file, &fl, lnk);
				mem_free(lnk);
			}
			add_to_str(&file, &fl, cast_uchar "\n");
		}
		mem_free(name);
		for (i = 0; i < dirl; i++) mem_free(dir[i].s), mem_free(dir[i].f);
		mem_free(dir);
		add_to_str(&file, &fl, cast_uchar "</pre></body></html>\n");
		head = stracpy(cast_uchar "\r\nContent-Type: text/html\r\n");
	} else {
		mem_free(name);
		if (
#ifndef __WATCOMC__
		    stt.st_size < 0 ||
#endif
		    stt.st_size > MAXINT) {
			EINTRLOOP(rs, close(h));
			setcstate(c, S_LARGE_FILE); abort_connection(c);
			return;
		}
		/* + !stt.st_size is there because of bug in Linux. Read returns
		   -EACCES when reading 0 bytes to invalid address */
		file = mem_alloc_mayfail((size_t)stt.st_size + !stt.st_size);
		if (!file) {
			EINTRLOOP(rs, close(h));
			setcstate(c, S_OUT_OF_MEM);
			abort_connection(c); return;
		}
		if ((r = hard_read(h, file, (int)stt.st_size))
#ifdef OPENVMS
			< 0
#else
			!= stt.st_size
#endif
			) {
			mem_free(file);
			EINTRLOOP(rs, close(h));
			setcstate(c, r == -1 ? get_error_from_errno(errno) : S_FILE_ERROR);
			abort_connection(c); return;
		}
		fl = r;
		EINTRLOOP(rs, close(h));
		head = stracpy(cast_uchar "");
	}
	if (!c->cache) {
		if (get_cache_entry(c->url, &c->cache)) {
			mem_free(file);
			mem_free(head);
			setcstate(c, S_OUT_OF_MEM); abort_connection(c); return;
		}
		c->cache->refcount--;
	}
	e = c->cache;
	if (e->head) mem_free(e->head);
	e->head = head;
	if ((r = add_fragment(e, 0, file, fl)) < 0) {
		mem_free(file);
		setcstate(c, r); abort_connection(c); return;
	}
	truncate_entry(e, fl, 1);
	mem_free(file);
	end:
	c->cache->incomplete = 0;
	setcstate(c, S__OK);
	abort_connection(c);
}
