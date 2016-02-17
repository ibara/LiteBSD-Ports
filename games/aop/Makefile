# aop Makefile
V= 0.6
PORT= aop
PROG= ${PORT}
NOMAN=

CFLAGS+= -ffunction-sections -fdata-sections
LDFLAGS+= -Wl,--gc-sections -s
LDADD+= -lncurses

beforeinstall:
	install -d ${DESTDIR}${PREFIX}/share/${PORT}
	install -c -m 444 aop-level-*.txt ${DESTDIR}${PREFIX}/share/${PORT}
	install -c -m 444 empty-level.txt ${DESTDIR}${PREFIX}/share/${PORT}

.include <bsd.port.mk>
