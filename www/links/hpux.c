#if defined(__hpux)

#include "cfg.h"

#include "com-defs.h"

#ifdef report
#include <stdio.h>
#include <unistd.h>
#endif
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>

#ifndef HAVE_SOCKLEN_T
#define socklen_t int
#endif

#include "hpux.h"

union len_t {
	int intlen;
	socklen_t socklen;
};

static int hp_detect_length(int s)
{
	static int mode_cache = -1;
	union len_t u;
	int val;
	int r;

	if (sizeof(int) == sizeof(socklen_t)) return 0;

	if (mode_cache >= 0) return mode_cache;

	memset(&u, 1, sizeof u);
	r = getsockopt(s, SOL_SOCKET, SO_ERROR, (void *)&val, (void *)&u);
	if (r) return -1;
	if (u.socklen == sizeof(int)) {
#ifdef report
		fprintf(stderr, "detected socklen argument\n");
		sleep(1);
#endif
		mode_cache = 1;
		return 1;
	} else if (u.intlen == sizeof(int)) {
#ifdef report
		fprintf(stderr, "detected int argument\n");
		sleep(1);
#endif
		mode_cache = 0;
		return 0;
	} else {
		fatal_exit("Could not detect HP-UX socklen argument type: %lx,%x", (long)u.socklen, u.intlen);
		errno = EINVAL;
		return -1;
	}
}

int hp_accept(int s, struct sockaddr *addr, socklen_t *len)
{
	union len_t u;
	int mode, r;
	if (addr == NULL || len == NULL) return accept(s, NULL, NULL);
	if ((mode = hp_detect_length(s)) == -1) return -1;
	memset(&u, 0, sizeof u);
	if (!mode) u.intlen = *len;
	else u.socklen = *len;
	r = accept(s, addr, (void *)&u);
	if (!mode) *len = u.intlen;
	else *len = u.socklen;
	return r;
}

int hp_getpeername(int s, struct sockaddr *addr, socklen_t *len)
{
	union len_t u;
	int mode, r;
	if ((mode = hp_detect_length(s)) == -1) return -1;
	memset(&u, 0, sizeof u);
	if (!mode) u.intlen = *len;
	else u.socklen = *len;
	r = getpeername(s, addr, (void *)&u);
	if (!mode) *len = u.intlen;
	else *len = u.socklen;
	return r;
}

int hp_getsockname(int s, struct sockaddr *addr, socklen_t *len)
{
	union len_t u;
	int mode, r;
	if ((mode = hp_detect_length(s)) == -1) return -1;
	memset(&u, 0, sizeof u);
	if (!mode) u.intlen = *len;
	else u.socklen = *len;
	r = getsockname(s, addr, (void *)&u);
	if (!mode) *len = u.intlen;
	else *len = u.socklen;
	return r;
}

int hp_getsockopt(int s, int level, int opt, void *val, socklen_t *len)
{
	union len_t u;
	int mode, r;
	if (val == NULL || len == NULL) return getsockopt(s, level, opt, NULL, NULL);
	if ((mode = hp_detect_length(s)) == -1) return -1;
	memset(&u, 0, sizeof u);
	if (!mode) u.intlen = *len;
	else u.socklen = *len;
	r = getsockopt(s, level, opt, val, (void *)&u);
	if (!mode) *len = u.intlen;
	else *len = u.socklen;
	return r;
}

#else

typedef int hpux_c_no_empty_unit;

#endif
