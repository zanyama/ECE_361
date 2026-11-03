#ifndef FSM_H
#define FSM_H

/* fsm: the Finite State Machine of the fuel pump.
   Defines the event, state, and action enums, the transition table, and fsm_step() */

/* the 5 states the project sheet gives; STATE_IDLE is the initial state */
typedef enum {
    STATE_IDLE,
    STATE_AUTHORIZED,
    STATE_PUMPING,
    STATE_PAUSED,
    STATE_COMPLETE,
    STATE_COUNT
} fsm_state_t;

/* the 9 events */
typedef enum {
    EVENT_CARD,
    EVENT_TRIGGER_ON,
    EVENT_TRIGGER_OFF,
    EVENT_HANG_UP,
    EVENT_AUTH_TIMEOUT,
    EVENT_LIMIT_REACHED,
    EVENT_DONE_TIMEOUT,
    EVENT_TEMP_HIGH,
    EVENT_TEMP_OK,
    EVENT_COUNT
} fsm_event_t;

/* The 7 actions. UNDEFINED is first, so an entry left out shows up as UNDEFINED,
   instead of a real action */
typedef enum {
    ACTION_UNDEFINED,
    ACTION_START_SALE,
    ACTION_MOTOR_ON,
    ACTION_MOTOR_OFF,
    ACTION_BOTH_OFF,
    ACTION_LAMP_OFF,
    ACTION_NONE,
    ACTION_COUNT
} fsm_action_t;

/* a single table entry: the state to go to next and the action to take */
typedef struct { fsm_state_t next; fsm_action_t action; } fsm_transition_t;

/* the transition table:      fsm_table[state][event] */
extern const fsm_transition_t fsm_table[STATE_COUNT][EVENT_COUNT];

/* fsm_step: takes one transition, by looking it up in fsm table

    Takes:   the current state, an event, and where to put the action
    Returns: the next state; *action is set to the transition's action
             (if action is not NULL)
    Assumes: s < STATE_COUNT and e < EVENT_COUNT */
fsm_state_t fsm_step(fsm_state_t s, fsm_event_t e, fsm_action_t *action);

#endif
