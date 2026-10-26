#ifndef FSM_H
#define FSM_H

/*
 * fsm: the state machine of the fuel pump.
 * Will define the event enum, the transition table, and one function
 * that takes the current state and an event and returns the next state.
 */

/* the five states the project sheet gives; IDLE is the initial state */
typedef enum {
    STATE_IDLE,
    STATE_AUTHORIZED,
    STATE_PUMPING,
    STATE_PAUSED,
    STATE_COMPLETE
} fsm_state_t;

#endif
