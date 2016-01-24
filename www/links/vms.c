#ifdef __VMS

#include "links.h"

#define __NEW_STARLET 1
#include <starlet.h>
#include <iodef.h>
#include <ssdef.h>
#include <stsdef.h>
#include <descrip.h>
#include <dvidef.h>
#include <lib$routines.h>
#include <libclidef.h>

#if defined(HAVE_OPENSSL) && !defined(OPENSSL_NO_SHA) && !defined(OPENSSL_NO_SHA1)
#include <openssl/sha.h>
#define USE_SHA
#endif

#ifdef __VAX
struct _iosb {
	unsigned short iosb$w_status;
	unsigned short iosb$w_bcnt;
	unsigned iosb$l_dev_depend;
};
struct _generic_64 {
	unsigned gen64$l_longword[2];
};
#else
#include <iosbdef.h>
#include <gen64def.h>
#endif

#include <pthread.h>
#include <tis.h>
#ifdef HAVE_SYS_IOCTL_H
#include <sys/ioctl.h>
#endif

/* reimplement pipes because sockets + select have too big latency */
#define VMS_VIRTUAL_PIPE

/* use a special thread for select processing (if not defined, sockets
   are polled with a dynamic timer) */
#define VMS_SELECT_THREAD

#define VIRTUAL_PIPE_SIZE		512

#define GETTIMEOFDAY_POOL		256

/*#define TEST_WAKE_BUG*/
/*#define TRACE_PIPES*/

#undef read
#undef write
#undef close
#undef select

#if !defined(VMS_VIRTUAL_PIPE) && defined(VMS_SELECT_THREAD)
#undef VMS_SELECT_THREAD
#endif

static void vms_fatal_exit(char *m, ...)
{
	va_list l;
	va_start(l, m);
	fprintf(stderr, "\n");
	vfprintf(stderr, cast_const_char m, l);
	fprintf(stderr, "%c\n", (unsigned char)7);
	fflush(stderr);
	va_end(l);
	exit(RET_FATAL);
}

void portable_sleep(unsigned msec)
{
	struct timespec tv;
	int rs;
	tv.tv_sec = msec / 1000;
	tv.tv_nsec = msec % 1000 * 1000000;
	if (!msec) tv.tv_nsec = 1;
	rs = pthread_delay_np(&tv);
	if (rs && rs != EINTR)
		vms_fatal_exit("pthread_delay_np failed: %d", rs);
}

static $DESCRIPTOR (output_channel_desc, "SYS$OUTPUT:");

int get_terminal_size(int fd, int *x, int *y)
{
	int ret;
	int x_code = DVI$_DEVBUFSIZ;
	int y_code = DVI$_TT_PAGE;
	unsigned long result;
	*x = 80;
	*y = 24;
	ret = lib$getdvi(&x_code, 0, &output_channel_desc, &result, 0, 0);
	if ($VMS_STATUS_SUCCESS(ret) && result) *x = result;
	ret = lib$getdvi(&y_code, 0, &output_channel_desc, &result, 0, 0);
	if ($VMS_STATUS_SUCCESS(ret) && result) *y = result;
	return 0;
}

static $DESCRIPTOR (display_desc, "DECW$DISPLAY");

int is_xterm(void)
{
	static int xt = -1;
	if (xt == -1) {
		xt = $VMS_STATUS_SUCCESS(lib$get_logical(&display_desc, NULL, NULL, NULL, NULL, NULL, NULL, NULL));
	}
	return xt;
}

void get_path_to_exe(void)
{
	path_to_exe = cast_uchar "links";
}

static volatile int io_raw = -1;

static int input_handle = -1;

static unsigned old_ctrl_mask = 0;

static $DESCRIPTOR (input_channel_desc, "SYS$INPUT:");
static unsigned short input_channel;

static pthread_mutex_t io_mutex;
static pthread_mutex_t cancel_mutex;

static volatile int must_redraw = 0;
static volatile int ctrl_c = 0;
static volatile int ctrl_y = 0;

static volatile struct timeval gettimeofday_pool[GETTIMEOFDAY_POOL];
static volatile unsigned gettimeofday_clock = 0;

static void vms_key_handler(int mode, void (*ast)(void))
{
	struct _iosb iosb;
	int ret;
	ret = sys$qiow(0, input_channel, IO$_SETMODE | mode, &iosb, 0, 0, ast, 0, 0, 0, 0, 0);
	if (!$VMS_STATUS_SUCCESS(ret))
		vms_fatal_exit("sys$qiow failed: %d", ret);
	if (!$VMS_STATUS_SUCCESS(iosb.iosb$w_status))
		vms_fatal_exit("IO$M_CTRLCAST failed: %d", iosb.iosb$w_status);
}

