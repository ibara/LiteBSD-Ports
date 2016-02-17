# gkermit Makefile
V= 1.00
PORT= gkermit
PROG= ${PORT}
SRCS= gcmdline.c gkermit.c gproto.c gunixio.c

CFLAGS+= -DPOSIX -ffunction-sections -fdata-sections
LDFLAGS+= -Wl,--gc-sections -s

.include <bsd.port.mk>
