OBJS=	yacc.o
CLEANFILES+= iyacc

CFLAGS+= -DROOT=\"$HOME\" -DPARSER=\"yaccpar\" -DPARSERS=\"yaccpar\" -I../lib9 -I../libbio -ffunction-sections -fdata-sections
LDFLAGS+= -Wl,--gc-sections -s

all: iyacc

iyacc: ${OBJS}
	${CC} ${LDFLAGS} -o $@ ${OBJS} ../libbio/libbio.a ../lib9/lib9.a -lc -lgcc

.include <bsd.port.mk>
