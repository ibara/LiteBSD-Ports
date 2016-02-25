# $Id$
V= 4.2
PORT= lcc
TARGET=mips/litebsd
BUILDDIR=${.CURDIR}
HOSTFILE=${.CURDIR}/etc/litebsd.c
A=.a
O=.o
E=-prog
CFLAGS+=-DNDEBUG -ffunction-sections -fdata-sections
LDFLAGS+=-Wl,--gc-sections -s
LD=$(CC)
ARFLAGS=ruv
DIFF=diff
RM=rm -f
TSTDIR=$(BUILDDIR)/$(TARGET)/tst
CUSTOM=custom.mk
include $(CUSTOM)
B=$(BUILDDIR)/
T=$(TSTDIR)/

all::	rcc lburg cpp lcc bprint liblcc

rcc:	$Brcc$E
lburg:	$Blburg$E
cpp:	$Bcpp$E
lcc:	$Blcc$E
bprint:	$Bbprint$E
liblcc:	$Bliblcc$A

RCCOBJS=$Balloc$O \
	$Bbind$O \
	$Bdag$O \
	$Bdagcheck$O \
	$Bdecl$O \
	$Benode$O \
	$Berror$O \
	$Bexpr$O \
	$Bevent$O \
	$Binit$O \
	$Binits$O \
	$Binput$O \
	$Blex$O \
	$Blist$O \
	$Bmain$O \
	$Boutput$O \
	$Bprof$O \
	$Bprofio$O \
	$Bsimp$O \
	$Bstmt$O \
	$Bstring$O \
	$Bsym$O \
	$Btrace$O \
	$Btree$O \
	$Btypes$O \
	$Bnull$O \
	$Bsymbolic$O \
	$Bgen$O \
	$Bbytecode$O \
	$Balpha$O \
	$Bmips$O \
	$Bsparc$O \
	$Bstab$O \
	$Bx86$O \
	$Bx86linux$O

$Brcc$E::	$Bmain$O $Blibrcc$A $(EXTRAOBJS)
		$(LD) $(LDFLAGS) -o $@ $Bmain$O $(EXTRAOBJS) $Blibrcc$A $(EXTRALIBS) -lc -lgcc

$Blibrcc$A:	$(RCCOBJS)
		$(AR) $(ARFLAGS) $@ $(RCCOBJS); $(RANLIB) $@ || true

$(RCCOBJS):	src/c.h src/ops.h src/token.h src/config.h

