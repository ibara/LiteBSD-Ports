# tradcpp Makefile
V=	0.4
PORT=	tradcpp
PROG=	${PORT}
SRCS=	main.c files.c directive.c eval.c macro.c output.c \
	place.c array.c utils.c

CFLAGS+= -ffunction-sections -fdata-sections
LDFLAGS+= -Wl,--gc-sections -s

.include <bsd.port.mk>
