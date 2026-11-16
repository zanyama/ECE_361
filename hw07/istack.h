#ifndef ISTACK_H
#define ISTACK_H

#include <stddef.h>
#include <stdbool.h>

/* room at first; doubles when full */
#define ISTACK_START 8

/* opaque */
typedef struct istack istack_t;

/* Takes:   nothing.
 * Returns: a new, empty stack with room for ISTACK_START values,
 *          or NULL if memory runs out (nothing is leaked).
 * Assumes: the caller frees it later with istack_destroy. */
istack_t    *istack_create(void);

/* Takes:   a stack from istack_create, or NULL.
 * Returns: nothing. Frees the stack and its values; NULL does nothing.
 * Assumes: s is not used again afterward (not even destroyed twice). */
void        istack_destroy(istack_t *s);

/* Takes:   a stack and a value to push.
 * Returns: 0 on success; -1 if s is NULL or memory runs out.
 *          If the stack is full, doubles its room with realloc first.
 *          On -1 the stack is unchanged. */
int         istack_push(istack_t *s, int v);

/* Takes:   a stack and where to put the top value (v may be NULL).
 * Returns: 0 and removes the top value, storing it in *v;
 *          -1 if s is NULL or empty, and *v is unchanged. */
int         istack_pop(istack_t *s, int *v);

/* Takes:   a stack and where to put the top value (v may be NULL).
 * Returns: 0 and stores the top value in *v, without removing it;
 *          -1 if s is NULL or empty, and *v is unchanged. */
int         istack_peek(const istack_t *s, int *v);

/* Takes:   a stack and where to put the number of values (n).
 * Returns: a new array of the values, bottom first, with their number in *n;
 *          NULL and *n = 0 if s is NULL or empty, or memory runs out.
 * Assumes: n is not NULL; the caller owns the array and frees it. */
int         *istack_to_array(const istack_t *s, size_t *n);

/* Takes:   a stack.
 * Returns: how many values it holds; 0 for NULL. */
size_t      istack_count(const istack_t *s);

/* Takes:   a stack.
 * Returns: how many values fit right now before it must grow; 0 for NULL. */
size_t      istack_capacity(const istack_t *s);

/* Takes:   a stack.
 * Returns: true if it holds no values; true for NULL. */
bool        istack_is_empty(const istack_t *s);

#endif
