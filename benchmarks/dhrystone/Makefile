#
# dhry Makefile
#

V=	2.1
PORT=	dhrystone
PROG=	dhry
NOMAN=	Yes

SRCS=	dhry_1.c dhry_2.c

CFLAGS+=-DTIME -ffunction-sections -fdata-sections
LDFLAGS+=-Wl,--gc-sections

.include <bsd.port.mk>
