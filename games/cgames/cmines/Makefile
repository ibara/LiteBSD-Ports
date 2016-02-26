OBJS= cmines.o userio.o
CLEANFILES+= cmines *.0

CFLAGS+= -DNOMOUSE -ffunction-sections -fdata-sections
LDFLAGS+= -Wl,--gc-sections -s
LDADD+= -lncurses

all: ${OBJS}
	${CC} ${LDFLAGS} -o cmines ${OBJS} ${LDADD} -lc -lgcc
	nroff -man cmines.6 > cmines.0

.include <bsd.port.mk>
