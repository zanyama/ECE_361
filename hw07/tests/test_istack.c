#include <stdio.h>
#include <stdlib.h>
#include "istack.h"

static int total = 0;
static int failures = 0;


/* Integer check: passes if got equals want; counts the check either way */
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

int main(void) {
    /* empty: pop and peek fail and leave *v alone */
    istack_t *s = istack_create();
    int x = 42;
    check_int("pop on empty returns -1", istack_pop(s, &x), -1);
    check_int("pop on empty leaves x",   x, 42);

    /* same for peek; reset x so we know peek is the one leaving it alone */
    x = 42;
    check_int("peek on empty returns -1", istack_peek(s, &x), -1);
    check_int("peek on empty leaves x",   x, 42);
    istack_destroy(s);


    /* LIFO: push 1, 2, 3 | pop gives 3, 2, 1 */
    istack_t *t = istack_create();

    check_int("push 1", istack_push(t, 1), 0);
    check_int("push 2", istack_push(t, 2), 0);
    check_int("push 3", istack_push(t, 3), 0);

    istack_pop(t, &x);
    check_int("first pop is 3", x, 3);
    istack_pop(t, &x);
    check_int("second pop is 2", x, 2);
    istack_pop(t, &x);
    check_int("third pop is 1", x, 1);
    istack_destroy(t);


    /* growth: starts at 8 and is full at 8 */
    istack_t *g = istack_create();
    check_int("capacity starts at 8", istack_capacity(g), 8);

    for (int i = 0; i < 8; i++) {
        istack_push(g, i);
    }
    check_int("capacity still at 8", istack_capacity(g), 8);

    /* adding a ninth element doubles the capacity to 16 and keeps every value */
    istack_push(g, 8);
    check_int("capacity increased to 16", istack_capacity(g), 16);

    size_t n;
    int *a = istack_to_array(g, &n);
    check_int("n is 9", n, 9);
    int mismatches = 0;
    for (int i = 0; i < 9; i++) {
        if (a[i] != i) {
            mismatches ++;
        }
    }
    check_int("all array values kept", mismatches, 0);
    free(a);

    /* g still holds 9 values, so this also destroys a stack that is not
     * empty (make memcheck shows no leak) */
    istack_destroy(g);

    /* 1000 pushes and pops: push 0..999, so the j-th pop should be 999 - j */
    istack_t *k = istack_create();
    for (int i = 0; i < 1000; i++) {
        istack_push(k, i);
    }

    /* pop all 1000 and count the ones that come back wrong */
    mismatches = 0;
    for (int j = 0; j < 1000; j++) {
        istack_pop(k, &x);
        if (x != 999 - j) {
            mismatches ++;
        }
    }
    check_int("all 1000 values popped ok", mismatches, 0);

    check_int("empty after 1000 pops", istack_is_empty(k), 1);
    istack_destroy(k);

    /* to_array is a copy: changing the array does not change the stack */
    istack_t *c = istack_create();
    istack_push(c, 10);
    istack_push(c, 20);
    a = istack_to_array(c, &n);
    a[1] = 999;             /* change the copy's top */
    istack_peek(c, &x);
    check_int("istack_to_array is a copy: stack top still 20", x, 20);

    /* the caller owns the copy and frees it */
    free(a);
    istack_destroy(c);

    /* NULL arguments: nothing crashes, the spec's answers */
    check_int("push NULL", istack_push(NULL, 1), -1);
    check_int("pop NULL", istack_pop(NULL, &x), -1);
    check_int("peek NULL", istack_peek(NULL, &x), -1);
    check_int("count NULL", istack_count(NULL), 0);
    check_int("is_empty NULL", istack_is_empty(NULL), 1);
    check_int("capacity NULL", istack_capacity(NULL), 0);
    check_int("to_array(NULL) returns NULL", istack_to_array(NULL, &n) == NULL, 1);
    check_int("to_array() n == 0", n, 0);

    istack_destroy(NULL);   /* make sure this doesn't crash */

    /* two stacks are independent: pushing onto p leaves q alone */
    istack_t *p = istack_create();
    istack_t *q = istack_create();
    istack_push(p, 5);
    check_int("q still empty after pushing to p", istack_count(q), 0);
    check_int("p's count is 1 after pushing to p", istack_count(p), 1);

    istack_destroy(p);
    istack_destroy(q);

    printf("%d/%d passed\n", total - failures, total);
    return failures != 0;
}





