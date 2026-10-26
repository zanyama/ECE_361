#ifndef TALLY_H
#define TALLY_H

#include <stdio.h>      /* FILE, for tally_print */

#define TALLY_MAX    4  /* tallies that can exist at the same time */
#define TALLY_NAMES 16  /* different names one tally can hold */
#define TALLY_LEN   15  /* longest name, without the '\0' */

/* opaque: only tally.c knows what is inside */
typedef struct tally tally_t;

/*
tally_create: hands out a new, empty tally

    Takes:   nothing
    Returns: the tally, or NULL if TALLY_MAX tallies are already in use
    Assumes: nothing
*/
tally_t *tally_create(void);

/*
tally_destroy: gives the tally back so tally_create can hand it out again

    Takes:   a tally, or NULL (does nothing)
    Returns: nothing
    Assumes: t is not used afterward
*/
void tally_destroy(tally_t *t);

/*
tally_add: counts one more occurrence of name

    Takes:   a tally and a name
    Returns: the new count of name, or -1 (and changes nothing) if t or
             name is NULL, name is empty or longer than TALLY_LEN, or
             name is new and the tally already holds TALLY_NAMES names
    Assumes: nothing
*/
int tally_add(tally_t *t, const char *name);

/*
tally_count: how often name was added

    Takes:   a tally and a name
    Returns: the count; 0 if name was never added, or if t or name is NULL
    Assumes: nothing
*/
int tally_count(const tally_t *t, const char *name);

/*
tally_size: how many different names the tally holds

    Takes:   a tally
    Returns: the number of names; 0 for NULL
    Assumes: nothing
*/
int tally_size(const tally_t *t);

/*
tally_print: prints one line per name, "name count", in strcmp order

    Takes:   a tally and an open stream
    Returns: nothing; prints nothing if t or out is NULL
    Assumes: out is open for writing
*/
void tally_print(const tally_t *t, FILE *out);

#endif
