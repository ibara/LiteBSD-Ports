/* os_dep.c
 * (c) 2002 Mikulas Patocka
 * This file is a part of the Links program, released under GPL.
 */

#include "links.h"

#ifdef HAVE_SYS_IOCTL_H
#include <sys/ioctl.h>
#endif

#ifdef USE_GPM
#include <gpm.h>
#endif

#ifdef WIN
#include <windows.h>
#endif

#ifdef HAVE_PTHREADS
#include <pthread.h>
#endif

#ifdef OS2

#define INCL_MOU
#define INCL_VIO
#define INCL_DOSPROCESS
#define INCL_DOSERRORS
#define INCL_DOSMODULEMGR
#define INCL_DOSMISC
#define INCL_DOSNLS
#define INCL_WIN
#define INCL_WINCLIPBOARD
#define INCL_WINSWITCHLIST
#include <os2.h>
#include <io.h>
#include <process.h>
#include <sys/video.h>
#ifdef HAVE_SYS_FMUTEX_H
#include <sys/builtin.h>
#include <sys/fmutex.h>
#endif

#ifdef X2
/* from xf86sup - XFree86 OS/2 support driver */
#include <pty.h>
#endif

#endif

#ifdef OS2

/* The process crashes if we write to console from high address - so we must
 * never do it.
 * TCP/IP 4.0 returns EFAULT if we do I/O to/from high address - we test for
 * EFAULT and retry with a bounce buffer.  */

#define BOUNCE_BUFFER_SIZE	256

int bounced_read(int fd, void *buf, size_t size)
{
	unsigned char *bounce_buffer;
	size_t xsiz;
	int r;
	if (fd < 3 && (unsigned long)buf + size > 0x20000000) goto bounce;
	r = _read(fd, buf, size);
	if (r == -1 && errno == EFAULT) goto bounce;
	return r;
	bounce:
	xsiz = size > BOUNCE_BUFFER_SIZE ? BOUNCE_BUFFER_SIZE : size;
	bounce_buffer = alloca(xsiz);
	r = _read(fd, bounce_buffer, xsiz);
	if (r > 0) memcpy(buf, bounce_buffer, r);
	return r;
}

int bounced_write(int fd, const void *buf, size_t size)
{
	unsigned char *bounce_buffer;
	size_t xsiz;
	int r;
	if (fd < 3 && (unsigned long)buf + size > 0x20000000) goto bounce;
	r = _write(fd, buf, size);
	if (r == -1 && errno == EFAULT) goto bounce;
	return r;
	bounce:
	xsiz = size > BOUNCE_BUFFER_SIZE ? BOUNCE_BUFFER_SIZE : size;
	bounce_buffer = alloca(xsiz);
	memcpy(bounce_buffer, buf, xsiz);
	return _write(fd, bounce_buffer, xsiz);
}

#endif

#ifdef OS2_ADVANCED_HEAP

#include <umalloc.h>

#ifndef OBJ_ANY
#define OBJ_ANY		0x0400
#endif

unsigned long mem_requested = 0;
unsigned long blocks_requested = 0;

static int dosallocmem_attrib = PAG_READ | PAG_WRITE | PAG_COMMIT;

#define HEAP_ALIGN		0x10000
#define HEAP_PAD		2
#define HEAP_MAXPAD		0x1000000
#define HEAP_IMMEDIATE_RELEASE	0x100000

static void heap_release(Heap_t h, void *ptr, size_t len)
{
	int rc;
	mem_requested -= ((len | 4095) + 1);
	blocks_requested--;
	rc = DosFreeMem(ptr);
	/*fprintf(stderr, "heap free %p -> %d\n", ptr, rc);*/
	if (rc) {
		fatal_exit("DosFreeMem failed: %d", rc);
	}
}

static void *heap_alloc(Heap_t h, size_t *size, int *pclean)
{
	void *result;
	int rc;
	/* If we rounded up to page size, EMX would join all allocations
	 * to one segment and refuse to free memory. So round up to
	 * page size - 1 */
	size_t real_size = *size;
	if (real_size < HEAP_MAXPAD / HEAP_PAD) {
		real_size *= HEAP_PAD;
		real_size = real_size | (HEAP_ALIGN - 1);
	} else {
		real_size |= 1;
	}
	rc = DosAllocMem(&result, real_size, dosallocmem_attrib);
	/*fprintf(stderr, "heap alloc %d -> %p, %d\n", *size, result, rc);*/
	if (!rc) {
		/*
		 * Hitting the shared arena has a negative impact on the whole
		 * system. Therefore, we fake failure (so that Links frees
		 * some caches) and try allocating near the shared arena only
		 * as a last resort.
		 */
		if ((unsigned long)result >= 0x12000000 &&
		    (unsigned long)result < 0x20000000) {
			if (!malloc_try_hard) {
				heap_release(NULL, result, real_size);
				return NULL;
			}
		}
		*size = real_size;
		*pclean = _BLOCK_CLEAN;
		mem_requested += ((real_size | 4095) + 1);
		blocks_requested++;
		return result;
	} else {
		return NULL;
	}
}

static void init_os2_heap(void)
{
	Heap_t new_heap;
	size_t init_size = _HEAP_MIN_SIZE;
	void *init_mem;
	int init_clean;
	dosallocmem_attrib |= OBJ_ANY;
	init_mem = heap_alloc(NULL, &init_size, &init_clean);
	if (!init_mem) {
		dosallocmem_attrib &= ~OBJ_ANY;
		init_mem = heap_alloc(NULL, &init_size, &init_clean);
		if (!init_mem) {
			return;
		}
	}
	new_heap = _ucreate(init_mem, init_size, init_clean, _HEAP_REGULAR, heap_alloc, heap_release);
	if (!new_heap) {
		heap_release(NULL, init_mem, init_size);
		return;
	}
	if (_uopen(new_heap) == -1) {
#if defined(HAVE__UDESTROY) && defined(_FORCE)
		_udestroy(new_heap, _FORCE);
#else
		heap_release(NULL, init_mem, init_size);
#endif
		return;
	}
	_udefault(new_heap);
}

void mem_freed_large(size_t size)
{
#if defined(HAVE__HEAPMIN)
	if (size >= HEAP_IMMEDIATE_RELEASE)
		_heapmin();
#endif
}

#endif


#ifdef OS2
static int os2_full_screen = 0;
static int os2_detached = 0;
static PTIB os2_tib = NULL;
PPIB os2_pib = NULL;
static HSWITCH os2_switchhandle = NULLHANDLE;
#ifdef HAVE_SYS_FMUTEX_H
static _fmutex fd_mutex;
#endif
#elif defined(HAVE_PTHREADS)
static pthread_mutex_t pth_mutex;
static void fd_lock(void);
static void fd_unlock(void);
static void fd_init(void)
{
	int r;
	r = pthread_mutex_init(&pth_mutex, NULL);
	if (r)
		fatal_exit("pthread_mutex_create failed: %s", strerror(r));
}
#endif

#if !defined(OPENVMS) && !defined(DOS)
void init_os(void)
{
	/* Disable per-thread heap */
#if defined(HAVE_MALLOPT) && defined(M_ARENA_TEST)
	mallopt(M_ARENA_TEST, 1);
#endif
#if defined(HAVE_MALLOPT) && defined(M_ARENA_MAX)
	mallopt(M_ARENA_MAX, 1);
#endif

#ifdef OS2
	DosGetInfoBlocks(&os2_tib, &os2_pib);
	if (os2_pib) {
		os2_switchhandle = WinQuerySwitchHandle(0, os2_pib->pib_ulpid);
		os2_full_screen = os2_pib->pib_ultype == 0;
		os2_detached = os2_pib->pib_ultype == 4;
		if (os2_pib->pib_ultype == 3) force_g = 1;
	}
#ifdef HAVE_SYS_FMUTEX_H
	if (_fmutex_create(&fd_mutex, 0))
		fatal_exit("failed to create fd mutex");
#endif
#elif defined(HAVE_PTHREADS)
	{
		int r;
		fd_init();
		r = pthread_atfork(fd_lock, fd_unlock, fd_init);
		if (r)
			fatal_exit("pthread_atfork failed: %s", strerror(r));
	}
#endif

#ifdef OS2_ADVANCED_HEAP
	init_os2_heap();
#endif

#ifdef WIN
	if (!GetConsoleCP())
		force_g = 1;
#endif
}
#endif


