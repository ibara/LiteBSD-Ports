#
# cpat Makefile
#
V= 1.2.1
PORT= cpat
PROG= ${PORT}
SRCS= bakersdozen.c canfield.c common.c cpat.c document.c \
	duchess.c fortythieves.c freecell.c highscores.c \
	klondike.c penguin.c scorpion.c spider.c strategy.c \
	yukon.c
CFLAGS+=-DHAVE_CONFIG_H -ffunction-sections -fdata-sections
LDFLAGS+=-Wl,--gc-sections
LDADD+=-lncurses

.include <bsd.port.mk>
