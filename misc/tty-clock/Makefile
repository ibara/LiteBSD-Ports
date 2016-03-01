# tty-clock Makefile
V= 2
PORT= tty-clock
PROG= ${PORT}

CFLAGS+= -ffunction-sections -fdata-sections
LDFLAGS+= -Wl,--gc-sections -s
LDADD+= -lncurses

.include <bsd.port.mk>
