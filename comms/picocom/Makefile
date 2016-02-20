# picocom Makefile
V= 2.2a
PORT= picocom
PROG= ${PORT}
SRCS= picocom.c term.c fdio.c split.c

CFLAGS+= -DVERSION_STR=\"$(V)\" -DTTY_Q_SZ=32768 -DHIGH_BAUD -DUSE_FLOCK -ffunction-sections -fdata-sections
LDFLAGS+= -Wl,--gc-sections -s

.include <bsd.port.mk>
