# atto Makefile
V= 1.6
PORT= atto
PROG= ${PORT}
NOMAN=
SRCS= command.c data.c display.c gap.c key.c search.c buffer.c replace.c window.c main.c

CFLAGS+= -ffunction-sections -fdata-sections
LDFLAGS+= -Wl,--gc-sections -s
LDADD+= -lcurses -ltermcap

.include <bsd.port.mk>
