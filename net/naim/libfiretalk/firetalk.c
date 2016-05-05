/*
firetalk.c - FireTalk wrapper definitions
Copyright (C) 2000 Ian Gulliver
Copyright 2002-2006 Daniel Reed <n@ml.org>

This program is free software; you can redistribute it and/or modify
it under the terms of version 2 of the GNU General Public License as
published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>
#include <strings.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <time.h>
#include <sys/stat.h>
#include <netdb.h>
#include <errno.h>
#include <signal.h>
#include <fcntl.h>

#define FIRETALK

#include "firetalk-int.h"
#include "firetalk.h"

typedef void (*ptrtotoc)(void *, ...);
typedef void (*sighandler_t)(int);

/* Global variables */
fte_t	firetalkerror;
static struct s_firetalk_handle *handle_head = NULL;

static const firetalk_protocol_t **firetalk_protocols = NULL;
static int FP_MAX = 0;

fte_t	firetalk_register_protocol(const firetalk_protocol_t *const proto) {
	const firetalk_protocol_t **ptr;

	if (proto == NULL)
		abort();

	ptr = realloc(firetalk_protocols, sizeof(*firetalk_protocols)*(FP_MAX+1));
	if (ptr == NULL)
		return(FE_UNKNOWN);
	firetalk_protocols = ptr;
	firetalk_protocols[FP_MAX++] = proto;
	return(FE_SUCCESS);
}

static void firetalk_register_default_protocols(void) {
	extern const firetalk_protocol_t
		firetalk_protocol_irc,
		firetalk_protocol_slcp,
		firetalk_protocol_toc2;

	if (firetalk_register_protocol(&firetalk_protocol_irc) != FE_SUCCESS)
		abort();
	if (firetalk_register_protocol(&firetalk_protocol_slcp) != FE_SUCCESS)
		abort();
	if (firetalk_register_protocol(&firetalk_protocol_toc2) != FE_SUCCESS)
		abort();
}

int	firetalk_find_protocol(const char *strprotocol) {
	static int registered_defaults = 0;
	int	i;

	if (strprotocol == NULL)
		abort();

	for (i = 0; i < FP_MAX; i++)
		if (strcasecmp(strprotocol, firetalk_protocols[i]->strprotocol) == 0)
			return(i);
	if (!registered_defaults) {
		registered_defaults = 1;
		firetalk_register_default_protocols();
		for (i = 0; i < FP_MAX; i++)
			if (strcasecmp(strprotocol, firetalk_protocols[i]->strprotocol) == 0)
				return(i);
	}
	return(-1);
}

/* Internal function definitions */

/* firetalk_find_by_toc searches the firetalk handle list for the toc handle passed, and returns the firetalk handle */
firetalk_t firetalk_find_handle(client_t c) {
	struct s_firetalk_handle *iter;

	for (iter = handle_head; iter != NULL; iter = iter->next)
		if (iter->handle == c)
			return(iter);
	abort();
}

firetalk_t firetalk_find_clientstruct(void *clientstruct) {
	struct s_firetalk_handle *iter;

	for (iter = handle_head; iter != NULL; iter = iter->next)
		if (iter->clientstruct == clientstruct)
			return(iter);
	return(NULL);
}

#define DEBUG

#ifdef DEBUG
# define VERIFYCONN \
	do { \
		if (firetalk_check_handle(conn) != FE_SUCCESS) \
			abort(); \
	} while(0)

static fte_t firetalk_check_handle(firetalk_t c) {
	struct s_firetalk_handle *iter;

	for (iter = handle_head; iter != NULL; iter = iter->next)
		if (iter == c)
			return(FE_SUCCESS);
	return(FE_BADHANDLE);
}
#else
# define VERIFYCONN \
	do { \
	} while(0)
#endif

static char **firetalk_parse_subcode_args(char *string) {
	static char *args[256];
	int i,n;
	size_t l;

	l = strlen(string);
	args[0] = string;
	n = 1;
	for (i = 0; (size_t) i < l && n < 255; i++) {
		if (string[i] == ' ') {
			string[i++] = '\0';
			args[n++] = &string[i];
		}
	}
	args[n] = NULL;
	return(args);
}

static unsigned char firetalk_debase64_char(const char c) {
	if ((c >= 'A') && (c <= 'Z'))
		return((unsigned char)(c - 'A'));
	if ((c >= 'a') && (c <= 'z'))
		return((unsigned char)(26 + (c - 'a')));
	if ((c >= '0') && (c <= '9'))
		return((unsigned char)(52 + (c - '0')));
	if (c == '+')
		return((unsigned char)62);
	if (c == '/')
		return((unsigned char)63);
	return((unsigned char)0);
}

const char *firetalk_debase64(const char *const str) {
        static unsigned char out[256];
	int	s, o, len = strlen(str);

	for (o = s = 0; (s <= (len - 3)) && (o < (sizeof(out)-1)); s += 4, o += 3) {
		out[o]   = (firetalk_debase64_char(str[s])   << 2) | (firetalk_debase64_char(str[s+1]) >> 4);
		out[o+1] = (firetalk_debase64_char(str[s+1]) << 4) | (firetalk_debase64_char(str[s+2]) >> 2);
		out[o+2] = (firetalk_debase64_char(str[s+2]) << 6) |  firetalk_debase64_char(str[s+3]);
	}
	out[o] = 0;
	return((char *)out);
}


fte_t	firetalk_im_internal_add_deny(firetalk_t conn, const char *const nickname) {
	struct s_firetalk_deny *iter;

	VERIFYCONN;

	for (iter = conn->deny_head; iter != NULL; iter = iter->next)
		if (firetalk_protocols[conn->protocol]->comparenicks(iter->nickname, nickname) == FE_SUCCESS)
			break; /* not an error, user is in buddy list */

	if (iter == NULL) {
		iter = conn->deny_head;
		conn->deny_head = calloc(1, sizeof(struct s_firetalk_deny));
		if (conn->deny_head == NULL)
			abort();
		conn->deny_head->next = iter;
		conn->deny_head->nickname = strdup(nickname);
		if (conn->deny_head->nickname == NULL)
			abort();
	}

	firetalk_callback_im_buddyonline(conn->handle, nickname, 0);

	return(FE_SUCCESS);
}

#undef FIRETALK_FORK_RESOLV
#ifdef FIRETALK_FORK_RESOLV
fte_t	firetalk_internal_resolve4(const char *const host, struct in_addr *inet4_ip) {
	int	pi[2];

	if (pipe(pi) != 0)
		return(FE_NOTFOUND);

	switch(fork()) {
	  case -1:
		close(pi[0]);
		close(pi[1]);
		return(FE_NOTFOUND);
	  case 0: {
			struct hostent
				*he;

			close(pi[0]);
			if (((he = gethostbyaddr(host, strlen(host), AF_INET)) != NULL) && (he->h_addr_list != NULL))
				write(pi[1], he->h_addr_list[0], sizeof(inet4_ip->s_addr));
			close(pi[1]);
			_exit(2);
		}
	  default: {
			struct timeval
				tv;
			fd_set	readset;

			FD_ZERO(&readset);
			FD_SET(pi[0], &readset);
			close(pi[1]);
			tv.tv_sec = 5;
			tv.tv_usec = 0;
			if ((select(pi[0]+1, &readset, NULL, NULL, &tv) > 0)
				&& FD_ISSET(pi[0], &readset)
				&& (read(pi[0], &(inet4_ip->s_addr), sizeof(inet4_ip->s_addr)) == sizeof(inet4_ip->s_addr))) {
				close(pi[0]);
				return(FE_SUCCESS);
			}
			close(pi[0]);
			return(FE_NOTFOUND);
		}
	}
}
#else
fte_t	firetalk_internal_resolve4(const char *const host, struct in_addr *inet4_ip) {
	struct hostent *he;

	he = gethostbyname(host);
	if (he && he->h_addr_list) {
		memmove(&(inet4_ip->s_addr), he->h_addr_list[0], sizeof(inet4_ip->s_addr));
		return(FE_SUCCESS);
	}

	memset(&(inet4_ip->s_addr), 0, sizeof(inet4_ip->s_addr));

	return(FE_NOTFOUND);
}
#endif

struct sockaddr_in *firetalk_internal_remotehost4(client_t c) {
	struct s_firetalk_handle
		*conn = firetalk_find_handle(c);

	return(&(conn->remote_addr));
}

#ifdef _FC_USE_IPV6
fte_t	firetalk_internal_resolve6(const char *const host, struct in6_addr *inet6_ip) {
	struct addrinfo *addr = NULL;  // xxx generalize this so that we can use this with v6 and v4
	struct addrinfo hints = {0, PF_INET6, 0, 0, 0, NULL, NULL, NULL};

	if (getaddrinfo(host, NULL, &hints, &addr) == 0) {
		struct addrinfo *cur;

		for (cur = addr; cur != NULL; cur = cur->ai_next)
			if (cur->ai_family == PF_INET6) {
				memcpy(&(inet6_ip->s6_addr), ((struct sockaddr_in6 *)cur->ai_addr)->sin6_addr.s6_addr, 16);
				freeaddrinfo(addr);
				return(FE_SUCCESS);
			}
	}

	memset(&(inet6_ip->s6_addr), 0, sizeof(inet6_ip->s6_addr));

	if (addr != NULL)
		freeaddrinfo(addr);

	return(FE_NOTFOUND);
}

struct sockaddr_in6 *firetalk_internal_remotehost6(client_t c) {
	struct s_firetalk_handle
		*conn = firetalk_find_handle(c);

	return(&(conn->remote_addr6));
}
#endif

int	firetalk_internal_connect_host_addr(const char *const host, const uint16_t port, struct sockaddr_in *inet4
#ifdef _FC_USE_IPV6
		, struct sockaddr_in6 *inet6
#endif
) {
#ifdef _FC_USE_IPV6
	if (firetalk_internal_resolve6(host, &(inet6->sin6_addr)) == FE_SUCCESS) {
		inet6->sin6_port = htons(port);
		inet6->sin6_family = AF_INET6;
	} else
		inet6 = NULL;
#endif
	if (firetalk_internal_resolve4(host, &(inet4->sin_addr)) == FE_SUCCESS) {
		inet4->sin_port = htons(port);
		inet4->sin_family = AF_INET;
	} else
		inet4 = NULL;

	return firetalk_internal_connect(inet4
#ifdef _FC_USE_IPV6
	   , inet6
#endif
	   );
}

int	firetalk_internal_connect_host(const char *const host, const int port) {
	struct sockaddr_in myinet4;
#ifdef _FC_USE_IPV6
	struct sockaddr_in6 myinet6;
#endif

	return(firetalk_internal_connect_host_addr(host, port, &myinet4
#ifdef _FC_USE_IPV6
		, &myinet6
#endif
	));
}

int	firetalk_internal_connect(struct sockaddr_in *inet4_ip
#ifdef _FC_USE_IPV6
		, struct sockaddr_in6 *inet6_ip
#endif
		) {
	int s,i;

#ifdef _FC_USE_IPV6
	if (inet6_ip && (inet6_ip->sin6_addr.s6_addr[0] || inet6_ip->sin6_addr.s6_addr[1]
		|| inet6_ip->sin6_addr.s6_addr[2] || inet6_ip->sin6_addr.s6_addr[3]
		|| inet6_ip->sin6_addr.s6_addr[4] || inet6_ip->sin6_addr.s6_addr[5]
		|| inet6_ip->sin6_addr.s6_addr[6] || inet6_ip->sin6_addr.s6_addr[7]
		|| inet6_ip->sin6_addr.s6_addr[8] || inet6_ip->sin6_addr.s6_addr[9]
		|| inet6_ip->sin6_addr.s6_addr[10] || inet6_ip->sin6_addr.s6_addr[11]
		|| inet6_ip->sin6_addr.s6_addr[12] || inet6_ip->sin6_addr.s6_addr[13]
		|| inet6_ip->sin6_addr.s6_addr[14] || inet6_ip->sin6_addr.s6_addr[15])) {
		h_errno = 0;
		s = socket(PF_INET6, SOCK_STREAM, 0);
		if ((s != -1) && (fcntl(s, F_SETFL, O_NONBLOCK) == 0)) {
			i = connect(s, (const struct sockaddr *)inet6_ip, sizeof(struct sockaddr_in6));
			if ((i == 0) || (errno == EINPROGRESS))
				return(s);
		}
	}
#endif

	if (inet4_ip && inet4_ip->sin_addr.s_addr) {
		h_errno = 0;
		s = socket(PF_INET, SOCK_STREAM, 0);
		if ((s != -1) && (fcntl(s, F_SETFL, O_NONBLOCK) == 0)) {
			i = connect(s, (const struct sockaddr *)inet4_ip, sizeof(struct sockaddr_in));
			if ((i == 0) || (errno == EINPROGRESS))
				return(s);
		}
	}

	firetalkerror = FE_CONNECT;
	return(-1);
}

enum firetalk_connectstate firetalk_internal_get_connectstate(client_t c) {
	struct s_firetalk_handle
		*conn = firetalk_find_handle(c);

	return(conn->connected);
}

