# lsx Makefile
V= 0.1
PROG= lsx
PORT= ${PROG}

CFLAGS+= -DVERSION=\"${V}\" -ffunction-sections -fdata-sections
LDFLAGS+= -Wl,--gc-sections -s

.include <bsd.port.mk>
