#ifdef __DJGPP

#include "links.h"

#include <pc.h>
#include <dpmi.h>
#include <bios.h>
#include <go32.h>
#include <libc/dosio.h>
#include <sys/exceptn.h>
#include <sys/movedata.h>

#define VIRTUAL_PIPE_SIZE	512

#define DOS_MOUSE_FLUSH_TIME	300

#undef read
#undef write
#undef close
#undef select

/*
 * Asynchronous exits from signal handler cause a crash if they interrupt
 * networking in a wrong place. So, we use synchronous exit.
 */
static volatile unsigned char break_pressed = 0;
static volatile unsigned char break_exiting = 0;

void dos_poll_break(void)
{
	if (break_pressed && !break_exiting) {
		break_exiting = 1;
		fatal_exit("Exiting on Ctrl+Break");
	}
}

static void sigbreak(int sig)
{
	break_pressed = 1;
}

static unsigned screen_x = 80;
static unsigned screen_y = 25;

int get_terminal_size(int fd, int *x, int *y)
{
	*x = screen_x = ScreenCols();
	*y = screen_y = ScreenRows();
	return 0;
}

void handle_terminal_resize(int fd, void (*fn)(void))
{
}

void unhandle_terminal_resize(int fd)
{
}

static size_t init_seq_len;

static unsigned char screen_initialized = 0;
static unsigned char *screen_backbuffer = NULL;
static int screen_backbuffer_x;
static int screen_backbuffer_y;
static int saved_cursor_x;
static int saved_cursor_y;
static unsigned char screen_default_attr;

static unsigned cursor_x;
static unsigned cursor_y;
static unsigned current_attr;

static unsigned char dos_mouse_initialized = 0;
static unsigned char dos_mouse_buttons;

static int dos_mouse_last_x;
static int dos_mouse_last_y;
static int dos_mouse_last_button;
static ttime dos_mouse_time;

static struct links_event *dos_mouse_queue = DUMMY;
static int dos_mouse_queue_n;

static void (*txt_mouse_handler)(void *, unsigned char *, int) = NULL;
static void *txt_mouse_data;

static int dos_mouse_coord(int v)
{
	if (!F) v /= 8;
	return v;
}

static void dos_mouse_show(void)
{
	if (dos_mouse_initialized && !F) {
		__dpmi_regs r;
		memset(&r, 0, sizeof r);
		r.x.ax = 1;
		__dpmi_int(0x33, &r);
	}
}

static void dos_mouse_hide(void)
{
	if (dos_mouse_initialized && !F) {
		__dpmi_regs r;
		memset(&r, 0, sizeof r);
		r.x.ax = 2;
		__dpmi_int(0x33, &r);
	}
}

static void dos_mouse_init(unsigned x, unsigned y)
{
	__dpmi_regs r;
	memset(&r, 0, sizeof r);
	__dpmi_int(0x33, &r);
	if (r.x.ax != 0xffff) return;
	dos_mouse_buttons = r.x.bx == 3 ? 3 : 2;
	dos_mouse_initialized = 1;
	memset(&r, 0, sizeof r);
	r.x.ax = 7;
	r.x.cx = 0;
	r.x.dx = x - 1;
	__dpmi_int(0x33, &r);
	memset(&r, 0, sizeof r);
	r.x.ax = 8;
	r.x.cx = 0;
	r.x.dx = y - 1;
	__dpmi_int(0x33, &r);
	memset(&r, 0, sizeof r);
	r.x.ax = 3;
	__dpmi_int(0x33, &r);
	dos_mouse_last_x = dos_mouse_coord(r.x.cx);
	dos_mouse_last_y = dos_mouse_coord(r.x.dx);
	dos_mouse_last_button = r.x.bx;
	dos_mouse_time = get_time();
}

void dos_mouse_terminate(void)
{
	mem_free(dos_mouse_queue);
	dos_mouse_queue = DUMMY;
	dos_mouse_queue_n = 0;
	dos_mouse_hide();
}

