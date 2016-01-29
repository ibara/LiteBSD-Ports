#
# nano Makefile
#
V= 2.5.1
PORT= nano
PROG= ${PORT}
MAN1= nano.0
MAN5= nanorc.0

SRCS= browser.c chars.c color.c cut.c files.c global.c help.c move.c nano.c \
	prompt.c rcfile.c search.c text.c utils.c winio.c dirname.c basename.c

CFLAGS+= -I${.CURDIR} -DHAVE_CONFIG_H -DLOCALEDIR=\"/usr/local/share/locale\" -DSYSCONFDIR=\"/usr/local/etc\" -ffunction-sections -fdata-sections

LDFLAGS+= -Wl,--gc-sections

DPADD= ${LIBNCURSES}
LDADD+= -lncurses

.include <bsd.port.mk>
