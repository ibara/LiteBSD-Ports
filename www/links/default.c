/* default.c
 * (c) 2002 Mikulas Patocka, Petr 'Brain' Kulhavy
 * This file is a part of the Links program, released under GPL
 *
 * Does the configuration file.
 */

#include "links.h"

unsigned char system_name[MAX_STR_LEN];

static void get_system_name(void)
{
#ifdef OS2
	if (!os_get_system_name(system_name))
		return;
#endif
#if defined(HAVE_SYS_UTSNAME_H) && defined(HAVE_UNAME)
	{
		struct utsname name;
		int rs;
		memset(&name, 0, sizeof name);
		EINTRLOOP(rs, uname(&name));
		if (!rs) {
#ifdef OPENVMS
			unsigned char * volatile p;
#endif
			unsigned char *str = init_str();
			int l = 0;
			add_to_str(&str, &l, cast_uchar name.sysname);
			add_to_str(&str, &l, cast_uchar " ");
#ifdef OPENVMS
			add_to_str(&str, &l, cast_uchar name.version);
#else
			add_to_str(&str, &l, cast_uchar name.release);
#endif
			add_to_str(&str, &l, cast_uchar " ");
#ifdef OPENVMS
			p = cast_uchar name.nodename + sizeof(name.nodename);
			if ((unsigned char *)(&name + 1) - p >= 16 && memchr(cast_const_char p, 0, 16))
				add_to_str(&str, &l, cast_uchar p);
			else
#endif
			add_to_str(&str, &l, cast_uchar name.machine);
			if (l >= MAX_STR_LEN) str[MAX_STR_LEN - 1] = 0;
			strcpy(cast_char system_name, cast_const_char str);
			mem_free(str);
			return;
		}
	}
#endif
#ifdef HAVE_POPEN
	/*if (0) {
		FILE *f;
		unsigned char *p;
		memset(system_name, 0, MAX_STR_LEN);
		ENULLLOOP(f, popen("uname -srm", "r"));
		if (!f) goto fail;
		if (!fread(system_name, 1, MAX_STR_LEN - 1, f)) {
			pclose(f);
			goto fail;
		}
		pclose(f);
		for (p = system_name; *p; p++) if (*p < ' ') {
			*p = 0;
			break;
		}
		if (system_name[0]) return;
	}
	fail:*/
#endif
	strcpy(cast_char system_name, SYSTEM_NAME);
}

unsigned char compiler_name[MAX_STR_LEN];

static void get_compiler_name(void)
{
#if defined(__BORLANDC__)

	int w = __BORLANDC__+0;
	int v1 = w / 0x100;
	int v2 = w / 0x10 % 0x10;
	int v3 = w % 0x10;
	if (v1 == 4 && v2 < 5) v1 = 3;
	if (v1 == 4 && v2 == 5) v2 = 0;

	if (!v3) sprintf(cast_char compiler_name, "Borland C %d.%d", v1, v2);
	else sprintf(cast_char compiler_name, "Borland C %d.%d.%d", v1, v2, v3);

#elif defined(__clang__)

#if !defined(__clang_major__) || !defined(__clang_minor__)
	sprintf(cast_char compiler_name, "LLVM/Clang");
#else
	int v1 = __clang_major__+0;
	int v2 = __clang_minor__+0;
#ifdef __clang_patchlevel__
	int v3 = __clang_patchlevel__+0;
#else
	int v3 = 0;
#endif
	if (v3 > 0) sprintf(cast_char compiler_name, "LLVM/Clang %d.%d.%d", v1, v2, v3);
	else sprintf(cast_char compiler_name, "LLVM/Clang %d.%d", v1, v2);
#endif

#elif defined(__COMO_VERSION__)

	int w = __COMO_VERSION__+0;
	int v1 = w / 100;
	int v2 = w % 100;
	if (!(v2 % 10)) sprintf(cast_char compiler_name, "Comeau C %d.%d", v1, v2 / 10);
	else sprintf(cast_char compiler_name, "Comeau C %d.%02d", v1, v2);

#elif defined(__convexc__)

	sprintf(cast_char compiler_name, "Convex C");

#elif defined(_CRAYC)

#if !defined(_RELEASE) || !defined(_RELEASE_MINOR)
	sprintf(cast_char compiler_name, "Cray C");
#else
	int v1 = _RELEASE+0;
	int v2 = _RELEASE_MINOR+0;
	sprintf(cast_char compiler_name, "Cray C %d.%d", v1, v2);
#endif

#elif defined(__DCC__)

#ifndef __VERSION_NUMBER__
	sprintf(cast_char compiler_name, "Diab C");
#else
	int w = __VERSION_NUMBER__+0;
	int v1 = w / 1000;
	int v2 = w / 100 % 10;
	int v3 = w % 100;
	sprintf(cast_char compiler_name, "Diab C %d.%d.%02d", v1, v2, v3);
#endif

#elif defined(__DMC__)

	int w = __DMC__+0;
	int v1 = w / 0x100;
	int v2 = w / 0x10 % 0x10;
	int v3 = w % 0x10;
	if (!v3) sprintf(cast_char compiler_name, "Digital Mars C %d.%d", v1, v2);
	else sprintf(cast_char compiler_name, "Digital Mars C %d.%d.%d", v1, v2, v3);

#elif defined(__DECC_VER)

	int w = __DECC_VER+0;
	int v1 = w / 10000000;
	int v2 = w / 100000 % 100;
	int v3 = w % 10000;
	sprintf(cast_char compiler_name, "DEC C %d.%d-%03d", v1, v2, v3);

#elif defined(__ghs__)

#ifndef __GHS_VERSION_NUMBER__
	sprintf(cast_char compiler_name, "Green Hill C");
#else
	int w = __GHS_VERSION_NUMBER__+0;
	int v1 = w / 100;
	int v2 = w / 10 % 10;
	int v3 = w % 10;
	sprintf(cast_char compiler_name, "Green Hill C %d.%d.%d", v1, v2, v3);
#endif

#elif defined(__HIGHC__)

	sprintf(cast_char compiler_name, "MetaWare High C");

#elif defined(__HP_cc)

	int w = __HP_cc+0;
	int v1 = w / 10000;
	int v2 = w / 100 % 100;
	int v3 = w % 100;
	if (w <= 1) sprintf(cast_char compiler_name, "HP CC");
	else sprintf(cast_char compiler_name, "HP CC %d.%02d.%02d", v1, v2, v3);

#elif defined(__xlc__)

	int w = __xlc__+0;
	int v1 = w / 0x100;
	int v2 = w % 0x100;
	sprintf(cast_char compiler_name, "IBM XL C %X.%X", v1, v2);

#elif defined(__IBMC__) && defined(__COMPILER_VER__)

	unsigned w = __COMPILER_VER__+0;
	int v0 = w / 0x10000000;
	int v1 = w / 0x1000000 % 0x10;
	int v2 = w / 0x10000 % 0x100;
	int v3 = w % 0x10000;
	unsigned char *os = !v0 ? "S/370" : v0 == 1 ? "OS/390" : v0 == 4 ? "z/OS" : "";
	sprintf(cast_char compiler_name, "IBM%s%s XL C %X.%0X.%X", *os ? " " : "", os, v1, v2, v3);

#elif defined(__ICC)

	int w = __ICC+0;
	int v1 = w / 100;
	int v2 = w % 100;
	if (!(v2 % 10)) sprintf(cast_char compiler_name, "Intel C %d.%d", v1, v2 / 10);
	else sprintf(cast_char compiler_name, "Intel C %d.%02d", v1, v2);

#elif defined(__LCC__)

	sprintf(cast_char compiler_name, "LCC");

#elif defined(__NDPC__)

	sprintf(cast_char compiler_name, "Microway NDP C");

#elif defined(_MSC_VER)

	int w = _MSC_VER+0;
	int v1 = w / 100;
	int v2 = w % 100;
	unsigned char *visual = cast_uchar "";
	if (v1 >= 8) {
		v1 -= 6;
		if (v1 == 2) v1 = 1;
		visual = cast_uchar "Visual ";
	}
	if (!(v2 % 10)) sprintf(cast_char compiler_name, "Microsoft %sC %d.%d", visual, v1, v2 / 10);
	else sprintf(cast_char compiler_name, "Microsoft %sC %d.%02d", visual, v1, v2);

#elif defined(__MWERKS__)

	int w = __MWERKS__+0;
	int v1 = w / 0x1000;
	int v2 = w / 0x100 % 0x10;
	int v3 = w % 0x100;
	if (w <= 1) sprintf(cast_char compiler_name, "Metrowerks CodeWarrior");
	sprintf(cast_char compiler_name, "Metrowerks CodeWarrior %x.%x.%x", v1, v2, v3);

#elif defined(__NWCC__)

	sprintf(cast_char compiler_name, "NWCC");

#elif defined(__OPEN64__)

	unsigned char *n = cast_uchar "Open64 " __OPEN64__;
	if (strlen(cast_const_char n) >= sizeof(cast_char compiler_name)) n = cast_uchar "Open64";
	strcpy(cast_char compiler_name, cast_const_char n);

#elif defined(__PATHSCALE__)

	unsigned char *n = cast_uchar "PathScale " __PATHSCALE__;
	if (strlen(cast_const_char n) >= sizeof(cast_char compiler_name)) n = cast_uchar "PathScale";
	strcpy(cast_char compiler_name, cast_const_char n);

#elif defined(__PCC__)

	int v1 = __PCC__+0;
#ifdef __PCC_MINOR__
	int v2 = __PCC_MINOR__+0;
#else
	int v2 = 0;
#endif
#ifdef __PCC_MINORMINOR__
	int v3 = __PCC_MINORMINOR__+0;
#else
	int v3 = 0;
#endif
	sprintf(cast_char compiler_name, "PCC %d.%d.%d", v1, v2, v3);

#elif defined(__PGI) || defined(__PGIC__)

#if !defined(__PGIC__) || !defined(__PGIC_MINOR__)
	sprintf(cast_char compiler_name, "The Portland Group C");
#else
	int v1 = __PGIC__+0;
	int v2 = __PGIC_MINOR__+0;
#ifdef __PGIC_PATCHLEVEL__
	int v3 = __PGIC_PATCHLEVEL__+0;
#else
	int v3 = 0;
#endif
	if (v3 > 0) sprintf(cast_char compiler_name, "The Portland Group C %d.%d.%d", v1, v2, v3);
	else sprintf(cast_char compiler_name, "The Portland Group C %d.%d", v1, v2);
#endif

#elif defined(__SASC__)

	int w = __SASC__+0;
	int v1 = w / 100;
	int v2 = w % 100;
	sprintf(cast_char compiler_name, "SAS C %d.%02d", v1, v2);

#elif (defined(__sgi) && defined(_COMPILER_VERSION)) || defined(_SGI_COMPILER_VERSION)

#ifdef _SGI_COMPILER_VERSION
	int w = _SGI_COMPILER_VERSION;
#else
	int w = _COMPILER_VERSION;
#endif
	int v1 = w / 100;
	int v2 = w / 10 % 10;
	int v3 = w % 10;
	sprintf(cast_char compiler_name, "MIPSpro %d.%d.%d", v1, v2, v3);

#elif defined(__SUNPRO_C)

	int w = __SUNPRO_C+0;
	int div = w >= 0x1000 ? 0x1000 : 0x100;
	int v2_digits = w >= 0x1000 ? 2 : 1;
	int v1 = w / div;
	int v2 = w % div / 0x10;
	int v3 = w % 0x10;
	if (!v3) sprintf(cast_char compiler_name, "Sun C %X.%0*X", v1, v2_digits, v2);
	else sprintf(cast_char compiler_name, "Sun C %X.%0*X.%X", v1, v2_digits, v2, v3);

#elif defined(__SYSC__) && defined(__SYSC_VER__)

	int w = __SYSC_VER__+0;
	int v1 = w / 10000;
	int v2 = w / 100 % 100;
	int v3 = w % 100;
	sprintf(cast_char compiler_name, "Dignus Systems C %d.%02d.%02d", v1, v2, v3);

#elif defined(__TenDRA__)

	sprintf(cast_char compiler_name, "TenDRA C");

#elif defined(__TINYC__)

	sprintf(cast_char compiler_name, "Tiny C");

#elif defined(_UCC)

#if !defined(_MAJOR_REV) || !defined(_MINOR_REV)
	sprintf(cast_char compiler_name, "Ultimate C");
#else
	int v1 = _MAJOR_REV+0;
	int v2 = _MAJOR_REV+0;
	sprintf(cast_char compiler_name, "Ultimate C %d.%d", v1, v2);
#endif

#elif defined(__USLC__)

	sprintf(cast_char compiler_name, "USL C");

#elif defined(__VAXC)

	sprintf(cast_char compiler_name, "VAX C");

#elif defined(__VOSC__)

	sprintf(cast_char compiler_name, "Stratus VOS C");

#elif defined(__WATCOMC__)

	int w = __WATCOMC__+0;
	int v1 = w / 100;
	int v2 = w % 100;
	unsigned char *op = cast_uchar "";
	if (v1 >= 12) {
		v1 -= 11;
		op = cast_uchar "Open";
	}
	if (!(v2 % 10)) sprintf(cast_char compiler_name, "%sWatcom C %d.%d", op, v1, v2 / 10);
	else sprintf(cast_char compiler_name, "%sWatcom C %d.%02d", op, v1, v2);

#elif defined(__GNUC__)

	int v1 = __GNUC__+0;
#ifdef __GNUC_MINOR__
	int v2 = __GNUC_MINOR__+0;
#else
	int v2 = -1;
#endif
#ifdef __GNUC_PATCHLEVEL__
	int v3 = __GNUC_PATCHLEVEL__+0;
#else
	int v3 = 0;
#endif
#if defined(__llvm__)
	unsigned char *prefix = cast_uchar "LLVM/";
#else
	unsigned char *prefix = cast_uchar "";
#endif
	if (v1 == 2 && (v2 >= 90 && v2 <= 91)) sprintf(cast_char compiler_name, "%sEGCS 1.%d", prefix, v2 - 90);
	else if (v3 > 0 && v2 >= 0) sprintf(cast_char compiler_name, "%sGNU C %d.%d.%d", prefix, v1, v2, v3);
	else if (v2 >= 0) sprintf(cast_char compiler_name, "%sGNU C %d.%d", prefix, v1, v2);
	else sprintf(cast_char compiler_name, "%sGNU C %d", prefix, v1);

#else

	strcpy(cast_char compiler_name, "unknown compiler");

#endif
}

