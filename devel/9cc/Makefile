# 9cc Makefile
V=	20160302
PORT=	9cc

# Order is important!
SUBDIR= lib9 libbio libmach libregexp cc ld 0a 0c 0l 2a 2c 2l 5a 5c 5l \
	6a 6c 6l 8a 8c 8l iar iyacc ka kc kl mk qa qc ql va vc vl

do-install:
	install -d ${DESTDIR}${BINDIR}
	install -c -m 555 ${.CURDIR}/0a/0a ${DESTDIR}${BINDIR}
	install -c -m 555 ${.CURDIR}/0c/0c ${DESTDIR}${BINDIR}
	install -c -m 555 ${.CURDIR}/0l/0l ${DESTDIR}${BINDIR}
	install -c -m 555 ${.CURDIR}/2a/2a ${DESTDIR}${BINDIR}
	install -c -m 555 ${.CURDIR}/2c/2c ${DESTDIR}${BINDIR}
	install -c -m 555 ${.CURDIR}/2l/2l ${DESTDIR}${BINDIR}
	install -c -m 555 ${.CURDIR}/5a/5a ${DESTDIR}${BINDIR}
	install -c -m 555 ${.CURDIR}/5c/5c ${DESTDIR}${BINDIR}
	install -c -m 555 ${.CURDIR}/5l/5l ${DESTDIR}${BINDIR}
	install -c -m 555 ${.CURDIR}/6a/6a ${DESTDIR}${BINDIR}
	install -c -m 555 ${.CURDIR}/6c/6c ${DESTDIR}${BINDIR}
	install -c -m 555 ${.CURDIR}/6l/6l ${DESTDIR}${BINDIR}
	install -c -m 555 ${.CURDIR}/8a/8a ${DESTDIR}${BINDIR}
	install -c -m 555 ${.CURDIR}/8c/8c ${DESTDIR}${BINDIR}
	install -c -m 555 ${.CURDIR}/8l/8l ${DESTDIR}${BINDIR}
	install -c -m 555 ${.CURDIR}/iar/iar ${DESTDIR}${BINDIR}
	install -c -m 555 ${.CURDIR}/iyacc/iyacc ${DESTDIR}${BINDIR}
	install -c -m 555 ${.CURDIR}/ka/ka ${DESTDIR}${BINDIR}
	install -c -m 555 ${.CURDIR}/kc/kc ${DESTDIR}${BINDIR}
	install -c -m 555 ${.CURDIR}/kl/kl ${DESTDIR}${BINDIR}
	install -c -m 555 ${.CURDIR}/mk/mk ${DESTDIR}${BINDIR}
	install -c -m 555 ${.CURDIR}/qa/qa ${DESTDIR}${BINDIR}
	install -c -m 555 ${.CURDIR}/qc/qc ${DESTDIR}${BINDIR}
	install -c -m 555 ${.CURDIR}/ql/ql ${DESTDIR}${BINDIR}
	install -c -m 555 ${.CURDIR}/va/va ${DESTDIR}${BINDIR}
	install -c -m 555 ${.CURDIR}/vc/vc ${DESTDIR}${BINDIR}
	install -c -m 555 ${.CURDIR}/vl/vl ${DESTDIR}${BINDIR}

.include <bsd.port.mk>
