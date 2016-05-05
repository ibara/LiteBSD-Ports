AC_DEFUN([AC_LIB_FIRETALK],
[
	AC_PREREQ(2.56)
	AC_ARG_ENABLE(raw-irc-modes,
		[  --enable-raw-irc-modes  enable the display of raw mode changes on IRC [[default=no]]],
		[
			AC_DEFINE(RAWIRCMODES, 1, [Define to enable the display of raw mode changes on IRC])
		]
	)
	AC_DEFINE(ENABLE_FT_LILY_CTCPMAGIC, 1, [Define to enable experimental CTCP magic support])
	AC_REQUIRE([AC_PROG_CC])
	AC_REQUIRE([AC_C_CONST])
	AC_REQUIRE([AC_PROG_RANLIB])
	AC_CHECK_HEADERS(inttypes.h stdint.h)
	AC_CHECK_TYPE(uint16_t,
		,
		AC_CHECK_TYPE(__uint16_t,
			AC_DEFINE(uint16_t, __uint16_t, [Define if you have __uint16_t instead of uint16_t]),
			AC_CHECK_TYPE(_G_uint16_t,
				AC_DEFINE(uint16_t, _G_uint16_t, [Define if you have _G_uint16_t instead of uint16_t]),
				AC_MSG_ERROR(unable to find C99 16 bit type)
			)
		)
	)
	AC_CHECK_TYPE(uint32_t,
		,
		AC_CHECK_TYPE(__uint32_t,
			AC_DEFINE(uint32_t, __uint32_t, [Define if you have __uint32_t instead of uint32_t]),
			AC_CHECK_TYPE(_G_uint32_t,
				AC_DEFINE(uint32_t, _G_uint32_t, [Define if you have _G_uint32_t instead of uint32_t]),
				AC_MSG_ERROR(unable to find C99 32 bit type)
			)
		)
	)
	AC_CHECK_FUNCS(getuid getpwuid inet_aton)
])# AC_LIB_FIRETALK