void firetalk_internal_set_connectstate(client_t c, enum firetalk_connectstate fcs) {
	struct s_firetalk_handle
		*conn = firetalk_find_handle(c);

	conn->connected = fcs;
}

void firetalk_internal_send_data(firetalk_t c, const char *const data, const int length) {
	if (c->connected == FCS_NOTCONNECTED || c->connected == FCS_WAITING_SYNACK)
		return;

	if (send(c->fd,data,length,MSG_DONTWAIT|MSG_NOSIGNAL) != length) {
		/* disconnect client (we probably overran the queue, or the other end is gone) */
		firetalk_callback_disconnect(c->handle,FE_PACKET);
	}

	/* request ratelimit info */
	/* immediate send or queue? */
}

fte_t	firetalk_user_visible(firetalk_t conn, const char *const nickname) {
	struct s_firetalk_room *iter;

	VERIFYCONN;

	for (iter = conn->room_head; iter != NULL; iter = iter->next) {
		struct s_firetalk_member *mem;

		for (mem = iter->member_head; mem != NULL; mem = mem->next)
			if (firetalk_protocols[conn->protocol]->comparenicks(mem->nickname, nickname) == FE_SUCCESS)
				return(FE_SUCCESS);
	}
	return(FE_NOMATCH);
}

fte_t	firetalk_user_visible_but(firetalk_t conn, const char *const room, const char *const nickname) {
	struct s_firetalk_room *iter;

	VERIFYCONN;

	for (iter = conn->room_head; iter != NULL; iter = iter->next) {
		struct s_firetalk_member *mem;

		if (firetalk_protocols[conn->protocol]->comparenicks(iter->name, room) == FE_SUCCESS)
			continue;
		for (mem = iter->member_head; mem != NULL; mem = mem->next)
			if (firetalk_protocols[conn->protocol]->comparenicks(mem->nickname, nickname) == FE_SUCCESS)
				return(FE_SUCCESS);
	}
	return(FE_NOMATCH);
}

fte_t	firetalk_chat_internal_add_room(firetalk_t conn, const char *const name) {
	struct s_firetalk_room *iter;

	VERIFYCONN;

	for (iter = conn->room_head; iter != NULL; iter = iter->next)
		if (firetalk_protocols[conn->protocol]->comparenicks(iter->name, name) == FE_SUCCESS)
			return(FE_DUPEROOM); /* not an error, we're already in room */

	iter = conn->room_head;
	conn->room_head = calloc(1, sizeof(struct s_firetalk_room));
	if (conn->room_head == NULL)
		abort();
	conn->room_head->next = iter;
	conn->room_head->name = strdup(name);
	if (conn->room_head->name == NULL)
		abort();

	return(FE_SUCCESS);
}

fte_t	firetalk_chat_internal_add_member(firetalk_t conn, const char *const room, const char *const nickname) {
	struct s_firetalk_room *iter;
	struct s_firetalk_member *memberiter;

	VERIFYCONN;

	for (iter = conn->room_head; iter != NULL; iter = iter->next)
		if (firetalk_protocols[conn->protocol]->comparenicks(iter->name, room) == FE_SUCCESS)
			break;

	if (iter == NULL) /* we don't know about that room */
		return(FE_NOTFOUND);

	for (memberiter = iter->member_head; memberiter != NULL; memberiter = memberiter->next)
		if (firetalk_protocols[conn->protocol]->comparenicks(memberiter->nickname, nickname) == FE_SUCCESS)
			return(FE_SUCCESS);

	memberiter = iter->member_head;
	iter->member_head = calloc(1, sizeof(struct s_firetalk_member));
	if (iter->member_head == NULL)
		abort();
	iter->member_head->next = memberiter;
	iter->member_head->nickname = strdup(nickname);
	if (iter->member_head->nickname == NULL)
		abort();

	return(FE_SUCCESS);
}

static void firetalk_im_delete_buddy(firetalk_t conn, const char *const nickname) {
	struct s_firetalk_buddy *iter, *prev;

	for (prev = NULL, iter = conn->buddy_head; iter != NULL; prev = iter, iter = iter->next) {
		assert(iter->nickname != NULL);
		assert(iter->group != NULL);

		if (firetalk_protocols[conn->protocol]->comparenicks(nickname, iter->nickname) == FE_SUCCESS)
			break;
	}
	if (iter == NULL)
		return;

	if (prev != NULL)
		prev->next = iter->next;
	else
		conn->buddy_head = iter->next;
	free(iter->nickname);
	iter->nickname = NULL;
	free(iter->group);
	iter->group = NULL;
	if (iter->friendly != NULL) {
		free(iter->friendly);
		iter->friendly = NULL;
	}
	if (iter->capabilities != NULL) {
		free(iter->capabilities);
		iter->capabilities = NULL;
	}
	free(iter);
	iter = NULL;
}

static struct s_firetalk_buddy *firetalk_im_find_buddy(firetalk_t conn, const char *const name) {
	struct s_firetalk_buddy *iter;

	for (iter = conn->buddy_head; iter != NULL; iter = iter->next) {
		assert(iter->nickname != NULL);
		assert(iter->group != NULL);

		if (firetalk_protocols[conn->protocol]->comparenicks(iter->nickname, name) == FE_SUCCESS)
			return(iter);
	}
	return(NULL);
}

fte_t	firetalk_im_remove_buddy(firetalk_t conn, const char *const name) {
	struct s_firetalk_buddy *iter;

	VERIFYCONN;

	if ((iter = firetalk_im_find_buddy(conn, name)) == NULL)
		return(FE_NOTFOUND);

	if (conn->connected != FCS_NOTCONNECTED) {
		int	ret;

		ret = firetalk_protocols[conn->protocol]->im_remove_buddy(conn->handle, iter->nickname, iter->group);
		if (ret != FE_SUCCESS)
			return(ret);
	}

	firetalk_im_delete_buddy(conn, name);

	return(FE_SUCCESS);
}

fte_t	firetalk_im_internal_remove_deny(firetalk_t conn, const char *const nickname) {
	struct s_firetalk_deny *iter;
	struct s_firetalk_deny *prev;

	VERIFYCONN;

	prev = NULL;
	for (iter = conn->deny_head; iter != NULL; iter = iter->next) {
		if (firetalk_protocols[conn->protocol]->comparenicks(nickname, iter->nickname) == FE_SUCCESS) {
			if (prev)
				prev->next = iter->next;
			else
				conn->deny_head = iter->next;
			free(iter->nickname);
			iter->nickname = NULL;
			free(iter);
			return(FE_SUCCESS);
		}
		prev = iter;
	}

	return(FE_NOTFOUND);
}

fte_t	firetalk_chat_internal_remove_room(firetalk_t conn, const char *const name) {
	struct s_firetalk_room *iter;
	struct s_firetalk_room *prev;
	struct s_firetalk_member *memberiter;
	struct s_firetalk_member *membernext;

	VERIFYCONN;

	prev = NULL;
	for (iter = conn->room_head; iter != NULL; iter = iter->next) {
		if (firetalk_protocols[conn->protocol]->comparenicks(name, iter->name) == FE_SUCCESS) {
			for (memberiter = iter->member_head; memberiter != NULL; memberiter = membernext) {
				membernext = memberiter->next;
				free(memberiter->nickname);
				memberiter->nickname = NULL;
				free(memberiter);
			}
			iter->member_head = NULL;
			if (prev)
				prev->next = iter->next;
			else
				conn->room_head = iter->next;
			if (iter->name) {
				free(iter->name);
				iter->name = NULL;
			}
			free(iter);
			return(FE_SUCCESS);
		}
		prev = iter;
	}

	return(FE_NOTFOUND);
}

fte_t	firetalk_chat_internal_remove_member(firetalk_t conn, const char *const room, const char *const nickname) {
	struct s_firetalk_room *iter;
	struct s_firetalk_member *memberiter;
	struct s_firetalk_member *memberprev;

	VERIFYCONN;

	for (iter = conn->room_head; iter != NULL; iter = iter->next)
		if (firetalk_protocols[conn->protocol]->comparenicks(iter->name, room) == FE_SUCCESS)
			break;

	if (iter == NULL) /* we don't know about that room */
		return(FE_NOTFOUND);

	memberprev = NULL;
	for (memberiter = iter->member_head; memberiter != NULL; memberiter = memberiter->next) {
		if (firetalk_protocols[conn->protocol]->comparenicks(memberiter->nickname,nickname) == FE_SUCCESS) {
			if (memberprev)
				memberprev->next = memberiter->next;
			else
				iter->member_head = memberiter->next;
			if (memberiter->nickname) {
				free(memberiter->nickname);
				memberiter->nickname = NULL;
			}
			free(memberiter);
			return(FE_SUCCESS);
		}
		memberprev = memberiter;
	}

	return(FE_SUCCESS);
}

struct s_firetalk_room *firetalk_find_room(firetalk_t c, const char *const room) {
	struct s_firetalk_room *roomiter;
	const char *normalroom;

	normalroom = firetalk_protocols[c->protocol]->room_normalize(room);
	for (roomiter = c->room_head; roomiter != NULL; roomiter = roomiter->next)
		if (firetalk_protocols[c->protocol]->comparenicks(roomiter->name, normalroom) == FE_SUCCESS)
			return(roomiter);

	firetalkerror = FE_NOTFOUND;
	return(NULL);
}

static struct s_firetalk_member *firetalk_find_member(firetalk_t c, struct s_firetalk_room *r, const char *const name) {
	struct s_firetalk_member *memberiter;

	for (memberiter = r->member_head; memberiter != NULL; memberiter = memberiter->next)
		if (firetalk_protocols[c->protocol]->comparenicks(memberiter->nickname, name) == FE_SUCCESS)
			return(memberiter);

	firetalkerror = FE_NOTFOUND;
	return(NULL);
}

void firetalk_callback_needpass(client_t c, char *pass, const int size) {
	struct s_firetalk_handle
		*conn = firetalk_find_handle(c);

	if (conn->callbacks[FC_NEEDPASS])
		conn->callbacks[FC_NEEDPASS](conn, conn->clientstruct, pass, size);
}

static const char *isonline_hack = NULL;

void firetalk_callback_im_getmessage(client_t c, const char *const sender, const int automessage, const char *const message) {
	struct s_firetalk_handle
		*conn = firetalk_find_handle(c);
	struct s_firetalk_deny *iter;

	if (strstr(message, "<a href=\"http://www.candidclicks.com/cgi-bin/enter.cgi?") != NULL) {
		firetalk_im_evil(conn, sender);
		return;
	}
	if (conn->callbacks[FC_IM_GETMESSAGE]) {
		for (iter = conn->deny_head; iter != NULL; iter = iter->next)
			if (firetalk_protocols[conn->protocol]->comparenicks(sender, iter->nickname) == FE_SUCCESS)
				return;
		isonline_hack = sender;
		conn->callbacks[FC_IM_GETMESSAGE](conn, conn->clientstruct, sender, automessage, message);
		isonline_hack = NULL;
	}
}

void firetalk_callback_im_getaction(client_t c, const char *const sender, const int automessage, const char *const message) {
	struct s_firetalk_handle
		*conn = firetalk_find_handle(c);
	struct s_firetalk_deny *iter;

	if (conn->callbacks[FC_IM_GETACTION]) {
		for (iter = conn->deny_head; iter != NULL; iter = iter->next)
			if (firetalk_protocols[conn->protocol]->comparenicks(sender, iter->nickname) == FE_SUCCESS)
				return;
		isonline_hack = sender;
		conn->callbacks[FC_IM_GETACTION](conn, conn->clientstruct, sender, automessage, message);
		isonline_hack = NULL;
	}
}

void firetalk_callback_im_buddyonline(client_t c, const char *const nickname, int online) {
	struct s_firetalk_handle
		*conn = firetalk_find_handle(c);
	struct s_firetalk_buddy *buddyiter;

	online = online?1:0;

	if ((buddyiter = firetalk_im_find_buddy(conn, nickname)) != NULL)
		if (buddyiter->online != online) {
			buddyiter->online = online;

			if (online != 0) {
				assert(buddyiter->away == 0);
				assert(buddyiter->typing == 0);
				assert(buddyiter->warnval == 0);
				assert(buddyiter->idletime == 0);
				if (strcmp(buddyiter->nickname, nickname) != 0) {
					free(buddyiter->nickname);
					buddyiter->nickname = strdup(nickname);
					if (buddyiter->nickname == NULL)
						abort();
				}
				if (conn->callbacks[FC_IM_BUDDYONLINE] != NULL)
					conn->callbacks[FC_IM_BUDDYONLINE](conn, conn->clientstruct, nickname);
			} else {
				buddyiter->away = 0;
				buddyiter->typing = 0;
				buddyiter->warnval = 0;
				buddyiter->idletime = 0;
				if (conn->callbacks[FC_IM_BUDDYOFFLINE] != NULL)
					conn->callbacks[FC_IM_BUDDYOFFLINE](conn, conn->clientstruct, nickname);
			}
		}
}

