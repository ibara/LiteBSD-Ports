#	$OpenBSD: Makefile,v 1.2 1999/08/19 22:17:38 niklas Exp $
V=	20151221
PORT=	arch
PROG=	${PORT}
LINKS=	${BINDIR}/arch ${BINDIR}/machine
MLINKS=	arch.1 machine.1

CFLAGS+=-ffunction-sections -fdata-sections
LDFLAGS+=-Wl,--gc-sections -s

.include <bsd.port.mk>
