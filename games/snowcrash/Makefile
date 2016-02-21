# snowcrash Makefile
V= 20131201
PORT= snowcrash
PROG= ${PORT}
NOMAN=

CFLAGS+= -ffunction-sections -fdata-sections
LDFLAGS+= -Wl,--gc-sections -s
LDADD+= -lncurses

.include <bsd.port.mk>
