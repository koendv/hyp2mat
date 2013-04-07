# used by automake
ACLOCAL_AMFLAGS=-I m4
SUBDIRS=lib src doc matlab eagle

README: README.md
	tr -d '#[]' < $< > $@

EXTRA_DIST=README.md
