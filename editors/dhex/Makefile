# dhex Makefile
V= 0.68
PORT= dhex
PROG= ${PORT}
SRCS=	buffers.c configfile.c correlation.c gpl.c hexcalc.c \
	input.c machine_type.c main.c markers.c menu.c output.c \
	search.c ui.c
MAN1= dhex.0
MAN5= dhexrc.0 dhex_markers.0 dhex_searchlog.0

CFLAGS+= -std=c99 -ffunction-sections -fdata-sections
LDFLAGS+= -Wl,--gc-sections -s
LDADD+= -lncurses

.include <bsd.port.mk>
