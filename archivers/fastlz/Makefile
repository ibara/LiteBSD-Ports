# fastlz Makefile
V= 0.1.0
PORT= fastlz
CLEANFILES+= 6pack 6unpack *.o

CFLAGS+= -ffunction-sections -fdata-sections
LDFLAGS+= -Wl,--gc-sections -s

all: 6pack 6unpack

6pack: fastlz.o 6pack.o
	${CC} ${LDFLAGS} -o $@ 6pack.o fastlz.o -lc -lgcc

6unpack: fastlz.o 6unpack.o
	${CC} ${LDFLAGS} -o $@ 6unpack.o fastlz.o -lc -lgcc

do-install:
	install -d ${DESTDIR}${BINDIR}
	install -c -m 555 6pack 6unpack ${DESTDIR}${BINDIR}

.include <bsd.port.mk>
