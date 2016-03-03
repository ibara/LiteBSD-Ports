OBJS=	a.o lex.o
CLEANFILES+= 5a

CFLAGS+= -I${.CURDIR} -I../lib9 -I../libbio -ffunction-sections -fdata-sections
LDFLAGS+= -Wl,--gc-sections -s

all: 5a

5a: ${OBJS}
	${CC} ${LDFLAGS} -o $@ ${OBJS} ../cc/libcc.a ../libbio/libbio.a ../lib9/lib9.a -lc -lgcc

.include <bsd.port.mk>
