AC_PREREQ([2.59])
AC_INIT([dirutils], [1.0.1], [kyle@tyrfingr.is],
        [], [https://github.com/kisom/dirutils])
AM_INIT_AUTOMAKE([1.11 foreign no-define])
AC_CONFIG_SRCDIR([src/dirutils.h])
AC_CONFIG_FILES([Makefile src/Makefile tests/Makefile docs/Makefile])
AC_CHECK_HEADERS

AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_RANLIB

NO_CUNIT_MSG="
    ==============================================
    Warning: CUnit was not found; will not be able
    to run unit tests!
    ==============================================
"
AC_SEARCH_LIBS([CU_initialize_registry], [cunit],
               [], [AC_MSG_WARN($NO_CUNIT_MSG)])
AC_OUTPUT
