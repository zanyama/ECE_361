#ifndef STATS_H
#define STATS_H

/* Result of stats_longest_run_above: length of the run and the index of
   its first element. length is 0 (and start is -1) if no value is above. */
typedef struct {
    int length;
    int start;
} stats_run_t;

/* Smallest value in a[0..n-1]. Assumes n >= 1. */
float stats_min(const float a[], int n);

/* Largest value in a[0..n-1], computed recursively. Assumes n >= 1. */
float stats_max(const float a[], int n);

/* Mean of a[0..n-1]. Assumes n >= 1. */
float stats_mean(const float a[], int n);

/* Longest run of consecutive values in a[0..n-1] strictly greater than
   threshold. A value equal to threshold ends a run. Returns the run's length
   and start index.  Assumes n >= 1. */
stats_run_t stats_longest_run_above(const float a[], int n, float threshold);

#endif
