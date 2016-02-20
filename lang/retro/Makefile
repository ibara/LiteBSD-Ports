# retro Makefile
V= 11.2
PORT= retro
PROG= ${PORT}

CFLAGS+= -ffunction-sections -fdata-sections
LDFLAGS+= -Wl,--gc-sections -s
LDADD+= -ltermcap

beforeinstall:
	install -d ${DESTDIR}${PREFIX}/share/examples/retro
	install -d ${DESTDIR}${PREFIX}/share/retro
	cp -R examples/ ${DESTDIR}${PREFIX}/share/examples/retro
	install -c -m 444 retroImage ${DESTDIR}${PREFIX}/share/retro

.include <bsd.port.mk>
