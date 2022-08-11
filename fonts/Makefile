CFLAGS=-ggdb3 -O0 -fsanitize=address
CPPFLAGS=-I. -W -Wall -Wno-unused-parameter -Wno-sign-compare
LDFLAGS=$(CFLAGS)
LDLIBS=

.PHONY: clean test

all: afm2add afmclient

#	-sort --version-sort --check --field-separator="=" --key=2 charmap.ini

# I didn't write this code.
afmclient.o parseafm.o: CPPFLAGS+=-w

afmclient: parseafm.o

afm2add: parseafm.o kerning.o spacing.o util.o typeface.o mapfile.o addall.o ini.o charmap.o

clean:
	rm -f *.o a.out
	rm -f glyphs.h
	rm -f afm2add afmclient
	rm -f *.TYP *.KRN *.MAP *.SPC *.ADD
