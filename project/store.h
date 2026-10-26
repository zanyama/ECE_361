#ifndef STORE_H
#define STORE_H

#include <stdint.h>     /* uint32_t, for the status word */
#include "sensor.h"     /* reading_t */
#include "fsm.h"        /* fsm_state_t */

/*
 * store: keeps a record of every reading, ordered by relative humidity
 * quantized to 1 %. A sorted array for now; the tree replaces it in week 8.
 */

/* one tick: the reading, the state at that moment, and the status word */
typedef struct {
    reading_t   reading;
    fsm_state_t state;
    uint32_t    status;
} record_t;

/* opaque: only store.c knows what is inside */
typedef struct store store_t;

/*
record_compare: orders two records by humidity, in whole percent
                (45.2 and 45.7 are the same)

    Takes:   two records
    Returns: negative, 0, or positive, like strcmp
    Assumes: a and b are not NULL
*/
int record_compare(const record_t *a, const record_t *b);

/* store_create: a new, empty store, or NULL if none is free */
store_t *store_create(void);

/* store_destroy: gives the store back; NULL does nothing */
void store_destroy(store_t *s);

/*
store_insert: copies r into the store, in order

    Returns: 0, or -1 if s or r is NULL or the store is full
*/
int store_insert(store_t *s, const record_t *r);

/* store_count: how many records the store holds; 0 for NULL */
int store_count(const store_t *s);

#endif
