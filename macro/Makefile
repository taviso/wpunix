CFLAGS=-ggdb3 -O0
CPPFLAGS=-I. -W -Wall -Wno-unused-parameter -Wno-sign-compare
LDFLAGS=$(CFLAGS)
LDLIBS=

.PHONY: clean

%.wpm: %.mac | mactool
	cpp $^ | ./mactool -s -t "$(shell grep -oP '^#pragma title \K.*' $^)" -c -o $@

all: mactool $(patsubst %.mac,%.wpm,$(wildcard macros/*.mac))

mactool: decompile.o charset.o

clean:
	rm -f *.o mactool
	rm -f macros/*.wpm
