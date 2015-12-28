#
# whetstone Makefile
#

V=	1.2
PORT=	whetstone
PROG=	whetstone
NOMAN=	Yes

CFLAGS+=-DPRINTOUT -ffunction-sections -fdata-sections
LDFLAGS+=-Wl,--gc-sections

DPADD=	${LIBM}
LDADD=	-lm

.include <bsd.port.mk>