$Balloc$O:	src/alloc.c;	$(CC) $(CFLAGS) -c -I${.CURDIR}/src -o $@ ${.CURDIR}/src/alloc.c
$Bbind$O:	src/bind.c;	$(CC) $(CFLAGS) -c -I${.CURDIR}/src -o $@ ${.CURDIR}/src/bind.c
$Bdag$O:	src/dag.c;	$(CC) $(CFLAGS) -c -I${.CURDIR}/src -o $@ ${.CURDIR}/src/dag.c
$Bdecl$O:	src/decl.c;	$(CC) $(CFLAGS) -c -I${.CURDIR}/src -o $@ ${.CURDIR}/src/decl.c
$Benode$O:	src/enode.c;	$(CC) $(CFLAGS) -c -I${.CURDIR}/src -o $@ ${.CURDIR}/src/enode.c
$Berror$O:	src/error.c;	$(CC) $(CFLAGS) -c -I${.CURDIR}/src -o $@ ${.CURDIR}/src/error.c
$Bevent$O:	src/event.c;	$(CC) $(CFLAGS) -c -I${.CURDIR}/src -o $@ ${.CURDIR}/src/event.c
$Bexpr$O:	src/expr.c;	$(CC) $(CFLAGS) -c -I${.CURDIR}/src -o $@ ${.CURDIR}/src/expr.c
$Bgen$O:	src/gen.c;	$(CC) $(CFLAGS) -c -I${.CURDIR}/src -o $@ ${.CURDIR}/src/gen.c
$Binit$O:	src/init.c;	$(CC) $(CFLAGS) -c -I${.CURDIR}/src -o $@ ${.CURDIR}/src/init.c
$Binits$O:	src/inits.c;	$(CC) $(CFLAGS) -c -I${.CURDIR}/src -o $@ ${.CURDIR}/src/inits.c
$Binput$O:	src/input.c;	$(CC) $(CFLAGS) -c -I${.CURDIR}/src -o $@ ${.CURDIR}/src/input.c
$Blex$O:	src/lex.c;	$(CC) $(CFLAGS) -c -I${.CURDIR}/src -o $@ ${.CURDIR}/src/lex.c
$Blist$O:	src/list.c;	$(CC) $(CFLAGS) -c -I${.CURDIR}/src -o $@ ${.CURDIR}/src/list.c
$Bmain$O:	src/main.c;	$(CC) $(CFLAGS) -c -I${.CURDIR}/src -o $@ ${.CURDIR}/src/main.c
$Bnull$O:	src/null.c;	$(CC) $(CFLAGS) -c -I${.CURDIR}/src -o $@ ${.CURDIR}/src/null.c
$Boutput$O:	src/output.c;	$(CC) $(CFLAGS) -c -I${.CURDIR}/src -o $@ ${.CURDIR}/src/output.c
$Bprof$O:	src/prof.c;	$(CC) $(CFLAGS) -c -I${.CURDIR}/src -o $@ ${.CURDIR}/src/prof.c
$Bprofio$O:	src/profio.c;	$(CC) $(CFLAGS) -c -I${.CURDIR}/src -o $@ ${.CURDIR}/src/profio.c
$Bsimp$O:	src/simp.c;	$(CC) $(CFLAGS) -c -I${.CURDIR}/src -o $@ ${.CURDIR}/src/simp.c
$Bstmt$O:	src/stmt.c;	$(CC) $(CFLAGS) -c -I${.CURDIR}/src -o $@ ${.CURDIR}/src/stmt.c
$Bstring$O:	src/string.c;	$(CC) $(CFLAGS) -c -I${.CURDIR}/src -o $@ ${.CURDIR}/src/string.c
$Bsym$O:	src/sym.c;	$(CC) $(CFLAGS) -c -I${.CURDIR}/src -o $@ ${.CURDIR}/src/sym.c
$Bsymbolic$O:	src/symbolic.c;	$(CC) $(CFLAGS) -c -I${.CURDIR}/src -o $@ ${.CURDIR}/src/symbolic.c
$Bbytecode$O:	src/bytecode.c;	$(CC) $(CFLAGS) -c -I${.CURDIR}/src -o $@ ${.CURDIR}/src/bytecode.c
$Btrace$O:	src/trace.c;	$(CC) $(CFLAGS) -c -I${.CURDIR}/src -o $@ ${.CURDIR}/src/trace.c
$Btree$O:	src/tree.c;	$(CC) $(CFLAGS) -c -I${.CURDIR}/src -o $@ ${.CURDIR}/src/tree.c
$Btypes$O:	src/types.c;	$(CC) $(CFLAGS) -c -I${.CURDIR}/src -o $@ ${.CURDIR}/src/types.c
$Bstab$O:	src/stab.c src/stab.h;	$(CC) $(CFLAGS) -c -I${.CURDIR}/src -o $@ ${.CURDIR}/src/stab.c

$Bdagcheck$O:	$Bdagcheck.c;	$(CC) $(CFLAGS) -c -I${.CURDIR}/src -o $@ $Bdagcheck.c
$Balpha$O:	$Balpha.c;	$(CC) $(CFLAGS) -c -I${.CURDIR}/src -o $@ $Balpha.c
$Bmips$O:	$Bmips.c;	$(CC) $(CFLAGS) -c -I${.CURDIR}/src -o $@ $Bmips.c
$Bsparc$O:	$Bsparc.c;	$(CC) $(CFLAGS) -c -I${.CURDIR}/src -o $@ $Bsparc.c
$Bx86$O:	$Bx86.c;	$(CC) $(CFLAGS) -c -I${.CURDIR}/src -o $@ $Bx86.c
$Bx86linux$O:	$Bx86linux.c;	$(CC) $(CFLAGS) -c -I${.CURDIR}/src -o $@ $Bx86linux.c

$Bdagcheck.c:	$Blburg$E src/dagcheck.md; $Blburg$E ${.CURDIR}/src/dagcheck.md $@
$Balpha.c:	$Blburg$E src/alpha.md;    $Blburg$E ${.CURDIR}/src/alpha.md    $@
$Bmips.c:	$Blburg$E src/mips.md;     $Blburg$E ${.CURDIR}/src/mips.md     $@
$Bsparc.c:	$Blburg$E src/sparc.md;    $Blburg$E ${.CURDIR}/src/sparc.md    $@
$Bx86.c:	$Blburg$E src/x86.md;      $Blburg$E ${.CURDIR}/src/x86.md      $@
$Bx86linux.c:	$Blburg$E src/x86linux.md; $Blburg$E ${.CURDIR}/src/x86linux.md $@

