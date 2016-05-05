
all: library
	make -C test

install: library
	install -o root -g root -m 644 src/ulog.h /usr/local/include
	install -o root -g root -m 644 src/ulog_internals.h /usr/local/include
	install -o root -g root -m 755 src/libulog.a /usr/local/lib

remove:
	rm -f /usr/local/include/ulog.h
	rm -f /usr/local/include/ulog_internals.h
	rm -f /usr/local/lib/libulog.a

library:
	make -C src

clean:
	make -C src clean
	make -C test clean
