OBJS=	asm.o enam.o list.o noop.o obj.o optab.o pass.o sched.o span.o
CLEANFILES+= kl

CFLAGS+= -I${.CURDIR} -I../lib9 -I../libbio -ffunction-sections -fdata-sections
LDFLAGS+= -Wl,--gc-sections -s

all: kl

kl: ${OBJS}
	${CC} ${LDFLAGS} -o $@ ${OBJS} ../ld/libld.a ../libbio/libbio.a ../lib9/lib9.a -lc -lgcc

.include <bsd.port.mk>
