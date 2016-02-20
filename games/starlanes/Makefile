# starlanes Makefile
V= 1.2.2
PORT= starlanes
PROG= ${PORT}
MAN6= ${PORT}.0

CFLAGS+= -ffunction-sections -fdata-sections
LDFLAGS+= -Wl,--gc-sections -s
LDADD+= -lncurses

beforeinstall:
	install -d ${DESTDIR}${PREFIX}/share/doc/starlanes
	install -c -m 444 COPYING README ${DESTDIR}${PREFIX}/share/doc/starlanes

.include <bsd.port.mk>
