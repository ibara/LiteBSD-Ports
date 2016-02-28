OBJS=	argv0.o charstod.o cistrcmp.o cistrncmp.o cistrstr.o cleanname.o \
	convD2M.o convM2D.o convM2S.o convS2M.o create.o dirstat-posix.o \
	dirwstat.o dofmt.o dorfmt.o errfmt.o errstr-posix.o exits.o \
	fltfmt.o fmt.o fmtfd.o fmtlock.o fmtprint.o fmtquote.o fmtrune.o \
	fmtvprint.o fprint.o getfields.o getuser-posix.o getwd-posix.o \
	nulldir.o pow10.o print.o qsort.o readn.o rerrstr.o rune.o \
	runeseprint.o runesmprint.o runesnprint.o runestrlen.o runevseprint.o \
	sbrk-posix.o seek.o seprint.o smprint.o snprint.o sprint.o \
	strdup.o strecpy.o strtoll.o sysfatal.o tokenize.o u16.o u32.o \
	u64.o utfecpy.o utflen.o utfnlen.o utfrrune.o utfrune.o vfprint.o \
	vseprint.o vsmprint.o vsnprint.o
CLEANFILES+= lib9.a

CFLAGS+= -I${.CURDIR}

all: lib9.a

lib9.a: ${OBJS}
	${AR} ${ARFLAGS} $@ ${OBJS}
	${RANLIB} $@

.include <bsd.port.mk>
