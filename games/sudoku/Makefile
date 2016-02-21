# sudoku Makefile
V= 0.1
PORT= sudoku
PROG= ${PORT}
NOMAN=

CFLAGS+= -ffunction-sections -fdata-sections
LDFLAGS+= -Wl,--gc-sections -s
LDADD+= -lncurses

.include <bsd.port.mk>