static void dos_mouse_enqueue(int x, int y, int b)
{
	if (dos_mouse_queue_n && ((b & BM_ACT) == B_DRAG || (b & BM_ACT) == B_MOVE) && (b & BM_ACT) == (dos_mouse_queue[dos_mouse_queue_n - 1].b & BM_ACT)) {
		dos_mouse_queue_n--;
		goto set_last;
	}
	dos_mouse_queue = mem_realloc(dos_mouse_queue, (dos_mouse_queue_n + 1) * sizeof(struct links_event));
set_last:
	dos_mouse_queue[dos_mouse_queue_n].ev = EV_MOUSE;
	dos_mouse_queue[dos_mouse_queue_n].x = x;
	dos_mouse_queue[dos_mouse_queue_n].y = y;
	dos_mouse_queue[dos_mouse_queue_n].b = b;
	dos_mouse_queue_n++;
}

static int dos_mouse_button(int b)
{
	switch (b) {
		default:
		case 0:	return B_LEFT;
		case 1: return B_RIGHT;
		case 2: return B_MIDDLE;
	}
}

static void dos_mouse_poll(void)
{
	int i;
	int cx, cy;
	__dpmi_regs r;
	dos_poll_break();
	if (dos_mouse_initialized) {
		if (dos_mouse_initialized == 1 && (uttime)get_time() - (uttime)dos_mouse_time > DOS_MOUSE_FLUSH_TIME)
			dos_mouse_initialized = 2;
		for (i = 0; i < dos_mouse_buttons; i++) {
			if (dos_mouse_initialized == 1 && i > 0) {
				memset(&r, 0, sizeof r);
				r.x.ax = 5;
				r.x.bx = i;
				__dpmi_int(0x33, &r);
				memset(&r, 0, sizeof r);
				r.x.ax = 6;
				r.x.bx = i;
				__dpmi_int(0x33, &r);
				continue;
			}
			memset(&r, 0, sizeof r);
			r.x.ax = !(dos_mouse_last_button & (1 << i)) ? 5 : 6;
			r.x.bx = i;
			__dpmi_int(0x33, &r);
px:
			if (r.x.bx) {
				dos_mouse_last_x = dos_mouse_coord(r.x.cx);
				dos_mouse_last_y = dos_mouse_coord(r.x.dx);
				dos_mouse_last_button ^= 1 << i;
				dos_mouse_enqueue(dos_mouse_last_x, dos_mouse_last_y, dos_mouse_button(i) | (dos_mouse_last_button & (1 << i) ? B_DOWN : B_UP));
				/*printf("%s %d %d\n", dos_mouse_last_button & (1 << i) ? "press" : "release", r.x.cx, r.x.dx);*/
				if (!((dos_mouse_last_button ^ r.x.ax) & (1 << i))) {
					memset(&r, 0, sizeof r);
					r.x.ax = !(dos_mouse_last_button & (1 << i)) ? 5 : 6;
					r.x.bx = i;
					__dpmi_int(0x33, &r);
					if ((dos_mouse_last_button ^ r.x.ax) & (1 << i))
						goto px;
				}
			}
		}
		memset(&r, 0, sizeof r);
		r.x.ax = 3;
		__dpmi_int(0x33, &r);
		cx = dos_mouse_coord(r.x.cx);
		cy = dos_mouse_coord(r.x.dx);
		if (cx != dos_mouse_last_x || cy != dos_mouse_last_y) {
			for (i = 0; i < dos_mouse_buttons; i++)
				if (dos_mouse_last_button & (1 << i)) {
					dos_mouse_enqueue(cx, cy, B_DRAG | dos_mouse_button(i));
					goto x;
				}
			if (F) dos_mouse_enqueue(cx, cy, B_MOVE);
x:
			dos_mouse_last_x = cx;
			dos_mouse_last_y = cy;
		}
	}
#if defined(G) && defined(GRDRV_GRX)
	if (grx_mouse_initialized) {
		grx_mouse_poll();
	}
#endif
}

void *handle_mouse(int cons, void (*fn)(void *, unsigned char *, int), void *data)
{
	dos_mouse_init(screen_backbuffer_x * 8, screen_backbuffer_y * 8);
	if (!dos_mouse_initialized) return NULL;
	dos_mouse_show();
	txt_mouse_handler = fn;
	txt_mouse_data = data;

	return (void *)1;
}

