# yoctolisp Makefile
V= 20150730
PORT= yoctolisp
PROG= ${PORT}
NOMAN=

CFLAGS+= -ffunction-sections -fdata-sections
LDFLAGS+= -Wl,--gc-sections -s

beforeinstall:
	install -d ${DESTDIR}${PREFIX}/share/yoctolisp
	install -d ${DESTDIR}${PREFIX}/share/examples/yoctolisp
	install -c -m 444 stdlib.l ${DESTDIR}${PREFIX}/share/yoctolisp
	install -c -m 444 examples/closure.l examples/hello.l examples/repl.l \
		${DESTDIR}${PREFIX}/share/examples/yoctolisp

.include <bsd.port.mk>
