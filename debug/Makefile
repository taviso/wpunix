ASFLAGS=--32 -gstabs
LDFLAGS=-m elf_i386 -shared
TARGET=wp
CFLAGS=-m32 -gstabs

.PHONY: clean objsize

all: $(TARGET:=.dbg)

wp:
	test -f /opt/wp80/wpbin/wp
	ln -s /opt/wp80/wpbin/wp wp

objsize: wp
	$(eval TEXT_addr := $(shell objdump -j.text -h $(TARGET) | awk '/\.text/ { print $$4 }'))
	$(eval TEXT_size := $(shell objdump -j.text -h $(TARGET) | awk '/\.text/ { print $$3 }'))
	$(eval DATA_addr := $(shell objdump -j.data -h $(TARGET) | awk '/\.data/ { print $$4 }'))
	$(eval DATA_size := $(shell objdump -j.data -h $(TARGET) | awk '/\.data/ { print $$3 }'))
	$(eval BSS_addr := $(shell objdump -j.bss -h $(TARGET) | awk '/\.bss/ { print $$4 }'))
	$(eval BSS_size := $(shell objdump -j.bss -h $(TARGET) | awk '/\.bss/ { print $$3 }'))

types.s: types.c
	$(CC) -c $(CFLAGS) -S -o $@ $<

basemap.s:
	@echo You may need to generate basemap.s with ida2stabs.py

%.o: ASFLAGS+=--defsym _textsize=0x$(TEXT_size)
%.o: ASFLAGS+=--defsym _datasize=0x$(DATA_size)
%.o: ASFLAGS+=--defsym _bsssize=0x$(BSS_size)
%.o: %.s | objsize basemap.s
	as $(ASFLAGS) -o $@ $<


%.dbg: %.o types.o | objsize
	ld $(LDFLAGS) -Tdata=$(DATA_addr) -Ttext=$(TEXT_addr) -Tbss=$(BSS_addr) -o $@ $^
	strip --only-keep-debug $@

clean:
	rm -f *.o
	rm -f *.dbg
	rm -f a.out
	rm -f types.s
	rm -f wp