static void do_exit(int x)
{
	/* avoid compiler warnings about unreachable code */
	static volatile int zero = 0;
	if (!zero) exit(x);
}

struct option {
	int p;
	unsigned char *(*rd_cmd)(struct option *, unsigned char ***, int *);
	unsigned char *(*rd_cfg)(struct option *, unsigned char *);
	void (*wr_cfg)(struct option *, unsigned char **, int *);
	int min, max;	/* for double min and max are in 1/100's (e.g. 0.1 is min==10) */
	void *ptr;
	char *cfg_name;
	char *cmd_name;
};

static unsigned char *p_arse_options(int argc, unsigned char *argv[], struct option **opt)
{
	unsigned char *e, *u = NULL;
	int i;
	for (i = 0; i < argc; i++) {
		if (strlen(cast_const_char argv[i]) >= MAXINT) {
			fprintf(stderr, "Too long parameter\n");
			return NULL;
		}
	}
	while (argc) {
		argv++, argc--;
		if (argv[-1][0] == '-') {
			struct option *options;
			struct option **op;
			for (op = opt; (options = *op); op++) for (i = 0; options[i].p; i++)
				if (options[i].rd_cmd && options[i].cmd_name &&
				    !strcasecmp(cast_const_char options[i].cmd_name, cast_const_char &argv[-1][1])) {
					if ((e = options[i].rd_cmd(&options[i], &argv, &argc))) {
						if (e[0]) fprintf(stderr, "Error parsing option %s: %s\n", argv[-1], e);
						return NULL;
					}
					goto found;
				}
			uu:
#ifdef GRDRV_DIRECTFB
			if (!strncmp(cast_const_char argv[-1], "--dfb:", 6))
				goto found;
#endif
			fprintf(stderr, "Unknown option %s\n", argv[-1]);
			return NULL;
		} else if (!u) u = argv[-1];
		else goto uu;
		found:;
	}
	if (u) return u;
	return cast_uchar "";
}

static unsigned char *get_token(unsigned char **line)
{
	unsigned char *s = NULL;
	int l = 0;
	int escape = 0;
	int quote = 0;

	while (**line == ' ' || **line == 9) (*line)++;
	if (**line) {
		for (s = init_str(); **line; (*line)++) {
			if (escape)
				escape = 0;
			else if (**line == '\\') {
				escape = 1;
				continue;
			}
			else if (**line == '"') {
				quote = !quote;
				continue;
			}
			else if ((**line == ' ' || **line == 9) && !quote)
				break;
			add_chr_to_str(&s, &l, **line);
		}
	}
	return s;
}

static int get_token_num(unsigned char **line)
{
	long l;
	unsigned char *end;
	unsigned char *t = get_token(line);
	if (!t) return -1;
	l = strtolx(t, &end);
	if (*end || end == t || l < 0 || l != (int)l) {
		mem_free(t);
		return -1;
	}
	mem_free(t);
	return (int)l;
}

static void parse_config_file(unsigned char *name, unsigned char *file, struct option **opt)
{
	struct option *options;
	struct option **op;
	int err = 0;
	int line = 0;
	unsigned char *e;
	int i;
	unsigned char *n, *p;
	unsigned char *tok;
	int nl, pl;
	while (file[0]) {
		line++;
		while (file[0] && (file[0] == ' ' || file[0] == 9)) file++;
		n = file;
		while (file[0] && file[0] > ' ') file++;
		if (file == n) {
			if (file[0]) file++;
			continue;
		}
		while (file[0] == 9 || file[0] == ' ') file++;
		p = file;
		while (file[0] && file[0] != 10 && file[0] != 13) file++;
		pl = (int)(file - p);
		if (file[0]) {
			if ((file[1] == 10 || file[1] == 13) && file[0] != file[1]) file++;
			file++;
		}
		tok = NULL;
		if (n[0] == '#') goto f;
		if (!(tok = get_token(&n))) goto f;
		nl = (int)strlen(cast_const_char tok);
		for (op = opt; (options = *op); op++)
			for (i = 0; options[i].p; i++) if (options[i].cfg_name && (size_t)nl == strlen(cast_const_char options[i].cfg_name) && !casecmp(tok, cast_uchar options[i].cfg_name, nl)) {
				unsigned char *o = memacpy(p, pl);
				if ((e = options[i].rd_cfg(&options[i], o))) {
					if (e[0]) fprintf(stderr, "Error parsing config file %s, line %d: %s\n", name, line, e), err = 1;
				}
				mem_free(o);
				goto f;
			}
		fprintf(stderr, "Unknown option in config file %s, line %d\n", name, line);
		err = 1;
		f:
		if (tok) mem_free(tok);
	}
	if (err) fprintf(stderr, "\007"), sleep(1);
}

static unsigned char *create_config_string(struct option *options)
{
	unsigned char *s = init_str();
	int l = 0;
	int i;
	add_to_str(&s, &l, cast_uchar "# This file is automatically generated by Links -- please do not edit.");
#ifdef __DECC_VER
	do_not_optimize_here(&options);
#endif
	for (i = 0; options[i].p; i++) {
		if (options[i].wr_cfg)
			options[i].wr_cfg(&options[i], &s, &l);
	}
	add_to_str(&s, &l, cast_uchar NEWLINE);
	return s;
}

#define FILE_BUF	1024

static unsigned char cfg_buffer[FILE_BUF];

unsigned char *read_config_file(unsigned char *name)
{
	int h, r;
	int l = 0;
	unsigned char *s;
	int rs;
	h = c_open(name, O_RDONLY | O_NOCTTY);
	if (h == -1) return NULL;
	s = init_str();
	while ((r = hard_read(h, cfg_buffer, FILE_BUF)) > 0) {
		int i;
		for (i = 0; i < r; i++) if (!cfg_buffer[i]) cfg_buffer[i] = ' ';
		add_bytes_to_str(&s, &l, cfg_buffer, r);
	}
	if (r == -1) mem_free(s), s = NULL;
	EINTRLOOP(rs, close(h));
	return s;
}

int write_to_config_file(unsigned char *name, unsigned char *c)
{
	int rr;
	int h, w;
	int count = 0;
	int tmp_namel;
	unsigned char *tmp_name;
	int rs, err;
try_new_count:
	tmp_namel = 0;
	tmp_name = init_str();
	add_to_str(&tmp_name, &tmp_namel, name);
	for (w = tmp_namel - 1; w >= 0; w--) {
		if (dir_sep(tmp_name[w]))
			break;
		if (tmp_name[w] == '.') {
			if (w <= tmp_namel - 2) {
				tmp_name[w + 2] = 0;
				tmp_namel = w + 2;
			}
			break;
		}
	}
	add_num_to_str(&tmp_name, &tmp_namel, count);
	h = c_open3(tmp_name, O_WRONLY | O_NOCTTY | O_CREAT | O_TRUNC | O_EXCL, 0600);
	if (h == -1) {
		err = errno;
		if (err == EEXIST && count < MAXINT) {
			count++;
			mem_free(tmp_name);
			goto try_new_count;
		}
		goto free_err;
	}
	rr = (int)strlen(cast_const_char c);
	if (hard_write(h, c, rr) != rr) {
		err = errno;
		EINTRLOOP(rs, close(h));
		goto unlink_err;
	}
	EINTRLOOP(rs, close(h));
	if (rs) {
		err = errno;
		goto unlink_err;
	}
#if defined(OPENVMS)
	/* delete all versions of the file */
	count = 0;
	do {
		EINTRLOOP(rs, unlink(cast_const_char name));
	} while (!rs && ++count < 65536);
#elif !defined(RENAME_OVER_EXISTING_FILES)
	EINTRLOOP(rs, unlink(cast_const_char name));
#endif
	EINTRLOOP(rs, rename(cast_const_char tmp_name, cast_const_char name));
	if (rs) {
		err = errno;
		goto unlink_err;
	}
	mem_free(tmp_name);
	return 0;

	unlink_err:
	EINTRLOOP(rs, unlink(cast_const_char tmp_name));
	free_err:
	mem_free(tmp_name);
	return get_error_from_errno(err);
}

#ifdef OPENVMS
static void translate_vms_to_unix(unsigned char **str)
{
	unsigned char *n;
	if (!*str || strchr(cast_const_char *str, '/')) return;
	n = cast_uchar decc$translate_vms(cast_const_char *str);
	if (!n || (my_intptr_t)n == -1) return;
	mem_free(*str);
	*str = stracpy(n);
}
#endif

