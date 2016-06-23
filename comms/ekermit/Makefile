# ekermit Makefile
V= 1.7
PORT= ekermit
PROG= ${PORT}
SRCS= kermit.c main.c unixio.c

CFLAGS+= -ffunction-sections -fdata-sections
LDFLAGS+= -Wl,--gc-sections -s

.include <bsd.port.mk>
