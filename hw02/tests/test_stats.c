#include <stdio.h>
#include "stats.h"

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
    total ++;
    if (got == want) {
        printf("PASS %s\n", name);
    }
    else {
        printf("FAIL %s: got %d, want %d\n", name, got, want);
        failures++;
    }
}

int main(void) {
    /* 1. one value */
    float one[] = {42.5f};
    check_float("one value: min", stats_min(one, 1), 42.5f);
    check_float("one value: max", stats_max(one, 1), 42.5f);
    check_float("one value: mean", stats_mean(one, 1), 42.5f);

    /* 2. maximum at the start and at the end */
    float first[] = {9.0f, 3.0f, 5.0f};
    float last[]  = {3.0f, 5.0f, 9.0f};
    check_float("max at start", stats_max(first, 3), 9.0f);
    check_float("max at end", stats_max(last, 3), 9.0f);

    /* 3. a value equal to the threshold ends a run */
    float eq[] = {31.0f, 30.0f, 31.0f, 32.0f};
    stats_run_t r = stats_longest_run_above(eq, 4, 30.0f);
    check_int("threshold: run length", r.length, 2);
    check_int("threshold: run start", r.start, 2);

    /* extra: no value above the threshold */
    float low[] = {1.0f, 2.0f, 3.0f};
    r = stats_longest_run_above(low, 3, 30.0f);
    check_int("no run: length", r.length, 0);
    check_int("no run: start", r.start, -1);

    /* 4. recursive maximum on 10,000 values */
    static float big[10000];
    for (int i = 0; i < 10000; i++)
        big[i] = (float) i;
    check_float("10,000 values: max", stats_max(big, 10000), 9999.0f);

    printf("\n%d of %d tests passed\n", total - failures, total);
    return failures ? 1 : 0;
}
