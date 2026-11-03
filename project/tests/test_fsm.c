#include <stdio.h>
#include "fsm.h"

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


static void check_step(const char *name, fsm_state_t s, fsm_event_t e, fsm_state_t want_next, fsm_action_t want_action) {
    fsm_action_t action;
    fsm_state_t next = fsm_step(s, e, &action);
    check_int(name, next, want_next);
    check_int(name, action, want_action);
}


int main(void) {
    check_step("IDLE+CARD", STATE_IDLE, EVENT_CARD, STATE_AUTHORIZED, ACTION_START_SALE);
    check_step("PUMPING+TEMP_HIGH", STATE_PUMPING, EVENT_TEMP_HIGH, STATE_PAUSED, ACTION_MOTOR_OFF);
    printf("\n%d of %d tests passed\n", total - failures,total);
    return failures ? 1 : 0;
}
