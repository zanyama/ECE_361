CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -pedantic
CPPFLAGS = -I.
LDFLAGS =

# -fno-sanitize-recover=all makes UBSan stop with a nonzero exit, so make memcheck fails
SANITIZE = -g -fsanitize=address,undefined -fno-sanitize-recover=all

all: balance
.PHONY: all test memcheck clean

istack.o: istack.c istack.h
balance.o: balance.c istack.h
tests/test_istack.o: tests/test_istack.c istack.h

%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

balance: balance.o istack.o
	$(CC) $(CFLAGS) $(LDFLAGS) balance.o istack.o -o balance

tests/test_istack: tests/test_istack.o istack.o
	$(CC) $(CFLAGS) $(LDFLAGS) tests/test_istack.o istack.o -o tests/test_istack

# unit tests, then the regression test; either one failing stops make
test: balance tests/test_istack
	./tests/test_istack
	sh regress.sh

# rebuild everything with the sanitizers, run the tests, then clean so the
# next plain make is not left with sanitized objects
memcheck:
	$(MAKE) clean
	$(MAKE) test CFLAGS="$(CFLAGS) $(SANITIZE)" LDFLAGS="$(SANITIZE)"
	$(MAKE) clean

clean:
	rm -f *.o tests/*.o balance tests/test_istack
