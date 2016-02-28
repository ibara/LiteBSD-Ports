OBJS=	elf.o mod.o pass.o pobj.o Posix.o sub.o
CLEANFILES+= libld.a

CFLAGS+= -I${.CURDIR} -I../lib9 -I../libbio

all: libld.a

libld.a: ${OBJS}
	${AR} ${ARFLAGS} $@ ${OBJS}
	${RANLIB} $@

.include <bsd.port.mk>