int is_safe_in_shell(unsigned char c)
{
	return c == '@' || c == '+' || c == '-' || c == '.' || c == ',' || c == '=' || (c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') || c == '_' || (c >= 'a' && c <= 'z');
}

static inline int is_safe_in_file(unsigned char c)
{
	return !(c < ' ' || c == '"' || c == '*' || c == '/' || c == ':' || c == '<' || c == '>' || c == '\\' || c == '|' || c >= 0x80);
}

static inline int is_safe_in_url(unsigned char c)
{
	return is_safe_in_shell(c) || c == ':' || c == '/' || c >= 0x80;
}

void check_shell_security(unsigned char **cmd)
{
	unsigned char *c = *cmd;
	while (*c) {
		if (!is_safe_in_shell(*c)) *c = '_';
		c++;
	}
}

void check_filename(unsigned char **file)
{
	unsigned char *c = *file;
	while (*c) {
		if (!is_safe_in_file(*c)) *c = '_';
		c++;
	}
}

int check_shell_url(unsigned char *url)
{
	while (*url) {
		if (!is_safe_in_url(*url)) return -1;
		url++;
	}
	return 0;
}

unsigned char *escape_path(unsigned char *path)
{
	unsigned char *result;
	size_t i;
	if (strchr(cast_const_char path, '"')) return stracpy(path);
	for (i = 0; path[i]; i++) if (!is_safe_in_url(path[i])) goto do_esc;
	return stracpy(path);
	do_esc:
	result = stracpy(cast_uchar "\"");
	add_to_strn(&result, path);
	add_to_strn(&result, cast_uchar "\"");
	return result;
}

static inline int get_e(unsigned char *env)
{
	unsigned char *v;
	if ((v = cast_uchar getenv(cast_const_char env))) return atoi(cast_const_char v);
	return 0;
}

void do_signal(int sig, void (*handler)(int))
{
	errno = 0;
	while (signal(sig, handler) == SIG_ERR && errno == EINTR) errno = 0;
}

void ignore_signals(void)
{
	do_signal(SIGPIPE, SIG_IGN);
#ifdef SIGXFSZ
	do_signal(SIGXFSZ, SIG_IGN);
#endif
#ifdef OPENVMS
#ifdef SIGCHLD
	do_signal(SIGCHLD, SIG_IGN);
#endif
	do_signal(SIGINT, SIG_IGN);
#endif
}

ttime get_time(void)
{
#if defined(OS2) || defined(WIN)
	static unsigned last_tim = 0;
	static ttime add = 0;
	unsigned tim;
#if defined(OS2)
	int rc;
	rc = DosQuerySysInfo(QSV_MS_COUNT, QSV_MS_COUNT, &tim, sizeof tim);
	if (rc) fatal_exit("DosQuerySysInfo failed: %d", rc);
#elif defined(WIN)
	tim = GetTickCount();
#endif
	if (tim < last_tim) {
		add += (ttime)1 << 31 << 1;
	}
	last_tim = tim;
	return tim | add;
#else
	union {
		struct timeval tv;
#if defined(HAVE_CLOCK_GETTIME) && defined(TIME_WITH_SYS_TIME)
		struct timespec ts;
#endif
	} u;
	int rs;
#if defined(HAVE_CLOCK_GETTIME) && defined(TIME_WITH_SYS_TIME)
#if defined(CLOCK_MONOTONIC_RAW)
	EINTRLOOP(rs, clock_gettime(CLOCK_MONOTONIC_RAW, &u.ts));
	if (!rs) return(uttime)u.ts.tv_sec * 1000 + u.ts.tv_nsec / 1000000;
#endif
#if defined(CLOCK_MONOTONIC)
	EINTRLOOP(rs, clock_gettime(CLOCK_MONOTONIC, &u.ts));
	if (!rs) return(uttime)u.ts.tv_sec * 1000 + u.ts.tv_nsec / 1000000;
#endif
#endif
	EINTRLOOP(rs, gettimeofday(&u.tv, NULL));
	if (rs) fatal_exit("gettimeofday failed: %d", errno);
	return (uttime)u.tv.tv_sec * 1000 + u.tv.tv_usec / 1000;
#endif
}


unsigned char *clipboard = NULL;


/* Terminal size */

#if defined(WIN) || defined(OPENVMS)

/* Cygwin has a bug and loses SIGWINCH sometimes, so poll it */

static void winch_thread(void *p, int l)
{
	static int old_xsize, old_ysize;
	static int cur_xsize, cur_ysize;
	if (get_terminal_size(1, &old_xsize, &old_ysize)) return;
	while (1) {
		if (get_terminal_size(1, &cur_xsize, &cur_ysize)) return;
		if ((old_xsize != cur_xsize) || (old_ysize != cur_ysize)) {
			int rr;
			old_xsize = cur_xsize;
			old_ysize = cur_ysize;
			EINTRLOOP(rr, raise(SIGWINCH));
		}
		sleep(1);
	}
}

static void terminal_resize_poll(void)
{
	static int winch_thread_running = 0;
	if (!winch_thread_running) {
		if (start_thread(winch_thread, NULL, 0, 0) >= 0)
			winch_thread_running = 1;
	}
}

#endif

#if defined(UNIX) || defined(WIN) || defined(INTERIX) || defined(BEOS) || defined(RISCOS) || defined(ATHEOS) || defined(SPAD) || defined(OPENVMS)

#ifdef SIGWINCH
static void sigwinch(void *s)
{
	((void (*)(void))s)();
}
#endif

void handle_terminal_resize(int fd, void (*fn)(void))
{
#ifdef SIGWINCH
	install_signal_handler(SIGWINCH, sigwinch, (void *)fn, 0);
#endif
#ifdef WIN
	terminal_resize_poll();
#endif
#ifdef OPENVMS
	if (is_xterm())
		terminal_resize_poll();
#endif
}

void unhandle_terminal_resize(int fd)
{
#ifdef SIGWINCH
	install_signal_handler(SIGWINCH, NULL, NULL, 0);
#endif
}

#if !defined(OPENVMS)

int get_terminal_size(int fd, int *x, int *y)
{
	int rs = -1;
#ifdef TIOCGWINSZ
#ifdef __SUNPRO_C
	volatile	/* Sun Studio misoptimizes it */
#endif
	struct winsize ws;
	EINTRLOOP(rs, ioctl(1, TIOCGWINSZ, &ws));
#endif
	if ((rs == -1
#ifdef TIOCGWINSZ
		|| !(*x = ws.ws_col)
#endif
		) && !(*x = get_e(cast_uchar "COLUMNS"))) {
		*x = 80;
#ifdef _UWIN
		*x = 79;
#endif
	}
	if ((rs == -1
#ifdef TIOCGWINSZ
		|| !(*y = ws.ws_row)
#endif
		) && !(*y = get_e(cast_uchar "LINES"))) {
		*y = 24;
	}
	return 0;
}

#endif

#elif defined(OS2)

#define A_DECL(type, var) type var##1, var##2, *var = _THUNK_PTR_STRUCT_OK(&var##1) ? &var##1 : &var##2

int is_xterm(void)
{
	static int xt = -1;
	if (xt == -1) xt = !!getenv("WINDOWID");
	return xt;
}

static int winch_pipe[2];
static unsigned char winch_thread_running = 0;

#define WINCH_SLEEPTIME 500 /* time in ms for winch thread to sleep */

static void winch_thread(void)
{
	/* A thread which regularly checks whether the size of
	   window has changed. Then raise SIGWINCH or notifiy
	   the thread responsible to handle this. */
	static int old_xsize, old_ysize;
	static int cur_xsize, cur_ysize;

	ignore_signals();
	if (get_terminal_size(1, &old_xsize, &old_ysize)) return;
	while (1) {
		if (get_terminal_size(1, &cur_xsize, &cur_ysize)) return;
		if ((old_xsize != cur_xsize) || (old_ysize != cur_ysize)) {
			int wr;
			old_xsize = cur_xsize;
			old_ysize = cur_ysize;
			EINTRLOOP(wr, (int)write(winch_pipe[1], "x", 1));
			/* Resizing may take some time. So don't send a flood
		     of requests?! */
			_sleep2(2*WINCH_SLEEPTIME);
		}
		else
			_sleep2(WINCH_SLEEPTIME);
	}
}

static void winch(void *s)
{
	unsigned char c;
	while (can_read(winch_pipe[0])) {
		int rd;
		EINTRLOOP(rd, (int)read(winch_pipe[0], &c, 1));
		if (rd != 1) break;
	}
	((void (*)(void))s)();
}

void handle_terminal_resize(int fd, void (*fn)(void))
{
	if (!is_xterm()) return;
	if (!winch_thread_running) {
		if (c_pipe(winch_pipe) < 0) return;
		winch_thread_running = 1;
		if (_beginthread((void (*)(void *))winch_thread, NULL, 0x32000, NULL) == -1) {
		}
	}
	set_handlers(winch_pipe[0], winch, NULL, fn);
}

void unhandle_terminal_resize(int fd)
{
	if (!winch_thread_running) return;
	set_handlers(winch_pipe[0], NULL, NULL, NULL);
}

int get_terminal_size(int fd, int *x, int *y)
{
	if (is_xterm()) {
#ifdef X2
		int arc;
		struct winsize win;

		/* fd = STDIN_FILENO; */
		arc = ptioctl(1, TIOCGWINSZ, &win);
		if (arc) {
			*x = 80;
			*y = 24;
			return 0;
		}
		*y = win.ws_row;
		*x = win.ws_col;
		goto set_default;
#else
		*x = 80; *y = 24;
		return 0;
#endif
	} else {
		int a[2] = { 0, 0 };
		_scrsize(a);
		*x = a[0];
		*y = a[1];
#ifdef X2
		set_default:
#endif
		if (*x == 0) {
			*x = get_e("COLUMNS");
			if (*x == 0) *x = 80;
		}
		if (*y == 0) {
			*y = get_e("LINES");
			if (*y == 0) *y = 24;
		}
	}
	return 0;
}

#endif


#if defined(OS2) && defined(HAVE_SYS_FMUTEX_H)

static void fd_lock(void)
{
	_fmutex_request(&fd_mutex, _FMR_IGNINT);
}

static void fd_unlock(void)
{
	_fmutex_release(&fd_mutex);
}

#elif defined(HAVE_PTHREADS)

static void fd_lock(void)
{
	int r;
	r = pthread_mutex_lock(&pth_mutex);
	if (r)
		fatal_exit("pthread_mutex_lock failed: %s", strerror(r));
}

static void fd_unlock(void)
{
	int r;
	r = pthread_mutex_unlock(&pth_mutex);
	if (r)
		fatal_exit("pthread_mutex_lock failed: %s", strerror(r));
}

#else

#define fd_lock()	do { } while (0)
#define fd_unlock()	do { } while (0)

#endif

static void new_fd_cloexec(int fd)
{
	int rs;
	EINTRLOOP(rs, fcntl(fd, F_SETFD, FD_CLOEXEC));
}

static void new_fd_bin(int fd)
{
	new_fd_cloexec(fd);
#if defined(OS2) || (defined(WIN) && !defined(_UWIN)) || defined(DOS)
	setmode(fd, O_BINARY);
#endif
}

/* Pipe */

#if !defined(OPENVMS) && !defined(DOS)

void set_nonblock(int fd)
{
#ifdef O_NONBLOCK
	int rs;
	EINTRLOOP(rs, fcntl(fd, F_SETFL, O_NONBLOCK));
#elif defined(FIONBIO)
	int rs;
	int on = 1;
	EINTRLOOP(rs, ioctl(fd, FIONBIO, &on));
#endif
}

int c_pipe(int *fd)
{
	int r;
	fd_lock();
	EINTRLOOP(r, pipe(fd));
	if (!r) new_fd_bin(fd[0]), new_fd_bin(fd[1]);
	fd_unlock();
	return r;
}

#endif

int c_dup(int oh)
{
	int h;
	fd_lock();
	EINTRLOOP(h, dup(oh));
	if (h != -1) new_fd_cloexec(h);
	fd_unlock();
	return h;
}

int c_socket(int d, int t, int p)
{
	int h;
	fd_lock();
	EINTRLOOP(h, socket(d, t, p));
	if (h != -1) new_fd_cloexec(h);
	fd_unlock();
	return h;
}

int c_accept(int h, struct sockaddr *addr, socklen_t *addrlen)
{
	int rh;
	fd_lock();
	EINTRLOOP(rh, accept(h, addr, addrlen));
	if (rh != -1) new_fd_cloexec(rh);
	fd_unlock();
	return rh;
}

int c_open(unsigned char *path, int flags)
{
	int h;
	fd_lock();
	EINTRLOOP(h, open(cast_const_char path, flags));
	if (h != -1) new_fd_bin(h);
	fd_unlock();
	return h;
}

int c_open3(unsigned char *path, int flags, int mode)
{
	int h;
	fd_lock();
	EINTRLOOP(h, open(cast_const_char path, flags, mode));
	if (h != -1) new_fd_bin(h);
	fd_unlock();
	return h;
}

DIR *c_opendir(unsigned char *path)
{
	DIR *d;
	fd_lock();
	ENULLLOOP(d, opendir(cast_const_char path));
#ifdef HAVE_DIRFD
	if (d) {
		int h;
		EINTRLOOP(h, dirfd(d));
		if (h != -1) new_fd_cloexec(h);
	}
#endif
	fd_unlock();
	return d;
}

#if defined(O_SIZE) && defined(__EMX__)

int open_prealloc(unsigned char *name, int flags, int mode, off_t siz)
{
	int h;
	fd_lock();
	EINTRLOOP(h, open(cast_const_char name, flags | O_SIZE, mode, (unsigned long)siz));
	if (h != -1) new_fd_bin(h);
	fd_unlock();
	return h;
}

#elif defined(HAVE_OPEN_PREALLOC)

int open_prealloc(unsigned char *name, int flags, int mode, off_t siz)
{
	int h, rs;
	fd_lock();
	EINTRLOOP(h, open(cast_const_char name, flags, mode));
	if (h == -1) {
		fd_unlock();
		return -1;
	}
	new_fd_bin(h);
	fd_unlock();
#if defined(HAVE_FALLOCATE)
#if defined(FALLOC_FL_KEEP_SIZE)
	EINTRLOOP(rs, fallocate(h, FALLOC_FL_KEEP_SIZE, 0, siz));
#else
	EINTRLOOP(rs, fallocate(h, 0, 0, siz));
#endif
	if (!rs) return h;
#endif
#if defined(HAVE_POSIX_FALLOCATE)
	/* posix_fallocate may fall back to overwriting the file with zeros,
	   so don't use it on too big files */
	if (siz > 134217728)
		return h;
	do {
		rs = posix_fallocate(h, 0, siz);
	} while (rs == EINTR);
	if (!rs) return h;
#endif
	return h;
}

#endif

/* Exec */

#if defined(UNIX) || defined(ATHEOS) || defined(INTERIX)

int is_twterm(void)
{
	static int xt = -1;
	if (xt == -1) xt = !!getenv("TWDISPLAY");
	return xt;
}

#else

int is_twterm(void)
{
	return 0;
}

#endif

#if defined(UNIX) || defined(ATHEOS) || defined(INTERIX)

int is_screen(void)
{
	static int xt = -1;
	if (xt == -1) xt = !!getenv("STY");
	return xt;
}

#else

int is_screen(void)
{
	return 0;
}

#endif

#if defined(UNIX) || defined(SPAD)

int is_xterm(void)
{
	static int xt = -1;
	if (xt == -1) xt = getenv("DISPLAY") && *(char *)getenv("DISPLAY");
	return xt;
}

#elif defined(BEOS) || defined(ATHEOS) || defined(DOS)

int is_xterm(void)
{
	return 0;
}

#elif defined(WIN) || defined(INTERIX)

int is_xterm(void)
{
	static int xt = -1;
	if (xt == -1) xt = !!getenv("WINDOWID");
	return xt;
}

#elif defined(RISCOS)

int is_xterm(void)
{
	return 1;
}

#endif

void close_fork_tty(void)
{
	struct terminal *t;
	struct download *d;
	struct connection *c;
	struct k_conn *k;
	int rs;
#ifndef NO_SIGNAL_HANDLERS
	EINTRLOOP(rs, close(signal_pipe[0]));
	EINTRLOOP(rs, close(signal_pipe[1]));
#endif
#ifdef G
	if (drv && drv->after_fork) drv->after_fork();
#endif
	if (terminal_pipe[1] != -1) EINTRLOOP(rs, close(terminal_pipe[1]));
	if (s_unix_fd != -1) close(s_unix_fd);
	foreach (t, terminals) {
		if (t->fdin > 0)
			EINTRLOOP(rs, close(t->fdin));
		if (t->handle_to_close >= 0)
			EINTRLOOP(rs, close(t->handle_to_close));
	}
	foreach (d, downloads) if (d->handle > 0)
		EINTRLOOP(rs, close(d->handle));
	foreach (c, queue) {
		if (c->sock1 >= 0) EINTRLOOP(rs, close(c->sock1));
		if (c->sock2 >= 0) EINTRLOOP(rs, close(c->sock2));
	}
	foreach (k, keepalive_connections)
		EINTRLOOP(rs, close(k->conn));
}


#if defined(WIN)

void get_path_to_exe(void)
{
	/* Standard method (argv[0]) doesn't work, if links is executed from
	   symlink --- it returns symlink name and cmd.exe is unable to start
	   it */
	unsigned r;
	static unsigned char path[4096];
	r = GetModuleFileNameA(NULL, cast_char path, sizeof path);
	if (r <= 0 || r >= sizeof path) {
		path_to_exe = g_argv[0];
		return;
	}
	path_to_exe = path;
}

#elif defined(OS2)

void get_path_to_exe(void)
{
	/* If you spawn links with quotation marks from cmd.exe,
	   the quotation marks will be present in g_argv[0] ... and will
	   prevent executing it */
	static unsigned char path[270];
	path_to_exe = g_argv[0];
	if (!os2_pib) return;
	if (DosQueryModuleName(os2_pib->pib_hmte, sizeof path, path)) return;
	path_to_exe = path;
}

static ULONG os2_old_type;
static HAB os2_hab;
static HMQ os2_hmq;

static int os2_init_pm(void)
{
	if (!os2_pib) goto err0;
	os2_old_type = os2_pib->pib_ultype;
	os2_pib->pib_ultype = 3;
	os2_hab = WinInitialize(0);
	if (os2_hab == NULLHANDLE) goto err1;
	os2_hmq = WinCreateMsgQueue(os2_hab, 0);
	if (os2_hmq == NULLHANDLE) goto err2;
	return 0;
err2:
	WinTerminate(os2_hab);
err1:
	os2_pib->pib_ultype = os2_old_type;
err0:
	return -1;
}

static void os2_exit_pm(void)
{
	WinDestroyMsgQueue(os2_hmq);
	WinTerminate(os2_hab);
	os2_pib->pib_ultype = os2_old_type;
}

int os_get_system_name(unsigned char *buffer)
{
	ULONG version[3];
	if (DosQuerySysInfo(QSV_VERSION_MAJOR, QSV_VERSION_REVISION, version, sizeof version))
		return -1;
	if (version[0] == 20) {
		version[0] = 2;
		if (version[1] == 10) {
			version[1] = 1;
		} else if (version[1] >= 30) {
			version[0] = version[1] / 10;
			version[1] %= 10;
		}
	}
	sprintf(cast_char buffer, "OS/2 %d.%d i386", (int)version[0], (int)version[1]);
	return 0;
}

#elif !defined(OPENVMS)

void get_path_to_exe(void)
{
	path_to_exe = g_argv[0];
}

#endif

void init_os_terminal(void)
{
#ifdef INTERIX
	/* Some sort of terminal bug in Interix, if we run xterm -e links,
	   terminal doesn't switch to raw mode, executing "stty sane" fixes it.
	   Don't do this workaround on console. */
	unsigned char *term = cast_uchar getenv("TERM");
	if (!term || strncasecmp(cast_const_char term, "interix", 7)) {
		system("stty sane 2>/dev/null");
	}
#endif
#ifdef OS2
	if (os2_detached) {
		fatal_exit("Links doesn't work in detached session");
	}
#endif
}

#ifdef INTERIX

static inline void cut_program_path(unsigned char *prog, unsigned char **prog_start, unsigned char **prog_end)
{
	while (WHITECHAR(*prog)) prog++;
	if (prog[0] == '"' || prog[0] == '\'') {
		*prog_start = prog + 1;
		*prog_end = cast_uchar strchr(cast_const_char(prog + 1), prog[0]);
		if (!*prog_end)
			*prog_end = cast_uchar strchr(cast_const_char prog, 0);
	} else {
		*prog_start = prog;
		*prog_end = prog + strcspn(cast_const_char prog, " ");
	}
}

static inline int is_windows_drive(unsigned char *prog_start, unsigned char *prog_end)
{
	if (prog_end - prog_start >= 3 && upcase(prog_start[0]) >= 'A' && upcase(prog_start[0]) <= 'Z' && prog_start[1] == ':')
		return 1;
	return 0;
}

static inline int is_windows_program(unsigned char *prog_start, unsigned char *prog_end)
{
	if (prog_end - prog_start > 4 && (
		!strncasecmp(cast_const_char(prog_end - 4), ".exe", 4) ||
		!strncasecmp(cast_const_char(prog_end - 4), ".bat", 4)))
			return 1;
	return 0;
}

#endif

#if defined(WIN) && defined(HAVE_CYGWIN_CONV_PATH)

unsigned char *os_conv_to_external_path(unsigned char *file, unsigned char *prog)
{
	unsigned char *new_path;
	ssize_t sz;
	sz = cygwin_conv_path(CCP_POSIX_TO_WIN_A | CCP_ABSOLUTE, file, NULL, 0);
	if (sz < 0) return stracpy(file);
	new_path = mem_alloc(sz);
	sz = cygwin_conv_path(CCP_POSIX_TO_WIN_A | CCP_ABSOLUTE, file, new_path, sz);
	if (sz < 0) {
		mem_free(new_path);
		return stracpy(file);
	}
	return new_path;
}

#elif defined(WIN) && defined(HAVE_CYGWIN_CONV_TO_FULL_WIN32_PATH)

unsigned char *os_conv_to_external_path(unsigned char *file, unsigned char *prog)
{
#ifdef MAX_PATH
	unsigned char new_path[MAX_PATH];
#else
	unsigned char new_path[1024];
#endif
	*new_path = 0;
	cygwin_conv_to_full_win32_path(cast_const_char file, cast_char new_path);
	if (!*new_path) return stracpy(file);
	return stracpy(new_path);
}

#elif defined(WIN) && defined(HAVE_UWIN_PATH)

unsigned char *os_conv_to_external_path(unsigned char *file, unsigned char *prog)
{
	unsigned char *new_path;
	ssize_t sz, sz2;
	sz = uwin_path(file, NULL, 0);
	if (sz < 0) return stracpy(file);
	new_path = mem_alloc(sz + 1);
	sz2 = uwin_path(file, new_path, sz + 1);
	if (sz2 < 0 || sz2 > sz) {
		mem_free(new_path);
		return stracpy(file);
	}
	return new_path;
}

#elif defined(INTERIX) && defined(HAVE_UNIXPATH2WIN)

unsigned char *os_conv_to_external_path(unsigned char *file, unsigned char *prog)
{
	unsigned char *prog_start, *prog_end;
	cut_program_path(prog, &prog_start, &prog_end);
	/* Convert path only if the program has ".exe" or ".bat" extension */
	if (is_windows_program(prog_start, prog_end)) {
#ifdef MAX_PATH
		unsigned char new_path[MAX_PATH];
#else
		unsigned char new_path[512];
#endif
		unsigned char *newstr;
		int newstrl;
		unsigned char *p;
		if (unixpath2win(file, 0, new_path, sizeof(new_path)))
			goto copy_path;
		/*return stracpy(new_path);*/
		newstr = init_str();
		newstrl = 0;
		for (p = new_path; *p; p++) {
			/*
			 * Unix shell hates backslash and Windows applications
			 * accept '/'
			 */
			if (*p == '\\') add_to_str(&newstr, &newstrl, cast_uchar "/");
			else add_chr_to_str(&newstr, &newstrl, *p);
		}
		return newstr;
	}
	copy_path:
	return stracpy(file);
}

#elif defined(DOS)

unsigned char *os_conv_to_external_path(unsigned char *file, unsigned char *prog)
{
	unsigned char *f, *x;
	f = stracpy(file);
	x = f;
	while ((x = cast_uchar strchr(cast_const_char x, '/'))) *x = '\\';
	return f;
}


#else

unsigned char *os_conv_to_external_path(unsigned char *file, unsigned char *prog)
{
	return stracpy(file);
}

#endif

#if defined(INTERIX) && defined(HAVE_WINPATH2UNIX)

unsigned char *os_fixup_external_program(unsigned char *prog)
{
	unsigned char *prog_start, *prog_end;
	cut_program_path(prog, &prog_start, &prog_end);
	if (is_windows_drive(prog_start, prog_end)) {
#ifdef MAX_PATH
		unsigned char new_path[MAX_PATH];
#else
		unsigned char new_path[1024];
#endif
		unsigned char *newstr;
		int newstrl;
		unsigned char *xpath;
		if (is_windows_program(prog_start, prog_end)) {
			/*
			 * There is some bug in Interix. Executing Win32
			 * binaries works from the console but doesn't work
			 * from xterm. So we prepend "cmd /c" to the program
			 * as a workaround.
			 */
			newstr = init_str();
			newstrl = 0;
			add_to_str(&newstr, &newstrl, cast_uchar "cmd /c ");
			add_to_str(&newstr, &newstrl, prog);
			return newstr;
		}
		xpath = memacpy(prog_start, prog_end - prog_start);
		if (winpath2unix(xpath, 0, new_path, sizeof(new_path))) {
			mem_free(xpath);
			goto copy_prog;
		}
		mem_free(xpath);
		newstr = init_str();
		newstrl = 0;
		add_bytes_to_str(&newstr, &newstrl, prog, prog_start - prog);
		add_to_str(&newstr, &newstrl, new_path);
		add_to_str(&newstr, &newstrl, prog_end);
		return newstr;
	}
	copy_prog:
	return stracpy(prog);
}

#else

unsigned char *os_fixup_external_program(unsigned char *prog)
{
	return stracpy(prog);
}

#endif


#if defined(UNIX) || defined(INTERIX) || defined(BEOS) || defined(RISCOS) || defined(ATHEOS) || defined(SPAD) || defined(OPENVMS) || defined(DOS)

#if defined(BEOS) && defined(HAVE_SETPGID)

int exe(unsigned char *path, int fg)
{
	pid_t p, rp;
	int s, rs;
	EINTRLOOP(p, fork());
	if (!p) {
		EINTRLOOP(rs, setpgid(0, 0));
		system(path);
		_exit(0);
	}
	if (p > 0) {
		EINTRLOOP(rp, waitpid(p, &s, 0));
	} else {
		system(path);
		return rs;
	}
	return 0;
}

#else

/* UNIX */
int exe(unsigned char *path, int fg)
{
#ifdef OPENVMS
	if (!strcmp(cast_const_char path, DEFAULT_SHELL))
		path = cast_uchar "";
#endif
#ifndef EXEC_IN_THREADS
	do_signal(SIGPIPE, SIG_DFL);
#ifdef SIGXFSZ
	do_signal(SIGXFSZ, SIG_DFL);
#endif
#ifdef SIGTSTP
	do_signal(SIGTSTP, SIG_DFL);
#endif
#ifdef SIGCONT
	do_signal(SIGCONT, SIG_DFL);
#endif
#ifdef SIGWINCH
	do_signal(SIGWINCH, SIG_DFL);
#endif
#endif
#ifdef G
	if (F && drv->exec) return drv->exec(path, fg);
#endif
	return system(cast_const_char path);
}

#endif

/* clipboard -> links */
unsigned char *get_clipboard_text(struct terminal *term)
{
#ifdef G
	if (F && drv->get_clipboard_text) {
		return drv->get_clipboard_text();
	}
#endif
	return stracpy(clipboard);
}

/* links -> clipboard */
void set_clipboard_text(struct terminal *term, unsigned char *data)
{
#ifdef G
	if (F && drv->set_clipboard_text) {
		drv->set_clipboard_text(term->dev, data);
		return;
	}
#endif
	if (clipboard) mem_free(clipboard);
	clipboard = stracpy(data);
}

int clipboard_support(struct terminal *term)
{
#ifdef G
	if (F && drv->set_clipboard_text) {
		return 1;
	}
#endif
	return 0;
}

void set_window_title(unsigned char *title)
{
	/* !!! FIXME */
}

unsigned char *get_window_title(void)
{
	/* !!! FIXME */
	return NULL;
}

int resize_window(int x, int y)
{
	return -1;
}

#elif defined(WIN)

int is_winnt(void)
{
	OSVERSIONINFO v;
	v.dwOSVersionInfoSize = sizeof v;
	if (!GetVersionEx(&v)) return 0;
	return v.dwPlatformId >= VER_PLATFORM_WIN32_NT;
}

static void close_handles(int keep_output)
{
	int i, rs;
	for (i = 0; i < FD_SETSIZE; i++) {
		if (keep_output && (i == 1 || i == 2)) continue;
		EINTRLOOP(rs, close(i));
	}
	EINTRLOOP(rs, open("nul", O_RDONLY));
	if (!keep_output) {
		EINTRLOOP(rs, open("nul", O_WRONLY));
		EINTRLOOP(rs, open("nul", O_WRONLY));
	}
}

#define WIN32_START_STRING	"start /wait "

int exe(unsigned char *path, int fg)
{
	/* This is very tricky. We must have exactly 3 arguments, the first
	   one shell and the second one "/c", otherwise Cygwin would quote
	   the arguments and trash them */
	int ct = 0;
	unsigned char buffer[1024];
	unsigned char buffer2[1024];
	size_t want_alloc;
	pid_t pid, rp;
#ifndef _UWIN
	int rs;
#endif
	unsigned char *x1;
	unsigned char *arg;
	x1 = cast_uchar GETSHELL;
	if (!x1) x1 = cast_uchar DEFAULT_SHELL;

	want_alloc = strlen(cast_const_char WIN32_START_STRING) + 3 + strlen(cast_const_char path) + 1;
#ifdef _UWIN
	want_alloc += strlen(cast_const_char x1) + 4;
	want_alloc *= 2;
#endif

	arg = malloc(want_alloc);
	if (!arg) return -1;
	*arg = 0;
#ifdef _UWIN
	strcat(cast_char arg, cast_const_char x1);
	strcat(cast_char arg, " /c ");
#endif
	strcat(cast_char arg, cast_const_char WIN32_START_STRING);
	if (*path == '"' && is_winnt()) strcat(cast_char arg, "\"\" ");
	strcat(cast_char arg, cast_const_char path);
	if (!is_winnt()) ct = GetConsoleTitleA(cast_char buffer, sizeof buffer);
#if defined(_UWIN) && !defined(__DMC__)
	{
		unsigned char *q1 = arg, *q2 = arg;
		while (*q1) {
			if (*q1 == '\\') q2++;
			q2++;
			q1++;
		}
		while (1) {
			*q2 = *q1;
			if (*q1 == '\\') {
				q2--;
				*q2 = '\\';
			}
			if (q1 == arg) break;
			q1--;
			q2--;
		}
	}
	/* UWin corrupts heap if we use threads and fork */
	fd_lock();
	pid = spawnl("/bin/sh", "/bin/sh", "-c", arg, NULL);
	fd_unlock();
#else
#if 1		/* spawn breaks mouse, do this only in graphics mode */
	if (F && is_winnt()) {
		/* spawn crashes on Win98 */
		fd_lock();
		spawnlp(_P_WAIT, cast_const_char x1, cast_const_char x1, "/c", cast_const_char arg, NULL);
		fd_unlock();
		/*FreeConsole();*/
		goto free_ret;
	} else
#endif
	{
		EINTRLOOP(pid, fork());
		if (!pid) {
	/* Win98 crashes if we spawn command.com and have some sockets open */
			close_handles(0);
			EINTRLOOP(rs, execlp(cast_const_char x1, cast_const_char x1, "/c", cast_const_char arg, NULL));
			_exit(1);
		}
	}
#endif
	if (!is_winnt()) {
		sleep(1);
		if (ct && GetConsoleTitleA(cast_char buffer2, sizeof buffer2) && !casecmp(buffer2, cast_uchar "start", 5)) {
			SetConsoleTitleA(cast_const_char buffer);
		}
	}
	if (pid != -1) EINTRLOOP(rp, waitpid(pid, NULL, 0));
	goto free_ret;

	free_ret:
	free(arg);
	return 0;
}

int exe_on_background(unsigned char *path, unsigned char *del)
{
#ifdef __CYGWIN__
	unsigned char *x1;
	unsigned char *arg;
	unsigned char *delx;
	int use_create_process = 0;
#if CYGWIN_VERSION_API_MAJOR > 1 || CYGWIN_VERSION_API_MINOR >= 154
	if (is_winnt()) {
		use_create_process = 1;
		if (cygwin_internal(CW_SYNC_WINENV))
			use_create_process = 0;
	}
#endif

	if (!is_winnt()) {
		if (del && *del)
			return -1;
	}

	x1 = cast_uchar GETSHELL;
	if (!x1) x1 = cast_uchar DEFAULT_SHELL;

	arg = stracpy(cast_uchar "");
	if (use_create_process) {
		add_to_strn(&arg, x1);
		add_to_strn(&arg, cast_uchar " /c ");
	}
	add_to_strn(&arg, cast_uchar WIN32_START_STRING);
	if (is_winnt()) if (*path == '"') add_to_strn(&arg, cast_uchar "\"\" ");
	add_to_strn(&arg, path);
	if (del && *del) {
		add_to_strn(&arg, cast_uchar " & ");
		add_to_strn(&arg, cast_uchar "del \"");
		delx = os_conv_to_external_path(del, path);
		add_to_strn(&arg, delx);
		mem_free(delx);
		add_to_strn(&arg, cast_uchar "\"");
	}

	/*debug("'%s'", arg);*/

	if (use_create_process) {
		PROCESS_INFORMATION pi;
		STARTUPINFO si;
		memset(&pi, 0, sizeof pi);
		memset(&si, 0, sizeof si);
		si.cb = sizeof si;
		fd_lock();
		if (CreateProcessA(cast_char x1, cast_char arg, NULL, NULL, FALSE, CREATE_NO_WINDOW | (is_winnt() ? DETACHED_PROCESS : 0), NULL, NULL, &si, &pi)) {
			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread);
		}
		fd_unlock();
	} else {
	/* We need to fork here so that we can close handles */
		pid_t pid;
		/*int rs;*/
		EINTRLOOP(pid, fork());
		if (!pid) {
			close_handles(0);
			/*EINTRLOOP(rs, execlp(cast_const_char x1, cast_const_char x1, "/c", cast_const_char arg, NULL));*/
			spawnlp(_P_DETACH, cast_const_char x1, cast_const_char x1, "/c", cast_const_char arg, NULL);
			_exit(1);
		}
	}

	mem_free(arg);
	return 0;
#else
	return -1;
#endif
}

