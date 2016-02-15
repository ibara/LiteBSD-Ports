# phoon Makefile
V= 20140814
PORT= phoon
PROG= ${PORT}
SRCS= astro.c date_parse.c phoon.c

CFLAGS+= -DOS_BSD -ffunction-sections -fdata-sections
LDFLAGS+= -Wl,--gc-sections -s
LDADD+= -lm

.include <bsd.port.mk>
