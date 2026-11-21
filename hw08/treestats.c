/* treestats.c: generates stats with bst.c: reads ints from stdin, inserts
 * them into a bst, prints the size and height */
#include <stdio.h>
#include "bst.h"

int main(void) {

    int x;
    int r;

    bst_t *tree = bst_create();
    if (tree == NULL) {
        fprintf(stderr, "treestats: out of memory\n");
        return 1;
    }

    size_t n = 0;
    while ((r = scanf("%d", &x)) == 1) {
        if (bst_insert(tree, x) != 0) {
            fprintf(stderr, "treestats: out of memory\n");
            bst_destroy(tree);
            return 1;
        }
        n ++;
    }

    if (r == 0) {
        printf("not a number after %zu values\n", n);
        bst_destroy(tree);
        return 1;
    }

    else if (r == EOF) {
        if (n > 0) {
            printf("n %zu, height %zu\n", n, bst_height(tree));
        }
        else {
            printf("empty tree\n");
        }
        bst_destroy(tree);
        return 0;
    }
}
