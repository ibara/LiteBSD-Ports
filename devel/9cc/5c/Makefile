OBJS=	cgen.o enam.o list.o mul.o peep.o reg.o sgen.o swt.o txt.o
CLEANFILES+= 5c

CFLAGS+= -I${.CURDIR} -I../lib9 -I../libbio -ffunction-sections -fdata-sections
LDFLAGS+= -Wl,--gc-sections -s

all: 5c

5c: ${OBJS}
	${CC} ${LDFLAGS} -o $@ ${OBJS} ../cc/libcc.a ../libbio/libbio.a ../lib9/lib9.a -lc -lgcc

.include <bsd.port.mk>
