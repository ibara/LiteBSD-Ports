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

#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <locale.h>
#include <netdb.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define GROUPMAX 8
#define NICKMAX 16
#define MSGMAX 253
#define PACKETMAX 255
#define HOSTMAX 256

extern const char    *__progname;

static char 	group[GROUPMAX] = "1";
static char 	nick[NICKMAX] = "icb";
static char 	packet[PACKETMAX];
static char 	tbuf[6];

static int 	running = 1;
static int 	success = 1;
static int 	sockfd;

static time_t 	t;
static struct tm *tm;

static int 	connectserver(char [], char []);
static void 	client(void);
static void 	commandparse(char []);
static void 	gotpacket(char [], int);
static void 	printicmd(char [], int);
static void 	printiwl(char []);
static void 	printmsg(char [], int, int);
static void 	sendbeep(char []);
static void 	sendlogin(void);
static void 	sendpacket(void);
static void 	sendpm(char []);
static void 	sendpong(void);
static void 	sendtopic(char []);
static void 	timestamp(void);
static void 	usage(void);
static void 	version(void);

int
main(int argc, char *argv[])
{
	int		ch, p;
	char 		addr[HOSTMAX] = "localhost";
	char 		port[6] = "7326";

	setlocale(LC_ALL, "");

	while ((ch = getopt(argc, argv, "g:hn:p:v")) != -1) {
		switch (ch) {
		case 'g':
			strlcpy(group, optarg, sizeof(group));
			break;
		case 'n':
			strlcpy(nick, optarg, sizeof(nick));
			break;
		case 'p':
			p = atoi(optarg);
			if (p < 0 || p > 66535)
				(void) fprintf(stderr, "%s: Bogus port number, using 7326", __progname);
			else
				strlcpy(port, optarg, sizeof(port));
			break;
		case 'v':
			version();
			break;
		case 'h':
		default:
			usage();
			/* NOTREACHED */
		}
	}
	argc -= optind;
	argv += optind;

	if (argc > 1)
		usage();

	if (argv[0] != NULL)
		strlcpy(addr, argv[0], sizeof(addr));

	/* Connect to server.  */
	if ((connectserver(addr, port)) != 0) {
		fputs("Could not connect to server", stderr);
		exit(1);
	}
	setvbuf(stdout, NULL, _IONBF, 0);

	client();
	close(sockfd);

	return 0;
}

void
client(void)
{
	char 		buf[MSGMAX];
	char 		dbuf[13];
	char 		msg[PACKETMAX];
	unsigned char 	psize[1];
	int 		len, p;
	int 		ts = 1;
	struct pollfd 	pfd[2];

	pfd[0].fd = sockfd;
	pfd[0].events = POLLIN;
	pfd[1].fd = fileno(stdin);
	pfd[1].events = POLLIN;

	/* Main loop */
	while (running) {
		p = poll(pfd, 2, 15000);
		if (p < 0) {
			fprintf(stderr, "%s\n", strerror(errno));
			exit(1);
		} else if (p == 0) {
			sendpong();
		} else {	/* p > 0 */
			timestamp();
			if ((strcmp("00:00", tbuf)) == 0) {
				if (ts) {
					strftime(dbuf, 13, "%d %b %Y", tm);
					fprintf(stdout, "Day changed to %s\n",
						dbuf);
					ts = 0;
				}
			} else {
				ts = 1;
			}
		}

		/* Lily people say processing in first makes it look snappy.  */
		if (pfd[1].revents & POLLIN) {
			ioctl(0, FIONREAD, &p);
			if (p > MSGMAX)
				p = MSGMAX;
			if (read(STDIN_FILENO, buf, p) == -1) {
				fprintf(stderr, "%s\n", strerror(errno));
				exit(1);
			}
			buf[--p] = '\0';
			if (buf[0] == '\0') {
				success = 0;
			} else if (buf[0] == '/') {
				commandparse(buf);
			} else {
				snprintf(packet, sizeof(packet), "%cb%s",
					 (int) sizeof(packet) - 1, buf);
			}
			if (success)
				sendpacket();
			success = 1;
		}
		if (pfd[0].revents & (POLLERR | POLLNVAL))
			fprintf(stderr, "bad fd %d\n", pfd[0].fd);
		if (pfd[0].revents & (POLLIN | POLLHUP)) {
			if ((recv(sockfd, psize, 1, 0)) > 0) {
				len = (int) psize[0];
				if ((recv(sockfd, msg, len, 0)) > 0) {
					gotpacket(msg, len);
				} else {
					timestamp();
					fprintf(stdout, "%s ", tbuf);
					fputs("-!- [Error] Connection closed by server\n", stdout);
					running = 0;
				}
			} else {
				timestamp();
				fprintf(stdout, "%s ", tbuf);
				fputs("-!- [Error] Connection closed by server\n", stdout);
				running = 0;
			}
		}
	}
}

