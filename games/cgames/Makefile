# cgames Makefile
V= 2.2a
PORT= cgames

SUBDIR=	cblocks cmines csokoban

do-install:
	install -d ${DESTDIR}${BINDIR}
	install -d ${DESTDIR}${MANDIR}6
	install -d ${DESTDIR}${PREFIX}/share/cgames/cblocks
	install -d ${DESTDIR}${PREFIX}/share/cgames/csokoban
	install -c -m 555 cblocks/cblocks ${DESTDIR}${BINDIR}
	install -c -m 555 cmines/cmines ${DESTDIR}${BINDIR}
	install -c -m 555 csokoban/csokoban ${DESTDIR}${BINDIR}
	install -c -m 444 cblocks/cblocks.0 ${DESTDIR}${MANDIR}6
	install -c -m 444 cmines/cmines.0 ${DESTDIR}${MANDIR}6
	install -c -m 444 csokoban/csokoban.0 ${DESTDIR}${MANDIR}6
	install -c -m 444 cblocks/series/* \
		${DESTDIR}${PREFIX}/share/cgames/cblocks
	install -c -m 444 csokoban/series/* \
		${DESTDIR}${PREFIX}/share/cgames/csokoban

.include <bsd.port.mk>