void firetalk_callback_im_buddyflags(client_t c, const char *const nickname, const int flags) {
	struct s_firetalk_handle
		*conn = firetalk_find_handle(c);
	struct s_firetalk_buddy *buddyiter;

	if ((buddyiter = firetalk_im_find_buddy(conn, nickname)) != NULL)
		if ((buddyiter->flags != flags) && (buddyiter->online == 1)) {
			buddyiter->flags = flags;
			if (conn->callbacks[FC_IM_BUDDYFLAGS] != NULL)
				conn->callbacks[FC_IM_BUDDYFLAGS](conn, conn->clientstruct, nickname, flags);
		}
}

void firetalk_callback_im_buddyaway(client_t c, const char *const nickname, const int away) {
	struct s_firetalk_handle
		*conn = firetalk_find_handle(c);
	struct s_firetalk_buddy *buddyiter;

	if ((buddyiter = firetalk_im_find_buddy(conn, nickname)) != NULL)
		if ((buddyiter->away != away) && (buddyiter->online == 1)) {
			buddyiter->away = away;
			if ((away == 1) && (conn->callbacks[FC_IM_BUDDYAWAY] != NULL))
				conn->callbacks[FC_IM_BUDDYAWAY](conn, conn->clientstruct, nickname);
			else if ((away == 0) && (conn->callbacks[FC_IM_BUDDYUNAWAY] != NULL))
				conn->callbacks[FC_IM_BUDDYUNAWAY](conn, conn->clientstruct, nickname);
		}
}

static void firetalk_im_insert_buddy(firetalk_t conn, const char *const name, const char *const group, const char *const friendly) {
	struct s_firetalk_buddy *newiter;

	newiter = calloc(1, sizeof(*newiter));
	if (newiter == NULL)
		abort();
	newiter->next = conn->buddy_head;
	conn->buddy_head = newiter;
	newiter->nickname = strdup(name);
	if (newiter->nickname == NULL)
		abort();
	newiter->group = strdup(group);
	if (newiter->group == NULL)
		abort();
	if (friendly == NULL)
		newiter->friendly = NULL;
	else {
		newiter->friendly = strdup(friendly);
		if (newiter->friendly == NULL)
			abort();
	}
}

void	firetalk_callback_buddyadded(client_t c, const char *const name, const char *const group, const char *const friendly) {
	struct s_firetalk_handle
		*conn = firetalk_find_handle(c);

	if (firetalk_im_find_buddy(conn, name) == NULL) {
		firetalk_im_insert_buddy(conn, name, group, friendly);
		if (conn->callbacks[FC_IM_BUDDYADDED] != NULL)
			conn->callbacks[FC_IM_BUDDYADDED](conn, conn->clientstruct, name, group, friendly);
	}
}

void	firetalk_callback_buddyremoved(client_t c, const char *const name, const char *const group) {
	struct s_firetalk_handle
		*conn = firetalk_find_handle(c);
	struct s_firetalk_buddy *iter;

	if (((iter = firetalk_im_find_buddy(conn, name)) != NULL) && ((group == NULL) || (strcmp(iter->group, group) == 0))) {
		firetalk_im_delete_buddy(conn, name);
		if (conn->callbacks[FC_IM_BUDDYREMOVED] != NULL)
			conn->callbacks[FC_IM_BUDDYREMOVED](conn, conn->clientstruct, name);
	}
}

void	firetalk_callback_typing(client_t c, const char *const name, const int typing) {
	struct s_firetalk_handle
		*conn = firetalk_find_handle(c);
	struct s_firetalk_buddy *buddyiter;

	assert(conn->username != NULL);
	assert(name != NULL);
	assert(typing >= 0);

	if (!conn->callbacks[FC_IM_TYPINGINFO])
		return;

	if ((buddyiter = firetalk_im_find_buddy(conn, name)) != NULL)
		if (buddyiter->online && (buddyiter->typing != typing)) {
			buddyiter->typing = typing;
			conn->callbacks[FC_IM_TYPINGINFO](conn, conn->clientstruct, buddyiter->nickname, typing);
		}
}

void	firetalk_callback_capabilities(client_t c, const char *const nickname, const char *const caps) {
	struct s_firetalk_handle
		*conn = firetalk_find_handle(c);
	struct s_firetalk_buddy *buddyiter;

	if (!conn->callbacks[FC_IM_CAPABILITIES])
		return;

	if ((buddyiter = firetalk_im_find_buddy(conn, nickname)) != NULL)
		if ((buddyiter->capabilities == NULL) || (strcmp(buddyiter->capabilities, caps) != 0)) {
			free(buddyiter->capabilities);
			buddyiter->capabilities = strdup(caps);
			conn->callbacks[FC_IM_CAPABILITIES](conn, conn->clientstruct, nickname, caps);
		}
}

void	firetalk_callback_warninfo(client_t c, const char *const nickname, const long warnval) {
	struct s_firetalk_handle
		*conn = firetalk_find_handle(c);
	struct s_firetalk_buddy *buddyiter;

	if (!conn->callbacks[FC_IM_EVILINFO])
		return;

	if ((buddyiter = firetalk_im_find_buddy(conn, nickname)) != NULL)
		if ((buddyiter->warnval != warnval) && (buddyiter->online == 1)) {
			buddyiter->warnval = warnval;
			conn->callbacks[FC_IM_EVILINFO](conn, conn->clientstruct, nickname, warnval);
		}
}

void firetalk_callback_error(client_t c, const int error, const char *const roomoruser, const char *const description) {
	struct s_firetalk_handle
		*conn = firetalk_find_handle(c);

	if (conn->callbacks[FC_ERROR])
		conn->callbacks[FC_ERROR](conn, conn->clientstruct, error, roomoruser, description);
}

void firetalk_callback_connectfailed(client_t c, const int error, const char *const description) {
	struct s_firetalk_handle
		*conn = firetalk_find_handle(c);

	if (conn->connected == FCS_NOTCONNECTED)
		return;

	close(conn->fd);
	conn->connected = FCS_NOTCONNECTED;
	if (conn->callbacks[FC_CONNECTFAILED])
		conn->callbacks[FC_CONNECTFAILED](conn, conn->clientstruct, error, description);
}

void firetalk_callback_disconnect(client_t c, const int error) {
	struct s_firetalk_handle
		*conn = firetalk_find_handle(c);
	struct s_firetalk_buddy *buddyiter, *buddynext;
	struct s_firetalk_deny *denyiter, *denynext;
	struct s_firetalk_room *roomiter, *roomnext;

	if (conn->connected == FCS_NOTCONNECTED)
		return;
	close(conn->fd);

	if (conn->username != NULL) {
		free(conn->username);
		conn->username = NULL;
	}

	for (buddyiter = conn->buddy_head; buddyiter != NULL; buddyiter = buddynext) {
		buddynext = buddyiter->next;
		buddyiter->next = NULL;
		free(buddyiter->nickname);
		buddyiter->nickname = NULL;
		free(buddyiter->group);
		buddyiter->group = NULL;
		free(buddyiter->friendly);
		buddyiter->friendly = NULL;
		if (buddyiter->capabilities != NULL) {
			free(buddyiter->capabilities);
			buddyiter->capabilities = NULL;
		}
		free(buddyiter);
	}
	conn->buddy_head = NULL;

	for (denyiter = conn->deny_head; denyiter != NULL; denyiter = denynext) {
		denynext = denyiter->next;
		denyiter->next = NULL;
		free(denyiter->nickname);
		denyiter->nickname = NULL;
		free(denyiter);
	}
	conn->deny_head = NULL;

	for (roomiter = conn->room_head; roomiter != NULL; roomiter = roomnext) {
		struct s_firetalk_member *memberiter;
		struct s_firetalk_member *membernext;

		roomnext = roomiter->next;
		roomiter->next = NULL;
		for (memberiter = roomiter->member_head; memberiter != NULL; memberiter = membernext) {
			membernext = memberiter->next;
			memberiter->next = NULL;
			free(memberiter->nickname);
			memberiter->nickname = NULL;
			free(memberiter);
		}
		roomiter->member_head = NULL;
		free(roomiter->name);
		roomiter->name = NULL;
		free(roomiter);
	}
	conn->room_head = NULL;

	if (conn->connected == FCS_ACTIVE) {
		conn->connected = FCS_NOTCONNECTED;
		if (conn->callbacks[FC_DISCONNECT])
			conn->callbacks[FC_DISCONNECT](conn, conn->clientstruct, error);
	} else
		conn->connected = FCS_NOTCONNECTED;
}

void firetalk_callback_gotinfo(client_t c, const char *const nickname, const char *const info, const int warning, const long online, const long idle, const int flags) {
	struct s_firetalk_handle
		*conn = firetalk_find_handle(c);

	if (conn->callbacks[FC_IM_GOTINFO])
		conn->callbacks[FC_IM_GOTINFO](conn, conn->clientstruct, nickname, info, warning, online, idle, flags);
}

void firetalk_callback_idleinfo(client_t c, const char *const nickname, const long idletime) {
	struct s_firetalk_handle
		*conn = firetalk_find_handle(c);
	struct s_firetalk_buddy *buddyiter;

	if (!conn->callbacks[FC_IM_IDLEINFO])
		return;

	if ((buddyiter = firetalk_im_find_buddy(conn, nickname)) != NULL)
		if ((buddyiter->idletime != idletime) && (buddyiter->online == 1)) {
			buddyiter->idletime = idletime;
			conn->callbacks[FC_IM_IDLEINFO](conn, conn->clientstruct, nickname, idletime);
		}
}

void firetalk_callback_statusinfo(client_t c, const char *const nickname, const char *const message) {
	struct s_firetalk_handle
		*conn = firetalk_find_handle(c);
	struct s_firetalk_buddy *buddyiter;

	if (conn->callbacks[FC_IM_STATUSINFO])
		conn->callbacks[FC_IM_STATUSINFO](conn, conn->clientstruct, nickname, message);
}

void firetalk_callback_doinit(client_t c, const char *const nickname) {
	struct s_firetalk_handle
		*conn = firetalk_find_handle(c);

	if (conn->callbacks[FC_DOINIT])
		conn->callbacks[FC_DOINIT](conn, conn->clientstruct, nickname);
}

void firetalk_callback_setidle(client_t c, long *const idle) {
	struct s_firetalk_handle
		*conn = firetalk_find_handle(c);

	if (conn->callbacks[FC_SETIDLE])
		conn->callbacks[FC_SETIDLE](conn, conn->clientstruct, idle);
}

void firetalk_callback_eviled(client_t c, const int newevil, const char *const eviler) {
	struct s_firetalk_handle
		*conn = firetalk_find_handle(c);

	if (conn->callbacks[FC_EVILED])
		conn->callbacks[FC_EVILED](conn, conn->clientstruct, newevil, eviler);
}

void firetalk_callback_newnick(client_t c, const char *const nickname) {
	struct s_firetalk_handle
		*conn = firetalk_find_handle(c);

	if (conn->callbacks[FC_NEWNICK])
		conn->callbacks[FC_NEWNICK](conn, conn->clientstruct, nickname);
}

void firetalk_callback_passchanged(client_t c) {
	struct s_firetalk_handle
		*conn = firetalk_find_handle(c);

	if (conn->callbacks[FC_PASSCHANGED])
		conn->callbacks[FC_PASSCHANGED](conn, conn->clientstruct);
}

void firetalk_callback_user_nickchanged(client_t c, const char *const oldnick, const char *const newnick) {
	struct s_firetalk_handle
		*conn = firetalk_find_handle(c);
	struct s_firetalk_buddy *buddyiter;
	struct s_firetalk_room *roomiter;
	struct s_firetalk_member *memberiter;
	char *tempstr;

	if ((buddyiter = firetalk_im_find_buddy(conn, oldnick)) != NULL)
		if (strcmp(buddyiter->nickname, newnick) != 0) {
			tempstr = buddyiter->nickname;
			buddyiter->nickname = strdup(newnick);
			if (buddyiter->nickname == NULL)
				abort();
			if (conn->callbacks[FC_IM_USER_NICKCHANGED])
				conn->callbacks[FC_IM_USER_NICKCHANGED](conn, conn->clientstruct, tempstr, newnick);
			free(tempstr);
		}

	for (roomiter = conn->room_head; roomiter != NULL; roomiter = roomiter->next)
		for (memberiter = roomiter->member_head; memberiter != NULL; memberiter = memberiter->next)
			if (firetalk_protocols[conn->protocol]->comparenicks(memberiter->nickname, oldnick) == FE_SUCCESS) {
				if (strcmp(memberiter->nickname, newnick) != 0) {
					tempstr = memberiter->nickname;
					memberiter->nickname = strdup(newnick);
					if (memberiter->nickname == NULL)
						abort();
					if (conn->callbacks[FC_CHAT_USER_NICKCHANGED])
						conn->callbacks[FC_CHAT_USER_NICKCHANGED](conn, conn->clientstruct, roomiter->name, tempstr, newnick);
					free(tempstr);
				}
				break;
			}
}

void firetalk_callback_chat_joined(client_t c, const char *const room) {
	struct s_firetalk_handle
		*conn = firetalk_find_handle(c);

	if (firetalk_chat_internal_add_room(conn, room) != FE_SUCCESS)
		return;
}

