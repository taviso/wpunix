CFLAGS	= -O0 -ggdb3 -m32 -std=gnu99 -Wno-multichar -w -fno-stack-protector
CPPFLAGS= -DNDEBUG
LDFLAGS	= $(CFLAGS) -m32

all: hook.o
	make -C libdisasm all
	cp libdisasm/libdisasm.a .

clean:
	rm -f *.o *.a
	make -C libdisasm clean
