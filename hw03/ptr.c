/* ptr.c: see ptr.h for each function's contract */

#include <stddef.h> /* for NULL*/
#include "ptr.h"

void swap_float(float *a, float *b) {

    // use temporary float to store a while swapping
	float temp = *a;

	*a = *b;
	*b = temp;
}

int minmax(const float *a, int n, float *min, float *max) {
	if (a == NULL || n < 1 || min == NULL || max == NULL) {
        return -1;
	}

    /* local copies to store value and avoid writing to *min and *max
       every time */
    float minVal = *a, maxVal = *a;

    for (const float *p = a + 1; p < a + n; p++) {
        if (*p < minVal) minVal = *p;
        if (*p > maxVal) maxVal = *p;
	}

    // write to pointer at the end
    *min = minVal;
    *max = maxVal;

	return 0;
}

const float *find_above(const float *a, int n, float threshold) {

    if (a == NULL) {
        return NULL;
    }

    for (const float *p = a; p < a + n; p++) {
        if (*p > threshold) {
            return p;
        }
    }

    return NULL;
}

void reverse(float *a, int n) {

    // do nothing if a is NULL or n < 2: 0 or 1 elements are already their own reverse
    if (a == NULL || n < 2) {
        return;
    }

    // point at the first and last elements of the array
    float *left = a;
    float *right = a + n - 1;

    // while the left one is to the left of right..
    while (left < right) {

        // swap the elements and move each towards the middle
        swap_float(left, right);
        left++;
        right--;
    }
}
