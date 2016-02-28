OBJS=	regaux.o regcomp.o regerror.o regexec.o regsub.o rregexec.o rregsub.o
CLEANFILES+= libregexp.a

CFLAGS+= -I${.CURDIR} -I.. -I../lib9 -I../libbio

all: libregexp.a

libregexp.a: ${OBJS}
	${AR} ${ARFLAGS} $@ ${OBJS}
	${RANLIB} $@

.include <bsd.port.mk>
