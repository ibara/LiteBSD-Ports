/*
 * Copyright (c) 2015-2016 Brian Callahan <bcallah@openbsd.org>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <errno.h>
#include <netdb.h>
#include <pwd.h>
#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <time.h>
#include <unistd.h>

#define MAX(a,b) (((a) > (b)) ? (a) : (b))

#define BACKLOG	5
#define TIMEBUF	30

#ifndef INET_ADDRSTRLEN
#define INET_ADDRSTRLEN 16
#endif

extern char    *__progname;

static char 	now[TIMEBUF];
static int 	tcpfd, udpfd;

char           *daytime(void);
void 		quit(int);
void 		sendtcp(void);
void 		sendudp(void);
void 		starttcp(void);
void 		startudp(void);
static void 	usage(void);

int
main(int argc, char *argv[])
{
	fd_set 		set;
	int 		ch, uflag = 0, p;
	struct passwd  *pw;

	if (geteuid() != 0) {
		(void) fprintf(stderr, "%s: need root privileges\n",
			       __progname);
		exit(1);
	}
	pw = getpwnam("daytimed");
	if (pw == NULL) {
		(void) fprintf(stderr, "%s: no daytimed user\n", __progname);
		exit(1);
	}

	while ((ch = getopt(argc, argv, "u")) != -1) {
		switch (ch) {
		case 'u':
			uflag = 1;
			break;
		default:
			usage();
			/* NOTREACHED */
		}
	}
	argc -= optind;
	argv += optind;

	openlog(__progname, LOG_PID | LOG_NDELAY, LOG_DAEMON);

	/* Start TCP and UDP protocols separately. */
	starttcp();
	if (uflag)
		startudp();

	/* Get time zone information before chroot. */
	tzset();

	/* Drop privileges */
	if (chroot("/var/empty") != 0 || chdir("/") != 0) {
		(void) fprintf(stderr, "%s: could not chroot\n", __progname);
		exit(1);
	}
	if (setgroups(1, &pw->pw_gid) ||
	    setgid(pw->pw_gid) || setuid(pw->pw_uid)) {
		(void) fprintf(stderr, "%s: can't drop privileges\n",
			       __progname);
		exit(1);
	}

        if (daemon(0, 0) < 0) {
                (void) fprintf(stderr, "%s: unable to daemonize\n", __progname);
		exit(1);
	}

#ifdef __OpenBSD__
	if ((p = pledge("stdio inet proc", NULL)) == -1) {
		fprintf(stderr, "%s: pledge(2) failed: %s\n", __progname,
			strerror(errno));
		exit(1);
	}
#endif

	signal(SIGCHLD, SIG_IGN);
	signal(SIGUSR1, quit);

	FD_ZERO(&set);

	while (1) {
		FD_SET(tcpfd, &set);
		if (uflag)
			FD_SET(udpfd, &set);

		if (select(MAX(tcpfd, udpfd) + 1, &set, NULL, NULL, NULL) < 0) {
			syslog(LOG_ERR, "%s", strerror(errno));
			exit(1);
		}

		if (tcpfd > -1 && FD_ISSET(tcpfd, &set))
			sendtcp();

		if (uflag && udpfd > -1 && FD_ISSET(udpfd, &set))
			sendudp();
	}

	return 0;
}

char *
daytime(void)
{
	time_t 		t;
	struct tm      *tm;

	t = time(NULL);
	tm = localtime(&t);
	strftime(now, TIMEBUF, "%a %b %e %H:%M:%S %Z %Y\n", tm);

	return now;
}

void
quit(int signal)
{
	syslog(LOG_INFO, "shutdown by user");
	closelog();
	exit(0);
}

void
sendtcp(void)
{
	struct sockaddr_storage ss;
	socklen_t 		ss_len = sizeof ss;
	char 			client[INET_ADDRSTRLEN];
	int 			timefd;

	if ((timefd = accept(tcpfd, (struct sockaddr *)&ss, &ss_len)) == -1) {
		syslog(LOG_ERR, "%s", strerror(errno));
		return;
	}
	inet_ntop(AF_INET,
		  &((struct sockaddr_in *)&ss)->sin_addr,
		  client, sizeof client);
	if (send(timefd, daytime(), TIMEBUF, 0) < 0) {
		syslog(LOG_ERR, "%s", strerror(errno));
		exit(1);
	}
	close(timefd);
}