void firetalk_callback_chat_left(client_t c, const char *const room) {
	struct s_firetalk_handle
		*conn = firetalk_find_handle(c);

	if (firetalk_chat_internal_remove_room(conn, room) != FE_SUCCESS)
		return;
	if (conn->callbacks[FC_CHAT_LEFT])
		conn->callbacks[FC_CHAT_LEFT](conn, conn->clientstruct, room);
}

void firetalk_callback_chat_kicked(client_t c, const char *const room, const char *const by, const char *const reason) {
	struct s_firetalk_handle
		*conn = firetalk_find_handle(c);

	if (firetalk_chat_internal_remove_room(conn, room) != FE_SUCCESS)
		return;
	if (conn->callbacks[FC_CHAT_KICKED])
		conn->callbacks[FC_CHAT_KICKED](conn, conn->clientstruct, room, by, reason);
}

void firetalk_callback_chat_getmessage(client_t c, const char *const room, const char *const from, const int automessage, const char *const message) {
	struct s_firetalk_handle
		*conn = firetalk_find_handle(c);

	if (conn->callbacks[FC_CHAT_GETMESSAGE])
		conn->callbacks[FC_CHAT_GETMESSAGE](conn, conn->clientstruct, room, from, automessage, message);
}

void firetalk_callback_chat_getaction(client_t c, const char *const room, const char *const from, const int automessage, const char *const message) {
	struct s_firetalk_handle
		*conn = firetalk_find_handle(c);

	if (conn->callbacks[FC_CHAT_GETACTION])
		conn->callbacks[FC_CHAT_GETACTION](conn, conn->clientstruct, room, from, automessage, message);
}

void firetalk_callback_chat_invited(client_t c, const char *const room, const char *const from, const char *const message) {
	struct s_firetalk_handle
		*conn = firetalk_find_handle(c);

	if (conn->callbacks[FC_CHAT_INVITED])
		conn->callbacks[FC_CHAT_INVITED](conn, conn->clientstruct, room, from, message);
}

void firetalk_callback_chat_user_joined(client_t c, const char *const room, const char *const who, const char *const extra) {
	struct s_firetalk_handle
		*conn = firetalk_find_handle(c);
	struct s_firetalk_room *iter;

	iter = firetalk_find_room(conn, room);
	if (iter == NULL)
		return;

	if (who == NULL) {
		if (iter->sentjoin == 0) {
			iter->sentjoin = 1;
			if (conn->callbacks[FC_CHAT_JOINED])
				conn->callbacks[FC_CHAT_JOINED](conn, conn->clientstruct, room);
		}
	} else {
		if (firetalk_chat_internal_add_member(conn, room, who) != FE_SUCCESS)
			return;
		if (iter->sentjoin == 1)
			if (conn->callbacks[FC_CHAT_USER_JOINED])
				conn->callbacks[FC_CHAT_USER_JOINED](conn, conn->clientstruct, room, who, extra);
	}
}

void firetalk_callback_chat_user_left(client_t c, const char *const room, const char *const who, const char *const reason) {
	struct s_firetalk_handle
		*conn = firetalk_find_handle(c);

	if (firetalk_chat_internal_remove_member(conn, room, who) != FE_SUCCESS)
		return;
	if (conn->callbacks[FC_CHAT_USER_LEFT])
		conn->callbacks[FC_CHAT_USER_LEFT](conn, conn->clientstruct, room, who, reason);
}

void firetalk_callback_chat_user_quit(client_t c, const char *const who, const char *const reason) {
	struct s_firetalk_handle
		*conn = firetalk_find_handle(c);
	struct s_firetalk_room *roomiter;
	struct s_firetalk_member *memberiter, *membernext;
	
	for (roomiter = conn->room_head; roomiter != NULL; roomiter = roomiter->next)
		for (memberiter = roomiter->member_head; memberiter != NULL; memberiter = membernext) {
			membernext = memberiter->next;
			if (firetalk_protocols[conn->protocol]->comparenicks(memberiter->nickname, who) == FE_SUCCESS)
				firetalk_callback_chat_user_left(c, roomiter->name, who, reason);
		}
}

void firetalk_callback_chat_gottopic(client_t c, const char *const room, const char *const topic, const char *const author) {
	struct s_firetalk_handle
		*conn = firetalk_find_handle(c);
	struct s_firetalk_room *r;

	r = firetalk_find_room(conn, room);
	if (r != NULL)
		if (conn->callbacks[FC_CHAT_GOTTOPIC])
			conn->callbacks[FC_CHAT_GOTTOPIC](conn, conn->clientstruct, room, topic, author);
}

#ifdef RAWIRCMODES
void firetalk_callback_chat_modechanged(client_t c, const char *const room, const char *const mode, const char *const by) {
	struct s_firetalk_handle
		*conn = firetalk_find_handle(c);

	if (conn->callbacks[FC_CHAT_MODECHANGED])
		conn->callbacks[FC_CHAT_MODECHANGED](conn, conn->clientstruct, room, mode, by);
}
#endif

void firetalk_callback_chat_user_opped(client_t c, const char *const room, const char *const who, const char *const by) {
	struct s_firetalk_handle
		*conn = firetalk_find_handle(c);
	struct s_firetalk_room *r;
	struct s_firetalk_member *m;

	r = firetalk_find_room(conn, room);
	if (r == NULL)
		return;
	m = firetalk_find_member(conn, r, who);
	if (m == NULL)
		return;
	if (m->admin == 0) {
		m->admin = 1;
		if (conn->callbacks[FC_CHAT_USER_OPPED])
			conn->callbacks[FC_CHAT_USER_OPPED](conn, conn->clientstruct, room, who, by);
	}
}

void firetalk_callback_chat_user_deopped(client_t c, const char *const room, const char *const who, const char *const by) {
	struct s_firetalk_handle
		*conn = firetalk_find_handle(c);
	struct s_firetalk_room *r;
	struct s_firetalk_member *m;

	r = firetalk_find_room(conn, room);
	if (r == NULL)
		return;
	m = firetalk_find_member(conn, r, who);
	if (m == NULL)
		return;
	if (m->admin == 1) {
		m->admin = 0;
		if (conn->callbacks[FC_CHAT_USER_DEOPPED])
			conn->callbacks[FC_CHAT_USER_DEOPPED](conn, conn->clientstruct, room, who, by);
	}
}

void firetalk_callback_chat_keychanged(client_t c, const char *const room, const char *const what, const char *const by) {
	struct s_firetalk_handle
		*conn = firetalk_find_handle(c);

	if (conn->callbacks[FC_CHAT_KEYCHANGED])
		conn->callbacks[FC_CHAT_KEYCHANGED](conn, conn->clientstruct, room, what, by);
}

void firetalk_callback_chat_opped(client_t c, const char *const room, const char *const by) {
	struct s_firetalk_handle
		*conn = firetalk_find_handle(c);
	struct s_firetalk_room *r;

	r = firetalk_find_room(conn,room);
	if (r == NULL)
		return;
	if (r->admin == 0)
		r->admin = 1;
	else
		return;
	if (conn->callbacks[FC_CHAT_OPPED])
		conn->callbacks[FC_CHAT_OPPED](conn, conn->clientstruct, room, by);
}

void firetalk_callback_chat_deopped(client_t c, const char *const room, const char *const by) {
	struct s_firetalk_handle
		*conn = firetalk_find_handle(c);
	struct s_firetalk_room *r;

	r = firetalk_find_room(conn,room);
	if (r == NULL)
		return;
	if (r->admin == 1)
		r->admin = 0;
	else
		return;
	if (conn->callbacks[FC_CHAT_DEOPPED])
		conn->callbacks[FC_CHAT_DEOPPED](conn, conn->clientstruct, room, by);
}

void firetalk_callback_chat_user_kicked(client_t c, const char *const room, const char *const who, const char *const by, const char *const reason) {
	struct s_firetalk_handle
		*conn = firetalk_find_handle(c);

	if (firetalk_chat_internal_remove_member(conn, room, who) != FE_SUCCESS)
		return;
	if (conn->callbacks[FC_CHAT_USER_KICKED])
		conn->callbacks[FC_CHAT_USER_KICKED](conn, conn->clientstruct, room, who, by, reason);
}

char *firetalk_subcode_get_request_reply(client_t c, const char *const command) {
	struct s_firetalk_handle
		*conn = firetalk_find_handle(c);
	struct s_firetalk_subcode_callback *iter;

	for (iter = conn->subcode_request_head; iter != NULL; iter = iter->next)
		if (strcmp(command, iter->command) == 0)
			if (iter->staticresp != NULL)
				return(iter->staticresp);
	return(NULL);
}

void firetalk_callback_subcode_request(client_t c, const char *const from, const char *const command, char *args) {
	struct s_firetalk_handle
		*conn = firetalk_find_handle(c);
	struct s_firetalk_subcode_callback *iter;
	enum firetalk_connectstate connectedsave = conn->connected; /* nasty hack: some IRC servers send CTCP VERSION requests during signon, before 001, and demand a reply; idiots */

	conn->connected = FCS_ACTIVE;

	for (iter = conn->subcode_request_head; iter != NULL; iter = iter->next)
		if (strcmp(command, iter->command) == 0) {
			if (iter->staticresp != NULL)
				firetalk_subcode_send_reply(conn, from, command, iter->staticresp);
			else {
				isonline_hack = from;
				iter->callback(conn, conn->clientstruct, from, command, args);
				isonline_hack = NULL;
			}

			conn->connected = connectedsave;

			return;
		}

	if (strcmp(command, "ACTION") == 0)
		/* we don't support chatroom subcodes, so we're just going to assume that this is a private ACTION and let the protocol code handle the other case */
		firetalk_callback_im_getaction(c, from, 0, args);
	else if (strcmp(command, "VERSION") == 0)
		firetalk_subcode_send_reply(conn, from, "VERSION", PACKAGE_NAME ":" PACKAGE_VERSION ":unknown");
	else if (strcmp(command, "CLIENTINFO") == 0)
		firetalk_subcode_send_reply(conn, from, "CLIENTINFO", "CLIENTINFO PING SOURCE TIME VERSION");
	else if (strcmp(command, "PING") == 0) {
		if (args != NULL)
			firetalk_subcode_send_reply(conn, from, "PING", args);
		else
			firetalk_subcode_send_reply(conn, from, "PING", "");
	} else if (strcmp(command, "TIME") == 0) {
		time_t	temptime;
		char	tempbuf[64];
		size_t	s;

		time(&temptime);
		strncpy(tempbuf, ctime(&temptime), sizeof(tempbuf)-1);
		tempbuf[sizeof(tempbuf)-1] = 0;
		s = strlen(tempbuf);
		if (s > 0)
			tempbuf[s-1] = '\0';
		firetalk_subcode_send_reply(conn, from, "TIME", tempbuf);
	} else if ((strcmp(command,"DCC") == 0) && (args != NULL) && (strncasecmp(args, "SEND ", 5) == 0)) {
		/* DCC send */
		struct in_addr addr;
		unsigned long ip;
		long	size = -1;
		uint16_t port;
		char	**myargs;
#ifdef _FC_USE_IPV6
		struct in6_addr addr6;
		struct in6_addr *sendaddr6 = NULL;
#endif
		myargs = firetalk_parse_subcode_args(&args[5]);
		if ((myargs[0] != NULL) && (myargs[1] != NULL) && (myargs[2] != NULL)) {
			/* valid dcc send */
			if (myargs[3] != NULL) {
				size = atol(myargs[3]);
#ifdef _FC_USE_IPV6
				if (myargs[4] != NULL) {
					/* ipv6-enabled dcc */
					inet_pton(AF_INET6,myargs[4],&addr6);
					sendaddr6 = &addr6;
				}
#endif
			}
			sscanf(myargs[1], "%lu", &ip);
			ip = htonl(ip);
			memcpy(&addr.s_addr, &ip, 4);
			port = (uint16_t)atoi(myargs[2]);
			firetalk_callback_file_offer(c, from, myargs[0], size, inet_ntoa(addr), NULL, port, FF_TYPE_DCC);
		}
	} else if (conn->subcode_request_default != NULL)
		conn->subcode_request_default->callback(conn, conn->clientstruct, from, command, args);

	conn->connected = connectedsave;
}

void firetalk_callback_subcode_reply(client_t c, const char *const from, const char *const command, const char *const args) {
	struct s_firetalk_handle
		*conn = firetalk_find_handle(c);
	struct s_firetalk_subcode_callback *iter;

	for (iter = conn->subcode_reply_head; iter != NULL; iter = iter->next)
		if (strcmp(command, iter->command) == 0) {
			isonline_hack = from;
			iter->callback(conn, conn->clientstruct, from, command, args);
			isonline_hack = NULL;
			return;
		}

	if (conn->subcode_reply_default != NULL)
		conn->subcode_reply_default->callback(conn, conn->clientstruct, from, command, args);
}

