# curseofwar Makefile
V=	1.2.1
PORT=	curseofwar
PROG=	${PORT}
SRCS=	grid.c state.c king.c network.c client.c server.c output-common.c \
	path.c main-common.c output.c main.c
MAN6=	curseofwar.0

CFLAGS+= -DVERSION=\"${V}\" -ffunction-sections -fdata-sections
LDFLAGS+= -Wl,--gc-sections -s
LDADD+= -lm -lncurses

.include <bsd.port.mk>
