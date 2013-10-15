#!/bin/sh

mkdir config 2> /dev/null

libtoolize -i || exit 1
aclocal || exit 1
automake --gnu --add-missing || exit 1
autoheader configure.ac || exit 1
autoconf