static void ctrl_c_ast(void)
{
	if (io_raw <= 0) return;
	must_redraw = 1;
	ctrl_c = 1;
	sys$cancel(input_channel);
	vms_key_handler(IO$M_CTRLCAST, ctrl_c_ast);
}

static void ctrl_y_ast(void)
{
	if (io_raw <= 0) return;
	must_redraw = 1;
	ctrl_y = 1;
	sys$cancel(input_channel);
	vms_key_handler(IO$M_CTRLYAST, ctrl_y_ast);
	vms_key_handler(IO$M_CTRLCAST, ctrl_c_ast);
}

static void handle_ast_keys(int h)
{
	if (ctrl_c) {
		ctrl_c = 0;
		hard_write(h, cast_uchar "\003", 1);
	}
	if (ctrl_y) {
		ctrl_y = 0;
		hard_write(h, cast_uchar "\031", 1);
	}
	if (must_redraw) {
		must_redraw = 0;
		hard_write(h, cast_uchar "\014", 1);
	}
}

#define INPUT_BUFFER_SIZE	16

static void vms_input_thread(void *n, int h)
{
	unsigned char buffer[INPUT_BUFFER_SIZE];
	int buffer_start = 0;
	int buffer_end = 0;
	unsigned ef;
	int vms_ret;
#ifndef VMS_VIRTUAL_PIPE
	/* VMS is buggy - if high priority thread blocks in write,
	   no low priority threads have a chance to run.
	   So, as a workaround, we set the socket nonblocking. */
	set_nonblock(h);
#endif
	vms_ret = lib$get_ef(&ef);
	if (!$VMS_STATUS_SUCCESS(vms_ret))
		vms_fatal_exit("lib$get_ef failed: %d", vms_ret);
	while (1) {
		int ret;
		int count;
		struct _iosb iosb;
		ret = pthread_mutex_lock(&cancel_mutex);
		if (ret)
			vms_fatal_exit("pthread_mutex_lock failed: %d", ret);
		ret = pthread_mutex_unlock(&cancel_mutex);
		if (ret)
			vms_fatal_exit("pthread_mutex_unlock failed: %d", ret);
		handle_ast_keys(h);
		ret = pthread_mutex_lock(&io_mutex);
		if (ret)
			vms_fatal_exit("pthread_mutex_lock failed: %d", ret);
		vms_ret = sys$clref(ef);
		if (!$VMS_STATUS_SUCCESS(vms_ret))
			vms_fatal_exit("sys$clref failed: %d", vms_ret);
		vms_ret = sys$qio(ef, input_channel, IO$_READVBLK | IO$M_NOECHO | IO$M_NOFILTR, &iosb, tis_io_complete, 0, buffer + buffer_end, 1, 0, 0, 0, 0);
		if ($VMS_STATUS_SUCCESS(vms_ret)) {
			if (!iosb.iosb$w_status) {
				if (buffer_start != buffer_end) {
					hard_write(h, buffer + buffer_start, buffer_end - buffer_start);
					buffer_start = buffer_end;
				}
			}
			vms_ret = tis_synch(ef, &iosb);
		}
		ret = pthread_mutex_unlock(&io_mutex);
		if (ret)
			vms_fatal_exit("pthread_mutex_unlock failed: %d", ret);
		handle_ast_keys(h);
		if (vms_ret == SS$_CANCEL ||
		    vms_ret == SS$_ABORT)
			continue;
		if (!$VMS_STATUS_SUCCESS(vms_ret))
			vms_fatal_exit("sys$qio failed: %d", vms_ret);
		if (!$VMS_STATUS_SUCCESS(iosb.iosb$w_status) &&
		    iosb.iosb$w_status != SS$_PARTESCAPE &&
		    iosb.iosb$w_status != SS$_DATAOVERUN &&
		    iosb.iosb$w_status != SS$_CANCEL &&
		    iosb.iosb$w_status != SS$_ABORT)
			vms_fatal_exit("IO$_READVBLK failed: %d, %d", (int)iosb.iosb$w_status, (int)iosb.iosb$w_bcnt);
		count = iosb.iosb$w_bcnt + (iosb.iosb$l_dev_depend >> 16);
		if (count == 1) {
			buffer_end++;
			if (buffer_start) {
				memmove(buffer, buffer + buffer_start, buffer_end - buffer_start);
				buffer_end -= buffer_start;
				buffer_start = 0;
			}
			if (buffer_end == INPUT_BUFFER_SIZE) {
				hard_write(h, buffer + buffer_start, buffer_end - buffer_start);
				buffer_start = buffer_end = 0;
			}
		}
	}
}

