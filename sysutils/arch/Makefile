#	$OpenBSD: Makefile,v 1.2 1999/08/19 22:17:38 niklas Exp $

V=	20151221			# Must have a value!
PROG=	arch
LINKS=	${BINDIR}/arch ${BINDIR}/machine
MLINKS=	arch.1 machine.1

CFLAGS+=-ffunction-sections -fdata-sections
LDFLAGS+=-Wl,--gc-sections

.include <bsd.port.mk>
