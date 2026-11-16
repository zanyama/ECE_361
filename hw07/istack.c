#include <stdlib.h>
#include "istack.h"

/* the only place the fields are visible; everyone else sees istack_t * */
struct istack {
    int *data;          /* the values, on the heap; [0] is the bottom */
    size_t count;       /* values held; also is the index of the next free slot */
    size_t capacity;    /* room in data right now; it doubles when full */
};

istack_t *istack_create(void) {
    /* create room for the struct */
    struct istack *s = malloc(sizeof *s);

    /* out of memory */
    if (s == NULL) {
        return NULL;
    }

    /* room for ISTACK_START ints */
    s->data = malloc(ISTACK_START * sizeof *s->data);

    /* the struct worked but the array didn't: give the struct back so nothing leaks */
    if (s->data == NULL) {
        free(s);
        return NULL;
    }

    /* stack is empty, with room for ISTACK_START */
    s->count = 0;
    s->capacity = ISTACK_START;

    return s;
}

void istack_destroy(istack_t *s) {
    /* nothing to free; s->data crashes on NULL */
    if (s == NULL) {
        return;
    }

    /* data first, since the pointer to it lives inside s */
    free(s->data);
    free(s);
}

int istack_push(istack_t *s, int v) {
    if (s == NULL) {
        return -1;
    }

    /* s is full: double the room before storing */
    if (s->count == s->capacity) {
        size_t new_cap = s->capacity * 2;

        /* realloc into a second pointer: on failure it returns NULL but the old
         * block is still allocated, so s->data = realloc(...) would lose it and
         * cause a leak */
        int *bigger = realloc(s->data, new_cap * sizeof *s->data);

        /* failed: leave s untouched */
        if (bigger == NULL) {
            return -1;
        }

        /* everything worked: store the new block and its size */
        s->data = bigger;
        s->capacity = new_cap;
    }

    /* count is the next free slot */
    s->data[s->count] = v;
    s->count ++;

    return 0;
}

int istack_pop(istack_t *s, int *v) {
    if (s == NULL) {
        return -1;
    }

    /* empty: nothing to pop, *v left untouched */
    if (s->count == 0) {
        return -1;
    }

    /* the top is the last element in the array, at count - 1 */
    int newVal = s->data[s->count - 1];

    /* v may be NULL (caller doesn't want the value): skip the store, still pop */
    if (v != NULL) {
        *v = newVal;
    }

    /* remove the top; the room stays */
    s->count--;
    return 0;
}


/* same as pop but only looks at the stack, so s is const */
int istack_peek(const istack_t *s, int *v) {
    if (s == NULL) {
        return -1;
    }

    /* empty: nothing to see, *v left untouched */
    if (s->count == 0) {
        return -1;
    }

    /* top is the last one held, at count - 1 */
    int newVal = s->data[s->count - 1];

    /* v may be NULL: skip the store */
    if (v != NULL) {
        *v = newVal;
    }

    return 0;
}

/* NULL holds nothing: 0 */
size_t istack_count(const istack_t *s) {
    if (s == NULL) {
        return 0;
    }

    return s->count;
}

/* NULL has no room: 0 */
size_t istack_capacity(const istack_t *s) {
    if (s == NULL) {
        return 0;
    }

    return s->capacity;
}

/* NULL holds nothing, so it counts as empty: true */
bool istack_is_empty(const istack_t *s) {
    if (s == NULL) {
        return true;
    }

    /* empty is about values held, not room */
    return s->count == 0;
}

int *istack_to_array(const istack_t *s, size_t *n) {

    /* nothing to copy */
    if (s == NULL || s->count == 0) {
        *n = 0;
        return NULL;
    }

    /* a copy: the caller owns it and frees it */
    int *newMem = malloc(s->count * sizeof(*newMem));

    /* out of memory: report nothing */
    if (newMem == NULL) {
        *n = 0;
        return NULL;
    }

    /* index 0 is the bottom, so copying in order gives bottom first */
    for (size_t i = 0; i < s->count; i ++) {
        newMem[i] = s->data[i];
    }

    /* hand the count back to the caller through n */
    *n = s->count;

    return newMem;
}
