# sc Makefile
V= 7.16
PORT= sc
OBJS= 	abbrev.o cmds.o color.o format.o frame.o gram.o help.o interp.o \
	lex.o pipe.o range.o screen.o sort.o vi.o vmtbl.o xmalloc.o
POBJS=	psc.o 
SOBJS=	sc.o
CLEANFILES+= psc.o sc.o psc sc psc.0 sc.0

CFLAGS+= -std=c99 -ffloat-store -ffunction-sections -fdata-sections
LDFLAGS+= -Wl,--gc-sections -s
LIBS= -lm -lncurses -lc -lgcc

all: sc psc

sc: ${SOBJS} ${OBJS}
	${CC} ${LDFLAGS} -o $@ ${SOBJS} ${OBJS} ${LIBS}
	nroff -man sc.1 > sc.0

psc: ${POBJS} ${OBJS}
	${CC} ${LDFLAGS} -o $@ ${POBJS} vmtbl.o xmalloc.o ${LIBS}
	nroff -man psc.1 > psc.0

do-install:
	install -d ${DESTDIR}${BINDIR}
	install -d ${DESTDIR}${MANDIR}1
	install -d ${DESTDIR}${PREFIX}/share/sc
	install -c -m 555 sc psc ${DESTDIR}${BINDIR}
	install -c -m 444 sc.0 psc.0 ${DESTDIR}${MANDIR}1
	install -c -m 444 tutorial.sc ${DESTDIR}${PREFIX}/share/sc

.include <bsd.port.mk>
