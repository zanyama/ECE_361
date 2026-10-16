# HW3: a pointer library

## What it does
Four small functions on float arrays, written with pointer arithmetic
(p++, p < a + n, *p) instead of a[i]:
- swap_float: exchanges *a and *b
- minmax: stores the smallest and largest of a[0..n-1] in *min and *max;
  returns -1 and changes nothing if n < 1 or any pointer is NULL
- find_above: returns a pointer to the first value greater than a
  threshold, or NULL if there is none
- reverse: reverses a[0..n-1] in place, using two pointers that move
  toward each other and swap_float

The contract for each function (what it takes, returns, and assumes) is
in ptr.h.

## Build and test
    make          # builds ptr.o and tests/test_ptr
    make test     # builds if needed, then runs tests/test_ptr
    make clean    # removes everything the build made

## Tests
tests/test_ptr.c checks:
- swap_float on two variables, and on the same address twice
- minmax on several values, on one value, with n = 0, and with each
  pointer NULL; the error cases check that *min and *max stay unchanged
- find_above with two matches (it returns the first), with none, and
  with a value equal to the threshold (it doesn't count)
- reverse on an even and an odd length

It prints PASS or FAIL per check and a summary, and exits nonzero if any
check fails.
