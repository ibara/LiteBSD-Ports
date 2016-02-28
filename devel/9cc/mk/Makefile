OBJS=	arc.o archive.o bufblock.o env.o file.o graph.o job.o lex.o main.o \
	match.o mk.o parse.o Posix.o recipe.o rule.o run.o sh.o shprint.o \
	symtab.o var.o varsub.o word.o

CLEANFILES+= mk

CFLAGS+= -I${.CURDIR} -I../lib9 -I../libbio -I../libregexp -ffunction-sections -fdata-sections
LDFLAGS+= -Wl,--gc-sections -s

all: mk

mk: ${OBJS}
	${CC} ${LDFLAGS} -o $@ ${OBJS} ../libregexp/libregexp.a ../libbio/libbio.a ../lib9/lib9.a -lc -lgcc

.include <bsd.port.mk>
