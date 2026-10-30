#ifndef CHECK_H
#define CHECK_H

#include <stdio.h>      /* FILE, for fsm_check */
#include "lock.h"       /* state_t, transition_t, N_STATES, N_EVENTS */

/*
fsm_check: looks for problems in a transition table

    Takes:   a table, the start state, and a stream for the report
    Returns: how many problems it found, 0 if none. One line is printed
             per problem:
               undefined: STATE EVENT        an entry left out (A_UNDEFINED)
               out of range: STATE EVENT -> N  a next state that is not a state
               unreachable: STATE            no path from start (breadth first)
               no way out: STATE             every entry stays in STATE
               bad start: N                  start is not a state
             An undefined or out-of-range entry is not followed when
             looking for paths or ways out. Nothing is printed if out is
             NULL, but problems are still counted.
    Assumes: table has N_STATES rows of N_EVENTS entries
*/
int fsm_check(const transition_t table[N_STATES][N_EVENTS], state_t start,
              FILE *out);

#endif
