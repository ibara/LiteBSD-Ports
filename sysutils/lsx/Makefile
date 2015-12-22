#
# lsx Makefile
#

V=	0.1
PROG=	lsx
SRCS=	lsx.c
CFLAGS+=-DVERSION=\"${V}\" -ffunction-sections -fdata-sections
LDFLAGS+=-Wl,--gc-sections

.include <bsd.port.mk>
