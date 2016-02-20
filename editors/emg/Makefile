# emg Makefile
V= 2.0
PORT= emg
PROG= ${PORT}
SRCS=	basic.c buffer.c display.c file.c fileio.c line.c main.c \
	random.c region.c search.c tcap.c ttyio.c word.c

CFLAGS+= -ffunction-sections -fdata-sections
LDFLAGS+= -Wl,--gc-sections -s
LDADD+= -ltermcap

beforeinstall:
	install -d ${DESTDIR}${PREFIX}/share/doc/emg
	install -c -m 444 emg.keys ${DESTDIR}${PREFIX}/share/doc/emg

.include <bsd.port.mk>