/* size may be -1 if unknown (0 is valid) */
void firetalk_callback_file_offer(client_t c, const char *const from, const char *const filename, const long size, const char *const ipstring, const char *const ip6string, const uint16_t port, const int type) {
	struct s_firetalk_handle
		*conn = firetalk_find_handle(c);
	struct s_firetalk_file *iter;

	iter = conn->file_head;
	conn->file_head = calloc(1, sizeof(struct s_firetalk_file));
	if (conn->file_head == NULL)
		abort();
	conn->file_head->who = strdup(from);
	if (conn->file_head->who == NULL)
		abort();
	conn->file_head->filename = strdup(filename);
	if (conn->file_head->filename == NULL)
		abort();
	conn->file_head->size = size;
	conn->file_head->bytes = 0;
	conn->file_head->acked = 0;
	conn->file_head->state = FF_STATE_WAITLOCAL;
	conn->file_head->direction = FF_DIRECTION_RECEIVING;
	conn->file_head->sockfd = -1;
	conn->file_head->filefd = -1;
	conn->file_head->port = htons(port);
	conn->file_head->type = type;
	conn->file_head->next = iter;
	conn->file_head->clientfilestruct = NULL;
	if (inet_aton(ipstring, &conn->file_head->inet_ip) == 0) {
		firetalk_file_cancel(c, conn->file_head);
		return;
	}
#ifdef _FC_USE_IPV6
	conn->file_head->tryinet6 = 0;
	if (ip6string)
		if (inet_pton(AF_INET6, ip6string, &conn->file_head->inet6_ip) != 0)
			conn->file_head->tryinet6 = 1;
#endif
	if (conn->callbacks[FC_FILE_OFFER])
		conn->callbacks[FC_FILE_OFFER](conn, conn->clientstruct, (void *)conn->file_head, from, filename, size);
}

void firetalk_handle_receive(struct s_firetalk_handle *c, struct s_firetalk_file *filestruct) {
	/* we have to copy from sockfd to filefd until we run out, then send the packet */
	static char buffer[4096];
	unsigned long netbytes;
	ssize_t s;

	while ((s = recv(filestruct->sockfd, buffer, 4096, MSG_DONTWAIT)) == 4096) {
		if (write(filestruct->filefd, buffer, 4096) != 4096) {
			if (c->callbacks[FC_FILE_ERROR])
				c->callbacks[FC_FILE_ERROR](c, c->clientstruct, filestruct, filestruct->clientfilestruct, FE_IOERROR);
			firetalk_file_cancel(c, filestruct);
			return;
		}
		filestruct->bytes += 4096;
	}
	if (s != -1) {
		if (write(filestruct->filefd, buffer, (size_t)s) != s) {
			if (c->callbacks[FC_FILE_ERROR])
				c->callbacks[FC_FILE_ERROR](c, c->clientstruct, filestruct, filestruct->clientfilestruct, FE_IOERROR);
			firetalk_file_cancel(c, filestruct);
			return;
		}
		filestruct->bytes += s;
	}
	if (filestruct->type == FF_TYPE_DCC) {
		netbytes = htonl((uint32_t)filestruct->bytes);
		if (write(filestruct->sockfd, &netbytes, 4) != 4) {
			if (c->callbacks[FC_FILE_ERROR])
				c->callbacks[FC_FILE_ERROR](c, c->clientstruct, filestruct, filestruct->clientfilestruct, FE_IOERROR);
			firetalk_file_cancel(c, filestruct);
			return;
		}
	}
	if (c->callbacks[FC_FILE_PROGRESS])
		c->callbacks[FC_FILE_PROGRESS](c, c->clientstruct, filestruct, filestruct->clientfilestruct, filestruct->bytes, filestruct->size);
	if (filestruct->bytes == filestruct->size) {
		if (c->callbacks[FC_FILE_FINISH])
			c->callbacks[FC_FILE_FINISH](c, c->clientstruct, filestruct, filestruct->clientfilestruct, filestruct->size);
		firetalk_file_cancel(c, filestruct);
	}
}

void firetalk_handle_send(struct s_firetalk_handle *c, struct s_firetalk_file *filestruct) {
	/* we have to copy from filefd to sockfd until we run out or sockfd refuses the data */
	static char buffer[4096];
	ssize_t s;

	while ((s = read(filestruct->filefd, buffer, 4096)) == 4096) {
		if ((s = send(filestruct->sockfd, buffer, 4096, MSG_DONTWAIT|MSG_NOSIGNAL)) != 4096) {
			lseek(filestruct->filefd, -(4096 - s), SEEK_CUR);
			filestruct->bytes += s;
			if (c->callbacks[FC_FILE_PROGRESS])
				c->callbacks[FC_FILE_PROGRESS](c, c->clientstruct, filestruct, filestruct->clientfilestruct, filestruct->bytes, filestruct->size);
			return;
		}
		filestruct->bytes += s;
		if (c->callbacks[FC_FILE_PROGRESS])
			c->callbacks[FC_FILE_PROGRESS](c, c->clientstruct, filestruct, filestruct->clientfilestruct, filestruct->bytes, filestruct->size);
		if (filestruct->type == FF_TYPE_DCC) {
			uint32_t acked = 0;

			while (recv(filestruct->sockfd, &acked, 4, MSG_DONTWAIT) == 4)
				filestruct->acked = ntohl(acked);
		}
	}
	if (send(filestruct->sockfd, buffer, s, MSG_NOSIGNAL) != s) {
		if (c->callbacks[FC_FILE_ERROR])
			c->callbacks[FC_FILE_ERROR](c, c->clientstruct, filestruct, filestruct->clientfilestruct, FE_IOERROR);
		firetalk_file_cancel(c, filestruct);
		return;
	}
	filestruct->bytes += s;
	if (filestruct->type == FF_TYPE_DCC) {
		while (filestruct->acked < (uint32_t)filestruct->bytes) {
			uint32_t acked = 0;

			if (recv(filestruct->sockfd, &acked, 4, 0) == 4)
				filestruct->acked = ntohl(acked);
		}
	}
	if (c->callbacks[FC_FILE_PROGRESS])
		c->callbacks[FC_FILE_PROGRESS](c, c->clientstruct, filestruct, filestruct->clientfilestruct, filestruct->bytes, filestruct->size);
	if (c->callbacks[FC_FILE_FINISH])
		c->callbacks[FC_FILE_FINISH](c, c->clientstruct, filestruct, filestruct->clientfilestruct, filestruct->bytes);
	firetalk_file_cancel(c, filestruct);
}

/* External function definitions */

const char *firetalk_strprotocol(const enum firetalk_protocol p) {
	if ((p >= 0) && (p < FP_MAX))
		return(firetalk_protocols[p]->strprotocol);
	return(NULL);
}

const char *firetalk_strerror(const fte_t e) {
	switch (e) {
		case FE_SUCCESS:
			return("Success");
		case FE_CONNECT:
			return("Connection failed");
		case FE_NOMATCH:
			return("Usernames do not match");
		case FE_PACKET:
			return("Packet transfer error");
		case FE_RECONNECTING:
			return("Server wants us to reconnect elsewhere");
		case FE_BADUSERPASS:
			return("Invalid username or password");
		case FE_SEQUENCE:
			return("Invalid sequence number from server");
		case FE_FRAMETYPE:
			return("Invalid frame type from server");
		case FE_PACKETSIZE:
			return("Packet too long");
		case FE_SERVER:
			return("Server problem; try again later");
		case FE_UNKNOWN:
			return("Unknown error");
		case FE_BLOCKED:
			return("You are blocked");
		case FE_WEIRDPACKET:
			return("Unknown packet received from server");
		case FE_CALLBACKNUM:
			return("Invalid callback number");
		case FE_BADUSER:
			return("Invalid username");
		case FE_NOTFOUND:
			return("Username not found in list");
		case FE_DISCONNECT:
			return("Server disconnected");
		case FE_SOCKET:
			return("Unable to create socket");
		case FE_RESOLV:
			return("Unable to resolve hostname");
		case FE_VERSION:
			return("Wrong server version");
		case FE_USERUNAVAILABLE:
			return("User is currently unavailable");
		case FE_USERINFOUNAVAILABLE:
			return("User information is currently unavailable");
		case FE_TOOFAST:
			return("You are sending messages too fast; last message was dropped");
		case FE_ROOMUNAVAILABLE:
			return("Chat room is currently unavailable");
		case FE_INCOMINGERROR:
			return("Incoming message delivery failure");
		case FE_USERDISCONNECT:
			return("User disconnected");
		case FE_INVALIDFORMAT:
			return("Server response was formatted incorrectly");
		case FE_IDLEFAST:
			return("You have requested idle to be reset too fast");
		case FE_BADROOM:
			return("Invalid room name");
		case FE_BADMESSAGE:
			return("Invalid message (too long?)");
		case FE_MESSAGETRUNCATED:
			return("Message truncated");
		case FE_BADPROTO:
			return("Invalid protocol");
		case FE_NOTCONNECTED:
			return("Not connected");
		case FE_BADCONNECTION:
			return("Invalid connection number");
		case FE_NOPERMS:
			return("No permission to perform operation");
		case FE_NOCHANGEPASS:
			return("Unable to change password");
		case FE_DUPEROOM:
			return("Room already in list");
		case FE_IOERROR:
        		return("Input/output error");
		case FE_BADHANDLE:
        		return("Invalid handle");
		case FE_TIMEOUT:
			return("Operation timed out");
		default:
			return("Invalid error number");
	}
}

firetalk_t firetalk_create_handle(const int protocol, void *clientstruct) {
	struct s_firetalk_handle *c;

	if ((protocol < 0) || (protocol >= FP_MAX)) {
		firetalkerror = FE_BADPROTO;
		return(NULL);
	}
	c = handle_head;
	handle_head = calloc(1, sizeof(*handle_head));
	if (handle_head == NULL)
		abort();
	handle_head->buffer = calloc(1, firetalk_protocols[protocol]->default_buffersize);
	if (handle_head->buffer == NULL)
		abort();
	handle_head->clientstruct = clientstruct;
	handle_head->next = c;
	handle_head->connected = FCS_NOTCONNECTED;
	handle_head->protocol = protocol;
	handle_head->handle = firetalk_protocols[protocol]->create_handle();
	return(handle_head);
}

void firetalk_destroy_handle(firetalk_t conn) {
	VERIFYCONN;

	assert(conn->deleted == 0);
	assert(conn->handle != NULL);

	firetalk_protocols[conn->protocol]->destroy_handle(conn->handle);
	conn->handle = NULL;
	conn->deleted = 1;
}

fte_t	firetalk_disconnect(firetalk_t conn) {
	VERIFYCONN;

	if (conn->connected == FCS_NOTCONNECTED)
		return(FE_NOTCONNECTED);

	return(firetalk_protocols[conn->protocol]->disconnect(conn->handle));
}

fte_t	firetalk_signon(firetalk_t conn, const char *server, short port, const char *const username) {
	VERIFYCONN;

	if (conn->connected != FCS_NOTCONNECTED) {
		firetalk_disconnect(conn);
		conn->connected = FCS_NOTCONNECTED;
	}

	free(conn->username);
	conn->username = strdup(username);
	if (conn->username == NULL)
		abort();
	conn->bufferpos = 0;

	if (server == NULL)
		server = firetalk_protocols[conn->protocol]->default_server;

	if (port == 0)
		port = firetalk_protocols[conn->protocol]->default_port;

	errno = 0;
	conn->fd = firetalk_internal_connect_host_addr(server, port, &(conn->remote_addr)
#ifdef _FC_USE_IPV6
		, &(conn->remote_addr6)
#endif
	);

	if (conn->fd != -1) {
		conn->connected = FCS_WAITING_SYNACK;
		return(FE_SUCCESS);
	} else
		return(firetalkerror);
}

fte_t	firetalk_handle_synack(firetalk_t conn) {
	int i;
	unsigned int o = sizeof(int);

	if (getsockopt(conn->fd, SOL_SOCKET, SO_ERROR, &i, &o)) {
		close(conn->fd);
		conn->connected = FCS_NOTCONNECTED;
		if (conn->callbacks[FC_CONNECTFAILED])
			conn->callbacks[FC_CONNECTFAILED](conn, conn->clientstruct, FE_SOCKET, strerror(errno));
		return(FE_SOCKET);
	}

	if (i != 0) {
		close(conn->fd);
		conn->connected = FCS_NOTCONNECTED;
		if (conn->callbacks[FC_CONNECTFAILED])
			conn->callbacks[FC_CONNECTFAILED](conn, conn->clientstruct, FE_CONNECT, strerror(i));
		return(FE_CONNECT);
	}

	conn->connected = FCS_WAITING_SIGNON;
	i = firetalk_protocols[conn->protocol]->signon(conn->handle, conn->username);
	if (i != FE_SUCCESS)
		return(i);

	return(FE_SUCCESS);
}

void firetalk_callback_connected(client_t c) {
	unsigned int l;
	struct sockaddr_in addr;
	struct s_firetalk_handle
		*conn = firetalk_find_handle(c);

	conn->connected = FCS_ACTIVE;
	l = (unsigned int)sizeof(struct sockaddr_in);
	getsockname(conn->fd, (struct sockaddr *)&addr, &l);
	memcpy(&conn->localip, &addr.sin_addr.s_addr, 4);
	conn->localip = htonl((uint32_t)conn->localip);

	if (conn->callbacks[FC_CONNECTED])
		conn->callbacks[FC_CONNECTED](conn, conn->clientstruct);
}

