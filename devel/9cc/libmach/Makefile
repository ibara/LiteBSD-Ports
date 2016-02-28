OBJS=	access.o executable.o machdata.o map.o obj.o setmach.o swap.o \
	sym.o v.o vcodas.o vdb.o vobj.o
CLEANFILES+= libmach.a

CFLAGS+= -I${.CURDIR} -I.. -I../lib9 -I../libbio

all: libmach.a

libmach.a: ${OBJS}
	${AR} ${ARFLAGS} $@ ${OBJS}
	${RANLIB} $@

.include <bsd.port.mk>
