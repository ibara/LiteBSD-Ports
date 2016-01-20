#
# heme Makefile
#
V= 0.4.2
PORT= heme
PROG= ${PORT}
SRCS= heme.c xmalloc.c error.c pconfig.c pgetopt.c

CFLAGS+= -DHAVE_MMAP -DPAGESIZE=4096 -ffunction-sections -fdata-sections
LDFLAGS+=-Wl,--gc-sections
LDADD+= -lncurses

.include <bsd.port.mk>
