CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -pedantic -Werror
CPPFLAGS = -I.
LDFLAGS =

# -fno-sanitize-recover=all makes UBSan stop with a nonzero exit, so make memcheck fails
SANITIZE = -g -fsanitize=address,undefined -fno-sanitize-recover=all

all: tests/test_bst
.PHONY: all test memcheck clean

bst.o: bst.c bst.h
treestats.o: treestats.c bst.h
tests/test_bst.o: tests/test_bst.c bst.h

%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

treestats: treestats.o bst.o
	$(CC) $(CFLAGS) $(LDFLAGS) treestats.o bst.o -o treestats

tests/test_bst: tests/test_bst.o bst.o
	$(CC) $(CFLAGS) $(LDFLAGS) tests/test_bst.o bst.o -o tests/test_bst

# unit tests, then the regression test; either one failing stops make
test: tests/test_bst
	./tests/test_bst

# rebuild everything with the sanitizers, run the tests, then clean so the
# next plain make is not left with sanitized objects
memcheck:
	$(MAKE) clean
	$(MAKE) test CFLAGS="$(CFLAGS) $(SANITIZE)" LDFLAGS="$(SANITIZE)"
	$(MAKE) clean

clean:
	rm -f *.o tests/*.o tests/test_bst
