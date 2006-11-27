#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.61)
AC_INIT([libdisarm],[0.1],[jonlst@gmail.com])
AM_INIT_AUTOMAKE([dist-bzip2 -Wall])

AC_CONFIG_SRCDIR([src/libdisarm/disarm.h])
AC_CONFIG_HEADER([config.h])

AC_SUBST(LIBDISARM_VERSION_INFO, [0:0:0])

# Checks for programs.
AC_PROG_CC
AX_CFLAGS_GCC_OPTION([-Wswitch])

AC_PROG_LIBTOOL

# Checks for libraries.

# Checks for header files.
AC_HEADER_ASSERT
AC_CHECK_HEADERS([stdint.h stdlib.h sys/endian.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_C_BIGENDIAN
AC_SYS_LARGEFILE
AC_TYPE_OFF_T
AC_TYPE_SIZE_T
AC_TYPE_UINT32_T

# Checks for library functions.

AC_CONFIG_FILES([
	Makefile
	libdisarm.pc
])
AC_OUTPUT


echo "
 $PACKAGE_NAME $VERSION

    prefix:		${prefix}
    compiler:		${CC}
    cflags:		${CFLAGS}
"