static unsigned char *get_home(int *n)
{
	struct stat st;
	int rs;
	unsigned char *home;
	unsigned char *home_links;
	unsigned char *config_dir;

	EINTRLOOP(rs, stat(".", &st));
	if (rs && (home = cast_uchar getenv("HOME")))
		EINTRLOOP(rs, chdir(cast_const_char home));
	home = NULL;

	config_dir = stracpy(cast_uchar getenv("CONFIG_DIR"));

	if (n) *n = 1;
#ifdef WIN
	if (!home) {
		home = stracpy(cast_uchar getenv("APPDATA"));
#ifdef HAVE_CYGWIN_CONV_PATH
		/*
		 * Newer Cygwin complains about windows-style path, so
		 * we have to convert it.
		 */
		if (home) {
			unsigned char *new_path;
			ssize_t sz = cygwin_conv_path(CCP_WIN_A_TO_POSIX | CCP_ABSOLUTE, home, NULL, 0);
			if (sz < 0)
				goto skip_path_conv;
			new_path = mem_alloc(sz);
			sz = cygwin_conv_path(CCP_WIN_A_TO_POSIX | CCP_ABSOLUTE, home, new_path, sz);
			if (sz < 0) {
				mem_free(new_path);
				goto skip_path_conv;
			}
			mem_free(home);
			home = new_path;
skip_path_conv:;
		}
#endif
		if (home) {
			EINTRLOOP(rs, stat(cast_const_char home, &st));
			if (rs || !S_ISDIR(st.st_mode)) {
				mem_free(home);
				home = NULL;
			}
		}
	}
#endif
	if (!home) home = stracpy(cast_uchar getenv("HOME"));
#ifdef WIN
/* When we run in Cygwin without Cygwin environment, it reports home "/".
   Unfortunatelly, it can't write anything to that directory */
	if (home && !strcmp(cast_const_char home, "/")) {
		mem_free(home);
		home = NULL;
	}
#endif
#ifdef OPENVMS
	if (!home) home = stracpy(cast_uchar "/SYS$LOGIN");
	translate_vms_to_unix(&home);
	translate_vms_to_unix(&config_dir);
#endif
	if (!home) {
		int i;
		home = stracpy(path_to_exe);
		if (!home) {
			if (config_dir) mem_free(config_dir);
			return NULL;
		}
		for (i = (int)strlen(cast_const_char home) - 1; i >= 0; i--) if (dir_sep(home[i])) {
			home[i + 1] = 0;
			goto br;
		}
		home[0] = 0;
		br:;
	}
	while (home[0] && home[1] && dir_sep(home[strlen(cast_const_char home) - 1])) home[strlen(cast_const_char home) - 1] = 0;
	if (home[0]) add_to_strn(&home, cast_uchar "/");
	home_links = stracpy(home);
	if (config_dir) {
		add_to_strn(&home_links, config_dir);
		while (home_links[0] && dir_sep(home_links[strlen(cast_const_char home_links) - 1])) home_links[strlen(cast_const_char home_links) - 1] = 0;
		EINTRLOOP(rs, stat(cast_const_char home_links, &st));
		if (!rs && S_ISDIR(st.st_mode)) {
			add_to_strn(&home_links, cast_uchar "/links");
		} else {
			fprintf(stderr, "CONFIG_DIR set to %s. But directory %s doesn't exist.\n\007", config_dir, home_links);
			sleep(3);
			mem_free(home_links);
			home_links = stracpy(home);
			add_to_strn(&home_links, cast_uchar ".links");
		}
		mem_free(config_dir);
	} else add_to_strn(&home_links, cast_uchar ".links");
	EINTRLOOP(rs, stat(cast_const_char home_links, &st));
	if (rs) {
#ifdef HAVE_MKDIR
		EINTRLOOP(rs, mkdir(cast_const_char home_links, 0700));
		if (!rs) goto home_creat;
#ifdef OPENVMS
		if (errno == EEXIST) goto home_ok;
#endif
#endif
		if (config_dir) goto failed;
		goto first_failed;
	}
	if (S_ISDIR(st.st_mode)) goto home_ok;
	/* This is a Cygwin hack! Cygwin reports stat for "links" if no
	   "links" exists and only "links.exe" does. So try to create directory
	   anyway. */
#ifdef HAVE_MKDIR
	EINTRLOOP(rs, mkdir(cast_const_char home_links, 0700));
	if (!rs) goto home_creat;
#endif
	first_failed:
	mem_free(home_links);
	home_links = stracpy(home);
#ifdef DOS
	add_to_strn(&home_links, cast_uchar "links.cfg");
#else
	add_to_strn(&home_links, cast_uchar "links");
#endif
	EINTRLOOP(rs, stat(cast_const_char home_links, &st));
	if (rs) {
#ifdef HAVE_MKDIR
		EINTRLOOP(rs, mkdir(cast_const_char home_links, 0700));
		if (!rs) goto home_creat;
#ifdef OPENVMS
		if (errno == EEXIST) goto home_ok;
#endif
#else
		mem_free(home_links);
		home_links = stracpy(home);
		goto home_ok;
#endif
		goto failed;
	}
	if (S_ISDIR(st.st_mode)) goto home_ok;
#ifdef HAVE_MKDIR
	EINTRLOOP(rs, mkdir(cast_const_char home_links, 0700));
	if (!rs) goto home_creat;
#endif
	failed:
	mem_free(home_links);
	mem_free(home);
	return NULL;

	home_ok:
	if (n) *n = 0;
	home_creat:
#ifdef HAVE_CHMOD
	EINTRLOOP(rs, chmod(cast_const_char home_links, 0700));
#endif
	add_to_strn(&home_links, cast_uchar "/");
	mem_free(home);
	return home_links;
}

void init_home(void)
{
	get_system_name();
	get_compiler_name();
	links_home = get_home(&first_use);
	if (!links_home) {
		fprintf(stderr, "Unable to find or create links config directory. Please check, that you have $HOME variable set correctly and that you have write permission to your home directory.\n\007");
		sleep(3);
		return;
	}
}

/* prefix: directory
 * name: name of the configuration file (typ. links.cfg)
 */
static int write_config_data(unsigned char *prefix, unsigned char *name, struct option *o, struct terminal *term)
{
	int err;
	unsigned char *c, *config_file;
	if (!(c = create_config_string(o))) return -1;
	config_file = stracpy(prefix);
	if (!config_file) {
		mem_free(c);
		return -1;
	}
	add_to_strn(&config_file, name);
	if ((err = write_to_config_file(config_file, c))) {
		if (term) msg_box(term, NULL, TEXT_(T_CONFIG_ERROR), AL_CENTER | AL_EXTD_TEXT, TEXT_(T_UNABLE_TO_WRITE_TO_CONFIG_FILE), cast_uchar ": ", get_err_msg(err), NULL, NULL, 1, TEXT_(T_CANCEL), NULL, B_ENTER | B_ESC);
		mem_free(c);
		mem_free(config_file);
		return -1;
	}
	mem_free(c);
	mem_free(config_file);
	return 0;
}

static void add_nm(struct option *o, unsigned char **s, int *l)
{
	if (*l) add_to_str(s, l, cast_uchar NEWLINE);
	add_to_str(s, l, cast_uchar o->cfg_name);
	add_to_str(s, l, cast_uchar " ");
}

static void add_quoted_to_str(unsigned char **s, int *l, unsigned char *q)
{
	add_chr_to_str(s, l, '"');
	while (*q) {
		if (*q == '"' || *q == '\\') add_chr_to_str(s, l, '\\');
		add_chr_to_str(s, l, *q);
		q++;
	}
	add_chr_to_str(s, l, '"');
}

static unsigned char *num_rd(struct option *o, unsigned char *c)
{
	unsigned char *tok = get_token(&c);
	unsigned char *end;
	long l;
	if (!tok) return cast_uchar "Missing argument";
	l = strtolx(tok, &end);
	if (*end) {
		mem_free(tok);
		return cast_uchar "Number expected";
	}
	if (l < o->min || l > o->max || l != (long)(int)l) {
		mem_free(tok);
		return cast_uchar "Out of range";
	}
	*(int *)o->ptr = (int)l;
	mem_free(tok);
	return NULL;
}

static void num_wr(struct option *o, unsigned char **s, int *l)
{
	add_nm(o, s, l);
	add_knum_to_str(s, l, *(int *)o->ptr);
}

static unsigned char *dbl_rd(struct option *o, unsigned char *c)
{
	unsigned char *tok = get_token(&c);
	unsigned char *end;
	double d;

	if (!tok) return cast_uchar "Missing argument";

	if (strlen(cast_const_char tok) >= 1000) return cast_uchar "Number is too long";

	d = strtod(cast_const_char tok, (char **)(void *)&end);

	if (*end) {
		mem_free(tok);
		return cast_uchar "Number expected";
	}
	if (d < 0 || d > o->max || 100*d < o->min || 100*d > o->max) {
		mem_free(tok);
		return cast_uchar "Out of range";
	}
	*(double *)o->ptr = d;
	mem_free(tok);
	return NULL;
}

static void dbl_wr(struct option *o, unsigned char **s, int *l)
{
	unsigned char number[80];
	snprintf(cast_char number, sizeof number, "%.6f", *(double*)o->ptr);

	add_nm(o, s, l);
	add_to_str(s, l, number);
}

static unsigned char *str_rd(struct option *o, unsigned char *c)
{
	unsigned char *tok = get_token(&c);
	unsigned char *e = NULL;
	if (!tok) return NULL;
	if (strlen(cast_const_char tok) + 1 > (size_t)o->max) e = cast_uchar "String too long";
	else strcpy(cast_char o->ptr, cast_const_char tok);
	mem_free(tok);
	return e;
}

static void str_wr(struct option *o, unsigned char **s, int *l)
{
	add_nm(o, s, l);
	if (strlen(cast_const_char o->ptr) + 1 > (size_t)o->max) {
		unsigned char *s1 = init_str();
		int l1 = 0;
		add_bytes_to_str(&s1, &l1, o->ptr, o->max - 1);
		add_quoted_to_str(s, l, s1);
		mem_free(s1);
	}
	else add_quoted_to_str(s, l, o->ptr);
}

static unsigned char *cp_rd(struct option *o, unsigned char *c)
{
	unsigned char *tok = get_token(&c);
	unsigned char *e = NULL;
	int i;
	if (!tok) return cast_uchar "Missing argument";
	if ((i = get_cp_index(tok)) == -1) e = cast_uchar "Unknown codepage";
#ifndef ENABLE_UTF8
	else if (o->min == 1 && i == utf8_table) e = cast_uchar "UTF-8 can't be here";
#endif
	else *(int *)o->ptr = i;
	mem_free(tok);
	return e;
}

static void cp_wr(struct option *o, unsigned char **s, int *l)
{
	unsigned char *n = get_cp_mime_name(*(int *)o->ptr);
	add_nm(o, s, l);
	add_to_str(s, l, n);
}

static unsigned char *lang_rd(struct option *o, unsigned char *c)
{
	int i;
	unsigned char *tok = get_token(&c);
	if (!tok) return cast_uchar "Missing argument";
	for (i = -1; i < n_languages(); i++)
		if (!(strcasecmp(cast_const_char language_name(i), cast_const_char tok))) {
			set_language(i);
			mem_free(tok);
			return NULL;
		}
	mem_free(tok);
	return cast_uchar "Unknown language";
}

static void lang_wr(struct option *o, unsigned char **s, int *l)
{
	add_nm(o, s, l);
	add_quoted_to_str(s, l, language_name(current_language));
}

static int getnum(unsigned char *s, int *n, int r1, int r2)
{
	unsigned char *e;
	long l = strtol(cast_const_char s, (char **)(void *)&e, 10);
	if (*e || !*s) return -1;
	if (l < r1 || l >= r2) return -1;
	*n = (int)l;
	return 0;
}

static unsigned char *type_rd(struct option *o, unsigned char *c)
{
	unsigned char *err = cast_uchar "Error reading association specification";
	struct assoc neww;
	unsigned char *w;
	int n = 0;	/* against warning */
	memset(&neww, 0, sizeof(struct assoc));
	if (!(neww.label = get_token(&c))) goto err;
	if (!(neww.ct = get_token(&c))) goto err;
	if (!(neww.prog = get_token(&c))) goto err;
	if (!(w = get_token(&c))) goto err;
	if (getnum(w, &n, 0, 128)) goto err_f;
	mem_free(w);
	neww.cons = !!(n & 1);
	neww.xwin = !!(n & 2);
	neww.ask = !!(n & 4);
	if ((n & 8) || (n & 16)) neww.block = !!(n & 16);
	else neww.block = !neww.xwin || neww.cons;
	neww.accept_http = !!(n & 32);
	neww.accept_ftp = !!(n & 64);
	if (!(w = get_token(&c))) goto err;
	if (getnum(w, &neww.system, 0, 256)) goto err_f;
	mem_free(w);
	update_assoc(&neww);
	err = NULL;
	err:
	if (neww.label) mem_free(neww.label);
	if (neww.ct) mem_free(neww.ct);
	if (neww.prog) mem_free(neww.prog);
	return err;
	err_f:
	mem_free(w);
	goto err;
}