void unhandle_mouse(void *data)
{
	dos_mouse_terminate();
	txt_mouse_handler = NULL;
}

void want_draw(void)
{
	dos_mouse_hide();
}

void done_draw(void)
{
	dos_mouse_show();
}

static int dos_mouse_event(void)
{
	if (dos_mouse_queue_n) {
		if (!F && txt_mouse_handler) {
			struct links_event *q = dos_mouse_queue;
			int ql = dos_mouse_queue_n;
			dos_mouse_queue = DUMMY;
			dos_mouse_queue_n = 0;
			txt_mouse_handler(txt_mouse_data, (unsigned char *)(void *)q, ql * sizeof(struct links_event));
			mem_free(q);
			return 1;
		}
	}
#if defined(G) && defined(GRDRV_GRX)
	if (grx_mouse_initialized) {
		return grx_mouse_event();
	}
#endif
	return 0;
}

void dos_save_screen(void)
{
	unsigned char *sc;
	screen_backbuffer_x = ScreenCols();
	screen_backbuffer_y = ScreenRows();
	screen_default_attr = ScreenAttrib;
	if (screen_backbuffer) {
		sc = screen_backbuffer;
		screen_backbuffer = NULL;
		mem_free(sc);
	}
	sc = mem_alloc(2 * screen_backbuffer_x * screen_backbuffer_y);
	ScreenRetrieve(sc);
	ScreenGetCursor(&saved_cursor_y, &saved_cursor_x);
	screen_backbuffer = sc;
}

void dos_restore_screen(void)
{
	if (screen_backbuffer) {
		unsigned char *sc;
		if (ScreenCols() == screen_backbuffer_x && ScreenRows() == screen_backbuffer_y) {
			ScreenUpdate(screen_backbuffer);
			ScreenSetCursor(saved_cursor_y, saved_cursor_x);
		}
		sc = screen_backbuffer;
		screen_backbuffer = NULL;
		mem_free(sc);
	}
}

static void ansi_initialize(void)
{
	if (screen_initialized)
		return;

	dos_save_screen();

	ScreenClear();
	cursor_x = 0;
	cursor_y = 0;
	current_attr = screen_default_attr;
	screen_initialized = 1;
}

static void ansi_terminate(void)
{
	if (!screen_initialized)
		return;

	ScreenSetCursor(0, 0);
	dos_restore_screen();
	screen_initialized = 0;
}

static unsigned ansi2pc(unsigned c)
{
	return ((c & 4) >> 2) | (c & 2) | ((c & 1) << 2);
}

static void ansi_write(const unsigned char *str, size_t size)
{
	if (!screen_initialized) {
		if (size >= init_seq_len && !memcmp(str, init_seq, init_seq_len)) {
			ansi_initialize();
			goto process_ansi;
		}
		write(1, str, size);
		return;
	}
process_ansi:
	while (size--) {
		unsigned char c = *str++;
		unsigned char buffer[128];
		unsigned buf_size;
		unsigned clen;
		unsigned x, y;
		switch (c) {
			case 7:
				continue;
			case 10:
				cursor_y++;
			case 13:
				cursor_x = 0;
				break;
			case 27:
				if (!size--) goto ret;
				switch (*str++) {
					case ')':
						if (!size--) goto ret;
						str++;
					case '7':
					default:
						continue;
					case '8':
						ansi_terminate();
						goto ret2;
					case '[':
						clen = 0;
						while (1) {
							unsigned char u;
							if (clen >= size) goto ret;
							u = upcase(str[clen]);
							if (u >= 'A' && u <= 'Z')
								break;
							clen++;
						}
				}
				if (clen >= sizeof(buffer)) goto ret;
				memcpy(buffer, str, clen);
				buffer[clen] = 0;
				switch (str[clen]) {
					case 'J':
						if (!strcmp(cast_const_char buffer, "2"))
							ScreenClear();
						break;
					case 'H':
						if (sscanf(cast_const_char buffer, "%u;%u", &y, &x) == 2) {
							cursor_x = x - 1;
							cursor_y = y - 1;
						}
						break;
					case 'm':
						while (buffer[0] >= '0' && buffer[0] < '9') {
							unsigned long a;
							unsigned char *e;
							a = strtoul(cast_const_char buffer, (char **)(void *)&e, 10);
							if (*e == ';') e++;
							memmove(buffer, e, strlen(cast_const_char e) + 1);
							switch (a) {
								case 0:
									current_attr = screen_default_attr;
									break;
								case 1:
									current_attr |= 0x08;
									break;
								case 7:
									current_attr = ((screen_default_attr & 0x70) >> 4) | ((screen_default_attr & 0x07) << 4);
									break;
								case 30: case 31: case 32: case 33: case 34: case 35: case 36: case 37:
									current_attr = (current_attr & 0x78) | ansi2pc(a - 30);
									break;
								case 40: case 41: case 42: case 43: case 44: case 45: case 46: case 47:
									current_attr = (current_attr & 0x0f) | (ansi2pc(a - 40) << 4);
									break;
							}
						}
						break;
				}
				str += clen + 1;
				size -= clen + 1;
				continue;
			default:
				buffer[0] = c;
				buf_size = 1;
				while (size && *str >= ' ' && buf_size < sizeof(buffer) - 1) {
					buffer[buf_size++] = *str++;
					size--;
				}
				buffer[buf_size] = 0;
				ScreenPutString(cast_const_char buffer, current_attr, cursor_x, cursor_y);
				cursor_x += buf_size;
				break;
		}
	}
ret:
	ScreenSetCursor(cursor_y, cursor_x);
ret2:;
}

