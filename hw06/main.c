#include <stdio.h> // printf & fprintf
#include <time.h>
#include "iom361_r4.h"
#include "scenario.h"
#include "sensor.h"
#include "panel.h"

#define NUM_SWITCHES 4

// helper function to determine if an iom361 or sensor call failed or not
static int failed(int rc, const char *what) {

    // prints what failed and returns 1 if rc is nonzero
    if (rc != 0) {
        fprintf(stderr, "%s failed: %d\n", what, rc);
        return 1;
    }

    // otherwise returns 0
    return 0;
}

int main(int argc, char *argv[]) {

    // needs exactly one argument
    if (argc != 2) {
        fprintf(stderr, "usage: ./replay scenario.txt\n");
        return 1;
    }

    // initialize scenario and load it (static so the 16KB struct isn't on the stack)
    static scenario_t scen;
    int rc = scenario_load(argv[1], &scen);

    if (rc == -1) {
        fprintf(stderr, "%s: cannot open\n", argv[1]);
        return 1;
    }

    if (rc != 0) {
        fprintf(stderr, "%s: bad line %d\n", argv[1], rc);
        return 1;
    }

    // initialize iom361 registers. number of LEDs matches NUM_SWITCHES
    int io_rc;
    uint32_t *base = iom361_initialize(NUM_SWITCHES, NUM_SWITCHES, &io_rc);

    // init fails if base is NULL or io_rc failed
    if (base == NULL || io_rc != 0) {
        fprintf(stderr, "iom361 initialization failed: %d\n", io_rc);
        return 1;
    }

    // turns off the model's LED printing, so nothing but trace lines go to stdout
    iom361_set_display(false);

    // get last tick
    int last = scen.lines[scen.num_lines - 1].tick;
    // last second's switches- they start at 0 and survives between passes
    uint32_t before = 0;

    // get the LED register- LEDS_REG is a byte offset, so divide by size of uint32_t
    uint32_t *leds = base + LEDS_REG / sizeof(uint32_t);

    // for every tick, 0-last:
    for (int t = 0; t <= last; t ++) {

        // get index of current tick
        int index = scenario_at(&scen, t);

        // get the line at that index
        const scenario_line_t *line = &scen.lines[index];

        // set temp, rh, and switches from line
        _iom361_set_sensor1(line->temp_c, line->rh_pct);
        _iom361_set_switches(line->switches);

        // create sensor reading, check if it failed
        reading_t r;
        int sensor = sensor_read(base, scen.start + t, &r);
        if (failed(sensor, "sensor read")) return 1;

        // read switches, check if it failed
        uint32_t switches = iom361_readReg(base, SWITCHES_REG, &io_rc);
        if (failed(io_rc, "switch read")) return 1;

        // get switches that have been pressed, and update the before state
        uint32_t pressed = panel_pressed(switches, before);
        before = switches;

        // for each switch, toggle the corresponding LED
        for (int k = 0; k < NUM_SWITCHES; k ++) {
            if ((pressed >> k) & 1) {
                panel_toggle(leds, k);
            }
        }

        // read LED register, check if it failed
        uint32_t led_reg = iom361_readReg(base, LEDS_REG, &io_rc);
        if (failed(io_rc, "led_reg read")) return 1;

        // build time string. gmtime=UTC, so the trace is the same on every computer
        struct tm *utc = gmtime(&r.t);
        char when[20];
        strftime(when, sizeof when, "%Y-%m-%d %H:%M:%S", utc);

        // print all the info
        printf("%s temp=%.1f rh=%.1f switches=0x%x pressed=0x%x leds=0x%x\n", when, r.temp_c, r.rh_pct, switches, pressed, led_reg);
    }
    return 0;
}
