V= 2014
PORT= ioccc
NOMAN=
CLEANFILES+= aidan anderson anon chia eldby hou hoyle konno natori sloane

CFLAGS+= -ffunction-sections -fdata-sections
LDFLAGS+= -Wl,--gc-sections -s

all: aidan anderson anon chia eldby hou hoyle konno natori sloane

aidan:
	${CC} ${CFLAGS} ${LDFLAGS} -o $@ $@.c -lc -lgcc

anderson:
	${CC} ${CFLAGS} ${LDFLAGS} -o $@ $@.c -lc -lgcc

anon:
	${CC} ${CFLAGS} ${LDFLAGS} -o $@ $@.c -lc -lgcc

chia:
	${CC} ${CFLAGS} ${LDFLAGS} -o $@ $@.c -lc -lgcc

eldby:
	${CC} ${CFLAGS} ${LDFLAGS} -o $@ $@.c -lc -lgcc

hou:
	${CC} ${CFLAGS} ${LDFLAGS} -o $@ $@.c -lm -lc -lgcc

hoyle:
	${CC} ${CFLAGS} ${LDFLAGS} -o $@ $@.c -lm -lc -lgcc

konno:
	${CC} ${CFLAGS} ${LDFLAGS} -o $@ $@.c -lc -lgcc

natori:
	${CC} ${CFLAGS} ${LDFLAGS} -o $@ $@.c -lm -lc -lgcc

sloane:
	${CC} ${CFLAGS} ${LDFLAGS} -o $@ $@.c -lm -lc -lgcc

do-install:
	install -d ${DESTDIR}${BINDIR}
	install -c -m 555 aidan anderson anon chia eldby \
		hou hoyle konno natori sloane ${DESTDIR}${BINDIR}

.include <bsd.port.mk>