static unsigned char *block_rd(struct option *o, unsigned char *c)
{
	unsigned char *err = cast_uchar "Error reading image block specification";
	unsigned char* url;

	if(!(url = get_token(&c)))
		return err;

	block_url_add(NULL, url);

	mem_free(url);

	return NULL;
}

static void block_wr(struct option *o, unsigned char **s, int *l)
{
	struct block *a;
	foreachback(a, blocks) {
		add_nm(o, s, l);
		add_quoted_to_str(s, l, a->url);
	}
}

static void type_wr(struct option *o, unsigned char **s, int *l)
{
	struct assoc *a;
	foreachback(a, assoc) {
		add_nm(o, s, l);
		add_quoted_to_str(s, l, a->label);
		add_to_str(s, l, cast_uchar " ");
		add_quoted_to_str(s, l, a->ct);
		add_to_str(s, l, cast_uchar " ");
		add_quoted_to_str(s, l, a->prog);
		add_to_str(s, l, cast_uchar " ");
		add_num_to_str(s, l, (!!a->cons) + (!!a->xwin) * 2 + (!!a->ask) * 4 + (!a->block) * 8 + (!!a->block) * 16 + (!!a->accept_http) * 32 + (!!a->accept_ftp) * 64);
		add_to_str(s, l, cast_uchar " ");
		add_num_to_str(s, l, a->system);
	}
}

static unsigned char *ext_rd(struct option *o, unsigned char *c)
{
	unsigned char *err = cast_uchar "Error reading extension specification";
	struct extension neww;
	memset(&neww, 0, sizeof(struct extension));
	if (!(neww.ext = get_token(&c))) goto err;
	if (!(neww.ct = get_token(&c))) goto err;
	update_ext(&neww);
	err = NULL;
	err:
	if (neww.ext) mem_free(neww.ext);
	if (neww.ct) mem_free(neww.ct);
	return err;
}

static void ext_wr(struct option *o, unsigned char **s, int *l)
{
	struct extension *a;
	foreachback(a, extensions) {
		add_nm(o, s, l);
		add_quoted_to_str(s, l, a->ext);
		add_to_str(s, l, cast_uchar " ");
		add_quoted_to_str(s, l, a->ct);
	}
}

static unsigned char *prog_rd(struct option *o, unsigned char *c)
{
	unsigned char *err = cast_uchar "Error reading program specification";
	unsigned char *prog, *w;
	int n;
	if (!(prog = get_token(&c))) goto err_1;
	if (!(w = get_token(&c))) goto err_2;
	if (getnum(w, &n, 0, 256)) goto err_3;
	update_prog(o->ptr, prog, n);
	err = NULL;
	err_3:
	mem_free(w);
	err_2:
	mem_free(prog);
	err_1:
	return err;
}

static void prog_wr(struct option *o, unsigned char **s, int *l)
{
	struct protocol_program *a;
	foreachback(a, *(struct list_head *)o->ptr) {
		if (!*a->prog) continue;
		add_nm(o, s, l);
		add_quoted_to_str(s, l, a->prog);
		add_to_str(s, l, cast_uchar " ");
		add_num_to_str(s, l, a->system);
	}
}

static unsigned char *term_rd(struct option *o, unsigned char *c)
{
	struct term_spec *ts;
	unsigned char *w;
	int i, l;
	if (!(w = get_token(&c))) goto err;
	ts = new_term_spec(w);
	mem_free(w);
	if (!(w = get_token(&c))) goto err;
	if (strlen(cast_const_char w) != 1 || w[0] < '0' || w[0] > '4') goto err_f;
	ts->mode = w[0] - '0';
	mem_free(w);
	if (!(w = get_token(&c))) goto err;
	if (strlen(cast_const_char w) != 1 || w[0] < '0' || w[0] > '3') goto err_f;
	ts->m11_hack = (w[0] - '0') & 1;
	ts->braille = !!((w[0] - '0') & 2);
	mem_free(w);
	if (!(w = get_token(&c))) goto err;
	if (strlen(cast_const_char w) != 1 || w[0] < '0' || w[0] > '7') goto err_f;
	ts->col = (w[0] - '0') & 1;
	ts->restrict_852 = !!((w[0] - '0') & 2);
	ts->block_cursor = !!((w[0] - '0') & 4);
	mem_free(w);
	if (!(w = get_token(&c))) goto err;
	if (!strcasecmp(cast_const_char w, "default")) {
		i = -1;
	} else {
		if ((i = get_cp_index(w)) == -1) goto err_f;
	}
#ifndef ENABLE_UTF8
	if (i == utf8_table) {
		i = 0;
	}
#endif
	ts->character_set = i;
	mem_free(w);
	l = get_token_num(&c);
	if (l < 0) goto ret;
	if (l > 999) goto err;
	ts->left_margin = l;
	l = get_token_num(&c);
	if (l < 0) goto err;
	if (l > 999) goto err;
	ts->right_margin = l;
	l = get_token_num(&c);
	if (l < 0) goto err;
	if (l > 999) goto err;
	ts->top_margin = l;
	l = get_token_num(&c);
	if (l < 0) goto err;
	if (l > 999) goto err;
	ts->bottom_margin = l;
	ret:
	return NULL;
	err_f:
	mem_free(w);
	err:
	return cast_uchar "Error reading terminal specification";
}

static unsigned char *term2_rd(struct option *o, unsigned char *c)
{
	struct term_spec *ts;
	unsigned char *w;
	int i;
	if (!(w = get_token(&c))) goto err;
	ts = new_term_spec(w);
	mem_free(w);
	if (!(w = get_token(&c))) goto err;
	if (strlen(cast_const_char w) != 1 || w[0] < '0' || w[0] > '3') goto err_f;
	ts->mode = w[0] - '0';
	mem_free(w);
	if (!(w = get_token(&c))) goto err;
	if (strlen(cast_const_char w) != 1 || w[0] < '0' || w[0] > '1') goto err_f;
	ts->m11_hack = w[0] - '0';
	mem_free(w);
	if (!(w = get_token(&c))) goto err;
	if (strlen(cast_const_char w) != 1 || w[0] < '0' || w[0] > '1') goto err_f;
	ts->restrict_852 = w[0] - '0';
	mem_free(w);
	if (!(w = get_token(&c))) goto err;
	if (strlen(cast_const_char w) != 1 || w[0] < '0' || w[0] > '1') goto err_f;
	ts->col = w[0] - '0';
	mem_free(w);
	if (!(w = get_token(&c))) goto err;
	if (!strcasecmp(cast_const_char w, "default")) {
		i = -1;
	} else {
		if ((i = get_cp_index(w)) == -1) goto err_f;
	}
#ifndef ENABLE_UTF8
	if (i == utf8_table) {
		i = 0;
	}
#endif
	ts->character_set = i;
	mem_free(w);
	return NULL;
	err_f:
	mem_free(w);
	err:
	return cast_uchar "Error reading terminal specification";
}

static void term_wr(struct option *o, unsigned char **s, int *l)
{
	struct term_spec *ts;
	foreachback(ts, term_specs) {
		add_nm(o, s, l);
		add_quoted_to_str(s, l, ts->term);
		add_to_str(s, l, cast_uchar " ");
		add_num_to_str(s, l, ts->mode);
		add_to_str(s, l, cast_uchar " ");
		add_num_to_str(s, l, !!ts->m11_hack + !!ts->braille * 2);
		add_to_str(s, l, cast_uchar " ");
		add_num_to_str(s, l, !!ts->col + !!ts->restrict_852 * 2 + !!ts->block_cursor * 4);
		add_to_str(s, l, cast_uchar " ");
		if (ts->character_set == -1) add_to_str(s, l, cast_uchar "default");
		else add_to_str(s, l, get_cp_mime_name(ts->character_set));
		if (ts->left_margin || ts->right_margin || ts->top_margin || ts->bottom_margin) {
			add_to_str(s, l, cast_uchar " ");
			add_num_to_str(s, l, ts->left_margin);
			add_to_str(s, l, cast_uchar " ");
			add_num_to_str(s, l, ts->right_margin);
			add_to_str(s, l, cast_uchar " ");
			add_num_to_str(s, l, ts->top_margin);
			add_to_str(s, l, cast_uchar " ");
			add_num_to_str(s, l, ts->bottom_margin);
		}
	}
}

static struct list_head driver_params = { &driver_params, &driver_params };

struct driver_param *get_driver_param(unsigned char *n)
{
	struct driver_param *dp;
	foreach(dp, driver_params) if (!strcasecmp(cast_const_char dp->name, cast_const_char n)) return dp;
	dp = mem_calloc(sizeof(struct driver_param) + strlen(cast_const_char n) + 1);
	dp->kbd_codepage = -1;
	strcpy(cast_char dp->name, cast_const_char n);
	dp->shell = mem_calloc(1);
	dp->nosave = 1;
	add_to_list(driver_params, dp);
	return dp;
}

static unsigned char *dp_rd(struct option *o, unsigned char *c)
{
	int cc;
	unsigned char *n, *param, *cp, *shell;
	struct driver_param *dp;
	if (!(n = get_token(&c))) goto err;
	if (!(param = get_token(&c))) {
		mem_free(n);
		goto err;
	}
	if (!(shell = get_token(&c))){
		mem_free(n);
		mem_free(param);
		goto err;
	}
	if (!(cp = get_token(&c))) {
		mem_free(n);
		mem_free(param);
		mem_free(shell);
		goto err;
	}
	if (!strcasecmp(cast_const_char cp, "default")) {
		cc = -1;
	} else if ((cc = get_cp_index(cp)) == -1) {
		mem_free(n);
		mem_free(param);
		mem_free(shell);
		mem_free(cp);
		goto err;
	}
	dp = get_driver_param(n);
	dp->kbd_codepage = cc;
	if (dp->param) mem_free(dp->param);
	dp->param = param;
	if (dp->shell) mem_free(dp->shell);
	dp->shell = shell;
	dp->nosave = 0;
	mem_free(cp);
	mem_free(n);
	return NULL;
	err:
	return cast_uchar "Error reading driver mode specification";
}

static void dp_wr(struct option *o, unsigned char **s, int *l)
{
	struct driver_param *dp;
	foreachback(dp, driver_params) {
		if ((!dp->param || !*dp->param) && dp->kbd_codepage == -1 && !*dp->shell) continue;
		if (dp->nosave) continue;
		add_nm(o, s, l);
		add_quoted_to_str(s, l, dp->name);
		add_to_str(s, l, cast_uchar " ");
		add_quoted_to_str(s, l, dp->param ? dp->param : (unsigned char*)"");
		add_to_str(s, l, cast_uchar " ");
		add_quoted_to_str(s, l, dp->shell);
		add_to_str(s, l, cast_uchar " ");
		if (dp->kbd_codepage == -1) add_to_str(s, l, cast_uchar "default");
		else add_to_str(s, l, get_cp_mime_name(dp->kbd_codepage));
	}
}

static unsigned char *ip_rd(struct option *o, unsigned char *c)
{
	unsigned char *e;
	e = str_rd(o, c);
	if (e) return e;
	if (*(unsigned char *)o->ptr && numeric_ip_address(o->ptr, NULL) == -1) return cast_uchar "Invalid IP address";
	return NULL;
}

static unsigned char *ipv6_rd(struct option *o, unsigned char *c)
{
	unsigned char *e;
	e = str_rd(o, c);
	if (e) return e;
#ifdef SUPPORT_IPV6
	if (*(unsigned char *)o->ptr && numeric_ipv6_address(o->ptr, NULL, NULL) == -1) return cast_uchar "Invalid IPv6 address";
#endif
	return NULL;
}

static unsigned char *gen_cmd(struct option *o, unsigned char ***argv, int *argc)
{
	unsigned char *e;
	int l;
	unsigned char *r;
	if (!*argc) return cast_uchar "Parameter expected";
	e = init_str();
	l = 0;
	add_quoted_to_str(&e, &l, **argv);
	r = o->rd_cfg(o, e);
	mem_free(e);
	if (r) return r;
	(*argv)++; (*argc)--;
	return NULL;
}

