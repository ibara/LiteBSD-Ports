# ce Makefile
V= 4.6
PORT= ce
PROG= ${PORT}
SRCS=	basic.c bind.c buffer.c bug.c cinfo.c cmode.c \
	complete.c dir.c display.c file.c fileio.c fio.c func.c \
	help.c key.c line.c main.c misc.c ml.c paragraph.c paren.c \
	random.c regexp.c region.c rsearch.c search.c spawn.c tty.c \
	ttyio.c vers.c version.c window.c word.c

HOST= mipsel-unknown-litebsd
DOCDIR=	${PREFIX}/lib/ce
CFLAGS+= -DHAVE_CONFIG_H -I${.CURDIR}
CFLAGS+= -DLIBDIR=\"$(DOCDIR)\" -DHOSTINFO=\"$(HOST)\"
CFLAGS+= -ffunction-sections -fdata-sections
LDFLAGS+= -Wl,--gc-sections -s
LDADD+=	-ltermcap

.include <bsd.port.mk>
