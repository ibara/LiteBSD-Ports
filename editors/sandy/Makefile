# sandy Makefile
V= 0.6
PORT= sandy
PROG= ${PORT}

CFLAGS+= -std=c99 -I. -DVERSION=\"${V}\" -D_BSD_SOURCE -D_POSIX_C_SOURCE=200809L -ffunction-sections -fdata-sections
LDFLAGS+= -Wl,--gc-sections
LDADD+= -lncurses

.include <bsd.port.mk>