static unsigned char *lookup_cmd(struct option *o, unsigned char ***argv, int *argc)
{
	int i;
	struct lookup_result addr;
	if (!*argc) return cast_uchar "Parameter expected";
	if (*argc >= 2) return cast_uchar "Too many parameters";
	(*argv)++; (*argc)--;
	do_real_lookup(*(*argv - 1), ipv6_options.addr_preference, &addr);
	if (!addr.n) {
#if !defined(USE_GETADDRINFO) && defined(HAVE_GETHOSTBYNAME) && defined(HAVE_HERROR)
		herror("error");
#else
		fprintf(stderr, "error: host not found\n");
#endif
		do_exit(RET_ERROR);
		return NULL;
	}
	for (i = 0; i < addr.n; i++) {
		switch (addr.a[i].af) {
			case AF_INET: {
				printf("%d.%d.%d.%d", (int)addr.a[i].addr[0], (int)addr.a[i].addr[1], (int)addr.a[i].addr[2], (int)addr.a[i].addr[3]);
				break;
			}
#ifdef SUPPORT_IPV6
			case AF_INET6: {
				int j;
#ifdef HAVE_INET_NTOP
				unsigned char buffer[INET6_ADDRSTRLEN];
				struct in6_addr i6a;
				memcpy(&i6a, addr.a[i].addr, 16);
				if (inet_ntop(AF_INET6, &i6a, cast_char buffer, sizeof buffer))
					printf("%s", cast_const_char buffer);
				else
#endif
				for (j = 0; j < 16; j += 2)
					printf("%x%s", (addr.a[i].addr[j] << 8) + addr.a[i].addr[j + 1], j == 14 ? "" : ":");
			}
#endif
		}
		if (addr.a[i].scope_id)
			printf("%%%u", addr.a[i].scope_id);
		printf("\n");
	}
	fflush(stdout);
	do_exit(RET_OK);
	return NULL;
}

static unsigned char *version_cmd(struct option *o, unsigned char ***argv, int *argc)
{
	printf("Links " VERSION_STRING "\n");
	fflush(stdout);
	do_exit(RET_OK);
	return NULL;
}

static unsigned char *set_cmd(struct option *o, unsigned char ***argv, int *argc)
{
	*(int *)o->ptr = 1;
	return NULL;
}

static unsigned char *unset_cmd(struct option *o, unsigned char ***argv, int *argc)
{
	*(int *)o->ptr = 0;
	return NULL;
}

static unsigned char *setstr_cmd(struct option *o, unsigned char ***argv, int *argc)
{
	if (!*argc) return cast_uchar "Parameter expected";
	safe_strncpy(o->ptr, **argv, o->max);
	(*argv)++; (*argc)--;
	return NULL;
}

static unsigned char *dump_cmd(struct option *o, unsigned char ***argv, int *argc)
{
	if (dmp != o->min && dmp) return cast_uchar "Can't use both -dump and -source";
	dmp = o->min;
	no_connect = 1;
	return NULL;
}

static unsigned char *printhelp_cmd(struct option *o, unsigned char ***argv, int *argc)
{
/* Changed and splited - translation is much easier.
 * Print to stdout instead stderr (,,links -help | more''
 * is much better than ,,links -help 2>&1 | more'').
 */
fprintf(stdout, "%s%s%s%s%s%s\n",

(
"links [options] URL\n"
"\n"
"Options are:\n"
"\n"
" -help\n"
"  Prints this help screen\n"
"\n"
" -version\n"
"  Prints the links version number and exit.\n"
"\n"
" -lookup <hostname>\n"
"  Does name lookup, like command \"host\".\n"
"\n"
" -g\n"
"  Run in graphics mode.\n"
"\n"
" -no-g\n"
"  Run in text mode (overrides previous -g).\n"
"\n"
" -driver <driver name>\n"
"  Graphics driver to use. Drivers are: x, svgalib, fb, directfb, pmshell,\n"
"    atheos.\n"
"  List of drivers will be shown if you give it an unknown driver.\n"
"  Available drivers depend on your operating system and available libraries.\n"
"\n"
" -mode <graphics mode>\n"
"  Graphics mode. For SVGALIB it is in format COLUMNSxROWSxCOLORS --\n"
"    for example 640x480x256, 800x600x64k, 960x720x16M, 1024x768x16M32\n"
"    List of modes will be shown if you give it an unknown videomode.\n"
"  For framebuffer it is number of pixels in border --- LEFT,TOP,RIGHT,BOTTOM\n"
"    other 3 values are optional --- i.e. -mode 10 will set all borders to 10,\n"
"    -mode 10,20 will set left & right border to 10 and top & bottom to 20.\n"
"  For Xwindow it is size of a window in format WIDTHxHEIGHT.\n"
"\n"
" -display <x-display>\n"
"  Set Xwindow display.\n"
"\n"
" -force-html\n"
"  Treat files with unknown type as html rather than text.\n"
"    (can be toggled with '\\' key)\n"
"\n"
" -source <url>\n"
"  Write unformatted data stream to stdout.\n"
"\n"
" -dump <url>\n"
"  Write formatted document to stdout.\n"
"\n"
" -width <number>\n"
"  For dump, document will be formatted to this screen width (but it can still\n"
"    exceed it if lines can't be broken).\n"
"\n"
" -codepage <codepage>\n"
"  For dump, convert output to specified character set --\n"
"    for eaxmple iso-8859-2, windows-1250.\n"
"\n"
" -anonymous\n"
"  Restrict links so that it can run on an anonymous account.\n"
"  No local file browsing. No downloads. Executing viewers\n"
"    is allowed, but user can't add or modify entries in\n"
"    association table.\n"
"\n"
" -no-connect\n"
"  Runs links as a separate instance - instead of connecting to\n"
"    existing instance.\n"
"\n"
" -download-dir <path>\n"
"  Default download directory.\n"
"    (default: actual dir)\n"
"\n"
" -language <language>\n"
"  Set user interface language.\n"
"\n"
" -max-connections <max>\n"
"  Maximum number of concurrent connections.\n"
"    (default: 10)\n"
"\n"),(
" -max-connections-to-host <max>\n"
"  Maximum number of concurrent connection to a given host.\n"
"    (default: 2)\n"
"\n"
" -retries <retry>\n"
"  Number of retries.\n"
"    (default: 3)\n"
"\n"
" -receive-timeout <sec>\n"
"  Timeout on receive.\n"
"    (default: 120)\n"
"\n"),(
" -unrestartable-receive-timeout <sec>\n"
"  Timeout on non restartable connections.\n"
"    (default: 600)\n"
"\n"
" -bind-address <ip address>\n"
"  Use a specific local IP address.\n"
"\n"
" -bind-address-ipv6 <ipv6 address>\n"
"  Use a specific local IPv6 address.\n"
"\n"
" -no-libevent\n"
"  Don't use libevent library.\n"
"\n"
" -no-openmp\n"
"  Don't use OpenMP.\n"
"\n"
" -async-dns <0>/<1>\n"
"  Asynchronous DNS resolver on(1)/off(0).\n"
"\n"
" -download-utime <0>/<1>\n"
"  Set time of downloaded files to last modification time reported by server.\n"
"\n"
" -format-cache-size <num>\n"
"  Number of formatted document pages cached.\n"
"    (default: 5)\n"
"\n"
" -memory-cache-size <bytes>\n"
"  Cache memory in bytes.\n"
"    (default: 1048576)\n"
"\n"
" -image-cache-size <bytes>\n"
"  Image cache in bytes.\n"
"    (default: 1048576)\n"
"\n"
" -font-cache-size <bytes>\n"
"  Font cache in bytes.\n"
"    (default: 2097152)\n"
"\n"
" -aggressive-cache <0>/<1>\n"
"    (default 1)\n"
"  Always cache everything regardless of server's caching recomendations.\n"
"    Many servers deny caching even if their content is not changing\n"
"    just to get more hits and more money from ads.\n"
"\n"),(
" -address-preference <0>/<1>/<2>/<3>/<4>\n"
"    (default 0)\n"
"  0 - use system default.\n"
"  1 - prefer IPv4.\n"
"  2 - prefer IPv6.\n"
"  3 - use only IPv4.\n"
"  4 - use only IPv6.\n"
"\n"
" -http-proxy <host:port>\n"
"  Host and port number of the HTTP proxy, or blank.\n"
"    (default: blank)\n"
"\n"
" -ftp-proxy <host:port>\n"
"  Host and port number of the FTP proxy, or blank.\n"
"    (default: blank)\n"
"\n"
" -https-proxy <host:port>\n"
"  Host and port number of the HTTPS proxy, or blank.\n"
"    (default: blank)\n"
"\n"
" -socks-proxy <user@host:port>\n"
"  Userid, host and port of Socks4a, or blank.\n"
"    (default: blank)\n"
"\n"
" -append-text-to-dns-lookups <text>\n"
"  Append text to dns lookups. It is useful for specifying fixed\n"
"    tor exit node.\n"
"    (default: blank)\n"
"\n"
" -only-proxies <0>/<1>\n"
"    (default 0)\n"
"  \"1\" causes that Links won't initiate any non-proxy connection.\n"
"    It is useful for anonymization with tor or similar networks.\n"
"\n"
" -ssl.certificates <0>/<1>/<2>\n"
"    (default 1)\n"
"  0 - ignore invalid certificate\n"
"  1 - warn on invalid certificate\n"
"  2 - reject invalid certificate\n"
"\n"
" -ssl.client-cert-key <filename>\n"
"  Name of the PEM encoded file with the user private key\n"
"  for client certificate authentication.\n"
"\n"
" -ssl.client-cert-crt <filename>\n"
"  Name of the PEM encoded file with the user certificate\n"
"  for client certificate authentication.\n"
"\n"
" -ssl.client-cert-password <text>\n"
"  Password for the user private key.\n"
"\n"),(
" -http-bugs.http10 <0>/<1>\n"
"    (default 0)\n"
"  \"1\" forces using only HTTP/1.0 protocol. (useful for buggy servers\n"
"    that claim to be HTTP/1.1 compliant but are not)\n"
"  \"0\" use both HTTP/1.0 and HTTP/1.1.\n"
"\n"
" -http-bugs.allow-blacklist <0>/<1>\n"
"    (default 1)\n"
"  \"1\" defaults to using list of servers that have broken HTTP/1.1 support.\n"
"     When links finds such server, it will retry the request with HTTP/1.0.\n"
"\n"
" -http-bugs.bug-302-redirect <0>/<1>\n"
"    (default 1)\n"
"  Process 302 redirect in a way that is incompatible with RFC1945 and RFC2068,\n"
"    but the same as Netscape and MSIE. Many pages depend on it.\n"
"\n"
" -http-bugs.bug-post-no-keepalive <0>/<1>\n"
"    (default 0)\n"
"  No keepalive connection after post requests. For some buggy servers.\n"
"\n"
" -http-bugs.bug-no-accept-charset <0>/<1>\n"
"    (default 0)\n"
"  Do not send Accept-Charset field of HTTP header. Because it is too long\n"
"    some servers will deny the request. Other servers will convert content\n"
"    to plain ascii when Accept-Charset is missing.\n"
"\n"
" -http-bugs.no-compression <0>/<1>\n"
"    (default 0)\n"
"  \"1\" causes that links won't advertise HTTP compression support (but it\n"
"    will still accept compressed data). Use it when you communicate with\n"
"    server that has broken compression support.\n"
"\n"
" -http-bugs.retry-internal-errors <0>/<1>\n"
"    (default 0)\n"
"  Retry on internal server errors (50x).\n"
"\n"
" -http.fake-firefox <0>/<1>\n"
"    (default 0)\n"
"  Fake Firefox in the HTTP header.\n"
"\n"
" -http.do-not-track <0>/<1>\n"
"    (default 0)\n"
"  Send do not track request in the HTTP header.\n"
"\n"
" -http.referer <0>/<1>/<2>/<3>/<4>\n"
"    (default 4)\n"
"  0 - do not send referer.\n"
"  1 - send the requested URL as referer.\n"
"  2 - send fake referer.\n"
"  3 - send real referer.\n"
"  4 - send real referer only to the same server.\n"
"\n"
" -http.fake-referer <string>\n"
"  Fake referer value.\n"
"\n"
" -http.fake-user-agent <string>\n"
"  Fake user agent value.\n"
"\n"
" -http.extra-header <string>\n"
"  Extra string added to HTTP header.\n"
"\n"
" -ftp.anonymous-password <string>\n"
"  Password for anonymous ftp access.\n"
"\n"
" -ftp.use-passive <0>/<1>\n"
"  Use ftp PASV command to bypass firewalls.\n"
"\n"
" -ftp.use-eprt-epsv <0>/<1>\n"
"  Use EPRT and EPSV commands instead of PORT and PASV.\n"
"\n"
" -ftp.fast <0>/<1>\n"
"  Send more ftp commands simultaneously. Faster response when\n"
"    browsing ftp directories, but it is incompatible with RFC\n"
"    and some servers don't like it.\n"
"\n"
" -ftp.set-iptos <0>/<1>\n"
"  Set IP Type-of-service to high throughput on ftp connections.\n"
"\n"
" -smb.allow-hyperlinks-to-smb <0>/<1>\n"
"  Allow hyperlinks to SMB protocol.\n"
"    Disabling this improves security, because internet sites cannot\n"
"    exploit possible bugs in the SMB client.\n"
"\n"
" -menu-font-size <size>\n"
"  Size of font in menu.\n"
"\n"
" -menu-background-color 0xRRGGBB\n"
"  Set menu background color in graphics mode, RRGGBB are hex.\n"
"\n"
" -menu-foreground-color 0xRRGGBB\n"
"  Set menu foreground color in graphics mode.\n"
"\n"
" -scroll-bar-area-color 0xRRGGBB\n"
"  Set color of scroll bar area.\n"
"\n"
" -scroll-bar-bar-color 0xRRGGBB\n"
"  Set color of scroll bar.\n"
"\n"
" -scroll-bar-frame-color 0xRRGGBB\n"
"  Set color of scroll bar frame.\n"
"\n"
" -bookmarks-file <file>\n"
"  File to store bookmarks.\n"
"\n"
" -bookmarks-codepage <codepage>\n"
"  Character set of bookmarks file.\n"
"\n"
" -save-url-history <0>/<1>\n"
"  Save URL history on exit.\n"
"\n"
" -display-red-gamma <fp-value>\n"
"  Red gamma of display.\n"
"    (default 2.2)\n"
"\n"
" -display-green-gamma <fp-value>\n"
"  Green gamma of display.\n"
"    (default 2.2)\n"
"\n"
" -display-blue-gamma <fp-value>\n"
"  Blue gamma of display.\n"
"    (default 2.2)\n"
"\n"
" -user-gamma <fp-value>\n"
"  Additional gamma.\n"
"    (default 1)\n"
"\n"
" -bfu-aspect <fp-value>\n"
"  Display aspect ration.\n"
"\n"
" -dither-letters <0>/<1>\n"
"  Do letter dithering.\n"
"\n"
" -dither-images <0>/<1>\n"
"  Do image dithering.\n"
"\n"
" -display-optimize <0>/<1>/<2>\n"
"  Optimize for CRT (0), LCD RGB (1), LCD BGR (2).\n"
"\n"
" -gamma-correction <0>/<1>/<2>\n"
"  Type of gamma correction:\n"
"    (default 2)\n"
"  0 - 8-bit (fast).\n"
"  1 - 16-bit (slow).\n"
"  2 - automatically detect according to speed of FPU.\n"
"\n"
" -overwrite-instead-of-scroll <0>/<1>\n"
"  Overwrite the screen instead of scrolling it\n"
"    (valid for svgalib and framebuffer).\n"
"    Overwriting may or may not be faster, depending on hardware.\n"
"\n"
#ifdef JS
" -enable-javascript <0>/<1>\n"
"  Enable javascript.\n"
"\n"
" -js.verbose-errors <0>/<1>\n"
"  Display javascript errors.\n"
"\n"
" -js.verbose-warnings <0>/<1>\n"
"  Display javascript warnings.\n"
"\n"
" -js.enable-all-conversions <0>/<1>\n"
"  Enable conversions between all types in javascript.\n"
"\n"
" -js.enable-global-resolution <0>/<1>\n"
"  Resolve global names.\n"
"\n"
" -js.manual-confirmation <0>/<1>\n"
"  Ask user to confirm potentially dangerous operations.\n"
"    (opening windows, going to url etc.) Default 1.\n"
"\n"
" -js.recursion-depth <integer>\n"
"  Depth of javascript call stack.\n"
"\n"
" -js.memory-limit <memory amount>\n"
"  Amount of kilobytes the javascript may allocate.\n"
"\n"
#endif
" -html-assume-codepage <codepage>\n"
"  If server didn't specify document character set, assume this.\n"
"\n"
" -html-hard-assume <0>/<1>\n"
"  Use always character set from \"-html-assume-codepage\" no matter\n"
"    what server sent.\n"
"\n"
" -html-tables <0>/<1>\n"
"  Render tables. (0) causes tables being rendered like in lynx.\n"
"\n"
" -html-frames <0>/<1>\n"
"  Render frames. (0) causes frames  rendered like in lynx.\n"
"\n"
" -html-break-long-lines <0>/<1>\n"
"  Break long lines in <pre> sections.\n"
"\n"
" -html-images <0>/<1>\n"
"  Display links to unnamed images as [IMG].\n"
"\n"
" -html-image-names <0>/<1>\n"
"  Display filename of an image instead of [IMG].\n"
"\n"
" -html-display-images <0>/<1>\n"
"  Display images in graphics mode.\n"
"\n"
" -html-image-scale <percent>\n"
"  Scale images in graphics mode.\n"
"\n"
" -html-bare-image-autoscale <0>/<1>\n"
"  Autoscale images displayed on full screen.\n"
"\n"
" -html-numbered-links <0>/<1>\n"
"  Number links in text mode. Allow quick link selection by typing\n"
"    link number and enter.\n"
"\n"
" -html-table-order <0>/<1>\n"
"  In text mode, walk through table by rows (0) or columns (1).\n"
"\n"
" -html-auto-refresh <0>/<1>\n"
"  Process refresh to other page (1), or display link to that page (0).\n"
"\n"
" -html-target-in-new-window <0>/<1>\n"
"  Allow opening new windows from html.\n"
"\n"
" -html-margin <number of spaces>\n"
"  Margin in text mode.\n"
"\n"
" -html-user-font-size <size>\n"
"  Size of font on pages in graphics mode.\n"
"\n"
" -html-t-text-color <0>-<15>\n"
"  Text color in text mode.\n"
"\n"
" -html-t-link-color <0>-<15>\n"
"  Link color in text mode.\n"
"\n"
" -html-t-background-color <0>-<7>\n"
"  Background color in text mode.\n"
"\n"
" -html-t-ignore-document-color <0>/<1>\n"
"  Ignore colors specified in html document in text mode.\n"
"\n"
" -html-g-text-color 0xRRGGBB\n"
"  Text color in graphics mode.\n"
"\n"
" -html-g-link-color 0xRRGGBB\n"
"  Link color in graphics mode.\n"
"\n"
" -html-g-background-color 0xRRGGBB\n"
"  Background color in graphics mode.\n"
"\n"
" -html-g-ignore-document-color <0>/<1>\n"
"  Ignore colors specified in html document in graphics mode.\n"
"\n"),(
"Keys:\n"
"	ESC	  display menu\n"
"	^C	  quit\n"
"	^P	  scroll up\n"
"	^N	  scroll down\n"
"	[, ]	  scroll left, right\n"
"	up, down  select link\n"
"	->, enter follow link\n"
"	<-, z	  go back\n"
"	g	  go to url\n"
"	G	  go to url based on current url\n"
"	^G	  go to url based on current link\n"
"	^R	  reload\n"
"	/	  search\n"
"	?	  search back\n"
"	n	  find next\n"
"	N	  find previous\n"
"	=	  document info\n"
"	\\	  document source\n"
"	|	  HTTP header\n"
"	*	  toggle displaying of image links (text mode)\n"
"	d	  download\n"
"	s	  bookmarks\n"
"	q	  quit or close current window\n"
"	^X	  cut to clipboard\n"
"	^B	  copy to clipboard\n"
"	^V	  paste from clipboard\n"
"	^K	  cut line (in textarea) or text to the end (in field)\n"
"	^U	  cut all text before cursor\n"
"	^W	  autocomplete url\n"
"	Alt-1 .. Alt-9\n"
"		  switch virtual screens (svgalib and framebuffer)\n"
"\n"
"Keys for braille terminal:\n"
"       arrows	  move the cursor\n"
"       enter	  follow link\n"
"	a	  cursor to status line\n"
"	w	  cursor to title\n"
"	^Y	  next link\n"
"	^T	  previous link\n"
"	y	  next word\n"
"	t	  previous word\n"
"	^O	  next form field entry\n"
));

	fflush(stdout);
	do_exit(RET_OK);
	return NULL;
}

