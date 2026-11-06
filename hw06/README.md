# HW6: reading the sensor and switches through registers

## What it does
`./replay scenario.txt` plays a scenario file one second (tick) at a
time. Each tick it sets the "world" in the iom361 model (temperature,
humidity, switches) from the scenario line in force, then acts as the
device: it reads the sensor and the switches back through the
registers, toggles LED k for every switch k that was just pressed, and
prints one trace line, for example

    2026-10-06 08:00:05 temp=25.0 rh=50.0 switches=0x1 pressed=0x1 leds=0x1

The time is start + tick, printed in UTC (gmtime), so a trace is the
same on every computer. A switch that is held does not toggle again,
and releasing a switch does nothing.

- sensor.h / sensor.c: reading_t and sensor_read, which reads TEMP_REG
  and HUMID_REG, keeps bits 19..0, converts them to degrees C and %RH,
  and rounds to 0.1, so a scenario that sets 91.0 reads 91.0. On a
  failed read it returns the iom361 code and leaves *r unchanged.
- panel.h / panel.c: panel_pressed and panel_released (which switches
  went off -> on or on -> off), and panel_led and panel_toggle, which
  change one LED by read-modify-write so the other LEDs keep their
  state (-1 if n is not 0 to 31).
- scenario.h / scenario.c: scenario_t (at most 1000 lines, no malloc),
  scenario_load, which returns 0, -1 if the file can't be opened, or
  the number of the first bad line, and scenario_at, which gives the
  line in force at a tick (values hold between lines).
- main.c: the replay loop. iom361 starts once with 4 switches and 4
  LEDs, and the display is turned off so only trace lines go to stdout.
- iom361_r4.h / iom361_r4.c: the course's model of the I/O board (not
  written by me).

## How it works
Each tick, main.c does these steps in order:

1. Finds the scenario line in force (scenario_at).
2. Plays the world: sets the temperature, humidity, and switches in the
   model with _iom361_set_sensor1 and _iom361_set_switches. These are
   test functions; on a real board this step is the physical world, so
   sensor.c and panel.c never call them.
3. Reads the sensor (sensor_read) and the switches (iom361_readReg)
   back through the registers, the way a real board's code would.
4. Finds the switches just pressed with panel_pressed(switches,
   before), where `before` is the previous tick's switches, so a held
   switch is not pressed again.
5. Toggles LED k for each pressed switch k (panel_toggle).
6. Reads the LED register back and prints the trace line.

The registers are 32-bit words at byte offsets from base. base is a
`uint32_t *`, so base + 1 moves 4 bytes, and the LED register is
`base + LEDS_REG / sizeof(uint32_t)`:

| offset   | 0x00     | 0x04    | 0x08    | 0x0C    | 0x10    |
|----------|----------|---------|---------|---------|---------|
| register | SWITCHES | LEDS    | RGB_LED | TEMP    | HUMID   |
| pointer  | base[0]  | base[1] | base[2] | base[3] | base[4] |

## Scenario files
    start 1791273600                # 2026-10-06 08:00:00 UTC
    # tick  temp   rh    switches
    0       25.0   50    0x0
    5       25.0   50    0x1        # switch 0 pressed

`#` starts a comment, blank lines are skipped, switches are in hex, and
ticks must start at 0 and increase. A bad file is rejected with the
number of its first bad line, counting every line from 1: no start
line, a missing field, a tick that does not increase, a first tick
that is not 0, more than 1000 data lines, or no data lines at all.

## Build and test
    make                               # builds replay
    ./replay scenarios/basic.txt       # prints the trace
    make test                          # unit tests, then the regression test
    make clean                         # removes everything the build made

## Tests
tests/test_hw06.c (37 checks) prints PASS or FAIL per check and a
summary, and exits nonzero if any check fails, so make test stops
there. It checks:

- panel_pressed and panel_released for a press, a release, and a held
  switch
- panel_led and panel_toggle on a fake register (a plain uint32_t):
  turning an LED on, turning one off, and toggling one each keep the
  other LEDs
- panel_led with LED 32 and LED -1 (just outside 0 to 31) returns -1 and
  leaves the register unchanged
- a reading of 91.0 C and 40.0 %RH reads back exactly as 91.0 and 40.0
- a failed read (wrong base) returns 1 and leaves *r unchanged
- a file with comments and blank lines everywhere loads with the right
  lines (tests/data/comments.txt)
- values hold between lines: scenario_at at ticks 0, 9, and 10
- bad files return the number of their first bad line:
  - tests/data/nostart.txt: no start line, line 2
  - tests/data/missing.txt: a missing field, line 3
  - tests/data/notinc.txt: ticks that do not increase, line 4
  - tests/data/first_tick.txt: first tick is not 0, line 3
  - tests/data/nodata.txt: no data lines, line 3 (the line after the last)
  - tests/data/toomany.txt: 1001 data lines, line 1002
- tests/data/max.txt (exactly 1000 data lines) loads, so the limit
  itself is allowed
- a file that cannot be opened returns -1

regress.sh runs ./replay on every file in scenarios/ and compares the
output with the file of the same name in expected/, using diff. The
expected traces were checked by hand line by line before they were
saved.

- basic.txt: the example from the project sheet
- multi.txt: two switches pressed at once, a held switch, a release, a
  press while another switch is held, 91.0 C, and the same switch
  pressed again so its LED goes back off
