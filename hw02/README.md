# HW2: readings, split into modules

## What it does
Takes the one-file readings.c starter and splits it into modules. It
reads "tick temperature humidity" lines from standard input and prints
a summary: how many readings, how many lines were skipped, min, max,
and mean for temperature and humidity, the longest run above a
threshold, and a histogram of the temperatures. The output is the same
as expected/, byte for byte.

## Build and test
    make          # builds ./readings
    make test     # runs tests/test_stats, then sh regress.sh
    make clean    # removes everything the build made

    ./readings [threshold] < data/normal.txt    # default threshold is 30.0 C

## Module diagram

               main.c
            /    |     \
     reading.c stats.c histogram.c

main calls the three modules. They don't call each other.

## What each module does
- reading: reads lines, skipping comments, blank lines, and bad lines
- stats: min, max, mean, and the longest run above a given threshold
- histogram: prints the temperature histogram
- main: command line, calls the modules, prints the summary

## Choices I made
- No global variables. The arrays and the threshold live in main and
  get passed in.
- One recursive stats_max(), called twice (temperature and humidity).
  stats_min() and stats_mean() are also one function each, called twice.
- reading_read() and stats_longest_run_above() each need to give back two
  numbers, so both return a small struct.
- Constants only one file uses (NUM_BINS, LINE_LEN) stay in that .c file.
- The Makefile lists every header a .o file includes. Changing stats.h
  rebuilds main.o and stats.o. Changing histogram.c rebuilds only
  histogram.o and the program.

## Tests
tests/test_stats.c checks stats: one value, the max at the start and
at the end, a value equal to the threshold (it ends a run), no run at
all, and stats_max on 10,000 values. make test also runs regress.sh,
which compares my output to expected/ with diff.
