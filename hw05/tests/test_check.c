#include <stdio.h>
#include <string.h>
#include "lock.h"
#include "check.h"

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

static void check_str(const char *name, const char *got, const char *want) {
    total++;
    if (strcmp(got, want) == 0) {
        printf("PASS %s\n", name);
    }
    else {
        printf("FAIL %s:\n--- got ---\n%s--- want ---\n%s", name, got, want);
        failures++;
    }
}

/* runs fsm_check into a temporary file and reads it back into buf,
   so the test can compare what was printed; returns the count */
static int check_to(const transition_t table[N_STATES][N_EVENTS],
                    state_t start, char *buf, size_t size) {
    buf[0] = '\0';

    FILE *f = tmpfile();
    if (f == NULL) {
        return -1;
    }

    int n = fsm_check(table, start, f);
    rewind(f);

    size_t got = fread(buf, 1, size - 1, f);
    buf[got] = '\0';

    fclose(f);
    return n;
}

/* checks one table: the count, then the exact output, which is also
   printed indented so it can be copied into BONUS.md */
static void check_table(const char *name,
                        const transition_t table[N_STATES][N_EVENTS],
                        state_t start, int want_n, const char *want_out) {
    char out[1024];
    char label[128];

    int n = check_to(table, start, out, sizeof out);

    snprintf(label, sizeof label, "%s: %d problems", name, want_n);
    check_int(label, n, want_n);
    snprintf(label, sizeof label, "%s: output", name);
    check_str(label, out, want_out);

    // show what the checker printed
    for (char *line = strtok(out, "\n"); line != NULL; line = strtok(NULL, "\n")) {
        printf("    %s\n", line);
    }
}

int main(void) {
    transition_t t[N_STATES][N_EVENTS];

    /* the real table has no problems */
    check_table("lock_table", lock_table, LOCKED, 0, "");

    /* broken 1: GOT2 + RIGHT left out, so the lock can never open */
    memcpy(t, lock_table, sizeof t);
    t[GOT2][E_RIGHT] = (transition_t) { 0 };
    check_table("GOT2 RIGHT left out", t, LOCKED, 2,
                "undefined: GOT2 RIGHT\n"
                "unreachable: OPEN\n");

    /* broken 2: OPEN never goes anywhere else, so it never locks again */
    memcpy(t, lock_table, sizeof t);
    t[OPEN][E_CLOSE] = (transition_t) { OPEN, A_LOCK };
    t[OPEN][E_TIMEOUT] = (transition_t) { OPEN, A_LOCK };
    check_table("OPEN stuck", t, LOCKED, 1,
                "no way out: OPEN\n");

    /* broken 3: a typo in a next state (4 is N_STATES, not a state) */
    memcpy(t, lock_table, sizeof t);
    t[GOT1][E_RIGHT].next = N_STATES;
    check_table("GOT1 RIGHT out of range", t, LOCKED, 3,
                "out of range: GOT1 RIGHT -> 4\n"
                "unreachable: GOT2\n"
                "unreachable: OPEN\n");

    /* GOT2 + RIGHT goes to LOCKED instead: every entry is fine on its
       own, but OPEN can't be reached */
    memcpy(t, lock_table, sizeof t);
    t[GOT2][E_RIGHT] = (transition_t) { LOCKED, A_BEEP };
    check_table("GOT2 RIGHT to LOCKED", t, LOCKED, 1,
                "unreachable: OPEN\n");

    /* a bad start: no search, but the entries and ways out are checked */
    check_table("start out of range", lock_table, N_STATES, 1,
                "bad start: 4\n");

    /* the same table from another start: LOCKED is still reachable */
    check_table("start at OPEN", lock_table, OPEN, 0, "");

    /* an empty table: every entry undefined, only start reached,
       and no state has a way out */
    memset(t, 0, sizeof t);
    check_int("empty table: 16 + 3 + 4 problems",
              fsm_check(t, LOCKED, NULL), 23);

    /* NULL out prints nothing but still counts */
    memcpy(t, lock_table, sizeof t);
    t[OPEN][E_CLOSE] = (transition_t) { OPEN, A_LOCK };
    t[OPEN][E_TIMEOUT] = (transition_t) { OPEN, A_LOCK };
    check_int("NULL out: still counts", fsm_check(t, LOCKED, NULL), 1);

    printf("\n%d of %d passed\n", total - failures, total);
    return failures == 0 ? 0 : 1;
}
