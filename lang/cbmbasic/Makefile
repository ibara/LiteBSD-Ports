# cbmbasic Makefile
V= 8.00
PORT= cbmbasic
PROG= ${PORT}
NOMAN=
SRCS= cbmbasic.c runtime.c plugin.c console.c

CFLAGS+= -ffunction-sections -fdata-sections
LDFLAGS+= -Wl,--gc-sections -s

beforeinstall:
	install -d ${DESTDIR}${PREFIX}/share/examples/${PORT}
	install -c -m 444 test/* ${DESTDIR}${PREFIX}/share/examples/${PORT}

.include <bsd.port.mk>
