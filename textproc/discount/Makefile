# discount Makefile
V=	2.2.0
PORT=	discount
OBJS=	mkdio.o markdown.o dumptree.o generate.o \
	resource.o docheader.o version.o toc.o css.o \
	xml.o Csio.o xmlpage.o basename.o emmatch.o \
	github_flavoured.o setup.o tags.o html5.o flags.o 
CLEANFILES+= $(PGMS) $(TESTFRAMEWORK) $(SAMPLE_PGMS) $(MKDLIB) mktags blocktags *.o *.0*

CFLAGS+= -Wno-return-type -Wno-implicit-int -I. -ffunction-sections -fdata-sections
LDFLAGS+= -Wl,--gc-sections -s -L.

PGMS=markdown
SAMPLE_PGMS=mkd2html makepage
SAMPLE_PGMS+= theme
MKDLIB=libmarkdown.a
OBJS=mkdio.o markdown.o dumptree.o generate.o \
     resource.o docheader.o version.o toc.o css.o \
     xml.o Csio.o xmlpage.o basename.o emmatch.o \
     github_flavoured.o setup.o tags.o html5.o flags.o 
TESTFRAMEWORK=echo cols

all: $(PGMS) $(SAMPLE_PGMS) $(TESTFRAMEWORK)

do-install: $(PGMS) $(SAMPLE_PGMS) $(TESTFRAMEWORK)
	nroff -man markdown.3 > markdown.03
	nroff -man mkd-callbacks.3 > mkd-callbacks.0
	nroff -man mkd-functions.3 > mkd-functions.0
	nroff -man mkd-line.3 > mkd-line.0
	nroff -man markdown.7 > markdown.07
	nroff -man mkd-extensions.7 > mkd-extensions.0
	install -d $(DESTDIR)$(BINDIR)
	install -d $(DESTDIR)$(MANDIR)1
	install -d $(DESTDIR)$(MANDIR)3
	install -d $(DESTDIR)$(MANDIR)7
	install -d $(DESTDIR)$(PREFIX)/include
	install -d $(DESTDIR)$(PREFIX)/lib
	install -c -m 555 $(PGMS) $(DESTDIR)$(BINDIR)
	install -c -m 555 $(SAMPLE_PGMS) $(DESTDIR)$(BINDIR)
	install -c -m 555 cols $(DESTDIR)$(BINDIR)
	install -c -m 444 $(MKDLIB) $(DESTDIR)$(PREFIX)/lib
	install -m 444 mkdio.h $(DESTDIR)$(PREFIX)/include
	install -c -m 444 markdown.0 makepage.0 theme.0 mkd2html.0 \
		${DESTDIR}${MANDIR}1
	install -c -m 444 markdown.03 ${DESTDIR}${MANDIR}3/markdown.0
	install -c -m 444 mkd-callbacks.0 mkd-functions.0 mkd-line.0 \
		${DESTDIR}${MANDIR}3
	install -c -m 444 markdown.07 ${DESTDIR}${MANDIR}7/markdown.0
	install -c -m 444 mkd-extensions.0 ${DESTDIR}${MANDIR}7

version.o: version.c VERSION
	$(CC) $(CFLAGS) -DVERSION=\"`cat VERSION`\" -c version.c

VERSION:
	@true

tags.o: tags.c blocktags

blocktags: mktags
	./mktags > blocktags

mktags:
	cc -o mktags mktags.c

# example programs
theme:  theme.o $(MKDLIB) mkdio.h
	$(CC) $(CFLAGS) $(LDFLAGS) -o theme theme.o pgm_options.o -lmarkdown -lc -lgcc
	nroff -man $@.1 > $@.0

mkd2html:  mkd2html.o $(MKDLIB) mkdio.h
	$(CC) $(CFLAGS) $(LDFLAGS) -o mkd2html mkd2html.o -lmarkdown -lc -lgcc
	nroff -man $@.1 > $@.0

markdown: main.o pgm_options.o $(MKDLIB)
	$(CC) $(CFLAGS) $(LDFLAGS) -o markdown main.o pgm_options.o -lmarkdown -lc -lgcc
	nroff -man $@.1 > $@.0
	
makepage:  makepage.c pgm_options.o $(MKDLIB) mkdio.h
	$(CC) $(CFLAGS) $(LDFLAGS) -o makepage makepage.c pgm_options.o -lmarkdown -lc -lgcc
	nroff -man $@.1 > $@.0

pgm_options.o: pgm_options.c mkdio.h config.h
	$(CC) $(CFLAGS) -I. -c pgm_options.c

main.o: main.c mkdio.h config.h
	$(CC) $(CFLAGS) -I. -c main.c

$(MKDLIB): $(OBJS)
	${AR} cru $(MKDLIB) VERSION $(OBJS)
	${RANLIB} ${MKDLIB}

verify: echo tools/checkbits.sh
	@./echo -n "headers ... "; tools/checkbits.sh && echo "GOOD"

test:	$(PGMS) $(TESTFRAMEWORK) verify
	@for x in tests/*.t; do \
	    HERE=`pwd` sh $$x || exit 1; \
	done

cols:   tools/cols.c config.h
	$(CC) $(CFLAGS) $(LDFLAGS) -o cols tools/cols.c -lc -lgcc
echo:   tools/echo.c config.h
	$(CC) $(CFLAGS) $(LDFLAGS) -o echo tools/echo.c -lc -lgcc
	
Csio.o: Csio.c cstring.h amalloc.h config.h markdown.h
amalloc.o: amalloc.c
basename.o: basename.c config.h cstring.h amalloc.h markdown.h
css.o: css.c config.h cstring.h amalloc.h markdown.h
docheader.o: docheader.c config.h cstring.h amalloc.h markdown.h
dumptree.o: dumptree.c markdown.h cstring.h amalloc.h config.h
emmatch.o: emmatch.c config.h cstring.h amalloc.h markdown.h
generate.o: generate.c config.h cstring.h amalloc.h markdown.h
main.o: main.c config.h amalloc.h
pgm_options.o: pgm_options.c pgm_options.h config.h amalloc.h
makepage.o: makepage.c
markdown.o: markdown.c config.h cstring.h amalloc.h markdown.h
mkd2html.o: mkd2html.c config.h mkdio.h cstring.h amalloc.h
mkdio.o: mkdio.c config.h cstring.h amalloc.h markdown.h
resource.o: resource.c config.h cstring.h amalloc.h markdown.h
theme.o: theme.c config.h mkdio.h cstring.h amalloc.h
toc.o: toc.c config.h cstring.h amalloc.h markdown.h
version.o: version.c config.h
xml.o: xml.c config.h cstring.h amalloc.h markdown.h
xmlpage.o: xmlpage.c config.h cstring.h amalloc.h markdown.h

.include <bsd.port.mk>
