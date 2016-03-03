OBJS=	asm.o enam.o list.o obj.o optab.o pass.o span.o
CLEANFILES+= 8l

CFLAGS+= -I${.CURDIR} -I../lib9 -I../libbio -ffunction-sections -fdata-sections
LDFLAGS+= -Wl,--gc-sections -s

all: 8l

8l: ${OBJS}
	${CC} ${LDFLAGS} -o $@ ${OBJS} ../ld/libld.a ../libbio/libbio.a ../lib9/lib9.a -lc -lgcc

.include <bsd.port.mk>
