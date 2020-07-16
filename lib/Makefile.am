# used by automake

#
# hyp2mat library
#

# if you wish to install libraries, headers, and package information
if INSTALL_LIBRARY
include_HEADERS=hyp2mat.h
lib_LTLIBRARIES=libhyp2mat.la
pkgconfig_DATA=hyp2mat.pc
else
# if you do not wish to install libraries, headers, and package information
noinst_HEADERS=hyp2mat.h
noinst_LTLIBRARIES=libhyp2mat.la
noinst_DATA=hyp2mat.pc
endif

pkgconfigdir=$(datarootdir)/pkgconfig

libhyp2mat_la_SOURCES=\
  clipper.cpp \
  clipper.hpp \
  copper.cc \
  crop.h \
  crop.cc \
  csxcad.cc \
  csxcad.h \
  draw.cc \
  exec_board.cc \
  exec_devices.cc \
  exec_end.cc \
  exec_net.cc \
  exec_netclass.cc \
  exec_padstack.cc \
  exec_polygon.cc \
  exec_stackup.cc \
  exec_supplies.cc \
  hyp2mat.h \
  hyperlynx.cc \
  hyperlynx.h \
  hypfile.cc \
  hypfile.h \
  misc.cc \
  palette.cc \
  palette.h \
  parse_param.h \
  parser.h \
  parse.yy \
  polygon.h \
  polygon.cc \
  pcb.cc \
  pdf.cc \
  pdf.h \
  qucsstudio.h \
  qucsstudio.cc \
  scan.ll

EXTRA_DIST=clipper.txt parse.h

BUILT_SOURCES=parse.cc parse.h scan.cc

scan.cc: scan.ll parse.h
	${LEX} -o $*.cc $<

parse.h parse.cc: parse.yy
	${YACC} --output=$*.cc --defines=$*.h $<

CLEANFILES=parse.cc parse.h scan.cc

#not truncated
