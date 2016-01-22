#
# ee Makefile
#
V= 1.5.2
PORT= ee
PROG= ${PORT}

CFLAGS+= -DHAS_UNISTD -DHAS_STDLIB -DHAS_CTYPE -DHAS_SYS_IOCTL -DHAS_SYS_WAIT -DSLCT_HDR -DNO_CATGETS -ffunction-sections -fdata-sections

LDFLAGS+=-Wl,--gc-sections

DPADD= ${LIBNCURSES}
LDADD+=-lncurses

.include <bsd.port.mk>
