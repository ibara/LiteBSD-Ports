/*
firetalk-int.h - FireTalk wrapper declarations
Copyright (C) 2000 Ian Gulliver

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
#ifndef _FIRETALK_INT_H
#define _FIRETALK_INT_H

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#ifdef HAVE_INTTYPES_H
# include <inttypes.h>
#endif

#ifdef HAVE_STDINT_H
# include <stdint.h>
#endif

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#ifndef HAVE_INET_ATON
# define inet_aton(x,y)	inet_pton(AF_INET,x,y)
#endif

#define DEBUG

typedef struct s_firetalk_handle *firetalk_t;
#define _HAVE_FIRETALK_T

#ifndef _HAVE_CLIENT_T
#define _HAVE_CLIENT_T
typedef void *client_t;
#endif

#include "firetalk.h"

#ifndef SHUT_RDWR
#define SHUT_RDWR 2
#endif


typedef void (*ptrtofnct)(firetalk_t, void *, ...);

typedef struct {
	char	**keys;
	void	**data;
	int	count;
} firetalk_queue_t;

struct s_firetalk_buddy {
	struct s_firetalk_buddy *next;
	char	*nickname,
		*group,
		*friendly;
	long	idletime,
		warnval;
	unsigned char
		online:1,
		away:1;
	int	typing,
		flags;
	char	*capabilities;
};

struct s_firetalk_deny {
	struct s_firetalk_deny *next;
	char *nickname;
};

struct s_firetalk_member {
	struct s_firetalk_member *next;
	char *nickname;
	unsigned char
		admin:1;
};

struct s_firetalk_room {
	struct s_firetalk_room *next;
	struct s_firetalk_member *member_head;
	char *name;
	unsigned char
		admin:1,
		sentjoin:1;
};

struct s_firetalk_file {
	struct s_firetalk_file *next;
	char *who;
	char *filename;
	struct in_addr inet_ip;
#ifdef _FC_USE_IPV6
	struct in6_addr inet6_ip;
	int tryinet6;
#endif
	uint16_t port;
	long size;
	long bytes;
	uint32_t acked;
#define FF_STATE_WAITLOCAL	0
#define FF_STATE_WAITREMOTE	1
#define FF_STATE_WAITSYNACK	2
#define FF_STATE_TRANSFERRING	3
	int state;
#define FF_DIRECTION_SENDING	0
#define FF_DIRECTION_RECEIVING	1
	int direction;
#define FF_TYPE_DCC		0
#define FF_TYPE_RAW		1
	int type;
	int sockfd;
	int filefd;
	void *clientfilestruct;
};

struct s_firetalk_subcode_callback {
	struct s_firetalk_subcode_callback *next;
	char *command, *staticresp;
	ptrtofnct callback;
};

struct s_firetalk_handle {
	void *handle;
	void *clientstruct;
	int connected;
	struct sockaddr_in remote_addr;
	struct in_addr local_addr;
#ifdef _FC_USE_IPV6
	struct sockaddr_in6 remote_addr6;
	struct in6_addr local_addr6;
#endif
	unsigned long localip;
	int protocol;
	char *username;
	int fd;
	ptrtofnct callbacks[FC_MAX];
	unsigned char *buffer;
	unsigned short bufferpos;
	struct s_firetalk_handle *next;
	struct s_firetalk_handle *prev;
	struct s_firetalk_buddy *buddy_head;
	struct s_firetalk_deny *deny_head;
	struct s_firetalk_room *room_head;
	struct s_firetalk_file *file_head;
	struct s_firetalk_subcode_callback *subcode_request_head;
	struct s_firetalk_subcode_callback *subcode_reply_head;
	struct s_firetalk_subcode_callback *subcode_request_default;
	struct s_firetalk_subcode_callback *subcode_reply_default;
	firetalk_queue_t subcode_requests;
	firetalk_queue_t subcode_replies;
	unsigned char deleted:1;
};

typedef struct s_firetalk_protocol_functions {
	const char *strprotocol;
	const char *default_server;
	const short default_port;
	const unsigned short default_buffersize;
	fte_t	(*periodic)(struct s_firetalk_handle *const);
	fte_t	(*preselect)(client_t, fd_set *read, fd_set *write, fd_set *except, int *n);
	fte_t	(*postselect)(client_t, fd_set *read, fd_set *write, fd_set *except);
	fte_t	(*got_data)(client_t, unsigned char *buffer, unsigned short *bufferpos);
	fte_t	(*got_data_connecting)(client_t, unsigned char *buffer, unsigned short *bufferpos);
	fte_t	(*comparenicks)(const char *const, const char *const);
	fte_t	(*isprintable)(const int);
	fte_t	(*disconnect)(client_t);
	fte_t	(*signon)(client_t, const char *const);
	fte_t	(*get_info)(client_t, const char *const);
	fte_t	(*set_info)(client_t, const char *const);
	fte_t	(*set_away)(client_t, const char *const, const int);
	fte_t	(*set_nickname)(client_t, const char *const);
	fte_t	(*set_password)(client_t, const char *const, const char *const);
	fte_t	(*set_privacy)(client_t, const char *const);
	fte_t	(*im_add_buddy)(client_t, const char *const, const char *const, const char *const);
	fte_t	(*im_remove_buddy)(client_t, const char *const, const char *const);
	fte_t	(*im_add_deny)(client_t, const char *const);
	fte_t	(*im_remove_deny)(client_t, const char *const);
	fte_t	(*im_upload_buddies)(client_t);
	fte_t	(*im_upload_denies)(client_t);
	fte_t	(*im_send_message)(client_t, const char *const, const char *const, const int);
	fte_t	(*im_send_action)(client_t, const char *const, const char *const, const int);
	fte_t	(*im_evil)(client_t, const char *const);
	fte_t	(*chat_join)(client_t, const char *const);
	fte_t	(*chat_part)(client_t, const char *const);
	fte_t	(*chat_invite)(client_t, const char *const, const char *const, const char *const);
	fte_t	(*chat_set_topic)(client_t, const char *const, const char *const);
	fte_t	(*chat_op)(client_t, const char *const, const char *const);
	fte_t	(*chat_deop)(client_t, const char *const, const char *const);
	fte_t	(*chat_kick)(client_t, const char *const, const char *const, const char *const);
	fte_t	(*chat_send_message)(client_t, const char *const, const char *const, const int);
	fte_t	(*chat_send_action)(client_t, const char *const, const char *const, const int);
	char	*(*subcode_encode)(client_t, const char *const, const char *const);
	const char *(*room_normalize)(const char *const);
	client_t (*create_handle)();
	void	(*destroy_handle)(client_t);
} firetalk_protocol_t;

enum firetalk_connectstate {
	FCS_NOTCONNECTED,
	FCS_WAITING_SYNACK,
	FCS_WAITING_SIGNON,
	FCS_ACTIVE
};


fte_t	firetalk_register_protocol(const firetalk_protocol_t *const proto);

void firetalk_callback_im_getmessage(client_t c, const char *const sender, const int automessage, const char *const message);
void firetalk_callback_im_getaction(client_t c, const char *const sender, const int automessage, const char *const message);
void firetalk_callback_im_buddyonline(client_t c, const char *const nickname, const int online);
void firetalk_callback_im_buddyflags(client_t c, const char *const nickname, const int flags);
void firetalk_callback_im_buddyaway(client_t c, const char *const nickname, const int away);
void firetalk_callback_buddyadded(client_t c, const char *const name, const char *const group, const char *const friendly);
void firetalk_callback_buddyremoved(client_t c, const char *const name, const char *const group);
void firetalk_callback_typing(client_t c, const char *const name, const int typing);
void firetalk_callback_capabilities(client_t c, const char *const nickname, const char *const caps);
void firetalk_callback_warninfo(client_t c, const char *const nickname, const long warnval);
void firetalk_callback_error(client_t c, const int error, const char *const roomoruser, const char *const description);
void firetalk_callback_connectfailed(client_t c, const int error, const char *const description);
void firetalk_callback_connected(client_t c);
void firetalk_callback_disconnect(client_t c, const int error);
void firetalk_callback_gotinfo(client_t c, const char *const nickname, const char *const info, const int warning, const long online, const long idle, const int flags);
void firetalk_callback_idleinfo(client_t c, const char *const nickname, const long idletime);
void firetalk_callback_statusinfo(client_t c, const char *const nickname, const char *const message);
void firetalk_callback_doinit(client_t c, const char *const nickname);
void firetalk_callback_setidle(client_t c, long *const idle);
void firetalk_callback_eviled(client_t c, const int newevil, const char *const eviler);
void firetalk_callback_newnick(client_t c, const char *const nickname);
void firetalk_callback_passchanged(client_t c);
void firetalk_callback_user_nickchanged(client_t c, const char *const oldnick, const char *const newnick);
void firetalk_callback_chat_joined(client_t c, const char *const room);
void firetalk_callback_chat_left(client_t c, const char *const room);
void firetalk_callback_chat_kicked(client_t c, const char *const room, const char *const by, const char *const reason);
void firetalk_callback_chat_getmessage(client_t c, const char *const room, const char *const from, const int automessage, const char *const message);
void firetalk_callback_chat_getaction(client_t c, const char *const room, const char *const from, const int automessage, const char *const message);
void firetalk_callback_chat_invited(client_t c, const char *const room, const char *const from, const char *const message);
void firetalk_callback_chat_user_joined(client_t c, const char *const room, const char *const who, const char *const extra);
void firetalk_callback_chat_user_left(client_t c, const char *const room, const char *const who, const char *const reason);
void firetalk_callback_chat_user_quit(client_t c, const char *const who, const char *const reason);
void firetalk_callback_chat_gottopic(client_t c, const char *const room, const char *const topic, const char *const author);
#ifdef RAWIRCMODES
void firetalk_callback_chat_modechanged(client_t c, const char *const room, const char *const mode, const char *const by);
#endif
void firetalk_callback_chat_user_opped(client_t c, const char *const room, const char *const who, const char *const by);
void firetalk_callback_chat_user_deopped(client_t c, const char *const room, const char *const who, const char *const by);
void firetalk_callback_chat_keychanged(client_t c, const char *const room, const char *const what, const char *const by);
void firetalk_callback_chat_opped(client_t c, const char *const room, const char *const by);
void firetalk_callback_chat_deopped(client_t c, const char *const room, const char *const by);
void firetalk_callback_chat_user_kicked(client_t c, const char *const room, const char *const who, const char *const by, const char *const reason);
char *firetalk_subcode_get_request_reply(client_t c, const char *const command);
void firetalk_callback_subcode_request(client_t c, const char *const from, const char *const command, char *args);
void firetalk_callback_subcode_reply(client_t c, const char *const from, const char *const command, const char *const args);
void firetalk_callback_file_offer(client_t c, const char *const from, const char *const filename, const long size, const char *const ipstring, const char *const ip6string, const uint16_t port, const int type);
void firetalk_callback_needpass(client_t c, char *pass, const int size);

void firetalk_enqueue(firetalk_queue_t *queue, const char *const key, void *data);
const void *firetalk_peek(firetalk_queue_t *queue, const char *const key);
void *firetalk_dequeue(firetalk_queue_t *queue, const char *const key);
void firetalk_queue_append(char *buf, int buflen, firetalk_queue_t *queue, const char *const key);

firetalk_t firetalk_find_handle(client_t c);

fte_t	firetalk_chat_internal_add_room(firetalk_t conn, const char *const name);
fte_t	firetalk_chat_internal_add_member(firetalk_t conn, const char *const room, const char *const nickname);
fte_t	firetalk_chat_internal_remove_room(firetalk_t conn, const char *const name);
fte_t	firetalk_chat_internal_remove_member(firetalk_t conn, const char *const room, const char *const nickname);

struct s_firetalk_room *firetalk_find_room(struct s_firetalk_handle *c, const char *const room);
fte_t	firetalk_user_visible(firetalk_t conn, const char *const nickname);
fte_t	firetalk_user_visible_but(firetalk_t conn, const char *const room, const char *const nickname);

void firetalk_handle_send(struct s_firetalk_handle *c, struct s_firetalk_file *filestruct);
void firetalk_handle_receive(struct s_firetalk_handle *c, struct s_firetalk_file *filestruct);

void firetalk_internal_send_data(struct s_firetalk_handle *c, const char *const data, const int length);

int firetalk_internal_connect_host(const char *const host, const int port);
int firetalk_internal_connect(struct sockaddr_in *inet4_ip
#ifdef _FC_USE_IPV6
		, struct sockaddr_in6 *inet6_ip
#endif
		);
fte_t firetalk_internal_resolve4(const char *const host, struct in_addr *inet4_ip);
struct sockaddr_in *firetalk_internal_remotehost4(client_t c);
#ifdef _FC_USE_IPV6
fte_t firetalk_internal_resolve6(const char *const host, struct in6_addr *inet6_ip);
struct sockaddr_in6 *firetalk_internal_remotehost6(client_t c);
#endif
enum firetalk_connectstate firetalk_internal_get_connectstate(client_t c);
void firetalk_internal_set_connectstate(client_t c, enum firetalk_connectstate fcs);

fte_t	firetalk_set_timeout(unsigned int seconds);
fte_t	firetalk_clear_timeout();

const char *firetalk_debase64(const char *const str);
const char *firetalk_htmlentities(const char *str);

#endif
