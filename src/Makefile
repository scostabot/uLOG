LIB=libulog.a

all: library

library: ulog.o
	$(AR) ruv $(LIB) ulog.o
	ranlib $(LIB)

ulog.o: ulog.h uloglib.h ulog_internals.h
	cp ulog_internals.h ulog.c
	$(CC) -g -DULOG_LIBC -c ulog.c -o ulog.o
	rm -f ulog.c

clean:
	rm -f ulog.o $(LIB)
