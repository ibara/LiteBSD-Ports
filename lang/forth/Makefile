# Serge forth Makefile
V= 1.4
PORT= forth
PROG= ${PORT}
NOMAN=
SRCS= forth.c func.c

CFLAGS+= -ffunction-sections -fdata-sections
LDFLAGS+= -Wl,--gc-sections
LDADD+= -lm

.include <bsd.port.mk>