void
commandparse(char buf[])
{
	char 		tmpgroup[GROUPMAX];
	char	 	tmpnick[NICKMAX];
	char 		toboot[NICKMAX];
	unsigned int 	i;
	int 		j = 0;

	if (strncmp(buf, "/nick", 5) == 0 || strncmp(buf, "/name", 5) == 0) {
		if (buf[5] != ' ') {
			success = 0;
		} else {
			for (i = 6; j < NICKMAX - 1; i++) {
				if (buf[i] == ' ')
					tmpnick[j++] = '_';
				else
					tmpnick[j++] = buf[i];
			}
			tmpnick[j] = '\0';
			if (tmpnick[0] != '\0') {
				strlcpy(nick, tmpnick, sizeof(nick));
				snprintf(packet, sizeof(packet),
					 "%chname\001%s",
					 (int) sizeof(packet) - 1, nick);
			} else {
				success = 0;
			}
		}
	} else if (strncmp(buf, "/m", 2) == 0) {
		sendpm(buf);
	} else if (strncmp(buf, "/g", 2) == 0) {
		if (buf[2] != ' ') {
			success = 0;
		} else {
			for (i = 3; j < GROUPMAX - 1; i++)
				tmpgroup[j++] = buf[i];
			tmpgroup[j] = '\0';
			if (tmpgroup[0] == '\0') {
				success = 0;
			} else {
				strlcpy(group, tmpgroup, sizeof(group));
				snprintf(packet, sizeof(packet), "%chg\001%s",
					 (int) sizeof(packet) - 1, group);
			}
		}
	} else if (strncmp(buf, "/beep", 5) == 0) {
		sendbeep(buf);
	} else if (strncmp(buf, "/topic", 6) == 0 ||
		   strncmp(buf, "/t", 2) == 0) {
		sendtopic(buf);
	} else if (strncmp(buf, "/boot", 5) == 0) {
		if (buf[5] != ' ') {
			success = 0;
		} else {
			for (i = 6; j < NICKMAX - 1; i++)
				toboot[j++] = buf[i];
			toboot[j] = '\0';
			if (toboot[0] == '\0') {
				success = 0;
			} else {
				snprintf(packet, sizeof(packet),
					 "%chboot\001%s",
					 (int) sizeof(packet) - 1, toboot);
			}
		}
	} else if (strcmp(buf, "/q") == 0 || strcmp(buf, "/quit") == 0) {
		success = 0;
		running = 0;
	} else if (strcmp(buf, "/?") == 0) {
		snprintf(packet, sizeof(packet), "%ch?",
			 (int) sizeof(packet) - 1);
	} else if (strcmp(buf, "/nobeep") == 0) {
		snprintf(packet, sizeof(packet), "%chnobeep",
			 (int) sizeof(packet) - 1);
	} else if (strcmp(buf, "/pass") == 0) {
		snprintf(packet, sizeof(packet), "%chpass",
			 (int) sizeof(packet) - 1);
	} else if (strcmp(buf, "/w") == 0) {
		snprintf(packet, sizeof(packet), "%chw",
			 (int) sizeof(packet) - 1);
	} else {
		fputs("Not a command\n", stdout);
		success = 0;
	}
}