int setraw(int ctl, int save)
{
	int ret;
	if (io_raw > 0)
		return 0;
	ret = sys$assign(&input_channel_desc, &input_channel, 0, NULL);
	if (!$VMS_STATUS_SUCCESS(ret))
		vms_fatal_exit("sys$assign failed: %d", ret);
	io_raw = 1;
	vms_key_handler(IO$M_CTRLCAST, ctrl_c_ast);
	vms_key_handler(IO$M_CTRLYAST, ctrl_y_ast);
	ret = pthread_mutex_unlock(&io_mutex);
	if (ret)
		vms_fatal_exit("pthread_mutex_unlock failed: %d", ret);
	return 0;
}

void setcooked(int ctl)
{
	int ret;
	if (io_raw <= 0)
		return;
	ret = pthread_mutex_lock(&cancel_mutex);
	if (ret)
		vms_fatal_exit("pthread_mutex_lock failed: %d", ret);
	clear_events(input_handle, 1);
	while ((ret = pthread_mutex_trylock(&io_mutex))) {
		if (ret != EBUSY)
			vms_fatal_exit("pthread_mutex_trylock failed: %d", ret);
		ret = sys$cancel(input_channel);
		if (!$VMS_STATUS_SUCCESS(ret))
			vms_fatal_exit("sys$cancel failed: %d", ret);
		clear_events(input_handle, 1);
		portable_sleep(0);
	}
	ret = pthread_mutex_unlock(&cancel_mutex);
	if (ret)
		vms_fatal_exit("pthread_mutex_unlock failed: %d", ret);
	io_raw = 0;
	vms_key_handler(IO$M_CTRLCAST, NULL);
	vms_key_handler(IO$M_CTRLYAST, NULL);
	ret = sys$dassgn(input_channel);
	if (!$VMS_STATUS_SUCCESS(ret))
		vms_fatal_exit("sys$dassgn failed: %d", ret);
}

int get_input_handle(void)
{
	unsigned int disable_ctrl_mask = LIB$M_CLI_CTRLT | LIB$M_CLI_CTRLY;
	int ret;
	if (input_handle >= 0)
		return input_handle;
	ret = pthread_mutex_init(&io_mutex, NULL);
	if (ret)
		vms_fatal_exit("pthread_mutex_init failed: %d", ret);
	ret = pthread_mutex_init(&cancel_mutex, NULL);
	if (ret)
		vms_fatal_exit("pthread_mutex_init failed: %d", ret);
	ret = pthread_mutex_lock(&io_mutex);
	if (ret)
		vms_fatal_exit("pthread_mutex_lock failed: %d", ret);
	lib$disable_ctrl(&disable_ctrl_mask, &old_ctrl_mask);
	vms_thread_high_priority = 1;
	input_handle = start_thread(vms_input_thread, NULL, 0, 0);
	if (input_handle == -1)
		vms_fatal_exit("unable to start keyboard thread");
	vms_thread_high_priority = 0;
	return input_handle;
}

#if defined(GRDRV_X)

static int x11_ef = -1;
static int x11_pipe = -1;

static void vms_x11_thread(void *n, int h)
{
	int ret;
	set_nonblock(h);
	while (1) {
		ret = sys$clref(x11_ef);
		if (!$VMS_STATUS_SUCCESS(ret))
			vms_fatal_exit("sys$clref failed: %d", ret);
		EINTRLOOP(ret, (int)vms_write(h, "", 1));
		ret = sys$waitfr(x11_ef);
		if (!$VMS_STATUS_SUCCESS(ret))
			vms_fatal_exit("sys$waitfr failed: %d", ret);
	}
}

int vms_x11_fd(int ef)
{
	if (x11_pipe >= 0) {
		if (ef != x11_ef) vms_fatal_exit("requesting multiple event flags: %d, %d", x11_ef, ef);
		return x11_pipe;
	}
	x11_ef = ef;
	x11_pipe = start_thread(vms_x11_thread, NULL, 0, 0);
	if (x11_pipe == -1)
		vms_fatal_exit("unable to start Xwindow event thread");
	set_nonblock(x11_pipe);
	return x11_pipe;
}

#endif

#if !defined(VMS_VIRTUAL_PIPE) || defined(VMS_SELECT_THREAD)

