OBJS=	ar.o Posix.o
CLEANFILES+= iar

CFLAGS+= -I${.CURDIR} -I../lib9 -I../libbio -ffunction-sections -fdata-sections
LDFLAGS+= -Wl,--gc-sections -s

all: iar

iar: ${OBJS}
	${CC} ${LDFLAGS} -o $@ ${OBJS} ../libmach/libmach.a ../libbio/libbio.a ../lib9/lib9.a -lc -lgcc

.include <bsd.port.mk>
