# an Makefile
V= 0.95
PORT= an
PROG= ${PORT}
SRCS= an.c gan_qsort.c
MAN6= an.0

CFLAGS+= -ffunction-sections -fdata-sections
LDFLAGS+= -Wl,--gc-sections -s

.include <bsd.port.mk>
