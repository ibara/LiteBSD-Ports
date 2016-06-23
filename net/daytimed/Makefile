# daytimed Makefile
V= 20160704
PORT= daytimed
PROG= ${PORT}
SBININSTALL= Yes
MAN8= ${PORT}.0

CFLAGS+= -ffunction-sections -fdata-sections
LDFLAGS+= -Wl,--gc-sections -s

.include <bsd.port.mk>
