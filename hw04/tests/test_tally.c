#include <stdio.h>
#include <string.h>
#include "tally.h"

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

/* passes if got is NULL when want_null is 1, or not NULL when it is 0 */
static void check_null(const char *name, const tally_t *got, int want_null) {
    total++;
    if ((got == NULL) == want_null) {
        printf("PASS %s\n", name);
    }
    else {
        printf("FAIL %s: got %s, want %s\n", name,
               got == NULL ? "NULL" : "a tally",
               want_null ? "NULL" : "a tally");
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

/* runs tally_print into a temporary file and reads it back into buf,
   so the test can compare what was printed */
static void print_to(const tally_t *t, char *buf, size_t size) {
    buf[0] = '\0';

    FILE *f = tmpfile();
    if (f == NULL) {
        return;
    }

    tally_print(t, f);
    rewind(f);

    size_t got = fread(buf, 1, size - 1, f);
    buf[got] = '\0';

    fclose(f);
}

int main(void) {
    char out[512];

    /* counting up: each add returns the new count */
    tally_t *t = tally_create();
    check_null("create: a tally", t, 0);
    check_int("new tally: size 0", tally_size(t), 0);
    check_int("add door: 1", tally_add(t, "door"), 1);
    check_int("add door: 2", tally_add(t, "door"), 2);
    check_int("add door: 3", tally_add(t, "door"), 3);
    check_int("count door: 3", tally_count(t, "door"), 3);
    check_int("size after one name: 1", tally_size(t), 1);

    /* a name never added */
    check_int("count never added: 0", tally_count(t, "window"), 0);

    /* empty name is refused and changes nothing */
    check_int("add empty: -1", tally_add(t, ""), -1);
    check_int("add empty: size unchanged", tally_size(t), 1);

    /* exactly TALLY_LEN characters fits, TALLY_LEN + 1 does not */
    check_int("add 15 chars: 1", tally_add(t, "abcdefghijklmno"), 1);
    check_int("add 16 chars: -1", tally_add(t, "abcdefghijklmnop"), -1);
    check_int("count 16 chars: 0", tally_count(t, "abcdefghijklmnop"), 0);
    check_int("size after long names: 2", tally_size(t), 2);

    /* NULL arguments */
    check_int("add NULL tally: -1", tally_add(NULL, "door"), -1);
    check_int("add NULL name: -1", tally_add(t, NULL), -1);
    check_int("count NULL tally: 0", tally_count(NULL, "door"), 0);
    check_int("count NULL name: 0", tally_count(t, NULL), 0);
    check_int("size NULL: 0", tally_size(NULL), 0);
    print_to(NULL, out, sizeof out);
    check_str("print NULL tally: nothing", out, "");
    tally_print(t, NULL);       // must not crash
    tally_destroy(NULL);        // must not crash
    check_int("NULL calls left the tally alone", tally_size(t), 2);

    tally_destroy(t);

    /* print order: names come out in strcmp order, whatever order they
       went in, with their counts moved along with them */
    t = tally_create();
    tally_add(t, "zone");
    tally_add(t, "door");
    tally_add(t, "alarm");      // goes in front: everything moves up
    tally_add(t, "bell");       // goes in the middle
    tally_add(t, "door");
    tally_add(t, "Zone");       // uppercase sorts before all lowercase
    print_to(t, out, sizeof out);
    check_str("print order", out,
              "Zone 1\nalarm 1\nbell 1\ndoor 2\nzone 1\n");
    tally_destroy(t);

    /* full tally: a new name is refused, an existing one still counts.
       Names go in backward, so each one is inserted at the front */
    t = tally_create();
    char name[TALLY_LEN + 1];
    for (int i = TALLY_NAMES - 1; i >= 0; i--) {
        snprintf(name, sizeof name, "n%02d", i);
        tally_add(t, name);
    }
    check_int("full: size TALLY_NAMES", tally_size(t), TALLY_NAMES);
    check_int("full: new name -1", tally_add(t, "new"), -1);
    check_int("full: size unchanged", tally_size(t), TALLY_NAMES);
    check_int("full: new name not counted", tally_count(t, "new"), 0);
    check_int("full: existing name 2", tally_add(t, "n00"), 2);
    check_int("full: last name 2", tally_add(t, "n15"), 2);
    tally_destroy(t);

    /* TALLY_MAX tallies, then one more is NULL */
    tally_t *pool[TALLY_MAX];
    for (int i = 0; i < TALLY_MAX; i++) {
        pool[i] = tally_create();
        check_null("create up to TALLY_MAX: a tally", pool[i], 0);
    }
    check_null("create one more: NULL", tally_create(), 1);

    /* create after destroy: the freed tally is handed out again, empty */
    tally_add(pool[1], "door");
    tally_destroy(pool[1]);
    pool[1] = tally_create();
    check_null("create after destroy: a tally", pool[1], 0);
    check_int("create after destroy: size 0", tally_size(pool[1]), 0);
    check_int("create after destroy: old name gone",
              tally_count(pool[1], "door"), 0);

    for (int i = 0; i < TALLY_MAX; i++) {
        tally_destroy(pool[i]);
    }

    /* two tallies are independent */
    tally_t *a = tally_create();
    tally_t *b = tally_create();
    tally_add(a, "door");
    tally_add(a, "door");
    tally_add(b, "door");
    check_int("independent: a door 2", tally_count(a, "door"), 2);
    check_int("independent: b door 1", tally_count(b, "door"), 1);
    tally_add(a, "alarm");
    check_int("independent: b has no alarm", tally_count(b, "alarm"), 0);
    check_int("independent: a size 2", tally_size(a), 2);
    check_int("independent: b size 1", tally_size(b), 1);
    tally_destroy(a);
    tally_destroy(b);

    printf("\n%d of %d tests passed\n", total - failures, total);
    return failures ? 1 : 0;
}
