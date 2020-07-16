# Process this file with autoconf to produce a configure script.
AC_INIT([hyp2mat], [0.0.18])
AC_CONFIG_AUX_DIR([config])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_SRCDIR([src/hyp2mat.cc])
AC_CONFIG_HEADER([src/config.h])
AC_CONFIG_FILES([Makefile src/Makefile lib/Makefile lib/hyp2mat.pc doc/Makefile matlab/Makefile eagle/Makefile])
AM_INIT_AUTOMAKE

# optionally install libraries and headers
AC_ARG_ENABLE([library],
AC_HELP_STRING([--enable-library], [install hyp2mat library and includes]),
[case "${enableval}" in
   yes) install_library=true ;;
   no)  install_library=false ;;
   *) AC_MSG_ERROR([bad value ${enableval} for --enable-library]) ;;
esac], [install_library=false])
AM_CONDITIONAL([INSTALL_LIBRARY], [test x$install_library = xtrue])

# For building documentation
#
AC_PATH_PROG(GENGETOPT, gengetopt)
AM_CONDITIONAL(NO_GENGETOPT, test -z "$ac_cv_path_GENGETOPT" )
AC_PATH_PROG(HELP2MAN, help2man)
AM_CONDITIONAL(NO_HELP2MAN, test -z "$ac_cv_path_HELP2MAN" )
AC_PATH_PROG(GROFF, groff)
AM_CONDITIONAL(NO_GROFF, test -z "$ac_cv_path_GROFF" )
AC_PATH_PROG(PS2PDF, ps2pdf)
AM_CONDITIONAL(NO_PS2PDF, test -z "$ac_cv_path_PS2PDF" )

AC_LANG([C++])
AC_PROG_CXX
AC_PROG_CC
AM_PROG_LEX
AC_PROG_YACC
AC_PROG_LIBTOOL
AC_DISABLE_SHARED

# libhpdf is used for pdf output
# libhpdf needs -lhpdf -lpng -lz -lm
AC_CHECK_HEADER([hpdf.h], [],[AC_MSG_ERROR([hpdf.h include file not found])])
AC_CHECK_LIB([m], [sqrt])
AC_CHECK_LIB([z], [deflate], [], [AC_ERROR([libz library not found])])
AC_CHECK_LIB([png], [png_read_info])
AC_CHECK_LIB([hpdf], [HPDF_New], [], [AC_ERROR([libhpdf library not found])])

AC_OUTPUT
# not truncated
