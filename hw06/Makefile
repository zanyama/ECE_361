CC = gcc
CFLAGS = -std=c11 -Wall -Wextra
LDLIBS = -lm
OBJS = main.o scenario.o sensor.o panel.o iom361_r4.o
all: replay
clean:
	rm -f *.o tests/*.o replay tests/test_hw06
.PHONY: all clean test

main.o: main.c iom361_r4.h scenario.h sensor.h panel.h
scenario.o: scenario.c scenario.h
sensor.o: sensor.c sensor.h iom361_r4.h
panel.o: panel.c panel.h
iom361_r4.o: iom361_r4.c iom361_r4.h

replay: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o replay $(LDLIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

test: replay tests/test_hw06
	./tests/test_hw06
	sh regress.sh

TEST_OBJS = panel.o sensor.o iom361_r4.o scenario.o

tests/test_hw06.o: tests/test_hw06.c panel.h sensor.h iom361_r4.h scenario.h
	$(CC) $(CFLAGS) -I. -c tests/test_hw06.c -o tests/test_hw06.o

tests/test_hw06: tests/test_hw06.o $(TEST_OBJS)
	$(CC) $(CFLAGS) tests/test_hw06.o $(TEST_OBJS) -o tests/test_hw06 $(LDLIBS)
