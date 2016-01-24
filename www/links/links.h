/* links.h
 * (c) 2002 Mikulas Patocka, Karel 'Clock' Kulhavy, Petr 'Brain' Kulhavy,
 *          Martin 'PerM' Pergel
 * This file is a part of the Links program, released under GPL.
 */

/*
 * WARNING: this file MUST be C++ compatible. this means:
 *	no implicit conversions from void *:
 *		BAD: unsigned char *c = mem_alloc(4);
 *		GOOD: unsigned char *c = (unsigned char *)mem_alloc(4);
 *	no implicit char * -> unsigned char * conversions:
 *		BAD: unsigned char *c = stracpy("A");
 *		GOOD: unsigned char *c = stracpy((unsigned char *)"A");
 *	no implicit unsigned char * -> char * conversions:
 *		BAD: unsigned char *x, *y, *z; z = strcpy(x, y);
 *		BAD: l = strlen(x);
 *		GOOD: unsigned char *x, *y; z = (unsigned char *)strcpy((char *)x, (char *)y);
 *		GOOD: l = strlen((char *)x);
 *	don't use C++ keywords (like template)
 *	if there is struct X, you cannot use variable X or typedef X
 *		(this applies to typedef ip as well -- don't use it!)
 *
 *	IF YOU WRITE ANYTHING NEW TO THIS FILE, try compiling this file in c++
 *		to make sure that you didn't break anything:
 *			g++ -DHAVE_CONFIG_H -x c++ links.h
 */
#ifndef LINKS_H
#define LINKS_H

#include "cfg.h"

#include "com-defs.h"

#define LINKS_COPYRIGHT "(C) 1999 - 2015 Mikulas Patocka\n(C) 2000 - 2015 Petr Kulhavy, Karel Kulhavy, Martin Pergel"
#define LINKS_COPYRIGHT_8859_1 "(C) 1999 - 2015 Mikul\341s Patocka\n(C) 2000 - 2015 Petr Kulhav\375, Karel Kulhav\375, Martin Pergel"
#define LINKS_COPYRIGHT_8859_2 "(C) 1999 - 2015 Mikul\341\271 Pato\350ka\n(C) 2000 - 2015 Petr Kulhav\375, Karel Kulhav\375, Martin Pergel"

#include "os_dep.h"
#include <stdio.h>
#ifdef HAVE_STDLIB_H_X
#include <stdlib.h>
#endif
#include <stdarg.h>
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#ifdef HAVE_STRING_H
#include <string.h>
#endif
#ifdef HAVE_STRINGS_H
#include <strings.h>
#endif
#include <errno.h>
#ifdef HAVE_LIMITS_H
#include <limits.h>
#endif
#include <sys/types.h>
#ifdef HAVE_INTTYPES_H
#include <inttypes.h>
#endif

typedef unsigned int socklen_t;

#ifndef __USE_XOPEN
#define U_X
#define __USE_XOPEN
#endif
#ifndef _XOPEN_SOURCE
#define X_S
#define _XOPEN_SOURCE	5	/* The 5 is a kludge to get a strptime() prototype in NetBSD */
#endif
#ifdef TIME_WITH_SYS_TIME
#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>
#endif
#ifdef HAVE_TIME_H
#include <time.h>
#endif
#else
#if defined(TM_IN_SYS_TIME) && defined(HAVE_SYS_TIME_H)
#include <sys/time.h>
#elif defined(HAVE_TIME_H)
#include <time.h>
#endif
#endif
#ifdef X_S
#undef _XOPEN_SOURCE
#endif
#ifdef U_X
#undef __USE_XOPEN
#endif

#include <sys/stat.h>
#ifdef HAVE_FCNTL_H
#include <fcntl.h>
#endif
#if defined(HAVE_LINUX_FALLOC_H) && !defined(FALLOC_FL_KEEP_SIZE)
#include <linux/falloc.h>
#endif
#ifdef HAVE_SYS_FILE_H
#include <sys/file.h>
#endif
#ifdef HAVE_DIRENT_H
#include <dirent.h>
#elif defined(HAVE_SYS_NDIR_H)
#include <sys/ndir.h>
#elif defined(HAVE_SYS_DIR_H)
#include <sys/dir.h>
#ifndef dirent
#define dirent direct
#endif
#elif defined(HAVE_NDIR_H)
#include <ndir.h>
#endif
#include <signal.h>
#ifdef HAVE_SYS_WAIT_H
#include <sys/wait.h>
#endif
#ifdef HAVE_SYS_SELECT_H
#include <sys/select.h>
#endif
#ifdef HAVE_SYS_RESOURCE_H
#include <sys/resource.h>
#endif
#ifdef HAVE_SYS_CYGWIN_H
#include <sys/cygwin.h>
#endif
#ifdef HAVE_UWIN_H
#include <uwin.h>
#endif
#ifdef HAVE_INTERIX_INTERIX_H
#include <interix/interix.h>
#endif
#ifdef HAVE_IO_H
#include <io.h>
#endif
#ifdef HAVE_PROCESS_H
#include <process.h>
#endif
#ifdef HAVE_CYGWIN_PROCESS_H
#include <cygwin/process.h>
#endif
#ifdef HAVE_CYGWIN_VERSION_H
#include <cygwin/version.h>
#endif
#ifdef HAVE_UNIXLIB_H
#include <unixlib.h>
#endif
#ifdef HAVE_SYS_UTSNAME_H
#include <sys/utsname.h>
#endif
#ifdef HAVE_PWD_H
#include <pwd.h>
#endif
#ifdef HAVE_GRP_H
#include <grp.h>
#endif
#ifdef HAVE_MALLOC_H
#include <malloc.h>
#endif
#ifdef HAVE_ALLOCA_H
#include <alloca.h>
#endif
#ifdef HAVE_SETJMP_H
#ifndef DONT_INCLUDE_SETJMP
#ifndef _SETJMP_H
#include <setjmp.h>
#endif /* _SETJMP_H */
#endif
#endif

#ifdef HAVE_NETINET_IN_SYSTM_H
#include <netinet/in_systm.h>
#else
#ifdef HAVE_NETINET_IN_SYSTEM_H
#include <netinet/in_system.h>
#endif
#endif
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#ifdef HAVE_NETINET_IP_H
#include <netinet/ip.h>
#endif
#ifdef HAVE_ARPA_INET_H
#include <arpa/inet.h>
#endif
#ifdef HAVE_NETINET_DHCP_H
#include <netinet/dhcp.h>
#endif
#ifdef HAVE_UTIME_H
#include <utime.h>
#endif

#ifdef HAVE_SSL
#ifdef HAVE_OPENSSL
#ifndef NO_SSL_CERTIFICATES
#define HAVE_SSL_CERTIFICATES
#endif
#if defined(OPENVMS) && defined(__VAX)
#define OPENSSL_NO_SHA512
#endif
#include <openssl/ssl.h>
#include <openssl/rand.h>
#include <openssl/x509v3.h>
#endif
#ifdef HAVE_NSS
#include <nss_compat_ossl/nss_compat_ossl.h>
#endif
#endif

#if defined(G)
#if defined(HAVE_PNG_H)
#define PNG_THREAD_UNSAFE_OK
#include <png.h>
#elif defined(HAVE_LIBPNG_PNG_H)
#define PNG_THREAD_UNSAFE_OK
#include <libpng/png.h>
#endif /* #if defined(HAVE_PNG_H) */
#ifndef png_jmpbuf
#define png_jmpbuf(png_ptr)	((png_ptr)->jmpbuf)
#endif
#endif /* #if defined(G) */

#ifdef HAVE_TERMIOS_H
#include <termios.h>
#elif defined(HAVE_SGTTY_H)
#include <sgtty.h>
#endif

#if defined(HAVE_POLL_H) && defined(HAVE_POLL) && !defined(INTERIX) && !defined(__HOS_AIX__)
#define USE_POLL
#include <poll.h>
#endif

#if (defined(HAVE_EVENT_H) || defined(HAVE_EV_EVENT_H)) && (defined(HAVE_LIBEVENT) || defined(HAVE_LIBEV)) && !defined(OPENVMS) && !defined(DOS)
#if defined(HAVE_EVENT_H)
#include <event.h>
#else
#include <ev-event.h>
#endif
#define USE_LIBEVENT
#endif

#ifdef HAVE_OPENMP
#include <omp.h>
#define SMP_ALIGN		256
#else
#define omp_get_num_threads()	1
#define omp_get_thread_num()	0
#define SMP_ALIGN		1
#endif
#if (defined(__alpha__) || defined(__alpha)) && !defined(__alpha_bwx__)
#define OPENMP_NONATOMIC	1
#else
#define OPENMP_NONATOMIC	0
#endif

#ifdef HAVE_LONG_LONG
#define longlong long long
#define ulonglong unsigned long long
#else
#define longlong double
#define ulonglong double
#endif

#if defined(__INTERIX) && defined(HAVE_STRTOQ)
extern quad_t
#if defined(__cdecl) || defined(_MSC_VER)
__cdecl
#endif
strtoq(const char *, char **, int);
#endif

#define stringify_internal(arg)	#arg
#define stringify(arg)		stringify_internal(arg)

#include "os_depx.h"

#include "setup.h"

#define LINKS_2

#ifdef HAVE_POINTER_COMPARISON_BUG
#define DUMMY ((void *)1L)
#else
#define DUMMY ((void *)-1L)
#endif

#define cast_const_char	(const char *)
#define cast_char	(char *)
#define cast_uchar	(unsigned char *)

#ifdef OPENVMS
#define RET_OK		1	/* SS$_NORMAL */
#define RET_ERROR	3586	/* SS$_LINEABRT */
#define RET_SIGNAL	2096	/* SS$_CANCEL */
#define RET_SYNTAX	20	/* SS$_BADPARAM */
#define RET_FATAL	44	/* SS$_ABORT */
#define RET_INTERNAL	44	/* SS$_ABORT */
#else
#define RET_OK		0
#define RET_ERROR	1
#define RET_SIGNAL	2
#define RET_SYNTAX	3
#define RET_FATAL	4
#define RET_INTERNAL	127
#endif

#ifndef HAVE_SNPRINTF
int my_snprintf(char *, int n, char *format, ...) PRINTF_FORMAT(3, 4);
#define snprintf my_snprintf
#endif
#ifndef HAVE_RAISE
int raise(int);
#endif
#ifndef HAVE_GETTIMEOFDAY
struct timeval {
	long tv_sec;
	long tv_usec;
};
struct timezone {
	int tz_minuteswest;
	int tz_dsttime;
};
int gettimeofday(struct timeval *tv, struct timezone *tz);
#endif
#ifndef HAVE_TEMPNAM
char *tempnam(const char *dir, const char *pfx);
#endif
#ifndef HAVE_STRDUP
char *strdup(const char *s);
#endif
#ifndef HAVE_STRTOL
long strtol(const char *, char **, int);
#endif
#ifndef HAVE_STRTOUL
unsigned long strtoul(const char *, char **, int);
#endif
#ifndef HAVE_STRTOD
double strtod(const char *nptr, char **endptr);
#endif
#ifndef HAVE_STRLEN
size_t strlen(const char *s);
#endif
#ifndef HAVE_STRCPY
char *strcpy(char *dst, const char *src);
#endif
#ifndef HAVE_STRCHR
char *strchr(const char *s, int c);
#endif
#ifndef HAVE_STRRCHR
char *strrchr(const char *s, int c);
#endif
#ifndef HAVE_STRCMP
int strcmp(const char *s1, const char *s2);
#endif
#ifndef HAVE_STRNCMP
int strncmp(const char *s1, const char *s2, size_t n);
#endif
#ifndef HAVE_STRCASECMP
int strcasecmp(const char *s1, const char *s2);
#endif
#ifndef HAVE_STRNCASECMP
int strncasecmp(const char *s1, const char *s2, size_t n);
#endif
#ifndef HAVE_STRCSPN
size_t strcspn(const char *s, const char *reject);
#endif
#ifndef HAVE_STRSPN
size_t strspn(const char *s, const char *accept);
#endif
#ifndef HAVE_STRSTR
char *strstr(const char *haystack, const char *needle);
#endif
#ifndef HAVE_MEMCHR
void *memchr(const void *s, int c, size_t length);
#endif
#ifndef HAVE_MEMCMP
int memcmp(const void *, const void *, size_t);
#endif
#ifndef HAVE_MEMCPY
void *memcpy(void *, const void *, size_t);
#endif
#ifndef HAVE_MEMMOVE
void *memmove(void *, const void *, size_t);
#endif
#ifndef HAVE_MEMSET
void *memset(void *, int, size_t);
#endif
#ifndef HAVE_STRERROR
char *strerror(int);
#endif

#define EINTRLOOPX(ret_, call_, x_)			\
do {							\
	(ret_) = (call_);				\
} while ((ret_) == (x_) && errno == EINTR)

#define EINTRLOOP(ret_, call_)	EINTRLOOPX(ret_, call_, -1)

#define ENULLLOOP(ret_, call_)				\
do {							\
	errno = 0;					\
	(ret_) = (call_);				\
} while (!(ret_) && errno == EINTR)

#if defined(HAVE_PTHREAD_SIGMASK)
static inline int do_sigprocmask(int how, const sigset_t *set, sigset_t *oset)
{
	int r;
	r = pthread_sigmask(how, set, oset);
	if (r) {
		errno = r;
		return -1;
	}
	return 0;
}
#elif defined(HAVE_SIGPROCMASK)
#define do_sigprocmask	sigprocmask
#else
#ifdef sigset_t
#undef sigset_t
#endif
#define sigset_t	int
#ifndef SIG_BLOCK
#define SIG_BLOCK	0
#endif
#ifndef SIG_SETMASK
#define SIG_SETMASK	2
#endif
static inline int do_sigprocmask(int how, const sigset_t *set, sigset_t *oset)
{
	sigset_t old = 0;
#if defined(HAVE_SIGBLOCK) && defined(HAVE_SIGSETMASK)
	switch (how) {
		case SIG_BLOCK:
			old = sigblock(*set);
			break;
		case SIG_SETMASK:
			old = sigsetmask(*set);
			break;
	}
#endif
	if (oset) *oset = old;
	return 0;
}
#ifdef sigdelset
#undef sigdelset
#endif
#define sigdelset(x, s)	(*(x) &= ~(1 << (s)), 0)
#ifndef HAVE_SIGDELSET
#define HAVE_SIGDELSET		1
#endif
#ifdef HAVE_SIGFILLSET
#undef HAVE_SIGFILLSET
#endif
#endif
#ifndef HAVE_SIGFILLSET
int sigfillset(sigset_t *set);
#endif

#define option option_dirty_workaround_for_name_clash_with_include_on_cygwin
#define table table_dirty_workaround_for_name_clash_with_libraries_on_macos
#define scroll scroll_dirty_workaround_for_name_clash_with_libraries_on_macos
#define list list_dirty_workaround_for_name_clash_in_stl_with_class_list

#ifndef G
#define F 0
#else
extern int F;
#endif

#if defined(DEBUG)
#if defined(G)
#define NO_GFX	do {if (F) internal("call to text-only function");} while (0)
#define NO_TXT	do {if (!F) internal("call to graphics-only function");} while (0)
#else
#define NO_GFX	do {} while (0)
#define NO_TXT	this_should_not_be_compiled
#endif
#else
#define NO_GFX	do {} while (0)
#define NO_TXT	do {} while (0)
#endif

#ifndef G
#define gf_val(x, y) (x)
#define GF(x)
#else
#define gf_val(x, y) (F ? (y) : (x))
#define GF(x) if (F) {x;}
#endif

#define MAX_STR_LEN	1024

#define BIN_SEARCH(entries, eq, ab, key, result)			\
{									\
	int s_ = 0, e_ = (entries) - 1;					\
	(result) = -1;							\
	while (s_ <= e_) {						\
		int m_ = (int)(((unsigned)s_ + (unsigned)e_) / 2);	\
		if (eq((m_), (key))) {					\
			(result) = m_;					\
			break;						\
		}							\
		if (ab((m_), (key))) e_ = m_ - 1;			\
		else s_ = m_ + 1;					\
	}								\
}									\

/* error.c */

#if defined(DOS)
#define ANSI_BELL		"\007"
#define ANSI_SET_BOLD		""
#define ANSI_CLEAR_BOLD		""
#else
#define ANSI_BELL		"\007"
#define ANSI_SET_BOLD		"\033[1m"
#define ANSI_CLEAR_BOLD		"\033[0m"
#endif

#ifdef LEAK_DEBUG
extern unsigned alloc_overhead;
#else
#define alloc_overhead	0
#endif

void *do_not_optimize_here(void *p);
void init_heap(void);
void check_memory_leaks(void);
void error(char *, ...) PRINTF_FORMAT(1, 2);
void fatal_exit(char *, ...) PRINTF_FORMAT(1, 2)
#ifdef __clang_analyzer__
	__attribute__((analyzer_noreturn))
#endif
	;
void debug_msg(char *, ...) PRINTF_FORMAT(1, 2);
void int_error(char *, ...) PRINTF_FORMAT(1, 2)
#ifdef __clang_analyzer__
	__attribute__((analyzer_noreturn))
#endif
	;
extern int errline;
extern unsigned char *errfile;

#define internal_ errfile = (unsigned char *)__FILE__, errline = __LINE__, int_error
#define internal internal_
#define debug errfile = (unsigned char *)__FILE__, errline = __LINE__, debug_msg

void fatal_tty_exit(void);

#ifdef __ICC
/* ICC OpenMP bug */
#define overalloc_condition	0
#else
#define overalloc_condition	1
#endif

#define overalloc_at(f, l)						\
do {									\
	fatal_exit("ERROR: attempting to allocate too large block at %s:%d", f, l);\
} while (overalloc_condition)	/* while (1) is not a typo --- it's here to allow the compiler
	that doesn't know that fatal_exit doesn't return to do better
	optimizations */

#define overalloc()	overalloc_at(__FILE__, __LINE__)

#define overflow()							\
do {									\
	fatal_exit("ERROR: arithmetic overflow at %s:%d", __FILE__, __LINE__);\
} while (1)

#define safe_add(x, y)							\
	((int)((x) | (y)) >= 0 && ((unsigned)(x) + (unsigned)(y)) >= MAXINT ?\
		fatal_exit("ERROR: arithmetic overflow at %s:%d: %d + %d", __FILE__, __LINE__, (x), (y)), 0\
		: (int)((unsigned)(x) + (unsigned)(y)))

#ifdef LEAK_DEBUG

extern my_uintptr_t mem_amount;
extern my_uintptr_t mem_blocks;

#endif

#ifdef LEAK_DEBUG

void *debug_mem_alloc(unsigned char *, int, size_t, int);
void *debug_mem_calloc(unsigned char *, int, size_t, int);
void debug_mem_free(unsigned char *, int, void *);
void *debug_mem_realloc(unsigned char *, int, void *, size_t, int);
void set_mem_comment(void *, unsigned char *, int);
unsigned char *get_mem_comment(void *);

#define mem_alloc(x) debug_mem_alloc((unsigned char *)__FILE__, __LINE__, x, 0)
#define mem_calloc(x) debug_mem_calloc((unsigned char *)__FILE__, __LINE__, x, 0)
#define mem_free(x) debug_mem_free((unsigned char *)__FILE__, __LINE__, x)
#define mem_realloc(x, y) debug_mem_realloc((unsigned char *)__FILE__, __LINE__, x, y, 0)

#define mem_alloc_mayfail(x) debug_mem_alloc((unsigned char *)__FILE__, __LINE__, x, 1)
#define mem_calloc_mayfail(x) debug_mem_calloc((unsigned char *)__FILE__, __LINE__, x, 1)
#define mem_realloc_mayfail(x, y) debug_mem_realloc((unsigned char *)__FILE__, __LINE__, x, y, 1)

