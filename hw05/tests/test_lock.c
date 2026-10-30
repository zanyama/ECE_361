#include <stdio.h>
#include <string.h>
#include "lock.h"

static int total = 0;
static int failures = 0;

static void check_int(const char *name, int got, int want) {
    total++;
    if (got == want) {
        printf("PASS %s\n", name);
    }
    else {
        printf("FAIL %s: got %d, want %d\n", name, got, want);
        failures++;
    }
}

static void check_str(const char *name, const char *got, const char *want) {
    total++;
    if (strcmp(got, want) == 0) {
        printf("PASS %s\n", name);
    }
    else {
        printf("FAIL %s: got %s, want %s\n", name, got, want);
        failures++;
    }
}

/* one table entry: lock_step(s, e) must go to next and do action */
static void check_step(state_t s, event_t e, state_t next, action_t action) {
    char name[64];
    snprintf(name, sizeof name, "table %s %s -> %s %s", state_name(s),
             event_name(e), state_name(next), action_name(action));

    action_t got_action = A_UNDEFINED;
    state_t got_next = lock_step(s, e, &got_action);

    total++;
    if (got_next == next && got_action == action) {
        printf("PASS %s\n", name);
    }
    else {
        printf("FAIL %s: got %s %s\n", name, state_name(got_next),
               action_name(got_action));
        failures++;
    }
}

/* runs the characters in input from LOCKED, like main.c does;
   returns the final state, and the last action taken in *last */
static state_t run(const char *input, action_t *last) {
    state_t s = LOCKED;
    *last = A_UNDEFINED;

    for (int i = 0; input[i] != '\0'; i++) {
        int e = lock_event(s, input[i]);
        if (e >= 0) {
            s = lock_step(s, (event_t) e, last);
        }
    }

    return s;
}

int main(void) {
    action_t last;

    /* all 16 table entries, written out here, not read from lock_table */
    check_step(LOCKED, E_RIGHT,   GOT1,   A_NONE);
    check_step(LOCKED, E_WRONG,   LOCKED, A_BEEP);
    check_step(LOCKED, E_CLOSE,   LOCKED, A_NONE);
    check_step(LOCKED, E_TIMEOUT, LOCKED, A_NONE);

    check_step(GOT1,   E_RIGHT,   GOT2,   A_NONE);
    check_step(GOT1,   E_WRONG,   LOCKED, A_BEEP);
    check_step(GOT1,   E_CLOSE,   GOT1,   A_NONE);
    check_step(GOT1,   E_TIMEOUT, LOCKED, A_NONE);

    check_step(GOT2,   E_RIGHT,   OPEN,   A_UNLOCK);
    check_step(GOT2,   E_WRONG,   LOCKED, A_BEEP);
    check_step(GOT2,   E_CLOSE,   GOT2,   A_NONE);
    check_step(GOT2,   E_TIMEOUT, LOCKED, A_NONE);

    check_step(OPEN,   E_RIGHT,   OPEN,   A_NONE);
    check_step(OPEN,   E_WRONG,   OPEN,   A_NONE);
    check_step(OPEN,   E_CLOSE,   LOCKED, A_LOCK);
    check_step(OPEN,   E_TIMEOUT, LOCKED, A_LOCK);

    /* no entry was left out */
    int undefined = 0;
    for (int s = 0; s < N_STATES; s++) {
        for (int e = 0; e < N_EVENTS; e++) {
            undefined += lock_table[s][e].action == A_UNDEFINED;
        }
    }
    check_int("table: no A_UNDEFINED entries", undefined, 0);

    /* lock_step with a NULL action still returns the next state */
    check_int("lock_step NULL action", lock_step(GOT2, E_RIGHT, NULL), OPEN);

    /* lock_event: a right and a wrong digit in each state */
    check_int("event LOCKED 3: RIGHT", lock_event(LOCKED, '3'), E_RIGHT);
    check_int("event LOCKED 1: WRONG", lock_event(LOCKED, '1'), E_WRONG);
    check_int("event GOT1 1: RIGHT",   lock_event(GOT1, '1'),   E_RIGHT);
    check_int("event GOT1 3: WRONG",   lock_event(GOT1, '3'),   E_WRONG);
    check_int("event GOT2 4: RIGHT",   lock_event(GOT2, '4'),   E_RIGHT);
    check_int("event GOT2 9: WRONG",   lock_event(GOT2, '9'),   E_WRONG);

    /* while open there is no next digit, so every digit is wrong */
    check_int("event OPEN 3: WRONG",   lock_event(OPEN, '3'),   E_WRONG);
    check_int("event OPEN 4: WRONG",   lock_event(OPEN, '4'),   E_WRONG);
    check_int("event OPEN 0: WRONG",   lock_event(OPEN, '0'),   E_WRONG);

    /* c and t in every state */
    for (int s = 0; s < N_STATES; s++) {
        char name[64];
        snprintf(name, sizeof name, "event %s c: CLOSE", state_name(s));
        check_int(name, lock_event(s, 'c'), E_CLOSE);
        snprintf(name, sizeof name, "event %s t: TIMEOUT", state_name(s));
        check_int(name, lock_event(s, 't'), E_TIMEOUT);
    }

    /* other characters are not events */
    check_int("event x: -1",       lock_event(LOCKED, 'x'),  -1);
    check_int("event C: -1",       lock_event(LOCKED, 'C'),  -1);
    check_int("event space: -1",   lock_event(GOT1, ' '),    -1);
    check_int("event newline: -1", lock_event(GOT2, '\n'),   -1);
    check_int("event NUL: -1",     lock_event(OPEN, '\0'),   -1);

    /* sequences */
    check_int("3 1 4 opens", run("314", &last), OPEN);
    check_int("3 1 4 last action UNLOCK", last, A_UNLOCK);

    check_int("3 3 1 4 does not open", run("3314", &last), LOCKED);
    check_int("3 3 1 4 last action BEEP", last, A_BEEP);

    check_int("3 1 t 4 does not open", run("31t4", &last), LOCKED);
    check_int("3 1 t 4 last action BEEP", last, A_BEEP);

    check_int("3 1 4 c locks again", run("314c", &last), LOCKED);
    check_int("3 1 4 c last action LOCK", last, A_LOCK);

    check_int("3 1 4 t locks again", run("314t", &last), LOCKED);
    check_int("3 c 1 4 still opens", run("3c14", &last), OPEN);
    check_int("3 1 4 9 stays open", run("3149", &last), OPEN);
    check_int("3 1 5 3 1 4 opens", run("315314", &last), OPEN);

    /* names, and "?" out of range */
    check_str("state_name GOT2",    state_name(GOT2),     "GOT2");
    check_str("state_name -1",      state_name(-1),       "?");
    check_str("state_name N_STATES", state_name(N_STATES), "?");
    check_str("event_name TIMEOUT", event_name(E_TIMEOUT), "TIMEOUT");
    check_str("event_name N_EVENTS", event_name(N_EVENTS), "?");
    check_str("action_name UNLOCK", action_name(A_UNLOCK), "UNLOCK");
    check_str("action_name N_ACTIONS", action_name(N_ACTIONS), "?");

    printf("\n%d of %d passed\n", total - failures, total);
    return failures == 0 ? 0 : 1;
}
