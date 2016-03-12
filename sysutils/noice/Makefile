# noice Makefile
V= 0.5
PORT= noice
PROG= ${PORT}

CFLAGS+= -ffunction-sections -fdata-sections
LDFLAGS+= -Wl,--gc-sections -s
LDADD+= -lncurses

.include <bsd.port.mk>