static int
connectserver(char addr[], char port[])
{
	int 		error;
	struct addrinfo hints, *res;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = PF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	if ((error = getaddrinfo(addr, port, &hints, &res)) != 0) {
		fprintf(stderr, "%s\n", gai_strerror(error));
		exit(1);
	}
	if ((sockfd = socket(res->ai_family, res->ai_socktype,
	     res->ai_protocol)) == -1) {
		fprintf(stderr, "%s\n", strerror(errno));
		exit(1);
	}
	if ((connect(sockfd, res->ai_addr, res->ai_addrlen)) == -1) {
		fprintf(stderr, "%s\n", strerror(errno));
		exit(1);
	}
	freeaddrinfo(res);

	return 0;
}

static void
gotpacket(char msg[], int len)
{
	int	i;

	/* What did we get?  */
	switch (msg[0]) {
	case 'a':
		break;
	case 'b':
		printmsg(msg, len, 0);
		break;
	case 'c':
		printmsg(msg, len, 1);
		break;
	case 'd':
	case 'f':
		printmsg(msg, len, 2);
		break;
	case 'e':
		timestamp();
		fprintf(stdout, "%s ", tbuf);
		fputs("-!- [Error] ", stdout);
		for (i = 1; i < len; i++)
			fputc(msg[i], stdout);
		fputc('\n', stdout);
		break;
	case 'g':
		timestamp();
		fprintf(stdout, "%s ", tbuf);
		fputs("-!- [Error] Connection closed by server\n", stdout);
		running = 0;
		break;
	case 'i':
		printicmd(msg, len);
		break;
	case 'j':
		sendlogin();	/* Now we can log in.  */
		break;
	case 'k':
		timestamp();
		fprintf(stdout, "%s ", tbuf);
		fputs("-!- [Beep] You were beeped by ", stdout);
		for (i = 1; i < len; i++)
			fputc(msg[i], stdout);
		fputc('\n', stdout);
		break;
	case 'l':
	case 'm':
		break;
	default:
		fputs("Received invalid packet.\n", stdout);
		break;
	}
}

static void
printicmd(char msg[], int len)
{
	int 	i;

	switch (msg[1]) {
	case 'c':
		timestamp();
		fprintf(stdout, "%s ", tbuf);
		for (i = 3; i < len; i++)
			fputc(msg[i], stdout);
		fputc('\n', stdout);
		break;
	case 'w':
		printiwl(msg);
		break;
	default:
		break;
	}
}

static void
printiwl(char msg[])
{
	int 		i;
	int 		j = 0;
	time_t 		tconv;
	char 		ltbuf[16];
	char 		timelog[17];
	struct tm      *ltm;

	timestamp();
	fprintf(stdout, "%s ", tbuf);

	/*
	 * What's going on here?
	 *
	 * This is what each for loop does:
	 * 1. Is this user a mod? Print '*' if yes or ' ' if no.
	 * 2. Print nick.
	 * 3. Idle time. Eat it for now (XXX).
	 * 4. Something no longer used. icbd sends a 0. Eat it.
	 * 5. Print login time.
	 * 6. Print user name. Nominally getuser().
	 * 7. Print hostname of user.
	 */
	for (i = 4; msg[i] != '\001'; i++) {
		if (msg[i] != ' ')
			fputc('*', stdout);
		else
			fputc(' ', stdout);
	}
	for (i++; msg[i] != '\001'; i++)
		fputc(msg[i], stdout);
	for (i++; msg[i] != '\001'; i++)
		;
	for (i++; msg[i] != '\001'; i++)
		;
	fputc(' ', stdout);
	for (i++; msg[i] != '\001'; i++)
		timelog[j++] = msg[i];
	timelog[j] = '\0';
	tconv = atoi(timelog);
	ltm = localtime(&tconv);
	strftime(ltbuf, 16, "%b %e %R", ltm);
	fprintf(stdout, "%s ", ltbuf);
	for (i++; msg[i] != '\001'; i++)
		fputc(msg[i], stdout);
	fputc('@', stdout);
	for (i++; msg[i] != '\001'; i++)
		fputc(msg[i], stdout);
	fputc('\n', stdout);
}

/*
 * int flag options:
 * 0 = normal message, 1 = private message, 2 = error or important message
 */
