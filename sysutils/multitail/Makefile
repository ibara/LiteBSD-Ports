# multitail Makefile
V= 6.4.2
PORT= multitail
PROG= ${PORT}
SRCS= utils.c mt.c error.c my_pty.c term.c scrollback.c help.c mem.c cv.c selbox.c stripstring.c color.c misc.c ui.c exec.c diff.c config.c cmdline.c globals.c history.c xclip.c

CFLAGS+= -funsigned-char -ffunction-sections -fdata-sections -DLiteBSD -DVERSION=\"$(V)\" -DCONFIG_FILE=\"$(CONFIG_FILE)\" -D_FORTIFY_SOURCE=2
LDFLAGS+= -Wl,--gc-sections -s
LDADD+= -lpanel -lncurses -lutil -lm

CONFIG_FILE=$(DESTDIR)$(PREFIX)/etc/multitail.conf

beforeinstall:
	install -d ${DESTDIR}${PREFIX}/share/doc/multitail
	cp *.txt INSTALL manual*.html $(DESTDIR)$(PREFIX)/share/doc/multitail
	install -d $(DESTDIR)${PREFIX}/etc
	install -c -m 444 multitail.conf $(CONFIG_FILE)

.include <bsd.port.mk>