#else

void *mem_alloc_(size_t size, int mayfail);
void *mem_calloc_(size_t size, int mayfail);
void mem_free(void *p);
void *mem_realloc_(void *p, size_t size, int mayfail);

#define mem_alloc(x)			mem_alloc_(x, 0)
#define mem_calloc(x)			mem_calloc_(x, 0)
#define mem_realloc(x, y)		mem_realloc_(x, y, 0)

#define mem_alloc_mayfail(x)		mem_alloc_(x, 1)
#define mem_calloc_mayfail(x)		mem_calloc_(x, 1)
#define mem_realloc_mayfail(x, y)	mem_realloc_(x, y, 1)


static inline void *debug_mem_alloc(unsigned char *f, int l, size_t s, int mayfail) { return mem_alloc_(s, mayfail); }
static inline void *debug_mem_calloc(unsigned char *f, int l, size_t s, int mayfail) { return mem_calloc_(s, mayfail); }
static inline void debug_mem_free(unsigned char *f, int l, void *p) { mem_free(p); }
static inline void *debug_mem_realloc(unsigned char *f, int l, void *p, size_t s, int mayfail) { return mem_realloc_(p, s, mayfail); }
static inline void set_mem_comment(void *p, unsigned char *c, int l) {}
static inline unsigned char *get_mem_comment(void *p){return (unsigned char *)"";}
#endif

#if !(defined(LEAK_DEBUG) && defined(LEAK_DEBUG_LIST))

unsigned char *memacpy(const unsigned char *src, size_t len);
unsigned char *stracpy(const unsigned char *src);

#else

unsigned char *debug_memacpy(unsigned char *f, int l, const unsigned char *src, size_t len);
#define memacpy(s, l) debug_memacpy((unsigned char *)__FILE__, __LINE__, s, l)

unsigned char *debug_stracpy(unsigned char *f, int l, const unsigned char *src);
#define stracpy(s) debug_stracpy((unsigned char *)__FILE__, __LINE__, s)

#endif

#if !defined(HAVE_SIGSETJMP) || !defined(HAVE_SETJMP_H)
#ifdef OOPS
#undef OOPS
#endif
#endif

#ifndef OOPS
#define pr(code) if (1) {code;} else
static inline void nopr(void) {}
static inline void xpr(void) {}
#else
sigjmp_buf *new_stack_frame(void);
void xpr(void);
#define pr(code) if (!sigsetjmp(*new_stack_frame(), 1)) {do {code;} while (0); xpr();} else
void nopr(void);
#endif

/* inline */

#define ALLOC_GR	0x040		/* must be power of 2 */


struct list_head {
	void *next;
	void *prev;
};

struct xlist_head {
	struct xlist_head *next;
	struct xlist_head *prev;
};

#define init_list(x) { do_not_optimize_here(&x); (x).next=&(x); (x).prev=&(x); do_not_optimize_here(&x);}
#define list_empty(x) ((x).next == &(x))
#define del_from_list(x) {do_not_optimize_here(x); ((struct list_head *)(void *)(x)->next)->prev=(x)->prev; ((struct list_head *)(void *)(x)->prev)->next=(x)->next; do_not_optimize_here(x);}
/*#define add_to_list(l,x) {(x)->next=(l).next; (x)->prev=(typeof(x))(void *)&(l); (l).next=(x); if ((l).prev==&(l)) (l).prev=(x);}*/

#if defined(HAVE_TYPEOF)
#if !defined(__GNUC__)
#define USE_TYPEOF
#elif __GNUC__ < 4
#define USE_TYPEOF
#endif
#endif

#if defined(USE_TYPEOF)
/* GCC 4 emits warnings about this and I haven't found a way to stop it */
#define add_at_pos(p,x) do {do_not_optimize_here(p); (x)->next=(p)->next; (x)->prev=(p); (p)->next=(x); (x)->next->prev=(x);do_not_optimize_here(p);} while(0)
#define add_to_list(l,x) add_at_pos((typeof(x))(void *)&(l),(x))
#define foreach(e,l) for ((e)=(l).next; (e)!=(typeof(e))(void *)&(l); (e)=(e)->next)
#define foreachback(e,l) for ((e)=(l).prev; (e)!=(typeof(e))(void *)&(l); (e)=(e)->prev)
#else
#define add_at_pos(p,x) do {do_not_optimize_here(p); (x)->next=(p)->next; (x)->prev=(void *)(p); (p)->next=(x); (x)->next->prev=(x); do_not_optimize_here(p); } while(0)
#define add_to_list(l,x) add_at_pos(&(l),(x))
#define foreach(e,l) for ((e)=(l).next; (e)!=(void *)&(l); (e)=(e)->next)
#define foreachback(e,l) for ((e)=(l).prev; (e)!=(void *)&(l); (e)=(e)->prev)
#endif
#define free_list(l) {do_not_optimize_here(&l); while ((l).next != &(l)) {struct list_head *a__=(l).next; del_from_list(a__); mem_free(a__); } do_not_optimize_here(&l);}

#define WHITECHAR(x) ((x) == 9 || (x) == 10 || (x) == 12 || (x) == 13 || (x) == ' ')
#define U(x) ((x) == '"' || (x) == '\'')


#define CI_BYTES	1
#define CI_FILES	2
#define CI_LOCKED	3
#define CI_LOADING	4
#define CI_TIMERS	5
#define CI_TRANSFER	6
#define CI_CONNECTING	7
#define CI_KEEP		8

/* string.c */

int snprint(unsigned char *s, int n, my_uintptr_t num);
int snzprint(unsigned char *s, int n, off_t num);
void add_to_strn(unsigned char **s, unsigned char *a);
void extend_str(unsigned char **s, int n);

#define init_str() init_str_x((unsigned char *)__FILE__, __LINE__)

static inline unsigned char *init_str_x(unsigned char *file, int line)
{
	unsigned char *p;

	p=(unsigned char *)debug_mem_calloc(file, line, 1L, 0);
	return p;
}

void add_bytes_to_str(unsigned char **s, int *l, unsigned char *a, size_t ll);
void add_to_str(unsigned char **s, int *l, unsigned char *a);
void add_chr_to_str(unsigned char **s, int *l, unsigned char a);
void add_unsigned_num_to_str(unsigned char **s, int *l, off_t n);
void add_unsigned_long_num_to_str(unsigned char **s, int *l, my_uintptr_t n);
void add_num_to_str(unsigned char **s, int *l, off_t n);
void add_knum_to_str(unsigned char **s, int *l, off_t n);
long strtolx(unsigned char *c, unsigned char **end);
#if defined(HAVE_STRTOLL) || defined(HAVE_STRTOQ) || defined(HAVE_STRTOIMAX)
#define my_strtoll_t	longlong
#else
#define my_strtoll_t	long
#endif
my_strtoll_t my_strtoll(unsigned char *string, unsigned char **end);

unsigned char *safe_strncpy(unsigned char *dst, const unsigned char *src, size_t dst_size);
#ifdef JS
void skip_nonprintable(unsigned char *txt);
#endif
/* case insensitive compare of 2 strings */
/* comparison ends after len (or less) characters */
/* return value: 1=strings differ, 0=strings are same */
static inline unsigned upcase(unsigned a)
{
	if (a >= 'a' && a <= 'z') a -= 0x20;
	return a;
}
static inline int srch_cmp(unsigned char c1, unsigned char c2)
{
	return upcase(c1) != upcase(c2);
}
int casecmp(const unsigned char *c1, const unsigned char *c2, size_t len);
int casestrstr(const unsigned char *h, const unsigned char *n);
static inline int xstrcmp(const unsigned char *s1, const unsigned char *s2)
{
	if (!s1 && !s2) return 0;
	if (!s1) return -1;
	if (!s2) return 1;
	return strcmp(cast_const_char s1, cast_const_char s2);
}

static inline int cmpbeg(const unsigned char *str, const unsigned char *b)
{
	while (*str && upcase(*str) == upcase(*b)) str++, b++;
	return !!*b;
}


/* os_dep.c */

#ifdef HAVE_LONG_LONG
typedef long long ttime;
typedef unsigned long long uttime;
typedef unsigned long long tcount;
#else
typedef long ttime;
typedef unsigned long uttime;
typedef unsigned long tcount;
#endif

#if defined(HAVE_GPM_H) && defined(HAVE_LIBGPM)
#define USE_GPM
#endif

#if defined(OS2) && defined(HAVE_UMALLOC_H) && defined(HAVE__UCREATE) && defined(HAVE__UOPEN) && defined(HAVE__UDEFAULT) && defined(HAVE_BEGINTHREAD) 
#define OS2_ADVANCED_HEAP
#define MEMORY_REQUESTED
extern unsigned long mem_requested;
extern unsigned long blocks_requested;
#endif

#ifdef OS2_ADVANCED_HEAP
void mem_freed_large(size_t size);
#else
#define mem_freed_large(x)	do { } while (0)
#endif

struct terminal;

struct open_in_new {
	unsigned char *text;
	unsigned char *hk;
	int (*open_window_fn)(struct terminal *, unsigned char *, unsigned char *);
};

void close_fork_tty(void);
int get_system_env(void);
int is_twterm(void);
int is_screen(void);
int is_xterm(void);
int get_terminal_size(int, int *, int *);
void handle_terminal_resize(int, void (*)(void));
void unhandle_terminal_resize(int);
void set_nonblock(int);
int c_pipe(int *);
int c_dup(int oh);
int c_socket(int, int, int);
int c_accept(int, struct sockaddr *, socklen_t *);
int c_open(unsigned char *, int);
int c_open3(unsigned char *, int, int);
DIR *c_opendir(unsigned char *);
#ifdef HAVE_OPEN_PREALLOC
int open_prealloc(unsigned char *, int, int, off_t);
#endif
int get_input_handle(void);
int get_output_handle(void);
int get_ctl_handle(void);
#ifdef OS_SETRAW
int setraw(int ctl, int save);
void setcooked(int ctl);
#endif
void want_draw(void);
void done_draw(void);
void terminate_osdep(void);
void *handle_mouse(int, void (*)(void *, unsigned char *, int), void *);
void unhandle_mouse(void *);
void add_gpm_version(unsigned char **s, int *l);
#ifdef OPENVMS
extern int vms_thread_high_priority;
#endif
int start_thread(void (*)(void *, int), void *, int, int);
unsigned char *get_clipboard_text(struct terminal *);
void set_clipboard_text(struct terminal *, unsigned char *);
int clipboard_support(struct terminal *);
int is_winnt(void);
int get_windows_cp(int cons);
void set_window_title(unsigned char *);
unsigned char *get_window_title(void);
int is_safe_in_shell(unsigned char);
unsigned char *escape_path(unsigned char *);
void check_shell_security(unsigned char **);
void check_filename(unsigned char **);
int check_shell_url(unsigned char *);
void do_signal(int sig, void (*handler)(int));
ttime get_time(void);
void ignore_signals(void);
void block_stdin(void);
void unblock_stdin(void);
void init_os(void);
void init_os_terminal(void);
void get_path_to_exe(void);
int os_get_system_name(unsigned char *buffer);
unsigned char *os_conv_to_external_path(unsigned char *, unsigned char *);
unsigned char *os_fixup_external_program(unsigned char *);
int exe(unsigned char *, int);
#ifdef WIN
int exe_on_background(unsigned char *, unsigned char *);
#define HAVE_EXE_ON_BACKGROUND
#endif
int resize_window(int, int);
int can_resize_window(struct terminal *);
int can_open_os_shell(int);
unsigned char *links_xterm(void);
struct open_in_new *get_open_in_new(int);
void set_highpri(void);
#if defined(OPENVMS) && defined(GRDRV_X)
int vms_x11_fd(int ef);
#endif

extern unsigned char *clipboard;

void os_seed_random(unsigned char **pool, int *pool_size);
int os_send_fg_cookie(int);
int os_receive_fg_cookie(int);
void os_detach_console(void);
int os_default_language(void);
int os_default_charset(void);

/* memory.c */

#define SH_CHECK_QUOTA		0
#define SH_FREE_SOMETHING	1
#define SH_FREE_ALL		2

#define ST_SOMETHING_FREED	1
#define ST_CACHE_EMPTY		2

#define MF_GPI			1

int shrink_memory(int, int);
void register_cache_upcall(int (*)(int), int, unsigned char *);
void free_all_caches(void);
extern int malloc_try_hard;
int out_of_memory_fl(int flags, unsigned char *msg, size_t size, unsigned char *file, int line);
#define out_of_memory(flags, msg, size)	out_of_memory_fl(flags, msg, size, (unsigned char *)__FILE__, __LINE__)

#ifndef DEBUG_TEST_FREE
#define debug_test_free(file, line)
#else
void debug_test_free(unsigned char *file, int line);
#endif

/* select.c */

#ifndef FD_SETSIZE
#define FD_SETSIZE	(sizeof(fd_set) * 8)
#endif

#define NBIT(p)		(sizeof((p)->fds_bits[0]) * 8)

#ifndef FD_SET
#define FD_SET(n, p)    ((p)->fds_bits[(n)/NBIT(p)] |= (1 << ((n) % NBIT(p))))
#endif
#ifndef FD_CLR
#define FD_CLR(n, p)    ((p)->fds_bits[(n)/NBIT(p)] &= ~(1 << ((n) % NBIT(p))))
#endif
#ifndef FD_ISSET
#define FD_ISSET(n, p)  ((p)->fds_bits[(n)/NBIT(p)] & (1 << ((n) % NBIT(p))))
#endif
#ifndef FD_ZERO
#define FD_ZERO(p)      memset((void *)(p), 0, sizeof(*(p)))
#endif


extern int terminate_loop;

int can_write(int fd);
int can_read(int fd);
int can_read_timeout(int fd, int sec);
unsigned long select_info(int);
void reinit_child(void);
void select_loop(void (*)(void));
void terminate_select(void);
int register_bottom_half(void (*)(void *), void *);
void unregister_bottom_half(void (*)(void *), void *);
void check_bottom_halves(void);
void add_event_string(unsigned char **, int *, struct terminal *);
struct timer;
struct timer *install_timer(ttime, void (*)(void *), void *);
void kill_timer(struct timer *);
void portable_sleep(unsigned msec);

#define H_READ	0
#define H_WRITE	1
#define H_DATA	2

void *get_handler(int, int);
extern unsigned char *sh_file;
extern int sh_line;
void set_handlers_file_line(int, void (*)(void *), void (*)(void *), void *);
#define set_handlers(a, b, c, d)	(sh_file = (unsigned char *)__FILE__, sh_line = __LINE__, set_handlers_file_line(a, b, c, d))
void clear_events(int, int);
extern pid_t signal_pid;
extern int signal_pipe[2];
void install_signal_handler(int, void (*)(void *), void *, int);
void interruptible_signal(int sig, int in);
void block_signals(int except1, int except2);
void unblock_signals(void);
void set_sigcld(void);
#ifdef HAVE_OPENMP
#define omp_start()	do { if (!disable_openmp) block_signals(0, 0); } while (0)
#define omp_end()	do { if (!disable_openmp) unblock_signals(); } while (0)
#else
#define omp_start()	do { } while (0)
#define omp_end()	do { } while (0)
#endif

/* dns.c */

#define MAX_ADDRESSES		64

struct host_address {
	int af;
	unsigned char addr[16];
	unsigned scope_id;
};

struct lookup_result {
	int n;
	struct host_address a[MAX_ADDRESSES];
};

#ifdef SUPPORT_IPV6
extern int support_ipv6;
#else
#define support_ipv6	0
#endif

int numeric_ip_address(unsigned char *name, unsigned char address[4]);
#ifdef SUPPORT_IPV6
int numeric_ipv6_address(unsigned char *name, unsigned char address[16], unsigned *scope_id);
#endif
void do_real_lookup(unsigned char *, int, struct lookup_result *);
int find_host(unsigned char *, struct lookup_result *, void **, void (*)(void *, int), void *);
int find_host_no_cache(unsigned char *, struct lookup_result *, void **, void (*)(void *, int), void *);
void kill_dns_request(void **);
void dns_set_priority(unsigned char *, struct host_address *, int);
unsigned long dns_info(int type);
int ipv6_full_access(void);
void init_dns(void);

/* cache.c */

struct cache_entry {
	struct cache_entry *next;
	struct cache_entry *prev;
	unsigned char *url;
	unsigned char *head;
	int http_code;
	unsigned char *redirect;
	int redirect_get;
	off_t length;
	off_t max_length;
	int incomplete;
	int tgc;
	unsigned char *last_modified;
	time_t expire_time;	/* 0 never, 1 always */
	off_t data_size;
	struct list_head frag;	/* struct fragment */
	tcount count;
	tcount count2;
	int refcount;
	unsigned char *decompressed;
	size_t decompressed_len;
#ifdef HAVE_SSL
	unsigned char *ssl_info;
#endif
};

struct fragment {
	struct fragment *next;
	struct fragment *prev;
	off_t offset;
	off_t length;
	off_t real_length;
	unsigned char data[1];
};

extern int page_size;

void init_cache(void);
my_uintptr_t cache_info(int);
my_uintptr_t decompress_info(int);
int find_in_cache(unsigned char *, struct cache_entry **);
int get_cache_entry(unsigned char *, struct cache_entry **);
int new_cache_entry(unsigned char *, struct cache_entry **);
void detach_cache_entry(struct cache_entry *);
int add_fragment(struct cache_entry *, off_t, unsigned char *, off_t);
int defrag_entry(struct cache_entry *);
void truncate_entry(struct cache_entry *, off_t, int);
void free_entry_to(struct cache_entry *, off_t);
void delete_entry_content(struct cache_entry *);
void delete_cache_entry(struct cache_entry *e);
void trim_cache_entry(struct cache_entry *e);

/* sched.c */

#define PRI_MAIN	0
#define PRI_DOWNLOAD	0
#define PRI_FRAME	1
#define PRI_NEED_IMG	2
#define PRI_IMG		3
#define PRI_PRELOAD	4
#define PRI_CANCEL	5
#define N_PRI		6

struct remaining_info {
	int valid;
	off_t size, loaded, last_loaded, cur_loaded;
	off_t pos;
	ttime elapsed;
	ttime last_time;
	ttime dis_b;
	off_t data_in_secs[CURRENT_SPD_SEC];
	struct timer *timer;
};

struct conn_info;

struct connection {
	struct connection *next;
	struct connection *prev;
	tcount count;
	unsigned char *url;
	unsigned char *prev_url;	/* allocated string with referrer or NULL */
	int running;
	int state;
	int prev_error;
	off_t from;
	int pri[N_PRI];
	int no_cache;
	int sock1;
	int sock2;
	void *dnsquery;
	pid_t pid;
	int tries;
	tcount netcfg_stamp;
	struct list_head statuss;
	void *info;
	void *buffer;
	struct conn_info *newconn;
	void (*conn_func)(void *);
	struct cache_entry *cache;
	off_t received;
	off_t est_length;
	int unrestartable;
	int no_compress;
	struct remaining_info prg;
	struct timer *timer;
	int detached;
	unsigned char socks_proxy[MAX_STR_LEN];
#ifdef HAVE_SSL
	SSL *ssl;
	int no_tls;
#endif
};

extern tcount netcfg_stamp;

extern struct list_head queue;

struct k_conn {
	struct k_conn *next;
	struct k_conn *prev;
	void (*protocol)(struct connection *);
	unsigned char *host;
	int port;
	int conn;
	ttime timeout;
	ttime add_time;
	int protocol_data;
#ifdef HAVE_SSL
	SSL *ssl;
#endif
};

