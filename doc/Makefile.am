# used by automake

# 
# man page and friends
#

dist_man_MANS=hyp2mat.1
dist_DATA=hyp2mat.pdf
distdir=$(pkgdatadir)

PROGNAME = $(top_builddir)/src/hyp2mat$(EXEEXT)

# create man page from --help output

if NO_HELP2MAN
hyp2mat.1: hyp2mat.h2m $(top_srcdir)/src/cmdline.ggo
	touch $@
else
hyp2mat.1: hyp2mat.h2m $(top_srcdir)/src/cmdline.ggo
	-$(HELP2MAN) --output=$@ --no-info --include=$< $(PROGNAME)
endif

#
# pdf man page, mainly for Windows users
#

if NO_GROFF
hyp2mat.pdf: hyp2mat.1
	touch $@
else 
if NO_PS2PDF
hyp2mat.pdf: hyp2mat.1
	touch $@
else
hyp2mat.pdf: hyp2mat.1
	-$(GROFF) -Tps -man hyp2mat.1 | $(PS2PDF) - hyp2mat.pdf
endif
endif

EXTRA_DIST=hyp2mat.h2m hyp2mat.pdf

CLEANFILES=hyp2mat.1 hyp2mat.pdf

#not truncated
