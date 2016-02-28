# 9cc Makefile
V=	20160222
PORT=	9cc

# Order is important!
SUBDIR= lib9 libbio libmach libregexp cc ld vc va vl iar mk iyacc

do-install:
	install -d ${DESTDIR}${BINDIR}
	install -c -m 555 ${.CURDIR}/vc/vc ${DESTDIR}${BINDIR}
	install -c -m 555 ${.CURDIR}/va/va ${DESTDIR}${BINDIR}
	install -c -m 555 ${.CURDIR}/vl/vl ${DESTDIR}${BINDIR}
	install -c -m 555 ${.CURDIR}/iar/iar ${DESTDIR}${BINDIR}
	install -c -m 555 ${.CURDIR}/mk/mk ${DESTDIR}${BINDIR}
	install -c -m 555 ${.CURDIR}/iyacc/iyacc ${DESTDIR}${BINDIR}

.include <bsd.port.mk>
