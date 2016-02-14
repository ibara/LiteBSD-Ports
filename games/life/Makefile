# life Makefile
V= 20091116
PORT= life
PROG= ${PORT}
NOMAN=

CFLAGS+= -ffunction-sections -fdata-sections
LDFLAGS+= -Wl,--gc-sections -s
LDADD+= -lncurses

beforeinstall:
	install -d ${DESTDIR}${PREFIX}/share/doc/life
	install -c -m 444 README ${DESTDIR}${PREFIX}/share/doc/life

.include <bsd.port.mk>
