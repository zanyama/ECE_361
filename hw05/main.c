/* main.c: runs the lock on standard input, one character at a time.
   Prints one line per event: state, character, event, next state, action,
   e.g. "GOT2 4 RIGHT -> OPEN UNLOCK"; at the end, "final: STATE".
   Characters that are not events (spaces, newlines, ...) are skipped. */

#include <stdio.h>      /* getchar, printf, EOF */
#include "lock.h"

int main(void) {
    state_t s = LOCKED;
    int c;

    while ((c = getchar()) != EOF) {
        int e = lock_event(s, (char) c);
        if (e < 0) {
            continue;   // not an event
        }

        action_t a;
        state_t next = lock_step(s, (event_t) e, &a);

        printf("%s %c %s -> %s %s\n", state_name(s), c,
               event_name((event_t) e), state_name(next), action_name(a));

        s = next;
    }

    printf("final: %s\n", state_name(s));
    return 0;
}
