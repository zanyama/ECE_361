#include "panel.h"
#include "sensor.h"
#include "iom361_r4.h"
#include "scenario.h"

#include <stdio.h>
#include <stdint.h>

#define NUM_SWITCHES 4

static int total = 0;
static int failures = 0;


/* Integer check: passes if got equals want; counts the check either way */
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

/* Float check: passes if got equals want; counts the check either way */
static void check_float(const char *name, float got, float want) {
    total++;
    if (got == want) {
        printf("PASS %s\n", name);
    }
    else {
        printf("FAIL %s: got %.5f, want %.5f\n", name, got, want);
        failures++;
    }
}

int main(void) {
    // panel_pressed(now, before): switches that went off -> on
    check_int("pressed: a press",   panel_pressed(0x1, 0x0), 0x1);
    check_int("pressed: a release", panel_pressed(0x0, 0x1), 0x0);
    check_int("pressed: held",      panel_pressed(0x1, 0x1), 0x0);

    // panel_released(now, before): switches that went on -> off
    check_int("released: a press",   panel_released(0x1, 0x0), 0x0);
    check_int("released: a release", panel_released(0x0, 0x1), 0x1);
    check_int("released: held",      panel_released(0x1, 0x1), 0x0);

    // fake LED register: A = 1010 = LEDs 1 and 3 on
    uint32_t reg = 0xA;

    // turn LED 0 on. should get 1011 (0xB)
    panel_led(&reg, 0, 1);
    check_int("led: turn 0 on keeps 1 and 3", reg, 0xB);

    // turn LED 3 off. should get 0010 (0x2)
    reg = 0xA;
    panel_led(&reg, 3, 0);
    check_int("led: turn 3 off keeps 1", reg, 0x2);

    // toggle LED 1. should get 1000 (0x8)
    reg = 0xA;
    panel_toggle(&reg, 1);
    check_int("led: toggle 1", reg, 0x8);

    // an invalid led number: return is -1 and reg is unchanged
    reg = 0xA;
    check_int("led: invalid LED number", panel_led(&reg, 32, 0), -1);
    check_int("led: invalid LED number: register unchanged", reg, 0xA);

    // the other edge: LED -1 is invalid too (1u << -1 would be undefined)
    reg = 0xA;
    check_int("led: LED -1 is invalid", panel_led(&reg, -1, 1), -1);
    check_int("led: LED -1: register unchanged", reg, 0xA);

    // sensor: set 91.0 C in the "world", read it back through the registers
    int rc;
    uint32_t *base = iom361_initialize(NUM_SWITCHES, NUM_SWITCHES, &rc);
    check_int("sensor: iom361_initialize", rc, 0);
    iom361_set_display(false);
    _iom361_set_sensor1(91.0, 40.0);

    reading_t r;
    check_int("sensor: read returns 0", sensor_read(base, 0, &r), 0);

    check_float("sensor: 91.0 C reads back as 91.0", r.temp_c, 91.0f);
    check_float("sensor: 40.0 % RH reads back as 40.0", r.rh_pct, 40.0f);

    r.temp_c = -1.0f;
    r.rh_pct = -1.0f;
    check_int("sensor: sensor_read called with WRONG base",sensor_read(base + 1, 0, &r), 1);
    check_float("sensor: temp still -1", r.temp_c, -1.0f);
    check_float("sensor: rh still -1", r.rh_pct, -1.0f);

    // scenario: comments and blank lines are skipped
    static scenario_t s;
    check_int("scenario: comments file loads", scenario_load("tests/data/comments.txt", &s), 0);
    check_int("scenario: comments file has 2 lines", s.num_lines, 2);
    check_int("scenario: second line's tick",        s.lines[1].tick, 10);
    check_float("scenario: second line's temp",      s.lines[1].temp_c, 21.5f);
    check_int("scenario: second line's switches",    s.lines[1].switches, 0x2);

    // scenario: values hold between lines (line in force at tick t)
    check_int("scenario: tick 0 uses line 0", scenario_at(&s, 0), 0);
    check_int("scenario: tick 9 uses line 0", scenario_at(&s, 9), 0);
    check_int("scenario: tick 10 uses line 1", scenario_at(&s, 10), 1);

    // scenario: bad files return the number of the first bad line
    check_int("scenario: no start line -> line 2",   scenario_load("tests/data/nostart.txt", &s), 2);
    check_int("scenario: missing field -> line 3",   scenario_load("tests/data/missing.txt", &s), 3);
    check_int("scenario: tick not increasing -> line 4", scenario_load("tests/data/notinc.txt", &s), 4);

    check_int("scenario: first tick not 0 -> line 3", scenario_load("tests/data/first_tick.txt", &s), 3);
    check_int("scenario: no data lines -> line 3", scenario_load("tests/data/nodata.txt", &s), 3);

    // scenario: a file that cannot be opened returns -1
    check_int("scenario: missing file -> -1", scenario_load("tests/data/no_such_file.txt", &s), -1);

    // scenario: the limit. exactly SCENARIO_MAX_LINES data lines load;
    // one more is bad (file line 1002: the start line plus 1001 data lines)
    check_int("scenario: 1000 data lines load", scenario_load("tests/data/max.txt", &s), 0);
    check_int("scenario: 1000 data lines: num_lines", s.num_lines, SCENARIO_MAX_LINES);
    check_int("scenario: 1001 data lines -> line 1002", scenario_load("tests/data/toomany.txt", &s), 1002);

    printf("%d of %d checks passed\n", total - failures, total);

    return failures != 0; // nonzero exit if anything failed

}
