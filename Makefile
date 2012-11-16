# hyp2mat makefile

VERSION = 0.0.2

CC     = gcc
LEX    = flex 
YACC   = bison -y

SRCS=scan.l parse.y hyp2mat.c
OBJS=scan.o parse.o hyp2mat.o

.PHONY: clean tarball install

hyp2mat: $(OBJS)
	$(CC) -o $@  $(LDFLAGS) $^

scan.o: scan.l parse.c
parse.o: parse.y
hyp2mat.o: hyp2mat.c

readme.txt: hyp2mat.1
	groff -man -Tascii hyp2mat.1 | col -b > readme.txt

clean:
	rm -f $(OBJS) scan.c parse.c parse.h hyp2mat matlab/tmp/tmp.xml

tarball:
	git archive --format=tar --prefix=hyp2mat-$(VERSION)/ HEAD | bzip2 > hyp2mat-$(VERSION).tar.bz2

install: hyp2mat
	install -d "$(DESTDIR)/usr/bin" "$(DESTDIR)/usr/share/hyp2mat/matlab"
	install -d "$(DESTDIR)/usr/share/man/man1"
	install hyp2mat "$(DESTDIR)/usr/bin"
	install --mode=u=rwX,go=rX matlab/*.m matlab/README.txt "$(DESTDIR)/usr/share/hyp2mat/matlab"
	install --mode=u=rwX,go=rX hyp2mat.1 "$(DESTDIR)/usr/share/man/man1"

# not truncated
