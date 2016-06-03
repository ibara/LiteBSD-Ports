# $NetBSD: Makefile,v 1.2 2011/08/18 11:29:27 christos Exp $
V= 1.22
PORT= unzip
PROG= ${PORT}
SRCS= unzip.c getline.c lchmod.c

CFLAGS+= -std=c99 -ffunction-sections -fdata-sections
LDFLAGS+= -Wl,--gc-sections -s
LDADD+=	-larchive -lz

.include <bsd.port.mk>
