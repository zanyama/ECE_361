/* store.c: see store.h for each function's contract */

#include <stddef.h>     /* NULL */
#include <math.h>       /* floor */
#include "store.h"

#define STORE_MAX   2       /* stores that can exist at the same time */
#define STORE_CAP   1024    /* records one store can hold */

/* the only place struct store is completed: callers never see this */
struct store {
    record_t records[STORE_CAP];    /* sorted by record_compare */
    int n;                          /* records in use: records[0..n-1] */
    int in_use;                     /* 1 if handed out, 0 if free */
};

/* the pool: every store that can ever exist (no malloc) */
static store_t pool[STORE_MAX];

int record_compare(const record_t *a, const record_t *b) {
    // floor: 45.0 to 45.9 are all 45
    int ka = (int) floor(a->reading.rh_pct);
    int kb = (int) floor(b->reading.rh_pct);

    return ka - kb;
}

store_t *store_create(void) {
    for (int i = 0; i < STORE_MAX; i++) {
        if (!pool[i].in_use) {
            pool[i].in_use = 1;
            pool[i].n = 0;
            return &pool[i];
        }
    }

    return NULL;
}

void store_destroy(store_t *s) {
    if (s == NULL) {
        return;
    }

    s->in_use = 0;
    s->n = 0;
}

int store_insert(store_t *s, const record_t *r) {
    if (s == NULL || r == NULL || s->n == STORE_CAP) {
        return -1;
    }

    // find where r goes: after every record that is smaller or the same
    int i = 0;
    while (i < s->n && record_compare(&s->records[i], r) <= 0) {
        i++;
    }

    // move the later records up by one, starting from the end
    for (int j = s->n; j > i; j--) {
        s->records[j] = s->records[j - 1];
    }

    s->records[i] = *r;
    s->n++;

    return 0;
}

int store_count(const store_t *s) {
    if (s == NULL) {
        return 0;
    }

    return s->n;
}