#ifdef DOS_EXTRA_KEYBOARD

static short dos_buffered_char = -1;

static int dos_select_keyboard(void)
{
	int bk;
	if (dos_buffered_char >= 0) return 1;
	bk = bioskey(0x11);
	return !!bk;
}

static int dos_read_keyboard(void *buf, size_t size)
{
	int k;
	if (!size) return 0;
	if (dos_buffered_char >= 0) {
		*(unsigned char *)buf = dos_buffered_char;
		dos_buffered_char = -1;
		return 1;
	}
	k = getkey();
	if (!k) return 0;
	/*printf("returned key %04x\n", k);*/
	if (k < 0x100) {
		*(unsigned char *)buf = k;
		return 1;
	} else {
		*(unsigned char *)buf = 0;
		if (size >= 2) {
			*((unsigned char *)buf + 1) = k;
			return 2;
		} else {
			dos_buffered_char = k & 0xff;
			return 1;
		}
	}
}

#endif

static inline void pipe_lock(void)
{
}

static inline void pipe_unlock(void)
{
}

static inline void pipe_unlock_wait(void)
{
}

static inline void pipe_wake(void)
{
}

#include "vpipe.inc"

int c_pipe(int *fd)
{
	int r = vpipe_create(fd);
	/*printf("c_pipe: (%d) : %d,%d\n", r, fd[0], fd[1]);*/
	return r;
}

void set_nonblock(int fd)
{
	int rs;
	EINTRLOOP(rs, fcntl(fd, F_SETFL, O_NONBLOCK));
}

int dos_read(int fd, void *buf, size_t size)
{
	int r;
	dos_mouse_poll();
	r = vpipe_read(fd, buf, size);
	/*printf("dos_read(%d,%d) : %d,%d\n", r, errno, fd, size);*/
	if (r != -2) return r;
#ifdef DOS_EXTRA_KEYBOARD
	if (fd == 0) return dos_read_keyboard(buf, size);
#endif
	return read(fd, buf, size);
}

int dos_write(int fd, const void *buf, size_t size)
{
	int r;
	dos_mouse_poll();
	r = vpipe_write(fd, buf, size);
	/*printf("dos_write(%d,%d) : %d,%d\n", r, errno, fd, size);*/
	if (r != -2) return r;
	if (fd == 1) {
		ansi_write(buf, size);
		return size;
	}
	return write(fd, buf, size);
}

int dos_close(int fd)
{
	int r;
	r = vpipe_close(fd);
	if (r != -2) return r;
	return close(fd);
}

