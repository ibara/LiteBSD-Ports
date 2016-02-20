# ii Makefile
V= 1.7
PORT= ii
PROG= ${PORT}

CFLAGS+=-DVERSION=\"${V}\" -ffunction-sections -fdata-sections
LDFLAGS+=-Wl,--gc-sections -s

.include <bsd.port.mk>
