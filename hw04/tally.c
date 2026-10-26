/* tally.c: see tally.h for each function's contract */

#include <stdio.h>      /* FILE, fprintf */
#include <string.h>     /* strcmp, strcpy */
#include "tally.h"

/* one name and how often it was added */
struct entry {
    char name[TALLY_LEN + 1];   /* +1 for the '\0' */
    int count;
};

/* the only place struct tally is completed: callers never see this */
struct tally {
    struct entry entries[TALLY_NAMES];  /* sorted by name, strcmp order */
    int n;                              /* entries in use: entries[0..n-1] */
    int in_use;                         /* 1 if handed out, 0 if free */
};

/* the pool: every tally that can ever exist (no malloc) */
static tally_t pool[TALLY_MAX];

/* length of name, but stops looking after TALLY_LEN + 1 characters,
   so a long or unterminated name can't make us read forever */
static int name_len(const char *name) {
    int len = 0;

    while (len <= TALLY_LEN && name[len] != '\0') {
        len++;
    }

    return len;
}

/* index of name in t if it is there, otherwise the index where it
   belongs to keep the entries sorted (can be t->n, the end) */
static int find_pos(const tally_t *t, const char *name) {
    int i = 0;

    // entries are sorted, so stop at the first one that is not smaller
    while (i < t->n && strcmp(t->entries[i].name, name) < 0) {
        i++;
    }

    return i;
}

/* returns 1 if entry i (from find_pos) is name itself, 0 if i is only
where name would go: past the last entry, or a different name */
static int is_at(const tally_t *t, int i, const char *name) {
    return i < t->n && strcmp(t->entries[i].name, name) == 0;
}

tally_t *tally_create(void) {

    /* for each tally in pool, check if it's in use, and if not, initialize
    to 0 and return tally_t */
    for (int i = 0; i < TALLY_MAX; i++) {
        if (!pool[i].in_use) {
            pool[i].in_use = 1;
            pool[i].n = 0;
            return &pool[i];
        }
    }

    // NULL if every tally in the pool is taken
    return NULL;
}

void tally_destroy(tally_t *t) {
    if (t == NULL) {
        return;
    }

    // clear use flag and set number of entries to 0
    t->in_use = 0;
    t->n = 0;
}

int tally_add(tally_t *t, const char *name) {
    if (t == NULL || name == NULL) {
        return -1;
    }

    // check the length before copying anything
    int len = name_len(name);
    if (len == 0 || len > TALLY_LEN) {
        return -1;
    }

    int i = find_pos(t, name);

    // name already there: just count it
    if (is_at(t, i, name)) {
        t->entries[i].count++;
        return t->entries[i].count;
    }

    // name is new: refuse if there is no room for it
    if (t->n == TALLY_NAMES) {
        return -1;
    }

    /* move entries i..n-1 up by one, starting from the end: going
       from the front would overwrite entries before they are moved */
    for (int j = t->n; j > i; j--) {
        t->entries[j] = t->entries[j - 1];
    }

    // len <= TALLY_LEN was checked above, so name and its '\0' always fit
    strcpy(t->entries[i].name, name);
    t->entries[i].count = 1;
    t->n++;

    return 1;
}

int tally_count(const tally_t *t, const char *name) {
    if (t == NULL || name == NULL) {
        return 0;
    }

    int i = find_pos(t, name);

    if (is_at(t, i, name)) {
        return t->entries[i].count;
    }

    // never added
    return 0;
}

int tally_size(const tally_t *t) {
    if (t == NULL) {
        return 0;
    }

    return t->n;
}

void tally_print(const tally_t *t, FILE *out) {
    if (t == NULL || out == NULL) {
        return;
    }

    // already sorted, so printing in order gives strcmp order
    for (int i = 0; i < t->n; i++) {
        fprintf(out, "%s %d\n", t->entries[i].name, t->entries[i].count);
    }
}
