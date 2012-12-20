# -*- coding: utf-8 -*-
MKOCTFILE ?= mkoctfile

#FLAGS = -mex -Wall -Wextra
FLAGS = -mex

all: poly_boolmex.mex poly_iscwmex.mex

poly_boolmex.mex: poly_boolmex.cpp clipper.cpp 
	$(MKOCTFILE) $^ $(FLAGS) -v  -I.

poly_iscwmex.mex: poly_iscwmex.c
	$(MKOCTFILE) $^ $(FLAGS) -v  -I.

clean:
	rm -rf *.oct *.mex *.o *~
