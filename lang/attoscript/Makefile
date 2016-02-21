# attoscript Makefile
V= 0.1
PORT= attoscript
PROG= ${PORT}
NOMAN=

CFLAGS+= -ffunction-sections -fdata-sections
LDFLAGS+= -Wl,--gc-sections -s

beforeinstall:
	install -d ${DESTDIR}${PREFIX}/share/doc/attoscript
	install -c -m 444 README ${DESTDIR}${PREFIX}/share/doc/attoscript

.include <bsd.port.mk>