extern struct list_head keepalive_connections;

static inline int getpri(struct connection *c)
{
	int i;
	for (i = 0; i < N_PRI; i++) if (c->pri[i]) return i;
	internal((char *)"connection has no owner");
	return N_PRI;
}

#define NC_ALWAYS_CACHE	0
#define NC_CACHE	1
#define NC_IF_MOD	2
#define NC_RELOAD	3
#define NC_PR_NO_CACHE	4

#define S_WAIT		0
#define S_DNS		1
#define S_CONN		2
#define S_CONN_ANOTHER	3
#define S_SOCKS_NEG	4
#define S_SSL_NEG	5
#define S_SENT		6
#define S_LOGIN		7
#define S_GETH		8
#define S_PROC		9
#define S_TRANS		10

#define S__OK			(-2000000000)
#define S_INTERRUPTED		(-2000000001)
#define S_EXCEPT		(-2000000002)
#define S_INTERNAL		(-2000000003)
#define S_OUT_OF_MEM		(-2000000004)
#define S_NO_DNS		(-2000000005)
#define S_CANT_WRITE		(-2000000006)
#define S_CANT_READ		(-2000000007)
#define S_MODIFIED		(-2000000008)
#define S_BAD_URL		(-2000000009)
#define S_BAD_PROXY		(-2000000010)
#define S_TIMEOUT		(-2000000011)
#define S_RESTART		(-2000000012)
#define S_STATE			(-2000000013)
#define S_CYCLIC_REDIRECT	(-2000000014)
#define S_LARGE_FILE		(-2000000015)
#define S_BLOCKED_URL		(-2000000016)
#define S_SMB_NOT_ALLOWED	(-2000000017)
#define S_FILE_NOT_ALLOWED	(-2000000018)
#define S_NO_PROXY		(-2000000019)

#define S_HTTP_ERROR		(-2000000100)
#define S_HTTP_100		(-2000000101)
#define S_HTTP_204		(-2000000102)
#define S_HTTPS_FWD_ERROR	(-2000000103)
#define S_INVALID_CERTIFICATE	(-2000000104)
#define S_INSECURE_CIPHER	(-2000000105)

#define S_FILE_TYPE		(-2000000200)
#define S_FILE_ERROR		(-2000000201)

#define S_FTP_ERROR		(-2000000300)
#define S_FTP_UNAVAIL		(-2000000301)
#define S_FTP_LOGIN		(-2000000302)
#define S_FTP_PORT		(-2000000303)
#define S_FTP_NO_FILE		(-2000000304)
#define S_FTP_FILE_ERROR	(-2000000305)

#define S_SSL_ERROR		(-2000000400)
#define S_NO_SSL		(-2000000401)

#define S_BAD_SOCKS_VERSION	(-2000000500)
#define S_SOCKS_REJECTED	(-2000000501)
#define S_SOCKS_NO_IDENTD	(-2000000502)
#define S_SOCKS_BAD_USERID	(-2000000503)
#define S_SOCKS_UNKNOWN_ERROR	(-2000000504)

#define S_NO_SMB_CLIENT		(-2000000600)

#define S_WAIT_REDIR		(-2000000700)

#define S_UNKNOWN_ERROR		(-2000000800)

#define S_MAX			(-2000000900)


extern struct s_msg_dsc {
	int n;
	unsigned char *msg;
} msg_dsc[];

struct status {
	struct status *next;
	struct status *prev;
	struct connection *c;
	struct cache_entry *ce;
	int state;
	int prev_error;
	int pri;
	void (*end)(struct status *, void *);
	void *data;
	struct remaining_info *prg;
};

unsigned char *get_proxy_string(unsigned char *url);
unsigned char *get_proxy(unsigned char *url);
unsigned char *remove_proxy_prefix(unsigned char *url);
int get_allow_flags(unsigned char *url);
int disallow_url(unsigned char *url, int allow_flags);
void check_queue(void *dummy);
unsigned long connect_info(int);
void setcstate(struct connection *c, int);
int get_keepalive_socket(struct connection *c, int *protocol_data);
void add_keepalive_socket(struct connection *c, ttime timeout, int protocol_data);
int is_connection_restartable(struct connection *c);
int is_last_try(struct connection *c);
void retry_connection(struct connection *c);
void abort_connection(struct connection *c);
#define ALLOW_SMB		1
#define ALLOW_FILE		2
#define ALLOW_ALL		(ALLOW_SMB | ALLOW_FILE)
void load_url(unsigned char *, unsigned char *, struct status *, int, int, int, int, off_t);
void change_connection(struct status *, struct status *, int);
void detach_connection(struct status *, off_t);
void abort_all_connections(void);
void abort_background_connections(void);
void abort_all_keepalive_connections(void);
int is_entry_used(struct cache_entry *);
void set_connection_timeout(struct connection *);
void add_blacklist_entry(unsigned char *, int);
void del_blacklist_entry(unsigned char *, int);
int get_blacklist_flags(unsigned char *);
void free_blacklist(void);

#define BL_HTTP10		0x01
#define BL_NO_ACCEPT_LANGUAGE	0x02
#define BL_NO_CHARSET		0x04
#define BL_NO_RANGE		0x08
#define BL_NO_COMPRESSION	0x10
#define BL_NO_BZIP2		0x20
#define BL_IGNORE_CERTIFICATE	0x40
#define BL_IGNORE_CIPHER	0x80

/* url.c */

struct session;

#define POST_CHAR 1

static inline int end_of_dir(unsigned char *url, unsigned char c)
{
	return c == POST_CHAR || c == '#' || ((c == ';' || c == '?') && (!url || !casecmp(url, (unsigned char *)"http", 4)));
}

int parse_url(unsigned char *, int *, unsigned char **, int *, unsigned char **, int *, unsigned char **, int *, unsigned char **, int *, unsigned char **, int *, unsigned char **);
unsigned char *get_protocol_name(unsigned char *);
unsigned char *get_host_name(unsigned char *);
unsigned char *get_keepalive_id(unsigned char *);
unsigned char *get_user_name(unsigned char *);
unsigned char *get_pass(unsigned char *);
int get_port(unsigned char *);
unsigned char *get_port_str(unsigned char *);
void (*get_protocol_handle(unsigned char *))(struct connection *);
void (*get_external_protocol_function(unsigned char *))(struct session *, unsigned char *);
int url_bypasses_socks(unsigned char *);
unsigned char *get_url_data(unsigned char *);
unsigned char *translate_hashbang(unsigned char *up);
unsigned char *join_urls(unsigned char *, unsigned char *);
unsigned char *translate_url(unsigned char *, unsigned char *);
unsigned char *extract_position(unsigned char *);
int url_not_saveable(unsigned char *);
void add_conv_str(unsigned char **s, int *l, unsigned char *b, int ll, int encode_special);

/* connect.c */

struct read_buffer {
	int sock;
	int len;
	int close;
	void (*done)(struct connection *, struct read_buffer *);
	unsigned char data[1];
};

int socket_and_bind(int pf, unsigned char *address);
void close_socket(int *);
void make_connection(struct connection *, int, int *, void (*)(struct connection *));
void continue_connection(struct connection *, int *, void (*)(struct connection *));
int is_ipv6(int);
int get_pasv_socket(struct connection *, int, int *, unsigned char *);
#ifdef SUPPORT_IPV6
int get_pasv_socket_ipv6(struct connection *, int, int *, unsigned char *);
#endif
void write_to_socket(struct connection *, int, unsigned char *, int, void (*)(struct connection *));
struct read_buffer *alloc_read_buffer(struct connection *c);
void read_from_socket(struct connection *, int, struct read_buffer *, void (*)(struct connection *, struct read_buffer *));
void kill_buffer_data(struct read_buffer *, int);

/* cookies.c */

struct cookie {
	struct cookie *next;
	struct cookie *prev;
	unsigned char *name, *value;
	unsigned char *server;
	unsigned char *path, *domain;
	time_t expires; /* zero means undefined */
	int secure;
};

struct c_domain {
	struct c_domain *next;
	struct c_domain *prev;
	unsigned char domain[1];
};


extern struct list_head all_cookies;
extern struct list_head c_domains;

int set_cookie(struct terminal *, unsigned char *, unsigned char *);
void add_cookies(unsigned char **, int *, unsigned char *);
void init_cookies(void);
void cleanup_cookies(void);
void free_cookies(void);
int is_in_domain(unsigned char *d, unsigned char *s);
int is_path_prefix(unsigned char *d, unsigned char *s);
int cookie_expired(struct cookie *c);
void free_cookie(struct cookie *c);

/* auth.c */

unsigned char *get_auth_realm(unsigned char *url, unsigned char *head, int proxy);
unsigned char *get_auth_string(unsigned char *url, int proxy);
void cleanup_auth(void);
void add_auth(unsigned char *url, unsigned char *realm, unsigned char *user, unsigned char *password, int proxy);
int find_auth(unsigned char *url, unsigned char *realm);

/* http.c */

int get_http_code(unsigned char *head, int *code, int *version);
unsigned char *parse_http_header(unsigned char *, unsigned char *, unsigned char **);
unsigned char *parse_header_param(unsigned char *, unsigned char *, int);
void http_func(struct connection *);
void proxy_func(struct connection *);


/* https.c */

void https_func(struct connection *c);
#ifdef HAVE_SSL
extern int ssl_asked_for_password;
void ssl_finish(void);
SSL *getSSL(void);
#ifdef HAVE_SSL_CERTIFICATES
int verify_ssl_certificate(SSL *ssl, unsigned char *host);
int verify_ssl_cipher(SSL *ssl);
#endif
#endif

/* data.c */

void data_func(struct connection *);

/* file.c */

void file_func(struct connection *);

/* finger.c */

void finger_func(struct connection *);

/* ftp.c */

#if defined(IP_TOS) && defined(IPTOS_THROUGHPUT)
#define HAVE_IPTOS
#endif

void ftp_func(struct connection *);

/* smb.c */

void smb_func(struct connection *);

/* mailto.c */

void magnet_func(struct session *, unsigned char *);
void mailto_func(struct session *, unsigned char *);
void telnet_func(struct session *, unsigned char *);
void tn3270_func(struct session *, unsigned char *);
void mms_func(struct session *, unsigned char *);

/* kbd.c */

#define BM_BUTT		15
#define B_LEFT		0
#define B_MIDDLE	1
#define B_RIGHT		2
#define B_FOURTH	3
#define B_FIFTH		4
#define B_SIXTH		5
#define B_WHEELUP	8
#define B_WHEELDOWN	9
#define B_WHEELUP1	10
#define B_WHEELDOWN1	11
#define B_WHEELLEFT	12
#define B_WHEELRIGHT	13
#define B_WHEELLEFT1	14
#define B_WHEELRIGHT1	15

#define BM_ACT		48
#define B_DOWN		0
#define B_UP		16
#define B_DRAG		32
#define B_MOVE		48

#define KBD_ENTER	-0x100
#define KBD_BS		-0x101
#define KBD_TAB		-0x102
#define KBD_ESC		-0x103
#define KBD_LEFT	-0x104
#define KBD_RIGHT	-0x105
#define KBD_UP		-0x106
#define KBD_DOWN	-0x107
#define KBD_INS		-0x108
#define KBD_DEL		-0x109
#define KBD_HOME	-0x10a
#define KBD_END		-0x10b
#define KBD_PAGE_UP	-0x10c
#define KBD_PAGE_DOWN	-0x10d

#define KBD_F1		-0x120
#define KBD_F2		-0x121
#define KBD_F3		-0x122
#define KBD_F4		-0x123
#define KBD_F5		-0x124
#define KBD_F6		-0x125
#define KBD_F7		-0x126
#define KBD_F8		-0x127
#define KBD_F9		-0x128
#define KBD_F10		-0x129
#define KBD_F11		-0x12a
#define KBD_F12		-0x12b

#define KBD_CTRL_C	-0x200
#define KBD_CLOSE	-0x201

#define KBD_SHIFT	1
#define KBD_CTRL	2
#define KBD_ALT		4

void handle_trm(int, int, int, int, int, void *, int);
void free_all_itrms(void);
void dispatch_special(unsigned char *);
void kbd_ctrl_c(void);
int is_blocked(void);

struct os2_key {
	int x, y;
};

extern struct os2_key os2xtd[256];

struct itrm;

extern unsigned char init_seq[];
extern unsigned char init_seq_x_mouse[];
extern unsigned char init_seq_tw_mouse[];
extern unsigned char term_seq[];
extern unsigned char term_seq_x_mouse[];
extern unsigned char term_seq_tw_mouse[];

#if defined(GRDRV_SVGALIB) || defined(GRDRV_FB) || defined(GRDRV_GRX)
extern int kbd_set_raw;
struct itrm *handle_svgalib_keyboard(void (*)(void *, unsigned char *, int));
void svgalib_free_trm(struct itrm *);
void svgalib_block_itrm(struct itrm *);
int svgalib_unblock_itrm(struct itrm *);
#endif


struct rgb {
	unsigned char r, g, b; /* This is 3*8 bits with sRGB gamma (in sRGB space)
				* This is not rounded. */
	unsigned char pad;
};

#ifdef G

/* lru.c */

struct lru_entry {
	struct lru_entry *above, *below, *next;
	struct lru_entry **previous;
	void *data;
	unsigned bytes_consumed;
};

struct lru {
	int (*compare_function)(void *, void *);
	struct lru_entry *top, *bottom;
	my_uintptr_t bytes, items;
};

void lru_insert(struct lru *cache, void *entry, struct lru_entry **row, unsigned bytes_consumed);
void *lru_get_bottom(struct lru *cache);
void lru_destroy_bottom(struct lru *cache);
void lru_init (struct lru *cache, int (*compare_function)(void *entry, void *templ));
void *lru_lookup(struct lru *cache, void *templ, struct lru_entry *row);

/* drivers.c */

struct irgb {
	int r,g,b; /* 0xffff=full white, 0x0000=full black */
};

/* Bitmap is allowed to pass only to that driver from which was obtained.
 * It is forbidden to get bitmap from svga driver and pass it to X driver.
 * It is impossible to get an error when registering a bitmap
 */
struct bitmap{
	int x,y; /* Dimensions */
	int skip; /* Byte distance between vertically consecutive pixels */
	void *data; /* Pointer to room for topleft pixel */
	void *flags; /* Allocation flags for the driver */
};

struct rect {
	int x1, x2, y1, y2;
};

struct rect_set {
	int rl;
	int m;
	struct rect r[1];
};

struct graphics_device {
	/* Only graphics driver is allowed to write to this */

	struct rect size; /* Size of the window */
	/*int left, right, top, bottom;*/
	struct rect clip;
		/* right, bottom are coords of the first point that are outside the clipping area */

	void *driver_data;

	/* Only user is allowed to write here, driver inits to zero's */
	void *user_data;
	void (*redraw_handler)(struct graphics_device *dev, struct rect *r);
	void (*resize_handler)(struct graphics_device *dev);
	void (*keyboard_handler)(struct graphics_device *dev, int key, int flags);
	void (*mouse_handler)(struct graphics_device *dev, int x, int y, int buttons);
};

struct graphics_driver {
	unsigned char *name;
	unsigned char *(*init_driver)(unsigned char *param, unsigned char *display);	/* param is get from get_driver_param and saved into configure file */

	/* Creates new device and returns pointer to it */
	struct graphics_device *(*init_device)(void);

	/* Destroys the device */
	void (*shutdown_device)(struct graphics_device *dev);

	void (*shutdown_driver)(void);

	void (*emergency_shutdown)(void);

	void (*after_fork)(void);

	unsigned char *(*get_driver_param)(void);	/* returns allocated string with parameter given to init_driver function */
	unsigned char *(*get_af_unix_name)(void);

	void (*get_margin)(int *left, int *right, int *top, int *bottom);
	int (*set_margin)(int left, int right, int top, int bottom);

	/* dest must have x and y filled in when get_empty_bitmap is called */
	int (*get_empty_bitmap)(struct bitmap *dest);

	/* dest must have x and y filled in when get_filled_bitmap is called. */
	/* bitmap is created, pre-filled with n_bytes of pattern, and registered */
	/*int (*get_filled_bitmap)(struct bitmap *dest, long color);*/

	void (*register_bitmap)(struct bitmap *bmp);

	void *(*prepare_strip)(struct bitmap *bmp, int top, int lines);
	void (*commit_strip)(struct bitmap *bmp, int top, int lines);

	/* Must not touch x and y. Suitable for re-registering. */
	void (*unregister_bitmap)(struct bitmap *bmp);
	void (*draw_bitmap)(struct graphics_device *dev, struct bitmap *hndl, int x, int y);
	/*void (*draw_bitmaps)(struct graphics_device *dev, struct bitmap **hndls, int n, int x, int y);*/

	/* Input into get_color has gamma 1/display_gamma.
	 * Input of 255 means exactly the largest sample the display is able to produce.
	 * Thus, if we have 3 bits for red, we will perform this code:
	 * red=((red*7)+127)/255;
	 */
	long (*get_color)(int rgb);

	void (*fill_area)(struct graphics_device *dev, int x1, int y1, int x2, int y2, long color);
	void (*draw_hline)(struct graphics_device *dev, int left, int y, int right, long color);
	void (*draw_vline)(struct graphics_device *dev, int x, int top, int bottom, long color);
	int (*hscroll)(struct graphics_device *dev, struct rect_set **set, int sc);
	int (*vscroll)(struct graphics_device *dev, struct rect_set **set, int sc);
	 /* When scrolling, the empty spaces will have undefined contents. */
	 /* returns:
	    0 - the caller should not care about redrawing, redraw will be sent
	    1 - the caller should redraw uncovered area */
	 /* when set is not NULL rectangles in the set (uncovered area) should be redrawn */
	void (*set_clip_area)(struct graphics_device *dev, struct rect *r);

	int (*block)(struct graphics_device *dev);	/* restore old videomode and disable drawing on terminal */
	int (*unblock)(struct graphics_device *dev);	/* reenable the terminal (return -1 if failed) */

	void (*set_title)(struct graphics_device *dev, unsigned char *title);
		/* set window title. title is in utf-8 encoding -- you should recode it to device charset */
		/* if device doesn't support titles (svgalib, framebuffer), this should be NULL, not empty function ! */

	int (*exec)(unsigned char *command, int flag);
		/* -if !NULL executes command on this graphics device,
		   -if NULL links uses generic (console) command executing
		    functions
		   -return value is the same as of the 'system' syscall
		   -if flag is !0, run command in separate shell
		    else run command directly
		 */

	void (*set_clipboard_text)(struct graphics_device *gd, unsigned char *text);
	unsigned char *(*get_clipboard_text)(void);

	int depth; /* Data layout
		    * depth
		    *  8 7 6 5 4 3 2 1 0
		    * +-+---------+-----+
		    * | |         |     |
		    * +-+---------+-----+
		    *
		    * 0 - 2 Number of bytes per pixel in passed bitmaps
		    * 3 - 7 Number of significant bits per pixel -- 1, 4, 8, 15, 16, 24
		    * 8   0-- normal order, 1-- misordered.Has the same value as vga_misordered from the VGA mode.
		    *
		    * This number is to be used by the layer that generates images.
		    * Memory layout for 1 bytes per pixel is:
		    * 2 colors:
		    *  7 6 5 4 3 2 1 0
		    * +-------------+-+
		    * |      0      |B| B is The Bit. 0 black, 1 white
		    * +-------------+-+
		    *
		    * 16 colors:
		    *  7 6 5 4 3 2 1 0
		    * +-------+-------+
		    * |   0   | PIXEL | Pixel is 4-bit index into palette
		    * +-------+-------+
		    *
		    * 256 colors:
		    *  7 6 5 4 3 2 1 0
		    * +---------------+
		    * |  --PIXEL--    | Pixels is 8-bit index into palette
		    * +---------------+
		    */
	int x, y;	/* size of screen. only for drivers that use virtual devices */
	int flags;	/* GD_xxx flags */
	int kbd_codepage;
	unsigned char *shell;
		/* -if exec is NULL string is unused
		   -otherwise this string describes shell to be executed by the
		    exec function, the '%' char means string to be executed
		   -shell cannot be NULL
		   -if exec is !NULL and shell is empty, exec should use some
		    default shell (e.g. "xterm -e %")
		*/
};