void end_config(void)
{
	struct driver_param *dp;
	foreach(dp,driver_params)
	{
		if (dp->param)mem_free(dp->param);
		if (dp->shell)mem_free(dp->shell);
	}
	free_list(driver_params);
	if (links_home) mem_free(links_home);
}

int ggr = 0;
int force_g = 0;
unsigned char ggr_drv[MAX_STR_LEN] = "";
unsigned char ggr_mode[MAX_STR_LEN] = "";
unsigned char ggr_display[MAX_STR_LEN] = "";

int anonymous = 0;

unsigned char default_target[MAX_STR_LEN] ="";

unsigned char *links_home = NULL;
int first_use = 0;

int disable_libevent = 0;
int disable_openmp = 0;
int no_connect = 0;
int base_session = 0;
int dmp = 0;
int screen_width = 80;
int dump_codepage = -1;
int force_html = 0;

#ifdef DOS
/* DOS networking is slow with too many connections */
int max_connections = 3;
int max_connections_to_host = 2;
#else
int max_connections = 10;
int max_connections_to_host = 8;
#endif
int max_tries = 3;
int receive_timeout = 120;
int unrestartable_receive_timeout = 600;
unsigned char bind_ip_address[16] = "";
unsigned char bind_ipv6_address[INET6_ADDRSTRLEN] = "";
int async_lookup = 1;
int download_utime = 0;

int max_format_cache_entries = 5;
int memory_cache_size = 4194304;
int image_cache_size = 1048576;
int font_cache_size = 2097152;
int aggressive_cache = 1;

struct ipv6_options ipv6_options = { ADDR_PREFERENCE_DEFAULT };
struct proxies proxies = { "", "", "", "", "", 0 };
struct ssl_options ssl_options = { SSL_WARN_ON_INVALID_CERTIFICATE, "", "", "" };
struct http_options http_options = { 0, 1, 1, 0, 0, 0, 0, { 0, 0, REFERER_REAL_SAME_SERVER, "", "", "" } };
struct ftp_options ftp_options = { "somebody@host.domain", 0, 0, 0, 1 };
struct smb_options smb_options = { 0 };

unsigned char download_dir[MAX_STR_LEN] = "";

static int sink;
int js_enable=1;   /* 0=disable javascript */
int js_verbose_errors=0;   /* 1=create dialog on every javascript error, 0=be quiet and continue */
int js_verbose_warnings=0;   /* 1=create dialog on every javascript warning, 0=be quiet and continue */
int js_all_conversions=1;
int js_global_resolve=1;	/* resolvovani v globalnim adresnim prostoru, kdyz BFU vomitne document */
int js_manual_confirmation=1; /* !0==annoying dialog on every goto url etc. */
int js_fun_depth=100;
int js_memory_limit=5*1024;  /* in kilobytes, should be in range 1M-20M (1MB=1024*1024B) */