$Bbprint$E:	$Bbprint$O;		$(LD) $(LDFLAGS) -o $@ $Bbprint$O -lc -lgcc
$Bops$E:	$Bops$O;		$(LD) $(LDFLAGS) -o $@ $Bops$O -lc -lgcc

$Bbprint$O:	etc/bprint.c src/profio.c;	$(CC) $(CFLAGS) -c -I${.CURDIR}/src -o $@ ${.CURDIR}/etc/bprint.c
$Bops$O:	etc/ops.c src/ops.h;		$(CC) $(CFLAGS) -c -I${.CURDIR}/src -o $@ ${.CURDIR}/etc/ops.c

$Blcc$E:	$Blcc$O $Bhost$O;	$(LD) $(LDFLAGS) -o $@ $Blcc$O $Bhost$O -lc -lgcc

$Blcc$O:	etc/lcc.c;		$(CC) $(CFLAGS) -c -o $@ ${.CURDIR}/etc/lcc.c
$Bhost$O:	$(HOSTFILE);	$(CC) $(CFLAGS) -c -o $@ $(HOSTFILE)

LIBOBJS=$Bassert$O $Bbbexit$O $Byynull$O

$Bliblcc$A:	$(LIBOBJS);	$(AR) $(ARFLAGS) $@ $Bassert$O $Bbbexit$O $Byynull$O; $(RANLIB) $@ || true

$Bassert$O:	lib/assert.c;	$(CC) $(CFLAGS) -c -o $@ ${.CURDIR}/lib/assert.c
$Byynull$O:	lib/yynull.c;	$(CC) $(CFLAGS) -c -o $@ ${.CURDIR}/lib/yynull.c
$Bbbexit$O:	lib/bbexit.c;	$(CC) $(CFLAGS) -c -o $@ ${.CURDIR}/lib/bbexit.c

$Blburg$E:	$Blburg$O $Bgram$O;	cc -o $@ $Blburg$O $Bgram$O 

$Blburg$O $Bgram$O:	lburg/lburg.h

$Blburg$O:	lburg/lburg.c;	cc -c -I${.CURDIR}/lburg -o $@ ${.CURDIR}/lburg/lburg.c
$Bgram$O:	lburg/gram.c;	cc -c -I${.CURDIR}/lburg -o $@ ${.CURDIR}/lburg/gram.c

CPPOBJS=$Bcpp$O $Blexer$O $Bnlist$O $Btokens$O $Bmacro$O $Beval$O \
	$Binclude$O $Bhideset$O $Bgetopt$O $Bunix$O

$Bcpp$E:	$(CPPOBJS)
		$(LD) $(LDFLAGS) -o $@ $(CPPOBJS) -lc -lgcc

$(CPPOBJS):	cpp/cpp.h

$Bcpp$O:	cpp/cpp.c;	$(CC) $(CFLAGS) -c -I${.CURDIR}/cpp -o $@ ${.CURDIR}/cpp/cpp.c
$Blexer$O:	cpp/lex.c;	$(CC) $(CFLAGS) -c -I${.CURDIR}/cpp -o $@ ${.CURDIR}/cpp/lex.c
$Bnlist$O:	cpp/nlist.c;	$(CC) $(CFLAGS) -c -I${.CURDIR}/cpp -o $@ ${.CURDIR}/cpp/nlist.c
$Btokens$O:	cpp/tokens.c;	$(CC) $(CFLAGS) -c -I${.CURDIR}/cpp -o $@ ${.CURDIR}/cpp/tokens.c
$Bmacro$O:	cpp/macro.c;	$(CC) $(CFLAGS) -c -I${.CURDIR}/cpp -o $@ ${.CURDIR}/cpp/macro.c
$Beval$O:	cpp/eval.c;	$(CC) $(CFLAGS) -c -I${.CURDIR}/cpp -o $@ ${.CURDIR}/cpp/eval.c
$Binclude$O:	cpp/include.c;	$(CC) $(CFLAGS) -c -I${.CURDIR}/cpp -o $@ ${.CURDIR}/cpp/include.c
$Bhideset$O:	cpp/hideset.c;	$(CC) $(CFLAGS) -c -I${.CURDIR}/cpp -o $@ ${.CURDIR}/cpp/hideset.c
$Bgetopt$O:	cpp/getopt.c;	$(CC) $(CFLAGS) -c -I${.CURDIR}/cpp -o $@ ${.CURDIR}/cpp/getopt.c
$Bunix$O:	cpp/unix.c;	$(CC) $(CFLAGS) -c -I${.CURDIR}/cpp -o $@ ${.CURDIR}/cpp/unix.c

