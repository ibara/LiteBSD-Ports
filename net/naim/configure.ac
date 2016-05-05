AC_PREREQ(2.50)
AC_INIT(naim, 0.11.8.3.2, [Daniel Reed <n@ml.org>])
AC_CONFIG_SRCDIR([src/naim-int.h])
AC_CONFIG_HEADER([src/config.h])
AM_INIT_AUTOMAKE([dist-bzip2 no-dist-gzip no-dependencies no-define subdir-objects std-options 1.8])
AM_PROG_CC_C_O

echo ""
echo ""
echo "Configuring firetalk"
echo ""
AC_LIB_FIRETALK


echo ""
echo ""
echo "Configuring libtool"
echo ""
AC_LIBLTDL_CONVENIENCE
AC_LIBTOOL_DLOPEN
AC_LIBTOOL_WIN32_DLL
AC_PROG_LIBTOOL
AC_SUBST([INCLTDL])
AC_SUBST([LIBLTDL])


echo ""
echo ""
echo "Configuring ltdl"
echo ""
AC_LIB_LTDL


echo ""
echo ""
echo "Configuring naim"
echo ""
AC_PROG_NAIM


echo ""
echo ""

test "$infodir" != '${prefix}/info' || infodir='${datadir}/info'
test "$mandir" != '${prefix}/man' || mandir='${datadir}/man'

AC_CONFIG_FILES([
Makefile
libfiretalk/Makefile
libltdl/Makefile
secs/Makefile
src/Makefile
CYGWIN-PATCHES/Makefile
])
AC_OUTPUT

echo ""
echo "Building ${PACKAGE_STRING} using ${CC}"
echo "       Compile flags:" ${CPPFLAGS} ${CFLAGS}
echo "          Link flags:" ${CFLAGS} ${LDFLAGS} ${LIBS}
echo ""
echo "Configuration results:"
printf "  Packaging features:"
if test "x${enable_debug}" = xyes;	then printf " --enable-debug";		else printf " --disable-debug"; fi
if test "x${enable_profile}" = xyes;	then printf " --enable-profile";	else printf " --disable-profile"; fi
if test "x${enable_detach}" = xyes;	then printf " --enable-detach";		else printf " --disable-detach"; fi
echo ""

echo ""
echo "Installation paths:"
echo "               --bindir:" `eval eval eval eval eval echo ${bindir}`
echo "       --with-pkgmoddir:" `eval eval eval eval eval echo ${pkgmoddir}`
echo "           --includedir:" `eval eval eval eval eval echo ${includedir}`
echo "   --with-pkgincludedir:" `eval eval eval eval eval echo ${pkgincludedir}`
echo "               --mandir:" `eval eval eval eval eval echo ${mandir}`
echo "       --with-pkgdocdir:" `eval eval eval eval eval echo ${pkgdocdir}`
if test "x${cygwindocdir}" = x; then echo " --without-cygwindocdir"; else echo "    --with-cygwindocdir:" `eval eval eval eval eval echo ${cygwindocdir}`; fi
echo "      --with-pkgtarball:" `eval eval eval eval eval echo ${pkgtarball}`
echo ""