double display_red_gamma=2.2; /* Red gamma exponent of the display */
double display_green_gamma=2.2; /* Green gamma exponent of the display */
double display_blue_gamma=2.2; /* Blue gamma exponent of the display */
double user_gamma=1.0; /* 1.0 for 64 lx. This is the number user directly changes in the menu */
double bfu_aspect=1; /* 0.1 to 10.0, 1.0 default. >1 makes circle wider */
int display_optimize=0;	/*0=CRT, 1=LCD RGB, 2=LCD BGR */
int dither_letters=1;
int dither_images=1;
int gamma_bits=2;	/*0 --- 8, 1 --- 16, 2 --- auto */
int overwrite_instead_of_scroll = 1;


int menu_font_size = G_BFU_DEFAULT_FONT_SIZE;

unsigned G_BFU_FG_COLOR = G_DEFAULT_BFU_FG_COLOR;
unsigned G_BFU_BG_COLOR = G_DEFAULT_BFU_BG_COLOR;
unsigned G_SCROLL_BAR_AREA_COLOR = G_DEFAULT_SCROLL_BAR_AREA_COLOR;
unsigned G_SCROLL_BAR_BAR_COLOR = G_DEFAULT_SCROLL_BAR_BAR_COLOR;
unsigned G_SCROLL_BAR_FRAME_COLOR = G_DEFAULT_SCROLL_BAR_FRAME_COLOR;

unsigned char bookmarks_file[MAX_STR_LEN]="";
int bookmarks_codepage=0; /* changed to utf-8 table in init_charset() */

int save_history = 1;

struct document_setup dds = {
	0, /* assumed codepage */
	0, /* ignore codepage from server */
	1, /* tables */
	1, /* frames */
	0, /* break_long_lines */
	0, /* images */
	0, /* image_names */
	3, /* margin */
	0, /* num_links */
	0, /* table_order */
	0, /* auto_refresh */
	20, /* font_size */
	1, /* display images */
	100, /* image scale */
	0, /* porn enable */
	0, /* target in new window */
	7, /* t text color */
	15, /* t link color */
	0, /* t background color */
	0, /* t ignore document color */
	0x000000, /* g text color */
	0x0000ff, /* g link color */
	0xc0c0c0, /* g background color */
	0, /* g ignore document color */
};

static struct option links_options[] = {
	{1, printhelp_cmd, NULL, NULL, 0, 0, NULL, NULL, "?"},
	{1, printhelp_cmd, NULL, NULL, 0, 0, NULL, NULL, "h"},
	{1, printhelp_cmd, NULL, NULL, 0, 0, NULL, NULL, "help"},
	{1, printhelp_cmd, NULL, NULL, 0, 0, NULL, NULL, "-help"},
	{1, version_cmd, NULL, NULL, 0, 0, NULL, NULL, "version"},
	{1, lookup_cmd, NULL, NULL, 0, 0, NULL, NULL, "lookup"},
	{1, set_cmd, NULL, NULL, 0, 0, &no_connect, NULL, "no-connect"},
	{1, set_cmd, NULL, NULL, 0, 0, &anonymous, NULL, "anonymous"},
	{1, set_cmd, NULL, NULL, 0, 0, &ggr, NULL, "g"},
	{1, unset_cmd, NULL, NULL, 0, 0, &ggr, NULL, "no-g"},
	{1, setstr_cmd, NULL, NULL, 0, MAX_STR_LEN, ggr_drv, NULL, "driver"},
	{1, setstr_cmd, NULL, NULL, 0, MAX_STR_LEN, default_target, NULL, "target"},
	{1, setstr_cmd, NULL, NULL, 0, MAX_STR_LEN, ggr_mode, NULL, "mode"},
	{1, setstr_cmd, NULL, NULL, 0, MAX_STR_LEN, ggr_display, NULL, "display"},
	{1, gen_cmd, num_rd, NULL, 0, MAXINT, &base_session, NULL, "base-session"},
	{1, set_cmd, NULL, NULL, 0, 0, &force_html, NULL, "force-html"},
	{1, dump_cmd, NULL, NULL, D_SOURCE, 0, NULL, NULL, "source"},
	{1, dump_cmd, NULL, NULL, D_DUMP, 0, NULL, NULL, "dump"},
	{1, gen_cmd, num_rd, NULL, 10, 512, &screen_width, "dump_width", "width" },
	{1, gen_cmd, cp_rd, NULL, 1, 0, &dump_codepage, "dump_codepage", "codepage" },
	{1, gen_cmd, str_rd, str_wr, 0, MAX_STR_LEN, download_dir, "download_dir", "download-dir"},
	{1, gen_cmd, lang_rd, lang_wr, 0, 0, &current_language, "language", "language"},
	{1, gen_cmd, num_rd, num_wr, 1, 99, &max_connections, "max_connections", "max-connections"},
	{1, gen_cmd, num_rd, num_wr, 1, 99, &max_connections_to_host, "max_connections_to_host", "max-connections-to-host"},
	{1, gen_cmd, num_rd, num_wr, 0, 16, &max_tries, "retries", "retries"},
	{1, gen_cmd, num_rd, num_wr, 1, 9999, &receive_timeout, "receive_timeout", "receive-timeout"},
	{1, gen_cmd, num_rd, num_wr, 1, 9999, &unrestartable_receive_timeout, "unrestartable_receive_timeout", "unrestartable-receive-timeout"},
	{1, gen_cmd, ip_rd, str_wr, 0, 16, bind_ip_address, "bind_address", "bind-address"},
	{1, gen_cmd, ipv6_rd, str_wr, 0, INET6_ADDRSTRLEN, bind_ipv6_address, "bind_address_ipv6", "bind-address-ipv6"},
	{1, set_cmd, NULL, NULL, 0, 0, &disable_libevent, NULL, "no-libevent"},
	{1, set_cmd, NULL, NULL, 0, 0, &disable_openmp, NULL, "no-openmp"},
	{1, gen_cmd, num_rd, num_wr, 0, 1, &async_lookup, "async_dns", "async-dns"},
	{1, gen_cmd, num_rd, num_wr, 0, 1, &download_utime, "download_utime", "download-utime"},
	{1, gen_cmd, num_rd, num_wr, 0, 999, &max_format_cache_entries, "format_cache_size", "format-cache-size"},
	{1, gen_cmd, num_rd, num_wr, 0, MAXINT, &memory_cache_size, "memory_cache_size", "memory-cache-size"},
	{1, gen_cmd, num_rd, num_wr, 0, MAXINT, &image_cache_size, "image_cache_size", "image-cache-size"},
	{1, gen_cmd, num_rd, num_wr, 0, MAXINT, &font_cache_size, "font_cache_size", "font-cache-size"},
	{1, gen_cmd, num_rd, num_wr, 0, 1, &aggressive_cache, "http_bugs.aggressive_cache", "aggressive-cache"},
	{1, gen_cmd, num_rd, num_wr, 0, 4, &ipv6_options.addr_preference, "ipv6.address_preference", "address-preference"},
	{1, gen_cmd, str_rd, str_wr, 0, MAX_STR_LEN, proxies.http_proxy, "http_proxy", "http-proxy"},
	{1, gen_cmd, str_rd, str_wr, 0, MAX_STR_LEN, proxies.ftp_proxy, "ftp_proxy", "ftp-proxy"},
	{1, gen_cmd, str_rd, str_wr, 0, MAX_STR_LEN, proxies.https_proxy, "https_proxy", "https-proxy"},
	{1, gen_cmd, str_rd, str_wr, 0, MAX_STR_LEN, proxies.socks_proxy, "socks_proxy", "socks-proxy"},
	{1, gen_cmd, str_rd, NULL, 0, MAX_STR_LEN, proxies.dns_append, "-append_text_to_dns_lookups", NULL}, /* old version incorrectly saved it with '-' */
	{1, gen_cmd, str_rd, str_wr, 0, MAX_STR_LEN, proxies.dns_append, "append_text_to_dns_lookups", "append-text-to-dns-lookups"},
	{1, gen_cmd, num_rd, num_wr, 0, 1, &proxies.only_proxies, "only_proxies", "only-proxies"},
	{1, gen_cmd, num_rd, num_wr, 0, 2, &ssl_options.certificates, "ssl.certificates", "ssl.certificates"},
	{1, gen_cmd, str_rd, str_wr, 0, MAX_STR_LEN, &ssl_options.client_cert_key, "ssl.client_cert_key", "ssl.client-cert-key"},
	{1, gen_cmd, str_rd, str_wr, 0, MAX_STR_LEN, &ssl_options.client_cert_crt, "ssl.client_cert_crt", "ssl.client-cert-crt"},
	{1, gen_cmd, str_rd, NULL, 0, MAX_STR_LEN, &ssl_options.client_cert_password, NULL, "ssl.client-cert-password"},
	{1, gen_cmd, str_rd, NULL, 0, MAX_STR_LEN, &ssl_options.client_cert_key, "client_cert_key", "http.client_cert_key"}, /* backward compatibility with Debian patches */
	{1, gen_cmd, str_rd, NULL, 0, MAX_STR_LEN, &ssl_options.client_cert_crt, "client_cert_crt", "http.client_cert_crt"}, /* backward compatibility with Debian patches */
	{1, gen_cmd, num_rd, num_wr, 0, 1, &http_options.http10, "http_bugs.http10", "http-bugs.http10"},
	{1, gen_cmd, num_rd, num_wr, 0, 1, &http_options.allow_blacklist, "http_bugs.allow_blacklist", "http-bugs.allow-blacklist"},
	{1, gen_cmd, num_rd, num_wr, 0, 1, &http_options.bug_302_redirect, "http_bugs.bug_302_redirect", "http-bugs.bug-302-redirect"},
	{1, gen_cmd, num_rd, num_wr, 0, 1, &http_options.bug_post_no_keepalive, "http_bugs.bug_post_no_keepalive", "http-bugs.bug-post-no-keepalive"},
	{1, gen_cmd, num_rd, num_wr, 0, 1, &http_options.no_accept_charset, "http_bugs.no_accept_charset", "http-bugs.bug-no-accept-charset"},
	{1, gen_cmd, num_rd, num_wr, 0, 1, &http_options.no_compression, "http_bugs.no_compression", "http-bugs.no-compression"},
	{1, gen_cmd, num_rd, num_wr, 0, 1, &http_options.retry_internal_errors, "http_bugs.retry_internal_errors", "http-bugs.retry-internal-errors"},
	{1, gen_cmd, num_rd, num_wr, 0, 1, &http_options.header.fake_firefox, "fake_firefox", "http.fake-firefox"},
	{1, gen_cmd, num_rd, num_wr, 0, 1, &http_options.header.do_not_track, "http_do_not_track", "http.do-not-track"},
	{1, gen_cmd, num_rd, num_wr, 0, 4, &http_options.header.referer, "http_referer", "http.referer"},
	{1, gen_cmd, str_rd, str_wr, 0, MAX_STR_LEN, &http_options.header.fake_referer, "fake_referer", "http.fake-referer"},
	{1, gen_cmd, str_rd, str_wr, 0, MAX_STR_LEN, &http_options.header.fake_useragent, "fake_useragent", "http.fake-user-agent"},
	{1, gen_cmd, str_rd, str_wr, 0, MAX_STR_LEN, &http_options.header.extra_header, "http.extra_header", "http.extra-header"},
	{1, gen_cmd, str_rd, str_wr, 0, MAX_STR_LEN, ftp_options.anon_pass, "ftp.anonymous_password", "ftp.anonymous-password"},
	{1, gen_cmd, num_rd, num_wr, 0, 1, &ftp_options.passive_ftp, "ftp.use_passive", "ftp.use-passive"},
	{1, gen_cmd, num_rd, num_wr, 0, 1, &ftp_options.eprt_epsv, "ftp.use_eprt_epsv", "ftp.use-eprt-epsv"},
	{1, gen_cmd, num_rd, num_wr, 0, 1, &ftp_options.fast_ftp, "ftp.fast", "ftp.fast"},
	{1, gen_cmd, num_rd, num_wr, 0, 1, &ftp_options.set_tos, "ftp.set_iptos", "ftp.set-iptos"},
	{1, gen_cmd, num_rd, num_wr, 0, 1, &smb_options.allow_hyperlinks_to_smb, "smb.allow_hyperlinks_to_smb", "smb.allow-hyperlinks-to-smb"},
	{1, gen_cmd, num_rd, num_wr, 1, MAX_FONT_SIZE, &menu_font_size, "menu_font_size", "menu-font-size"},
	{1, gen_cmd, num_rd, num_wr, 0, 0xffffff, &G_BFU_BG_COLOR, "background_color", "menu-background-color"},
	{1, gen_cmd, num_rd, num_wr, 0, 0xffffff, &G_BFU_FG_COLOR, "foreground_color", "menu-foreground-color"},
	{1, gen_cmd, num_rd, num_wr, 0, 0xffffff, &G_SCROLL_BAR_AREA_COLOR, "scroll_bar_area_color", "scroll-bar-area-color"},
	{1, gen_cmd, num_rd, num_wr, 0, 0xffffff, &G_SCROLL_BAR_BAR_COLOR, "scroll_bar_bar_color", "scroll-bar-bar-color"},
	{1, gen_cmd, num_rd, num_wr, 0, 0xffffff, &G_SCROLL_BAR_FRAME_COLOR, "scroll_bar_frame_color", "scroll-bar-frame-color"},
	{1, gen_cmd, str_rd, str_wr, 0, MAX_STR_LEN, bookmarks_file, "bookmarks_file", "bookmarks-file"},
	{1, gen_cmd, cp_rd, cp_wr, 0, 0, &bookmarks_codepage, "bookmarks_codepage", "bookmarks-codepage"},
	{1, gen_cmd, num_rd, num_wr, 0, 1, &save_history, "save_url_history", "save-url-history"},
	{1, gen_cmd, dbl_rd, dbl_wr, 1, 10000, &display_red_gamma, "display_red_gamma", "display-red-gamma"},
	{1, gen_cmd, dbl_rd, dbl_wr, 1, 10000, &display_green_gamma, "display_green_gamma", "display-green-gamma"},
	{1, gen_cmd, dbl_rd, dbl_wr, 1, 10000, &display_blue_gamma, "display_blue_gamma", "display-blue-gamma"},
	{1, gen_cmd, dbl_rd, dbl_wr, 1, 10000, &user_gamma, "user_gamma", "user-gamma"},
	{1, gen_cmd, dbl_rd, dbl_wr, 25, 400, &bfu_aspect, "bfu_aspect", "bfu-aspect"},
	{1, gen_cmd, num_rd, NULL, 0, 1, &sink, "aspect_on", NULL},
	{1, gen_cmd, num_rd, num_wr, 0, 2, &display_optimize, "display_optimize", "display-optimize"},
	{1, gen_cmd, num_rd, num_wr, 0, 1, &dither_letters, "dither_letters", "dither-letters"},
	{1, gen_cmd, num_rd, num_wr, 0, 1, &dither_images, "dither_images", "dither-images"},
	{1, gen_cmd, num_rd, num_wr, 0, 2, &gamma_bits, "gamma_correction", "gamma-correction"},
	{1, gen_cmd, num_rd, num_wr, 0, 1, &overwrite_instead_of_scroll, "overwrite_instead_of_scroll", "overwrite-instead-of-scroll"},
	{1, gen_cmd, num_rd, NULL, 0, 1, &js_enable, "enable_javascript", NULL},
	{1, gen_cmd, num_rd, NULL, 0, 1, &js_verbose_errors, "verbose_javascript_errors", NULL},
	{1, gen_cmd, num_rd, NULL, 0, 1, &js_verbose_warnings, "verbose_javascript_warnings", NULL},
	{1, gen_cmd, num_rd, NULL, 0, 1, &js_all_conversions, "enable_all_conversions", NULL},
	{1, gen_cmd, num_rd, NULL, 0, 1, &js_global_resolve, "enable_global_resolution", NULL},
	{1, gen_cmd, num_rd, NULL, 0, 1, &js_manual_confirmation, "javascript_manual_confirmation", NULL},
	{1, gen_cmd, num_rd, NULL, 0, 999999, &js_fun_depth, "js_recursion_depth", NULL},
	{1, gen_cmd, num_rd, NULL, 1024, 30*1024, &js_memory_limit, "js_memory_limit", NULL},
	{1, gen_cmd, cp_rd, NULL, 0, 0, &dds.assume_cp, "assume_codepage", NULL},
	{1, NULL, term_rd, term_wr, 0, 0, NULL, "terminal", NULL},
	{1, NULL, term2_rd, NULL, 0, 0, NULL, "terminal2", NULL},
	{1, NULL, type_rd, type_wr, 0, 0, NULL, "association", NULL},
	{1, NULL, ext_rd, ext_wr, 0, 0, NULL, "extension", NULL},
	{1, NULL, prog_rd, prog_wr, 0, 0, &mailto_prog, "mailto", NULL},
	{1, NULL, prog_rd, prog_wr, 0, 0, &telnet_prog, "telnet", NULL},
	{1, NULL, prog_rd, prog_wr, 0, 0, &tn3270_prog, "tn3270", NULL},
	{1, NULL, prog_rd, prog_wr, 0, 0, &mms_prog, "mms", NULL},
	{1, NULL, prog_rd, prog_wr, 0, 0, &magnet_prog, "magnet", NULL},
	{1, NULL, block_rd, block_wr, 0, 0, NULL, "imageblock", NULL},
	{1, NULL, dp_rd, dp_wr, 0, 0, NULL, "video_driver", NULL},
	{0, NULL, NULL, NULL, 0, 0, NULL, NULL, NULL},
};