test:	$T8q.s \
	$Tarray.s \
	$Tcf.s \
	$Tcq.s \
	$Tcvt.s \
	$Tfields.s \
	$Tfront.s \
	$Tincr.s \
	$Tinit.s \
	$Tlimits.s \
	$Tparanoia.s \
	$Tsort.s \
	$Tspill.s \
	$Tstdarg.s \
	$Tstruct.s \
	$Tswitch.s \
	$Twf1.s \
	$Tyacc.s

$T8q.s:	tst/8q.c tst/8q.0 all;	@env BUILDDIR=$(BUILDDIR) TSTDIR=$(TSTDIR) src/run.sh $@
$Tarray.s:	tst/array.c tst/array.0 all;	@env BUILDDIR=$(BUILDDIR) TSTDIR=$(TSTDIR) src/run.sh $@
$Tcf.s:	tst/cf.c tst/cf.0 all;	@env BUILDDIR=$(BUILDDIR) TSTDIR=$(TSTDIR) src/run.sh $@
$Tcq.s:	tst/cq.c tst/cq.0 all;	@env BUILDDIR=$(BUILDDIR) TSTDIR=$(TSTDIR) src/run.sh $@
$Tcvt.s:	tst/cvt.c tst/cvt.0 all;	@env BUILDDIR=$(BUILDDIR) TSTDIR=$(TSTDIR) src/run.sh $@
$Tfields.s:	tst/fields.c tst/fields.0 all;	@env BUILDDIR=$(BUILDDIR) TSTDIR=$(TSTDIR) src/run.sh $@
$Tfront.s:	tst/front.c tst/front.0 all;	@env BUILDDIR=$(BUILDDIR) TSTDIR=$(TSTDIR) src/run.sh $@
$Tincr.s:	tst/incr.c tst/incr.0 all;	@env BUILDDIR=$(BUILDDIR) TSTDIR=$(TSTDIR) src/run.sh $@
$Tinit.s:	tst/init.c tst/init.0 all;	@env BUILDDIR=$(BUILDDIR) TSTDIR=$(TSTDIR) src/run.sh $@
$Tlimits.s:	tst/limits.c tst/limits.0 all;	@env BUILDDIR=$(BUILDDIR) TSTDIR=$(TSTDIR) src/run.sh $@
$Tparanoia.s:	tst/paranoia.c tst/paranoia.0 all;	@env BUILDDIR=$(BUILDDIR) TSTDIR=$(TSTDIR) src/run.sh $@
$Tsort.s:	tst/sort.c tst/sort.0 all;	@env BUILDDIR=$(BUILDDIR) TSTDIR=$(TSTDIR) src/run.sh $@
$Tspill.s:	tst/spill.c tst/spill.0 all;	@env BUILDDIR=$(BUILDDIR) TSTDIR=$(TSTDIR) src/run.sh $@
$Tstdarg.s:	tst/stdarg.c tst/stdarg.0 all;	@env BUILDDIR=$(BUILDDIR) TSTDIR=$(TSTDIR) src/run.sh $@
$Tstruct.s:	tst/struct.c tst/struct.0 all;	@env BUILDDIR=$(BUILDDIR) TSTDIR=$(TSTDIR) src/run.sh $@
$Tswitch.s:	tst/switch.c tst/switch.0 all;	@env BUILDDIR=$(BUILDDIR) TSTDIR=$(TSTDIR) src/run.sh $@
$Twf1.s:	tst/wf1.c tst/wf1.0 all;	@env BUILDDIR=$(BUILDDIR) TSTDIR=$(TSTDIR) src/run.sh $@
$Tyacc.s:	tst/yacc.c tst/yacc.0 all;	@env BUILDDIR=$(BUILDDIR) TSTDIR=$(TSTDIR) src/run.sh $@

