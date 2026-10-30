CC     = gcc
CFLAGS = -std=c11 -Wall -Wextra

all: lock

lock: main.o lock.o
	$(CC) $(CFLAGS) main.o lock.o -o lock

main.o: main.c lock.h
	$(CC) $(CFLAGS) -c main.c -o main.o

lock.o: lock.c lock.h
	$(CC) $(CFLAGS) -c lock.c -o lock.o

tests/test_lock.o: tests/test_lock.c lock.h
	$(CC) $(CFLAGS) -I. -c tests/test_lock.c -o tests/test_lock.o

tests/test_lock: tests/test_lock.o lock.o
	$(CC) $(CFLAGS) tests/test_lock.o lock.o -o tests/test_lock

check.o: check.c check.h lock.h
	$(CC) $(CFLAGS) -c check.c -o check.o

tests/test_check.o: tests/test_check.c lock.h check.h
	$(CC) $(CFLAGS) -I. -c tests/test_check.c -o tests/test_check.o

tests/test_check: tests/test_check.o check.o lock.o
	$(CC) $(CFLAGS) tests/test_check.o check.o lock.o -o tests/test_check

test: tests/test_lock
	./tests/test_lock

bonus: tests/test_check
	./tests/test_check

run: lock
	./lock < data/codes.txt

clean:
	rm -f *.o lock tests/*.o tests/test_lock tests/test_check

.PHONY: all test bonus run clean
