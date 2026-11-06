#ifndef SCENARIO_H
#define SCENARIO_H

#include <stdint.h>
#include <time.h>

#define SCENARIO_MAX_LINES 1000

/*
scenario: loads a scenario file and finds the line in force at each tick
format: "start <timestamp>", then "tick temp rh switches" lines (switches in hex),
        # starts a comment
*/

/* one line of a scenario: when, the temp, %RH, and switch state */
typedef struct {
    int tick;
    float  temp_c;
    float  rh_pct;
    uint32_t switches;
} scenario_line_t;

/* a whole scenario: start time, how many lines were loaded, and the lines */
typedef struct {
    time_t start;
    int num_lines;
    scenario_line_t lines[SCENARIO_MAX_LINES];
} scenario_t;

/*
scenario_load: reads a scenario file into *scenario

    Takes:   file: the scenario file name
             scenario: where the loaded scenario is stored
    Returns: 0 on success
             -1 if the file can't be opened
             the number of the first bad line, counting every line from 1
             bad lines include: no start line, missing field(s), tick not
                                increasing, more than SCENARIO_MAX_LINES data
                                lines, first tick not 0, no data lines
    Assumes: file and scenario are not NULL
*/
int scenario_load(const char *file, scenario_t *scenario);

/*
scenario_at: finds the line at a tick value (values hold between lines)

    Takes:   scenario: the loaded scenario
             tick: the second to look up
    Returns: the index of the last line whose tick is <= tick
    Assumes: scenario was loaded properly (so num_lines >= 1
             and lines[0].tick == 0)
             tick >= 0
*/
int scenario_at(const scenario_t *scenario, int tick);

#endif
