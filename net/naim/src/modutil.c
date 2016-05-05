/*  _ __   __ _ ___ __  __
** | '_ \ / _` |_ _|  \/  | naim
** | | | | (_| || || |\/| | Copyright 1998-2003 Daniel Reed <n@ml.org>
** |_| |_|\__,_|___|_|  |_| ncurses-based chat client
*/
#include "naim-int.h"
#include <naim/modutil.h>

mod_fd_list_t	*mod_fd_listar = NULL;
int	mod_fd_listc = 0;

int	mod_fd_register(int fd, int type, char *buf, int buflen,
		void (*func)()) {
	int	i = mod_fd_listc;

	if (type == (O_RDONLY+1))
		for (i = 0; i < mod_fd_listc; i++) {
			if ((mod_fd_listar[i].fd == fd)
				&& (mod_fd_listar[i].type == type))
				break;
		}
	if (i == mod_fd_listc) {
		mod_fd_listc++;
		mod_fd_listar = realloc(mod_fd_listar,
			sizeof(*mod_fd_listar)*mod_fd_listc);
		mod_fd_listar[i].fd = fd;
	}
	mod_fd_listar[i].type = type;
	mod_fd_listar[i].buf = buf;
	mod_fd_listar[i].buflen = buflen;
	mod_fd_listar[i].func = func;
	return(i);
}

void	mod_fd_read_raw(int fd, void (*func)(int, int)) {
	mod_fd_register(fd, (O_RDONLY+1), NULL, 0, func);
}

void	mod_fd_read_buf(int fd, void (*func)(char *, int), char *buf,
		int buflen) {
	mod_fd_register(fd, (O_RDONLY+1), buf, buflen, func);
}

#if 0
void	mod_fd_connect_raw(const char *host, int port, void (*func)(int)) {
	int	fd = firetalk_internal_connect_host(host, port);

	mod_fd_register(fd, MOD_FD_CONNECT, NULL, 0, func);
}
#endif

void	mod_fd_write_buf(int fd, const char *buf, int buflen) {
	mod_fd_register(fd, MOD_FD_WRITE, (char *)buf, buflen, NULL);
}

void	mod_fd_unregister(int i) {
	assert(i < mod_fd_listc);
	memmove(mod_fd_listar+i, mod_fd_listar+i+1,
		sizeof(*mod_fd_listar)*(mod_fd_listc-i-1));
	mod_fd_listc--;
	mod_fd_listar = realloc(mod_fd_listar,
		sizeof(*mod_fd_listar)*mod_fd_listc);
}
