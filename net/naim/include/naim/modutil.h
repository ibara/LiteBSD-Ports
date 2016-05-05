/*  _ __   __ _ ___ __  __
** | '_ \ / _` |_ _|  \/  | naim
** | | | | (_| || || |\/| | Copyright 1998-2003 Daniel Reed <n@ml.org>
** |_| |_|\__,_|___|_|  |_| ncurses-based chat client
*/
#ifndef modutil_h
#define modutil_h	1

#include <naim/naim.h>
#include <fcntl.h>
#include <time.h>

typedef int (*mod_hook_t)(void *mod, ...);
typedef struct {
	int	count;
	struct {
		int	weight;
		unsigned long passes, hits;
		mod_hook_t func;
		char	*name;
		void	*mod;
	} *hooks;
} chain_t;
#define HOOK_JUMP	1
#define HOOK_STOP	0
#define HOOK_CONTINUE	(-1)

#define HOOK_DECLARE(x)	chain_t chain_ ## x = { 0, NULL }
#define HOOK_EXT_L(x)	extern chain_t chain_ ## x
#define HOOK_CALL(x, args)					\
	if ((chain_ ## x).count > 0) do { 			\
		int	i;					\
		for (i = 0; (i < chain_ ## x.count)		\
			&& (chain_ ## x.hooks[i].passes++ || 1)	\
			&& ((chain_ ## x.hooks[i].func args	\
			 == HOOK_CONTINUE)			\
			 || (chain_ ## x.hooks[i].hits++ && 0)); i++); \
	} while (0)
#define HOOK_ADD(x, m, f, w)					\
	do { 							\
		HOOK_EXT_L(x);					\
		int	i;					\
								\
		HOOK_DEL(x, m, f);				\
		for (i = 0; (i < chain_ ## x.count)		\
			&& (chain_ ## x.hooks[i].weight <= w); i++); \
		HOOK_INS(x, m, f, w, i);			\
	} while (0)
#define HOOK_INS(x, m, f, w, pos)				\
	do {							\
		HOOK_EXT_L(x);					\
								\
		if (pos > chain_ ## x.count)			\
			pos = chain_ ## x.count;		\
		chain_ ## x.hooks = realloc(chain_ ## x.hooks,	\
			(chain_ ## x.count+1) *			\
			sizeof(*(chain_ ## x.hooks)));		\
		memmove(chain_ ## x.hooks+pos+1, chain_ ## x.hooks+pos, \
			(chain_ ## x.count-pos) *		\
			sizeof(*(chain_ ## x.hooks)));		\
		chain_ ## x.hooks[pos].weight = w;		\
		chain_ ## x.hooks[pos].passes = 0;		\
		chain_ ## x.hooks[pos].hits = 0;		\
		chain_ ## x.hooks[pos].func = (mod_hook_t)f;	\
		chain_ ## x.hooks[pos].name = strdup(#f);	\
		chain_ ## x.hooks[pos].mod = m;			\
		chain_ ## x.count++;				\
	} while (0)
#define HOOK_DEL(x, m, f)					\
	do { 							\
		HOOK_EXT_L(x);					\
		int	i;					\
								\
		for (i = 0; (i < chain_ ## x.count)		\
			&& ((chain_ ## x.hooks[i].mod != m)	\
			 || (chain_ ## x.hooks[i].func != (mod_hook_t)f)); i++); \
		if (i < chain_ ## x.count) {			\
			free(chain_ ## x.hooks[i].name);	\
			memmove(chain_ ## x.hooks+i, chain_ ## x.hooks+i+1, \
				(chain_ ## x.count-i-1) *	\
				sizeof(*(chain_ ## x.hooks)));	\
			chain_ ## x.hooks = realloc(chain_ ## x.hooks, \
				(chain_ ## x.count-1) *		\
				sizeof(*(chain_ ## x.hooks)));	\
			chain_ ## x.count--;			\
		}						\
	} while (0)

#define MOD_REMAINLOADED	1
#define MOD_FINISHED		0

#define MODULE_LICENSE(x)	const char module_license[] = #x
#define MODULE_AUTHOR(x)	const char module_author[] = #x
#define MODULE_CATEGORY(x)	const char module_category[] = #x
#define MODULE_DESCRIPTION(x)	const char module_description[] = #x
#define MODULE_VERSION(x)	const double module_version = x



typedef struct {
	int	fd, type, buflen;
	char	*buf;
	void	(*func)();
} mod_fd_list_t;

#define MOD_FD_TYPE(x)	((O_WRONLY+1) | (1 << (2+(x))))

#define MOD_FD_CONNECT	MOD_FD_TYPE(0)
#define MOD_FD_WRITE	MOD_FD_TYPE(1)

/* modutil.c */
int	mod_fd_register(int fd, int type, char *buf, int buflen,
		void (*func)());
void	mod_fd_unregister(int i);

void	mod_fd_read_raw(int fd, void (*func)(int, int));
void	mod_fd_read_buf(int fd, void (*func)(char *, int), char *, int);

void	mod_fd_connect_raw(const char *host, int port, void (*func)(int));
void	mod_fd_write_buf(int fd, const char *buf, int buflen);

#endif /* naim_h */
