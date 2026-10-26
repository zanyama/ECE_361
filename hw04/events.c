/* events.c: counts the event names read from standard input

   Usage: ./events < data/pump.txt

   Trailing spaces and tabs are removed, then blank lines and lines
   starting with # are skipped. Every other line is added to one tally,
   then the tally is printed, followed by "names: N, lines skipped: S",
   where S counts the lines tally_add refused. */

#include <stdio.h>      /* fgets, printf, fprintf, getchar, feof */
#include <string.h>     /* strchr, strcspn, strlen */
#include "tally.h"

#define LINE_LEN 64     /* longest line read at once, longer than TALLY_LEN */

int main(void) {
    tally_t *t = tally_create();
    if (t == NULL) {
        fprintf(stderr, "events: no tally available\n");
        return 1;
    }

    char line[LINE_LEN];
    int skipped = 0;

    while (fgets(line, sizeof line, stdin) != NULL) {

        /* no newline means the line did not fit: throw away the rest of
           it so it isn't read as more lines (the part we have is too long
           for a name, so tally_add refuses it below) */
        if (strchr(line, '\n') == NULL && !feof(stdin)) {
            int c;
            while ((c = getchar()) != '\n' && c != EOF) {
            }
        }

        // remove the newline (and a '\r' from files saved on Windows)
        line[strcspn(line, "\r\n")] = '\0';

        // remove trailing spaces and tabs, so "CARD " counts as CARD
        size_t len = strlen(line);
        while (len > 0 && (line[len - 1] == ' ' || line[len - 1] == '\t')) {
            len--;
            line[len] = '\0';
        }

        // skip blank lines and comments
        if (line[0] == '\0' || line[0] == '#') {
            continue;
        }

        if (tally_add(t, line) == -1) {
            skipped++;
        }
    }

    tally_print(t, stdout);
    printf("names: %d, lines skipped: %d\n", tally_size(t), skipped);

    tally_destroy(t);
    return 0;
}