static struct option html_options[] = {
	{1, gen_cmd, cp_rd, cp_wr, 0, 0, &dds.assume_cp, "html_assume_codepage", "html-assume-codepage"},
	{1, gen_cmd, num_rd, num_wr, 0, 1, &dds.hard_assume, "html_hard_assume", "html-hard-assume"},
	{1, gen_cmd, num_rd, num_wr, 0, 1, &dds.tables, "html_tables", "html-tables"},
	{1, gen_cmd, num_rd, num_wr, 0, 1, &dds.frames, "html_frames", "html-frames"},
	{1, gen_cmd, num_rd, num_wr, 0, 1, &dds.break_long_lines, "html_break_long_lines", "html-break-long-lines"},
	{1, gen_cmd, num_rd, num_wr, 0, 1, &dds.images, "html_images", "html-images"},
	{1, gen_cmd, num_rd, num_wr, 0, 1, &dds.image_names, "html_image_names", "html-image-names"},
	{1, gen_cmd, num_rd, num_wr, 0, 9, &dds.margin, "html_margin", "html-margin"},
	{1, gen_cmd, num_rd, num_wr, 0, 1, &dds.num_links, "html_numbered_links", "html-numbered-links"},
	{1, gen_cmd, num_rd, num_wr, 0, 1, &dds.table_order, "html_table_order", "html-table-order"},
	{1, gen_cmd, num_rd, num_wr, 0, 1, &dds.auto_refresh, "html_auto_refresh", "html-auto-refresh"},
	{1, gen_cmd, num_rd, num_wr, 1, MAX_FONT_SIZE, &dds.font_size, "html_font_size", "html-user-font-size"},
	{1, gen_cmd, num_rd, num_wr, 0, 1, &dds.display_images, "html_display_images", "html-display-images"},
	{1, gen_cmd, num_rd, num_wr, 1, 999, &dds.image_scale, "html_image_scale", "html-image-scale"},
	{1, gen_cmd, num_rd, num_wr, 0, 1, &dds.porn_enable, "html_bare_image_autoscale", "html-bare-image-autoscale"},
	{1, gen_cmd, num_rd, num_wr, 0, 1, &dds.target_in_new_window, "html_target_in_new_window", "html-target-in-new-window"},
	{1, gen_cmd, num_rd, num_wr, 0, 15, &dds.t_text_color, "html_text_color", "html-text-color"},
	{1, gen_cmd, num_rd, num_wr, 0, 15, &dds.t_link_color, "html_link_color", "html-link-color"},
	{1, gen_cmd, num_rd, num_wr, 0, 7, &dds.t_background_color, "html_background_color", "html-background-color"},
	{1, gen_cmd, num_rd, num_wr, 0, 1, &dds.t_ignore_document_color, "html_ignore_document_color", "html-ignore-document-color"},
	{1, gen_cmd, num_rd, num_wr, 0, 0xffffff, &dds.g_text_color, "html_g_text_color", "html-g-text-color"},
	{1, gen_cmd, num_rd, num_wr, 0, 0xffffff, &dds.g_link_color, "html_g_link_color", "html-g-link-color"},
	{1, gen_cmd, num_rd, num_wr, 0, 0xffffff, &dds.g_background_color, "html_g_background_color", "html-g-background-color"},
	{1, gen_cmd, num_rd, num_wr, 0, 1, &dds.g_ignore_document_color, "html_g_ignore_document_color", "html-g-ignore-document-color"},
	{0, NULL, NULL, NULL, 0, 0, NULL, NULL, NULL},
};

static struct option *all_options[] = { links_options, html_options, NULL, };

unsigned char *parse_options(int argc, unsigned char *argv[])
{
	return p_arse_options(argc, argv, all_options);
}

static void load_config_file(unsigned char *prefix, unsigned char *name)
{
	unsigned char *c, *config_file;
	config_file = stracpy(prefix);
	if (!config_file) return;
	add_to_strn(&config_file, name);
	if ((c = read_config_file(config_file))) goto ok;
	mem_free(config_file);
	config_file = stracpy(prefix);
	if (!config_file) return;
	add_to_strn(&config_file, cast_uchar ".");
	add_to_strn(&config_file, name);
	if ((c = read_config_file(config_file))) goto ok;
	mem_free(config_file);
	return;
	ok:
	parse_config_file(config_file, c, all_options);
	mem_free(c);
	mem_free(config_file);
}

void load_config(void)
{
#ifdef SHARED_CONFIG_DIR
	load_config_file(cast_uchar SHARED_CONFIG_DIR, cast_uchar "links.cfg");
#endif
	load_config_file(links_home, cast_uchar "links.cfg");
	load_config_file(links_home, cast_uchar "html.cfg");
	load_config_file(links_home, cast_uchar "user.cfg");
}

void write_config(struct terminal *term)
{
#ifdef G
	if (F) update_driver_param();
#endif
	write_config_data(links_home, cast_uchar "links.cfg", links_options, term);
}

void write_html_config(struct terminal *term)
{
	write_config_data(links_home, cast_uchar "html.cfg", html_options, term);
}

void load_url_history(void)
{
	unsigned char *history_file, *hs;
	unsigned char *hsp;

	if (anonymous) return;
	/* Must have been called after init_home */
	if (!links_home) return;
	history_file = stracpy(links_home);
	add_to_strn(&history_file, cast_uchar "links.his");
	hs = read_config_file(history_file);
	mem_free(history_file);
	if (!hs) return;
	for (hsp = hs; *hsp; ) {
		unsigned char *hsl, *hsc;
		for (hsl = hsp; *hsl && *hsl != 10 && *hsl != 13; hsl++)
			;
		hsc = memacpy(hsp, hsl - hsp);
		add_to_history(&goto_url_history, hsc, 0);
		mem_free(hsc);
		hsp = hsl;
		while (*hsp == 10 || *hsp == 13) hsp++;
	}
	mem_free(hs);
}

void save_url_history(void)
{
	struct history_item *hi;
	unsigned char *history_file;
	unsigned char *hs;
	int hsl = 0;
	if (anonymous || !save_history || proxies.only_proxies) return;

	/* Must have been called after init_home */
	if (!links_home) return;
	history_file = stracpy(links_home);
	add_to_strn(&history_file, cast_uchar "links.his");
	hs = init_str();
	hsl = 0;
	foreachback(hi, goto_url_history.items) {
		if (!*hi->d || hi->d[0] == ' ' || strchr(cast_const_char hi->d, 10) || strchr(cast_const_char hi->d, 13)) continue;
		if (!url_not_saveable(hi->d)) {
			add_to_str(&hs, &hsl, hi->d);
			add_to_str(&hs, &hsl, cast_uchar NEWLINE);
		}
	}
	write_to_config_file(history_file, hs);
	mem_free(history_file);
	mem_free(hs);
	return;
}

