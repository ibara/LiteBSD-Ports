OBJS=	cgen.o div.o enam.o list.o machcap.o \
	mul.o peep.o reg.o sgen.o swt.o txt.o
CLEANFILES+= 6c

CFLAGS+= -I${.CURDIR} -I../lib9 -I../libbio -ffunction-sections -fdata-sections
LDFLAGS+= -Wl,--gc-sections -s

all: 6c

6c: ${OBJS}
	${CC} ${LDFLAGS} -o $@ ${OBJS} ../cc/libcc.a ../libbio/libbio.a ../lib9/lib9.a -lc -lgcc

.include <bsd.port.mk>
