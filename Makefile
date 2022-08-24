CPPFLAGS=-Iinclude -Iintercept
CFLAGS=-fno-stack-protector -m32

.PHONY: clean

all: wordperfect8_i386.deb

%_i386.deb: %_i386
	dpkg-deb --build --root-owner-group $^
	@ls -l $@

wordperfect8_i386: libcompat.so libwppatch.so
	bash build8.sh $@

wordperfect7_i386: libcompat.so libwppatch.so
	bash build7.sh $@

libcompat.so: compat.o
	ld -m elf_i386 -shared -o $@ $<

intercept/hook.o intercept/libdisasm.a:
	make -C intercept


libwppatch.so: patch.o intercept/hook.o intercept/libdisasm.a
	ld -m elf_i386 -shared -o $@ $^

clean:
	rm -f *.o *.deb *.so
	rm -rf build wordperfect?_i386
	rm -rf lib
	make -C intercept clean
