/* lock.c: see lock.h for each function's contract */

#include <stddef.h>     /* NULL */
#include "lock.h"

/* the code, one digit per state: code[s] is the right digit in state s.
   code[OPEN] is the '\0', which no digit matches, so while open every
   digit is E_WRONG (and the table makes that do nothing) */
static const char code[] = "314";

/* every state and event has an entry; none is left as A_UNDEFINED */
const transition_t lock_table[N_STATES][N_EVENTS] = {
    [LOCKED] = {
        [E_RIGHT]   = { GOT1,   A_NONE   },
        [E_WRONG]   = { LOCKED, A_BEEP   },
        [E_CLOSE]   = { LOCKED, A_NONE   },  /* already locked */
        [E_TIMEOUT] = { LOCKED, A_NONE   },  /* no timer runs here */
    },
    [GOT1] = {
        [E_RIGHT]   = { GOT2,   A_NONE   },
        [E_WRONG]   = { LOCKED, A_BEEP   },  /* start over */
        [E_CLOSE]   = { GOT1,   A_NONE   },  /* typing: changes nothing */
        [E_TIMEOUT] = { LOCKED, A_NONE   },  /* start over, silently */
    },
    [GOT2] = {
        [E_RIGHT]   = { OPEN,   A_UNLOCK },
        [E_WRONG]   = { LOCKED, A_BEEP   },  /* start over */
        [E_CLOSE]   = { GOT2,   A_NONE   },  /* typing: changes nothing */
        [E_TIMEOUT] = { LOCKED, A_NONE   },  /* start over, silently */
    },
    [OPEN] = {
        [E_RIGHT]   = { OPEN,   A_NONE   },  /* digits do nothing */
        [E_WRONG]   = { OPEN,   A_NONE   },
        [E_CLOSE]   = { LOCKED, A_LOCK   },
        [E_TIMEOUT] = { LOCKED, A_LOCK   },
    },
};

/* names indexed by the enums, so the order must match lock.h */
static const char *const state_names[N_STATES] = {
    "LOCKED", "GOT1", "GOT2", "OPEN"
};

static const char *const event_names[N_EVENTS] = {
    "RIGHT", "WRONG", "CLOSE", "TIMEOUT"
};

static const char *const action_names[N_ACTIONS] = {
    "UNDEFINED", "NONE", "UNLOCK", "LOCK", "BEEP"
};

state_t lock_step(state_t s, event_t e, action_t *action) {
    // one lookup: the table says where to go and what to do
    const transition_t *t = &lock_table[s][e];

    if (action != NULL) {
        *action = t->action;
    }

    return t->next;
}

int lock_event(state_t s, char c) {
    if (c >= '0' && c <= '9') {
        // only LOCKED, GOT1, GOT2 have a next digit; OPEN or a bad state never match
        if ((unsigned) s < OPEN && c == code[s]) {
            return E_RIGHT;
        }
        return E_WRONG;
    }

    if (c == 'c') {
        return E_CLOSE;
    }

    if (c == 't') {
        return E_TIMEOUT;
    }

    return -1;
}

/* the unsigned cast also catches a negative value in one compare */
const char *state_name(state_t s) {
    return (unsigned) s < N_STATES ? state_names[s] : "?";
}

const char *event_name(event_t e) {
    return (unsigned) e < N_EVENTS ? event_names[e] : "?";
}

const char *action_name(action_t a) {
    return (unsigned) a < N_ACTIONS ? action_names[a] : "?";
}
