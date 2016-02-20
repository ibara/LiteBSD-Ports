# blake Makefile
V= 20160220
PORT= blake2
PROG= b2sum
SRCS= b2sum.c blake2b-ref.c blake2s-ref.c blake2bp-ref.c blake2sp-ref.c 

CFLAGS+= -std=c99 -ffunction-sections -fdata-sections
LDFLAGS+= -Wl,--gc-sections -s

.include <bsd.port.mk>