fte_t	firetalk_handle_file_synack(firetalk_t conn, struct s_firetalk_file *file) {
	int i;
	unsigned int o = sizeof(int);

	if (getsockopt(file->sockfd, SOL_SOCKET, SO_ERROR, &i, &o)) {
		firetalk_file_cancel(conn, file);
		return(FE_SOCKET);
	}

	if (i != 0) {
		firetalk_file_cancel(conn, file);
		return(FE_CONNECT);
	}

	file->state = FF_STATE_TRANSFERRING;

	if (conn->callbacks[FC_FILE_START])
		conn->callbacks[FC_FILE_START](conn, conn->clientstruct, file, file->clientfilestruct);
	return(FE_SUCCESS);
}

enum firetalk_protocol firetalk_get_protocol(firetalk_t conn) {
	VERIFYCONN;

	return(conn->protocol);
}

fte_t	firetalk_register_callback(firetalk_t conn, const int type, void (*function)(firetalk_t, void *, ...)) {
	VERIFYCONN;

	if (type < 0 || type >= FC_MAX)
		return(FE_CALLBACKNUM);
	conn->callbacks[type] = function;
	return(FE_SUCCESS);
}

fte_t	firetalk_im_add_buddy(firetalk_t conn, const char *const name, const char *const group, const char *const friendly) {
	struct s_firetalk_buddy *iter;

	VERIFYCONN;

	if ((iter = firetalk_im_find_buddy(conn, name)) != NULL) {
		if (!((strcmp(iter->group, group) == 0) && (((iter->friendly == NULL) && (friendly == NULL)) || ((iter->friendly != NULL) && (friendly != NULL) && (strcmp(iter->friendly, friendly) == 0))))) {
			/* user is in buddy list somewhere other than where the clients wants it */
			if (conn->connected != FCS_NOTCONNECTED) {
				fte_t	ret;

				ret = firetalk_protocols[conn->protocol]->im_remove_buddy(conn->handle, iter->nickname, iter->group);
				if (ret != FE_SUCCESS)
					return(ret);
			}
			free(iter->group);
			iter->group = strdup(group);
			if (iter->group == NULL)
				abort();
			free(iter->friendly);
			if (friendly == NULL)
				iter->friendly = NULL;
			else {
				iter->friendly = strdup(friendly);
				if (iter->friendly == NULL)
					abort();
			}
		}
	} else
		firetalk_im_insert_buddy(conn, name, group, friendly);

        if (conn->connected != FCS_NOTCONNECTED) {
		fte_t	ret;

		ret = firetalk_protocols[conn->protocol]->im_add_buddy(conn->handle, name, group, friendly);
		if (ret != FE_SUCCESS)
			return(ret);
	}

	if ((isonline_hack != NULL) && (firetalk_protocols[conn->protocol]->comparenicks(name, isonline_hack) == FE_SUCCESS))
		firetalk_callback_im_buddyonline(conn->handle, isonline_hack, 1);

	return(FE_SUCCESS);
}

fte_t	firetalk_im_add_deny(firetalk_t conn, const char *const nickname) {
	int ret;

	VERIFYCONN;

	if (conn->connected != FCS_ACTIVE)
		return(FE_NOTCONNECTED);

	ret = firetalk_im_internal_add_deny(conn,nickname);
	if (ret != FE_SUCCESS)
		return(ret);

	return(firetalk_protocols[conn->protocol]->im_add_deny(conn->handle,nickname));
}

fte_t	firetalk_im_remove_deny(firetalk_t conn, const char *const nickname) {
	int ret;

	VERIFYCONN;

	if (conn->connected != FCS_ACTIVE)
		return(FE_NOTCONNECTED);

	ret = firetalk_im_internal_remove_deny(conn,nickname);
	if (ret != FE_SUCCESS)
		return(ret);

	return(firetalk_protocols[conn->protocol]->im_remove_deny(conn->handle,nickname));
}

fte_t	firetalk_im_upload_buddies(firetalk_t conn) {
	VERIFYCONN;

	if (conn->connected != FCS_ACTIVE)
		return(FE_NOTCONNECTED);

	return(firetalk_protocols[conn->protocol]->im_upload_buddies(conn->handle));
}

fte_t	firetalk_im_upload_denies(firetalk_t conn) {
	VERIFYCONN;

	if (conn->connected != FCS_ACTIVE)
		return(FE_NOTCONNECTED);

	return(firetalk_protocols[conn->protocol]->im_upload_denies(conn->handle));
}

fte_t	firetalk_im_send_message(firetalk_t conn, const char *const dest, const char *const message, const int auto_flag) {
	fte_t	e;

	VERIFYCONN;

	if ((conn->connected != FCS_ACTIVE) && (strcasecmp(dest, ":RAW") != 0))
		return(FE_NOTCONNECTED);

	e = firetalk_protocols[conn->protocol]->im_send_message(conn->handle, dest, message, auto_flag);
	if (e != FE_SUCCESS)
		return(e);

	e = firetalk_protocols[conn->protocol]->periodic(conn);
	if (e != FE_SUCCESS && e != FE_IDLEFAST)
		return(e);

	return(FE_SUCCESS);
}

fte_t	firetalk_im_send_action(firetalk_t conn, const char *const dest, const char *const message, const int auto_flag) {
	fte_t	e;

	VERIFYCONN;

	if (conn->connected != FCS_ACTIVE)
		return(FE_NOTCONNECTED);

	e = firetalk_protocols[conn->protocol]->im_send_action(conn->handle, dest, message, auto_flag);
	if (e != FE_SUCCESS)
		return(e);

	e = firetalk_protocols[conn->protocol]->periodic(conn);
	if (e != FE_SUCCESS && e != FE_IDLEFAST)
		return(e);

	return(FE_SUCCESS);
}

fte_t	firetalk_im_get_info(firetalk_t conn, const char *const nickname) {
	VERIFYCONN;

	if (conn->connected != FCS_ACTIVE)
		return(FE_NOTCONNECTED);

	return(firetalk_protocols[conn->protocol]->get_info(conn->handle, nickname));
}

fte_t	firetalk_set_info(firetalk_t conn, const char *const info) {
	VERIFYCONN;

	if (conn->connected == FCS_NOTCONNECTED)
		return(FE_NOTCONNECTED);

	return(firetalk_protocols[conn->protocol]->set_info(conn->handle, info));
}

fte_t	firetalk_im_list_buddies(firetalk_t conn) {
	struct s_firetalk_buddy *buddyiter;

	VERIFYCONN;

	if (conn->connected != FCS_ACTIVE)
		return(FE_NOTCONNECTED);

	if (!conn->callbacks[FC_IM_LISTBUDDY])
		return(FE_SUCCESS);

	for (buddyiter = conn->buddy_head; buddyiter != NULL; buddyiter = buddyiter->next)
		conn->callbacks[FC_IM_LISTBUDDY](conn, conn->clientstruct, buddyiter->nickname, buddyiter->group, buddyiter->friendly, buddyiter->online, buddyiter->away, buddyiter->idletime);

	return(FE_SUCCESS);
}

fte_t	firetalk_chat_listmembers(firetalk_t conn, const char *const roomname) {
	struct s_firetalk_room *room;
	struct s_firetalk_member *memberiter;

	VERIFYCONN;

	if (conn->connected != FCS_ACTIVE)
		return(FE_NOTCONNECTED);

	if (!conn->callbacks[FC_CHAT_LISTMEMBER])
		return(FE_SUCCESS);

	room = firetalk_find_room(conn, roomname);
	if (room == NULL)
		return(firetalkerror);

	for (memberiter = room->member_head; memberiter != NULL; memberiter = memberiter->next)
		conn->callbacks[FC_CHAT_LISTMEMBER](conn, conn->clientstruct, room->name, memberiter->nickname, memberiter->admin);

	return(FE_SUCCESS);
}

const char *firetalk_chat_normalize(firetalk_t conn, const char *const room) {
	return(firetalk_protocols[conn->protocol]->room_normalize(room));
}

fte_t	firetalk_set_away(firetalk_t conn, const char *const message, const int auto_flag) {
	VERIFYCONN;

	if (conn->connected == FCS_NOTCONNECTED)
		return(FE_NOTCONNECTED);

	return(firetalk_protocols[conn->protocol]->set_away(conn->handle, message, auto_flag));
}

fte_t	firetalk_set_nickname(firetalk_t conn, const char *const nickname) {
	VERIFYCONN;

	if (conn->connected == FCS_NOTCONNECTED)
		return(FE_NOTCONNECTED);

	return(firetalk_protocols[conn->protocol]->set_nickname(conn->handle, nickname));
}

fte_t	firetalk_set_password(firetalk_t conn, const char *const oldpass, const char *const newpass) {
	VERIFYCONN;

	if (conn->connected != FCS_ACTIVE)
		return(FE_NOTCONNECTED);

	return(firetalk_protocols[conn->protocol]->set_password(conn->handle, oldpass, newpass));
}

fte_t	firetalk_set_privacy(firetalk_t conn, const char *const mode) {
	VERIFYCONN;

	assert(mode != NULL);

	if (conn->connected == FCS_NOTCONNECTED)
		return(FE_NOTCONNECTED);

	return(firetalk_protocols[conn->protocol]->set_privacy(conn->handle, mode));
}

fte_t	firetalk_im_evil(firetalk_t conn, const char *const who) {
	VERIFYCONN;

	if (conn->connected != FCS_ACTIVE)
		return(FE_NOTCONNECTED);

	return(firetalk_protocols[conn->protocol]->im_evil(conn->handle, who));
}

fte_t	firetalk_chat_join(firetalk_t conn, const char *const room) {
	const char *normalroom;

	VERIFYCONN;

	if (conn->connected == FCS_NOTCONNECTED)
		return(FE_NOTCONNECTED);

	normalroom = firetalk_protocols[conn->protocol]->room_normalize(room);
	if (!normalroom)
		return(FE_ROOMUNAVAILABLE);

	return(firetalk_protocols[conn->protocol]->chat_join(conn->handle, normalroom));
}

fte_t	firetalk_chat_part(firetalk_t conn, const char *const room) {
	const char *normalroom;

	VERIFYCONN;

	if (conn->connected == FCS_NOTCONNECTED)
		return(FE_NOTCONNECTED);

	normalroom = firetalk_protocols[conn->protocol]->room_normalize(room);
	if (!normalroom)
		return(FE_ROOMUNAVAILABLE);

	return(firetalk_protocols[conn->protocol]->chat_part(conn->handle, normalroom));
}

fte_t	firetalk_chat_send_message(firetalk_t conn, const char *const room, const char *const message, const int auto_flag) {
	const char *normalroom;

	VERIFYCONN;

	if (conn->connected != FCS_ACTIVE)
		return(FE_NOTCONNECTED);

	if (*room == ':')
		normalroom = room;
	else
		normalroom = firetalk_protocols[conn->protocol]->room_normalize(room);
	if (!normalroom)
		return(FE_ROOMUNAVAILABLE);

	return(firetalk_protocols[conn->protocol]->chat_send_message(conn->handle, normalroom, message, auto_flag));
}

fte_t	firetalk_chat_send_action(firetalk_t conn, const char *const room, const char *const message, const int auto_flag) {
	const char *normalroom;

	VERIFYCONN;

	if (conn->connected != FCS_ACTIVE)
		return(FE_NOTCONNECTED);

	normalroom = firetalk_protocols[conn->protocol]->room_normalize(room);
	if (!normalroom)
		return(FE_ROOMUNAVAILABLE);

	return(firetalk_protocols[conn->protocol]->chat_send_action(conn->handle, normalroom, message, auto_flag));
}

fte_t	firetalk_chat_invite(firetalk_t conn, const char *const room, const char *const who, const char *const message) {
	const char *normalroom;

	VERIFYCONN;

	if (conn->connected != FCS_ACTIVE)
		return(FE_NOTCONNECTED);

	normalroom = firetalk_protocols[conn->protocol]->room_normalize(room);
	if (!normalroom)
		return(FE_ROOMUNAVAILABLE);

	return(firetalk_protocols[conn->protocol]->chat_invite(conn->handle, normalroom, who, message));
}

fte_t	firetalk_chat_set_topic(firetalk_t conn, const char *const room, const char *const topic) {
	const char *normalroom;

	VERIFYCONN;

	if (conn->connected != FCS_ACTIVE)
		return(FE_NOTCONNECTED);

	normalroom = firetalk_protocols[conn->protocol]->room_normalize(room);
	if (!normalroom)
		return(FE_ROOMUNAVAILABLE);

	return(firetalk_protocols[conn->protocol]->chat_set_topic(conn->handle, normalroom, topic));
}

fte_t	firetalk_chat_op(firetalk_t conn, const char *const room, const char *const who) {
	const char *normalroom;

	VERIFYCONN;

	if (conn->connected != FCS_ACTIVE)
		return(FE_NOTCONNECTED);

	normalroom = firetalk_protocols[conn->protocol]->room_normalize(room);
	if (!normalroom)
		return(FE_ROOMUNAVAILABLE);

	return(firetalk_protocols[conn->protocol]->chat_op(conn->handle, normalroom, who));
}