void
sendudp(void)
{
	struct sockaddr_storage ss;
	socklen_t 		ss_len = sizeof ss;
	char 			client[INET_ADDRSTRLEN];

	if ((recvfrom(udpfd, NULL, 0, 0, (struct sockaddr *)&ss,
		      &ss_len)) < 0) {
		syslog(LOG_ERR, "%s", strerror(errno));
		return;
	}
	inet_ntop(AF_INET,
		  &((struct sockaddr_in *)&ss)->sin_addr,
		  client, sizeof client);
	if (sendto(udpfd, daytime(), TIMEBUF, 0,
		   (struct sockaddr *)&ss, ss_len) < 0)
		syslog(LOG_ERR, "%s", strerror(errno));
}

void
starttcp(void)
{
	struct addrinfo hints, *res, *res0;
	int 		error, on = 1;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = PF_UNSPEC;
	hints.ai_flags = AI_PASSIVE;
	hints.ai_socktype = SOCK_STREAM;

	if ((error = getaddrinfo(NULL, "13", &hints, &res0)) != 0) {
		syslog(LOG_ERR, "getaddrinfo: %s",
			 gai_strerror(error));
		exit(1);
	}
	for (res = res0; res != NULL; res = res->ai_next) {
		if ((tcpfd = socket(res->ai_family, res->ai_socktype,
				    res->ai_protocol)) < 0) {
			syslog(LOG_ERR, "%s", strerror(errno));
			continue;
		}
		if (setsockopt(tcpfd, SOL_SOCKET, SO_REUSEADDR, &on,
			       sizeof on) < 0) {
			syslog(LOG_ERR, "%s", strerror(errno));
			close(tcpfd);
			continue;
		}
		if (bind(tcpfd, res->ai_addr, res->ai_addrlen) < 0) {
			syslog(LOG_ERR, "%s", strerror(errno));
			close(tcpfd);
			continue;
		}

		break;
	}

	freeaddrinfo(res0);

	if (res == NULL) {
		syslog(LOG_ERR, "Failed to bind");
		exit(1);
	}

	if (listen(tcpfd, BACKLOG) < 0) {
		syslog(LOG_ERR, "%s", strerror(errno));
		exit(1);
	}
}

void
startudp(void)
{
	struct addrinfo hints, *res, *res0;
	int 		error, on = 1;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = PF_UNSPEC;
	hints.ai_flags = AI_PASSIVE;
	hints.ai_socktype = SOCK_DGRAM;

	if ((error = getaddrinfo(NULL, "13", &hints, &res0)) != 0) {
		syslog(LOG_ERR, "getaddrinfo: %s",
			 gai_strerror(error));
		exit(1);
	}
	for (res = res0; res != NULL; res = res->ai_next) {
		if ((udpfd = socket(res->ai_family, res->ai_socktype,
				    res->ai_protocol)) < 0) {
			syslog(LOG_ERR, "%s", strerror(errno));
			continue;
		}
		if (setsockopt(udpfd, SOL_SOCKET, SO_REUSEADDR,
			       &on, sizeof on) < 0) {
			syslog(LOG_ERR, "%s", strerror(errno));
			close(udpfd);
			continue;
		}
		if (bind(udpfd, res->ai_addr, res->ai_addrlen) < 0) {
			syslog(LOG_ERR, "%s", strerror(errno));
			close(udpfd);
			continue;
		}

		break;
	}

	freeaddrinfo(res0);

	if (res == NULL) {
		syslog(LOG_ERR, "Failed to bind");
		exit(1);
	}
}

static void
usage(void)
{
	(void) fprintf(stderr, "usage: %s [-u]\n", __progname);
	exit(1);
}
