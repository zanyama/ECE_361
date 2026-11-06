#include "scenario.h"
#include <stdio.h>

// index of the line in effect, at this tick
int scenario_at(const scenario_t *scenario, int tick) {

    // line 0 is tick 0 (load checks it), so its always in effect
    int answer = 0;

    // go through each line, if the tick is at or before the given tick, keep advancing
    for (int i = 0; i < scenario->num_lines; i ++) {
        if (scenario->lines[i].tick <= tick) {
            answer = i;
        }

        // ticks increase, so every later line is also too late
        else {
            break;
        }
    }
    return answer;
}


/* Read a text file and fill in *scenario with the start time and the
data lines. Return 0 if the file is good, −1 if it can't be opened, or the
number of the first bad line */

int scenario_load(const char *file, scenario_t *scenario) {

    // open the file to read, check if its NULL (can't open)
    FILE *reading = fopen(file, "r");
    if (reading == NULL) {
        return -1;
    }

    // create line buffer, initialize variables
    char buff[256];          // holds one line of text
    int line_num = 0;        // which file line we're on
    int have_start = 0;      // have we seen the "start" line yet?
    int result = 0;          // the first bad line
    scenario->num_lines = 0; // no data lines stored yet

    // read the file, one line at a time
    while (fgets(buff, sizeof buff, reading) != NULL) {
        line_num ++;
        char first;
        long long value;

        // blank: skip
        if (sscanf(buff, " %c", &first) != 1)
            continue;

        // comment: skip
        if (first == '#')
            continue;

        // start line: log its value and indicate that we've seen it
        if (sscanf(buff, " start %lld", &value) == 1) {
            scenario->start = value;
            have_start = 1;
        }

        // otherwise its a data line
        else {
            unsigned int sw;
            float temp, rh;
            int tick;
            int n = sscanf(buff, "%d %f %f %x", &tick, &temp, &rh, &sw);

            // not enough fields: bad line, stop
            if (n != 4) {
                result = line_num;
                break;
            }

            // haven't started yet: bad line, stop
            if (!have_start) {
                result = line_num;
                break;
            }

            // first data line must be tick 0: bad line, stop
            if (scenario->num_lines == 0 && tick != 0) {
                result = line_num;
                break;
            }

            // tick must be greater than the previous line's tick: bad line, stop
            if (scenario->num_lines > 0 && tick <= scenario->lines[scenario->num_lines - 1].tick) {
                result = line_num;
                break;
            }

            // num_lines is at or over the max: bad line, stop
            if (scenario->num_lines >= SCENARIO_MAX_LINES) {
                result = line_num;
                break;
            }

            // passed each check: fill the next free slot, then count it
            scenario_line_t *slot = &scenario->lines[scenario->num_lines];

            slot->tick = tick;
            slot->temp_c = temp;
            slot->rh_pct = rh;
            slot->switches = sw;
            scenario->num_lines ++;
        }
    }

    // no error, but no data lines: bad, point to the line after the last
    if (result == 0 && scenario->num_lines == 0) {
        result = line_num + 1;
    }
    fclose(reading);
    return result;

}