unsigned char *get_clipboard_text(struct terminal *term)
{
	unsigned char buffer[256];
	unsigned char *str, *s, *d;
	int l;
	int r;
	int rs;
	int h;
	if (!clipboard_support(term)) return stracpy(clipboard);
	/* O_TEXT doesn't work on clipboard handle */
	h = c_open(cast_uchar "/dev/clipboard", O_RDONLY);
	if (h == -1) return stracpy(clipboard);
	str = init_str();
	l = 0;
	/* Don't use hard_read because UWin has buggy end-of-file signalling.
	   It resets the position to the beginning after signalling eof. */
	while (1) {
		EINTRLOOP(r, (int)read(h, buffer, sizeof buffer));
		if (r <= 0) break;
		add_bytes_to_str(&str, &l, buffer, r);
	}
	EINTRLOOP(rs, close(h));
	for (s = str, d = str; *s; s++)
		if (!(s[0] == '\r' && s[1] == '\n')) *d++ = *s;
	*d = 0;
	return str;
}

/* Putting Czech characters to clipboard doesn't work, but it should be fixed
   rather in Cygwin than here */
void set_clipboard_text(struct terminal *term, unsigned char *data)
{
	unsigned char *conv_data;
	int l;
	int h;
	int rs;
	if (clipboard) mem_free(clipboard);
	clipboard = stracpy(data);
	if (!clipboard_support(term)) return;
	/* O_TEXT doesn't work on clipboard handle */
	h = c_open(cast_uchar "/dev/clipboard", O_WRONLY);
	if (h == -1) return;
	conv_data = init_str();
	l = 0;
	for (; *data; data++)
		if (*data == '\n') add_to_str(&conv_data, &l, cast_uchar "\r\n");
		else add_chr_to_str(&conv_data, &l, *data);
	hard_write(h, conv_data, l);
	mem_free(conv_data);
	EINTRLOOP(rs, close(h));
}

