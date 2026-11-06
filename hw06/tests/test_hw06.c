#include "panel.h"
#include "sensor.h"
#include "iom361_r4.h"
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

    printf("%d of %d checks passed\n", total - failures, total);

    return failures != 0; // nonzero exit if anything failed

}