int dos_select(int n, fd_set *rs, fd_set *ws, fd_set *es, struct timeval *t, int from_main_loop)
{
	int i;
	int last_pass = 0;
	int ret_cnt = 0;

	int r;
	fd_set rsb, wsb, esb;
	struct timeval xtime;

	dos_mouse_poll();

	pipe_lock();
	for (i = 0; i < n; i++) {
		int ts = 0;
		if (rs && FD_ISSET(i, rs)) {
			int signaled = 0;
			if (pipe_desc[i]) {
				if (vpipe_may_read(i))
					signaled = 1;
				else
					FD_CLR(i, rs);
				/*printf("dos_select_read(%d) : %d\n", i, vpipe_may_read(i));*/
			} else {
			}
			if (!last_pass) {
				if (signaled) {
					clear_inactive(rs, i);
					clear_inactive(ws, i);
					clear_inactive(es, i);
					last_pass = 1;
				}
			} else {
				if (!signaled) FD_CLR(i, rs);
			}
			ts |= signaled;
		}
		if (ws && FD_ISSET(i, ws)) {
			int signaled = 0;
			if (pipe_desc[i]) {
				if (vpipe_may_write(i))
					signaled = 1;
				else
					FD_CLR(i, ws);
				/*printf("dos_select_write(%d) : %d\n", i, vpipe_may_write(i));*/
			} else {
			}
			if (!last_pass) {
				if (signaled) {
					clear_inactive(rs, i + 1);
					clear_inactive(ws, i);
					clear_inactive(es, i);
					last_pass = 1;
				}
			} else {
				if (!signaled) FD_CLR(i, ws);
			}
			ts |= signaled;
		}
		if (es && FD_ISSET(i, es)) {
			int signaled = 0;
			if (pipe_desc[i]) {
				FD_CLR(i, es);
			} else {
			}
			if (!last_pass) {
				if (signaled) {
					clear_inactive(rs, i + 1);
					clear_inactive(ws, i + 1);
					clear_inactive(es, i);
					last_pass = 1;
				}
			} else {
				if (!signaled) FD_CLR(i, es);
			}
			ts |= signaled;
		}
		if (last_pass) ret_cnt += ts;
	}
	pipe_unlock();
	if (last_pass) {
		/*printf("ret_cnt: %d\n", ret_cnt);*/
		return ret_cnt;
	}
	/*printf("real select\n");*/
	/*return select(n, rs, ws, es, t);*/
	if (rs) rsb = *rs;
	else FD_ZERO(&rsb);
	if (ws) wsb = *ws;
	else FD_ZERO(&wsb);
	if (es) esb = *es;
	else FD_ZERO(&esb);
	if (t) {
		EINTRLOOP(r, gettimeofday(&xtime, NULL));
		if (r) fatal_exit("gettimeofday failed: %d", errno);
		xtime.tv_usec += t->tv_usec;
		if (xtime.tv_usec >= 1000000) {
			xtime.tv_usec -= 1000000;
			xtime.tv_sec++;
		}
		xtime.tv_sec += t->tv_sec;
	}
	while (1) {
		struct timeval zero = { 0, 0 };
		struct timeval now;
#ifdef DOS_EXTRA_KEYBOARD
		if (rs && FD_ISSET(0, rs)) {
			if (dos_select_keyboard()) {
				FD_ZERO(rs);
				FD_SET(0, rs);
				if (ws) FD_ZERO(ws);
				if (es) FD_ZERO(es);
				if (from_main_loop && dos_mouse_event())
					check_bottom_halves();
				return 1;
			}
			FD_CLR(0, rs);
		}
#endif
		if (from_main_loop && dos_mouse_event()) {
			check_bottom_halves();
			return 0;
		}
		r = select(n, rs, ws, es, &zero);
		if (r == -1 && errno == EBADF) {
			/* DJGPP occasionally returns EBADF */
			int re = 0;
			if (rs) FD_ZERO(rs);
			if (ws) FD_ZERO(ws);
			if (es) FD_ZERO(es);
			i = 0;
#ifdef DOS_EXTRA_KEYBOARD
			i++;
#endif
			for (; i < n; i++) {
				fd_set rsx;
				fd_set wsx;
				fd_set esx;
				int x, s;
				if (!FD_ISSET(i, &rsb) &&
				    !FD_ISSET(i, &wsb) &&
				    !FD_ISSET(i, &esb))
					continue;
				FD_ZERO(&rsx);
				FD_ZERO(&wsx);
				FD_ZERO(&esx);
				if (FD_ISSET(i, &rsb)) FD_SET(i, &rsx);
				if (FD_ISSET(i, &wsb)) FD_SET(i, &wsx);
				if (FD_ISSET(i, &esb)) FD_SET(i, &esx);
				zero.tv_sec = 0;
				zero.tv_usec = 0;
				x = select(i + 1, &rsx, &wsx, &esx, &zero);
				s = 0;
				if (FD_ISSET(i, &rsb) && (x < 0 || (x > 0 && FD_ISSET(i, &rsx)))) FD_SET(i, rs), s = 1;
				if (FD_ISSET(i, &wsb) && (x < 0 || (x > 0 && FD_ISSET(i, &wsx)))) FD_SET(i, ws), s = 1;
				if (FD_ISSET(i, &esb) && (x < 0 || (x > 0 && FD_ISSET(i, &esx)))) FD_SET(i, es), s = 1;
				if (s) re++;
			}
			if (re) return re;
			r = 0;
		}
		if (r) return r;
		EINTRLOOP(r, gettimeofday(&now, NULL));
		if (r) fatal_exit("gettimeofday failed: %d", errno);
		if (t) {
			if (now.tv_sec > xtime.tv_sec ||
			   (now.tv_sec == xtime.tv_sec && now.tv_usec >= xtime.tv_usec))
				return 0;
		}
		if (rs) *rs = rsb;
		if (ws) *ws = wsb;
		if (es) *es = esb;
		dos_mouse_poll();
		__dpmi_yield();
		dos_mouse_poll();
	}
}

