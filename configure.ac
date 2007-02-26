AC_PREREQ(2.60)
AC_INIT([libxdg], [0.0.1], mark.nevill@gmail.com)
AM_INIT_AUTOMAKE([-Wall foreign])
AC_ARG_VAR(HAS_DOXYGEN, "Is doxygen in PATH?")
# Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL
AC_CHECK_PROG([HAS_DOXYGEN], [doxygen], [yes])
# Checks for header files.
AC_HEADERS_STDC
AC_CHECK_HEADERS([stdlib.h])
# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([memset])

AC_CONFIG_HEADER([config.h])
AC_CONFIG_FILES([Makefile
                 include/Makefile
                 src/Makefile])
AC_OUTPUT
