/*
tester.c - FireTalk testing code
Copyright (C) 2000 Ian Gulliver
Copyright 2003-2004 Daniel Reed <n@ml.org>

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
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <time.h>

#include "firetalk-int.h"
#include "firetalk.h"

#define FIRETALK_TEST_STRING	"FireTalk test for " PACKAGE_NAME " " PACKAGE_VERSION

struct {
	void	*handle;
	char	*username;
	char	*password;
	int	waitingfor;
	char	*waitingstr;
}	conn1 = { 0 },
	conn2 = { 0 };

int	proto = -1;

char	*events[] = {
	  "",
#define WF_DOINIT 1
	  "DOINIT",
#define WF_BUDDYONLINE 2
	  "BUDDYONLINE",
#define WF_BUDDYOFFLINE 3
	  "BUDDYOFFLINE",
#define WF_IM_GETMESSAGE 4
	  "IM_GETMESSAGE",
#define WF_IM_GETACTION 5
	  "IM_GETACTION",
#define WF_IM_BUDDYAWAY 6
	  "IM_BUDDYAWAY",
#define WF_IM_BUDDYUNAWAY 7
	  "IM_BUDDYUNAWAY",
#define WF_GOTINFO 8
	  "GOTINFO",
#define WF_SUBCODE_REPLY 9
	  "SUBCODE_REPLY",
#define WF_CHAT_JOINED 10
	  "CHAT_JOINED",
#define WF_CHAT_USER_JOINED 11
	  "CHAT_USER_JOINED",
#define WF_CHAT_USER_LEFT 12
	  "CHAT_USER_LEFT",
#define WF_CHAT_LEFT 13
	  "CHAT_LEFT",
#define WF_CHAT_GETMESSAGE 14
	  "CHAT_GETMESSAGE",
#define WF_CHAT_GETACTION 15
	  "CHAT_GETACTION",
#define WF_CONNECTED 16
	  "CONNECTED",
};

#ifdef DEBUG_ECHO
void statrefresh(void) {
}
void status_echof(void) {
}
void *curconn = NULL;
#endif

#define CALLBACK_SANITY()					\
	assert((c == conn1.handle) || (c == conn2.handle));


#define WAITING_MUSTBE(EVENT) do {				\
	if (c == conn1.handle) {				\
		assert(conn1.waitingfor == EVENT);		\
		conn1.waitingfor = 0;				\
		conn1.waitingstr = NULL;			\
	} else if (c == conn2.handle) {				\
		assert(conn2.waitingfor == EVENT);		\
		conn2.waitingfor = 0;				\
		conn2.waitingstr = NULL;			\
	} else							\
		abort();					\
	fprintf(stderr, " [caught %s]", events[EVENT]);		\
	fflush(stderr);						\
} while (0)

#define WAITING_WANT(EVENT, STR) do {				\
	if (c == conn1.handle) {				\
		if (conn1.waitingfor == EVENT) {		\
			if (strcmp(STR, conn1.waitingstr) == 0)	{ \
				conn1.waitingfor = 0;		\
				conn1.waitingstr = NULL;	\
			} else					\
				fprintf(stderr, " [%s:%i: warning: spurious %s event (want [[[%s]]], got [[[%s]]])]", \
					__FILE__, __LINE__, events[EVENT], conn1.waitingstr, STR); \
		} else if (conn1.waitingfor != 0)		\
			fprintf(stderr, " [%s:%i: warning: spurious %s event while waiting for %s event]", \
				__FILE__, __LINE__, events[EVENT], events[conn1.waitingfor]); \
		else						\
			fprintf(stderr, " [%s:%i: warning: spurious %s event (unwanted)]", \
				__FILE__, __LINE__, events[EVENT]); \
	} else if (c == conn2.handle) {				\
		if (conn2.waitingfor == EVENT) {		\
			if (strcmp(STR, conn2.waitingstr) == 0) { \
				conn2.waitingfor = 0;		\
				conn2.waitingstr = NULL;	\
			} else					\
				fprintf(stderr, " [%s:%i: warning: spurious %s event (want [[[%s]]], got [[[%s]]])]", \
					__FILE__, __LINE__, events[EVENT], conn2.waitingstr, STR); \
		} else if (conn2.waitingfor != 0)		\
			fprintf(stderr, " [%s:%i: warning: spurious %s event while waiting for %s event]", \
				__FILE__, __LINE__, events[EVENT], events[conn2.waitingfor]); \
		else						\
			fprintf(stderr, " [%s:%i: warning: spurious %s event (unwanted)]", \
				__FILE__, __LINE__, events[EVENT]); \
	} else							\
		abort();					\
	fprintf(stderr, " [caught %s]", events[EVENT]);		\
	fflush(stderr);						\
} while (0)


void	needpass(void *c, void *cs, char *pass, int size) {
	pass[size-1] = 0;
	if (c == conn1.handle)
		strncpy(pass, conn1.password, size-1);
	else if (c == conn2.handle)
		strncpy(pass, conn2.password, size-1);
}

void	doinit(void *c, void *cs, char *nickname) {
	CALLBACK_SANITY();
	WAITING_MUSTBE(WF_DOINIT);
	if (c == conn1.handle)
		conn1.waitingfor = WF_CONNECTED;
	else if (c == conn2.handle)
		conn2.waitingfor = WF_CONNECTED;
	else
		abort();
}

void	error(void *c, void *cs, const int error, const char *const roomoruser, const char *const description) {
	if (roomoruser == NULL)
		fprintf(stderr, " [%s:%i: error %i (%s): %s]", __FILE__, __LINE__, error, firetalk_strerror(error), description);
	else
		fprintf(stderr, " [%s:%i: error %i in %s (%s): %s]", __FILE__, __LINE__, error, roomoruser, firetalk_strerror(error), description);
	fflush(stderr);
}

void	connected(void *c, void *cs) {
	CALLBACK_SANITY();
	WAITING_MUSTBE(WF_CONNECTED);
}

void	connectfailed(void *c, void *cs) {
	CALLBACK_SANITY();
	fprintf(stderr, "\r%s:%i: connection failed\r\n", __FILE__, __LINE__);
	abort();
}

void	buddy_online(void *c, void *cs, char *nickname) {
	CALLBACK_SANITY();
	WAITING_WANT(WF_BUDDYONLINE, nickname);
}

void	buddy_offline(void *c, void *cs, char *nickname) {
	CALLBACK_SANITY();
	WAITING_WANT(WF_BUDDYOFFLINE, nickname);
}

void	im_getmessage(void *c, void *cs, char *n, int a, char *m) {
	CALLBACK_SANITY();
	WAITING_WANT(WF_IM_GETMESSAGE, m);
}

void	im_getaction(void *c, void *cs, char *n, int a, char *m) {
	CALLBACK_SANITY();
	WAITING_WANT(WF_IM_GETACTION, m);
}

void	im_buddyaway(void *c, void *cs, char *nickname) {
	CALLBACK_SANITY();
	WAITING_WANT(WF_IM_BUDDYAWAY, nickname);
}

void	im_buddyunaway(void *c, void *cs, char *nickname) {
	CALLBACK_SANITY();
	WAITING_WANT(WF_IM_BUDDYUNAWAY, nickname);
}

void	gotinfo(void *c, void *cs, char *n, char *i) {
	CALLBACK_SANITY();
	WAITING_WANT(WF_GOTINFO, n);
}

void	subcode_reply(void *c, void *cs, const char *const from, const char *const command, const char *const args) {
	CALLBACK_SANITY();
	WAITING_WANT(WF_SUBCODE_REPLY, args);
}

void	chat_joined(void *c, void *cs, char *room) {
	CALLBACK_SANITY();
	WAITING_WANT(WF_CHAT_JOINED, room);
}

void	chat_user_joined(void *c, void *cs, char *room, char *who) {
	CALLBACK_SANITY();
	WAITING_WANT(WF_CHAT_USER_JOINED, who);
}

void	chat_getmessage(char *c, void *cs, const char *const room, const char *const from, const int autoflag, const char *const m) {
	CALLBACK_SANITY();
	WAITING_WANT(WF_CHAT_GETMESSAGE, m);
}

void	chat_getaction(char *c, void *cs, const char *const room, const char *const from, const int autoflag, const char *const m) {
	CALLBACK_SANITY();
	WAITING_WANT(WF_CHAT_GETACTION, m);
}

void	chat_user_left(void *c, void *cs, char *room, char *who, char *reason) {
	CALLBACK_SANITY();
	WAITING_WANT(WF_CHAT_USER_LEFT, who);
}

void	chat_left(void *c, void *cs, char *room) {
	CALLBACK_SANITY();
	WAITING_WANT(WF_CHAT_LEFT, room);
}

#define DO_TEST(suffix, args, failtest, errorcode) do {					\
	fprintf(stderr, "calling %s...", #suffix #args);					\
	fflush(stderr);									\
	errno = 0;									\
	ret = (void *)firetalk_ ## suffix args;						\
	if (failtest) {									\
		fprintf(stderr, " error %i (%s", (int)errorcode, firetalk_strerror((int)errorcode)); \
		if (errno != 0)								\
			fprintf(stderr, ": %m");					\
		fprintf(stderr, ")\r\n");						\
		exit(EXIT_FAILURE);							\
	}										\
	fprintf(stderr, " done");							\
	if ((ret != (void *)0) && (ret < (void *)10000))				\
		fprintf(stderr, " (%i)", (int)ret);					\
	else if (ret != (void *)0)							\
		fprintf(stderr, " (%p)", ret);						\
	fprintf(stderr, "\r\n");							\
} while (0)

#define DO_WAITFOR(conn, event, str) do {						\
	struct timeval t;								\
	time_t	tt;									\
											\
	conn.waitingfor = event;							\
	conn.waitingstr = str;								\
	time(&tt);									\
	fprintf(stderr, "%s waiting (up to 20 seconds) for event %s...\r\n", conn.username, events[event]); \
	while (((tt + 20) > time(NULL)) && (conn.waitingfor != 0)) {			\
		t.tv_sec = 20 - (time(NULL)-tt);					\
		t.tv_usec = 0;								\
		fprintf(stderr, " [%lu]\t", 20 - (time(NULL)-tt));			\
		DO_TEST(select_custom, (0, NULL, NULL, NULL, &t), ret != FE_SUCCESS, ret); \
	}										\
	if (conn.waitingfor != 0) {							\
		fprintf(stderr, "%s waiting (up to 20 seconds) for event %s... timed out waiting for event %s\r\n", conn.username, events[event], events[event]); \
		exit(EXIT_FAILURE);							\
	}										\
	fprintf(stderr, "%s waiting (up to 20 seconds) for event %s... done, %s event caught\r\n", conn.username, events[event], events[event]); \
	fprintf(stderr, "%s waiting for server sync...", conn.username);		\
	fflush(stderr);									\
	sleep(1);									\
	fprintf(stderr, " done\r\n");							\
} while (0)

int	main(int argc, char *argv[]) {
	int	i;
//	const char *protostr;

	fprintf(stderr, "API test for FireTalk/" PACKAGE_NAME " " PACKAGE_VERSION "\r\n");
	firetalk_find_protocol("");
//	for (i = 0; (protostr = firetalk_strprotocol(i)) != NULL; i++) {
	for (i = 0; i < 1; i++) {
		void	*ret;
		char	*fttest;

		conn1.username = strdup("naimtest1");
		if (conn1.username == NULL)
			abort();
		conn1.password = strdup("");
		if (conn1.password == NULL)
			abort();

		conn2.username = strdup("naimtest2");
		if (conn2.username == NULL)
			abort();
		conn2.password = strdup("");
		if (conn2.password == NULL)
			abort();

		fprintf(stderr, "TEST %i\r\n", i);
		fprintf(stderr, "Protocol: %s\r\n", firetalk_strprotocol(i));
		fprintf(stderr, "\r\n");

		DO_TEST(create_handle, (i, NULL), ret == NULL, firetalkerror);
			conn1.handle = ret;
		DO_TEST(create_handle, (i, NULL), ret == NULL, firetalkerror);
			conn2.handle = ret;
		DO_TEST(register_callback, (conn1.handle, FC_ERROR, (ptrtofnct)error), ret != FE_SUCCESS, ret);
		DO_TEST(register_callback, (conn1.handle, FC_NEEDPASS, (ptrtofnct)needpass), ret != FE_SUCCESS, ret);
		DO_TEST(register_callback, (conn1.handle, FC_CONNECTED, (ptrtofnct)connected), ret != FE_SUCCESS, ret);
		DO_TEST(register_callback, (conn1.handle, FC_CONNECTFAILED, (ptrtofnct)connectfailed), ret != FE_SUCCESS, ret);
		DO_TEST(signon, (conn1.handle, NULL, 0, conn1.username), ret != FE_SUCCESS, ret);
		DO_WAITFOR(conn1, WF_CONNECTED, NULL);

		DO_TEST(register_callback, (conn2.handle, FC_ERROR, (ptrtofnct)error), ret != FE_SUCCESS, ret);
		DO_TEST(register_callback, (conn2.handle, FC_NEEDPASS, (ptrtofnct)needpass), ret != FE_SUCCESS, ret);
		DO_TEST(register_callback, (conn2.handle, FC_CONNECTED, (ptrtofnct)connected), ret != FE_SUCCESS, ret);
		DO_TEST(register_callback, (conn2.handle, FC_CONNECTFAILED, (ptrtofnct)connectfailed), ret != FE_SUCCESS, ret);
		DO_TEST(register_callback, (conn2.handle, FC_DOINIT, (ptrtofnct)doinit), ret != FE_SUCCESS, ret);
		DO_TEST(signon, (conn2.handle, NULL, 0, conn2.username), ret != FE_SUCCESS, ret);
		DO_WAITFOR(conn2, WF_DOINIT, NULL);

		DO_TEST(register_callback, (conn1.handle, FC_IM_GOTINFO, (ptrtofnct)gotinfo), ret != FE_SUCCESS, ret);
		DO_TEST(set_info, (conn2.handle, FIRETALK_TEST_STRING), ret != FE_SUCCESS, ret);
		DO_TEST(im_get_info, (conn1.handle, conn2.username), ret != FE_SUCCESS, ret);
		DO_WAITFOR(conn1, WF_GOTINFO, conn2.username);

		DO_TEST(register_callback, (conn1.handle, FC_IM_GETMESSAGE, (ptrtofnct)im_getmessage), ret != FE_SUCCESS, ret);
		DO_TEST(im_send_message, (conn2.handle, conn1.username, FIRETALK_TEST_STRING, 0), ret != FE_SUCCESS, ret);
		DO_WAITFOR(conn1, WF_IM_GETMESSAGE, FIRETALK_TEST_STRING);

		DO_TEST(register_callback, (conn1.handle, FC_IM_GETACTION, (ptrtofnct)im_getaction), ret != FE_SUCCESS, ret);
		DO_TEST(im_send_action, (conn2.handle, conn1.username, FIRETALK_TEST_STRING, 0), ret != FE_SUCCESS, ret);
		DO_WAITFOR(conn1, WF_IM_GETACTION, FIRETALK_TEST_STRING);

		DO_TEST(subcode_register_reply_callback, (conn1.handle, "PING", subcode_reply), ret != FE_SUCCESS, ret);
		DO_TEST(subcode_send_request, (conn1.handle, conn2.username, "PING", FIRETALK_TEST_STRING), ret != FE_SUCCESS, ret);
		DO_WAITFOR(conn1, WF_SUBCODE_REPLY, FIRETALK_TEST_STRING);

		DO_TEST(chat_normalize, (conn1.handle, "fttest"), ret == NULL, firetalkerror);
			fttest = strdup(ret);

		DO_TEST(register_callback, (conn1.handle, FC_CHAT_JOINED, (ptrtofnct)chat_joined), ret != FE_SUCCESS, ret);
		DO_TEST(chat_join, (conn1.handle, fttest), ret != FE_SUCCESS, ret);
		DO_WAITFOR(conn1, WF_CHAT_JOINED, fttest);

		DO_TEST(register_callback, (conn1.handle, FC_CHAT_USER_JOINED, (ptrtofnct)chat_user_joined), ret != FE_SUCCESS, ret);
		DO_TEST(register_callback, (conn2.handle, FC_CHAT_JOINED, (ptrtofnct)chat_joined), ret != FE_SUCCESS, ret);
		DO_TEST(chat_join, (conn2.handle, fttest), ret != FE_SUCCESS, ret);
		DO_WAITFOR(conn1, WF_CHAT_USER_JOINED, conn2.username);
		DO_WAITFOR(conn2, WF_CHAT_JOINED, fttest);

		DO_TEST(register_callback, (conn1.handle, FC_IM_BUDDYONLINE, (ptrtofnct)buddy_online), ret != FE_SUCCESS, ret);
		DO_TEST(im_remove_buddy, (conn1.handle, conn2.username), (ret != FE_SUCCESS) && (ret != (void *)FE_NOTFOUND), ret);
		DO_TEST(im_add_buddy, (conn1.handle, conn2.username, "Test Group", "Test Friendly"), ret != FE_SUCCESS, ret);
//		DO_WAITFOR(conn1, WF_BUDDYONLINE, conn2.username);

		DO_TEST(register_callback, (conn1.handle, FC_IM_BUDDYAWAY, (ptrtofnct)im_buddyaway), ret != FE_SUCCESS, ret);
		DO_TEST(set_away, (conn2.handle, FIRETALK_TEST_STRING, 0), ret != FE_SUCCESS, ret);
//		DO_WAITFOR(conn1, WF_IM_BUDDYAWAY, conn2.username);

		DO_TEST(register_callback, (conn1.handle, FC_IM_BUDDYUNAWAY, (ptrtofnct)im_buddyunaway), ret != FE_SUCCESS, ret);
		DO_TEST(set_away, (conn2.handle, NULL, 0), ret != FE_SUCCESS, ret);
//		DO_WAITFOR(conn1, WF_IM_BUDDYUNAWAY, conn2.username);

		DO_TEST(register_callback, (conn1.handle, FC_CHAT_GETMESSAGE, (ptrtofnct)chat_getmessage), ret != FE_SUCCESS, ret);
		DO_TEST(chat_send_message, (conn2.handle, fttest, FIRETALK_TEST_STRING, 0), ret != FE_SUCCESS, ret);
		DO_WAITFOR(conn1, WF_CHAT_GETMESSAGE, FIRETALK_TEST_STRING);

		DO_TEST(register_callback, (conn1.handle, FC_CHAT_GETACTION, (ptrtofnct)chat_getaction), ret != FE_SUCCESS, ret);
		DO_TEST(chat_send_action, (conn2.handle, fttest, FIRETALK_TEST_STRING, 0), ret != FE_SUCCESS, ret);
		DO_WAITFOR(conn1, WF_CHAT_GETACTION, FIRETALK_TEST_STRING);

		DO_TEST(register_callback, (conn1.handle, FC_CHAT_USER_LEFT, (ptrtofnct)chat_user_left), ret != FE_SUCCESS, ret);
		DO_TEST(chat_part, (conn2.handle, fttest), ret != FE_SUCCESS, ret);
		DO_WAITFOR(conn1, WF_CHAT_USER_LEFT, conn2.username);

		DO_TEST(register_callback, (conn1.handle, FC_CHAT_LEFT, (ptrtofnct)chat_left), ret != FE_SUCCESS, ret);
		DO_TEST(chat_part, (conn1.handle, fttest), ret != FE_SUCCESS, ret);
		DO_WAITFOR(conn1, WF_CHAT_LEFT, fttest);

		DO_TEST(register_callback, (conn1.handle, FC_IM_BUDDYOFFLINE, (ptrtofnct)buddy_offline), ret != FE_SUCCESS, ret);
		DO_TEST(disconnect, (conn2.handle), ret != FE_SUCCESS, ret);
//		DO_WAITFOR(conn1, WF_BUDDYOFFLINE, conn2.username);

		DO_TEST(disconnect, (conn1.handle), ret != FE_SUCCESS, ret);

		fprintf(stderr, "destroying conn1.handle...");
		fflush(stderr);
		firetalk_destroy_handle(conn1.handle);
		fprintf(stderr, " done\r\n");

		fprintf(stderr, "destroying conn2.handle...");
		fflush(stderr);
		firetalk_destroy_handle(conn2.handle);
		fprintf(stderr, " done\r\n");

		free(fttest);
		fttest = NULL;
		free(conn1.username);
		conn1.username = NULL;
		free(conn1.password);
		conn1.password = NULL;
		free(conn2.username);
		conn2.username = NULL;
		free(conn2.password);
		conn2.password = NULL;

		fprintf(stderr, "\r\n");
	}
	return 0;
}
