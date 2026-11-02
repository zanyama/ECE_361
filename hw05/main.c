/* main.c: runs the lock on standard input, one character at a time

   Usage: ./lock < data/codes.txt

   Prints one line per event: state, character, event, next state, action,
   e.g. "GOT2 4 RIGHT -> OPEN UNLOCK"; at the end, "final: STATE".
   Characters that are not events (spaces, newlines, ...) are skipped. */

#include <stdio.h>      /* getchar, printf, EOF */
#include "lock.h"

int main(void) {

    // initialized in LOCKED state
    state_t s = LOCKED;

    // an int, not a char, so EOF (-1) can't look like a character
    int c;

    // read one character at a time until the input runs out (EOF)
    while ((c = getchar()) != EOF) {

        // turn the character into an event for the current state
        int e = lock_event(s, (char) c);

        // not an event
        if (e < 0) {
            continue;
        }

        /* one step: lock_step returns the next state and writes the action
           into a */
        action_t a;
        state_t next = lock_step(s, (event_t) e, &a);

        // print before s changes, to capture both the old and new state
        printf("%s %c %s -> %s %s\n", state_name(s), c,
               event_name((event_t) e), state_name(next), action_name(a));

        // update state
        s = next;
    }

    // where the lock ended up when the input ran out
    printf("final: %s\n", state_name(s));
    return 0;
}