#define GD_DONT_USE_SCROLL	1
#define GD_NEED_CODEPAGE	2
#define GD_ONLY_1_WINDOW	4
#define GD_NOAUTO		8
#define GD_NO_OS_SHELL		16
#define GD_NO_LIBEVENT		32

extern struct graphics_driver *drv;

void add_graphics_drivers(unsigned char **s, int *l);
unsigned char *init_graphics(unsigned char *, unsigned char *, unsigned char *);
void shutdown_graphics(void);
void update_driver_param(void);
int g_kbd_codepage(struct graphics_driver *drv);

int dummy_block(struct graphics_device *);
int dummy_unblock(struct graphics_device *);

extern struct graphics_device **virtual_devices;
extern int n_virtual_devices;
extern struct graphics_device *current_virtual_device;

void generic_set_clip_area(struct graphics_device *dev, struct rect *r);

int init_virtual_devices(struct graphics_driver *, int);
struct graphics_device *init_virtual_device(void);
#define VD_NEXT		-1
void switch_virtual_device(int);
void shutdown_virtual_device(struct graphics_device *dev);
void resize_virtual_devices(int x, int y);
void shutdown_virtual_devices(void);

/* dip.c */

/* Digital Image Processing utilities
 * (c) 2000 Clock <clock@atrey.karlin.mff.cuni.cz>
 *
 * This file is a part of Links
 *
 * This file does gray scaling (for prescaling fonts), color scaling (for scaling images
 * where different size is defined in the HTML), two colors mixing (alpha monochromatic letter
 * on a monochromatic backround and font operations.
 */

#if !SIZEOF_UNSIGNED_SHORT || SIZEOF_UNSIGNED_SHORT > 2
#define limit_16(x)	((x) >= 0xffff ? 0xffff : (x))
#define cmd_limit_16(x)	do { if ((x) >= 0xffff) (x) = 0xffff; } while (0)
#else
#define limit_16(x)	(x)
#define cmd_limit_16(x)	do { } while (0)
#endif

#define FC_COLOR 0
#define FC_BW 1

#define sRGB_gamma	0.45455		/* For HTML, which runs
					 * according to sRGB standard. Number
					 * in HTML tag is linear to photons raised
					 * to this power.
					 */

extern unsigned aspect; /* Must hold at least 20 bits */
my_uintptr_t fontcache_info(int type);

#define G_BFU_FONT_SIZE menu_font_size

struct letter {
	const unsigned char *begin; /* Begin in the byte stream (of PNG data) */
	int length; /* Length (in bytes) of the PNG data in the byte stream */
	int code; /* Unicode code of the character */
	short xsize; /* x size of the PNG image */
	short ysize; /* y size of the PNG image */
	struct lru_entry* color_list;
	struct lru_entry* bw_list;
};

struct font {
	unsigned char *family;
	unsigned char *weight;
	unsigned char *slant;
	unsigned char *adstyl;
	unsigned char *spacing;
	int begin; /* Begin in the letter stream */
	int length; /* Length in the letter stream */
};

struct style {
	int refcount;
	unsigned char r0, g0, b0, r1, g1, b1;
	/* ?0 are background, ?1 foreground.
	 * These are unrounded 8-bit sRGB space
	 */
	int height;
	int flags; /* non-zero means underline */
	long underline_color; /* Valid only if flags are nonzero */
	int *table; /* First is refcount, then n_fonts entries. Total
		     * size is n_fonts+1 integers.
		     */
	int mono_space; /* -1 if the font is not monospaced
			 * width of the space otherwise
			 */
	int mono_height; /* Height of the space if mono_space is >=0
			  * undefined otherwise
			  */
	/*
	unsigned char font[1];
	*/
};

struct font_cache_entry {
	int type; /* One of FC_BW or FC_COLOR */
	int r0,g0,b0,r1,g1,b1; /* Invalid for FC_BW */
	struct bitmap bitmap; /* If type==FC_BW, then this is not a normal registered
			       * bitmap, but a black-and-white bitmap
			       */
	int mono_space, mono_height; /* if the letter was rendered for a
	monospace font, then size of the space. Otherwise, mono_space
	is -1 and mono_height is undefined. */
};


struct cached_image;

void g_print_text(struct graphics_device *device, int x, int y, struct style *style, unsigned char *text, int *width);
int g_text_width(struct style *style, unsigned char *text);
int g_char_width(struct style *style, unsigned ch);
/*unsigned char ags_8_to_8(unsigned char input, float gamma);*/
unsigned short ags_8_to_16(unsigned char input, float gamma);
unsigned char ags_16_to_8(unsigned short input, float gamma);
unsigned short ags_16_to_16(unsigned short input, float gamma);
void agx_24_to_48(unsigned short *dest, const unsigned char *src, int
			  lenght, float red_gamma, float green_gamma, float
			  blue_gamma);
void make_gamma_table(struct cached_image *cimg);
void agx_24_to_48_table(unsigned short *dest, const unsigned char *src
	,int lenght, unsigned short *gamma_table);
void agx_48_to_48_table(unsigned short *dest,
		const unsigned short *src, int lenght, unsigned short *table);
void agx_48_to_48(unsigned short *dest,
		const unsigned short *src, int lenght, float red_gamma,
		float green_gamma, float blue_gamma);
void agx_and_uc_32_to_48_table(unsigned short *dest,
		const unsigned char *src, int lenght, unsigned short *table,
		unsigned short rb, unsigned short gb, unsigned short bb);
void agx_and_uc_32_to_48(unsigned short *dest,
		const unsigned char *src, int lenght, float red_gamma,
		float green_gamma, float blue_gamma, unsigned short rb, unsigned
		short gb, unsigned short bb);
void agx_and_uc_64_to_48_table(unsigned short *dest,
		const unsigned short *src, int lenght, unsigned short *gamma_table,
		unsigned short rb, unsigned short gb, unsigned short bb);
void agx_and_uc_64_to_48(unsigned short *dest,
		const unsigned short *src, int lenght, float red_gamma,
		float green_gamma, float blue_gamma, unsigned short rb, unsigned
		short gb, unsigned short bb);
void mix_one_color_48(unsigned short *dest, int length,
		   unsigned short r, unsigned short g, unsigned short b);
void mix_one_color_24(unsigned char *dest, int length,
		   unsigned char r, unsigned char g, unsigned char b);
void scale_color(unsigned short *in, int ix, int iy, unsigned short **out,
	int ox, int oy);
void update_aspect(void);

struct wrap_struct {
	struct style *style;
	unsigned char *text;
	int pos;
	int width;
	void *obj;
	void *last_wrap_obj;
	unsigned char *last_wrap;
	int force_break;
};

int g_wrap_text(struct wrap_struct *);

int hack_rgb(int rgb);

#define FF_UNDERLINE	1

struct style *g_get_style(int fg, int bg, int size, unsigned char *font, int fflags);
struct style *g_invert_style(struct style *);
void g_free_style(struct style *style0);
struct style *g_clone_style(struct style *);

extern long gamma_cache_color;
extern int gamma_cache_rgb;

extern long real_dip_get_color_sRGB(int rgb);

static inline long dip_get_color_sRGB(int rgb)
{
	if (rgb == gamma_cache_rgb) return gamma_cache_color;
	else return real_dip_get_color_sRGB(rgb);
}

void init_dip(void);
void get_links_icon(unsigned char **data, int *width, int *height, int *skip, int pad);

#ifdef PNG_USER_MEM_SUPPORTED
void *my_png_alloc(png_structp png_ptr, png_size_t size);
void my_png_free(png_structp png_ptr, void *ptr);
#endif

/* dither.c */

extern int slow_fpu;	/* -1 --- don't know, 0 --- no, 1 --- yes */

/* Dithering functions (for blocks of pixels being dithered into bitmaps) */
void dither (unsigned short *in, struct bitmap *out);
int *dither_start(unsigned short *in, struct bitmap *out);
void dither_restart(unsigned short *in, struct bitmap *out, int *dregs);
extern void (*round_fn)(unsigned short *in, struct bitmap *out);

long (*get_color_fn(int depth))(int rgb);
void init_dither(int depth);
void round_color_sRGB_to_48(unsigned short *red, unsigned short *green,
		unsigned short *blue, int rgb);

void q_palette(unsigned size, unsigned color, unsigned scale, unsigned rgb[3]);
double rgb_distance(int r1, int g1, int b1, int r2, int g2, int b2);

void free_dither(void);

#endif

/* terminal.c */

extern unsigned char frame_dumb[];

#ifndef ENABLE_UTF8
typedef unsigned char char_t;
#else
typedef unsigned char_t;
#endif

typedef struct {
	char_t ch;
	unsigned char at;
}
#if defined(ENABLE_UTF8)
ATTR_PACKED
#endif
chr;

struct links_event {
	int ev;
	int x;
	int y;
	my_intptr_t b;
};

#define EV_INIT		0
#define EV_KBD		1
#define EV_MOUSE	2
#define EV_REDRAW	3
#define EV_RESIZE	4
#define EV_ABORT	5

#define EVH_NOT_PROCESSED		0
#define EVH_LINK_KEYDOWN_PROCESSED	1
#define EVH_LINK_KEYPRESS_PROCESSED	2
#define EVH_DOCUMENT_KEYDOWN_PROCESSED	3
#define EVH_DOCUMENT_KEYPRESS_PROCESSED	4

struct window {
	struct window *next;
	struct window *prev;
	void (*handler)(struct window *, struct links_event *, int fwd);
	void *data;
	int xp, yp;
	struct terminal *term;
#ifdef G
	struct rect pos;
	struct rect redr;
#endif
};

#define MAX_TERM_LEN	32	/* this must be multiple of 8! (alignment problems) */

#define MAX_CWD_LEN	4096	/* this must be multiple of 8! (alignment problems) */

#define ENV_XWIN	1
#define ENV_SCREEN	2
#define ENV_OS2VIO	4
#define ENV_BE		8
#define ENV_TWIN	16
#define ENV_WIN32	32
#define ENV_INTERIX	64
#define ENV_G		32768

struct term_spec;

struct terminal {
	struct terminal *next;
	struct terminal *prev;
	tcount count;

	int x;
	int y;
	/* text only */
	int master;
	int fdin;
	int fdout;
	int environment;
	unsigned char term[MAX_TERM_LEN];
	unsigned char cwd[MAX_CWD_LEN];
	chr *screen;
	chr *last_screen;
	struct term_spec *spec;
	int default_character_set;
	int cx;
	int cy;
	int lcx;
	int lcy;
	int dirty;
	int redrawing;
	int blocked;
	unsigned char *input_queue;
	int qlen;
	int real_x;
	int real_y;
	int left_margin;
	int top_margin;
	/* end-of text only */

	struct list_head windows;
	unsigned char *title;

	int handle_to_close;
#ifdef G
	struct graphics_device *dev;
	int last_mouse_x;
	int last_mouse_y;
	int last_mouse_b;
#endif
#if defined(G) || defined(ENABLE_UTF8)
	unsigned char utf8_buffer[7];
#endif
};

struct term_spec {
	struct term_spec *next;
	struct term_spec *prev;
	unsigned char term[MAX_TERM_LEN];
	int mode;
	int m11_hack;
	int restrict_852;
	int block_cursor;
	int col;
	int braille;
	int character_set;
	int left_margin;
	int right_margin;
	int top_margin;
	int bottom_margin;
};

#define TERM_DUMB	0
#define TERM_VT100	1
#define TERM_LINUX	2
#define TERM_KOI8	3
#define TERM_FREEBSD	4

#define ATTR_FRAME	0x80

extern struct list_head term_specs;
extern struct list_head terminals;

static inline int term_charset(struct terminal *term)
{
	if (term->spec->character_set >= 0)
		return term->spec->character_set;
	return term->default_character_set;
}

int hard_write(int, unsigned char *, int);
int hard_read(int, unsigned char *, int);
unsigned char *get_cwd(void);
void set_cwd(unsigned char *);
unsigned char get_attribute(int, int);
struct terminal *init_term(int, int, void (*)(struct window *, struct links_event *, int));
#ifdef G
struct terminal *init_gfx_term(void (*)(struct window *, struct links_event *, int), unsigned char *, void *, int);
int restrict_clip_area(struct graphics_device *, struct rect *, int, int, int, int);
#endif
struct term_spec *new_term_spec(unsigned char *);
void free_term_specs(void);
void destroy_terminal(struct terminal *);
void cls_redraw_all_terminals(void);
void redraw_below_window(struct window *);
void add_window(struct terminal *, void (*)(struct window *, struct links_event *, int), void *);
void add_window_at_pos(struct terminal *, void (*)(struct window *, struct links_event *, int), void *, struct window *);
void delete_window(struct window *);
void delete_window_ev(struct window *, struct links_event *ev);
void set_window_ptr(struct window *, int, int);
void get_parent_ptr(struct window *, int *, int *);
void add_empty_window(struct terminal *, void (*)(void *), void *);
void draw_to_window(struct window *, void (*)(struct terminal *, void *), void *);
void redraw_screen(struct terminal *);
void redraw_all_terminals(void);

#ifdef G

void set_window_pos(struct window *, int, int, int, int);
int do_rects_intersect(struct rect *, struct rect *);
void intersect_rect(struct rect *, struct rect *, struct rect *);
void unite_rect(struct rect *, struct rect *, struct rect *);
int is_rect_valid(struct rect *);

struct rect_set *init_rect_set(void);
void add_to_rect_set(struct rect_set **, struct rect *);
void exclude_rect_from_set(struct rect_set **, struct rect *);
static inline void exclude_from_set(struct rect_set **s, int x1, int y1, int x2, int y2)
{
	struct rect r;
	r.x1 = x1, r.x2 = x2, r.y1 = y1, r.y2 = y2;
	exclude_rect_from_set(s, &r);
}

void t_redraw(struct graphics_device *, struct rect *);
void t_resize(struct graphics_device *);
void t_kbd(struct graphics_device *, int, int);
void t_mouse(struct graphics_device *, int, int, int);

#endif

/* text only */
void set_char(struct terminal *, int, int, unsigned, unsigned char);
chr *get_char(struct terminal *, int, int);
void set_color(struct terminal *, int, int, unsigned char);
void set_only_char(struct terminal *, int, int, unsigned, unsigned char);
void set_line(struct terminal *, int, int, int, chr *);
void set_line_color(struct terminal *, int, int, int, unsigned char);
void fill_area(struct terminal *, int, int, int, int, unsigned, unsigned char);
void draw_frame(struct terminal *, int, int, int, int, unsigned char, int);
void print_text(struct terminal *, int, int, int, unsigned char *, unsigned char);
void set_cursor(struct terminal *, int, int, int, int);

void destroy_all_terminals(void);
void block_itrm(int);
int unblock_itrm(int);
void exec_thread(unsigned char *, int);
void close_handle(void *);

#define TERM_FN_TITLE	1
#define TERM_FN_RESIZE	2

#ifdef G
int have_extra_exec(void);
#endif
void exec_on_terminal(struct terminal *, unsigned char *, unsigned char *, unsigned char);
void set_terminal_title(struct terminal *, unsigned char *);
void do_terminal_function(struct terminal *, unsigned char, unsigned char *);

/* language.c */

#include "language.h"

extern unsigned char dummyarray[];

extern int current_language;

void init_trans(void);
void shutdown_trans(void);
#if defined(OS2) || defined(DOS)
int get_country_language(int c);
#endif
int get_default_language(void);
int get_default_charset(void);
int get_current_language(void);
unsigned char *get_text_translation(unsigned char *, struct terminal *term);
unsigned char *get_english_translation(unsigned char *);
void set_language(int);
int n_languages(void);
unsigned char *language_name(int);

#define _(x_, y_) get_text_translation(x_, y_)
#define TEXT_(x) (dummyarray + x) /* TEXT causes name clash on windows */

/* dos.c */

#ifdef DOS
void dos_poll_break(void);
void dos_mouse_terminate(void);
void dos_save_screen(void);
void dos_restore_screen(void);
#else
#define dos_poll_break()	do { } while (0)
#endif

/* grx.c */

#ifdef GRDRV_GRX
extern int grx_mouse_initialized;
void grx_mouse_poll(void);
int grx_mouse_event(void);
#endif

/* af_unix.c */

extern int s_unix_fd;
int bind_to_af_unix(unsigned char *name);
void af_unix_close(void);

/* main.c */

extern int terminal_pipe[2];

extern int retval;

extern unsigned char *path_to_exe;
extern unsigned char **g_argv;
extern int g_argc;

void sig_tstp(struct terminal *t);
void sig_cont(struct terminal *t);

void unhandle_terminal_signals(struct terminal *term);
int attach_terminal(int, int, int, void *, int);
#ifdef G
int attach_g_terminal(unsigned char *, void *, int);
void gfx_connection(int);
#endif

/* types.c */

struct list;

struct assoc {
	struct assoc *next;
	struct assoc *prev;
	unsigned char type;
	int depth;
	void *fotr;

	unsigned char *label;
	unsigned char *ct;
	unsigned char *prog;
	int cons;
	int xwin;
	int block;
	int ask;
	int accept_http;
	int accept_ftp;
	int system;
};

struct extension {
	struct extension *next;
	struct extension *prev;
	unsigned char type;
	int depth;
	void *fotr;

	unsigned char *ext;
	unsigned char *ct;
};

struct protocol_program {
	struct protocol_program *next;
	struct protocol_program *prev;
	unsigned char *prog;
	int system;
};

extern struct list assoc;
extern struct list extensions;

extern struct list_head mailto_prog;
extern struct list_head telnet_prog;
extern struct list_head tn3270_prog;
extern struct list_head mms_prog;
extern struct list_head magnet_prog;

unsigned char *get_compress_by_extension(unsigned char *ext, unsigned char *ext_end);
unsigned char *get_content_type_by_extension(unsigned char *url);
unsigned char *get_content_type(unsigned char *, unsigned char *);
unsigned char *get_content_encoding(unsigned char *head, unsigned char *url);
unsigned char *encoding_2_extension(unsigned char *);
struct assoc *get_type_assoc(struct terminal *term, unsigned char *, int *);
int is_html_type(unsigned char *ct);
unsigned char *get_filename_from_header(unsigned char *head);
unsigned char *get_filename_from_url(unsigned char *, unsigned char *, int);

void menu_assoc_manager(struct terminal *,void *,struct session *);
void update_assoc(struct assoc *);
void menu_ext_manager(struct terminal *,void *,struct session *);
void update_ext(struct extension *);
void update_prog(struct list_head *, unsigned char *, int);
unsigned char *get_prog(struct list_head *);
void create_initial_extensions(void);


void free_types(void);

/* block.c */

/*URL blocking calls*/
struct block {
	struct block *next;
	struct block *prev;
	unsigned char type;
	int depth;
	void* fotr;

	unsigned char* url;
};
extern struct list blocks;
int is_url_blocked(unsigned char* url);
void block_url_query(struct session *ses, unsigned char *u);
void* block_url_add(struct session *ses, unsigned char *url);
void block_manager(struct terminal *term,void *fcp,struct session *ses);
void free_blocks(void);

/* objreq.c */

