/* test_bst.c: unit tests for bst. Prints PASS or FAIL per check and a
 * summary, and exits nonzero if any check fails, so make test stops. */
#include <stdio.h>
#include "bst.h"

static int total = 0;
static int failures = 0;

/* size_t check: passes if got equals want; counts the check either way */
static void check_size(const char *name, size_t got, size_t want) {
    total++;
    if (got == want) {
        printf("PASS %s\n", name);
    }
    else {
        printf("FAIL %s: got %zu, want %zu\n", name, got, want);
        failures++;
    }
}

/* bool check: passes if got equals want; counts the check either way */
static void check_bool(const char *name, bool got, bool want) {
    total++;
    if (got == want) {
        printf("PASS %s\n", name);
    }
    else {
        printf("FAIL %s: got %s, want %s\n", name,
            got ? "true" : "false",
            want ? "true" : "false");
        failures++;
    }
}

int main(void) {

    bst_t *newTree = bst_create();
    size_t visits = 0;
    int vals[7] = {27, 14, 35, 10, 19, 31, 42};
    for (int i = 0; i < 7; i ++) {
        bst_insert(newTree, vals[i]);
    }
    check_size("seven keys: size 7", bst_size(newTree), 7);
    check_size("three layers", bst_height(newTree), 3);
    check_bool("31 found", bst_contains(newTree, 31, &visits), true);
    check_size("31 in 3 visits", visits, 3);
    check_bool("15 not found", bst_contains(newTree, 15, &visits), false);
    check_size("15 in 3 visits", visits, 3);
    bst_destroy(newTree);

    bst_t *dupeTree = bst_create();
    bst_insert(dupeTree, 69);
    bst_insert(dupeTree, 69);
    check_size("pass a value twice: size is 2", bst_size(dupeTree), 2);
    check_size("pass a value twice: height is 2", bst_height(dupeTree), 2);
    bst_destroy(dupeTree);

    bst_t *incTree = bst_create();
    for (int i = 0; i < 10; i ++) {
        bst_insert(incTree, i);
    }
    check_size("insert 0-9: size is 10", bst_size(incTree), 10);
    check_size("insert 0-9: height is 10", bst_height(incTree), 10);
    bst_destroy(incTree);


    printf("%d/%d passed\n", total - failures, total);
    return failures != 0;
}
