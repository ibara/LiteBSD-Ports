# whetstone Makefile
V= 1.2
PORT= whetstone
PROG= ${PORT}
NOMAN=

CFLAGS+= -DPRINTOUT -ffunction-sections -fdata-sections
LDFLAGS+= -Wl,--gc-sections -s
LDADD+=	-lm

.include <bsd.port.mk>
