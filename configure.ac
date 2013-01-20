AC_PREREQ([2.68])
AC_INIT([code128], [1.0], [fhunleth@troodon-software.com])
AC_CONFIG_SRCDIR([code128.h])
AC_CONFIG_HEADERS([config.h])

AM_INIT_AUTOMAKE

# Checks for programs.
AC_PROG_CC

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([stddef.h stdlib.h string.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_PID_T
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_FORK
AC_FUNC_REALLOC
AC_CHECK_FUNCS([memset mkdir strcasecmp strdup])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
