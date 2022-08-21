CFLAGS=-ggdb3 -O0
CPPFLAGS=-I. -W -Wall -Wno-unused-parameter -Wno-sign-compare
LDFLAGS=$(CFLAGS)
LDLIBS=

.PHONY: clean test

all: mactool

mactool: decompile.o charset.o

clean:
	rm -f *.o mactool