testclean:
	$(RM) $T8q$E $T8q.s $T8q.2 $T8q.1
	$(RM) $Tarray$E $Tarray.s $Tarray.2 $Tarray.1
	$(RM) $Tcf$E $Tcf.s $Tcf.2 $Tcf.1
	$(RM) $Tcq$E $Tcq.s $Tcq.2 $Tcq.1
	$(RM) $Tcvt$E $Tcvt.s $Tcvt.2 $Tcvt.1
	$(RM) $Tfields$E $Tfields.s $Tfields.2 $Tfields.1
	$(RM) $Tfront$E $Tfront.s $Tfront.2 $Tfront.1
	$(RM) $Tincr$E $Tincr.s $Tincr.2 $Tincr.1
	$(RM) $Tinit$E $Tinit.s $Tinit.2 $Tinit.1
	$(RM) $Tlimits$E $Tlimits.s $Tlimits.2 $Tlimits.1
	$(RM) $Tparanoia$E $Tparanoia.s $Tparanoia.2 $Tparanoia.1
	$(RM) $Tsort$E $Tsort.s $Tsort.2 $Tsort.1
	$(RM) $Tspill$E $Tspill.s $Tspill.2 $Tspill.1
	$(RM) $Tstdarg$E $Tstdarg.s $Tstdarg.2 $Tstdarg.1
	$(RM) $Tstruct$E $Tstruct.s $Tstruct.2 $Tstruct.1
	$(RM) $Tswitch$E $Tswitch.s $Tswitch.2 $Tswitch.1
	$(RM) $Twf1$E $Twf1.s $Twf1.2 $Twf1.1
	$(RM) $Tyacc$E $Tyacc.s $Tyacc.2 $Tyacc.1

clean::		testclean
		$(RM) $B*$O
		$(RM) $Bdagcheck.c $Balpha.c $Bmips.c $Bx86.c $Bsparc.c $Bx86linux.c
		$(RM) $Brcc1$E $Brcc1$E $B1rcc$E $B2rcc$E
		$(RM) $B*.ilk
		$(RM) liblcc.a librcc.a *.0 *-prog

clobber::	clean
		$(RM) $Brcc$E $Blburg$E $Bcpp$E $Blcc$E $Bcp$E $Bbprint$E $B*$A
		$(RM) $B*.pdb $B*.pch

RCCSRCS=src/alloc.c \
	src/bind.c \
	src/dag.c \
	src/decl.c \
	src/enode.c \
	src/error.c \
	src/expr.c \
	src/event.c \
	src/init.c \
	src/inits.c \
	src/input.c \
	src/lex.c \
	src/list.c \
	src/main.c \
	src/output.c \
	src/prof.c \
	src/profio.c \
	src/simp.c \
	src/stmt.c \
	src/string.c \
	src/sym.c \
	src/trace.c \
	src/tree.c \
	src/types.c \
	src/null.c \
	src/symbolic.c \
	src/bytecode.c \
	src/gen.c \
	src/stab.c \
	$Bdagcheck.c \
	$Balpha.c \
	$Bmips.c \
	$Bsparc.c \
	$Bx86linux.c \
	$Bx86.c

C=$Blcc -A -d0.6 -Wo-lccdir=$(BUILDDIR) -I${.CURDIR}/src -I$(BUILDDIR)
triple:	$B2rcc$E
	strip $B1rcc$E $B2rcc$E
	dd if=$B1rcc$E of=$Brcc1$E bs=512 skip=1
	dd if=$B2rcc$E of=$Brcc2$E bs=512 skip=1
	if cmp $Brcc1$E $Brcc2$E; then \
		mv $B2rcc$E $Brcc$E; \
		$(RM) $B1rcc$E $Brcc[12]$E; fi

$B1rcc$E:	$Brcc$E $Blcc$E $Bcpp$E
		$C -o $@ -B$B $(RCCSRCS) -lc -lgcc
$B2rcc$E:	$B1rcc$E
		$C -o $@ -B$B1 $(RCCSRCS) -lc -lgcc

do-install:
	nroff -man bprint.1 > bprint.0
	nroff -man lcc.1 > lcc.0
	install -d ${DESTDIR}${BINDIR}
	install -d ${DESTDIR}${MANDIR}1
	install -d ${DESTDIR}${PREFIX}/lib
	install -d ${DESTDIR}${PREFIX}/libexec/lcc
	install -c -m 555 lcc$E ${DESTDIR}${BINDIR}/lcc
	install -c -m 555 cpp$E ${DESTDIR}${PREFIX}/libexec/lcc/cpp
	install -c -m 555 rcc$E ${DESTDIR}${PREFIX}/libexec/lcc/rcc
	install -c -m 555 bprint$E ${DESTDIR}${BINDIR}/bprint
	install -c -m 444 liblcc.a librcc.a ${DESTDIR}${PREFIX}/lib
	install -c -m 444 bprint.0 lcc.0 ${DESTDIR}${MANDIR}1

.include <bsd.port.mk>
