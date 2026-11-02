/* check.c: see check.h for fsm_check's contract */

#include <stdio.h>      /* FILE, fprintf */
#include "check.h"

/* 1 if the entry is usable and can be followed: it was filled in and
goes to a valid state */
static int usable(const transition_t *t) {
    return t->action != A_UNDEFINED && (unsigned) t->next < N_STATES;
}

/* checks the entries in a table. prints the problem cells in output file.
returns number of problem cells */
static int check_entries(const transition_t table[N_STATES][N_EVENTS],
                         FILE *out) {
    int problems = 0;

    // loop through each cell in the table
    for (int s = 0; s < N_STATES; s++) {
        for (int e = 0; e < N_EVENTS; e++) {
            const transition_t *t = &table[s][e];

            // "left out"- if action is undefined
            if (t->action == A_UNDEFINED) {
                if (out != NULL) {
                    fprintf(out, "undefined: %s %s\n", state_name(s),
                            event_name(e));
                }
                problems++;
            }

            /* else if, so an entry that is left out and also out of range
               counts as one problem, not two */
            else if ((unsigned) t->next >= N_STATES) {
                if (out != NULL) {
                    fprintf(out, "out of range: %s %s -> %d\n",
                            state_name(s), event_name(e), (int) t->next);
                }
                problems++;
            }
        }
    }
    // returns number of problems, logged in out
    return problems;
}

/* breadth first from start: every state no path reaches is a problem */
static int check_reachable(const transition_t table[N_STATES][N_EVENTS],
                           state_t start, FILE *out) {
    int seen[N_STATES] = { 0 };
    state_t queue[N_STATES];    // each state goes in at most once
    int head = 0;               // the next state to take off the queue
    int tail = 0;               // where the next new state goes on

    // start counts as reached, and is the first state to look at
    seen[start] = 1;
    queue[tail++] = start;      // put it at tail, then move tail up one

    // take the oldest state off the queue and add every new state it leads to
    while (head < tail) {
        state_t s = queue[head++];  // take it from head, then move head up one

        for (int e = 0; e < N_EVENTS; e++) {
            const transition_t *t = &table[s][e];

            /* mark it seen when it goes on, not when it comes off, so it
               can't be added twice and overflow the queue */
            if (usable(t) && !seen[t->next]) {
                seen[t->next] = 1;
                queue[tail++] = t->next;
            }
        }
    }

    // any state the search never got to is unreachable
    int problems = 0;
    for (int s = 0; s < N_STATES; s++) {
        if (!seen[s]) {
            if (out != NULL) {
                fprintf(out, "unreachable: %s\n", state_name(s));
            }
            problems++;
        }
    }

    return problems;
}

/* a state is stuck if no usable entry leads to a different state */
static int check_way_out(const transition_t table[N_STATES][N_EVENTS],
                         FILE *out) {
    int problems = 0;

    for (int s = 0; s < N_STATES; s++) {
        int way_out = 0;    // stuck until an entry shows otherwise

        for (int e = 0; e < N_EVENTS; e++) {
            const transition_t *t = &table[s][e];

            // an entry that goes to a different state (cast: s is an int)
            if (usable(t) && t->next != (state_t) s) {
                way_out = 1;
                break;      // one is enough
            }
        }

        if (!way_out) {
            if (out != NULL) {
                fprintf(out, "no way out: %s\n", state_name(s));
            }
            problems++;
        }
    }

    return problems;
}

int fsm_check(const transition_t table[N_STATES][N_EVENTS], state_t start,
              FILE *out) {
    int problems = check_entries(table, out);

    // without a real start there is nothing to search from
    if ((unsigned) start >= N_STATES) {
        if (out != NULL) {
            fprintf(out, "bad start: %d\n", (int) start);
        }
        problems++;
    }
    else {
        problems += check_reachable(table, start, out);
    }

    // the ways out don't depend on start, so they are always checked
    return problems + check_way_out(table, out);
}
