CC      = gcc
CFLAGS  = -std=c11 -Wall -Wextra
LDLIBS  = -lm

OBJS = main.o sensor.o fsm.o store.o aux.o report.o iom361_r4.o

pump: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o pump $(LDLIBS)

main.o: main.c iom361_r4.h
sensor.o: sensor.c sensor.h iom361_r4.h
fsm.o: fsm.c fsm.h
store.o: store.c store.h
aux.o: aux.c aux.h
report.o: report.c report.h
iom361_r4.o: iom361_r4.c iom361_r4.h

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

tests/test_sensor.o: tests/test_sensor.c sensor.h iom361_r4.h
	$(CC) $(CFLAGS) -I. -c tests/test_sensor.c -o tests/test_sensor.o

tests/test_sensor: tests/test_sensor.o sensor.o iom361_r4.o
	$(CC) $(CFLAGS) tests/test_sensor.o sensor.o iom361_r4.o -o tests/test_sensor $(LDLIBS)

test: pump tests/test_sensor
	./tests/test_sensor

clean:
	rm -f *.o tests/*.o pump tests/test_sensor

.PHONY: test clean
