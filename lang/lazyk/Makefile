# lazyk Makefile
V= 20160301
PORT= lazyk
PROG= ${PORT}
NOMAN=

CFLAGS+= -ffunction-sections -fdata-sections
LDFLAGS+= -Wl,--gc-sections -s

beforeinstall:
	install -d ${DESTDIR}${PREFIX}/share/examples/lazyk
	install -c -m 444 calc.lazy fib.lazy primes.lazy \
		rot13.lazy unlambda.lazy \
		${DESTDIR}${PREFIX}/share/examples/lazyk

.include <bsd.port.mk>