#define O_WAITING	0
#define O_LOADING	1
#define O_FAILED	-1
#define O_INCOMPLETE	-2
#define O_OK		-3

struct object_request {
	struct object_request *next;
	struct object_request *prev;
	int refcount;
	tcount count;
	tcount term;
	struct status stat;
	struct cache_entry *ce_internal;
	struct cache_entry *ce;
	unsigned char *orig_url;
	unsigned char *url;
	unsigned char *prev_url;	/* allocated string with referrer or NULL */
	unsigned char *goto_position;
	int pri;
	int cache;
	void (*upcall)(struct object_request *, void *);
	void *data;
	int redirect_cnt;
	int state;
	int hold;
	int dont_print_error;
	struct timer *timer;

	off_t last_bytes;

	ttime last_update;
	ttime z;
};

void request_object(struct terminal *, unsigned char *, unsigned char *, int, int, int, void (*)(struct object_request *, void *), void *, struct object_request **);
void clone_object(struct object_request *, struct object_request **);
void release_object(struct object_request **);
void release_object_get_stat(struct object_request **, struct status *, int);
void detach_object_connection(struct object_request *, off_t);

/* compress.c */

#if defined(HAVE_ZLIB) || defined(HAVE_BZIP2) || defined(HAVE_LZMA)
#define HAVE_ANY_COMPRESSION
#endif

extern my_uintptr_t decompressed_cache_size;

int get_file_by_term(struct terminal *term, struct cache_entry *ce, unsigned char **start, unsigned char **end, int *errp);
int get_file(struct object_request *o, unsigned char **start, unsigned char **end);
void free_decompressed_data(struct cache_entry *e);
void add_compress_methods(unsigned char **s, int *l);

/* session.c */

struct link_def {
	unsigned char *link;
	unsigned char *target;

	unsigned char *label;	/* only for image maps */
	unsigned char *shape;
	unsigned char *coords;

	unsigned char *onclick;
	unsigned char *ondblclick;
	unsigned char *onmousedown;
	unsigned char *onmouseup;
	unsigned char *onmouseover;
	unsigned char *onmouseout;
	unsigned char *onmousemove;
};

struct line {
	int l;
	int allocated;
	chr *d;
};

struct point {
	int x;
	int y;
};

struct form {
	unsigned char *action;
	unsigned char *target;
	unsigned char *form_name;
	unsigned char *onsubmit;
	int method;
	int num;
};

#define FM_GET		0
#define FM_POST		1
#define FM_POST_MP	2

#define FC_TEXT		1
#define FC_PASSWORD	2
#define FC_FILE		3
#define FC_TEXTAREA	4
#define FC_CHECKBOX	5
#define FC_RADIO	6
#define FC_SELECT	7
#define FC_SUBMIT	8
#define FC_IMAGE	9
#define FC_RESET	10
#define FC_HIDDEN	11
#define FC_BUTTON	12

struct menu_item;

struct form_control {
	struct form_control *next;
	struct form_control *prev;
	int form_num;	/* cislo formulare */
	int ctrl_num;	/* identifikace polozky v ramci formulare */
	int g_ctrl_num;	/* identifikace polozky mezi vsemi polozkami (poradi v poli form_info) */
	int position;
	int method;
	unsigned char *action;
	unsigned char *target;
	unsigned char *onsubmit; /* script to be executed on submit */
	int type;
	unsigned char *name;
	unsigned char *form_name;
	unsigned char *alt;
	int ro;
	unsigned char *default_value;
	int default_state;
	int size;
	int cols, rows, wrap;
	int maxlength;
	int nvalues; /* number of values in a select item */
	unsigned char **values; /* values of a select item */
	unsigned char **labels; /* labels (shown text) of a select item */
	struct menu_item *menu;
};

struct form_state {
	int form_num;	/* cislo formulare */
	int ctrl_num;	/* identifikace polozky v ramci formulare */
	int g_ctrl_num;	/* identifikace polozky mezi vsemi polozkami (poradi v poli form_info) */
	int position;
	int type;
	unsigned char *value; /* selected value of a select item */
	int state; /* index of selected item of a select item */
	int vpos;
	int vypos;
#ifdef JS
	int changed;	/* flag if form element has changed --- for onchange handler */
#endif
};

struct js_event_spec;
struct g_object;

struct link {
	int type;   /* one of L_XXX constants */
	int num;    /* link number (used when user turns on link numbering) */
	unsigned char *where;   /* URL of the link */
	unsigned char *target;   /* name of target frame where to open the link */
	unsigned char *where_img;   /* URL of image (if any) */
	unsigned char *img_alt;		/* alt of image (if any) - valid only when link is an image */
	struct form_control *form;   /* form info, usually NULL */
	unsigned sel_color;   /* link color */
	int n;   /* number of points */
	int first_point_to_move;
	struct point *pos;
	struct js_event_spec *js_event;
	int obj_order;
#ifdef G
	struct rect r;
	struct g_object *obj;
#endif
};

#define L_LINK		0
#define L_BUTTON	1
#define L_CHECKBOX	2
#define L_SELECT	3
#define L_FIELD		4
#define L_AREA		5

struct link_bg {
	int x, y;
	unsigned char c;
};

struct tag {
	struct tag *next;
	struct tag *prev;
	int x;
	int y;
	unsigned char name[1];
};

extern struct rgb palette_16_colors[16];

/* when you add anything, don't forget to initialize it in default.c on line:
 * struct document_setup dds = { ... };
 */
struct document_setup {
	int assume_cp, hard_assume;
	int tables, frames, break_long_lines, images, image_names;
	int margin;
	int num_links, table_order;
	int auto_refresh;
	int font_size;
	int display_images;
	int image_scale;
	int porn_enable;
	int target_in_new_window;
	int t_text_color;
	int t_link_color;
	int t_background_color;
	int t_ignore_document_color;
	int g_text_color;
	int g_link_color;
	int g_background_color;
	int g_ignore_document_color;
};


/* IMPORTANT!!!!!
 * if you add anything, fix it in compare_opt and if you add it into
 * document_setup, fix it in ds2do too
 */

struct document_options {
	int xw, yw; /* size of window */
	int xp, yp; /* pos of window */
	int scrolling;
	int col, cp, assume_cp, hard_assume;
	int braille;
	int tables, frames, break_long_lines, images, image_names, margin;
	int js_enable;
	int plain;
	int num_links, table_order;
	int auto_refresh;
	struct rgb default_fg;
	struct rgb default_bg;
	struct rgb default_link;
	unsigned char *framename;
	int font_size;
	int display_images;
	int image_scale;
	int porn_enable;
	double bfu_aspect; /* 0.1 to 10.0, 1.0 default. >1 makes circle wider */
	int real_cp;	/* codepage of document. Does not really belong here. Must not be compared. Used only in get_attr_val */
};

static inline void color2rgb(struct rgb *rgb, int color)
{
	memset(rgb, 0, sizeof(struct rgb));
	rgb->r = (color >> 16) & 0xff;
	rgb->g = (color >> 8) & 0xff;
	rgb->b = color & 0xff;
}

static inline void ds2do(struct document_setup *ds, struct document_options *doo, int col)
{
	doo->assume_cp = ds->assume_cp;
	doo->hard_assume = ds->hard_assume;
	doo->tables = ds->tables;
	doo->frames = ds->frames;
	doo->break_long_lines = ds->break_long_lines;
	doo->images = ds->images;
	doo->image_names = ds->image_names;
	doo->margin = ds->margin;
	doo->num_links = ds->num_links;
	doo->table_order = ds->table_order;
	doo->auto_refresh = ds->auto_refresh;
	doo->font_size = ds->font_size;
	doo->display_images = ds->display_images;
	doo->image_scale = ds->image_scale;
	doo->porn_enable = ds->porn_enable;
	if (!F) {
		if (!col) {
			doo->default_fg = palette_16_colors[7];
			doo->default_bg = palette_16_colors[0];
			doo->default_link = palette_16_colors[15];
		} else {
			doo->default_fg = palette_16_colors[ds->t_text_color];
			doo->default_bg = palette_16_colors[ds->t_background_color];
			doo->default_link = palette_16_colors[ds->t_link_color];
		}
	}
#ifdef G
	else {
		color2rgb(&doo->default_fg, ds->g_text_color);
		color2rgb(&doo->default_bg, ds->g_background_color);
		color2rgb(&doo->default_link, ds->g_link_color);
	}
#endif
}

struct node {
	struct node *next;
	struct node *prev;
	int x, y;
	int xw, yw;
};

struct search {
	int idx;
	int x, y;
	unsigned short n;
	unsigned short co;
};

struct frameset_desc;

struct frame_desc {
	struct frameset_desc *subframe;
	unsigned char *name;
	unsigned char *url;
	int marginwidth;
	int marginheight;
	int line;
	int xw, yw;
	unsigned char scrolling;
};

struct frameset_desc {
	int n;			/* = x * y */
	int x, y;		/* velikost */
	int xp, yp;		/* pozice pri pridavani */
#ifdef JS
	unsigned char *onload_code;
#endif
	struct frame_desc f[1];
};

struct f_data;

#ifdef G

#define SHAPE_DEFAULT	0
#define SHAPE_RECT	1
#define SHAPE_CIRCLE	2
#define SHAPE_POLY	3

struct map_area {
	int shape;
	int *coords;
	int ncoords;
	int link_num;
};

struct image_map {
	int n_areas;
	struct map_area area[1];
};

struct background {
	union {
		int sRGB; /* This is 3*8 bytes with sRGB_gamma (in sRGB space).
			     This is not rounded. */
	} u;
};

struct f_data_c;

#define G_OBJ_ALIGN_SPECIAL	(MAXINT - 2)
#define G_OBJ_ALIGN_MIDDLE	(MAXINT - 2)
#define G_OBJ_ALIGN_TOP		(MAXINT - 1)

struct g_object {
	/* public data --- must be same in all g_object* structures */
	void (*mouse_event)(struct f_data_c *, struct g_object *, int, int, int);
		/* pos is relative to object */
	void (*draw)(struct f_data_c *, struct g_object *, int, int);
		/* absolute pos on screen */
	void (*destruct)(struct g_object *);
	void (*get_list)(struct g_object *, void (*)(struct g_object *parent, struct g_object *child));
	int x, y, xw, yw;
	struct g_object *parent;
	/* private data... */
};

struct g_object_text {
	/* public data --- must be same in all g_object* structures */
	void (*mouse_event)(struct f_data_c *, struct g_object_text *, int, int, int);
	void (*draw)(struct f_data_c *, struct g_object_text *, int, int);
	void (*destruct)(struct g_object_text *);
	void (*get_list)(struct g_object_text *, void (*)(struct g_object *parent, struct g_object *child));
	int x, y, xw, yw;
	struct g_object *parent;
	/* must be same in g_object_image */
	int link_num;
	int link_order;
	struct image_map *map;
	int ismap;
	/* end of compatibility with g_object_image */
	struct style *style;
	int srch_pos;
	unsigned char text[1];
};

struct g_object_line {
	/* public data --- must be same in all g_object* structures */
	void (*mouse_event)(struct f_data_c *, struct g_object_line *, int, int, int);
	void (*draw)(struct f_data_c *, struct g_object_line *, int, int);
	void (*destruct)(struct g_object_line *);
	void (*get_list)(struct g_object_line *, void (*)(struct g_object *parent, struct g_object *child));
	int x, y, xw, yw;
	struct g_object *parent;
	/* private data */
	struct background *bg;
	int n_entries;
	struct g_object *entries[1];
};

struct g_object_area {
	/* public data --- must be same in all g_object* structures */
	void (*mouse_event)(struct f_data_c *, struct g_object_area *, int, int, int);
	void (*draw)(struct f_data_c *, struct g_object_area *, int, int);
	void (*destruct)(struct g_object_area *);
	void (*get_list)(struct g_object_area *, void (*)(struct g_object *parent, struct g_object *child));
	int x, y, xw, yw;
	struct g_object *parent;
	/* private data */
	struct background *bg;
	int n_lines;
	struct g_object_line *lines[1];
};

struct table;

struct g_object_table {
	/* public data --- must be same in all g_object* structures */
	void (*mouse_event)(struct f_data_c *, struct g_object_table *, int, int, int);
	void (*draw)(struct f_data_c *, struct g_object_table *, int, int);
	void (*destruct)(struct g_object_table *);
	void (*get_list)(struct g_object_table *, void (*)(struct g_object *parent, struct g_object *child));
	int x, y, xw, yw;
	struct g_object *parent;
	/* private data */
	struct table *t;
};

struct g_object_tag {
	/* public data --- must be same in all g_object* structures */
	void (*mouse_event)(struct f_data_c *, struct g_object *, int, int, int);
		/* pos is relative to object */
	void (*draw)(struct f_data_c *, struct g_object *, int, int);
		/* absolute pos on screen */
	void (*destruct)(struct g_object *);
	void (*get_list)(struct g_object *, void (*)(struct g_object *parent, struct g_object *child));
	int x, y, xw, yw;
	struct g_object *parent;
	unsigned char name[1];
	/* private data... */
};

#define IM_PNG 0
#define IM_GIF 1
#define IM_XBM 2

#ifdef HAVE_JPEG
#define IM_JPG 3
#endif /* #ifdef HAVE_JPEG */

#ifdef HAVE_TIFF
#define IM_TIFF 4
#endif /* #ifdef HAVE_TIFF */

#ifdef HAVE_SVG
#define IM_SVG 5
#endif /* #ifdef HAVE_SVG */

#define MEANING_DIMS	  0
#define MEANING_AUTOSCALE 1
struct cached_image {
	struct cached_image *next;
	struct cached_image *prev;
	int refcount;

	int background_color; /* nezaokrouhlene pozadi:
			       * sRGB, (r<<16)+(g<<8)+b */
	unsigned char *url;
	int wanted_xw, wanted_yw; /* This is what is written in the alt.
				     If some dimension is omitted, then
				     it's <0. This is what was requested
				     when the image was created. */
	int wanted_xyw_meaning; /* MEANING_DIMS or MEANING_AUTOSCALE.
				   The meaning of wanted_xw and wanted_yw. */
	int scale; /* How is the image scaled */
	unsigned aspect; /* What aspect ratio the image is for. But the
		       PNG aspect is ignored :( */

	int xww, yww; /* This is the resulting dimensions on the screen
			 measured in screen pixels. */

	int width, height; /* From image header.
			    * If the buffer is allocated,
			    * it is always allocated to width*height.
			    * If the buffer is NULL then width and height
			    * are garbage. We assume these dimensions
			    * are given in the meter space (not pixel space).
			    * Which is true for all images except aspect
			    * PNG, but we don't support aspect PNG yet.
			    */
	unsigned char image_type; /* IM_??? constant */
	unsigned char *buffer; /* Buffer with image data */
	unsigned char buffer_bytes_per_pixel; /* 3 or 4 or 6 or 8
				     * 3: RGB
				     * 4: RGBA
				     * 6: RRGGBB
				     * 8: RRGGBBAA
				     */
	float red_gamma, green_gamma, blue_gamma;
		/* data=light_from_monitor^[red|green|blue]_gamma.
		 * i. e. 0.45455 is here if the image is in sRGB
		 * makes sense only if buffer is !=NULL
		 */
	tcount gamma_stamp; /* Number that is increased every gamma change */
	struct bitmap bmp; /* Registered bitmap. bmp.x=-1 and bmp.y=-1
			    * if the bmp is not registered.
			    */
	unsigned char bmp_used;
	int last_length; /* length of cache entry at which last decoding was
			  * done. Makes sense only if reparse==0
			  */
	tcount last_count; /* Always valid. */
	tcount last_count2; /* Always valid. */
	void *decoder;	      /* Decoder unfinished work. If NULL, decoder
			       * has finished or has not yet started.
			       */
	int rows_added; /* 1 if some rows were added inside the decoder */
	unsigned char state; /* 0...3 or 8...15 */
	unsigned char strip_optimized; /* 0 no strip optimization
				1 strip-optimized (no buffer allocated permanently
				and bitmap is always allocated)
			      */
	unsigned char eof_hit;
	int *dregs; /* Only for stip-optimized cached images */
	unsigned short *gamma_table; /* When suitable and source is 8 bits per pixel,
				      * this is allocated to 256*3*sizeof(*gamma_table)
				      * = 1536 bytes and speeds up the gamma calculations
				      * tremendously */
};

struct additional_file;

struct g_object_image {
	/* public data --- must be same in all g_object* structures */
	void (*mouse_event)(struct f_data_c *, struct g_object_text *, int, int, int);
	void (*draw)(struct f_data_c *, struct g_object_image *, int, int);
	void (*destruct)(struct g_object *);
	void (*get_list)(struct g_object *, void (*)(struct g_object *parent, struct g_object *child));
	int x, y, xw, yw; /* x,y: coordinates
			     xw, yw: width on the screen, or <0 if
			     not yet known. Already scaled. */
	/* For html parser. If xw or yw are zero, then entries
	       background_color
	       af
	       width
	       height
	       image_type
	       buffer
	       buffer_bytes_per_pixel
	       *_gamma
	       gamma_stamp
	       bmp
	       last_length
	       last_count2
	       decoder
	       rows_added
	       reparse
	are uninitialized and thus garbage
	*/

	struct g_object *parent;
	/* must be same in g_object_text */
	int link_num;
	int link_order;
	struct image_map *map;
	int ismap;
	/* End of compatibility with g_object_text */

	struct xlist_head image_list;

	struct cached_image *cimg;
	struct additional_file *af;

	long id;
	unsigned char *name;
	unsigned char *alt;
	int vspace, hspace, border;
	unsigned char *orig_src;
	unsigned char *src;
	int background; /* Remembered background from insert_image
			 * (g_part->root->bg->u.sRGB)
			 */
	int xyw_meaning;
};

void refresh_image(struct f_data_c *fd, struct g_object *img, ttime tm);

#endif

struct additional_file *request_additional_file(struct f_data *f, unsigned char *url);

struct js_event_spec {
#ifdef JS
	unsigned char *move_code;
	unsigned char *over_code;
	unsigned char *out_code;
	unsigned char *down_code;
	unsigned char *up_code;
	unsigned char *click_code;
	unsigned char *dbl_code;
	unsigned char *blur_code;
	unsigned char *focus_code;
	unsigned char *change_code;
	unsigned char *keypress_code;
	unsigned char *keyup_code;
	unsigned char *keydown_code;
#else
	char dummy;
#endif
};

/*
 * warning: if you add more additional file stuctures, you must
 * set RQ upcalls correctly
 */

struct additional_files {
	int refcount;
	struct list_head af;	/* struct additional_file */
};

struct additional_file {
	struct additional_file *next;
	struct additional_file *prev;
	struct object_request *rq;
	tcount use_tag;
	tcount use_tag2;
	int need_reparse;
	int unknown_image_size;
	unsigned char url[1];
};

#ifdef G
struct image_refresh {
	struct image_refresh *next;
	struct image_refresh *prev;
	struct g_object *img;
	uttime tim;
	uttime start;
};
#endif

struct f_data {
	struct f_data *next;
	struct f_data *prev;
	struct session *ses;
	struct f_data_c *fd;
	struct object_request *rq;
	tcount use_tag;
	struct additional_files *af;
	struct document_options opt;
	unsigned char *title;
	int cp, ass;
	int x, y; /* size of document */
	ttime time_to_get;
	ttime time_to_draw;
	struct frameset_desc *frame_desc;
	int frame_desc_link;	/* if != 0, do not free frame_desc because it is link */

