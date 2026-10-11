#include "stats.h"

float stats_min(const float a[], int n) {
    float m = a[0];
    for (int i = 1; i < n; i++) {
        if (a[i] < m)
            m = a[i];
    }
    return m;
}

float stats_mean(const float a[], int n) {
    float sum = 0.0f;
    for (int i = 0; i < n; i++)
        sum += a[i];
    return sum / n;
}

float stats_max(const float a[], int n) {
    if (n == 1) {
        return a[0];
    }

    float rest = stats_max(a + 1, n - 1);
    return a[0] > rest ? a[0] : rest;
}

stats_run_t stats_longest_run_above(const float a[], int n, float threshold) {
    int run = 0, best = 0, best_start = -1, start = 0;
    for (int i = 0; i < n; i++) {
        if (a[i] > threshold) {
            if (run == 0) {
                start = i;
            }
            run ++;
            if (run > best) {
                best = run;
                best_start = start;
            }
        }
	else {
            run = 0;
        }
    }
    stats_run_t result;
    result.length = best;
    result.start = best_start;

    return result;
}
