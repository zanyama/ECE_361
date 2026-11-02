/* lock.c: see lock.h for each function's contract */

#include <stddef.h>     /* NULL */
#include "lock.h"

/* the code- one digit per state: code[s] is the right digit in state s.
   code[OPEN] is the '\0', which no digit matches, so while open every
   digit is E_WRONG (and the table makes that do nothing!) */
static const char code[] = "314";

/* every state and event has an entry; none is left as A_UNDEFINED */
const transition_t lock_table[N_STATES][N_EVENTS] = {

    [LOCKED] = {
        [E_RIGHT]   = { GOT1,   A_NONE   },  /* correct, move to 2nd digit */
        [E_WRONG]   = { LOCKED, A_BEEP   },  /* incorrect: beep, stay locked */
        [E_CLOSE]   = { LOCKED, A_NONE   },  /* already locked */
        [E_TIMEOUT] = { LOCKED, A_NONE   },  /* no timer runs here */
    },

    [GOT1] = {
        [E_RIGHT]   = { GOT2,   A_NONE   },  /* correct, move to 3rd digit */
        [E_WRONG]   = { LOCKED, A_BEEP   },  /* incorrect, start over */
        [E_CLOSE]   = { GOT1,   A_NONE   },  /* typing: changes nothing */
        [E_TIMEOUT] = { LOCKED, A_NONE   },  /* start over, silently */
    },

    [GOT2] = {
        [E_RIGHT]   = { OPEN,   A_UNLOCK },  /* correct, unlock lock!! */
        [E_WRONG]   = { LOCKED, A_BEEP   },  /* incorrect, start over */
        [E_CLOSE]   = { GOT2,   A_NONE   },  /* typing: changes nothing */
        [E_TIMEOUT] = { LOCKED, A_NONE   },  /* start over, silently */
    },

    [OPEN] = {
        [E_RIGHT]   = { OPEN,   A_NONE   },  /* digits do nothing */
        [E_WRONG]   = { OPEN,   A_NONE   },
        [E_CLOSE]   = { LOCKED, A_LOCK   },  /* goes back to locked, upon */
        [E_TIMEOUT] = { LOCKED, A_LOCK   },  /* closing or timeout */
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


/* given the current state and an event, returns the next state and
   writes the action into *action */
state_t lock_step(state_t s, event_t e, action_t *action) {

    // one lookup: the table says where to go
    const transition_t *t = &lock_table[s][e];

    // the caller can pass NULL if it only wants the next state
    if (action != NULL) {
        *action = t->action;
    }

    return t->next;
}

/* given a state and a character, returns the event that character
   means in that state, or -1 if it isn't an event */
int lock_event(state_t s, char c) {

    // a digit: right if it is the next one in the code, wrong otherwise
    if (c >= '0' && c <= '9') {

        /* only LOCKED, GOT1, GOT2 have a next digit; OPEN or a bad state
           never match */
        if ((unsigned) s < OPEN && c == code[s]) {
            return E_RIGHT;
        }
        // otherwise register wrong input
        return E_WRONG;
    }

    // the door closing
    if (c == 'c') {
        return E_CLOSE;
    }

    // the 5-second timer running out
    if (c == 't') {
        return E_TIMEOUT;
    }

    // anything else (spaces, newlines, other letters) is not an event
    return -1;
}

/* for all three: the unsigned cast turns a negative value into a huge one,
   so one compare catches both too big and negative */
const char *state_name(state_t s) {
    return (unsigned) s < N_STATES ? state_names[s] : "?";
}

const char *event_name(event_t e) {
    return (unsigned) e < N_EVENTS ? event_names[e] : "?";
}

const char *action_name(action_t a) {
    return (unsigned) a < N_ACTIONS ? action_names[a] : "?";
}
