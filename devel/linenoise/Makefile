# linenoise Makefile
V=	1.0
PORT=	linenoise
OBJS=	linenoise.o
CLEANFILES+= liblinenoise.a *.o

all: ${OBJS}
	${AR} ${ARFLAGS} liblinenoise.a ${OBJS}
	${RANLIB} liblinenoise.a

do-install:
	install -d ${DESTDIR}${PREFIX}/include
	install -d ${DESTDIR}${PREFIX}/lib
	install -c -m 444 linenoise.h ${DESTDIR}${PREFIX}/include
	install -c -m 444 liblinenoise.a ${DESTDIR}${PREFIX}/lib

.include <bsd.port.mk>
