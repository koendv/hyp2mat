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

# hyp2mat flags
AM_CXXFLAGS=@GTK_CFLAGS@ @CAIRO_CFLAGS@ -Igerbv -Iclipper
# libgerbv flags. Switch off debugging in libgerbv (-DDEBUG=0)
AM_CFLAGS=@GTK_CFLAGS@ @CAIRO_CFLAGS@ -Igerbv -Iclipper -DDEBUG=0

AM_LDFLAGS=@GTK_LIBS@ @CAIRO_LIBS@

AUTOMAKE_OPTIONS = subdir-objects

libhyp2mat_la_SOURCES=\
  gerbv/amacro.c \
  gerbv/amacro.h \
  gerbv/common.h \
  gerbv/csv.c \
  gerbv/csv_defines.h \
  gerbv/csv.h \
  gerbv/draw.c \
  gerbv/draw-gdk.c \
  gerbv/draw-gdk.h \
  gerbv/draw.h \
  gerbv/drill.c \
  gerbv/drill.h \
  gerbv/drill_stats.c \
  gerbv/drill_stats.h \
  gerbv/export-drill.c \
  gerbv/exportimage.c \
  gerbv/export-rs274x.c \
  gerbv/gerber.c \
  gerbv/gerber.h \
  gerbv/gerb_file.c \
  gerbv/gerb_file.h \
  gerbv/gerb_image.c \
  gerbv/gerb_image.h \
  gerbv/gerb_stats.c \
  gerbv/gerb_stats.h \
  gerbv/gerbv.c \
  gerbv/gerbv.h \
  gerbv/gerbv_icon.h \
  gerbv/pick-and-place.c \
  gerbv/pick-and-place.h \
  gerbv/tooltable.c \
  clipper/cairo_clipper.cpp \
  clipper/cairo_clipper.hpp \
  clipper/clipper.cpp \
  clipper/clipper.hpp \
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
  import_gerber.cc \
  import_gerber.h \
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
  scan.ll

EXTRA_DIST=clipper.txt parse.h

BUILT_SOURCES=parse.cc parse.h scan.cc

scan.cc: scan.ll parse.h
	${LEX} -o $*.cc $<

parse.h parse.cc: parse.yy
	${YACC} --output=$*.cc --defines=$*.h $<

CLEANFILES=parse.cc parse.h scan.cc

#not truncated