static int vms_socketpair(int *fd)
{
#if defined(HAVE_SOCKETPAIR)
	int r;
	EINTRLOOP(r, socketpair(AF_INET, SOCK_STREAM, 0, fd));
	return r;
#else
#define PIPE_RETRIES	10
	int rs;
	int s1, s2, s3;
	socklen_t l;
	struct sockaddr_in sa1, sa2;
	int retry_count = 0;
	again:
	s1 = c_socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (s1 < 0)
		goto err0;
	s2 = c_socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (s2 < 0)
		goto err1;
	memset(&sa1, 0, sizeof(sa1));
	sa1.sin_family = AF_INET;
	sa1.sin_port = htons(0);
	sa1.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	EINTRLOOP(rs, bind(s1, (struct sockaddr *)&sa1, sizeof(sa1)));
	if (rs)
		goto err2;
	l = sizeof(sa2);
	EINTRLOOP(rs, getsockname(s1, (struct sockaddr *)&sa1, &l));
	if (rs)
		goto err2;
	EINTRLOOP(rs, listen(s1, 1));
	if (rs)
		goto err2;
	EINTRLOOP(rs, connect(s2, (struct sockaddr *)&sa1, sizeof(sa1)));
	if (rs)
		goto err2;
	l = sizeof(sa1);
	EINTRLOOP(rs, getsockname(s2, (struct sockaddr *)&sa1, &l));
	if (rs)
		goto err2;
	l = sizeof(sa2);
	s3 = c_accept(s1, (struct sockaddr *)&sa2, &l);
	if (s3 < 0)
		goto err2;
	if (sa1.sin_addr.s_addr != sa2.sin_addr.s_addr ||
	    sa1.sin_port != sa2.sin_port) {
		errno = EINVAL;
		goto err2;
	}
	EINTRLOOP(rs, close(s1));
	fd[0] = s2;
	fd[1] = s3;
	return 0;
	err2:
	EINTRLOOP(rs, close(s2));
	err1:
	EINTRLOOP(rs, close(s1));
	err0:
	if (++retry_count > PIPE_RETRIES) return -1;
	portable_sleep(100);
	goto again;
#endif
}

#endif

#ifdef VMS_VIRTUAL_PIPE

static pthread_mutex_t pipe_mutex;
static pthread_cond_t pipe_cond;

static void pipe_lock(void)
{
	int ret;
	ret = pthread_mutex_lock(&pipe_mutex);
	if (ret)
		vms_fatal_exit("pthread_mutex_lock failed: %d", ret);
}

static void pipe_unlock(void)
{
	int ret;
	ret = pthread_mutex_unlock(&pipe_mutex);
	if (ret)
		vms_fatal_exit("pthread_unmutex_lock failed: %d", ret);
}

static void pipe_unlock_wait_cond(pthread_cond_t *cond)
{
	int ret;
	ret = pthread_cond_wait(cond, &pipe_mutex);
	if (ret && ret != EINTR)
		vms_fatal_exit("pthread_cond_wait failed: %d", ret);
}

static void pipe_unlock_wait(void)
{
	pipe_unlock_wait_cond(&pipe_cond);
}

static int pipe_unlock_wait_time(struct timespec *ts)
{
	int ret;
	ret = pthread_cond_timedwait(&pipe_cond, &pipe_mutex, ts);
	if (ret && ret != EINTR && ret != ETIMEDOUT)
		vms_fatal_exit("pthread_cond_timedwait failed: %d", ret);
	return ret == ETIMEDOUT;
}

static void pipe_wake_cond(pthread_cond_t *cond)
{
	int ret;
	ret = pthread_cond_broadcast(cond);
	if (ret)
		vms_fatal_exit("pthread_cond_broadcast failed: %d", ret);
}

static void pipe_wake(void)
{
	pipe_wake_cond(&pipe_cond);
}

#include "vpipe.inc"

static void get_abstime(time_t sec, unsigned usec, struct timespec *ts)
{
#if 0
	int ret;
	struct timespec exp;
	exp.tv_sec = sec;
	exp.tv_nsec = usec * 1000;
	ret = pthread_get_expiration_np(&exp, ts);
	if (ret)
		vms_fatal_exit("pthread_get_expiration_np failed: %d", ret);
#else
	int ret;
	struct timeval tv;
	ret = gettimeofday(&tv, NULL);
	if (ret)
		vms_fatal_exit("gettimeofday failed: %d", errno);
	tv.tv_sec += sec;
	tv.tv_usec += usec;
	if (tv.tv_usec >= 1000000) {
		tv.tv_usec -= 1000000;
		tv.tv_sec++;
	}
	{
		unsigned c = gettimeofday_clock;
		if (c >= GETTIMEOFDAY_POOL) c = 0;
		gettimeofday_clock = c;
		gettimeofday_pool[c].tv_sec = (unsigned long)gettimeofday_pool[c].tv_sec * 11 + (unsigned long)tv.tv_sec;
		gettimeofday_pool[c].tv_usec = (unsigned long)gettimeofday_pool[c].tv_usec * 11 + (unsigned long)tv.tv_usec;
	}
	ts->tv_sec = tv.tv_sec;
	ts->tv_nsec = tv.tv_usec * 1000;
#endif
}

