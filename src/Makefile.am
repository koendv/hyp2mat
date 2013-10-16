# used by automake

#
# hyp2mat command line program
#

bin_PROGRAMS=hyp2mat

hyp2mat_SOURCES=hyp2mat.cc cmdline.ggo cmdline.c cmdline.h
hyp2mat_LDADD=$(top_builddir)/lib/libhyp2mat.la
AM_CPPFLAGS= -I$(top_srcdir)/lib

# command line option parsing using gengetopt.
# create cmdline.c and cmdline.h from cmdline.ggo

cmdline.c cmdline.h: cmdline.ggo
	$(GENGETOPT) < $^

BUILT_SOURCES=cmdline.c cmdline.h

CLEANFILES=cmdline.c cmdline.h

#not truncated
