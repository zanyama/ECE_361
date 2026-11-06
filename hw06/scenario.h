/*
format:
start 1791273600                # 2026-10-06 08:00:00 UTC
# tick  temp   rh    switches
0       25.0   50    0x0
5       25.0   50    0x1      # CARD pressed

*/
#ifndef SCENARIO_H
#define SCENARIO_H

#include <stdint.h>
#include <time.h>

#define SCENARIO_MAX_LINES 1000

/* one line of a scenario: when, the temp, %RH, and switch state */

typedef struct {
    int tick;
    float  temp_c;
    float  rh_pct;
    uint32_t switches;
} scenario_line_t;


typedef struct {
    time_t start;
    int num_lines;
    scenario_line_t lines[SCENARIO_MAX_LINES];
} scenario_t;

/* scenario_load:
        Takes:   the filename and the scenario to load
        Returns: 0 on success
                 -1 on file error
                 # of first bad line, counting every line from 1
                 bad lines include: no start line, missing field(s), tick not
                                    increasing, more than max lines, first
                                    tick not 0, no data lines
        Assumes: file and scenario are not NULL

*/

int scenario_load(const char *file, scenario_t *scenario);

/* scenario_at:
        Takes:   pointer to a scenario and the tick in question
        Returns: the index of the last line whose tick is <= tick
        Assumes: scenario was loaded properly (so num_lines >= 1
                 and lines[0].tick == 0)
                 tick >= 0

*/

int scenario_at(const scenario_t *scenario, int tick);

#endif