static int check_fd(int fd, int what)
{
	fd_set fds;
	struct timeval tv = {0, 0};
	int rs;
	FD_ZERO(&fds);
	FD_SET(fd, &fds);
	EINTRLOOP(rs, select(fd + 1,
		what == 0 ? &fds : NULL,
		what == 1 ? &fds : NULL,
		what == 2 ? &fds : NULL,
		&tv));
	if (rs < 0) vms_fatal_exit("select for %d/%d write failed: %d", fd, what, errno);
	return rs;
}

#ifdef VMS_SELECT_THREAD

static pthread_cond_t select_cond;

static int select_thread_signal[2];

static int select_set_max;

static fd_set select_set_read;
static fd_set select_set_write;
static fd_set select_set_exception;

static fd_set add_set_read;
static fd_set add_set_write;
static fd_set add_set_exception;

static fd_set remove_set;

static fd_set rs, ws, es;

static void add_to_thread_set(int fd, fd_set *s)
{
	FD_SET(fd, s);
	if (fd >= select_set_max)
		select_set_max = fd + 1;
}

static void vms_select_thread(void *p, int n)
{
	int should_wake = 0;
	pipe_lock();
	FD_ZERO(&rs);
	FD_ZERO(&ws);
	FD_ZERO(&es);
	while (1) {
		int i;
		int r;
		int need_select = 0;
		int new_max = 0;
		for (i = 0; i < select_set_max; i++) {
			int set;
			if (FD_ISSET(i, &add_set_read)) {
				FD_CLR(i, &add_set_read);
				FD_SET(i, &select_set_read);
			}
			if (FD_ISSET(i, &add_set_write)) {
				FD_CLR(i, &add_set_write);
				FD_SET(i, &select_set_write);
			}
			if (FD_ISSET(i, &add_set_exception)) {
				FD_CLR(i, &add_set_exception);
				FD_SET(i, &select_set_exception);
			}
			if (FD_ISSET(i, &remove_set)) {
				FD_CLR(i, &remove_set);
#ifndef  TEST_WAKE_BUG
				FD_CLR(i, &select_set_read);
				FD_CLR(i, &select_set_write);
				FD_CLR(i, &select_set_exception);
#endif
				should_wake = 1;
			}
			set = 0;
			if (FD_ISSET(i, &select_set_read)) {
				set = 1;
				FD_SET(i, &rs);
			}
			if (FD_ISSET(i, &select_set_write)) {
				set = 1;
				FD_SET(i, &ws);
			}
			if (FD_ISSET(i, &select_set_exception)) {
				set = 1;
				FD_SET(i, &es);
			}
			if (set && i >= new_max)
				new_max = i + 1;
			if (set && i != select_thread_signal[0])
				need_select = 1;
		}
		select_set_max = new_max;
		if (!need_select) {
			if (should_wake) {
				pipe_wake();
				should_wake = 0;
			}
			pipe_unlock_wait_cond(&select_cond);
			clear_events(select_thread_signal[0], 0);
		} else {
			pipe_unlock();
			if (should_wake) {
#ifdef TEST_WAKE_BUG
				fprintf(stderr, "wake 1. ");
#endif
				pipe_wake();
#ifdef TEST_WAKE_BUG
				fprintf(stderr, "wake 2. ");
#endif
				should_wake = 0;
			}
			/* It is needed to sleep here with pthread_yield_np() or
			   portable_sleep(0), otherwise some bug in decthreads
			   is triggered and the main thread isn't woken up.
			   tis_io_complete apparently seems to fix the bug too,
			   so we do a dummy syscall that calls tis_io_complete
			   as an AST */
			{
				struct _generic_64 priv;
				memset(&priv, 0, sizeof priv);
				sys$check_privilegew(0, &priv, NULL, 0, NULL, NULL, tis_io_complete, 0);
			}
			/*tis_io_complete();*/
			/*fprintf(stderr, "time1 %d.   ", get_time());
			pthread_yield_np();
			portable_sleep(0);
			fprintf(stderr, "time2 %d.   ", get_time());*/
#ifdef TEST_WAKE_BUG
			fprintf(stderr, "wake 3. ");
#endif
			EINTRLOOP(r, select(select_set_max, &rs, &ws, &es, NULL));
#ifdef TEST_WAKE_BUG
			fprintf(stderr, "wake 4. ");
#endif
			if (r < 0)
				vms_fatal_exit("select in thread failed: %d", errno);
			pipe_lock();
			for (i = 0; i < select_set_max; i++) {
				if (i == select_thread_signal[0]) {
					if (FD_ISSET(i, &rs))
						clear_events(i, 0);
					continue;
				}
				if (FD_ISSET(i, &rs)) {
					FD_CLR(i, &rs);
					FD_CLR(i, &select_set_read);
					should_wake = 1;
				}
				if (FD_ISSET(i, &ws)) {
					FD_CLR(i, &ws);
					FD_CLR(i, &select_set_write);
					should_wake = 1;
				}
				if (FD_ISSET(i, &es)) {
					FD_CLR(i, &es);
					FD_CLR(i, &select_set_exception);
					should_wake = 1;
				}
			}
		}
	}
}

