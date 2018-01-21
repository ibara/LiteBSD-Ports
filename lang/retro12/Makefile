# retro Makefile
V= 2018.1
PORT= retro12
PROG= ${PORT}
SRCS= retro12.c

CFLAGS+= -ffunction-sections -fdata-sections
LDFLAGS+= -Wl,--gc-sections -s
LDADD+=

beforeinstall:
	install -d ${DESTDIR}${PREFIX}/share/retro12
	install -c -m 444 ngaImage ${DESTDIR}${PREFIX}/share/retro12

.include <bsd.port.mk>