int clipboard_support(struct terminal *term)
{
	struct stat st;
	int rs;
	EINTRLOOP(rs, stat("/dev/clipboard", &st));
	return !rs && S_ISCHR(st.st_mode);
}

int get_windows_cp(int cons)
{
	unsigned char str[8];
	int cp, idx;
	if (cons && is_winnt())
		cp = GetConsoleOutputCP();
	else
		cp = GetACP();
	if (cp <= 0 || cp >= 100000) return 0;
	if (cp == 874) cp = 28605;
	if (cp >= 28591 && cp <= 28605) {
		sprintf(cast_char str, "8859-%d", cp - 28590);
	} else {
		sprintf(cast_char str, "%d", cp);
	}
	if ((idx = get_cp_index(str)) < 0) return 0;
	return idx;
}

void set_window_title(unsigned char *title)
{
	unsigned char *t, *p;
	struct conv_table *ct;
	if (!title) return;
	if (is_xterm()) return;
	ct = get_translation_table(utf8_table, get_windows_cp(1));
	t = convert_string(ct, title, strlen(cast_const_char title), NULL);
	for (p = cast_uchar strchr(cast_const_char t, 1); p; p = cast_uchar strchr(cast_const_char(p + 1), 1))
		*p = ' ';
	SetConsoleTitleA(cast_const_char t);
	mem_free(t);
}

unsigned char *get_window_title(void)
{
	struct conv_table *ct;
	int r;
	unsigned char buffer[1024];
	if (is_xterm()) return NULL;
	if (!(r = GetConsoleTitleA(cast_char buffer, sizeof buffer))) return NULL;
	ct = get_translation_table(get_windows_cp(1), utf8_table);
	return convert_string(ct, buffer, r, NULL);
}

static void call_resize(unsigned char *x1, int x, int y)
{
	pid_t pid, rp;
#ifndef _UWIN
	int rs;
#endif
	unsigned char arg[MAX_STR_LEN];
#ifdef _UWIN
	x++;
#endif
	sprintf(cast_char arg, "mode %d,%d", x, y);
#if defined(_UWIN) && !defined(__DMC__)
	pid = spawnlp(x1, x1, "/c", arg, NULL);
#else
#if 0		/* spawn breaks mouse, don't use this */
	if (is_winnt()) {
		/* spawn crashes on Win98 */
		fd_lock();
		spawnlp(_P_WAIT, x1, x1, "/c", arg, NULL);
		fd_unlock();
		return;
	} else
#endif
	{
		EINTRLOOP(pid, fork());
		if (!pid) {
	/* Win98 crashes if we spawn command.com and have some sockets open */
			close_handles(1);
			EINTRLOOP(rs, execlp(cast_const_char x1, cast_const_char x1, "/c", cast_const_char arg, NULL));
			_exit(1);
		}
	}
#endif
	if (pid != -1) EINTRLOOP(rp, waitpid(pid, NULL, 0));
}

