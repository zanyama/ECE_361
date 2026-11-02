#include <stddef.h> /* NULL */
#include "fsm.h"

/* every state and event has an entry; none is left as ACTION_UNDEFINED.
   an entry that stays in its own state changes no LEDs, so it is NONE */
const fsm_transition_t fsm_table[STATE_COUNT][EVENT_COUNT] = {

    /* IDLE: only a card does anything; it starts the sale, lamp on */
    [STATE_IDLE] = {
        [EVENT_CARD]          = { STATE_AUTHORIZED, ACTION_START_SALE },
        [EVENT_TRIGGER_ON]    = { STATE_IDLE,       ACTION_NONE       },
        [EVENT_TRIGGER_OFF]   = { STATE_IDLE,       ACTION_NONE       },
        [EVENT_HANG_UP]       = { STATE_IDLE,       ACTION_NONE       },
        [EVENT_AUTH_TIMEOUT]  = { STATE_IDLE,       ACTION_NONE       },
        [EVENT_LIMIT_REACHED] = { STATE_IDLE,       ACTION_NONE       },
        [EVENT_DONE_TIMEOUT]  = { STATE_IDLE,       ACTION_NONE       },
        [EVENT_TEMP_HIGH]     = { STATE_IDLE,       ACTION_NONE       },
        [EVENT_TEMP_OK]       = { STATE_IDLE,       ACTION_NONE       },
    },

    /* AUTHORIZED: a squeeze starts the motor. hanging up or waiting
       60 ticks cancels the sale, lamp off. too hot: PAUSED, nothing
       changes but the color */
    [STATE_AUTHORIZED] = {
        [EVENT_CARD]          = { STATE_AUTHORIZED, ACTION_NONE     },
        [EVENT_TRIGGER_ON]    = { STATE_PUMPING,    ACTION_MOTOR_ON },
        [EVENT_TRIGGER_OFF]   = { STATE_AUTHORIZED, ACTION_NONE     },
        [EVENT_HANG_UP]       = { STATE_IDLE,       ACTION_LAMP_OFF },
        [EVENT_AUTH_TIMEOUT]  = { STATE_IDLE,       ACTION_LAMP_OFF },
        [EVENT_LIMIT_REACHED] = { STATE_AUTHORIZED, ACTION_NONE     },
        [EVENT_DONE_TIMEOUT]  = { STATE_AUTHORIZED, ACTION_NONE     },
        [EVENT_TEMP_HIGH]     = { STATE_PAUSED,     ACTION_NONE     },
        [EVENT_TEMP_OK]       = { STATE_AUTHORIZED, ACTION_NONE     },
    },

    /* PUMPING: letting go or too hot stops the motor and pauses.
       hanging up or reaching 40 liters ends the sale, both off */
    [STATE_PUMPING] = {
        [EVENT_CARD]          = { STATE_PUMPING,  ACTION_NONE      },
        [EVENT_TRIGGER_ON]    = { STATE_PUMPING,  ACTION_NONE      },
        [EVENT_TRIGGER_OFF]   = { STATE_PAUSED,   ACTION_MOTOR_OFF },
        [EVENT_HANG_UP]       = { STATE_COMPLETE, ACTION_BOTH_OFF  },
        [EVENT_AUTH_TIMEOUT]  = { STATE_PUMPING,  ACTION_NONE      },
        [EVENT_LIMIT_REACHED] = { STATE_COMPLETE, ACTION_BOTH_OFF  },
        [EVENT_DONE_TIMEOUT]  = { STATE_PUMPING,  ACTION_NONE      },
        [EVENT_TEMP_HIGH]     = { STATE_PAUSED,   ACTION_MOTOR_OFF },
        [EVENT_TEMP_OK]       = { STATE_PUMPING,  ACTION_NONE      },
    },

    /* PAUSED: a new squeeze resumes (main.c holds back TRIGGER_ON
       while it is too hot). hanging up ends the sale, lamp off */
    [STATE_PAUSED] = {
        [EVENT_CARD]          = { STATE_PAUSED,   ACTION_NONE     },
        [EVENT_TRIGGER_ON]    = { STATE_PUMPING,  ACTION_MOTOR_ON },
        [EVENT_TRIGGER_OFF]   = { STATE_PAUSED,   ACTION_NONE     },
        [EVENT_HANG_UP]       = { STATE_COMPLETE, ACTION_LAMP_OFF },
        [EVENT_AUTH_TIMEOUT]  = { STATE_PAUSED,   ACTION_NONE     },
        [EVENT_LIMIT_REACHED] = { STATE_PAUSED,   ACTION_NONE     },
        [EVENT_DONE_TIMEOUT]  = { STATE_PAUSED,   ACTION_NONE     },
        [EVENT_TEMP_HIGH]     = { STATE_PAUSED,   ACTION_NONE     },
        [EVENT_TEMP_OK]       = { STATE_PAUSED,   ACTION_NONE     },
    },

    /* COMPLETE: shows the finished sale for 5 ticks, then back to IDLE */
    [STATE_COMPLETE] = {
        [EVENT_CARD]          = { STATE_COMPLETE, ACTION_NONE },
        [EVENT_TRIGGER_ON]    = { STATE_COMPLETE, ACTION_NONE },
        [EVENT_TRIGGER_OFF]   = { STATE_COMPLETE, ACTION_NONE },
        [EVENT_HANG_UP]       = { STATE_COMPLETE, ACTION_NONE },
        [EVENT_AUTH_TIMEOUT]  = { STATE_COMPLETE, ACTION_NONE },
        [EVENT_LIMIT_REACHED] = { STATE_COMPLETE, ACTION_NONE },
        [EVENT_DONE_TIMEOUT]  = { STATE_IDLE,     ACTION_NONE },
        [EVENT_TEMP_HIGH]     = { STATE_COMPLETE, ACTION_NONE },
        [EVENT_TEMP_OK]       = { STATE_COMPLETE, ACTION_NONE },
    },
};

fsm_state_t fsm_step(fsm_state_t s, fsm_event_t e, fsm_action_t *action) {
    // table lookup: the table says where to go
    const fsm_transition_t *t = &fsm_table[s][e];

    // caller can pass NULL if it just wants the next state
    if (action != NULL) {
        *action = t->action;
    }

    return t->next;
}
