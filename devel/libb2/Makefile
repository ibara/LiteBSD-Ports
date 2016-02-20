# libb2 Makefile
V= 1.2.0
PORT= libb2
OBJS= blake2b-ref.o blake2s-ref.o
CLEANFILES+= libb2.a *.o

CFLAGS+= -std=c99 -DNATIVE_LITTLE_ENDIAN -DHAVE_ALIGNED_ACCESS_REQUIRED

all: ${OBJS}
	${AR} ${ARFLAGS} libb2.a ${OBJS}
	${RANLIB} libb2.a

do-install: all
	install -d ${DESTDIR}${PREFIX}/lib
	install -c -m 444 libb2.a ${DESTDIR}${PREFIX}/lib

.include <bsd.port.mk>
