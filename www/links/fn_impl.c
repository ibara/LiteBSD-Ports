#include "links.h"

#ifndef HAVE_SNPRINTF

#define B_SZ	65536

static char snprtintf_buffer[B_SZ];

int my_snprintf(char *str, int n, char *f, ...)
{
	int i;
	va_list l;
	if (!n) return -1;
	va_start(l, f);
#ifdef HAVE_VPRINTF
	vsprintf(snprtintf_buffer, f, l);
#elif defined(HAVE_DOPRNT)
	{
		struct _iobuf strbuf;
		strbuf._flag = _IOWRT+_IOSTRG;
		strbuf._ptr = snprtintf_buffer;
		strbuf._cnt = 32767;
		_doprnt(f, l, &strbuf);
		putc('\0', &strbuf);
	}
#else
	fatal_exit("No vsprintf!");
#endif
	va_end(l);
	i = strlen(snprtintf_buffer);
	if (i >= B_SZ) {
		fatal_exit("String size too large!");
	}
	if (i >= n) {
		memcpy(str, snprtintf_buffer, n);
		str[n - 1] = 0;
		return -1;
	}
	strcpy(str, snprtintf_buffer);
	return i;
}

#endif

#ifndef HAVE_RAISE
int raise(int s)
{
#ifdef HAVE_GETPID
	pid_t p;
	EINTRLOOP(p, getpid());
	if (p == -1) return -1;
	return kill(p, s);
#else
	return 0;
#endif
};
#endif
#ifndef HAVE_GETTIMEOFDAY
int gettimeofday(struct timeval *tv, struct timezone *tz)
{
	time_t t;
	errno = 0;
	EINTRLOOPX(t, time(NULL), (time_t)-1);
	if (tv) tv->tv_sec = t, tv->tv_usec = 0;
	if (tz) tz->tz_minuteswest = tz->tz_dsttime = 0;
	return 0;
}
#endif
#ifndef HAVE_GETCWD
char *getcwd(char *buf, size_t size)
{
#ifndef MAXPATHLEN
#define	MAXPATHLEN	1024
#endif
	static char cwd[MAXPATHLEN];
	if (!getwd(cwd))
		return NULL;
	if (strlen(cwd) >= size) {
		errno = ERANGE;
		return NULL;
	}
	strcpy(buf, cwd);
	return 0;
}
#endif
#ifndef HAVE_TEMPNAM
char *tempnam(const char *dir, const char *pfx)
{
	static int counter = 0;
	unsigned char *d, *s, *a;
	int l;
	if (!(d = cast_uchar getenv("TMPDIR"))) {
		if (dir) d = cast_uchar dir;
		else if (!(d = cast_uchar getenv("TMP")) && !(d = cast_uchar getenv("TEMP"))) {
#ifdef P_tmpdir
			d = cast_uchar(P_tmpdir);
#else
			d = cast_uchar "/tmp";
#endif
		}
	}
	l = 0;
	s = init_str();
	add_to_str(&s, &l, d);
	if (s[0] && s[strlen(cast_const_char s) - 1] != '/') add_chr_to_str(&s, &l, '/');
	add_to_str(&s, &l, cast_uchar pfx);
	add_num_to_str(&s, &l, counter++);
	a = cast_uchar strdup(cast_const_char s);
	mem_free(s);
	return cast_char a;
}
#endif
#ifndef HAVE_SIGFILLSET
int sigfillset(sigset_t *set)
{
	memset(set, -1, sizeof(sigset_t));
	return 0;
}
#endif
#ifndef HAVE_STRDUP
char *strdup(const char *s)
{
	char *a = malloc(strlen(s) + 1);
	if (!a) return NULL;
	return strcpy(a, s);
}
#endif
#ifndef HAVE_STRTOL
long strtol(const char *nptr, char **endptr, int base)
{
	unsigned long result;
	char negative = 0;
	if (*nptr == '-') {
		negative = 1;
		nptr++;
	}
	result = 0;
	while (1) {
		char c = *nptr;
		char val;
		unsigned long nr;
		if (c >= '0' && c <= '9') val = c - '0';
		else if (c >= 'A' && c <= 'Z') val = c - 'A' + 10;
		else if (c >= 'a' && c <= 'z') val = c - 'a' + 10;
		else break;
		if (val >= base) break;
		nr = result * base + val;
		if ((long)nr < val || (nr - val) / base != result) break;
		result = nr;
		nptr++;
	}
	if (endptr) *endptr = (char *)nptr;
	if (negative) return -result;
	return result;
}
#endif
#ifndef HAVE_STRTOUL
unsigned long strtoul(const char *nptr, char **endptr, int base)
{
	if (*nptr == '-') {
		if (endptr) *endptr = nptr;
		return 0;
	}
	return (unsigned long)strtol(nptr,endptr,base);
};
#endif
#ifndef HAVE_STRTOD
double strtod(const char *nptr, char **endptr)
{
	double d = 0;
	char dummy;
	if (endptr) *endptr = (char *)nptr;
	if (sscanf(nptr, "%lf%c", &d, &dummy) == 1) {
		if (endptr) *endptr = strchr(nptr, 0);
	}
	return d;
}
#endif
#ifndef HAVE_STRLEN
size_t strlen(const char *s)
{
	size_t len = 0;
	while (s[len]) len++;
	return len;
}
#endif
#ifndef HAVE_STRCPY
char *strcpy(char *dst, const char *src)
{
	return memcpy(dst, src, strlen(src) + 1);
}
#endif
#ifndef HAVE_STRCHR
char *strchr(const char *s, int c)
{
	do {
		if (*s == (char)c)
			return (char *)s;
	} while (*s++);
	return NULL;
}
#endif
#ifndef HAVE_STRRCHR
char *strrchr(const char *s, int c)
{
	char *ret = NULL;
	do {
		if (*s == (char)c)
			ret = (char *)s;
	} while (*s++);
	return ret;
}
#endif
#ifndef HAVE_STRCMP
int strcmp(const char *s1, const char *s2)
{
	while (1) {
		unsigned char c1 = (unsigned char)*s1;
		unsigned char c2 = (unsigned char)*s2;
		if (c1 != c2) {
			return (int)c1 - (int)c2;
		}
		if (!c1) break;
		s1++, s2++;
	}
	return 0;
}
#endif
#ifndef HAVE_STRNCMP
int strncmp(const char *s1, const char *s2, size_t n)
{
	while (n--) {
		unsigned char c1 = (unsigned char)*s1;
		unsigned char c2 = (unsigned char)*s2;
		if (c1 != c2) {
			return (int)c1 - (int)c2;
		}
		if (!c1) break;
		s1++, s2++;
	}
	return 0;
}
#endif
#ifndef HAVE_STRCASECMP
int strcasecmp(const char *s1, const char *s2)
{
	while (1) {
		unsigned char c1 = (unsigned char)*s1;
		unsigned char c2 = (unsigned char)*s2;
		c1 = upcase(c1);
		c2 = upcase(c2);
		if (c1 != c2) {
			return (int)c1 - (int)c2;
		}
		if (!*s1) break;
		s1++, s2++;
	}
	return 0;
}
#endif
#ifndef HAVE_STRNCASECMP
int strncasecmp(const char *s1, const char *s2, size_t n)
{
	while (n--) {
		unsigned char c1 = (unsigned char)*s1;
		unsigned char c2 = (unsigned char)*s2;
		c1 = upcase(c1);
		c2 = upcase(c2);
		if (c1 != c2) {
			return (int)c1 - (int)c2;
		}
		if (!*s1) break;
		s1++, s2++;
	}
	return 0;
}
#endif
#ifndef HAVE_STRCSPN
size_t strcspn(const char *s, const char *reject)
{
	size_t r;
	for (r = 0; *s; r++, s++) {
		const char *rj;
		for (rj = reject; *rj; rj++) if (*s == *rj) goto brk;
	}
	brk:
	return r;
}
#endif
#ifndef HAVE_STRSPN
size_t strspn(const char *s, const char *accept)
{
	size_t r;
	for (r = 0; *s; r++, s++) {
		const char *rj;
		for (rj = accept; *rj; rj++) if (*s == *rj) goto ac;
		break;
		ac:;
	}
	return r;
}
#endif
#ifndef HAVE_STRSTR
char *strstr(const char *haystack, const char *needle)
{
	size_t hs = strlen(haystack);
	size_t ns = strlen(needle);
	while (hs >= ns) {
		if (!memcmp(haystack, needle, ns)) return (char *)haystack;
		haystack++, hs--;
	}
	return NULL;
}
#endif
#ifndef HAVE_MEMCHR
void *memchr(const void *s, int c, size_t length)
{
	const char *sp = s;
	while (length--) {
		if (*sp == (char)c)
			return (char *)s;
		sp++;
	}
	return NULL;
}
#endif
#ifndef HAVE_MEMCMP
int memcmp(const void *src0, const void *src1, size_t length)
{
	const unsigned char *s0, *s1;
#ifdef HAVE_BCMP
	if (!bcmp(src0, src1, length))
		return 0;
#endif
	s0 = src0;
	s1 = src1;
	while (length--) {
		int c = *s0 - *s1;
		if (c) return c;
		s0++, s1++;
	}
	return 0;
}
#endif
#ifndef HAVE_MEMCPY
void *memcpy(void *dst0, const void *src0, size_t length)
{
	return memmove(dst0, src0, length);
}
#endif
#ifndef HAVE_MEMMOVE
void *memmove(void *dst0, const void *src0, size_t length)
{
#ifdef HAVE_BCOPY
	bcopy(src0, dst0, length);
	return dst0;
#else
	unsigned char *dst = dst0;
	const unsigned char *src = src0;

	if ((const unsigned char *)dst == src || !length)
		return dst0;

	if ((const unsigned char *)dst <= src) {
		while (length--) *dst++ = *src++;
	} else {
		dst += length - 1;
		src += length - 1;
		while (length--) *dst-- = *src--;
	}
	return dst0;
#endif
}
#endif
#ifndef HAVE_MEMSET
void *memset(void *s, int c, size_t n)
{
	char *sc = s;
#ifdef HAVE_BZERO
	if (!c) bzero(s, n);
	else
#endif
	while (n--) *sc++ = c;
	return s;
}
#endif
#ifndef HAVE_STRERROR
extern char *sys_errlist[];
extern int sys_nerr;
char *strerror(int errnum)
{
	if (errnum < 0 || errnum >= sys_nerr) return "Unknown error";
	return sys_errlist[errnum];
};
#endif
