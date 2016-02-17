# dos2unix Makefile
V= 7.3.3
PORT= dos2unix
CLEANFILES+= dos2unix unix2dos mac2unix unix2mac *.o *.0

COBJS= common.o querycp.o
DOBJS= dos2unix.o
UOBJS= unix2dos.o

CFLAGS+= -DVER_REVISION=\"${V}\" -DVER_DATE=\"2016-02-13\" -DVER_AUTHOR=\"'Erwin Waterlander'\" -DDEBUG=0 -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64 -DLOCALEDIR=\"/usr/local/share/locale\" -DPACKAGE=\"dos2unix\" -ffunction-sections -fdata-sections
LDFLAGS+= -Wl,--gc-sections -s

all: dos2unix unix2dos

dos2unix: ${COBJS} ${DOBJS}
	${CC} ${LDFLAGS} -o $@ ${COBJS} ${DOBJS} -lc -lgcc
	ln -sf $@ mac2unix
	nroff -man dos2unix.1 > dos2unix.0

unix2dos: ${COBJS} ${UOBJS}
	${CC} ${LDFLAGS} -o $@ ${COBJS} ${UOBJS} -lc -lgcc
	ln -sf $@ unix2mac

do-install:
	install -d ${DESTDIR}${BINDIR}
	install -d ${DESTDIR}${MANDIR}1
	install -c -m 555 dos2unix unix2dos mac2unix unix2mac ${DESTDIR}${BINDIR}
	install -c -m 444 dos2unix.0 ${DESTDIR}${MANDIR}1

.include <bsd.port.mk>
