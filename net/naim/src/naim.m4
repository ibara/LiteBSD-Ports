AC_DEFUN([AC_PROG_NAIM],
[
	AC_PREREQ(2.50)

	AC_SUBST([pkgmoddir], ['${libdir}/naim'])
	AC_ARG_WITH(pkgmoddir,
		AC_HELP_STRING([--with-pkgmoddir=DIR], [package modules [[LIBDIR/naim]]]),
		[
			pkgmoddir=${withval}
		]
	)

	AC_SUBST([pkgincludedir], ['${includedir}/naim'])
	AC_ARG_WITH(pkgincludedir,
		AC_HELP_STRING([--with-pkgincludedir=DIR], [package header files [[INCLUDEDIR/naim]]]),
		[
			pkgincludedir=${withval}
		]
	)

	AC_SUBST([pkgtarball], ['${PACKAGE_TARNAME}-${PACKAGE_VERSION}-${host}.tbz2'])
	AC_ARG_WITH(pkgtarball,
		AC_HELP_STRING([--with-pkgtarball=FILE], [binary tarball file [[naim-VERSION-HOST.tbz2]]]),
		[
			pkgtarball=${withval}
		]
	)

	AC_ARG_ENABLE(debug,
		AC_HELP_STRING([--enable-debug], [add debugging to CFLAGS and enable UI debugging]),
		[
			if test "x${enable_debug}" = xyes; then
				CFLAGS="$CFLAGS -g3"
				AC_DEFINE(DEBUG_ECHO, 1, [Define to enable debugging messages])
			fi
		]
	)

	AC_ARG_ENABLE(profile,
		AC_HELP_STRING([--enable-profile], [add profiling to CFLAGS and search for profilable ncurses]),
		[
			if test "x${enable_profile}" = xyes; then
				CFLAGS="$CFLAGS -pg"
				PROFILESUF="_p"
			fi
		]
	)

	AC_MSG_CHECKING(whether to enable /detach)
	AC_ARG_ENABLE(detach,
		AC_HELP_STRING([--enable-detach], [enable /detach command]),
		[
			if test "x${enable_detach}" = xyes; then
				AC_MSG_RESULT(yes)
				AC_DEFINE(ALLOW_DETACH, 1, [Define to enable the /detach command])
			else
				AC_MSG_RESULT([no, you explicitly disabled it])
			fi
		], [
			AC_MSG_RESULT([no, re-run ./configure with --enable-detach])
		]
	)

	AC_ARG_ENABLE(forceascii,
		AC_HELP_STRING([--enable-forceascii], [enable the $forceascii setting to force alternate line drawing characters]),
		[
			if test "x${enable_forceascii}" = xyes; then
				AC_DEFINE(ENABLE_FORCEASCII, 1, [Define to enable the $forceascii setting to force alternate line drawing characters])
			fi
		]
	)

	AC_ARG_ENABLE(dnsupdate,
		AC_HELP_STRING([--disable-dnsupdate], [DO NOT allow naim to check for newer versions of itself using DNS])
	)

	AC_ARG_WITH(update-message,
		AC_HELP_STRING([--with-update-message=MESSAGE], [message to display when an update is available]),
		[
			if test "x${with_update_message}" != xyes && test "x${with_update_message}" != xno; then
				enable_dnsupdate=yes
				AC_DEFINE_UNQUOTED(DNSUPDATE_MESSAGE, "${with_update_message}", [Define to the message to display when an update is available])
			fi
		]
	)
	if test "x${enable_dnsupdate}" != xno; then
		AC_DEFINE(ENABLE_DNSUPDATE, 1, [Define to allow naim to check for newer versions of itself using DNS])
	fi

	AC_ARG_WITH(irc-support-channel,
		AC_HELP_STRING([--with-irc-support-channel=CHANNEL], [default channel to join when started as `nirc' [[#naim]]]),
		[
			if test "x${with_irc_support_channel}" != xyes && test "x${with_irc_support_channel}" != xno; then
				AC_DEFINE_UNQUOTED(IRC_SUPPORT_CHANNEL, "${with_irc_support_channel}", [Define to the default channel to join when started as `nirc'])
			fi
		]
	)

	AC_REQUIRE([AC_CANONICAL_HOST])
	AC_MSG_CHECKING([for Cygwin])
	case $host_os in
		*cygwin*)
			AC_MSG_RESULT(yes)
			AC_SUBST([cygwindocdir], ['${datadir}/doc/Cygwin'])
			AC_SUBST([pkgdocdir], ['${datadir}/doc/${PACKAGE_TARNAME}-${PACKAGE_VERSION}'])
			AC_DEFINE(FAKE_MAIN_STUB, 1, [Define to enable a workaround on Windows for module loading])
			AC_DEFINE(DLOPEN_SELF_LIBNAIM_CORE, 1, [Define to dlopen libnaim_core rather than NULL])
			AM_CONDITIONAL(CYGWIN, true)
			;;
		*)
			AC_MSG_RESULT(no)
			AC_SUBST([cygwindocdir], [''])
			AC_SUBST([pkgdocdir], ['${datadir}/doc/${PACKAGE_TARNAME}'])
			AM_CONDITIONAL(CYGWIN, false)
			;;
	esac

	AC_ARG_WITH(pkgdocdir,
		AC_HELP_STRING([--with-pkgdocdir], [package documentation [[PREFIX/doc/naim-VERSION]]]),
		[
			pkgdocdir=${withval}
		]
	)

	AC_ARG_WITH(cygwindocdir,
		AC_HELP_STRING([--with-cygwindocdir], [Cygwin-specific package documentation [[PREFIX/doc/Cygwin]]]),
		[
			cygwindocdir=${withval}
		]
	)
	AM_CONDITIONAL(INSTALLCYGWINDOC, test x$cygwindocdir != x)

	AC_REQUIRE([AC_PROG_AWK])
	AC_REQUIRE([AC_PROG_CC])
	AC_REQUIRE([AC_PROG_CPP])
	AC_REQUIRE([AC_C_CONST])
	AC_REQUIRE([AC_HEADER_STDC])
	AC_CHECK_HEADERS(errno.h sys/errno.h)
	AC_CHECK_HEADERS(inttypes.h stdint.h)
	AC_CHECK_HEADERS(stdlib.h)
	AC_CHECK_HEADERS(sys/stat.h)
	AC_CHECK_HEADERS(dirent.h)
	AC_CHECK_HEADERS(sys/types.h)
	AC_CHECK_HEADERS(time.h sys/time.h)
	AC_REQUIRE([AC_HEADER_TIME])
	AC_REQUIRE([AC_STRUCT_TIMEZONE])
	AC_CHECK_MEMBERS([struct tm.tm_gmtoff], [
		], [
			AC_MSG_CHECKING([for extern long int timezone])
			AC_TRY_LINK([
#					include <time.h>

					extern long int timezone;
				], [
					timezone = 0;
				], [
					AC_DEFINE(HAVE_TIMEZONE, 1, [Define if you have extern long int timezone])
					AC_MSG_RESULT(yes)
				], [
					AC_MSG_RESULT(no)
				]
			)
		], [
#			include <sys/types.h>
#			include <$ac_cv_struct_tm>
		]
	)

	AC_REQUIRE([AC_FUNC_FORK])

	NETLIB=
	for func in inet_ntoa gethostbyname gethostbyaddr socket connect; do
		AC_CHECK_FUNC($func, [
				NETLIB="${NETLIB}"
			], [
				AC_CHECK_LIB(dns, $func,
					NETLIB="${NETLIB} -ldns",
					AC_CHECK_LIB(bind, $func,
						NETLIB="${NETLIB} -lbind",
						AC_CHECK_LIB(resolv, $func,
							NETLIB="${NETLIB} -lresolv",
							AC_CHECK_LIB(nsl, $func,
								NETLIB="${NETLIB} -lnsl",
								AC_CHECK_LIB(socket, $func,
									NETLIB="${NETLIB} -lsocket",
									AC_MSG_ERROR(unable to find $func)
								)
							)
						)
					)
				)
			])
	done
	AC_SUBST(NETLIB)

	AC_CHECK_LIB(ncurses${PROFILESUF}, wresize, [
			AC_CHECK_LIB(ncurses${PROFILESUF}, use_default_colors, AC_DEFINE(HAVE_USE_DEFAULT_COLORS, 1, [Define if you have use_default_colors()]))
			AC_CHECK_HEADERS(ncurses.h)
			CURSESLIB="-lncurses${PROFILESUF}"
		], [
			AC_CHECK_LIB(curses${PROFILESUF}, wresize, [
					AC_CHECK_LIB(curses${PROFILESUF}, use_default_colors, AC_DEFINE(HAVE_USE_DEFAULT_COLORS, 1, [Define if you have use_default_colors()]))
					AC_CHECK_HEADERS(curses.h)
					CURSESLIB="-lcurses${PROFILESUF}"
				], [
					AC_MSG_ERROR(unable to find a curses library -- FATAL)
				]
			)
		]
	)
	AC_SUBST(CURSESLIB)

	AC_CHECK_FUNC(backtrace, [
			AC_CHECK_FUNC(backtrace_symbols_fd, [
					AC_CHECK_HEADER(execinfo.h, [
							AC_DEFINE(HAVE_BACKTRACE, 1, [Define if you have backtrace() and backtrace_symbols(), a glibc extension])
							AC_DEFINE(HAVE_EXECINFO_H, 1, [Define if you have execinfo.h, a glibc extension])
					])
			])
	])

	AC_CHECK_FUNC(getopt_long, [
			AC_CHECK_HEADER(getopt.h, [
					AC_DEFINE(HAVE_GETOPT_LONG, 1, [Define if you have getopt_long() and getopt.h])
			])
	])

	AC_CHECK_FUNCS(hstrerror strverscmp waitpid)
])# AC_PROG_NAIM
