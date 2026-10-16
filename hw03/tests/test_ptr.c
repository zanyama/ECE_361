#include <stdio.h>
#include <stddef.h>
#include "ptr.h"

static int total = 0;
static int failures = 0;

static void check_float(const char *name, float got, float want) {
    total++;
    if (got == want) {
        printf("PASS %s\n", name);
    }
    else {
        printf("FAIL %s: got %f, want %f\n", name, got, want);
        failures++;
    }
}

static void check_int(const char *name, int got, int want) {
    total++;
    if (got == want) {
        printf("PASS %s\n", name);
    }
    else {
        printf("FAIL %s: got %d, want %d\n", name, got, want);
        failures++;
    }
}

static void check_ptr(const char *name, const float *got, const float *want) {
    total++;
    if (got == want) {
        printf("PASS %s\n", name);
    }
    else {
        printf("FAIL %s: got %p, want %p\n", name, (const void *) got,
               (const void *) want);
        failures++;
    }
}

int main(void) {
    /* swap_float */
    float x = 1.5f, y = -2.0f;
    swap_float(&x, &y);
    check_float("swap: x", x, -2.0f);
    check_float("swap: y", y, 1.5f);

    /* extra: same address twice leaves the value alone */
    swap_float(&x, &x);
    check_float("swap same address", x, -2.0f);

    /* minmax: several values, min and max not at the ends */
    float several[] = {3.0f, -1.0f, 7.5f, 2.0f, 0.0f};
    float min = 0.0f, max = 0.0f;
    check_int("minmax several: return", minmax(several, 5, &min, &max), 0);
    check_float("minmax several: min", min, -1.0f);
    check_float("minmax several: max", max, 7.5f);

    /* minmax: one value is both min and max */
    float one[] = {42.5f};
    check_int("minmax one: return", minmax(one, 1, &min, &max), 0);
    check_float("minmax one: min", min, 42.5f);
    check_float("minmax one: max", max, 42.5f);

    /* minmax: n = 0 returns -1 and leaves the outputs unchanged */
    min = 111.0f;
    max = 222.0f;
    check_int("minmax n=0: return", minmax(several, 0, &min, &max), -1);
    check_float("minmax n=0: min unchanged", min, 111.0f);
    check_float("minmax n=0: max unchanged", max, 222.0f);

    /* minmax: NULL pointers return -1 and leave the outputs unchanged */
    check_int("minmax a=NULL: return", minmax(NULL, 5, &min, &max), -1);
    check_float("minmax a=NULL: min unchanged", min, 111.0f);
    check_float("minmax a=NULL: max unchanged", max, 222.0f);

    check_int("minmax min=NULL: return", minmax(several, 5, NULL, &max), -1);
    check_float("minmax min=NULL: max unchanged", max, 222.0f);

    check_int("minmax max=NULL: return", minmax(several, 5, &min, NULL), -1);
    check_float("minmax max=NULL: min unchanged", min, 111.0f);

    /* find_above: two matches returns the first */
    float vals[] = {1.0f, 5.0f, 3.0f, 8.0f};
    check_ptr("find_above two matches", find_above(vals, 4, 4.0f), &vals[1]);

    /* find_above: no match */
    check_ptr("find_above none", find_above(vals, 4, 10.0f), NULL);

    /* find_above: equal to the threshold does not count */
    check_ptr("find_above equal", find_above(vals, 4, 8.0f), NULL);

    /* reverse: even length */
    float even[] = {1.0f, 2.0f, 3.0f, 4.0f};
    reverse(even, 4);
    check_float("reverse even: [0]", even[0], 4.0f);
    check_float("reverse even: [1]", even[1], 3.0f);
    check_float("reverse even: [2]", even[2], 2.0f);
    check_float("reverse even: [3]", even[3], 1.0f);

    /* reverse: odd length, the middle stays put */
    float odd[] = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f};
    reverse(odd, 5);
    check_float("reverse odd: [0]", odd[0], 5.0f);
    check_float("reverse odd: [1]", odd[1], 4.0f);
    check_float("reverse odd: [2]", odd[2], 3.0f);
    check_float("reverse odd: [3]", odd[3], 2.0f);
    check_float("reverse odd: [4]", odd[4], 1.0f);

    printf("\n%d of %d tests passed\n", total - failures, total);
    return failures ? 1 : 0;
}