fte_t	firetalk_chat_deop(firetalk_t conn, const char *const room, const char *const who) {
	const char *normalroom;

	VERIFYCONN;

	if (conn->connected != FCS_ACTIVE)
		return(FE_NOTCONNECTED);

	normalroom = firetalk_protocols[conn->protocol]->room_normalize(room);
	if (!normalroom)
		return(FE_ROOMUNAVAILABLE);

	return(firetalk_protocols[conn->protocol]->chat_deop(conn->handle, normalroom, who));
}

fte_t	firetalk_chat_kick(firetalk_t conn, const char *const room, const char *const who, const char *const reason) {
	const char *normalroom;

	VERIFYCONN;

	if (conn->connected != FCS_ACTIVE)
		return(FE_NOTCONNECTED);

	normalroom = firetalk_protocols[conn->protocol]->room_normalize(room);
	if (!normalroom)
		return(FE_ROOMUNAVAILABLE);

	return(firetalk_protocols[conn->protocol]->chat_kick(conn->handle, normalroom, who, reason));
}

fte_t	firetalk_subcode_send_request(firetalk_t conn, const char *const to, const char *const command, const char *const args) {
	VERIFYCONN;

	if (conn->connected != FCS_ACTIVE)
		return(FE_NOTCONNECTED);

//	return(firetalk_protocols[conn->protocol]->subcode_send_request(conn->handle, to, command, args));
	firetalk_enqueue(&conn->subcode_requests, to, firetalk_protocols[conn->protocol]->subcode_encode(conn->handle, command, args));
	return(FE_SUCCESS);
}

fte_t	firetalk_subcode_send_reply(firetalk_t conn, const char *const to, const char *const command, const char *const args) {
	VERIFYCONN;

	if ((conn->connected != FCS_ACTIVE) && (*to != ':'))
		return(FE_NOTCONNECTED);

//	return(firetalk_protocols[conn->protocol]->subcode_send_reply(conn->handle, to, command, args));
	firetalk_enqueue(&conn->subcode_replies, to, firetalk_protocols[conn->protocol]->subcode_encode(conn->handle, command, args));
	return(FE_SUCCESS);
}

fte_t	firetalk_subcode_register_request_callback(firetalk_t conn, const char *const command, void (*callback)(firetalk_t, void *, const char *const, const char *const, const char *const)) {
	struct s_firetalk_subcode_callback *iter;

	VERIFYCONN;

	if (command == NULL) {
		if (conn->subcode_request_default)
			free(conn->subcode_request_default);
		conn->subcode_request_default = calloc(1, sizeof(struct s_firetalk_subcode_callback));
		if (conn->subcode_request_default == NULL)
			abort();
		conn->subcode_request_default->callback = (ptrtofnct)callback;
	} else {
		iter = conn->subcode_request_head;
		conn->subcode_request_head = calloc(1, sizeof(struct s_firetalk_subcode_callback));
		if (conn->subcode_request_head == NULL)
			abort();
		conn->subcode_request_head->next = iter;
		conn->subcode_request_head->command = strdup(command);
		if (conn->subcode_request_head->command == NULL)
			abort();
		conn->subcode_request_head->callback = (ptrtofnct)callback;
	}
	return(FE_SUCCESS);
}

fte_t	firetalk_subcode_register_request_reply(firetalk_t conn, const char *const command, const char *const reply) {
	struct s_firetalk_subcode_callback *iter;

	VERIFYCONN;

	if (command == NULL) {
		if (conn->subcode_request_default)
			free(conn->subcode_request_default);
		conn->subcode_request_default = calloc(1, sizeof(struct s_firetalk_subcode_callback));
		if (conn->subcode_request_default == NULL)
			abort();
		conn->subcode_request_default->staticresp = strdup(reply);
		if (conn->subcode_request_default->staticresp == NULL)
			abort();
	} else {
		iter = conn->subcode_request_head;
		conn->subcode_request_head = calloc(1, sizeof(struct s_firetalk_subcode_callback));
		if (conn->subcode_request_head == NULL)
			abort();
		conn->subcode_request_head->next = iter;
		conn->subcode_request_head->command = strdup(command);
		if (conn->subcode_request_head->command == NULL)
			abort();
		conn->subcode_request_head->staticresp = strdup(reply);
		if (conn->subcode_request_head->staticresp == NULL)
			abort();
	}
	return(FE_SUCCESS);
}

fte_t	firetalk_subcode_register_reply_callback(firetalk_t conn, const char *const command, void (*callback)(firetalk_t, void *, const char *const, const char *const, const char *const)) {
	struct s_firetalk_subcode_callback *iter;

	VERIFYCONN;

	if (command == NULL) {
		if (conn->subcode_reply_default)
			free(conn->subcode_reply_default);
		conn->subcode_reply_default = calloc(1, sizeof(struct s_firetalk_subcode_callback));
		if (conn->subcode_reply_default == NULL)
			abort();
		conn->subcode_reply_default->callback = (ptrtofnct)callback;
	} else {
		iter = conn->subcode_reply_head;
		conn->subcode_reply_head = calloc(1, sizeof(struct s_firetalk_subcode_callback));
		if (conn->subcode_reply_head == NULL)
			abort();
		conn->subcode_reply_head->next = iter;
		conn->subcode_reply_head->command = strdup(command);
		if (conn->subcode_reply_head->command == NULL)
			abort();
		conn->subcode_reply_head->callback = (ptrtofnct)callback;
	}
	return(FE_SUCCESS);
}

fte_t	firetalk_file_offer(firetalk_t conn, const char *const nickname, const char *const filename, void *clientfilestruct) {
	struct s_firetalk_file *iter;
	struct stat s;
	struct sockaddr_in addr;
	char args[256];
	unsigned int l;

	VERIFYCONN;

	iter = conn->file_head;
	conn->file_head = calloc(1, sizeof(struct s_firetalk_file));
	if (conn->file_head == NULL)
		abort();
	conn->file_head->who = strdup(nickname);
	if (conn->file_head->who == NULL)
		abort();
	conn->file_head->filename = strdup(filename);
	if (conn->file_head->filename == NULL)
		abort();
	conn->file_head->sockfd = -1;
	conn->file_head->clientfilestruct = clientfilestruct;

	conn->file_head->filefd = open(filename, O_RDONLY);
	if (conn->file_head->filefd == -1) {
		firetalk_file_cancel(conn, conn->file_head);
		return(FE_IOERROR);
	}

	if (fstat(conn->file_head->filefd, &s) != 0) {
		firetalk_file_cancel(conn, conn->file_head);
		return(FE_IOERROR);
	}

	conn->file_head->size = (long)s.st_size;

	conn->file_head->sockfd = socket(PF_INET, SOCK_STREAM, 0);
	if (conn->file_head->sockfd == -1) {
		firetalk_file_cancel(conn, conn->file_head);
		return(FE_SOCKET);
	}

	addr.sin_family = AF_INET;
	addr.sin_port = 0;
	addr.sin_addr.s_addr = INADDR_ANY;
	if (bind(conn->file_head->sockfd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in)) != 0) {
		firetalk_file_cancel(conn, conn->file_head);
		return(FE_SOCKET);
	}

	if (listen(conn->file_head->sockfd, 1) != 0) {
		firetalk_file_cancel(conn, conn->file_head);
		return(FE_SOCKET);
	}

	l = (unsigned int)sizeof(struct sockaddr_in);
	if (getsockname(conn->file_head->sockfd, (struct sockaddr *)&addr, &l) != 0) {
		firetalk_file_cancel(conn, conn->file_head);
		return(FE_SOCKET);
	}

	conn->file_head->bytes = 0;
	conn->file_head->state = FF_STATE_WAITREMOTE;
	conn->file_head->direction = FF_DIRECTION_SENDING;
	conn->file_head->port = ntohs(addr.sin_port);
	conn->file_head->next = iter;
	conn->file_head->type = FF_TYPE_DCC;
	snprintf(args, sizeof(args), "SEND %s %lu %u %ld", conn->file_head->filename, conn->localip, conn->file_head->port, conn->file_head->size);
	return(firetalk_subcode_send_request(conn, nickname, "DCC", args));
}

fte_t	firetalk_file_accept(firetalk_t conn, void *filehandle, void *clientfilestruct, const char *const localfile) {
	struct s_firetalk_file *fileiter;
	struct sockaddr_in addr;

	VERIFYCONN;

	fileiter = filehandle;
	fileiter->clientfilestruct = clientfilestruct;

	fileiter->filefd = open(localfile, O_WRONLY|O_CREAT|O_EXCL, S_IRWXU);
	if (fileiter->filefd == -1)
		return(FE_NOPERMS);

	addr.sin_family = AF_INET;
	addr.sin_port = fileiter->port;
	memcpy(&addr.sin_addr.s_addr, &fileiter->inet_ip, 4);
	fileiter->sockfd = firetalk_internal_connect(&addr
#ifdef _FC_USE_IPV6
	, NULL
#endif
	);
	if (fileiter->sockfd == -1) {
		firetalk_file_cancel(conn, filehandle);
		return(FE_SOCKET);
	}
	fileiter->state = FF_STATE_WAITSYNACK;
	return(FE_SUCCESS);
}

fte_t	firetalk_file_cancel(firetalk_t conn, void *filehandle) {
	struct s_firetalk_file *fileiter, *prev;

	VERIFYCONN;

	prev = NULL;
	for (fileiter = conn->file_head; fileiter != NULL; fileiter = fileiter->next) {
		if (fileiter == filehandle) {
			if (prev != NULL)
				prev->next = fileiter->next;
			else
				conn->file_head = fileiter->next;
			if (fileiter->who) {
				free(fileiter->who);
				fileiter->who = NULL;
			}
			if (fileiter->filename) {
				free(fileiter->filename);
				fileiter->filename = NULL;
			}
			if (fileiter->sockfd >= 0) {
				close(fileiter->sockfd);
				fileiter->sockfd = -1;
			}
			if (fileiter->filefd >= 0) {
				close(fileiter->filefd);
				fileiter->filefd = -1;
			}
			free(fileiter);
			return(FE_SUCCESS);
		}
		prev = fileiter;
	}
	return(FE_NOTFOUND);
}

fte_t	firetalk_file_refuse(firetalk_t conn, void *filehandle) {
	return(firetalk_file_cancel(conn, filehandle));
}

fte_t	firetalk_compare_nicks(firetalk_t conn, const char *const nick1, const char *const nick2) {
	VERIFYCONN;

	return(firetalk_protocols[conn->protocol]->comparenicks(nick1, nick2));
}

fte_t	firetalk_isprint(firetalk_t conn, const int c) {
	VERIFYCONN;

	return(firetalk_protocols[conn->protocol]->isprintable(c));
}

fte_t	firetalk_select() {
	return(firetalk_select_custom(0, NULL, NULL, NULL, NULL));
}

