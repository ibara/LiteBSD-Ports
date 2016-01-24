# Makefile.am
# (c) 2002 Mikulas Patocka, Karel 'Clock' Kulhavy, Petr 'Brain' Kulhavy,
#          Martin 'PerM' Pergel

man_MANS=links.1

EXTRA_DIST=$(man_MANS) atheos.cpp BRAILLE_HOWTO KEYS SITES descrip.mms *.opt PATCH-gpm-1.20.0-smooth-cursor PATCH-libpng-1.2.18 mailcap.pl rebuild Links_logo.png Unicode/*.cp Unicode/[!C]* intl/[!C]* graphics/*.c graphics/arrow.png graphics/gen graphics/system_font/*.png graphics/font/century_extensions-medium-roman-serif-vari/*.png graphics/font/century_school-bold-roman-serif-vari/*.png graphics/font/century_school-medium-roman-serif-vari/*.png graphics/font/courier-medium-roman-serif-mono/*.png graphics/font/japanese-medium-roman-sans-mono/*.png graphics/font/symbol-medium-roman-sans-vari/*.png graphics/font/century_school-bold-roman-serif-vari/aliases graphics/font/century_school-medium-roman-serif-vari/aliases graphics/font/japanese-medium-roman-sans-mono/aliases graphics/font/symbol-medium-roman-sans-vari/aliases doc/links_cal/*.png doc/links_cal/*.html graphics/Makefile graphics/Fontmap graphics/clip.c graphics/links.xpm graphics/makefont graphics/pdf2html graphics/spacer.png pkg-config.m4 *.xpm *.ico *.rc mk-release-os2 mk-release-win mk-dos mk-dos83.cmd links.wis *.nsi links.crt

bin_PROGRAMS=links

if ATHEOS_GR
links_LDADD=atheos.o
else
endif

links_SOURCES=af_unix.c auth.c beos.c bfu.c block.c bookmark.c cache.c charsets.c compress.c connect.c cookies.c data.c default.c dip.c directfb.c dither.c dns.c dos.c drivers.c error.c file.c finger.c fn_impl.c font_inc.c framebuf.c ftp.c gif.c grx.c hpux.c html.c html_gr.c html_r.c html_tbl.c http.c https.c img.c imgcache.c jpeg.c jsint.c kbd.c language.c listedit.c lru.c mailto.c main.c memory.c menu.c objreq.c os_dep.c pmshell.c png.c sched.c select.c session.c smb.c string.c svg.c svgalib.c terminal.c tiff.c types.c url.c view.c view_gr.c vms.c x.c xbm.c beos.h bits.h cfg.h codepage.h com-defs.h config-vms.h dfb_cur.h hpux.h language.h links.h os_dep.h os_depx.h setup.h arrow.inc codepage.inc entity.inc fbcommon.inc language.inc links_ic.inc uni_7b.inc upcase.inc vpipe.inc

dist-hook:
#remove the symlinka:
	rm $(distdir)/INSTALL
	echo Links $(VERSION) -- How To Install > $(distdir)/INSTALL
	echo Links $(VERSION) -- How To Install | tr -c '\n' '-' >> $(distdir)/INSTALL
	echo >> $(distdir)/INSTALL
	cat $(srcdir)/INSTALL >> $(distdir)/INSTALL

CXXFLAGS=@CXXFLAGS@

atheos.o: $(srcdir)/atheos.cpp
	$(CXX) $(DEFS) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c $(srcdir)/atheos.cpp

datadir = $(prefix)/@DATADIRNAME@
LIBS = @LIBS@

