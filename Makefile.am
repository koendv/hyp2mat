# used by automake
ACLOCAL_AMFLAGS=-I m4
SUBDIRS=lib src doc matlab eagle

README: README.md
	tr -d '#[]' < $< > $@

EXTRA_DIST=README.md

# create a git archive without generated files
tarball:
	git archive --format=tar --prefix=hyp2mat-@PACKAGE_VERSION@/ HEAD | bzip2 > hyp2mat-@PACKAGE_VERSION@.tar.bz2

# create a tarball with all generated files inside
tarball-dist: all
	$(MAKE) -C doc
	git archive --format=tar --prefix=hyp2mat-@PACKAGE_VERSION@/ HEAD > hyp2mat-@PACKAGE_VERSION@-dist.tar
	echo configure > .tar-files
	echo doc/hyp2mat.pdf >> .tar-files
	echo doc/hyp2mat.1 >> .tar-files
	echo lib/parse.cc >> .tar-files
	echo lib/parse.h >> .tar-files
	echo lib/scan.cc >> .tar-files
	echo src/cmdline.c >> .tar-files
	echo src/cmdline.h >> .tar-files
	echo aclocal.m4 >> .tar-files
	echo libtool >> .tar-files
	find . -name '*.in' >> .tar-files
	find . -path './config/*' -or -path './m4/*' >> .tar-files
	tar --append -f hyp2mat-@PACKAGE_VERSION@-dist.tar --dereference --transform 's,^,hyp2mat-@PACKAGE_VERSION@/,' --files-from .tar-files
	bzip2 -f hyp2mat-@PACKAGE_VERSION@-dist.tar
	rm .tar-files

.PHONY: tarball tarball-dist
