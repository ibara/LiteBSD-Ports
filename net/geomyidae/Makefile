# geomyidae Makefile
V= 0.26
PORT= geomyidae
OBJS= main.o ind.o handlr.o
MAN8= geomyidae.0
CLEANFILES+= geomyidae *.o *.0

CFLAGS+= -ffunction-sections -fdata-sections
LDFLAGS+= -Wl,--gc-sections -s

all: ${OBJS}
	${CC} ${LDFLAGS} -o geomyidae ${OBJS} -lc -lgcc
	nroff -man geomyidae.8 > geomyidae.0

do-install:
	install -d ${DESTDIR}${PREFIX}/sbin
	install -d ${DESTDIR}${MANDIR}8
	install -c -m 555 geomyidae ${DESTDIR}${PREFIX}/sbin
	install -c -m 444 geomyidae.0 ${DESTDIR}${MANDIR}8

.include <bsd.port.mk>
