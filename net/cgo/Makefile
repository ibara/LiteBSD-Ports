# see cgo.c for copyright and license details
V= 0.4.1
PORT= cgo
PROG= ${PORT}

CFLAGS+=-ffunction-sections -fdata-sections
LDFLAGS+=-Wl,--gc-sections

beforeinstall:
	install -d ${DESTDIR}${PREFIX}/etc
	install -c -m 444 cgorc ${DESTDIR}${PREFIX}/etc

.include <bsd.port.mk>
