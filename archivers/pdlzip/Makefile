# pdlzip Makefile
V= 1.7
PORT= pdlzip
PROG= ${PORT}
SRCS= carg_parser.c LzFind.c LzmaEnc.c LzmaDec.c main.c

CFLAGS+= -DPROGVERSION=\"${V}\" -ffunction-sections -fdata-sections
LDFLAGS+= -Wl,--gc-sections -s

.include <bsd.port.mk>
