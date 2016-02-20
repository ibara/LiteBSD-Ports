# fpp Makefile
V= 1.1
PORT= fpp
PROG= ${PORT}
SRCS= fpp.c proc.c lex.c error.c rgram.c symtab.c service.c y.tab.c

CFLAGS+= -DRELEASE=\"v${V}\" -ffunction-sections -fdata-sections -ansi
LDFLAGS+= -Wl,--gc-sections -s

.include <bsd.port.mk>
