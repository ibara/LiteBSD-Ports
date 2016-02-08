# sre Makefile
V= 8
PORT= sre
PROG= ${PORT}
NOMAN=
SRCS= tag.c cmd.c file.c main.c misc.c init.c render.c setup.c srch.c

CFLAGS+= -ffunction-sections -fdata-sections
LDFLAGS+= -Wl,--gc-sections

.include <bsd.port.mk>