static void
printmsg(char msg[], int len, int flag)
{
	int 	i;

	timestamp();
	fprintf(stdout, "%s ", tbuf);
	if (flag == 1)
		fputs("-P- <", stdout);
	else if (flag == 2)
		fputs("-!- [", stdout);
	else
		fputc('<', stdout);

	for (i = 1; i < len; i++) {
		if (msg[i] == '\001') {
			if (flag == 2)
				fputs("] ", stdout);
			else
				fputs("> ", stdout);
		} else {
			fputc(msg[i], stdout);
		}
	}
	fputc('\n', stdout);
}

static void
sendbeep(char buf[])
{
	char 		msg[MSGMAX];
	unsigned int 	i;
	int 		j = 0;

	if (strncmp(buf, "/beep ", 6) == 0) {
		for (i = 6; i < strlen(buf); i++)
			msg[j++] = buf[i];
		msg[j] = '\0';
		snprintf(packet, sizeof(packet), "%chbeep\001%s",
			 (int) sizeof(packet) - 1, msg);
	} else {
		success = 0;
	}
}

static void
sendlogin(void)
{
	char   *password = NULL;	/* Required but unused.  */

	/* Build login packet.  */
	snprintf(packet, sizeof(packet), "%ca%s\001%s\001%s\001login\001%s",
		 (int) sizeof(packet) - 1, getlogin(), nick, group, password);

	/* Send login packet.  */
	sendpacket();
}

static void
sendpacket(void)
{
	if ((send(sockfd, packet, sizeof(packet), 0)) == -1) {
		fprintf(stderr, "%s\n", strerror(errno));
		exit(1);
	}
}

static void
sendpm(char buf[])
{
	char		msg[MSGMAX];
	unsigned int 	i;
	int 		j = 0;

	if (strncmp(buf, "/m ", 3) == 0) {
		for (i = 3; i < strlen(buf); i++)
			msg[j++] = buf[i];
		msg[j] = '\0';
		snprintf(packet, sizeof(packet), "%chm\001%s",
			 (int) sizeof(packet) - 1, msg);
	} else if (strncmp(buf, "/msg ", 5) == 0) {
		for (i = 5; i < strlen(buf); i++)
			msg[j++] = buf[i];
		msg[j] = '\0';
		snprintf(packet, sizeof(packet), "%chmsg\001%s",
			 (int) sizeof(packet) - 1, msg);
	} else {
		success = 0;
	}
}

static void
sendpong(void)
{
	snprintf(packet, sizeof(packet), "%cm",
		 (int) sizeof(packet) - 1);

	sendpacket();
}

static void
sendtopic(char buf[])
{
	char 		msg[MSGMAX];
	unsigned int 	i;
	int 		j = 0;

	if (strncmp(buf, "/topic ", 7) == 0) {
		for (i = 7; i < strlen(buf); i++)
			msg[j++] = buf[i];
		msg[j] = '\0';
	} else if (strncmp(buf, "/t ", 3) == 0) {
		for (i = 3; i < strlen(buf); i++)
			msg[j++] = buf[i];
		msg[j] = '\0';
	} else if (strncmp(buf, "/topic", 5) == 0 || strncmp(buf, "/t", 2) == 0) {
		msg[0] = '\0';
	} else {
		success = 0;
	}
	if (success) {
		snprintf(packet, sizeof(packet), "%chtopic\001%s",
			 (int) sizeof(packet) - 1, msg);
	}
}

static void
timestamp(void)
{
	t = time(NULL);
	tm = localtime(&t);
	strftime(tbuf, 6, "%R", tm);
}

static void
usage(void)
{
	(void) fprintf(stderr, "usage: %s [-hv] [-g group] [-n nick] [-p port] [server]\n", __progname);
	exit(1);
}

static void
version(void)
{
	const char 	v[] = "icb version 6\nCopyright (c) 2015-2016 Brian Callahan <bcallah@openbsd.org>\n\nPermission to use, copy, modify, and distribute this software for any\npurpose with or without fee is hereby granted, provided that the above\ncopyright notice and this permission notice appear in all copies.\n\nTHE SOFTWARE IS PROVIDED \"AS IS\" AND THE AUTHOR DISCLAIMS ALL WARRANTIES\nWITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF\nMERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR\nANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES\nWHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN\nACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF\nOR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.\n";

	(void) fprintf(stderr, v);
	exit(1);
}
