#=======================================================================
# Makefile for dired 3.05.  Generates BSD 4.2 UNIX version as is.
#
# Command-line macros:
#	BINDIR=directory-to-install-executable-in
#	MANDIR=directory-to-install-man-pages-in
#	MINFILES=minimum-number-of-files-preallocatted
#	MOREPGM=name-of-more-or-less-or-pager
#	EXTRADEFINES=-Dxyz -Dxxx ...
#	OPT=compiler-optimization
#
# Major Targets:
#	all		build everything
#	clean		remove temp and object files
#	clobber		remove all automatically-regenerable files
#	dired		build everything
#	install		install executable and man pages
#	install-exe	install executable
#	install-man	install man pages
#	uninstall	uninstall executable and man pages
#	uninstall-exe	uninstall executable
#	uninstall-man	uninstall man pages
#
# Convenience targets for particular systems:
#	aixps2		dired for IBM PC AIX PS/2
#	aix370		dired for IBM 370-family mainframe AIX 370
#	alpha		dired for DEC Alpha AXP systems on OSF/1
#	alpha-gcc		dired for DEC Alpha AXP systems on OSF/1
#	convex-c1	dired for Convex C1
#	fips		dired for FIPS 151-1
#	hpux		dired for HP-UX 9.0 (HP 9000/7xx)
#	ibm-rs6000	dired for IBM RS/6000 AIX 3.1 and 3.2
#	linux		dired for linux 1.2.8 or later and gcc
#	mips		dired for MIPS RC/6280 RISCos 2.1.1
#	next		dired for NeXT Mach 3.0
#	posix		dired for POSIX.1
#	sgi4		dired for Silicon Graphics IRIX 4.0.x
#	sgi		dired for Silicon Graphics IRIX 5.x
#	stdc		dired for ANSI/ISO Standard C
#	sun		dired for Sun 3, 386i, 4 SunOS 4.x
#	sun-gcc		dired for Sun 3, 386i, 4 SunOS 4.x with gcc
#	sun-solaris	dired for Sun Solaris 2.x
#	sun-solaris-gcc	dired for Sun Solaris 2.x with gcc
#	ultrix		dired for DECstation ULTRIX 4.x
#	xpg3		dired for XPG3
#
# [2-January-1997]
#=======================================================================
V                = 3.05
PORT             = dired
PROG             = dired
SRCS             = dired.c regexpr.c

#local pathname for installed version
CFLAGS          += -ffunction-sections -fdata-sections $(DEFINES) \
			-DMINFILES=$(MINFILES) \
			-DMOREPGM=\"$(MOREPGM)\"

DEFINES		= -DHAVE_TERMIOS_H -DHAVE_DIRENT_H -DHAVE_TERM_H

# A few systems need -lcurses too, but usually just -ltermlib suffices
LDFLAGS        += -Wl,--gc-sections

DPADD           = ${LIBTERMLIB}
LDADD           = -ltermlib

MINFILES        = 100

# less is better than most alternatives:
MOREPGM         = /usr/bin/more

.include <bsd.port.mk>
