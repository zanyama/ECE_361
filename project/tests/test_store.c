#include <stdio.h>
#include "store.h"

static int total = 0;
static int failures = 0;

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

/* a record with only the humidity set */
static record_t make(float rh) {
    record_t r = { { 0, 25.0f, rh }, STATE_IDLE, 0 };
    return r;
}

int main(void) {
    /* record_compare */
    record_t a = make(45.2f), b = make(45.7f);
    check_int("compare: same 1% step", record_compare(&a, &b), 0);

    a = make(45.7f); b = make(46.2f);
    check_int("compare: neighboring steps", record_compare(&a, &b) < 0, 1);

    a = make(45.9f); b = make(46.0f);
    check_int("compare: on a step boundary", record_compare(&a, &b) < 0, 1);

    /* insert and count */
    store_t *s = store_create();
    check_int("create", s != NULL, 1);
    check_int("empty: count", store_count(s), 0);

    a = make(50.0f);
    check_int("insert", store_insert(s, &a), 0);
    check_int("1 insert: count", store_count(s), 1);

    for (int i = 0; i < 99; i++) {
        a = make(99.0f - i);
        store_insert(s, &a);
    }
    check_int("100 inserts: count", store_count(s), 100);

    /* NULL arguments */
    check_int("insert NULL store", store_insert(NULL, &a), -1);
    check_int("insert NULL record", store_insert(s, NULL), -1);
    check_int("count NULL", store_count(NULL), 0);

    store_destroy(s);
    store_destroy(NULL);

    printf("\n%d of %d tests passed\n", total - failures, total);
    return failures ? 1 : 0;
}
