AC_PREREQ(2.64)
AC_INIT([diptych], [x.x.x], [https://github.com/whatdoineed2do/oflkr/issues], [https://github.com/whatdoineed2do/oflkr], [whatdoineed2do@nospam-github.com])

AM_INIT_AUTOMAKE([dist-bzip2 no-dist-gzip])
AC_CONFIG_MACRO_DIRS([m4])
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])
AC_CONFIG_SRCDIR([src])
AC_CONFIG_HEADERS([config.h])

AC_PROG_CC
AC_PROG_CXX
AM_PROG_CC_C_O
AC_HEADER_STDC
AC_PROG_INSTALL
PKG_PROG_PKG_CONFIG

AX_CXX_COMPILE_STDCXX([14], [noext], [mandatory])

PKG_CHECK_MODULES(LIBCONFIG, [libconfig++ >= 1.5])
PKG_CHECK_MODULES(IMAGEMAGICK, [Magick++ >= 6.9.9])
PKG_CHECK_MODULES(EXIV, [exiv2 >= 0.25])
PKG_CHECK_MODULES(SQLITE3, [sqlite3 >= 3.22.0])
PKG_CHECK_MODULES(LOG4CPP, [log4cpp >= 1.1.1])

AC_LANG_PUSH([C++])

AC_CHECK_HEADERS([restbed], [], [AC_MSG_ERROR([[Missing required header: <restbed>]])])
AC_SEARCH_LIBS([_ZN7restbed7ServiceC1Ev], [restbed],
        [AC_DEFINE([HAVE_RESTBED], [1]) RESTBED_LIBS="-lrestbed"],
        [AC_MSG_ERROR([[Missing restbed library is required]])])

AC_CHECK_HEADERS([nlohmann/json.hpp], [], [AC_MSG_ERROR([[Missing required header: nlohmann/json.hpp]])])


AC_ARG_ENABLE([debug-code],
	[AS_HELP_STRING([--enable-debug],
	[enable debug (default is no)])],
	[no_debug=false],
	[no_debug=true])
if test "$no_debug" = true; then
        building_debug=no
        AC_DEFINE(NDEBUG,1,[debug code])
else
        building_debug=yes
        AC_DEFINE(DEBUG,1,[debug code])
fi


AC_OUTPUT([
Makefile
src/Makefile
])