int resize_window(int x, int y)
{
	int old_x, old_y;
	int ct = 0, fullscreen = 0;
	unsigned char buffer[1024];
	unsigned char *x1;
	if (is_xterm()) return -1;
	if (get_terminal_size(1, &old_x, &old_y)) return -1;
	x1 = cast_uchar GETSHELL;
	if (!x1) x1 = cast_uchar DEFAULT_SHELL;
	if (!is_winnt()) {
		ct = GetConsoleTitleA(cast_char buffer, sizeof buffer);
	}

	call_resize(x1, x, y);
	if (!is_winnt()) {
		int new_x, new_y;
	/* If we resize console on Win98 in fullscreen mode, it won't be
	   notified by Cygwin (it is valid for all Cygwin apps). So we must
	   switch to windowed mode, resize it again (twice, because resizing
	   to the same size won't have an effect) and switch back to full-screen
	   mode. */
	/* I'm not sure what's the behavior on WinNT 4. Anybody wants to test?
	   */
		if (!fullscreen && !get_terminal_size(1, &new_x, &new_y) && (new_x != x || new_y != y)) {
			fullscreen = 1;
#ifdef __CYGWIN__
			keybd_event(VK_MENU, 0x38, 0, 0);
			keybd_event(VK_RETURN, 0x1c, 0, 0);
			keybd_event(VK_RETURN, 0x1c, KEYEVENTF_KEYUP, 0);
			keybd_event(VK_MENU, 0x38, KEYEVENTF_KEYUP, 0);
#endif
			if (y != 25) call_resize(x1, 80, 25);
			else call_resize(x1, 80, 50);
			call_resize(x1, x, y);
			if (get_terminal_size(1, &new_x, &new_y) || new_x != x || new_y != y) call_resize(x1, old_x, old_y);
#ifdef __CYGWIN__
			keybd_event(VK_MENU, 0x38, 0, 0);
			keybd_event(VK_RETURN, 0x1c, 0, 0);
			keybd_event(VK_RETURN, 0x1c, KEYEVENTF_KEYUP, 0);
			keybd_event(VK_MENU, 0x38, KEYEVENTF_KEYUP, 0);
#endif
		}
		if (ct) SetConsoleTitleA(cast_const_char buffer);
	}
	return 0;
}

#elif defined(OS2)

int exe(unsigned char *path, int fg)
{
	int flags = P_SESSION;
	pid_t pid, rs;
	int ret;
#ifdef G
	int old0 = 0, old1 = 1, old2 = 2;
#endif
	unsigned char *shell;
	if (!(shell = GETSHELL)) shell = DEFAULT_SHELL;
	if (is_xterm()) flags |= P_BACKGROUND;
#ifdef G
	if (F) {
		old0 = c_dup(0);
		old1 = c_dup(1);
		old2 = c_dup(2);
		fd_lock();
		if (old0 >= 0) EINTRLOOP(rs, close(0));
		if (old1 >= 0) EINTRLOOP(rs, close(1));
		if (old2 >= 0) EINTRLOOP(rs, close(2));
		if (old0 >= 0) EINTRLOOP(rs, open("con", O_RDONLY));
		if (old1 >= 0) EINTRLOOP(rs, open("con", O_WRONLY));
		if (old2 >= 0) EINTRLOOP(rs, open("con", O_WRONLY));
	} else
#endif
	{
		fd_lock();
	}
	pid = spawnlp(flags, shell, shell, "/c", path, NULL);
#ifdef G
	if (F) {
		if (old0 >= 0) EINTRLOOP(rs, dup2(old0, 0));
		if (old1 >= 0) EINTRLOOP(rs, dup2(old1, 1));
		if (old2 >= 0) EINTRLOOP(rs, dup2(old2, 2));
		if (old0 >= 0) EINTRLOOP(rs, close(old0));
		if (old1 >= 0) EINTRLOOP(rs, close(old1));
		if (old2 >= 0) EINTRLOOP(rs, close(old2));
	}
#endif
	fd_unlock();
	if (pid != -1) EINTRLOOP(rs, waitpid(pid, &ret, 0));
	else ret = -1;
	return ret;
}

unsigned char *get_clipboard_text(struct terminal *term)
{
	unsigned char *ret = NULL;

	if (os2_init_pm()) return NULL;

	if (WinOpenClipbrd(os2_hab)) {
		ULONG fmtInfo = 0;

		if (WinQueryClipbrdFmtInfo(os2_hab, CF_TEXT, &fmtInfo)!=FALSE)
		{
			ULONG selClipText = WinQueryClipbrdData(os2_hab, CF_TEXT);

			if (selClipText) {
				unsigned char *u;
				PCHAR pchClipText = (PCHAR)selClipText;
				ret = stracpy(pchClipText);
				while ((u = cast_uchar strchr(cast_const_char ret, 13))) memmove(u, u + 1, strlen(cast_const_char(u + 1)) + 1);
			}
		}

		WinCloseClipbrd(os2_hab);
	}

#ifdef G
	if (F && ret) {
		static int cp = -1;
		struct conv_table *ct;
		unsigned char *d;
		if (cp == -1) {
			int c = WinQueryCp(os2_hmq);
			unsigned char a[8];
			snprintf(cast_char a, sizeof a, "%d", c);
			if ((cp = get_cp_index(a)) < 0 || cp == utf8_table) cp = 0;
		}
		ct = get_translation_table(cp, utf8_table);
		d = convert_string(ct, ret, strlen(cast_const_char ret), NULL);
		mem_free(ret);
		ret = d;
	}
#endif

	os2_exit_pm();

	return ret;
}

void set_clipboard_text(struct terminal *term, unsigned char *data)
{
	unsigned char *d = NULL;

	if (os2_init_pm()) return;

#ifdef G
	if (F) {
		static int cp = -1;
		struct conv_table *ct;
		unsigned char *p;
		if (cp == -1) {
			int c = WinQueryCp(os2_hmq);
			unsigned char a[8];
			snprintf(cast_char a, sizeof a, "%d", c);
			if ((cp = get_cp_index(a)) < 0 || cp == utf8_table) cp = 0;
		}
		ct = get_translation_table(utf8_table, cp);
		d = convert_string(ct, data, strlen(cast_const_char data), NULL);
		for (p = cast_uchar strchr(cast_const_char d, 1); p; p = cast_uchar strchr(cast_const_char(p + 1), 1))
			*p = ' ';
		data = d;
	}
#endif
	if (WinOpenClipbrd(os2_hab)) {
		PVOID pvShrObject = NULL;
		if (DosAllocSharedMem(&pvShrObject, NULL, strlen(cast_const_char data)+1, PAG_COMMIT | PAG_READ | PAG_WRITE | OBJ_GIVEABLE) == NO_ERROR) {
			strcpy(cast_char pvShrObject, cast_const_char data);
			WinEmptyClipbrd(os2_hab);
			WinSetClipbrdData(os2_hab, (ULONG)pvShrObject, CF_TEXT, CFI_POINTER);
		}
		WinCloseClipbrd(os2_hab);
	}

	os2_exit_pm();

	if (d) mem_free(d);
}

int clipboard_support(struct terminal *term)
{
	return 1;
}

unsigned char *get_window_title(void)
{
#ifndef OS2_DEBUG
	unsigned char *win_title = NULL;
	SWCNTRL swData;

	memset(&swData, 0, sizeof swData);
	if (os2_switchhandle != NULLHANDLE && !WinQuerySwitchEntry(os2_switchhandle, &swData)) {
		swData.szSwtitle[MAXNAMEL - 1] = 0;
		win_title = stracpy(swData.szSwtitle);
		if (swData.hwnd != NULLHANDLE && !os2_init_pm()) {
			LONG len = WinQueryWindowTextLength(swData.hwnd);
			if (len > 0) {
				mem_free(win_title);
				win_title = mem_alloc(len + 1);
				win_title[0] = 0;
				WinQueryWindowText(swData.hwnd, len + 1, win_title);
				win_title[len] = 0;
			}
			os2_exit_pm();
		}
	}

	return win_title;
#else
	return NULL;
#endif
}

void set_window_title(unsigned char *title)
{
#ifndef OS2_DEBUG
	SWCNTRL swData;

	if (!title) return;

	memset(&swData, 0, sizeof swData);
	if (os2_switchhandle != NULLHANDLE && !WinQuerySwitchEntry(os2_switchhandle, &swData)) {
		safe_strncpy(swData.szSwtitle, title, MAXNAMEL);
		WinChangeSwitchEntry(os2_switchhandle, &swData);
		if (swData.hwnd != NULLHANDLE && !os2_init_pm()) {
			WinSetWindowText(swData.hwnd, title);
			os2_exit_pm();
		}
	}
#endif
}

static tcount resize_count = 0;

int resize_window(int x, int y)
{
	int xfont, yfont;
	A_DECL(VIOMODEINFO, vmi);
	SWCNTRL swData;

	resize_count++;
	if (is_xterm()) return -1;
	vmi->cb = sizeof(*vmi);
	if (VioGetMode(vmi, 0)) return -1;
	vmi->col = x;
	vmi->row = y;
	/*debug("%d %d %d", vmi->buf_length, vmi->full_length, vmi->partial_length);*/
	for (xfont = 9; xfont >= 8; xfont--)
		for (yfont = 16; yfont >= 8; yfont--) {
			vmi->hres = x * xfont;
			vmi->vres = y * yfont;
			if (vmi->vres <= 400) vmi->vres = 400;
			else if (vmi->vres <= 480) vmi->vres = 480;
			vmi->buf_length = vmi->full_length = vmi->partial_length = x * ((vmi->vres + yfont - 1) / yfont) * 2;
			vmi->full_length = (vmi->full_length + 4095) & ~4095;
			vmi->partial_length = (vmi->partial_length + 4095) & ~4095;
			if (!VioSetMode(vmi, 0)) goto resized;
		}
	return -1;

	resized:
	memset(&swData, 0, sizeof swData);
	if (os2_switchhandle != NULLHANDLE && !WinQuerySwitchEntry(os2_switchhandle, &swData) && swData.hwnd != NULLHANDLE && !os2_init_pm()) {
		SWP swp;
		if (WinQueryWindowPos(swData.hwnd, &swp) && !(swp.fl & (SWP_MAXIMIZE | SWP_MINIMIZE | SWP_HIDE))) {
			const int expand = 16383;
			WinSetWindowPos(swData.hwnd, NULLHANDLE, swp.x, swp.y - expand, swp.cx + expand, swp.cy + expand, SWP_MOVE | SWP_SIZE);
		}
		os2_exit_pm();
	}
	return 0;
}

#endif

/* Threads */

#if (defined(HAVE_BEGINTHREAD) && defined(OS2)) || defined(BEOS) || defined(HAVE_PTHREADS) || defined(HAVE_ATHEOS_THREADS_H)

struct tdata {
	void (*fn)(void *, int);
	int h;
	int counted;
	unsigned char data[1];
};

static void bgt(void *t_)
{
	struct tdata *t = t_;
	int rs;
	ignore_signals();
	t->fn(t->data, t->h);
	EINTRLOOP(rs, (int)write(t->h, "x", 1));
	EINTRLOOP(rs, close(t->h));
	free(t);
}

#ifdef HAVE_ATHEOS_THREADS_H
#include <atheos/threads.h>
static uint32 bgat(void *t)
{
	bgt(t);
	return 0;
}
#endif

#endif

#if defined(UNIX) || defined(OS2) || defined(WIN) || defined(INTERIX) || defined(RISCOS) || defined(ATHEOS) || defined(SPAD)

void terminate_osdep(void)
{
}

#endif

#ifndef BEOS

void block_stdin(void) {}
void unblock_stdin(void) {}

#endif

#if defined(BEOS)

#include <be/kernel/OS.h>

static int thr_sem_init = 0;
static sem_id thr_sem;

static struct list_head active_threads = { &active_threads, &active_threads };

struct active_thread {
	struct active_thread *next;
	struct active_thread *prev;
	thread_id tid;
	void (*fn)(void *);
	void *data;
};

static int32 started_thr(void *data)
{
	struct active_thread *thrd = data;
	thrd->fn(thrd->data);
	if (acquire_sem(thr_sem) < B_NO_ERROR) return 0;
	del_from_list(thrd);
	free(thrd);
	release_sem(thr_sem);
	return 0;
}

