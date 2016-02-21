# cmatrix Makefile
V= 1.2a
PORT= cmatrix
PROG= ${PORT}

CFLAGS+= -DHAVE_CONFIG_H -I${.CURDIR} -ffunction-sections -fdata-sections
LDFLAGS+= -Wl,--gc-sections -s
LDADD+= -lncurses

.include <bsd.port.mk>
