#
# tinyscheme Makefile
#
V= 1.41
PORT= tinyscheme
PROG= ${PORT}
NOMAN= noman

SRCS= scheme.c

CFLAGS+= -std=gnu99 -pedantic -ffunction-sections -fdata-sections -DSTANDALONE=1 -DUSE_DL=0 -DUSE_MATH=1 -DUSE_ASCII_NAMES=0 -DUSE_STRCASECMP=1 -DInitFile="\"${PREFIX}/share/tinyscheme/init.scm\""

LDFLAGS+= -Wl,--gc-sections

DPADD= ${LIBM}
LDADD+= -lm

beforeinstall:
	install -d ${DESTDIR}${PREFIX}/share/tinyscheme
	install -c -m 444 init.scm ${DESTDIR}${PREFIX}/share/tinyscheme

.include <bsd.port.mk>
