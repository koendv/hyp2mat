# Process this file with autoconf to produce a configure script.
AC_INIT([hyp2mat], [0.0.5])
AM_INIT_AUTOMAKE

AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_SRCDIR([src/hyp2mat.cc])
AC_CONFIG_HEADER(src/config.h)
AC_CONFIG_FILES([Makefile src/Makefile src/hyp2mat.pc matlab/Makefile eagle/Makefile])

# optionally install libraries and headers
AC_ARG_ENABLE([library],
AC_HELP_STRING([--enable-library], [install hyp2mat library and includes]),
[case "${enableval}" in
   yes) install_library=true ;;
   no)  install_library=false ;;
   *) AC_MSG_ERROR([bad value ${enableval} for --enable-library]) ;;
esac], [install_library=false])
AM_CONDITIONAL([INSTALL_LIBRARY], [test x$install_library = xtrue])

AC_LANG(C++)
AC_PROG_CXX
# Compile all C programs with g++.
# Bison and flex generate C source. 
# But because our parse.y contains C++ code, we need to compile parse.c and scan.c with g++
AC_PROG_CC([$CXX])
AM_PROG_LEX
AC_PROG_YACC
AC_DISABLE_SHARED
AC_PROG_LIBTOOL
AC_PATH_PROG(HELP2MAN, help2man, false // No help2man // )

# libhpdf is used for pdf output
# libhpdf needs -lhpdf -lpng -lz -lm
AC_CHECK_HEADER([hpdf.h], [],[AC_MSG_ERROR([hpdf.h include file not found])])
AC_CHECK_LIB([m], [sqrt])
AC_CHECK_LIB([z], [gzread], [], [AC_ERROR([libz library not found])])
AC_CHECK_LIB([png], [png_read_info])
AC_CHECK_LIB([hpdf], [HPDF_New], [], [AC_ERROR([libhpdf library not found])])

AC_OUTPUT
# not truncated
