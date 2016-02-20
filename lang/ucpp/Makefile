# ucpp Makefile
V= 1.3
PORT= ucpp
PROG= ${PORT}
SRCS= assert.c cpp.c eval.c lexer.c macro.c mem.c nhash.c

CFLAGS+= -DUCPP_CONFIG -DSTAND_ALONE -ffunction-sections -fdata-sections
LDFLAGS+= -Wl,--gc-sections -s

.include <bsd.port.mk>
