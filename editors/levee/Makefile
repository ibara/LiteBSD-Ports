# levee Makefile
V= 3.5b
PORT= levee
PROG= ${PORT}
SRCS=	blockio.c display.c editcor.c exec.c find.c \
	unixcall.c globals.c insert.c main.c misc.c \
	modify.c move.c ucsd.c undo.c wildargs.c \
	version.c

CFLAGS+= -ffunction-sections -fdata-sections
LDFLAGS+= -Wl,--gc-sections -s
LDADD+= -ltermcap

.include <bsd.port.mk>
