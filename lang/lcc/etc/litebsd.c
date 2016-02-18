/* Little endian MIPS on LiteBSD */

#include <string.h>

static char rcsid[] = "$Id$";

#ifndef LCCDIR
#define LCCDIR "/usr/local/libexec/lcc/"
#endif
#ifndef LIBDIR
#define LIBDIR "/usr/lib/"
#endif

char *suffixes[] = { ".c", ".i", ".s", ".o", ".out", 0 };
char inputs[256] = "";
char *cpp[] = { "/usr/libexec/cpp", "-D__STDC__=1",
	"-DLANGUAGE_C",
	"-DMIPSEL",
	"-D__LITEBSD__",
	"-D_CFE",
	"-D_LANGUAGE_C",
	"-D_MIPSEL",
	"-D_MIPS_FPSET=16",
	"-D_MIPS_ISA=_MIPS_ISA_MIPS1",
	"-D_MIPS_SIM=_MIPS_SIM_ABI32",
	"-D_MIPS_SZINT=32",
	"-D_MIPS_SZLONG=32",
	"-D_MIPS_SZPTR=32",
	"-D__host_mips",
	"-D__mips=1",
	"-D__litebsd",
	"-D__bsd",
	"-D__unix",
	"-Dhost_mips",
	"-Dmips",
	"-Dunix",
	"$1", "$2", "$3", 0 };
char *com[] =  { LCCDIR "rcc", "-target=mips/litebsd", "$1", "$2", "$3", "", 0 };
char *include[] = { "-I" LCCDIR "include", "-I/usr/local/include",
	"-I/usr/include", 0 };
char *as[] = { "/usr/bin/as", "-o", "$3", "$1", "$2", 0 };
char *ld[] = { "/usr/bin/ld", "-o", "$3", LIBDIR "crt0.o", "$1", "$2", "", "-L" LIBDIR, "-lc", "-llcc", 0 };

extern char *concat(char *, char *);

int option(char *arg) {
	if (strncmp(arg, "-lccdir=", 8) == 0) {
		cpp[0] = concat(&arg[8], "/cpp");
		include[0] = concat("-I", concat(&arg[8], "/include"));
		com[0] = concat(&arg[8], "/rcc");
		ld[18] = concat("-L", &arg[8]);
	} else if (strcmp(arg, "-g") == 0)
		;
	else if (strcmp(arg, "-p") == 0)
		ld[13] = LIBDIR "mcrt1.o";
	else if (strcmp(arg, "-b") == 0)
		;
	else
		return 0;
	return 1;
}
