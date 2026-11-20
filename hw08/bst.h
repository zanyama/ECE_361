#ifndef BST_H
#define BST_H

#include <stdbool.h> /* bool, true, false */
#include <stddef.h> /* size_t, NULL */

typedef struct bst bst_t; /* opaque */

/* Takes:   nothing
 * Returns: a newly created tree
 *          NULL if memory runs out
 * Assumes: freed with bst_destroy
 */
bst_t *bst_create(void);

/* Takes:   a tree from bst_create, or NULL
 * Returns: frees every node and the tree
 *          NULL does nothing
 * Assumes: t isn't used afterward
 */
void bst_destroy(bst_t *t);

/* Takes:   t: a tree from bst_create, or NULL
 *          key: the number to insert
 * Returns: 0 if successful
 *          -1 if t is NULL or memory runs out; tree is unchanged
 * Assumes: t has not been destroyed
 *          smaller keys go left, greater than or equal go to the right
 */
int bst_insert(bst_t *t, int key);

/* Takes:   t: a tree from bst_create, or NULL
 *          key: the number to look for
 *          visits: where to store the number of nodes visited, or NULL
 * Returns: true if key was found
 *          false if not found, or if t is NULL (0 visits)
 *          *visits counts the number of nodes compared with the key,
 *           whether or not it is found
 * Assumes: t has not been destroyed
 */
bool bst_contains(const bst_t *t, int key, size_t *visits);

/* Takes:   t: a tree from bst_create, or NULL
 * Returns: the number of keys that are in the tree
 *          0 if NULL
 * Assumes: t has not been destroyed
 */
size_t bst_size(const bst_t *t);

/* Takes:   t: a tree from bst_create, or NULL
 * Returns: the number of nodes on the longest path from the root down
 *          0 if t is empty or NULL
 *          1 if t is only one node
 * Assumes: t has not been destroyed
 */
size_t bst_height(const bst_t *t);

#endif
