#include <stdio.h>
#include "reading.h"

#define LINE_LEN 128

reading_result_t reading_read(FILE *in, int ticks[], float temps[], float hums[])
{
    reading_result_t r = {0, 0};
    char line[LINE_LEN];

    while (fgets(line, sizeof line, in) != NULL) {
        int i = 0;
        while (line[i] == ' ' || line[i] == '\t')
            i++;
        if (line[i] == '\n' || line[i] == '\0' || line[i] == '#')
            continue;
        if (r.count == READING_MAX) {
            fprintf(stderr, "warning: more than %d readings, the rest are ignored\n", READING_MAX);
            break;
        }
        if (sscanf(line, "%d %f %f", &ticks[r.count], &temps[r.count], &hums[r.count]) == 3)
            r.count++;
        else
            r.skipped++;
    }
    return r;
}
