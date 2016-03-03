# business-card-brainfuck Makefile
V= 20151203
PORT= business-card-brainfuck
PROG= bf
NOMAN=

CFLAGS+= -std=c99 -pedantic -ffunction-sections -fdata-sections
LDFLAGS+= -Wl,--gc-sections -s

beforeinstall:
	install -d ${DESTDIR}${PREFIX}/share/examples/business-card-brainfuck
	install -c -m 444 bottles.bf hw.bf mandelbrot.bf \
		${DESTDIR}${PREFIX}/share/examples/business-card-brainfuck

.include <bsd.port.mk>