fte_t	firetalk_select_custom(int n, fd_set *fd_read, fd_set *fd_write, fd_set *fd_except, struct timeval *timeout) {
	int ret;
	fd_set *my_read, *my_write, *my_except;
	fd_set internal_read, internal_write, internal_except;
	struct timeval internal_timeout, *my_timeout;
	struct s_firetalk_handle *fchandle;

	my_read = fd_read;
	my_write = fd_write;
	my_except = fd_except;
	my_timeout = timeout;

	if (!my_read) {
		my_read = &internal_read;
		FD_ZERO(my_read);
	}

	if (!my_write) {
		my_write = &internal_write;
		FD_ZERO(my_write);
	}

	if (!my_except) {
		my_except = &internal_except;
		FD_ZERO(my_except);
	}

	if (!my_timeout) {
		my_timeout = &internal_timeout;
		my_timeout->tv_sec = 15;
		my_timeout->tv_usec = 0;
	}

	if (my_timeout->tv_sec > 15)
		my_timeout->tv_sec = 15;

	/* internal preselect */
	for (fchandle = handle_head; fchandle != NULL; fchandle = fchandle->next) {
		struct s_firetalk_file *fileiter;

		if (fchandle->deleted)
			continue;

		if (fchandle->connected == FCS_NOTCONNECTED)
			continue;

		while (fchandle->subcode_requests.count > 0) {
			int	count = fchandle->subcode_requests.count;
			char	*key = strdup(fchandle->subcode_requests.keys[0]);

			firetalk_protocols[fchandle->protocol]->im_send_message(fchandle->handle, key, "", 0);
			free(key);
			assert(fchandle->subcode_requests.count < count);
		}

		while (fchandle->subcode_replies.count > 0) {
			int	count = fchandle->subcode_replies.count;
			char	*key = strdup(fchandle->subcode_replies.keys[0]);

			firetalk_protocols[fchandle->protocol]->im_send_message(fchandle->handle, key, "", 1);
			free(key);
			assert(fchandle->subcode_replies.count < count);
		}

		firetalk_protocols[fchandle->protocol]->periodic(fchandle);
		if (fchandle->connected == FCS_NOTCONNECTED)
			continue;

		if (fchandle->fd >= n)
			n = fchandle->fd + 1;
		assert(fchandle->fd >= 0);
		FD_SET(fchandle->fd, my_except);
		if (fchandle->connected == FCS_WAITING_SYNACK)
			FD_SET(fchandle->fd, my_write);
		else
			FD_SET(fchandle->fd, my_read);

		for (fileiter = fchandle->file_head; fileiter != NULL; fileiter = fileiter->next) {
			if (fileiter->state == FF_STATE_TRANSFERRING) {
				if (fileiter->sockfd >= n)
					n = fileiter->sockfd + 1;
				switch (fileiter->direction) {
				  case FF_DIRECTION_SENDING:
					assert(fileiter->sockfd >= 0);
					FD_SET(fileiter->sockfd, my_write);
					FD_SET(fileiter->sockfd, my_except);
					break;
				  case FF_DIRECTION_RECEIVING:
					assert(fileiter->sockfd >= 0);
					FD_SET(fileiter->sockfd, my_read);
					FD_SET(fileiter->sockfd, my_except);
					break;
				}
			} else if (fileiter->state == FF_STATE_WAITREMOTE) {
				assert(fileiter->sockfd >= 0);
				if (fileiter->sockfd >= n)
					n = fileiter->sockfd + 1;
				FD_SET(fileiter->sockfd, my_read);
				FD_SET(fileiter->sockfd, my_except);
			} else if (fileiter->state == FF_STATE_WAITSYNACK) {
				assert(fileiter->sockfd >= 0);
				if (fileiter->sockfd >= n)
					n = fileiter->sockfd + 1;
				FD_SET(fileiter->sockfd, my_write);
				FD_SET(fileiter->sockfd, my_except);
			}
		}
	}

	/* per-protocol preselect, UI prepoll */
	for (fchandle = handle_head; fchandle != NULL; fchandle = fchandle->next) {
		if (fchandle->deleted)
			continue;
		firetalk_protocols[fchandle->protocol]->preselect(fchandle->handle, my_read, my_write, my_except, &n);
		if (fchandle->callbacks[FC_PRESELECT])
			fchandle->callbacks[FC_PRESELECT](fchandle, fchandle->clientstruct);
	}

	/* select */
	if (n > 0) {
		ret = select(n, my_read, my_write, my_except, my_timeout);
		if (ret == -1)
			return(FE_PACKET);
	}

	/* per-protocol postselect, UI postpoll */
	for (fchandle = handle_head; fchandle != NULL; fchandle = fchandle->next) {
		if (fchandle->deleted)
			continue;

		firetalk_protocols[fchandle->protocol]->postselect(fchandle->handle, my_read, my_write, my_except);
		if (fchandle->callbacks[FC_POSTSELECT])
			fchandle->callbacks[FC_POSTSELECT](fchandle, fchandle->clientstruct);
	}

	/* internal postpoll */
	for (fchandle = handle_head; fchandle != NULL; fchandle = fchandle->next) {
		struct s_firetalk_file *fileiter, *filenext;

		if (fchandle->deleted)
			continue;

		if (fchandle->connected == FCS_NOTCONNECTED)
			continue;
		assert(fchandle->fd >= 0);
		if (FD_ISSET(fchandle->fd, my_except))
			firetalk_protocols[fchandle->protocol]->disconnect(fchandle->handle);
		else if (FD_ISSET(fchandle->fd, my_read)) {
			short	length;

			/* read data into handle buffer */
			length = recv(fchandle->fd, &fchandle->buffer[fchandle->bufferpos], firetalk_protocols[fchandle->protocol]->default_buffersize - fchandle->bufferpos, MSG_DONTWAIT);

			if (length < 1)
				firetalk_callback_disconnect(fchandle->handle, FE_DISCONNECT);
			else {
				fchandle->bufferpos += length;
				if (fchandle->connected == FCS_ACTIVE)
					firetalk_protocols[fchandle->protocol]->got_data(fchandle->handle, fchandle->buffer, &fchandle->bufferpos);
				else
					firetalk_protocols[fchandle->protocol]->got_data_connecting(fchandle->handle, fchandle->buffer, &fchandle->bufferpos);
				if (fchandle->bufferpos == firetalk_protocols[fchandle->protocol]->default_buffersize)
					firetalk_callback_disconnect(fchandle->handle, FE_PACKETSIZE);
			}
		} else if (FD_ISSET(fchandle->fd, my_write))
			firetalk_handle_synack(fchandle);

		for (fileiter = fchandle->file_head; fileiter != NULL; fileiter = filenext) {
			filenext = fileiter->next;
			if (fileiter->state == FF_STATE_TRANSFERRING) {
				assert(fileiter->sockfd >= 0);
				if (FD_ISSET(fileiter->sockfd, my_write))
					firetalk_handle_send(fchandle, fileiter);
				if ((fileiter->sockfd != -1) && FD_ISSET(fileiter->sockfd, my_read))
					firetalk_handle_receive(fchandle, fileiter);
				if ((fileiter->sockfd != -1) && FD_ISSET(fileiter->sockfd, my_except)) {
					if (fchandle->callbacks[FC_FILE_ERROR])
						fchandle->callbacks[FC_FILE_ERROR](fchandle, fchandle->clientstruct, fileiter, fileiter->clientfilestruct, FE_IOERROR);
					firetalk_file_cancel(fchandle, fileiter);
				}
			} else if (fileiter->state == FF_STATE_WAITREMOTE) {
				assert(fileiter->sockfd >= 0);
				if (FD_ISSET(fileiter->sockfd, my_read)) {
					unsigned int l = sizeof(struct sockaddr_in);
					struct sockaddr_in addr;
					int s;

					s = accept(fileiter->sockfd, (struct sockaddr *)&addr, &l);
					if (s == -1) {
						if (fchandle->callbacks[FC_FILE_ERROR])
							fchandle->callbacks[FC_FILE_ERROR](fchandle, fchandle->clientstruct, fileiter, fileiter->clientfilestruct, FE_SOCKET);
						firetalk_file_cancel(fchandle, fileiter);
					} else {
						close(fileiter->sockfd);
						fileiter->sockfd = s;
						fileiter->state = FF_STATE_TRANSFERRING;
						if (fchandle->callbacks[FC_FILE_START])
							fchandle->callbacks[FC_FILE_START](fchandle, fchandle->clientstruct, fileiter, fileiter->clientfilestruct);
					}
				} else if (FD_ISSET(fileiter->sockfd, my_except)) {
					if (fchandle->callbacks[FC_FILE_ERROR])
						fchandle->callbacks[FC_FILE_ERROR](fchandle, fchandle->clientstruct, fileiter, fileiter->clientfilestruct, FE_IOERROR);
					firetalk_file_cancel(fchandle, fileiter);
				}
			} else if (fileiter->state == FF_STATE_WAITSYNACK) {
				assert(fileiter->sockfd >= 0);
				if (FD_ISSET(fileiter->sockfd, my_write))
					firetalk_handle_file_synack(fchandle, fileiter);
				if (FD_ISSET(fileiter->sockfd, my_except))
					firetalk_file_cancel(fchandle, fileiter);
			}
		}
	}

	/* handle deleted connections */
	{
		struct s_firetalk_handle *fchandleprev, *fchandlenext;

		fchandleprev = NULL;
		for (fchandle = handle_head; fchandle != NULL; fchandle = fchandlenext) {
			fchandlenext = fchandle->next;
			if (fchandle->deleted == 1) {
				assert(fchandle->handle == NULL);
				if (fchandle->buddy_head != NULL) {
					struct s_firetalk_buddy *iter, *iternext;

					for (iter = fchandle->buddy_head; iter != NULL; iter = iternext) {
						iternext = iter->next;
						if (iter->nickname != NULL) {
							free(iter->nickname);
							iter->nickname = NULL;
						}
						if (iter->group != NULL) {
							free(iter->group);
							iter->group = NULL;
						}
						if (iter->capabilities != NULL) {
							free(iter->capabilities);
							iter->capabilities = NULL;
						}
						free(iter);
					}
					fchandle->buddy_head = NULL;
				}
				if (fchandle->deny_head != NULL) {
					struct s_firetalk_deny *iter, *iternext;

					for (iter = fchandle->deny_head; iter != NULL; iter = iternext) {
						iternext = iter->next;
						if (iter->nickname != NULL) {
							free(iter->nickname);
							iter->nickname = NULL;
						}
						free(iter);
					}
					fchandle->deny_head = NULL;
				}
				if (fchandle->room_head != NULL) {
					struct s_firetalk_room *iter, *iternext;

					for (iter = fchandle->room_head; iter != NULL; iter = iternext) {
						struct s_firetalk_member *memberiter, *memberiternext;

						for (memberiter = iter->member_head; memberiter != NULL; memberiter = memberiternext) {
							memberiternext = memberiter->next;
							if (memberiter->nickname != NULL) {
								free(memberiter->nickname);
								memberiter->nickname = NULL;
							}
							free(memberiter);
						}
						iter->member_head = NULL;
						iternext = iter->next;
						if (iter->name != NULL) {
							free(iter->name);
							iter->name = NULL;
						}
						free(iter);
					}
					fchandle->room_head = NULL;
				}
				if (fchandle->file_head != NULL) {
					struct s_firetalk_file *iter, *iternext;

					for (iter = fchandle->file_head; iter != NULL; iter = iternext) {
						iternext = iter->next;
						if (iter->who != NULL) {
							free(iter->who);
							iter->who = NULL;
						}
						if (iter->filename != NULL) {
							free(iter->filename);
							iter->filename = NULL;
						}
						free(iter);
					}
					fchandle->file_head = NULL;
				}
				if (fchandle->subcode_request_head != NULL) {
					struct s_firetalk_subcode_callback *iter, *iternext;

					for (iter = fchandle->subcode_request_head; iter != NULL; iter = iternext) {
						iternext = iter->next;
						if (iter->command != NULL) {
							free(iter->command);
							iter->command = NULL;
						}
						if (iter->staticresp != NULL) {
							free(iter->staticresp);
							iter->staticresp = NULL;
						}
						free(iter);
					}
					fchandle->subcode_request_head = NULL;
				}
				if (fchandle->subcode_request_default != NULL) {
					if (fchandle->subcode_request_default->command != NULL) {
						free(fchandle->subcode_request_default->command);
						fchandle->subcode_request_default->command = NULL;
					}
					free(fchandle->subcode_request_default);
					fchandle->subcode_request_default = NULL;
				}
				if (fchandle->subcode_reply_head != NULL) {
					struct s_firetalk_subcode_callback *iter, *iternext;

					for (iter = fchandle->subcode_reply_head; iter != NULL; iter = iternext) {
						iternext = iter->next;
						free(iter->command);
						free(iter);
					}
					fchandle->subcode_reply_head = NULL;
				}
				if (fchandle->subcode_reply_default != NULL) {
					if (fchandle->subcode_reply_default->command != NULL) {
						free(fchandle->subcode_reply_default->command);
						fchandle->subcode_reply_default->command = NULL;
					}
					free(fchandle->subcode_reply_default);
					fchandle->subcode_reply_default = NULL;
				}
				if (fchandle->username != NULL) {
					free(fchandle->username);
					fchandle->username = NULL;
				}
				if (fchandle->buffer != NULL) {
					free(fchandle->buffer);
					fchandle->buffer = NULL;
				}
				if (fchandleprev == NULL) {
					assert(fchandle == handle_head);
					handle_head = fchandlenext;
				} else {
					assert(fchandle != handle_head);
					fchandleprev->next = fchandlenext;
				}

				free(fchandle);
			} else
				fchandleprev = fchandle;
		}
	}

	return(FE_SUCCESS);
}

void	firetalk_enqueue(firetalk_queue_t *queue, const char *const key, void *data) {
	queue->count++;
	queue->keys = realloc(queue->keys, (queue->count)*sizeof(*(queue->keys)));
	queue->data = realloc(queue->data, (queue->count)*sizeof(*(queue->data)));
	queue->keys[queue->count-1] = strdup(key);
	if (queue->keys[queue->count-1] == NULL)
		abort();
	queue->data[queue->count-1] = data;
}

const void *firetalk_peek(firetalk_queue_t *queue, const char *const key) {
	int	i;

	assert(queue != NULL);
	assert(key != NULL);

	for (i = 0; i < queue->count; i++)
		if (strcmp(queue->keys[i], key) == 0)
			return(queue->data[i]);
	return(NULL);
}

void	*firetalk_dequeue(firetalk_queue_t *queue, const char *const key) {
	int	i;

	assert(queue != NULL);
	assert(key != NULL);

	for (i = 0; i < queue->count; i++)
		if (strcmp(queue->keys[i], key) == 0) {
			void	*data = queue->data[i];

			free(queue->keys[i]);
			queue->keys[i] = NULL;
			memmove(queue->keys+i, queue->keys+i+1, (queue->count-i-1)*sizeof(*(queue->keys)));
			memmove(queue->data+i, queue->data+i+1, (queue->count-i-1)*sizeof(*(queue->data)));
			queue->count--;
			queue->keys = realloc(queue->keys, (queue->count)*sizeof(*(queue->keys)));
			queue->data = realloc(queue->data, (queue->count)*sizeof(*(queue->data)));
			return(data);
		}
	return(NULL);
}

void	firetalk_queue_append(char *buf, int buflen, firetalk_queue_t *queue, const char *const key) {
	const char *data;

	while ((data = firetalk_peek(queue, key)) != NULL) {
		if (strlen(buf)+strlen(data) >= buflen-1)
			break;
		strcat(buf, data);
		free(firetalk_dequeue(queue, key));
	}
}
