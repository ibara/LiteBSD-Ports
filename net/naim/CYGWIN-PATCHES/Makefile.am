READMEFILE = $(PACKAGE)-$(VERSION).README
EXTRA_DIST = ${READMEFILE}

if INSTALLCYGWINDOC
cygwindoc_DATA = ${READMEFILE}
endif

${READMEFILE}:
	rm -f ${top_srcdir}/CYGWIN-PATCHES/*.README *.README
	@printf "Generating Cygwin README file, ${READMEFILE}..."
	@(echo "naim"; \
	echo "         naim is a console AIM, ICQ, IRC, and lily CMC client. Development work"; \
	echo "         is done on Linux, but naim should work on any system with an ANSI C"; \
	echo "         compiler, BSD sockets, and ncurses. This includes operating systems"; \
	echo "         such as FreeBSD, Irix, NetBSD, OpenBSD, Solaris, etc., and naim can"; \
	echo "         also be run under Windows 2000 (using Interix Services For UNIX or"; \
	echo "         Cygwin). naim has been in development since October, 1998."; \
	echo ""; \
	echo "Build procedure:"; \
	echo "  Standard for autoconf packages. See the INSTALL file for complete"; \
	echo "  documentation."; \
	echo ""; \
	echo "Packaging procedure:"; \
	echo "  To create a Slackware pkgtool-compatible tarball, simply run:"; \
	echo "  ./configure \\"; \
	printf "\t"; \
	grep with.options ${top_builddir}/config.status | sed -e 's/^  with options \\\"//' -e 's/\\\"$$//' -e "s/' '/' \\\\@	'/g" -e "s/'$$/' \\\\/" | tr @ '\n'; \
	echo "	&& make tgz"; \
	echo "  If configure is run under Cygwin, make tgz will adjust itself to conform to"; \
	echo "  Cygwin standards. Use make cyg instead of make tgz to produce a complete"; \
	echo "  (source and binary) Cygwin release."; \
	echo ""; \
	echo "Runtime requirements:"; \
	echo "  libncurses7"; \
	echo "  screen [optional]"; \
	echo ""; \
	echo "Build requirements:"; \
	echo "  gcc"; \
	echo "  libncurses-devel"; \
	echo "  libtool"; \
	echo "  make"; \
	echo ""; \
	echo "Master sites:"; \
	echo "  http://shell.n.ml.org/n/naim/"; \
	echo "  file:///afs/n.ml.org/dist/naim/"; \
	echo "  afsdb://n.ml.org/dist/naim/"; \
	echo ""; \
	echo "Maintainer:"; \
	echo "  Daniel Reed <n@ml.org>") \
	>${READMEFILE}
	@echo ""

all-am: Makefile ${READMEFILE}
clean:
	rm -f ${top_srcdir}/CYGWIN-PATCHES/*.README *.README
