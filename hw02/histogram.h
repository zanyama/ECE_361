#ifndef HISTOGRAM_H
#define HISTOGRAM_H

/*
prints histogram of temperatures in bins of 5 C

inputs:  temps[]: array of temperatures 
               n: length of array

returns: nothing

prints:  "histogram:", followed by one row for every non-empty bin:
         (bin range, one  "*" per reading, and the count)

assumes: n >= 1

temp range: 0-100 C (values < 0 go in the first bin, values >= 100
go in the last bin)
*/
void histogram_print(const float temps[], int n);

#endif
