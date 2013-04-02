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
  csxcad.cc \
  csxcad.h \
  draw.cc \
  edge.cc \
  exec_board.cc \
  exec_devices.cc \
  exec_end.cc \
  exec_net.cc \
  exec_netclass.cc \
  exec_padstack.cc \
  exec_polygon.cc \
  exec_stackup.cc \
  exec_supplies.cc \
  geometry.cc \
  geometry.h \
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
  parse.y \
  pcb.cc \
  pdf.cc \
  pdf.h \
  scan.l

#
# hyp2mat command line program
#

bin_PROGRAMS=hyp2mat

hyp2mat_SOURCES=hyp2mat.cc cmdline.ggo cmdline.c cmdline.h
hyp2mat_LDADD=libhyp2mat.la
dist_man_MANS=hyp2mat.1

# command line option parsing using gengetopt.

# create cmdline.c and cmdline.h from cmdline.ggo
BUILT_SOURCES=cmdline.c cmdline.h
cmdline.c cmdline.h: cmdline.ggo
	gengetopt < $^

# create man page from --help output
hyp2mat.1: hyp2mat.h2m cmdline.ggo hyp2mat.cc
	-$(HELP2MAN) --output=$@ --no-info --include=$< $(CURDIR)/hyp2mat

EXTRA_DIST=parse.h hyp2mat.h2m clipper.txt

#not truncated
