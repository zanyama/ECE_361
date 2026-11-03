#include <stdio.h>
#include "fsm.h"

/* create ints to track total number of tests and number of failures */
static int total = 0;
static int failures = 0;

/* check_int() borrowed from other test file: compares got and want */
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

/* checks one table entry: starting at state s with event e, the FSM should go to want_next and do want_action */
static void check_step(const char *name, fsm_state_t s, fsm_event_t e, fsm_state_t want_next, fsm_action_t want_action) {
    // buffer to store the print string
    char label[64];

    // fsm_step returns the next state and writes the action into action through &action
    fsm_action_t action;
    fsm_state_t next = fsm_step(s, e, &action);

    // check next state with expected state
    snprintf(label, sizeof label, "%s: next", name);
    check_int(label, next, want_next);

    // check produced action with expected action
    snprintf(label, sizeof label, "%s: action", name);
    check_int(label, action, want_action);
}

/* checks the 12 rows that change state, plus the SPEC decisions and one safety row (IDLE+TRIGGER_ON) */
int main(void) {
    check_step("IDLE+CARD",               STATE_IDLE,       EVENT_CARD,          STATE_AUTHORIZED, ACTION_START_SALE);
    check_step("IDLE+TRIGGER_ON",         STATE_IDLE,       EVENT_TRIGGER_ON,    STATE_IDLE,       ACTION_NONE);
    check_step("AUTHORIZED+TRIGGER_ON",   STATE_AUTHORIZED, EVENT_TRIGGER_ON,    STATE_PUMPING,    ACTION_MOTOR_ON);
    check_step("AUTHORIZED+HANG_UP",      STATE_AUTHORIZED, EVENT_HANG_UP,       STATE_IDLE,       ACTION_LAMP_OFF);
    check_step("AUTHORIZED+AUTH_TIMEOUT", STATE_AUTHORIZED, EVENT_AUTH_TIMEOUT,  STATE_IDLE,       ACTION_LAMP_OFF);
    check_step("AUTHORIZED+TEMP_HIGH",    STATE_AUTHORIZED, EVENT_TEMP_HIGH,     STATE_PAUSED,     ACTION_NONE);
    check_step("PUMPING+TRIGGER_OFF",     STATE_PUMPING,    EVENT_TRIGGER_OFF,   STATE_PAUSED,     ACTION_MOTOR_OFF);
    check_step("PUMPING+HANG_UP",         STATE_PUMPING,    EVENT_HANG_UP,       STATE_COMPLETE,   ACTION_BOTH_OFF);
    check_step("PUMPING+LIMIT_REACHED",   STATE_PUMPING,    EVENT_LIMIT_REACHED, STATE_COMPLETE,   ACTION_BOTH_OFF);
    check_step("PUMPING+TEMP_HIGH",       STATE_PUMPING,    EVENT_TEMP_HIGH,     STATE_PAUSED,     ACTION_MOTOR_OFF);
    check_step("PUMPING+CARD",            STATE_PUMPING,    EVENT_CARD,          STATE_PUMPING,    ACTION_NONE);
    check_step("PAUSED+AUTH_TIMEOUT",     STATE_PAUSED,     EVENT_AUTH_TIMEOUT,  STATE_COMPLETE,   ACTION_LAMP_OFF);
    check_step("PAUSED+TRIGGER_ON",       STATE_PAUSED,     EVENT_TRIGGER_ON,    STATE_PUMPING,    ACTION_MOTOR_ON);
    check_step("PAUSED+HANG_UP",          STATE_PAUSED,     EVENT_HANG_UP,       STATE_COMPLETE,   ACTION_LAMP_OFF);
    check_step("PAUSED+TEMP_OK",          STATE_PAUSED,     EVENT_TEMP_OK,       STATE_PAUSED,     ACTION_NONE);
    check_step("COMPLETE+DONE_TIMEOUT",   STATE_COMPLETE,   EVENT_DONE_TIMEOUT,  STATE_IDLE,       ACTION_NONE);

    printf("\n%d of %d tests passed\n", total - failures, total);
    return failures ? 1 : 0;
}
