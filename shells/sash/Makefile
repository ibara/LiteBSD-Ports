# sash Makefile
V= 3.8
PORT= sash
PROG= ${PORT}
SRCS=	sash.c cmds.c cmd_dd.c cmd_ed.c cmd_grep.c cmd_ls.c cmd_tar.c \
	cmd_gzip.c cmd_find.c cmd_file.c cmd_chattr.c cmd_ar.c utils.c

CFLAGS+=-Wall -Wmissing-prototypes -DHAVE_GZIP -DMOUNT_TYPE='"none"'\
	-ffunction-sections -fdata-sections -Wno-error=unused-but-set-variable
LDFLAGS+=-Wl,--gc-sections -s
LDADD=	-lz

beforeinstall:
	install -d ${DESTDIR}${PREFIX}/share/doc/sash
	install -c -m 444 README ${DESTDIR}${PREFIX}/share/doc/sash
	-sed -i "s,LOCALBASE,${LOCALBASE},g" \
		${DESTDIR}${PREFIX}/share/doc/sash/README
	-sed -i "s,PROG,${PROG},g" \
		${DESTDIR}${PREFIX}/share/doc/sash/README

.include <bsd.port.mk>
