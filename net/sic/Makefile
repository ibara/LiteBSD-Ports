# sic - simple irc client
V = 1.3
PORT = sic
PROG = sic

# sic version
VERSION = 1.3-LiteBSD

# flags
CPPFLAGS = -DVERSION=\"${VERSION}\" -D_GNU_SOURCE
CFLAGS += -std=c99 ${CPPFLAGS} -ffunction-sections -fdata-sections
LDFLAGS += -Wl,--gc-sections

.include <bsd.port.mk>
