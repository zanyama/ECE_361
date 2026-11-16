/* balance.c: ./balance < file reads stdin one character at a time and checks
 * that every (, [, { is closed in the correct order. Stops at the first error,
 * prints and exits 1; prints and exits 0 otherwise. */

#include <stdio.h>
#include <stdlib.h>
#include "istack.h"

/* find the opener that goes with closer c */
static int opener_for(int c) {
    if (c == ')') return '(';
    if (c == ']') return '[';
    return '{';
}

/* find the closer that goes with opener c */
static int closer_for(int c) {
    if (c == '(') return ')';
    if (c == '[') return ']';
    return '}';
}


int main(void) {
    istack_t *s = istack_create();
    size_t deepest = 0;
    int ch; /* not char: getchar returns EOF (-1), as well as every char */
    int line = 1; /* position coordinates */
    int column = 1;
    int pairs = 0; /* number of pairs of brackets completed*/
    int top ; /* the opener popped/peeked from the top of the stack */

    while((ch = getchar()) != EOF) {

        /* openers: push to stack and check if it's deeper than deepest*/
        if (ch == '(' || ch == '[' || ch == '{') {
            istack_push(s, ch);

            if (istack_count(s) > deepest) {
                deepest = istack_count(s);
            }
        }

        /* closers: pop returns -1 on an empty stack: a closer with nothing open*/
        else if (ch == ')' || ch == ']' || ch == '}') {
            if (istack_pop(s, &top) == -1) {
                printf("line %d, column %d: found '%c', nothing open\n", line, column, ch);
                istack_destroy(s);
                return 1;
            }

            /* the most recent opener must match, otherwise destroy stack and exit */
            if (top != opener_for(ch)) {
                printf("line %d, column %d: found '%c', expected '%c'\n", line, column, ch, closer_for(top));
                istack_destroy(s);
                return 1;
            }
            pairs++;
        }

        /* update position after bracket check, so line/column still point at the bracket */
        if (ch == '\n') {
            line++;
            column = 1;
        }

        else {
            column ++;
        }
    }

    /* if anything is left on the stack, something never closed */
    if (!istack_is_empty(s)) {
        istack_peek(s, &top);
        printf("end of input: %zu still open, innermost '%c'\n", istack_count(s), top);
        istack_destroy(s);

        return 1;
    }

    printf("balanced: %d pairs, deepest %zu\n", pairs, deepest);
    istack_destroy(s);

    return 0;
}
