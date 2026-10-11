#ifndef READING_H
#define READING_H

#include <stdio.h>

#define READING_MAX 1000

typedef struct {
    int count;     /* number of readings stored */
    int skipped;   /* lines that did not hold three numbers */
} reading_result_t;

reading_result_t reading_read(FILE *in, int ticks[], float temps[], float hums[]);

#endif
