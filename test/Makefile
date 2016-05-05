
OLD_CFLAGS=$(CFLAGS)
CFLAGS+=-Wall -g -I../src
LIB=../src/libulog.a
OBJ_LIB=file1_a.o lib1_a.o main_a.o
OBJ_INLINE=file1_h.o lib1_h.o main_h.o
ULOGSRC=../src/ulog.h

INCLUDECOMP=$(CC) -c $(CFLAGS) $< -o $@
LIBCOMP=$(CC) -c $(CFLAGS) -DULOG_LIB $< -o $@

all: test

file1_a.o: file1.c function1.h $(ULOGSRC)
	$(LIBCOMP)

lib1_a.o: lib1.c lib1.h $(ULOGSRC)
	$(LIBCOMP)

main_a.o: main.c function1.h $(ULOGSRC)
	$(LIBCOMP)

file1_h.o: file1.c function1.h $(ULOGSRC)
	$(INCLUDECOMP)

lib1_h.o: lib1.c lib1.h $(ULOGSRC)
	$(INCLUDECOMP)

main_h.o: main.c function1.h $(ULOGSRC)
	$(INCLUDECOMP)

testuloga: $(OBJ_LIB) $(LIB)
	$(CC) $(OBJ_LIB) -L../src -lulog -o testuloga

testulogh: $(OBJ_INLINE) $(LIB)
	$(CC) $(OBJ_INLINE) -o testulogh

run_testuloga: testuloga
	./testuloga 1 2> ./test.stderr
	./testuloga 2 1> ./test.stdout
	rm -f /tmp/trace;./testuloga 3
	./testuloga 4
	./testuloga 5
	./testuloga 6
	./testuloga 7
	./testuloga 8
	./testuloga 9
	./testuloga 10

run_testulogh: testulogh
	./testulogh 1 2> ./test.stderr
	./testulogh 2 1> ./test.stdout
	rm -f /tmp/trace;./testulogh 3
	./testulogh 4
	./testulogh 5
	./testulogh 6
	./testulogh 7
	./testulogh 8
	./testulogh 9
	./testulogh 10

test: run_testuloga run_testulogh

clean:
	rm -f *.o testulogh testuloga /tmp/trace ./test.stderr ./test.stdout
