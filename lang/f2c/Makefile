# f2c Makefile
V= 20100827
PORT= f2c
CLEANFILES+= f2c.0

SUBDIR= f2c libF77

do-install:
	nroff -man f2c.1t > f2c.0
	install -d ${DESTDIR}${BINDIR}
	install -d ${DESTDIR}${MANDIR}1
	install -d ${DESTDIR}${PREFIX}/include
	install -d ${DESTDIR}${PREFIX}/lib
	install -c -m 555 fc ${DESTDIR}${BINDIR}/f77
	install -c -m 555 f2c/f2c ${DESTDIR}${BINDIR}
	install -c -m 444 f2c.0 ${DESTDIR}${MANDIR}1
	install -c -m 444 libF77/f2c.h ${DESTDIR}${PREFIX}/include
	install -c -m 444 libF77/libF77.a ${DESTDIR}${PREFIX}/lib

.include <bsd.port.mk>
