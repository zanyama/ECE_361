/* check.c: see check.h for fsm_check's contract */

#include <stdio.h>      /* FILE, fprintf */
#include "check.h"

/* 1 if the entry can be followed: it was filled in and goes to a state */
static int usable(const transition_t *t) {
    return t->action != A_UNDEFINED && (unsigned) t->next < N_STATES;
}

/* the entries themselves: left out, or a next state out of range */
static int check_entries(const transition_t table[N_STATES][N_EVENTS],
                         FILE *out) {
    int problems = 0;

    for (int s = 0; s < N_STATES; s++) {
        for (int e = 0; e < N_EVENTS; e++) {
            const transition_t *t = &table[s][e];

            if (t->action == A_UNDEFINED) {
                if (out != NULL) {
                    fprintf(out, "undefined: %s %s\n", state_name(s),
                            event_name(e));
                }
                problems++;
            }
            else if ((unsigned) t->next >= N_STATES) {
                if (out != NULL) {
                    fprintf(out, "out of range: %s %s -> %d\n",
                            state_name(s), event_name(e), (int) t->next);
                }
                problems++;
            }
        }
    }

    return problems;
}

/* breadth first from start: every state no path reaches is a problem */
static int check_reachable(const transition_t table[N_STATES][N_EVENTS],
                           state_t start, FILE *out) {
    int seen[N_STATES] = { 0 };
    state_t queue[N_STATES];    // each state goes in at most once
    int head = 0;
    int tail = 0;

    seen[start] = 1;
    queue[tail++] = start;

    // take the oldest state off the queue and add every new state it leads to
    while (head < tail) {
        state_t s = queue[head++];

        for (int e = 0; e < N_EVENTS; e++) {
            const transition_t *t = &table[s][e];

            if (usable(t) && !seen[t->next]) {
                seen[t->next] = 1;
                queue[tail++] = t->next;
            }
        }
    }

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
        int way_out = 0;

        for (int e = 0; e < N_EVENTS; e++) {
            const transition_t *t = &table[s][e];

            if (usable(t) && t->next != (state_t) s) {
                way_out = 1;
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

    return problems + check_way_out(table, out);
}
