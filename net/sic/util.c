/* See LICENSE file for license details. */
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>

static unsigned short
lhtons(unsigned short x)
{
	unsigned char *s = (void *) &x;

	return (unsigned short)(s[0] << 8 | s[1]);
}

static void
eprint(const char *fmt, ...) {
	va_list ap;

	va_start(ap, fmt);
	vsnprintf(bufout, sizeof bufout, fmt, ap);
	va_end(ap);
	fprintf(stderr, "%s", bufout);
	if(fmt[0] && fmt[strlen(fmt) - 1] == ':')
		fprintf(stderr, " %s\n", strerror(errno));
	exit(1);
}

static int
dial(char *host, char *port) {
	int fd;
	struct sockaddr_in sin;
	struct hostent *hp = gethostbyname(host);

	memset(&sin, 0, sizeof(struct sockaddr_in));
	if (!hp) {
		fprintf(stderr, "sic: cannot retrieve host information\n");
		exit(1);
	}
	sin.sin_family = AF_INET;
	memcpy(&sin.sin_addr, hp->h_addr, hp->h_length);
	sin.sin_port = lhtons(atoi(port));
	if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		fprintf(stderr, "sic: cannot create socket\n");
		exit(1);
	}
	if (connect(fd, (const struct sockaddr *) &sin, sizeof(sin)) < 0) {
		fprintf(stderr, "sic: cannot connect to host\n");
		exit(1);
	}
	return fd;
}

static char *
eat(char *s, int (*p)(int), int r) {
	while(*s != '\0' && p(*s) == r)
		s++;
	return s;
}

static char*
skip(char *s, char c) {
	while(*s != c && *s != '\0')
		s++;
	if(*s != '\0')
		*s++ = '\0';
	return s;
}

static void
trim(char *s) {
	char *e;

	e = s + strlen(s) - 1;
	while(isspace(*e) && e > s)
		e--;
	*(e + 1) = '\0';
}
