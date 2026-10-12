#ifndef READING_H
#define READING_H

#include <stdio.h>

#define READING_MAX 1000

typedef struct {
    int count;     /* number of readings stored */
    int skipped;   /* lines that did not hold three numbers */
} reading_result_t;

/*
 * Reads readings from 'in', one per line: "tick temperature humidity".
 *
 * Takes:   an open input stream, and three arrays that the caller owns.
 *          Each array must have room for READING_MAX elements.
 * Does:    fills ticks[], temps[], hums[] from index 0; skips blank
 *          lines and lines starting with '#'; counts any other line
 *          that does not hold three numbers as skipped; stops with a
 *          warning on stderr when there are more than READING_MAX
 *          readings.
 * Returns: the number of readings stored and the number of lines
 *          skipped.
 * Assumes: 'in' is not NULL.
 */
reading_result_t reading_read(FILE *in, int ticks[], float temps[], float hums[]);

#endif