int start_thr(void (*fn)(void *), void *data, unsigned char *name)
{
	struct active_thread *thrd;
	int tid;
	if (!thr_sem_init) {
		if ((thr_sem = create_sem(0, "thread_sem")) < B_NO_ERROR) return -1;
		thr_sem_init = 1;
	} else if (acquire_sem(thr_sem) < B_NO_ERROR) return -1;
	retry:
	if (!(thrd = malloc(sizeof(struct active_thread)))) {
		if (out_of_memory(0, NULL, 0))
			goto retry;
		goto err1;
	}
	thrd->fn = fn;
	thrd->data = data;
	if ((tid = thrd->tid = spawn_thread(started_thr, name, B_NORMAL_PRIORITY, thrd)) < B_NO_ERROR)
		goto err2;
	resume_thread(thrd->tid);
	add_to_list(active_threads, thrd);
	release_sem(thr_sem);
	return tid;

	err2:
	free(thrd);
	err1:
	release_sem(thr_sem);
	return -1;
}

void terminate_osdep(void)
{
	struct list_head *p;
	struct active_thread *thrd;
	if (acquire_sem(thr_sem) < B_NO_ERROR) return;
	foreach(thrd, active_threads) kill_thread(thrd->tid);
	while ((p = active_threads.next) != &active_threads) {
		del_from_list(p);
		free(p);
	}
	release_sem(thr_sem);
}

int start_thread(void (*fn)(void *, int), void *ptr, int l, int counted)
{
	int p[2];
	struct tdata *t;
	int rs;
	if (c_pipe(p) < 0) return -1;
	retry:
	if (!(t = malloc(sizeof(struct tdata) + l))) {
		if (out_of_memory(0, NULL, 0))
			goto retry;
		goto err1;
	}
	t->fn = fn;
	t->h = p[1];
	t->counted = counted;
	memcpy(t->data, ptr, l);
	if (start_thr(bgt, t, cast_uchar "links_thread") < 0)
		goto err2;
	return p[0];

	err2:
	free(t);
	err1:
	EINTRLOOP(rs, close(p[0]));
	EINTRLOOP(rs, close(p[1]));
	return -1;
}


#elif defined(HAVE_BEGINTHREAD) && defined(OS2)

int start_thread(void (*fn)(void *, int), void *ptr, int l, int counted)
{
	int p[2];
	struct tdata *t;
	int rs;
	if (c_pipe(p) < 0) return -1;
	retry:
	if (!(t = malloc(sizeof(struct tdata) + l))) {
		if (out_of_memory(0, NULL, 0))
			goto retry;
		goto err1;
	}
	t->fn = fn;
	t->h = p[1];
	t->counted = counted;
	memcpy(t->data, ptr, l);
	if (_beginthread(bgt, NULL, 65536, t) == -1)
		goto err2;
	return p[0];

	err2:
	free(t);
	err1:
	EINTRLOOP(rs, close(p[0]));
	EINTRLOOP(rs, close(p[1]));
	return -1;
}

#ifdef HAVE__READ_KBD

static int tp = -1;
static int ti = -1;

static void input_thread(void *p)
{
	unsigned char c[2];
	int h = (int)p;
	int rs;
	ignore_signals();
	while (1) {
	   /* for the records:
		 _read_kbd(0, 1, 1) will
		 read a char, don't echo it, wait for one available and
		 accept CTRL-C.
		 Knowing that, I suggest we replace this call completly!
	    */
		*c = _read_kbd(0, 1, 1);
		EINTRLOOP(rs, (int)write(h, c, 1));
	}
	EINTRLOOP(rs, close(h));
}
#endif /* #ifdef HAVE__READ_KBD */

#if defined(HAVE_MOUOPEN) && defined(HAVE_BEGINTHREAD) && !defined(USE_GPM)

#define USING_OS2_MOUSE

static int mouse_h = -1;

struct os2_mouse_spec {
	int p[2];
	void (*fn)(void *, unsigned char *, int);
	void *data;
	unsigned char buffer[sizeof(struct links_event)];
	int bufptr;
	int terminate;
};

#define MOU_EMULATE_CURSOR

#ifdef MOU_EMULATE_CURSOR
static int mouse_x = -1, mouse_y = -1;
static unsigned char mouse_attr;
#endif

static void mouse_remove_pointer(void)
{
#ifndef MOU_EMULATE_CURSOR
	A_DECL(NOPTRRECT, pa);
	static int x = -1, y = -1;
	static tcount c = -1;
	if (x == -1 || y == -1 || (c != resize_count)) get_terminal_size(1, &x, &y), c = resize_count;
	pa->row = 0;
	pa->col = 0;
	pa->cRow = y - 1;
	pa->cCol = x - 1;
	MouRemovePtr(pa, mouse_h);
#else
	if (mouse_x >= 0 && mouse_y >= 0) {
		VioWrtNAttr(&mouse_attr, 1, mouse_y, mouse_x, 0);
	}
	mouse_x = -1, mouse_y = -1;
#endif
}

static void mouse_draw_pointer(int x, int y)
{
#ifndef MOU_EMULATE_CURSOR
	MouDrawPtr(mouse_h);
#else
	unsigned char str[4];
	USHORT str_len;
	unsigned char attr;
	unsigned char fg, bg;
	int r;
	if (!os2_full_screen)
		return;
	DosSetPriority(PRTYS_THREAD, PRTYC_TIMECRITICAL, 0, 0);
	if (mouse_x == x && mouse_y == y)
		return;
	mouse_remove_pointer();
	str_len = sizeof(str);
	r = VioReadCellStr(str, &str_len, y, x, 0);
	if (r || str_len < 2) return;
	mouse_attr = str[1];
	fg = mouse_attr & 0x07;
	bg = (mouse_attr & 0x70) >> 4;
	if (fg == bg) fg ^= 0x07, bg ^= 0x07;
	attr = (mouse_attr & 0x88) | (fg << 4) | bg;
	r = VioWrtNAttr(&attr, 1, y, x, 0);
	if (r) return;
	mouse_x = x, mouse_y = y, mouse_attr = str[1];
#endif
}

static void mouse_thread(void *p)
{
	int status;
	int rs;
	struct os2_mouse_spec *oms = p;
	A_DECL(HMOU, mh);
	A_DECL(MOUEVENTINFO, ms);
	A_DECL(USHORT, rd);
	A_DECL(USHORT, mask);
	struct links_event ev;
	ignore_signals();
	ev.ev = EV_MOUSE;
	if (MouOpen(NULL, mh)) goto ret;
	mouse_h = *mh;
	*mask = MOUSE_MOTION_WITH_BN1_DOWN | MOUSE_BN1_DOWN |
		MOUSE_MOTION_WITH_BN2_DOWN | MOUSE_BN2_DOWN |
		MOUSE_MOTION_WITH_BN3_DOWN | MOUSE_BN3_DOWN |
		MOUSE_MOTION;
	MouSetEventMask(mask, *mh);
	*rd = MOU_WAIT;
	status = -1;
	while (1) {
		/*int w, ww;*/
		if (MouReadEventQue(ms, rd, *mh)) break;
		fd_lock();
		if (!oms->terminate) mouse_draw_pointer(ms->col, ms->row);
		fd_unlock();
		ev.x = ms->col;
		ev.y = ms->row;
		/*debug("status: %d %d %d", ms->col, ms->row, ms->fs);*/
		if (ms->fs & (MOUSE_BN1_DOWN | MOUSE_BN2_DOWN | MOUSE_BN3_DOWN)) ev.b = status = B_DOWN | (ms->fs & MOUSE_BN1_DOWN ? B_LEFT : ms->fs & MOUSE_BN2_DOWN ? B_RIGHT : B_MIDDLE);
		else if (ms->fs & (MOUSE_MOTION_WITH_BN1_DOWN | MOUSE_MOTION_WITH_BN2_DOWN | MOUSE_MOTION_WITH_BN3_DOWN)) {
			int b = ms->fs & MOUSE_MOTION_WITH_BN1_DOWN ? B_LEFT : ms->fs & MOUSE_MOTION_WITH_BN2_DOWN ? B_RIGHT : B_MIDDLE;
			if (status == -1) b |= B_DOWN;
			else b |= B_DRAG;
			ev.b = status = b;
		}
		else {
			if (status == -1) continue;
			ev.b = (status & BM_BUTT) | B_UP;
			status = -1;
		}
		if (hard_write(oms->p[1], (unsigned char *)&ev, sizeof(struct links_event)) != sizeof(struct links_event)) break;
	}
	fd_lock();
	mouse_h = -1;
	MouClose(*mh);
	fd_unlock();
	ret:
	EINTRLOOP(rs, close(oms->p[1]));
	/*free(oms);*/
}

static void mouse_handle(struct os2_mouse_spec *oms)
{
	int r;
	EINTRLOOP(r, (int)read(oms->p[0], oms->buffer + oms->bufptr, sizeof(struct links_event) - oms->bufptr));
	if (r <= 0) {
		unhandle_mouse(oms);
		return;
	}
	if ((oms->bufptr += r) == sizeof(struct links_event)) {
		oms->bufptr = 0;
		oms->fn(oms->data, oms->buffer, sizeof(struct links_event));
	}
}

void *handle_mouse(int cons, void (*fn)(void *, unsigned char *, int), void *data)
{
	struct os2_mouse_spec *oms;
	if (is_xterm()) return NULL;
		/* This is never freed but it's allocated only once */
	retry:
	if (!(oms = malloc(sizeof(struct os2_mouse_spec)))) {
		if (out_of_memory(0, NULL, 0))
			goto retry;
		return NULL;
	}
	oms->fn = fn;
	oms->data = data;
	oms->bufptr = 0;
	oms->terminate = 0;
	if (c_pipe(oms->p)) {
		free(oms);
		return NULL;
	}
	if (_beginthread(mouse_thread, NULL, 0x10000, (void *)oms) == -1) {
	}
	set_handlers(oms->p[0], (void (*)(void *))mouse_handle, NULL, oms);
	return oms;
}

void unhandle_mouse(void *om)
{
	struct os2_mouse_spec *oms = om;
	want_draw();
	oms->terminate = 1;
	close_socket(&oms->p[0]);
	done_draw();
}

void want_draw(void)
{
	static int ansi = 0;
	fd_lock();
	if (!ansi) {
		VioSetAnsi(1, 0);
		ansi = 1;
	}
	if (mouse_h != -1) {
		mouse_remove_pointer();
	}
}

void done_draw(void)
{
	fd_unlock();
}

#endif /* if HAVE_MOUOPEN */

#elif defined(HAVE_PTHREADS)

#ifdef OPENVMS
#define THREAD_NEED_STACK_SIZE		65536
int vms_thread_high_priority = 0;
#endif

static unsigned thread_count = 0;

static inline void reset_thread_count(void)
{
	fd_lock();
	thread_count = 0;
	fd_unlock();
}

static void inc_thread_count(void)
{
	fd_lock();
	thread_count++;
	fd_unlock();
}

static void dec_thread_count(void)
{
	fd_lock();
	if (!thread_count)
		internal("thread_count underflow");
	thread_count--;
	fd_unlock();
}

static inline unsigned get_thread_count(void)
{
	unsigned val;
	fd_lock();
	val = thread_count;
	fd_unlock();
	return val;
}

static void *bgpt(void *t)
{
	int counted = ((struct tdata *)t)->counted;
	bgt(t);
	if (counted) dec_thread_count();
	return NULL;
}

int start_thread(void (*fn)(void *, int), void *ptr, int l, int counted)
{
	pthread_attr_t attr;
	pthread_t thread;
	struct tdata *t;
	int p[2];
	int rs;
	if (c_pipe(p) < 0) return -1;
	retry1:
	if (!(t = malloc(sizeof(struct tdata) + l))) {
		if (out_of_memory(0, NULL, 0))
			goto retry1;
		goto err1;
	}
	t->fn = fn;
	t->h = p[1];
	t->counted = counted;
	memcpy(t->data, ptr, l);
	retry2:
	if (pthread_attr_init(&attr)) {
		if (out_of_memory(0, NULL, 0))
			goto retry2;
		goto err2;
	}
	retry3:
	if (pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED)) {
		if (out_of_memory(0, NULL, 0))
			goto retry3;
		goto err3;
	}
#ifdef THREAD_NEED_STACK_SIZE
	retry4:
	if (pthread_attr_setstacksize(&attr, THREAD_NEED_STACK_SIZE)) {
		if (out_of_memory(0, NULL, 0))
			goto retry4;
		goto err3;
	}
