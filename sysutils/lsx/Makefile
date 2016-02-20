# lsx Makefile
V= 0.1
PROG= lsx
PORT= ${PORT}

CFLAGS+= -DVERSION=\"${V}\" -ffunction-sections -fdata-sections
LDFLAGS+= -Wl,--gc-sections -s

.include <bsd.port.mk>