	/* text only */
	int bg;
	struct line *data;
	struct link *links;
	int nlinks;
#ifdef JS
	struct js_event_spec **link_events;
	int nlink_events;
#endif
	struct link **lines1;
	struct link **lines2;
	struct list_head nodes;		/* struct node */
	struct search *search_pos;
	char_t *search_chr;
	int nsearch_chr;
	int nsearch_pos;
	int *slines1;
	int *slines2;

	struct list_head forms;		/* struct form_control */
	struct list_head tags;		/* struct tag */

#ifdef JS
	int are_there_scripts;
	unsigned char *script_href_base;
#endif

	unsigned char *refresh;
	int refresh_seconds;

	int uncacheable;	/* cannot be cached - either created from source modified by document.write or modified by javascript */

#ifdef JS
	struct js_event_spec *js_event;
#endif

	/* graphics only */
#ifdef G
	struct g_object *root;
	struct g_object *locked_on;

	unsigned char *srch_string;
	int srch_string_size;

	unsigned char *last_search;
	int *search_positions;
	int *search_lengths;
	int n_search_positions;
	int hlt_pos; /* index of first highlighted byte */
	int hlt_len; /* length of highlighted bytes; (hlt_pos+hlt_len) is index of last highlighted character */
	int start_highlight_x;
	int start_highlight_y;
	struct list_head images;	/* list of all images in this f_data */
	int n_images;	/* pocet obrazku (tim se obrazky taky identifikujou), po kazdem pridani obrazku se zvedne o 1 */

	struct list_head image_refresh;
#endif
};

struct view_state {
	int refcount;

	int view_pos;
	int view_posx;
	int orig_view_pos;
	int orig_view_posx;
	int current_link;	/* platny jen kdyz je <f_data->n_links */
	int orig_link;
	int frame_pos;
	int plain;
	struct form_state *form_info;
	int form_info_len;
	int brl_x;
	int brl_y;
	int orig_brl_x;
	int orig_brl_y;
	int brl_in_field;
#ifdef G
	int g_display_link;
#endif
};

struct location;

struct f_data_c {
	struct f_data_c *next;
	struct f_data_c *prev;
	struct f_data_c *parent;
	struct session *ses;
	struct location *loc;
	struct view_state *vs;
	struct f_data *f_data;
	int xw, yw; /* size of window */
	int xp, yp; /* pos of window on screen */
	int xl, yl; /* last pos of view in window */

	int hsb, vsb;
	int hsbsize, vsbsize;

	struct link_bg *link_bg;
	int link_bg_n;
	int depth;

	struct object_request *rq;
	unsigned char *goto_position;
	unsigned char *went_to_position;
	struct additional_files *af;

	struct list_head subframes;	/* struct f_data_c */

	ttime next_update;
	int done;
	int parsed_done;
	int script_t;	/* offset of next script to execute */

	int active;	/* temporary, for draw_doc */

	long id;	/* unique document identification for javascript */

	int marginwidth, marginheight;

#ifdef JS
	struct js_state *js;
#endif

	struct timer *image_timer;

	struct timer *refresh_timer;

#ifdef JS
	unsigned char *onload_frameset_code;
#endif
	unsigned char scrolling;
};

struct location {
	struct location *next;
	struct location *prev;
	struct location *parent;
	unsigned char *name;	/* frame name */
	unsigned char *url;
	unsigned char *prev_url;   /* allocated string with referrer */
	struct list_head subframes;	/* struct location */
	struct view_state *vs;
	unsigned location_id;
};

#define WTD_NO		0
#define WTD_FORWARD	1
#define WTD_IMGMAP	2
#define WTD_RELOAD	3
#define WTD_BACK	4

#define cur_loc(x) ((struct location *)((x)->history.next))

struct kbdprefix {
	int rep;
	int rep_num;
	int prefix;
};

struct download {
	struct download *next;
	struct download *prev;
	unsigned char *url;
	struct status stat;
	unsigned char decompress;
	unsigned char *cwd;
	unsigned char *orig_file;
	unsigned char *file;
	off_t last_pos;
	off_t file_shift;
	int handle;
	int redirect_cnt;
	int downloaded_something;
	unsigned char *prog;
	int prog_flag_block;
	time_t remotetime;
	struct session *ses;
	struct window *win;
	struct window *ask;
};

extern struct list_head downloads;

struct session {
	struct session *next;
	struct session *prev;
	struct list_head history;	/* struct location */
	struct list_head forward_history;
	struct terminal *term;
	struct window *win;
	int id;
	unsigned char *st;		/* status line string */
	unsigned char *st_old;		/* old status line --- compared with st to prevent cursor flicker */
	unsigned char *default_status;	/* default value of the status line */
	struct f_data_c *screen;
	struct object_request *rq;
	void (*wtd)(struct session *);
	unsigned char *wtd_target;
	struct f_data_c *wtd_target_base;
	unsigned char *wanted_framename;
	int wtd_refresh;
	int wtd_num_steps;
	unsigned char *goto_position;
	struct document_setup ds;
	struct kbdprefix kbdprefix;
	int reloadlevel;
	struct object_request *tq;
	unsigned char *tq_prog;
	int tq_prog_flag_block;
	int tq_prog_flag_direct;
	unsigned char *dn_url;
	int dn_allow_flags;
	unsigned char *search_word;
	unsigned char *last_search_word;
	int search_direction;
	int exit_query;
	struct list_head format_cache;	/* struct f_data */

	unsigned char *imgmap_href_base;
	unsigned char *imgmap_target_base;

#ifdef JS
	unsigned char *defered_url;
	unsigned char *defered_target;
	struct f_data_c *defered_target_base;
	int defered_data;	/* for submit: form number, jinak -1 */
	tcount defered_seq;
#endif

	int brl_cursor_mode;

#ifdef G
	int locked_link;	/* for graphics - when link is locked on FIELD/AREA */
	int scrolling;
	int scrolltype;
	int scrolloff;

	int back_size;
#endif
};

struct dialog_data;

int get_file(struct object_request *o, unsigned char **start, unsigned char **end);

int f_is_finished(struct f_data *f);
unsigned long formatted_info(int);
void init_fcache(void);
void html_interpret_recursive(struct f_data_c *);
void fd_loaded(struct object_request *, struct f_data_c *);

extern struct list_head sessions;

time_t parse_http_date(unsigned char *);
unsigned char *encode_url(unsigned char *);
unsigned char *decode_url(unsigned char *);
unsigned char *subst_file(unsigned char *, unsigned char *, int);
int are_there_downloads(void);
unsigned char *translate_download_file(unsigned char *);
void free_strerror_buf(void);
int get_error_from_errno(int errn);
unsigned char *get_err_msg(int);
void print_screen_status(struct session *);
void change_screen_status(struct session *);
void print_error_dialog(struct session *, struct status *, unsigned char *);
void start_download(struct session *, unsigned char *, int);
int test_abort_downloads_to_file(unsigned char *, unsigned char *, int);
void abort_all_downloads(void);
unsigned char *download_percentage(struct download *down, int pad);
void download_window_function(struct dialog_data *dlg);
void display_download(struct terminal *, struct download *, struct session *);
struct f_data *cached_format_html(struct f_data_c *fd, struct object_request *rq, unsigned char *url, struct document_options *opt, int *cch);
struct f_data_c *create_f_data_c(struct session *, struct f_data_c *);
void reinit_f_data_c(struct f_data_c *);
int f_data_c_allow_flags(struct f_data_c *fd);
#define CDF_RESTRICT_PERMISSION		1
#define CDF_EXCL			2
#define CDF_NOTRUNC			4
#define CDF_NO_POPUP_ON_EEXIST		8
int create_download_file(struct session *, unsigned char *, unsigned char *, int, off_t);
void *create_session_info(int, unsigned char *, unsigned char *, int *);
void win_func(struct window *, struct links_event *, int);
void goto_url_f(struct session *, void (*)(struct session *), unsigned char *, unsigned char *, struct f_data_c *, int, int, int, int);
void goto_url(struct session *, unsigned char *);
void goto_url_not_from_dialog(struct session *, unsigned char *, struct f_data_c *);
void goto_imgmap(struct session *ses, struct f_data_c *fd, unsigned char *url, unsigned char *href, unsigned char *target);
void map_selected(struct terminal *term, struct link_def *ld, struct session *ses);
void go_back(struct session *, int);
void go_backwards(struct terminal *term, void *psteps, struct session *ses);
void reload(struct session *, int);
void destroy_session(struct session *);
void ses_destroy_defered_jump(struct session *ses);
struct f_data_c *find_frame(struct session *ses, unsigned char *target, struct f_data_c *base);


/* Information about the current document */
unsigned char *get_current_url(struct session *, unsigned char *, size_t);
unsigned char *get_current_title(struct session *, unsigned char *, size_t);

/*unsigned char *get_current_link_url(struct session *, unsigned char *, size_t);*/
unsigned char *get_form_url(struct session *ses, struct f_data_c *f, struct form_control *form, int *onsubmit);

/* js.c */

#ifdef JS
struct javascript_context *js_create_context(void *, long);
void js_destroy_context(struct javascript_context *);
void js_execute_code(struct javascript_context *, unsigned char *, int, void (*)(void *));
#endif

/* jsint.c */

#ifdef JS

#define JS_OBJ_MASK 255
#define JS_OBJ_MASK_SIZE 8

#define JS_OBJ_T_UNKNOWN 0
#define JS_OBJ_T_DOCUMENT 1
#define JS_OBJ_T_FRAME 2	/* document a frame se tvari pro mne stejne  --Brain */
#define JS_OBJ_T_LINK 3
#define JS_OBJ_T_FORM 4
#define JS_OBJ_T_ANCHOR 5
#define JS_OBJ_T_IMAGE 6
/* form elements */
#define JS_OBJ_T_TEXT 7
#define JS_OBJ_T_PASSWORD 8
#define JS_OBJ_T_TEXTAREA 9
#define JS_OBJ_T_CHECKBOX 10
#define JS_OBJ_T_RADIO 11
#define JS_OBJ_T_SELECT 12
#define JS_OBJ_T_SUBMIT 13
#define JS_OBJ_T_RESET 14
#define JS_OBJ_T_HIDDEN 15
#define JS_OBJ_T_BUTTON 16

extern long js_zaflaknuto_pameti;

extern struct history js_get_string_history;
extern int js_manual_confirmation;

struct js_state {
	struct javascript_context *ctx;	/* kontext beziciho javascriptu??? */
	struct list_head queue;		/* struct js_request - list of javascripts to run */
	struct js_request *active;	/* request is running */
	unsigned char *src;		/* zdrojak beziciho javascriptu??? */	/* mikulas: ne. to je zdrojak stranky */
	int srclen;
	int wrote;
	int newdata;
};


/* funkce js_get_select_options vraci pole s temito polozkami */
struct js_select_item{
	/* index je poradi v poli, ktere vratim, takze se tu nemusi skladovat */
	int default_selected;
	int selected;
	unsigned char *text;	/* text, ktery se zobrazuje */
	unsigned char *value;	/* value, ktera se posila */
};

struct fax_me_tender_string{
	void *ident;   /* struct f_data_c*, but JS doesn't know it ;-) */
	unsigned char *string;
};

struct fax_me_tender_int_string{
	void *ident;   /* struct f_data_c*, but JS doesn't know it ;-) */
	int num;
	unsigned char *string;
};

struct fax_me_tender_string_2_longy{
	void *ident;   /* struct f_data_c*, but JS doesn't know it ;-) */
	unsigned char *string;
	long doc_id,obj_id;
};

struct fax_me_tender_2_stringy{
	void *ident;   /* struct f_data_c*, but JS doesn't know it ;-) */
	unsigned char *string1;
	unsigned char *string2;
};

struct fax_me_tender_nothing{
	void *ident;   /* struct f_data_c*, but JS doesn't know it ;-) */
};

extern tcount jsint_execute_seq;

void javascript_func(struct session *ses, unsigned char *code);
void jsint_run_queue(struct f_data_c *);
long *jsint_resolve(void *context, long obj_id, char *takhle_tomu_u_nas_nadavame,int *n_items);
int jsint_object_type(long);
void jsint_set_cookies(struct f_data_c *fd, int final_flush);
struct f_data_c *jsint_find_document(long doc_id);

void js_upcall_document_write(void *p, unsigned char *str, int len);
void js_upcall_alert(void *struct_fax_me_tender_string);
unsigned char *js_upcall_get_title(void *data);
void js_upcall_set_title(void *data, unsigned char *title);
unsigned char *js_upcall_get_location(void *data);
unsigned char *js_upcall_get_useragent(void *data);
void js_upcall_confirm(void *struct_fax_me_tender_string);
void js_upcall_get_string(void *data);
unsigned char *js_upcall_get_referrer(void *data);
unsigned char *js_upcall_get_appname(void);
unsigned char *js_upcall_get_appcodename(void);
unsigned char *js_upcall_get_appversion(void);
long js_upcall_get_document_id(void *data);
long js_upcall_get_window_id(void *data);
void js_upcall_close_window(void *struct_fax_me_tender_nothing);
unsigned char *js_upcall_document_last_modified(void *data, long document_id);
unsigned char *js_upcall_get_window_name(void *data);
void js_upcall_clear_window(void *);
long *js_upcall_get_links(void *data, long document_id, int *len);
unsigned char *js_upcall_get_link_target(void *data, long document_id, long link_id);
long *js_upcall_get_forms(void *data, long document_id, int *len);
unsigned char *js_upcall_get_form_action(void *data, long document_id, long form_id);
unsigned char *js_upcall_get_form_target(void *data, long document_id, long form_id);
unsigned char *js_upcall_get_form_method(void *data, long document_id, long form_id);
unsigned char *js_upcall_get_form_encoding(void *data, long document_id, long form_id);
unsigned char *js_upcall_get_location_protocol(void *data);
unsigned char *js_upcall_get_location_port(void *data);
unsigned char *js_upcall_get_location_hostname(void *data);
unsigned char *js_upcall_get_location_host(void *data);
unsigned char *js_upcall_get_location_pathname(void *data);
unsigned char *js_upcall_get_location_search(void *data);
unsigned char *js_upcall_get_location_hash(void *data);
long *js_upcall_get_form_elements(void *data, long document_id, long form_id, int *len);
long *js_upcall_get_anchors(void *hej_Hombre, long document_id, int *len);
int js_upcall_get_checkbox_radio_checked(void *smirak, long document_id, long radio_tv_id);
void js_upcall_set_checkbox_radio_checked(void *smirak, long document_id, long radio_tv_id, int value);
int js_upcall_get_checkbox_radio_default_checked(void *smirak, long document_id, long radio_tv_id);
void js_upcall_set_checkbox_radio_default_checked(void *smirak, long document_id, long radio_tv_id, int value);
unsigned char *js_upcall_get_form_element_name(void *smirak, long document_id, long ksunt_id);
void js_upcall_set_form_element_name(void *smirak, long document_id, long ksunt_id, unsigned char *name);
unsigned char *js_upcall_get_form_element_default_value(void *smirak, long document_id, long ksunt_id);
void js_upcall_set_form_element_default_value(void *smirak, long document_id, long ksunt_id, unsigned char *name);
void js_upcall_set_form_element_event_handler(void *bidak, long document_id, long ksunt_id, long typ, unsigned char *name);
unsigned char *js_upcall_get_form_element_value(void *smirak, long document_id, long ksunt_id);
void js_upcall_set_form_element_value(void *smirak, long document_id, long ksunt_id, unsigned char *name);
void js_upcall_click(void *smirak, long document_id, long elem_id);
void js_upcall_focus(void *smirak, long document_id, long elem_id);
void js_upcall_blur(void *smirak, long document_id, long elem_id);
void js_upcall_submit(void *bidak, long document_id, long form_id);
void js_upcall_reset(void *bidak, long document_id, long form_id);
int js_upcall_get_radio_length(void *smirak, long document_id, long radio_id); /* radio.length */
int js_upcall_get_select_length(void *smirak, long document_id, long select_id); /* select.length */
int js_upcall_get_select_index(void *smirak, long document_id, long select_id); /* select.selectedIndex */
struct js_select_item* js_upcall_get_select_options(void *smirak, long document_id, long select_id, int *n);
void js_upcall_goto_url(void* struct_fax_me_tender_string);
int js_upcall_get_history_length(void *context);
void js_upcall_goto_history(void* data);
void js_upcall_set_default_status(void *context, unsigned char *tak_se_ukaz_Kolbene);
unsigned char *js_upcall_get_default_status(void *context);
void js_upcall_set_status(void *context, unsigned char *tak_se_ukaz_Kolbene);
unsigned char *js_upcall_get_status(void *context);
unsigned char *js_upcall_get_cookies(void *context);
long *js_upcall_get_images(void *smirak, long document_id, int *len);
long * js_upcall_get_all(void *context, long document_id, int *len);
int js_upcall_get_image_width(void *smirak, long document_id, long image_id);
int js_upcall_get_image_height(void *smirak, long document_id, long image_id);
int js_upcall_get_image_border(void *smirak, long document_id, long image_id);
int js_upcall_get_image_vspace(void *smirak, long document_id, long image_id);
int js_upcall_get_image_hspace(void *smirak, long document_id, long image_id);
unsigned char *js_upcall_get_image_name(void *smirak, long document_id, long image_id);
unsigned char *js_upcall_get_image_alt(void *smirak, long document_id, long image_id);
void js_upcall_set_image_name(void *smirak, long document_id, long image_id, unsigned char *name);
void js_upcall_set_image_alt(void *smirak, long document_id, long image_id, unsigned char *alt);
unsigned char *js_upcall_get_image_src(void *smirak, long document_id, long image_id);
void js_upcall_set_image_src(void *chuligane);
int js_upcall_image_complete(void *smirak, long document_id, long image_id);
long js_upcall_get_parent(void *smirak, long frame_id);
long js_upcall_get_frame_top(void *smirak, long frame_id);
long * js_upcall_get_subframes(void *smirak, long frame_id, int *count);
void js_upcall_set_form_action(void *context, long document_id, long form_id, unsigned char *action);


void js_downcall_vezmi_true(void *context);
void js_downcall_vezmi_false(void *context);
void js_downcall_vezmi_null(void *context);
void js_downcall_game_over(void *context);
void js_downcall_quiet_game_over(void *context);
void js_downcall_vezmi_int(void *context, int i);
void js_downcall_vezmi_float(void*context,double f);
/*void js_downcall_vezmi_float(void *context, float f);*/
void js_downcall_vezmi_string(void *context, unsigned char *string);

#endif

void jsint_execute_code(struct f_data_c *, unsigned char *, int, int, int, int, struct links_event *);
void jsint_destroy(struct f_data_c *);
void jsint_scan_script_tags(struct f_data_c *);
int jsint_get_source(struct f_data_c *, unsigned char **, unsigned char **);

/* bfu.c */

extern struct style *bfu_style_wb, *bfu_style_bw, *bfu_style_wb_b, *bfu_style_bw_u, *bfu_style_bw_mono, *bfu_style_wb_mono, *bfu_style_wb_mono_u;
extern long bfu_bg_color, bfu_fg_color;

struct memory_list {
	int n;
	void *p[1];
};

struct memory_list *getml(void *, ...);
void add_to_ml(struct memory_list **, ...);
void freeml(struct memory_list *);

void iinit_bfu(void);
void init_bfu(void);
void shutdown_bfu(void);

#define DIALOG_LB	gf_val(DIALOG_LEFT_BORDER + DIALOG_LEFT_INNER_BORDER + 1, G_DIALOG_LEFT_BORDER + G_DIALOG_VLINE_SPACE + 1 + G_DIALOG_LEFT_INNER_BORDER)
#define DIALOG_TB	gf_val(DIALOG_TOP_BORDER + DIALOG_TOP_INNER_BORDER + 1, G_DIALOG_TOP_BORDER + G_DIALOG_HLINE_SPACE + 1 + G_DIALOG_TOP_INNER_BORDER)