#endif
#ifdef OPENVMS
	if (vms_thread_high_priority) {
		struct sched_param param;
		memset(&param, 0, sizeof param);
		if (pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED))
			goto err3;
		if (vms_thread_high_priority > 0) {
			if (pthread_attr_setschedpolicy(&attr, SCHED_FIFO))
				goto err3;
			param.sched_priority = PRI_FIFO_MIN;
			if (pthread_attr_setschedparam(&attr, &param))
				goto err3;
		} else {
			if (pthread_attr_setschedpolicy(&attr, SCHED_LFI_NP))
				goto err3;
			param.sched_priority = PRI_BG_MIN_NP;
			if (pthread_attr_setschedparam(&attr, &param))
				goto err3;
		}
	}
#endif
	if (counted) inc_thread_count();
	if (pthread_create(&thread, &attr, bgpt, t)) {
		if (counted) dec_thread_count();
		goto err3;
	}
	pthread_attr_destroy(&attr);
	return p[0];

	err3:
	pthread_attr_destroy(&attr);
	err2:
	free(t);
	err1:
	EINTRLOOP(rs, close(p[0]));
	EINTRLOOP(rs, close(p[1]));
	return -1;
}

#elif defined(HAVE_ATHEOS_THREADS_H) && defined(HAVE_SPAWN_THREAD) && defined(HAVE_RESUME_THREAD)

#include <atheos/threads.h>

int start_thread(void (*fn)(void *, int), void *ptr, int l, int counted)
{
	int p[2];
	int rs;
	thread_id f;
	struct tdata *t;
	if (c_pipe(p) < 0) return -1;
	retry:
	if (!(t = malloc(sizeof(struct tdata) + l))) {
		if (out_of_memory(0, NULL, 0))
			goto retry;
		goto err1;
	}
	t->fn = fn;
	t->h = p[1];
	t->counted = counted;
	memcpy(t->data, ptr, l);
	if ((f = spawn_thread("links_lookup", bgat, 0, 0, t)) == -1)
		goto err2;
	resume_thread(f);
	return p[0];

	err2:
	free(t);
	err1:
	EINTRLOOP(rs, close(p[0]));
	EINTRLOOP(rs, close(p[1]));
	return -1;
}

#elif defined(DOS)

int start_thread(void (*fn)(void *, int), void *ptr, int l, int counted)
{
	int p[2];
	int rs;
	if (c_pipe(p) < 0) return -1;
	fn(ptr, p[1]);
	EINTRLOOP(rs, close(p[1]));
	return p[0];
}

#else /* HAVE_BEGINTHREAD */

int start_thread(void (*fn)(void *, int), void *ptr, int l, int counted)
{
	int p[2];
	pid_t f;
	int rs;
	if (c_pipe(p) < 0) return -1;
	EINTRLOOP(f, fork());
	if (!f) {
		close_fork_tty();
		EINTRLOOP(rs, close(p[0]));
		fn(ptr, p[1]);
		EINTRLOOP(rs, (int)write(p[1], "x", 1));
		EINTRLOOP(rs, close(p[1]));
		_exit(0);
	}
	if (f == -1) {
		EINTRLOOP(rs, close(p[0]));
		EINTRLOOP(rs, close(p[1]));
		return -1;
	}
	EINTRLOOP(rs, close(p[1]));
	return p[0];
}

#endif

#if !defined(USING_OS2_MOUSE) && !defined(DOS)
void want_draw(void) {}
void done_draw(void) {}
#endif

int get_output_handle(void) { return 1; }

#if defined(OS2)

int get_ctl_handle(void) { return get_input_handle(); }

#else

int get_ctl_handle(void) { return 0; }

#endif

#if defined(BEOS)

#elif defined(HAVE_BEGINTHREAD) && defined(HAVE__READ_KBD)

int get_input_handle(void)
{
	int rs;
	int fd[2];
	if (ti != -1) return ti;
	if (is_xterm()) return 0;
	if (c_pipe(fd) < 0) return 0;
	ti = fd[0];
	tp = fd[1];
	if (_beginthread(input_thread, NULL, 0x10000, (void *)tp) == -1) {
		EINTRLOOP(rs, close(fd[0]));
		EINTRLOOP(rs, close(fd[1]));
		return 0;
	}
/*
#if defined(HAVE_MOUOPEN) && !defined(USE_GPM)
	_beginthread(mouse_thread, NULL, 0x10000, (void *)tp);
#endif
*/
	return fd[0];
}

#elif !defined(OPENVMS)

int get_input_handle(void)
{
	return 0;
}

#endif /* defined(HAVE_BEGINTHREAD) && defined(HAVE__READ_KBD) */


#ifdef USE_GPM

struct gpm_mouse_spec {
	int h;
	void (*fn)(void *, unsigned char *, int);
	void *data;
};

/* GPM installs its own signal handlers and we don't want them */

static sigset_t gpm_sigset;
static unsigned char gpm_sigset_valid;
#ifdef SIGWINCH
static struct sigaction gpm_winch;
static unsigned char gpm_winch_valid;
#endif
#ifdef SIGTSTP
static struct sigaction gpm_tstp;
static unsigned char gpm_tstp_valid;
#endif

static void save_gpm_signals(void)
{
	sigset_t sig;
	int rs;
	sigemptyset(&sig);
#ifdef SIGWINCH
	sigaddset(&sig, SIGWINCH);
#endif
#ifdef SIGTSTP
	sigaddset(&sig, SIGTSTP);
#endif
	EINTRLOOP(rs, do_sigprocmask(SIG_BLOCK, &sig, &gpm_sigset));
	gpm_sigset_valid = !rs;
#ifdef SIGWINCH
	EINTRLOOP(rs, sigaction(SIGWINCH, NULL, &gpm_winch));
	gpm_winch_valid = !rs;
#endif
#ifdef SIGTSTP
	EINTRLOOP(rs, sigaction(SIGTSTP, NULL, &gpm_tstp));
	gpm_tstp_valid = !rs;
#endif
}

static void restore_gpm_signals(void)
{
	int rs;
#ifdef SIGWINCH
	if (gpm_winch_valid)
		EINTRLOOP(rs, sigaction(SIGWINCH, &gpm_winch, NULL));
#endif
#ifdef SIGTSTP
	if (gpm_tstp_valid)
		EINTRLOOP(rs, sigaction(SIGTSTP, &gpm_tstp, NULL));
#endif
	if (gpm_sigset_valid)
		EINTRLOOP(rs, do_sigprocmask(SIG_SETMASK, &gpm_sigset, NULL));
}

static void gpm_mouse_in(struct gpm_mouse_spec *gms)
{
	int g;
	Gpm_Event gev;
	struct links_event ev;
	set_handlers(gms->h, NULL, NULL, NULL);
	save_gpm_signals();
	g = Gpm_GetEvent(&gev);
	restore_gpm_signals();
	if (g <= 0) {
		gms->h = -1;
		return;
	}
	set_handlers(gms->h, (void (*)(void *))gpm_mouse_in, NULL, gms);
	ev.ev = EV_MOUSE;
	ev.x = gev.x - 1;
	ev.y = gev.y - 1;
	if (ev.x < 0) ev.x = 0;
	if (ev.y < 0) ev.y = 0;
	if (gev.buttons & GPM_B_LEFT) ev.b = B_LEFT;
	else if (gev.buttons & GPM_B_MIDDLE) ev.b = B_MIDDLE;
	else if (gev.buttons & GPM_B_RIGHT) ev.b = B_RIGHT;
	else return;
	if ((int)gev.type & GPM_DOWN) ev.b |= B_DOWN;
	else if ((int)gev.type & GPM_UP) ev.b |= B_UP;
	else if ((int)gev.type & GPM_DRAG) ev.b |= B_DRAG;
	else return;
	gms->fn(gms->data, (unsigned char *)&ev, sizeof(struct links_event));
}

void *handle_mouse(int cons, void (*fn)(void *, unsigned char *, int), void *data)
{
	int h;
	Gpm_Connect conn;
	struct gpm_mouse_spec *gms;
	conn.eventMask = (unsigned short)~(unsigned)GPM_MOVE;
	conn.defaultMask = GPM_MOVE;
	conn.minMod = 0;
	conn.maxMod = 0;
	save_gpm_signals();
	h = Gpm_Open(&conn, cons);
	restore_gpm_signals();
	if (h < 0) return NULL;
	gms = mem_alloc(sizeof(struct gpm_mouse_spec));
	gms->h = h;
	gms->fn = fn;
	gms->data = data;
	set_handlers(h, (void (*)(void *))gpm_mouse_in, NULL, gms);
	return gms;
}

void unhandle_mouse(void *h)
{
	struct gpm_mouse_spec *gms = h;
	if (gms->h != -1) set_handlers(gms->h, NULL, NULL, NULL);
	save_gpm_signals();
	Gpm_Close();
	restore_gpm_signals();
	mem_free(gms);
}

void add_gpm_version(unsigned char **s, int *l)
{
	add_to_str(s, l, cast_uchar "GPM");
#ifdef HAVE_GPM_GETLIBVERSION
	add_to_str(s, l, cast_uchar " (");
	add_to_str(s, l, cast_uchar Gpm_GetLibVersion(NULL));
	add_to_str(s, l, cast_uchar ")");
#endif
}

#elif !defined(USING_OS2_MOUSE) && !defined(DOS)

void *handle_mouse(int cons, void (*fn)(void *, unsigned char *, int), void *data) { return NULL; }
void unhandle_mouse(void *data) { }

#endif /* #ifdef USE_GPM */


#if defined(WIN) || defined(INTERIX)

static int is_remote_connection(void)
{
	return !!getenv("SSH_CONNECTION");
}

#endif


#if defined(OS2)

int get_system_env(void)
{
	if (is_xterm()) return 0;
	return ENV_OS2VIO;		/* !!! FIXME: telnet */
}

#elif defined(BEOS)

int get_system_env(void)
{
	unsigned char *term = cast_uchar getenv("TERM");
	if (!term || (upcase(term[0]) == 'B' && upcase(term[1]) == 'E')) return ENV_BE;
	return 0;
}

#elif defined(WIN)

int get_system_env(void)
{
	if (is_xterm()) return 0;
	if (is_remote_connection()) return 0;
	return ENV_WIN32;
}

#elif defined(INTERIX)

#define INTERIX_START_COMMAND	"/usr/contrib/win32/bin/start"

int get_system_env(void)
{
	if (is_xterm()) return 0;
	if (is_remote_connection()) return 0;
	if (!access(INTERIX_START_COMMAND, X_OK)) return ENV_INTERIX;
	return 0;
}

#else

int get_system_env(void)
{
	return 0;
}

#endif

static void exec_new_links(struct terminal *term, unsigned char *xterm, unsigned char *exe, unsigned char *param)
{
	unsigned char *str;
	str = mem_alloc(strlen(cast_const_char xterm) + 1 + strlen(cast_const_char exe) + 1 + strlen(cast_const_char param) + 1);
	if (*xterm) sprintf(cast_char str, "%s %s %s", xterm, exe, param);
	else sprintf(cast_char str, "%s %s", exe, param);
	exec_on_terminal(term, str, cast_uchar "", 2);
	mem_free(str);
}

static int open_in_new_twterm(struct terminal *term, unsigned char *exe, unsigned char *param)
{
	unsigned char *twterm;
	if (!(twterm = cast_uchar getenv("LINKS_TWTERM"))) twterm = cast_uchar "twterm -e";
	exec_new_links(term, twterm, exe, param);
	return 0;
}

unsigned char *links_xterm(void)
{
	unsigned char *xterm;
	if (!(xterm = cast_uchar getenv("LINKS_XTERM"))) {
#ifdef OPENVMS
		xterm = cast_uchar "CREATE /TERMINAL /WAIT";
#else
		xterm = cast_uchar "xterm -e";
#endif
	}
	return xterm;
}

static int open_in_new_xterm(struct terminal *term, unsigned char *exe, unsigned char *param)
{
	exec_new_links(term, links_xterm(), exe, param);
	return 0;
}

static int open_in_new_screen(struct terminal *term, unsigned char *exe, unsigned char *param)
{
	exec_new_links(term, cast_uchar "screen", exe, param);
	return 0;
}

