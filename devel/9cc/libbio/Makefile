OBJS=	bbuffered.o bfildes.o bflush.o bgetc.o bgetd.o bgetrune.o binit.o \
	boffset.o bprint.o bputc.o bputrune.o brdline.o brdstr.o bread.o \
	bseek.o bvprint.o bwrite.o
CLEANFILES+= libbio.a
CFLAGS+= -I${.CURDIR} -I../lib9

all: libbio.a

libbio.a: ${OBJS}
	${AR} ${ARFLAGS} $@ ${OBJS}
	${RANLIB} $@

.include <bsd.port.mk>
