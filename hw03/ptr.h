#ifndef PTR_H
#define PTR_H

/*
swap_float:  exchanges the values stored at *a and *b.

    Takes:   addresses of two floats (can be the same address)
    Returns: nothing. The caller's variables are changed through the pointers
    Assumes: both pointers are non-NULL
*/

void swap_float(float *a, float *b);

/*
minmax:      finds the smallest and largest float in a[0..n-1]

    Takes:   pointer to the first of n floats (read only), the count n, and the
             addresses where the min/max are stored
    Returns: 0 on success,
             -1 and leaves *min and *max unchanged if n < 1 or any pointer is NULL
    Assumes: a points to at least n floats
*/

int minmax(const float *a, int n, float *min, float *max);

/*
find_above:  finds the first value greater than the given threshold

    Takes:   a pointer to the first of n floats (read only), the count n, and
             threshold to compare against
    Returns: a pointer to the first value greater than threshold (strictly;
             equal doesn't count), or NULL if there is none
    Assumes: a points to at least n floats
*/

const float *find_above(const float *a, int n, float threshold);

/*
reverse:     reverses a[0..n-1] in place

    Takes:   a pointer to the first of n floats, and the count n
    Returns: nothing (it does nothing if a is NULL or n < 2)
    Assumes: a points to at least n floats
*/

void reverse(float *a, int n);

#endif
