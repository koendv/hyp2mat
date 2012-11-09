# hyp2mat makefile

CC     = gcc
LEX    = flex 
YACC   = bison -y

SRCS=scan.l parse.y hyp2mat.c
OBJS=scan.o parse.o hyp2mat.o

hyp2mat: $(OBJS)
	$(CC) -o $@  $(LDFLAGS) $^

scan.o: scan.l parse.c
parse.o: parse.y
hyp2mat.o: hyp2mat.c

readme.txt: hyp2mat.1
	groff -man -Tascii hyp2mat.1 | col -b > readme.txt

clean:
	rm -f $(OBJS) scan.c parse.c parse.h hyp2mat matlab/tmp/tmp.xml

# not truncated
