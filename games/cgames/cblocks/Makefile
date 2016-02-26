OBJS= cblocks.o movelist.o parse.o fileread.o answers.o play.o dirio.o userio.o
CLEANFILES+= cblocks *.0

CFLAGS+= -DNOMOUSE -DDATADIR=\"/usr/local/share/cgames/cblocks\" -ffunction-sections -fdata-sections
LDFLAGS+= -Wl,--gc-sections -s
LDADD+= -lncurses

all: ${OBJS}
	${CC} ${LDFLAGS} -o cblocks ${OBJS} ${LDADD} -lc -lgcc
	nroff -man cblocks.6 > cblocks.0

.include <bsd.port.mk>
