# Makefile for "p2c", the Pascal to C translator.
#  Copyright (C) 1989, 1990, 1991, 1992, 1993 Free Software Foundation.
#  Author: Dave Gillespie.
#  Author's address: daveg@synaptics.com.

# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation (any version).

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
V = 1.21a
PORT = p2c
CFLAGS += -ffunction-sections -fdata-sections
LDFLAGS += -Wl,--gc-sections -s

# Custom translator modules
CUSTSRCS = hpmods.c citmods.c
CUSTOBJS = hpmods.o citmods.o
CUSTDEFS = -DCUST1=hpmods -DCUST2=citmods


# File names
P2CSRCS = trans.c stuff.c out.c comment.c lex.c parse.c decl.c \
          expr.c pexpr.c funcs.c dir.c
P2COBJS = trans.o stuff.o out.o comment.o lex.o parse.o decl.o \
          expr.o pexpr.o funcs.o dir.o

SRCS = $(P2CSRCS) $(CUSTSRCS)
OBJS = $(P2COBJS) $(CUSTOBJS)

LIBSRCS = p2clib.c loc.p2clib.c
LIBOBJS = p2clib.o loc.p2clib.o
OTHERLIBOBJS =

ABSHOMEDIR = /usr/local/share/p2c
ABSINCDIR = `cd $(INCDIR); pwd`
ABSLIBDIR = `cd $(LIBDIR); pwd`

MISCSRCS = makeproto.c
PROTOS = p2c.proto p2c.hdrs
HDRS = trans.h p2c.h


# Top-level targets
#all: proto p2c libp2c.a p2c.cat
all: proto p2c libp2c.a
proto: $(PROTOS)


# Making p2c
p2c: $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o p2c -lc -lgcc
	nroff -man p2c.1 > p2c.0

dir.o: dir.c trans.h
	$(CC) -c $(CFLAGS) $(CUSTDEFS) dir.c

trans.o: trans.c trans.h
	$(CC) -c $(CFLAGS) -DHASDUMPS -DP2C_HOME=\"$(ABSHOMEDIR)\" trans.c


# Making and using makeproto
p2c.hdrs: $(SRCS) makeproto
	./makeproto -n -m -h -t16 -a35 -s0 -x $(SRCS) -o p2c.hdrs

p2c.proto: $(SRCS) makeproto
	./makeproto -n -m -h -t16 -a35 -s1 -i $(SRCS) -o p2c.proto

makeproto: makeproto.c
	gcc makeproto.c -o makeproto


# Making the p2c runtime library
libp2c.a: $(LIBOBJS)
	${AR} r libp2c.a $(LIBOBJS) $(OTHERLIBOBJS)
	${RANLIB} libp2c.a

p2clib.o: p2clib.c
	$(CC) -c $(CFLAGS) p2clib.c


# Making the p2c man page
p2c.man.inst: p2c.man
	sed -e "s;--HOMEDIR--;$(ABSHOMEDIR);"   \
            -e "s;--INCDIR--;$(ABSINCDIR);"     \
            -e "s;--LIBDIR--;$(ABSLIBDIR);"     \
            p2c.man >p2c.man.inst

p2c.man.Z: p2c.man.inst
	compress -c p2c.man.inst >p2c.man.Z

p2c.cat: p2c.man.inst
	if [ -f /usr/bin/nroff -o -f /bin/nroff ];  \
	    then nroff -man p2c.man.inst >p2c.cat; fi


# The p2cc script.
p2cc: p2cc.perl
	sed -e "s;--HOMEDIR--;$(ABSHOMEDIR);"   \
	    -e "s;--INCDIR--;$(ABSINCDIR);"     \
	    -e "s;--LIBDIR--;$(ABSLIBDIR);"     \
	    -e "s;/bin/perl;`which perl`;"      \
	    p2cc.perl >p2cc
	chmod a+x p2cc



# Initially installing p2c:
#  First, make sure $(HOMEDIR) and $(INCDIR) exist and are writable;
#  Second, make sure $(LIBDIR), $(BINDIR) and $(MANDIR) are writable;
#  Third, execute "make install" to compile and set things up.
# (You may need to have a system operator do these steps for you.)

COPY = cp

newhome:
	rm -f trans.o     # force trans.c to be recompiled (if HOMEDIR changes)