#ifdef OS2
static int open_in_new_vio(struct terminal *term, unsigned char *exe, unsigned char *param)
{
	unsigned char *x = stracpy(cast_uchar "\"");
	add_to_strn(&x, exe);
	add_to_strn(&x, cast_uchar "\"");
	exec_new_links(term, cast_uchar "start \"Links\" /c /f /win", x, param);
	mem_free(x);
	return 0;
}

static int open_in_new_fullscreen(struct terminal *term, unsigned char *exe, unsigned char *param)
{
	unsigned char *x = stracpy(cast_uchar "\"");
	add_to_strn(&x, exe);
	add_to_strn(&x, cast_uchar "\"");
	exec_new_links(term, cast_uchar "start \"Links\" /c /f /fs", x, param);
	mem_free(x);
	return 0;
}
#endif

#ifdef WIN
static int open_in_new_win32(struct terminal *term, unsigned char *exe, unsigned char *param)
{
	exec_new_links(term, cast_uchar "", exe, param);
	return 0;
}
#endif

#ifdef INTERIX
static int open_in_new_interix(struct terminal *term, unsigned char *exe, unsigned char *param)
{
	unsigned char *param_x = stracpy(param);
	add_to_strn(&param_x, cast_uchar "'");
	exec_new_links(term, cast_uchar(INTERIX_START_COMMAND " '\"Links\"' posix /u /c /bin/sh -c '"), exe, param_x);
	mem_free(param_x);
	return 0;
}
#endif

#ifdef BEOS
static int open_in_new_be(struct terminal *term, unsigned char *exe, unsigned char *param)
{
	exec_new_links(term, cast_uchar "Terminal", exe, param);
	return 0;
}
#endif

#ifdef G
static int open_in_new_g(struct terminal *term, unsigned char *exe, unsigned char *param)
{
	void *info;
	unsigned char *target = NULL;
	int len;
	int base = 0;
	unsigned char *url;
	if (!cmpbeg(param, cast_uchar "-base-session ")) {
		param = cast_uchar strchr(cast_const_char param, ' ') + 1;
		base = atoi(cast_const_char param);
		param += strcspn(cast_const_char param, " ");
		if (*param == ' ') param++;
	}
	if (!cmpbeg(param, cast_uchar "-target ")) {
		param = cast_uchar strchr(cast_const_char param, ' ') + 1;
		target = param;
		param += strcspn(cast_const_char param, " ");
		if (*param == ' ') *param++ = 0;
	}
	url = param;
	info = create_session_info(base, url, target, &len);
	return attach_g_terminal(term->cwd, info, len);
}
#endif

static struct {
	int env;
	int (*open_window_fn)(struct terminal *term, unsigned char *, unsigned char *);
	unsigned char *text;
	unsigned char *hk;
} oinw[] = {
	{ENV_XWIN, open_in_new_xterm, TEXT_(T_XTERM), TEXT_(T_HK_XTERM)},
	{ENV_TWIN, open_in_new_twterm, TEXT_(T_TWTERM), TEXT_(T_HK_TWTERM)},
	{ENV_SCREEN, open_in_new_screen, TEXT_(T_SCREEN), TEXT_(T_HK_SCREEN)},
#ifdef OS2
	{ENV_OS2VIO, open_in_new_vio, TEXT_(T_WINDOW), TEXT_(T_HK_WINDOW)},
	{ENV_OS2VIO, open_in_new_fullscreen, TEXT_(T_FULL_SCREEN), TEXT_(T_HK_FULL_SCREEN)},
#endif
#ifdef WIN
	{ENV_WIN32, open_in_new_win32, TEXT_(T_WINDOW), TEXT_(T_HK_WINDOW)},
#endif
#ifdef INTERIX
	{ENV_INTERIX, open_in_new_interix, TEXT_(T_WINDOW), TEXT_(T_HK_WINDOW)},
#endif
#ifdef BEOS
	{ENV_BE, open_in_new_be, TEXT_(T_BEOS_TERMINAL), TEXT_(T_HK_BEOS_TERMINAL)},
#endif
#ifdef G
	{ENV_G, open_in_new_g, TEXT_(T_WINDOW), TEXT_(T_HK_WINDOW)},
#endif
	{0, NULL, NULL, NULL}
};

struct open_in_new *get_open_in_new(int environment)
{
	int i;
	struct open_in_new *oin = DUMMY;
	int noin = 0;
	if (anonymous) return NULL;
	if (environment & ENV_G) environment = ENV_G;
	for (i = 0; oinw[i].env; i++) if ((environment & oinw[i].env) == oinw[i].env) {
		if ((unsigned)noin > MAXINT / sizeof(struct open_in_new) - 2) overalloc();
		oin = mem_realloc(oin, (noin + 2) * sizeof(struct open_in_new));
		oin[noin].text = oinw[i].text;
		oin[noin].hk = oinw[i].hk;
		oin[noin].open_window_fn = oinw[i].open_window_fn;
		noin++;
		oin[noin].text = NULL;
		oin[noin].hk = NULL;
		oin[noin].open_window_fn = NULL;
	}
	if (oin == DUMMY) return NULL;
	return oin;
}

int can_resize_window(struct terminal *term)
{
#if defined(OS2) || defined(WIN)
	if (!strncmp(cast_const_char term->term, "xterm", 5)) return 0;
	if (term->environment & (ENV_OS2VIO | ENV_WIN32)) return 1;
#endif
	return 0;
}

int can_open_os_shell(int environment)
{
#ifdef OS2
	if (environment & ENV_XWIN) return 0;
#endif
#ifdef WIN
	if (!F && !(environment & ENV_WIN32)) return 0;
#endif
#ifdef BEOS
	if (!(environment & ENV_BE)) return 0;
#endif
#ifdef G
	if (F && drv->flags & GD_NO_OS_SHELL) return 0;
#endif
	return 1;
}

void set_highpri(void)
{
#ifdef OS2
	DosSetPriority(PRTYS_PROCESS, PRTYC_FOREGROUNDSERVER, 0, 0);
#endif
}

#if !defined(DOS) && !defined(OPENVMS)
void os_seed_random(unsigned char **pool, int *pool_size)
{
	*pool = DUMMY;
	*pool_size = 0;
}
#endif

#if defined(WIN)
int os_send_fg_cookie(int h)
{
	DWORD pid;
	pid = GetCurrentProcessId();
	if (hard_write(h, (unsigned char *)&pid, sizeof pid) != sizeof pid)
		return -1;
	return 0;
}
int os_receive_fg_cookie(int h)
{
	DWORD pid;
	BOOL (WINAPI *fn_AllowSetForegroundWindow)(DWORD);
	if (hard_read(h, (unsigned char *)&pid, sizeof pid) != sizeof pid)
		return -1;
	fn_AllowSetForegroundWindow = (BOOL (WINAPI *)(DWORD))GetProcAddress(GetModuleHandleA("user32.dll"), "AllowSetForegroundWindow");
	if (fn_AllowSetForegroundWindow)
		fn_AllowSetForegroundWindow(pid);
	return 0;
}
#else
int os_send_fg_cookie(int h)
{
	return 0;
}
int os_receive_fg_cookie(int h)
{
	return 0;
}
#endif

void os_detach_console(void)
{
#if defined(WIN)
	if (is_winnt())
		FreeConsole();
#endif
#if !defined(NO_FORK_ON_EXIT)
	{
		pid_t rp;
	/* Intel and PathScale handle fork gracefully */
#if !defined(__ICC) && !defined(__PATHSCALE__)
		disable_openmp = 1;
#endif
		EINTRLOOP(rp, fork());
		if (!rp) {
			reinit_child();
#if defined(HAVE_PTHREADS)
			reset_thread_count();
#endif
		}
		if (rp > 0) {
#if defined(HAVE_PTHREADS)
			while (get_thread_count()) {
				portable_sleep(1000);
			}
#endif
			_exit(0);
		}
	}
#endif
}

#if defined(OS2) || defined(DOS)

int get_country_language(int c)
{
	static const struct {
		int code;
		unsigned char *language;
	} countries[] = {
		{ 1, cast_uchar "English" },
		{ 2, cast_uchar "French" },
		{ 3, cast_uchar "Spanish" },
		{ 4, cast_uchar "English" },
		{ 7, cast_uchar "Russian" },
		{ 27, cast_uchar "English" },
		{ 30, cast_uchar "Greek" },
		{ 31, cast_uchar "Dutch" },
		{ 32, cast_uchar "Dutch" },
		{ 33, cast_uchar "French" },
		{ 34, cast_uchar "Spanish" },
		{ 36, cast_uchar "Hungarian" },
		{ 38, cast_uchar "Serbian" },
		{ 39, cast_uchar "Italian" },
		{ 40, cast_uchar "Romanian" },
		{ 41, cast_uchar "Swiss German" },
		{ 42, cast_uchar "Czech" },
		{ 43, cast_uchar "German" },
		{ 44, cast_uchar "English" },
		{ 45, cast_uchar "Danish" },
		{ 46, cast_uchar "Swedish" },
		{ 47, cast_uchar "Norwegian" },
		{ 48, cast_uchar "Polish" },
		{ 49, cast_uchar "German" },
		{ 52, cast_uchar "Spanish" },
		{ 54, cast_uchar "Spanish" },
		{ 55, cast_uchar "Brazilian Portuguese" },
		{ 56, cast_uchar "Spanish" },
		{ 57, cast_uchar "Spanish" },
		{ 58, cast_uchar "Spanish" },
		{ 61, cast_uchar "English" },
		{ 64, cast_uchar "English" },
		{ 65, cast_uchar "English" },
		{ 90, cast_uchar "Turkish" },
		{ 99, cast_uchar "English" },
		{ 351, cast_uchar "Portuguese" },
		{ 353, cast_uchar "English" },
		{ 354, cast_uchar "Icelandic" },
		{ 358, cast_uchar "Finnish" },
		{ 359, cast_uchar "Bulgarian" },
		{ 371, cast_uchar "Lithuanian" },
		{ 372, cast_uchar "Estonian" },
		{ 381, cast_uchar "Serbian" },
		{ 384, cast_uchar "Croatian" },
		{ 385, cast_uchar "Croatian" },
#ifdef DOS
		{ 421, cast_uchar "Slovak" },
#else
		{ 421, cast_uchar "Czech" },
#endif
		{ 422, cast_uchar "Slovak" },
		{ 593, cast_uchar "Spanish" },
	};
	int idx, i;
#define C_EQUAL(a, b)	countries[a].code == (b)
#define C_ABOVE(a, b)	countries[a].code > (b)
	BIN_SEARCH(sizeof(countries) / sizeof(*countries), C_EQUAL, C_ABOVE, c, idx);
	if (idx == -1)
		return -1;
	for (i = 0; i < n_languages(); i++)
		if (!strcasecmp(cast_const_char language_name(i), cast_const_char countries[idx].language))
			return i;
	return -1;
}

#endif

#if defined(OS2)

int os_default_language(void)
{
	COUNTRYCODE cc;
	COUNTRYINFO ci;
	ULONG ul;
	int rc;
	memset(&cc, 0, sizeof cc);
	rc = DosQueryCtryInfo(sizeof ci, &cc, &ci, &ul);
	if (!rc)
		return get_country_language(ci.country);
	return -1;
}

#elif !defined(DOS)

int os_default_language(void)
{
	return -1;
}

#endif

#if defined(WIN) && defined(__CYGWIN__) && defined(HAVE_CYGWIN_CONV_PATH)

int os_default_charset(void)
{
	unsigned char *term = cast_uchar getenv("TERM");
	if (term && !casecmp(term, cast_uchar "cygwin", 6))
		return utf8_table;
	return -1;
}

#elif defined(OS2)

int os_default_charset(void)
{
	ULONG os2_cp[1];
	ULONG size = 0;
	int rc;
	rc = DosQueryCp(sizeof(os2_cp), os2_cp, &size);
	if ((!rc || rc == ERROR_CPLIST_TOO_SMALL) && size >= sizeof(ULONG)) {
		unsigned char a[8];
		int cp;
		snprintf(cast_char a, sizeof a, "%lu", os2_cp[0]);
		if ((cp = get_cp_index(a)) >= 0 && cp != utf8_table)
			return cp;
	}
	return 0;
}

#elif !defined(DOS)

int os_default_charset(void)
{
	return -1;
}

#endif
