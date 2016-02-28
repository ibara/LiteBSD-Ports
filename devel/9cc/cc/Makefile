OBJS=	acid.o bits.o cc.o com64.o com.o dcl.o dpchk.o funct.o lex.o mac.o \
	mpatof.o omachcap.o pgen.o pickle.o Posix.o pswt.o scon.o sub.o
CLEANFILES+= libcc.a *.o

CFLAGS+= -I${.CURDIR} -I../lib9 -I../libbio -I../vc

all: libcc.a

libcc.a: ${OBJS}
	${AR} ${ARFLAGS} $@ ${OBJS}
	${RANLIB} $@

.include <bsd.port.mk>