install: proto \
	makedirs \
	$(BINDIR)/p2c         \
	$(BINDIR)/p2cc        \
	$(LIBDIR)/libp2c.a    \
	$(MANDIR)/p2c.1	      \
	$(INCDIR)/p2c.h	      \
	$(HOMEDIR)/p2crc      \
	$(HOMEDIR)/loc.p2crc  \
	$(HOMEDIR)/system.imp \
	$(HOMEDIR)/system.m2  \
	$(HOMEDIR)/turbo.imp  \
	$(HOMEDIR)/string.pas

SHELL=/bin/sh
makedirs:
	if [ ! -d $(HOMEDIR) ]; then mkdir $(HOMEDIR); else true; fi
	if [ ! -d $(BINDIR) ]; then mkdir $(BINDIR); else true; fi
	if [ ! -d $(LIBDIR) ]; then mkdir $(LIBDIR); else true; fi
	if [ ! -d $(MANDIR) ]; then mkdir $(MANDIR); else true; fi
	if [ ! -d $(INCDIR) ]; then mkdir $(INCDIR); else true; fi

$(BINDIR)/p2c: p2c
	$(COPY)  p2c          $(BINDIR)/p2c

$(BINDIR)/p2cc: p2cc
	$(COPY)  p2cc         $(BINDIR)/p2cc

$(LIBDIR)/libp2c.a: libp2c.a
	$(COPY)  libp2c.a     $(LIBDIR)/libp2c.a
	if [ -f /usr/bin/ranlib -o -f /bin/ranlib ]; then ranlib $(LIBDIR)/libp2c.a; fi

$(MANDIR)/p2c.1: $(MANFILE)
	$(COPY)  $(MANFILE)   $(MANDIR)/p2c.1

$(INCDIR)/p2c.h: p2c.h
	$(COPY)  p2c.h        $(INCDIR)/p2c.h

$(HOMEDIR)/p2crc: sys.p2crc
	$(COPY)  sys.p2crc    $(HOMEDIR)/p2crc

$(HOMEDIR)/loc.p2crc: loc.p2crc
	$(COPY)  loc.p2crc    $(HOMEDIR)/loc.p2crc

$(HOMEDIR)/system.imp: system.imp
	$(COPY)  system.imp   $(HOMEDIR)/system.imp

$(HOMEDIR)/system.m2: system.m2
	$(COPY)  system.m2    $(HOMEDIR)/system.m2

$(HOMEDIR)/turbo.imp: turbo.imp
	$(COPY)  turbo.imp    $(HOMEDIR)/turbo.imp

$(HOMEDIR)/string.pas: string.pas
	$(COPY)  string.pas   $(HOMEDIR)/string.pas

do-install:
	install -d ${DESTDIR}${BINDIR}
	install -d ${DESTDIR}${MANDIR}1
	install -d ${DESTDIR}${PREFIX}/include/p2c
	install -d ${DESTDIR}${PREFIX}/lib
	install -d ${DESTDIR}${PREFIX}/share/p2c
	install -d ${DESTDIR}${PREFIX}/share/examples/p2c
	install -c -m 555 p2c ${DESTDIR}${BINDIR}
	install -c -m 444 p2c.h ${DESTDIR}${PREFIX}/include/p2c
	install -c -m 444 libp2c.a ${DESTDIR}${PREFIX}/lib
	install -c -m 444 sys.p2crc ${DESTDIR}${PREFIX}/share/p2c/p2crc
	install -c -m 444 loc.p2crc ${DESTDIR}${PREFIX}/share/p2c/loc.p2crc
	install -c -m 444 *.imp *.m2 *.pas ${DESTDIR}${PREFIX}/share/p2c
	install -c -m 444 examples/*.p ${DESTDIR}${PREFIX}/share/examples/p2c


# Miscellaneous
tags:
	etags $(SRCS) $(LIBSRCS) $(MISCSRCS) $(HDRS)

clean.o:
	rm -f $(OBJS) $(LIBOBJS) $(PROTOS) makeproto p2c libp2c.a *.0

clean:
	rm -f $(OBJS) $(LIBOBJS) $(PROTOS) makeproto p2c libp2c.a *.0

wc:
	wc $(SRCS) $(LIBSRCS) trans.h

test:
	echo '"make test" should be used in the outer-level p2c directory.'
	echo 'Type "cd .." and "make test" again.'

.include <bsd.port.mk>
