CC     = gcc
CFLAGS = -std=c11 -Wall -Wextra

OBJS = main.o reading.o stats.o histogram.o

readings: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o readings

main.o: main.c reading.h stats.h histogram.h
reading.o: reading.c reading.h
stats.o: stats.c stats.h
histogram.o: histogram.c histogram.h

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

tests/test_stats.o: tests/test_stats.c stats.h
	$(CC) $(CFLAGS) -I. -c tests/test_stats.c -o tests/test_stats.o

tests/test_stats: tests/test_stats.o stats.o
	$(CC) $(CFLAGS) tests/test_stats.o stats.o -o tests/test_stats

test: readings tests/test_stats
	./tests/test_stats
	sh regress.sh

clean:
	rm -f *.o tests/*.o readings tests/test_stats

.PHONY: test clean