#ifdef DOS_EXTRA_KEYBOARD

int setraw(int ctl, int save)
{
	__djgpp_set_ctrl_c(0);
	return 0;
}

void setcooked(int ctl)
{
}

#endif

#define RANDOM_POOL_SIZE	65536

void os_seed_random(unsigned char **pool, int *pool_size)
{
	unsigned *random_pool, *tmp_pool;
	int a, i;
	random_pool = mem_alloc(RANDOM_POOL_SIZE);
	tmp_pool = mem_alloc(RANDOM_POOL_SIZE);
	for (a = 0; a <= 640 * 1024 - RANDOM_POOL_SIZE; a += RANDOM_POOL_SIZE) {
		dosmemget(a, RANDOM_POOL_SIZE, tmp_pool);
		for (i = 0; i < RANDOM_POOL_SIZE / 4; i++)
			random_pool[i] += tmp_pool[i];
	}
	mem_free(tmp_pool);
	*pool = (unsigned char *)(void *)random_pool;
	*pool_size = RANDOM_POOL_SIZE;
}

void init_os(void)
{
	int s, rs;
	struct sigaction sa;

	init_seq_len = strlen(cast_const_char init_seq);

	/* preload the packet driver */
	s = c_socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (s >= 0) {
		EINTRLOOP(rs, close(s));
	}

	tcp_cbreak(1);

	memset(&sa, 0, sizeof sa);
	sa.sa_handler = sigbreak;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	EINTRLOOP(rs, sigaction(SIGINT, &sa, NULL));
}

void terminate_osdep(void)
{
	if (screen_backbuffer)
		mem_free(screen_backbuffer);
}

int os_default_language(void)
{
	__dpmi_regs r;
	memset(&r, 0, sizeof r);
	r.x.ax = 0x3800;
	r.x.dx = __tb_offset;
	r.x.ds = __tb_segment;
	__dpmi_int(0x21, &r);
	if (!(r.x.flags & 1)) {
		return get_country_language(r.x.bx);
	}
	return -1;
}

int os_default_charset(void)
{
	__dpmi_regs r;
	memset(&r, 0, sizeof r);
	r.x.ax = 0x6601;
	__dpmi_int(0x21, &r);
	if (!(r.x.flags & 1)) {
		unsigned char a[8];
		int cp;
		snprintf(cast_char a, sizeof a, "%d", r.x.bx);
		if ((cp = get_cp_index(a)) >= 0 && cp != utf8_table)
			return cp;
	}
	return 0;
}

#else

typedef int dos_c_no_empty_unit;

#endif
