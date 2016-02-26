OBJS= csokoban.o movelist.o fileread.o answers.o play.o dirio.o userio.o
CLEANFILES+= csokoban *.0

CFLAGS+= -DDATADIR=\"/usr/local/share/cgames/csokoban\" -ffunction-sections -fdata-sections
LDFLAGS+= -Wl,--gc-sections -s
LDADD+= -lncurses

all: ${OBJS}
	${CC} ${LDFLAGS} -o csokoban ${OBJS} ${LDADD} -lc -lgcc
	nroff -man csokoban.6 > csokoban.0

.include <bsd.port.mk>
