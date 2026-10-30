#ifndef LOCK_H
#define LOCK_H

/* the combination lock: the code is 3-1-4 */

/* where the lock is: nothing typed yet, one or two right digits, or open */
typedef enum { LOCKED, GOT1, GOT2, OPEN, N_STATES } state_t;

/* what can happen: the next digit of the code, any other digit,
   the door closing, or the 5-second timer running out */
typedef enum { E_RIGHT, E_WRONG, E_CLOSE, E_TIMEOUT, N_EVENTS } event_t;

/* what the lock does on a transition; A_UNDEFINED is 0, so an entry
   left out of the table shows up as undefined instead of as A_NONE */
typedef enum { A_UNDEFINED, A_NONE, A_UNLOCK, A_LOCK, A_BEEP, N_ACTIONS } action_t;

/* one table entry: the state to go to and the action to take */
typedef struct { state_t next; action_t action; } transition_t;

/* the transition table: lock_table[state][event] */
extern const transition_t lock_table[N_STATES][N_EVENTS];

/*
lock_step: takes one transition, by looking it up in lock_table

    Takes:   the current state, an event, and where to put the action
    Returns: the next state; *action is set to the transition's action
             (if action is not NULL)
    Assumes: s < N_STATES and e < N_EVENTS
*/
state_t lock_step(state_t s, event_t e, action_t *action);

/*
lock_event: turns a character into an event for state s

    Takes:   the current state and a character
    Returns: E_RIGHT if c is the next digit of the code in state s,
             E_WRONG for any other digit, E_CLOSE for 'c', E_TIMEOUT
             for 't', and -1 for anything else
    Assumes: nothing
*/
int lock_event(state_t s, char c);

/*
state_name, event_name, action_name: the name of a state, event, or action

    Takes:   a state, event, or action
    Returns: its name, e.g. "GOT2", "RIGHT", "UNLOCK"; "?" if out of range
    Assumes: nothing
*/
const char *state_name(state_t s);
const char *event_name(event_t e);
const char *action_name(action_t a);

#endif
