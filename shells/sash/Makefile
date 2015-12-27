#
# Makefile for sash
#
# The HAVE_GZIP definition adds the -gzip and -gunzip commands.
# The HAVE_LINUX_ATTR definition adds the -chattr and -lsattr commands.
# The HAVE_LINUX_CHROOT definition adds the -chroot command.
# The HAVE_LINUX_PIVOT definition adds the -pivot_root command.
# The HAVE_LINUX_LOSETUP definition adds the -losetup command.
# The HAVE_LINUX_MOUNT definition makes -mount and -umount work on Linux.
# The HAVE_BSD_MOUNT definition makes -mount and -umount work on BSD.
# The MOUNT_TYPE definition sets the default file system type for -mount.
#
# Note that the linker may show warnings about 'statically linked
# programs' requiring getpwnam, getpwuid, getgrnam and getgrgid.
# This is unavoidable since those routines use dynamic libraries anyway.
# Sash will still run, but if there are shared library problems then
# the user might have to be be careful when using the -chown, -chgrp,
# and -ls commands.
#
V=	3.8
PORT=	sash
PROG=	sash

CFLAGS+=-Wall -Wmissing-prototypes -DHAVE_GZIP -DMOUNT_TYPE='"none"'\
	-ffunction-sections -fdata-sections -Wno-error=unused-but-set-variable

LDFLAGS+=-Wl,--gc-sections

DPADD=	${LIBZ}
LDADD=	-lz

SRCS=	sash.c cmds.c cmd_dd.c cmd_ed.c cmd_grep.c cmd_ls.c cmd_tar.c \
	cmd_gzip.c cmd_find.c cmd_file.c cmd_chattr.c cmd_ar.c utils.c

afterinstall:
	install -d ${DESTDIR}${PKGBASE}/messages
	install -c -m 444 sash-3.8 ${DESTDIR}${PKGBASE}/messages
	-sed -i "s,LOCALBASE,${LOCALBASE},g" \
		${DESTDIR}${PKGBASE}/messages/sash-3.8
	-sed -i "s,PROG,${PROG},g" \
		${DESTDIR}${PKGBASE}/messages/sash-3.8

.include <bsd.port.mk>