#define MENU_FUNC (void (*)(struct terminal *, void *, void *))

extern unsigned char m_bar;

#define M_BAR	(&m_bar)

struct menu_item {
	unsigned char *text;
	unsigned char *rtext;
	unsigned char *hotkey;
	void (*func)(struct terminal *, void *, void *);
	void *data;
	int in_m;
	int free_i;
};

struct menu {
	int selected;
	int view;
	int nview;
	int xp, yp;
	int x, y, xw, yw;
	int ni;
	void *data;
	struct window *win;
	struct menu_item *items;
#ifdef G
	unsigned char **hktxt1;
	unsigned char **hktxt2;
	unsigned char **hktxt3;
	int xl1, yl1, xl2, yl2;
#endif
	void (*free_function)(void *);
	void *free_data;
	unsigned hotkeys[1];
};

struct mainmenu {
	int selected;
	int sp;
	int ni;
	void *data;
	struct window *win;
	struct menu_item *items;
#ifdef G
	int xl1, yl1, xl2, yl2;
#endif
	unsigned hotkeys[1];
};

struct history_item {
	struct history_item *next;
	struct history_item *prev;
	unsigned char d[1];
};

struct history {
	int n;
	struct list_head items;
};

#define D_END		0
#define D_CHECKBOX	1
#define D_FIELD		2
#define D_FIELD_PASS	3
#define D_BUTTON	4

#define B_ENTER		1
#define B_ESC		2

struct dialog_item_data;

struct dialog_item {
	int type;
	int gid, gnum; /* for buttons: gid - flags B_XXX */	/* for fields: min/max */ /* for box: gid is box height */
	int (*fn)(struct dialog_data *, struct dialog_item_data *);
	struct history *history;
	int dlen;
	unsigned char *data;
	void *udata; /* for box: holds list */
	unsigned char *text;
};

struct dialog_item_data {
	int x, y, l;
	int vpos, cpos;
	int checked;
	struct dialog_item *item;
	struct list_head history;
	struct history_item *cur_hist;
	unsigned char *cdata;
};

#define	EVENT_PROCESSED		0
#define EVENT_NOT_PROCESSED	1

struct dialog {
	unsigned char *title;
	void (*fn)(struct dialog_data *);
	int (*handle_event)(struct dialog_data *, struct links_event *);
	void (*abort)(struct dialog_data *);
	void *udata;
	void *udata2;
	int align;
	void (*refresh)(void *);
	void *refresh_data;
	struct dialog_item items[1];
};

struct dialog_data {
	struct window *win;
	struct dialog *dlg;
	int x, y, xw, yw;
	int n;
	int selected;
	struct memory_list *ml;
	int brl_y;
#ifdef G
	struct rect_set *s;
	struct rect r;
	struct rect rr;
#endif
	struct dialog_item_data items[1];
};

struct menu_item *new_menu(int);
void add_to_menu(struct menu_item **, unsigned char *, unsigned char *, unsigned char *, void (*)(struct terminal *, void *, void *), void *, int, int);
void do_menu(struct terminal *, struct menu_item *, void *);
void do_menu_selected(struct terminal *, struct menu_item *, void *, int, void (*)(void *), void *);
void do_mainmenu(struct terminal *, struct menu_item *, void *, int);
void do_dialog(struct terminal *, struct dialog *, struct memory_list *);
int check_number(struct dialog_data *, struct dialog_item_data *);
int check_hex_number(struct dialog_data *, struct dialog_item_data *);
int check_float(struct dialog_data *, struct dialog_item_data *);
int check_nonempty(struct dialog_data *, struct dialog_item_data *);
int check_local_ip_address(struct dialog_data *, struct dialog_item_data *);
int check_local_ipv6_address(struct dialog_data *, struct dialog_item_data *);
void max_text_width(struct terminal *, unsigned char *, int *, int);
void min_text_width(struct terminal *, unsigned char *, int *, int);
int dlg_format_text(struct dialog_data *, struct terminal *, unsigned char *, int, int *, int, int *, unsigned char, int);
void dlg_format_text_and_field(struct dialog_data *, struct terminal *, unsigned char *, struct dialog_item_data *, int, int *, int, int *, unsigned char, int);
void max_buttons_width(struct terminal *, struct dialog_item_data *, int, int *);
void min_buttons_width(struct terminal *, struct dialog_item_data *, int, int *);
void dlg_format_buttons(struct dialog_data *, struct terminal *, struct dialog_item_data *, int, int, int *, int, int *, int);
void checkboxes_width(struct terminal *, unsigned char * const *, int, int *, void (*)(struct terminal *, unsigned char *, int *, int));
void dlg_format_checkbox(struct dialog_data *, struct terminal *, struct dialog_item_data *, int, int *, int, int *, unsigned char *);
void dlg_format_checkboxes(struct dialog_data *, struct terminal *, struct dialog_item_data *, int, int, int *, int, int *, unsigned char * const *);
void dlg_format_field(struct dialog_data *, struct terminal *, struct dialog_item_data *, int, int *, int, int *, int);
void max_group_width(struct terminal *, unsigned char * const *, struct dialog_item_data *, int, int *);
void min_group_width(struct terminal *, unsigned char * const *, struct dialog_item_data *, int, int *);
void dlg_format_group(struct dialog_data *, struct terminal *, unsigned char * const *, struct dialog_item_data *, int, int, int *, int, int *);
/*void dlg_format_box(struct terminal *, struct terminal *, struct dialog_item_data *, int, int *, int, int *, int);*/
void checkbox_list_fn(struct dialog_data *);
void group_fn(struct dialog_data *);
void center_dlg(struct dialog_data *);
void draw_dlg(struct dialog_data *);
void display_dlg_item(struct dialog_data *, struct dialog_item_data *, int);
int check_dialog(struct dialog_data *);
void get_dialog_data(struct dialog_data *);
int ok_dialog(struct dialog_data *, struct dialog_item_data *);
int cancel_dialog(struct dialog_data *, struct dialog_item_data *);
void msg_box_fn(struct dialog_data *dlg);
void msg_box(struct terminal *, struct memory_list *, unsigned char *, int, /*unsigned char *, void *, int,*/ ...);
/* msg_box arguments:
 *		terminal,
 *		blocks to free,
 *		title,
 *		alignment (and/or optional AL_EXTD_TEXT),
 *		string (or optional several strings followed with NULL),
 *		data for function,
 *		number of buttons,
 *		button title, function, hotkey,
 *		... other buttons
 */
void input_field_fn(struct dialog_data *);
void input_field(struct terminal *, struct memory_list *, unsigned char *, unsigned char *, void *, struct history *, int, unsigned char *, int, int, int (*)(struct dialog_data *, struct dialog_item_data *), ...);
/* input_field arguments:
 *		terminal,
 *		blocks to free,
 *		title,
 *		question,
 *		data for functions,
 *		history,
 *		length,
 *		string to fill the dialog with,
 *		minimal value,
 *		maximal value,
 *		check_function,
 *		OK button text,
 *		ok function,
 *		CANCEL button text,
 *		cancel function,
 *		NULL
 *
 *	field can have multiple buttons and functions, and finally NULL
 *	(warning: if there's no cancel function, there will be two NULLs in
 *	a call). Functions have type
 *	void (*fn)(void *data, unsigned char *text), only the last one has type
 *	void (*fn)(void *data). Check it carefully because the compiler wont!
 */
void add_to_history(struct history *, unsigned char *, int);

void dialog_func(struct window *, struct links_event *, int);

/* menu.c */

extern struct history goto_url_history;

void activate_keys(struct session *ses);
void activate_bfu_technology(struct session *, int);
void dialog_goto_url(struct session *ses, unsigned char *url);
void dialog_save_url(struct session *ses);
void free_history_lists(void);
void query_file(struct session *, unsigned char *, unsigned char *, void (*)(struct session *, unsigned char *, int), void (*)(struct session *), int);
#define DOWNLOAD_DEFAULT	0
#define DOWNLOAD_OVERWRITE	1
#define DOWNLOAD_CONTINUE	2
void search_dlg(struct session *, struct f_data_c *, int);
void search_back_dlg(struct session *, struct f_data_c *, int);
void exit_prog(struct terminal *, void *, struct session *);
void really_exit_prog(struct session *ses);
void query_exit(struct session *ses);

#ifdef G
extern tcount gamma_stamp;
#endif

/* charsets.c */

#include "codepage.h"

extern int utf8_table;

struct conv_table {
	int t;
	union {
		unsigned char *str;
		struct conv_table *tbl;
	} u;
};

struct conv_table *get_translation_table(int, int);
int get_entity_number(unsigned char *st, int l);
unsigned char *get_entity_string(unsigned char *, int, int);
unsigned char *convert_string(struct conv_table *, unsigned char *, int, struct document_options *);
int get_cp_index(unsigned char *);
unsigned char *get_cp_name(int);
unsigned char *get_cp_mime_name(int);
void free_conv_table(void);
unsigned char *encode_utf_8(int);
unsigned char *u2cp(int u, int to, int fallback);
int cp2u(unsigned, int);

unsigned charset_upcase(unsigned, int);
unsigned uni_upcase(unsigned);
void charset_upcase_string(unsigned char **, int);
unsigned char *unicode_upcase_string(unsigned char *ch);
unsigned char *to_utf8_upcase(unsigned char *str, int cp);
int compare_case_utf8(unsigned char *u1, unsigned char *u2);
int strlen_utf8(unsigned char *s);
unsigned char *cp_strchr(int charset, unsigned char *str, unsigned chr);
void init_charset(void);

unsigned get_utf_8(unsigned char **p);
extern unsigned short int utf8_2_uni_table[0x200];
#define GET_UTF_8(s, c)							\
do {									\
	if ((unsigned char)(s)[0] < 0x80)				\
		(c) = (s)++[0];						\
	else if (((c) = utf8_2_uni_table[((unsigned char)(s)[0] << 2) +	\
				((unsigned char)(s)[1] >> 6) - 0x200]))	\
		(c) += (unsigned char)(s)[1] & 0x3f, (s) += 2;		\
	else								\
		(c) = get_utf_8(&(s));					\
} while (0)
#define FWD_UTF_8(s)							\
do {									\
	if ((unsigned char)(s)[0] < 0x80)				\
		(s)++;							\
	else								\
		get_utf_8(&(s));					\
} while (0)
#define BACK_UTF_8(p, b)						\
do {									\
	while ((p) > (b)) {						\
		(p)--;							\
		if ((*(p) & 0xc0) != 0x80)				\
			break;						\
	}								\
} while (0)

int cp_len(int cp, unsigned char *s);

extern unsigned char utf_8_1[256];

static inline int utf8chrlen(unsigned char c)
{
	unsigned char l = utf_8_1[c];
	if (l == 7) return 1;
	return 7 - l;
}

static inline unsigned GET_TERM_CHAR(struct terminal *term, unsigned char **str)
{
	unsigned ch;
#if defined(G) || defined(ENABLE_UTF8)
	if (term_charset(term) == utf8_table)
		GET_UTF_8(*str, ch);
	else
#endif
		ch = *(*str)++;
	return ch;
}

/* view.c */

unsigned char *textptr_add(unsigned char *t, int i, int cp);
int textptr_diff(unsigned char *t2, unsigned char *t1, int cp);

extern int ismap_link, ismap_x, ismap_y;

struct line_info {
	unsigned char *st;
	unsigned char *en;
};

struct line_info *format_text(unsigned char *text, int width, int wrap, int cp);

void frm_download(struct session *, struct f_data_c *);
void frm_download_image(struct session *, struct f_data_c *);
void frm_view_image(struct session *, struct f_data_c *);
struct form_state *find_form_state(struct f_data_c *, struct form_control *);
void fixup_select_state(struct form_control *fc, struct form_state *fs);
int enter(struct session *ses, struct f_data_c *f, int a);
int field_op(struct session *ses, struct f_data_c *f, struct link *l, struct links_event *ev, int rep);
int area_cursor(struct f_data_c *f, struct form_control *form, struct form_state *fs);

int can_open_in_new(struct terminal *);
void open_in_new_window(struct terminal *, void (*)(struct terminal *, int (*)(struct terminal *, unsigned char *, unsigned char *), struct session *ses), struct session *);
void send_open_new_xterm(struct terminal *, int (*)(struct terminal *, unsigned char *, unsigned char *), struct session *);
void destroy_fc(struct form_control *);
void sort_links(struct f_data *);
struct view_state *create_vs(void);
void destroy_vs(struct view_state *);
int dump_to_file(struct f_data *, int);
void check_vs(struct f_data_c *);
void draw_doc(struct terminal *t, struct f_data_c *scr);
void draw_formatted(struct session *);
void draw_fd(struct f_data_c *);
void next_frame(struct session *, int);
void send_event(struct session *, struct links_event *);
void link_menu(struct terminal *, void *, struct session *);
void save_as(struct terminal *, void *, struct session *);
void save_url(struct session *, unsigned char *);
void menu_save_formatted(struct terminal *, void *, struct session *);
void copy_url_location(struct terminal *, void *, struct session *);
void selected_item(struct terminal *, void *, struct session *);
void toggle(struct session *, struct f_data_c *, int);
void do_for_frame(struct session *, void (*)(struct session *, struct f_data_c *, int), int);
int get_current_state(struct session *);
unsigned char *print_current_link(struct session *);
unsigned char *print_current_title(struct session *);
void loc_msg(struct terminal *, struct location *, struct f_data_c *);
void state_msg(struct session *);
void head_msg(struct session *);
void search_for(struct session *, unsigned char *);
void search_for_back(struct session *, unsigned char *);
void find_next(struct session *, struct f_data_c *, int);
void find_next_back(struct session *, struct f_data_c *, int);
void set_frame(struct session *, struct f_data_c *, int);
struct f_data_c *current_frame(struct session *);
void reset_form(struct f_data_c *f, int form_num);
void set_textarea(struct session *, struct f_data_c *, int);
void free_format_text_cache(void);

void copy_js_event_spec(struct js_event_spec **, struct js_event_spec *);
int join_js_event_spec(struct js_event_spec **, struct js_event_spec *);
void free_js_event_spec(struct js_event_spec *);
void create_js_event_spec(struct js_event_spec **);
int compare_js_event_spec(struct js_event_spec *, struct js_event_spec *);
unsigned char *print_js_event_spec(struct js_event_spec *);

/* font_inc.c */

#ifdef G
extern struct letter letter_data[];
extern struct font font_table[];
extern int n_fonts; /* Number of fonts. font number 0 is system_font (it's
		     * images are in system_font/ directory) and is used
		     * for special purpose.
		     */
#endif

/* gif.c */

#ifdef G

void gif_destroy_decoder(struct cached_image *);
void gif_start(struct cached_image *goi);
void gif_restart(unsigned char *data, int length);

void xbm_start(struct cached_image *goi);
void xbm_restart(struct cached_image *goi, unsigned char *data, int length);

#endif

/* png.c */

#ifdef G

void png_start(struct cached_image *cimg);
void png_restart(struct cached_image *cimg, unsigned char *data, int length);
void png_destroy_decoder(struct cached_image *cimg);
void add_png_version(unsigned char **s, int *l);

#endif /* #ifdef G */

/* tiff.c */

#if defined(G) && defined(HAVE_TIFF)

void tiff_start(struct cached_image *cimg);
void tiff_restart(struct cached_image *cimg, unsigned char *data, int length);
void tiff_finish(struct cached_image *cimg);
void tiff_destroy_decoder(struct cached_image *cimg);

void add_tiff_version(unsigned char **s, int *l);

#endif /* #if defined(G) && defined(HAVE_TIFF) */

/* svg.c */

#ifdef G

void svg_start(struct cached_image *cimg);
void svg_restart(struct cached_image *cimg, unsigned char *data, int length);
void svg_finish(struct cached_image *cimg);
void svg_destroy_decoder(struct cached_image *cimg);

void add_svg_version(unsigned char **s, int *l);

#endif

/* img.c */

#ifdef G

struct image_description {
	unsigned char *url;		/* url=completed url */
	int xsize, ysize;		/* -1 --- unknown size. Space:pixel
					   space of the screen */
	int link_num;
	int link_order;
	unsigned char *name;
	unsigned char *alt;
	unsigned char *src;		/* reflects the src attribute */
	int border, vspace, hspace;
	int align;
	int ismap;
	int insert_flag;		/* pokud je 1, ma se vlozit do seznamu obrazku ve f_data */

	unsigned char *usemap;
	unsigned autoscale_x, autoscale_y; /* Requested autoscale dimensions
					      (maximum allowed rectangle), 0,0
					      means turned off. 0,something or
					      something,0 not allowed. */
};

struct gif_table_entry
{
 unsigned char end_char;
 unsigned char garbage; /* This has nothing common to do with code table:
			   this is temporarily used for reverting strings :-) */
 short pointer; /* points onto another entry in table, number 0...4095.
		   number -1 means it end there, the end_char is the last
		   number -2 means that this entry is no filled in yet.
		*/
};

extern int end_callback_hit;
extern struct cached_image *global_cimg;

/* Below are internal functions shared with imgcache.c, gif.c, and xbm.c */
int header_dimensions_known(struct cached_image *cimg);
void img_end(struct cached_image *cimg);
void compute_background_8(unsigned char *rgb, struct cached_image *cimg);
void buffer_to_bitmap_incremental(struct cached_image *cimg
	,unsigned char *buffer, int height, int yoff, int *dregs, int use_strip);

/* Below is external interface provided by img.c */
struct g_part;
int get_foreground(int rgb);
struct g_object_image *insert_image(struct g_part *p, struct image_description *im);
void change_image (struct g_object_image *goi, unsigned char *url, unsigned char *src, struct f_data *fdata);
void img_destruct_cached_image(struct cached_image *img);

#endif

/* jpeg.c */

#if defined(G) && defined(HAVE_JPEG)

/* Functions exported by jpeg.c for higher layers */
void jpeg_start(struct cached_image *cimg);
void jpeg_restart(struct cached_image *cimg, unsigned char *data, int length);
void jpeg_destroy_decoder(struct cached_image *cimg);
void add_jpeg_version(unsigned char **s, int *l);

#endif /* #if defined(G) && defined(HAVE_JPEG) */

int known_image_type(unsigned char *type);

/* imgcache.c */

#ifdef G

void init_imgcache(void);
my_uintptr_t imgcache_info(int type);
struct cached_image *find_cached_image(int bg, unsigned char *url, int xw, int
		yw, int xyw_meaning, int scale, unsigned aspect);
void add_image_to_cache(struct cached_image *ci);

#endif

/* view_gr.c */

#ifdef G

/* intersection of 2 intervals s=start, l=len (len 0 is empty interval) */
static inline void intersect(int s1, int l1, int s2, int l2, int *s3, int *l3)
{
	int e1=s1+l1;
	int e2=s2+l2;
	int e3;

	if (e1<s1){int tmp=s1; s1=e1; e1=tmp; }
	if (e2<s2){int tmp=s2; s2=e2; e2=tmp; }

	if (!l1||!l2) goto __intersect_empty;
	if (s1<=s2&&s2<=e1)
		*s3=s2;
	else if (s2<s1)
		*s3=s1;
	else
		goto __intersect_empty;

	if (s1<=e2&&e2<=e1)
		e3=e2;
	else if (e2>e1)
		e3=e1;
	else goto __intersect_empty;

	*l3=e3-*s3;
	return;

	__intersect_empty:
	*s3=0;
	*l3=0;
	return;
}


void g_release_background(struct background *bg);
void g_draw_background(struct graphics_device *dev, struct background *bg, int x, int y, int xw, int yw);
int g_forward_mouse(struct f_data_c *fd, struct g_object *a, int x, int y, int b);