static void select_thread_wake(void)
{
	int r;
	EINTRLOOP(r, (int)write(select_thread_signal[1], "", 1));
	pipe_wake_cond(&select_cond);
}

#endif

#ifndef VMS_SELECT_THREAD

#ifdef __VAX
#define MIN_WAIT_TIME	10000
#else
#define MIN_WAIT_TIME	900
#endif
#define MAX_WAIT_TIME	999999

static unsigned wait_time = MIN_WAIT_TIME;

static void increase_dynamic_time(void)
{
	wait_time += (wait_time / 2);
	if (wait_time > MAX_WAIT_TIME) wait_time = MAX_WAIT_TIME;
}

static void decrease_dynamic_time(void)
{
	wait_time /= 2;
	if (wait_time > MIN_WAIT_TIME) wait_time = MIN_WAIT_TIME;
}

#endif

int vms_select(int n, fd_set *rs, fd_set *ws, fd_set *es, struct timeval *t)
{
	struct timespec ts;
	int i;
	int last_pass = 0;
	int ret_cnt = 0;
	int dynamic_wait;
#ifndef VMS_SELECT_THREAD
	int dynamically_waited = 0;
#endif
	/*if (!rs && !ws && !es)
		return select(0, NULL, NULL, NULL, t);*/
	if (t) {
		if (!t->tv_sec && !t->tv_usec) {
			last_pass = 1;
		} else {
			get_abstime(t->tv_sec, t->tv_usec, &ts);
		}
	}
	pipe_lock();
	test_again:
	dynamic_wait = 0;
	for (i = 0; i < n; i++) {
		int ts = 0;
		if (rs && FD_ISSET(i, rs)) {
			int signaled = 0;
			if (pipe_desc[i]) {
				signaled |= vpipe_may_read(i);
			} else {
#ifdef VMS_SELECT_THREAD
				if (FD_ISSET(i, &select_set_read)) dynamic_wait |= 1;
				else
#endif
				if (check_fd(i, 0)) signaled = 1;
				else {
#ifdef VMS_SELECT_THREAD
					add_to_thread_set(i, &add_set_read);
#endif
					dynamic_wait |= 3;
				}
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
				signaled |= vpipe_may_write(i);
			} else {
#ifdef VMS_SELECT_THREAD
				if (FD_ISSET(i, &select_set_write)) dynamic_wait |= 1;
				else
#endif
				if (check_fd(i, 1)) signaled = 1;
				else {
#ifdef VMS_SELECT_THREAD
					add_to_thread_set(i, &add_set_write);
#endif
					dynamic_wait |= 3;
				}
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
			} else {
#ifdef VMS_SELECT_THREAD
				if (FD_ISSET(i, &select_set_exception)) dynamic_wait |= 1;
				else
#endif
				if (check_fd(i, 2)) signaled = 1;
				else {
#ifdef VMS_SELECT_THREAD
					add_to_thread_set(i, &add_set_exception);
					dynamic_wait |= 3;
#endif
				}
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
	if (!last_pass) {
#ifndef VMS_SELECT_THREAD
		if (dynamic_wait) {
			struct timespec ts2;
			if (dynamically_waited)
				increase_dynamic_time();
			/*fprintf(stderr, "wait time: %d\n", wait_time);*/
			get_abstime(0, wait_time, &ts2);
			if (t &&
			    (ts2.tv_sec > ts.tv_sec ||
			    (ts2.tv_sec == ts.tv_sec && ts2.tv_nsec > ts.tv_nsec)))
				goto full_wait;
			dynamically_waited = pipe_unlock_wait_time(&ts2);
			goto test_again;
		}
#else
		if (dynamic_wait & 2)
			select_thread_wake();
#endif
#ifndef VMS_SELECT_THREAD
		dynamically_waited = 0;
#endif
		if (!t) {
			pipe_unlock_wait();
			goto test_again;
		} else {
full_wait:
			if (pipe_unlock_wait_time(&ts))
				last_pass = 1;
			goto test_again;
		}
	}
#ifndef VMS_SELECT_THREAD
	if (dynamically_waited)
		decrease_dynamic_time();
#endif
	pipe_unlock();
	return ret_cnt;
}

#else

int vms_select(int n, fd_set *rs, fd_set *ws, fd_set *es, struct timeval *t)
{
	return select(n, rs, ws, es, t);
}

#endif

int vms_close(int fd)
{
#ifdef VMS_VIRTUAL_PIPE
	int r = vpipe_close(fd);
	if (r != -2) return r;
#ifdef VMS_SELECT_THREAD
	pipe_lock();
	while (1) {
		FD_CLR(fd, &add_set_read);
		FD_CLR(fd, &add_set_write);
		FD_CLR(fd, &add_set_exception);
		if (!(FD_ISSET(fd, &select_set_read) ||
		      FD_ISSET(fd, &select_set_write) ||
		      FD_ISSET(fd, &select_set_exception)))
			break;
		add_to_thread_set(fd, &remove_set);
#ifdef TEST_WAKE_BUG
		fprintf(stderr, "close 1. ");
#endif
		select_thread_wake();
#ifdef TEST_WAKE_BUG
		fprintf(stderr, "close 2. ");
#endif
		pipe_unlock_wait();
#ifdef TEST_WAKE_BUG
		fprintf(stderr, "close 3. ");
#endif
	}
	pipe_unlock();
#endif
#endif
	return close(fd);
}

int c_pipe(int *fd)
{
#ifdef VMS_VIRTUAL_PIPE
	return vpipe_create(fd);
#else
	return vms_socketpair(fd);
#endif
}

void set_nonblock(int fd)
{
#ifdef VMS_VIRTUAL_PIPE
	if (get_virtual_pipe(fd)) {
		pipe_flags[fd] |= VIRTUAL_PIPE_FLAG_NONBLOCK;
		pipe_unlock();
		pipe_wake();
	} else
#endif
	{
		int rs;
		int on = 1;
		EINTRLOOP(rs, ioctl(fd, FIONBIO, &on));
	}
}

#define BOUNCE_BUFFER_SIZE	64

int vms_read(int fd, void *buf, size_t size)
{
#ifdef VMS_VIRTUAL_PIPE
	int r = vpipe_read(fd, buf, size);
	if (r != -2) return r;
#endif
#ifndef OPENVMS_64BIT
	return read(fd, buf, size);
#else
	{
		int r;
		unsigned char static_buffer[BOUNCE_BUFFER_SIZE];
		unsigned char *bounce_buffer = NULL;
		if (size > BOUNCE_BUFFER_SIZE)
			bounce_buffer = _malloc32(size);
		if (!bounce_buffer) {
			bounce_buffer = static_buffer;
			if (size > BOUNCE_BUFFER_SIZE) size = BOUNCE_BUFFER_SIZE;
		}
		r = read(fd, bounce_buffer, size);
		if (r > 0) memcpy(buf, bounce_buffer, r);
		if (bounce_buffer != static_buffer)
			free(bounce_buffer);
		return r;
	}
#endif
}

int vms_write(int fd, const void *buf, size_t size)
{
#ifdef VMS_VIRTUAL_PIPE
	int r = vpipe_write(fd, buf, size);
	if (r != -2) return r;
#endif
#ifndef OPENVMS_64BIT
	return write(fd, buf, size);
#else
	{
		int r;
		unsigned char static_buffer[BOUNCE_BUFFER_SIZE];
		unsigned char *bounce_buffer = NULL;
		if (size > BOUNCE_BUFFER_SIZE)
			bounce_buffer = _malloc32(size);
		if (!bounce_buffer) {
			bounce_buffer = static_buffer;
			if (size > BOUNCE_BUFFER_SIZE) size = BOUNCE_BUFFER_SIZE;
		}
		memcpy(bounce_buffer, buf, size);
		r = write(fd, bounce_buffer, size);
		if (bounce_buffer != static_buffer)
			free(bounce_buffer);
		return r;
	}
#endif
}

void init_os(void)
{
#if defined(OPENVMS_64BIT)
	{
		int i;
		unsigned char **new_argv = malloc((g_argc + 1) * sizeof(unsigned char *));
		if (!new_argv) vms_fatal_exit("can't allocate argv");
		for (i = 0; i <= g_argc; i++)
			new_argv[i] = (unsigned char *)(my_intptr_t)(((unsigned *)g_argv)[i]);
		g_argv = new_argv;
	}
#endif
#ifdef VMS_VIRTUAL_PIPE
	{
		int ret;
		ret = pthread_mutex_init(&pipe_mutex, NULL);
		if (ret)
			vms_fatal_exit("pthread_mutex_init failed: %d", ret);
		ret = pthread_cond_init(&pipe_cond, NULL);
		if (ret)
			vms_fatal_exit("pthread_cond_init failed: %d", ret);
#ifdef VMS_SELECT_THREAD
		ret = pthread_cond_init(&select_cond, NULL);
		if (ret)
			vms_fatal_exit("pthread_cond_init failed: %d", ret);
		ret = vms_socketpair(select_thread_signal);
		if (ret)
			vms_fatal_exit("can't create select thread socket");
		set_nonblock(select_thread_signal[0]);
		set_nonblock(select_thread_signal[1]);
		select_set_max = 0;
		FD_ZERO(&select_set_read);
		FD_ZERO(&select_set_write);
		FD_ZERO(&select_set_exception);
		FD_ZERO(&add_set_read);
		FD_ZERO(&add_set_write);
		FD_ZERO(&add_set_exception);
		FD_ZERO(&remove_set);
		add_to_thread_set(select_thread_signal[0], &select_set_read);
		select_set_max = select_thread_signal[0] + 1;
		vms_thread_high_priority = -1;
		ret = start_thread(vms_select_thread, NULL, 0, 0);
		if (ret == -1)
			vms_fatal_exit("unable to start select thread");
		vms_thread_high_priority = 0;
#endif
	}
#endif
}

void os_seed_random(unsigned char **pool, int *pool_size)
{
	struct history_item *hi;
	DIR *dir;
	int n, h;

	*pool = init_str();
	*pool_size = 0;

	/*
	 * This is not very secure, but I don't know a better way.
	 */
	add_bytes_to_str(pool, pool_size, (unsigned char *)(void *)&gettimeofday_pool, sizeof gettimeofday_pool);

#ifdef USE_SHA
	/*
	 * Make sure that even if the transformation is reversible (due to
	 * poor randomness on OpenVMS), the adversary won't be able to find
	 * any URLs in the history.
	 */
	n = 0;
	foreach(hi, goto_url_history.items) {
		SHA_CTX ctx;
		unsigned char result[SHA_DIGEST_LENGTH];
		unsigned char sum;
		int i;

		SHA1_Init(&ctx);
		SHA1_Update(&ctx, hi->d, strlen(cast_const_char hi->d));
		SHA1_Final(result, &ctx);

		sum = 0;
		for (i = 0; i < SHA_DIGEST_LENGTH; i++) {
			sum += result[i] + (result[i] >> 4);
		}
		add_chr_to_str(pool, pool_size, sum & 0xf);
		if (++n >= 64)
			break;
	}
#endif

	dir = c_opendir(cast_uchar "/SYS$LOGIN");
	if (dir) {
		for (n = 0; n < 256; n++) {
			struct dirent *de;
			unsigned char *path;
			struct stat st;
			ENULLLOOP(de, (void *)readdir(dir));
			if (!de)
				break;
			path = stracpy(cast_uchar "/SYS$LOGIN/");
			add_to_strn(&path, cast_uchar de->d_name);
			if (!stat(cast_const_char path, &st)) {
				add_bytes_to_str(pool, pool_size, (unsigned char *)&st.st_ctime, (int)sizeof st.st_ctime);
			}
			mem_free(path);
		}
		closedir(dir);
	}

	h = c_open(cast_uchar "/SYS$LOGIN/SSH2/RANDOM_SEED", O_RDONLY);
	if (h == -1)
		h = c_open(cast_uchar "/SYS$LOGIN/SSH/RANDOM_SEED", O_RDONLY);
	if (h != -1) {
		unsigned char buffer[512];
		int r;
		r = hard_read(h, buffer, (int)sizeof buffer);
		if (r >= 0)
			add_bytes_to_str(pool, pool_size, buffer, r);
		EINTRLOOP(r, close(h));
	}
}

void terminate_osdep(void)
{
	if (old_ctrl_mask)
		lib$enable_ctrl(&old_ctrl_mask, NULL);
}

#else

typedef int vms_c_no_empty_unit;

#endif