void draw_vscroll_bar(struct graphics_device *dev, int x, int y, int yw, int total, int view, int pos);
void draw_hscroll_bar(struct graphics_device *dev, int x, int y, int xw, int total, int view, int pos);
void get_scrollbar_pos(int dsize, int total, int vsize, int vpos, int *start, int *end);


void get_parents(struct f_data *f, struct g_object *a);

void g_dummy_mouse(struct f_data_c *, struct g_object *, int, int, int);
void g_text_mouse(struct f_data_c *, struct g_object_text *, int, int, int);
void g_line_mouse(struct f_data_c *, struct g_object_line *, int, int, int);
void g_area_mouse(struct f_data_c *, struct g_object_area *, int, int, int);

void g_dummy_draw(struct f_data_c *, struct g_object *, int, int);
void g_text_draw(struct f_data_c *, struct g_object_text *, int, int);
void g_line_draw(struct f_data_c *, struct g_object_line *, int, int);
void g_area_draw(struct f_data_c *, struct g_object_area *, int, int);

void g_tag_destruct(struct g_object *);
void g_text_destruct(struct g_object_text *);
void g_line_destruct(struct g_object_line *);
void g_line_bg_destruct(struct g_object_line *);
void g_area_destruct(struct g_object_area *);

void g_line_get_list(struct g_object_line *, void (*)(struct g_object *parent, struct g_object *child));
void g_area_get_list(struct g_object_area *, void (*)(struct g_object *parent, struct g_object *child));

void draw_one_object(struct f_data_c *fd, struct g_object *o);
void draw_title(struct f_data_c *f);
void draw_graphical_doc(struct terminal *t, struct f_data_c *scr, int active);
int g_next_link(struct f_data_c *fd, int dir);
int g_frame_ev(struct session *ses, struct f_data_c *fd, struct links_event *ev);
void g_find_next(struct f_data_c *f, int);

int is_link_in_view(struct f_data_c *fd, int nl);

void init_grview(void);

#endif

/* html.c */

#define AT_BOLD		1
#define AT_ITALIC	2
#define AT_UNDERLINE	4
#define AT_FIXED	8
#define AT_GRAPHICS	16
#define AT_INVERT	32

#define AL_LEFT		0
#define AL_CENTER	1
#define AL_RIGHT	2
#define AL_BLOCK	3
#define AL_NO		4
#define AL_NO_BREAKABLE	5
#define AL_BOTTOM	6
#define AL_MIDDLE	7
#define AL_TOP		8

#define AL_MASK		0x1f

#define AL_NOBRLEXP	0x20
#define AL_MONO		0x40
#define AL_EXTD_TEXT	0x80
	/* DIRTY! for backward compatibility with old menu code */

struct text_attrib_beginning {
	int attr;
	struct rgb fg;
	struct rgb bg;
	int fontsize;
	int baseline;
};

struct text_attrib {
	int attr;
	struct rgb fg;
	struct rgb bg;
	int fontsize;
	int baseline;
	unsigned char *fontface;
	unsigned char *link;
	unsigned char *target;
	unsigned char *image;
	struct js_event_spec *js_event;
	struct form_control *form;
	struct rgb clink;
	unsigned char *href_base;
	unsigned char *target_base;
	unsigned char *select;
	int select_disabled;
};

#define P_NUMBER	1
#define P_alpha		2
#define P_ALPHA		3
#define P_roman		4
#define P_ROMAN		5
#define P_STAR		1
#define P_O		2
#define P_PLUS		3
#define P_LISTMASK	7
#define P_COMPACT	8

struct par_attrib {
	int align;
	int leftmargin;
	int rightmargin;
	int width;
	int list_level;
	unsigned list_number;
	int dd_margin;
	int flags;
	struct rgb bgcolor;
	int implicit_pre_wrap;
};

struct html_element {
	struct html_element *next;
	struct html_element *prev;
	struct text_attrib attr;
	struct par_attrib parattr;
	int invisible;
	unsigned char *name;
	int namelen;
	unsigned char *options;
	int linebreak;
	int dontkill;
	struct frameset_desc *frameset;
};

extern int get_attr_val_nl;

extern struct list_head html_stack;
extern int line_breax;

extern int html_format_changed;

extern unsigned char *startf;
extern unsigned char *eofff;

#define format_ (((struct html_element *)html_stack.next)->attr)
#define par_format (((struct html_element *)html_stack.next)->parattr)
#define html_top (*(struct html_element *)html_stack.next)

extern void *ff;
extern void (*put_chars_f)(void *, unsigned char *, int);
extern void (*line_break_f)(void *);
extern void *(*special_f)(void *, int, ...);

extern int table_level;
extern int empty_format;

extern struct form form;
extern unsigned char *last_form_tag;
extern unsigned char *last_form_attr;
extern unsigned char *last_input_tag;

extern unsigned char *last_link;
extern unsigned char *last_image;
extern unsigned char *last_target;
extern struct form_control *last_form;
extern struct js_event_spec *last_js_event;
extern int js_fun_depth;
extern int js_memory_limit;

int parse_element(unsigned char *, unsigned char *, unsigned char **, int *, unsigned char **, unsigned char **);
unsigned char *get_attr_val(unsigned char *, unsigned char *);
int has_attr(unsigned char *, unsigned char *);
int get_num(unsigned char *, unsigned char *);
int get_width(unsigned char *, unsigned char *, int);
int get_color(unsigned char *, unsigned char *, struct rgb *);
int get_bgcolor(unsigned char *, struct rgb *);
void html_stack_dup(void);
void kill_html_stack_item(struct html_element *);
unsigned char *skip_comment(unsigned char *, unsigned char *);
void parse_html(unsigned char *, unsigned char *, void (*)(void *, unsigned char *, int), void (*)(void *), void *(*)(void *, int, ...), void *, unsigned char *);
int get_image_map(unsigned char *, unsigned char *, unsigned char *, unsigned char *a, struct menu_item **, struct memory_list **, unsigned char *, unsigned char *, int, int, int, int gfx);
void scan_http_equiv(unsigned char *, unsigned char *, unsigned char **, int *, unsigned char **, unsigned char **, unsigned char **, int *, struct js_event_spec **);

int decode_color(unsigned char *, struct rgb *);

#define SP_TAG		0
#define SP_CONTROL	1
#define SP_TABLE	2
#define SP_USED		3
#define SP_FRAMESET	4
#define SP_FRAME	5
#define SP_SCRIPT	6
#define SP_IMAGE	7
#define SP_NOWRAP	8
#define SP_REFRESH	9
#define SP_SET_BASE	10
#define SP_HR		11

struct frameset_param {
	struct frameset_desc *parent;
	int x, y;
	int *xw, *yw;
};

#define SCROLLING_NO	0
#define SCROLLING_YES	1
#define SCROLLING_AUTO	2

struct frame_param {
	struct frameset_desc *parent;
	unsigned char *name;
	unsigned char *url;
	int marginwidth;
	int marginheight;
	unsigned char scrolling;
};

struct refresh_param {
	unsigned char *url;
	int time;
};

struct hr_param {
	int size;
	int width;
};

void free_menu(struct menu_item *);
void do_select_submenu(struct terminal *, struct menu_item *, struct session *);

void clr_white(unsigned char *name);
void clr_spaces(unsigned char *name, int firstlast);

/* html_r.c */

extern int g_ctrl_num;

extern struct conv_table *convert_table;

struct part {
	int x, y;
	int xp, yp;
	int xmax;
	int xa;
	int cx, cy;
	struct f_data *data;
	int attribute;
	unsigned char *spaces;
	int z_spaces;
	int spl;
	int link_num;
	struct list_head uf;
#ifdef ENABLE_UTF8
	unsigned char utf8_part[7];
	unsigned char utf8_part_len;
#endif
};

#ifdef G
struct g_part {
	int x, y;
	int xmax;
	int cx, cy;
	int cx_w;
	struct g_object_area *root;
	struct g_object_line *line;
	struct g_object_text *text;
	int pending_text_len;
	struct wrap_struct w;
	struct style *current_style;
	struct f_data *data;
	int link_num;
	struct list_head uf;
};
#endif

struct sizes {
	int xmin, xmax, y;
};

extern struct f_data *current_f_data;

void free_additional_files(struct additional_files **);
void free_frameset_desc(struct frameset_desc *);
struct frameset_desc *copy_frameset_desc(struct frameset_desc *);

struct f_data *init_formatted(struct document_options *);
void destroy_formatted(struct f_data *);

/* d_opt je podle Mikulase nedefinovany mimo html parser, tak to jinde nepouzivejte
 *
 * -- Brain
 */
extern struct document_options dd_opt;
extern struct document_options *d_opt;
extern int margin;

int find_nearest_color(struct rgb *r, int l);
int fg_color(int fg, int bg);

void xxpand_line(struct part *, int, int);
void xxpand_lines(struct part *, int);
void xset_hchar(struct part *, int, int, unsigned, unsigned char);
void xset_hchars(struct part *, int, int, int, unsigned, unsigned char);
void html_tag(struct f_data *, unsigned char *, int, int);
void process_script(struct f_data *, unsigned char *);
void set_base(struct f_data *, unsigned char *);
void html_process_refresh(struct f_data *, unsigned char *, int );

int compare_opt(struct document_options *, struct document_options *);
void copy_opt(struct document_options *, struct document_options *);

struct link *new_link(struct f_data *);
struct conv_table *get_convert_table(unsigned char *, int, int, int *, int *, int);
struct part *format_html_part(unsigned char *, unsigned char *, int, int, int, struct f_data *, int, int, unsigned char *, int);
void really_format_html(struct cache_entry *, unsigned char *, unsigned char *, struct f_data *, int frame);
struct link *get_link_at_location(struct f_data *f, int x, int y);
int get_search_data(struct f_data *);

struct frameset_desc *create_frameset(struct f_data *fda, struct frameset_param *fp);
void create_frame(struct frame_param *fp);

/* html_gr.c */

#ifdef G

void release_image_map(struct image_map *map);
int is_in_area(struct map_area *a, int x, int y);

struct background *get_background(unsigned char *bg, unsigned char *bgcolor);

void g_x_extend_area(struct g_object_area *a, int width, int height, int align);
struct g_part *g_format_html_part(unsigned char *, unsigned char *, int, int, int, unsigned char *, int, unsigned char *, unsigned char *, struct f_data *);
void g_release_part(struct g_part *);
int g_get_area_width(struct g_object_area *o);
void add_object(struct g_part *pp, struct g_object *o);
void add_object_to_line(struct g_part *pp, struct g_object_line **lp,
	struct g_object *go);
void flush_pending_text_to_line(struct g_part *p);
void flush_pending_line_to_obj(struct g_part *p, int minheight);

#endif

/* html_tbl.c */

unsigned char *skip_element(unsigned char *, unsigned char *, unsigned char *, int);
void format_table(unsigned char *, unsigned char *, unsigned char *, unsigned char **, void *);
void table_bg(struct text_attrib *ta, unsigned char bgstr[8]);

void *find_table_cache_entry(unsigned char *start, unsigned char *end, int align, int m, int width, int xs, int link_num);
void add_table_cache_entry(unsigned char *start, unsigned char *end, int align, int m, int width, int xs, int link_num, void *p);

/* default.c */

extern int ggr;
extern int force_g;
extern unsigned char ggr_drv[MAX_STR_LEN];
extern unsigned char ggr_mode[MAX_STR_LEN];
extern unsigned char ggr_display[MAX_STR_LEN];

extern unsigned char default_target[MAX_STR_LEN];

unsigned char *parse_options(int, unsigned char *[]);
void init_home(void);
unsigned char *read_config_file(unsigned char *);
int write_to_config_file(unsigned char *, unsigned char *);
void load_config(void);
void write_config(struct terminal *);
void write_html_config(struct terminal *);
void end_config(void);

void load_url_history(void);
void save_url_history(void);

struct driver_param {
	struct driver_param *next;
	struct driver_param *prev;
	int kbd_codepage;
	unsigned char *param;
	unsigned char *shell;
	int nosave;
	unsigned char name[1];
};

struct driver_param *get_driver_param(unsigned char *);

extern int anonymous;

extern unsigned char system_name[];
extern unsigned char compiler_name[];

extern unsigned char *links_home;
extern int first_use;

extern int disable_libevent;
extern int disable_openmp;
extern int no_connect;
extern int base_session;
#define D_DUMP		1
#define D_SOURCE	2
extern int dmp;
extern int screen_width;
extern int dump_codepage;
extern int force_html;

extern int max_connections;
extern int max_connections_to_host;
extern int max_tries;
extern int receive_timeout;
extern int unrestartable_receive_timeout;
extern unsigned char bind_ip_address[16];
extern unsigned char bind_ipv6_address[INET6_ADDRSTRLEN];
extern int async_lookup;
extern int download_utime;

extern int max_format_cache_entries;
extern int memory_cache_size;
extern int image_cache_size;
extern int font_cache_size;
extern int aggressive_cache;

struct ipv6_options {
	int addr_preference;
};

#define ADDR_PREFERENCE_DEFAULT		0
#define ADDR_PREFERENCE_IPV4		1
#define ADDR_PREFERENCE_IPV6		2
#define ADDR_PREFERENCE_IPV4_ONLY	3
#define ADDR_PREFERENCE_IPV6_ONLY	4

extern struct ipv6_options ipv6_options;

#define REFERER_NONE			0
#define REFERER_SAME_URL		1
#define REFERER_FAKE			2
#define REFERER_REAL			3
#define REFERER_REAL_SAME_SERVER	4

struct proxies {
	unsigned char http_proxy[MAX_STR_LEN];
	unsigned char ftp_proxy[MAX_STR_LEN];
	unsigned char https_proxy[MAX_STR_LEN];
	unsigned char socks_proxy[MAX_STR_LEN];
	unsigned char dns_append[MAX_STR_LEN];
	int only_proxies;
};

extern struct proxies proxies;

#define SSL_ACCEPT_INVALID_CERTIFICATE	0
#define SSL_WARN_ON_INVALID_CERTIFICATE	1
#define SSL_REJECT_INVALID_CERTIFICATE	2

struct ssl_options {
	int certificates;
	unsigned char client_cert_key[MAX_STR_LEN];
	unsigned char client_cert_crt[MAX_STR_LEN];
	unsigned char client_cert_password[MAX_STR_LEN];
};

extern struct ssl_options ssl_options;

struct http_header_options {
	int fake_firefox;
	int do_not_track;
	int referer;
	unsigned char fake_referer[MAX_STR_LEN];
	unsigned char fake_useragent[MAX_STR_LEN];
	unsigned char extra_header[MAX_STR_LEN];
};

struct http_options {
	int http10;
	int allow_blacklist;
	int bug_302_redirect;
	int bug_post_no_keepalive;
	int no_accept_charset;
	int no_compression;
	int retry_internal_errors;
	struct http_header_options header;
};

extern struct http_options http_options;

struct ftp_options {
	unsigned char anon_pass[MAX_STR_LEN];
	int passive_ftp;
	int eprt_epsv;
	int fast_ftp;
	int set_tos;
};

extern struct ftp_options ftp_options;

struct smb_options {
	int allow_hyperlinks_to_smb;
};

extern struct smb_options smb_options;

extern unsigned char download_dir[];

#define SCRUB_HEADERS	(proxies.only_proxies || http_options.header.fake_firefox)

#ifdef JS
extern int js_enable;
extern int js_verbose_errors;
extern int js_verbose_warnings;
extern int js_all_conversions;
extern int js_global_resolve;
#endif

extern double display_red_gamma,display_green_gamma,display_blue_gamma;
extern double user_gamma;
extern double bfu_aspect;
extern int display_optimize;	/*0=CRT, 1=LCD RGB, 2=LCD BGR */
extern int dither_letters;
extern int dither_images;
extern int gamma_bits;
extern int overwrite_instead_of_scroll;

extern int menu_font_size;
extern unsigned G_BFU_FG_COLOR, G_BFU_BG_COLOR, G_SCROLL_BAR_AREA_COLOR, G_SCROLL_BAR_BAR_COLOR, G_SCROLL_BAR_FRAME_COLOR;

extern unsigned char bookmarks_file[MAX_STR_LEN];
extern int bookmarks_codepage;

extern int save_history;

extern struct document_setup dds;

/* listedit.c */

#define TITLE_EDIT 0
#define TITLE_ADD 1

struct list {
	void *next;
	void *prev;
	unsigned char type;
	/*
	 * bit 0: 0=item, 1=directory
	 * bit 1: directory is open (1)/closed (0); for item unused
	 * bit 2: 1=item is selected 0=item is not selected
	 */
	int depth;
	void *fotr;   /* ignored when list is flat */
};

/* regexp.c */

char *regexp_replace(char *, char *, char *);


struct list_description {
	unsigned char type;  /* 0=flat, 1=tree */
	struct list *list;   /* head of the list */
	void *(*new_item)(void * /* data in internal format */);  /* creates new item, does NOT add to the list */
	void (*edit_item)(struct dialog_data *, void * /* item */, void(*)(struct dialog_data *,void *,void *,struct list_description *) /* ok function */, void * /* parameter for the ok_function */, unsigned char);  /* must call call delete_item on the item after all */
	void *(*default_value)(struct session *, unsigned char /* 0=item, 1=directory */);  /* called when add button is pressed, allocates memory, return value is passed to the new_item function, new_item fills the item with this data */
	void (*delete_item)(void *);  /* delete item, if next and prev are not NULL adjusts pointers in the list */
	void (*copy_item)(void * /* old */, void * /* new */);  /* gets 2 allocated items, copies all item data except pointers from first item to second one, old data (in second item) will be destroyed */
	unsigned char* (*type_item)(struct terminal *, void*, int /* 0=type whole item (e.g. when deleting item), 1=type only e.g title (in list window )*/);   /* alllocates buffer and writes item into it */
	void *(*find_item)(void *start_item, unsigned char *string, int direction /* 1 or -1 */); /* returns pointer to the first item matching given string or NULL if failed. Search starts at start_item including. */
	struct history *search_history;
	int codepage;	/* codepage of all string */
	int n_items;   /* number of items in main window */

	/* following items are string codes */
	int item_description;  /* e.g. "bookmark" or "extension" ... */
	int already_in_use;   /* e.g. "Bookmarks window is already open" */
	int window_title;   /* main window title */
	int delete_dialog_title;   /* e.g. "Delete bookmark dialog" */
	int button;  /* when there's no button button_fn is NULL */

	void (*button_fn)(struct session *, void *);  /* gets pointer to the item */
	void (*save)(struct session *);

	/* internal variables, should not be modified, initially set to 0 */
	struct list *current_pos;
	struct list *win_offset;
	int win_pos;
	int open;  /* 0=closed, 1=open */
	int modified; /* listedit reports 1 when the list was modified by user (and should be e.g. saved) */
	struct dialog_data *dlg;  /* current dialog, valid only when open==1 */
	unsigned char *search_word;
	int search_direction;
};

int test_list_window_in_use(struct list_description *ld, struct terminal *term);
int create_list_window(struct list_description *,struct list *,struct terminal *,struct session *);
void reinit_list_window(struct list_description *ld);	/* reinitializes list window */

/* bookmark.c */

/* Where all bookmarks are kept */
extern struct list bookmarks;

void finalize_bookmarks(void);   /* called, when exiting links */
void init_bookmarks(void);   /* called at start */
void reinit_bookmarks(struct session *ses, unsigned char *new_bookmarks_file, int new_bookmarks_codepage);

/* Launches bookmark manager */
void menu_bookmark_manager(struct terminal *, void *, struct session *);

#endif /* #ifndef LINKS_H */
