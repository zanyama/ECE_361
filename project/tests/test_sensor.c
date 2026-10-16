#include <stdio.h>
#include <stdint.h>
#include "iom361_r4.h"
#include "sensor.h"

static int total = 0;
static int failures = 0;

static void check_float(const char *name, float got, float want) {
    total++;
    if (got == want) {
        printf("PASS %s\n", name);
    }
    else {
        printf("FAIL %s: got %f, want %f\n", name, got, want);
        failures++;
    }
}

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

int main(void) {
    int rc;
    uint32_t *base = iom361_initialize(3, 2, &rc);
    if (base == NULL) {
        printf("FAIL iom361_initialize: %d\n", rc);
        return 1;
    }
    iom361_set_display(false);

    float temp = 0.0f, rh = 0.0f;

    /* after initialize the sensor reads 23.5 C and 75 %RH */
    check_int("initial: return", sensor_read(base, &temp, &rh), 0);
    check_float("initial: temp", temp, 23.5f);
    check_float("initial: rh", rh, 75.0f);

    /* 0 C */
    _iom361_set_sensor1(0.0f, 50.0f);
    check_int("0 C: return", sensor_read(base, &temp, &rh), 0);
    check_float("0 C: temp", temp, 0.0f);
    check_float("0 C: rh", rh, 50.0f);

    /* 100 C */
    _iom361_set_sensor1(100.0f, 50.0f);
    check_int("100 C: return", sensor_read(base, &temp, &rh), 0);
    check_float("100 C: temp", temp, 100.0f);

    /* the thresholds in SPEC.md: TEMP_HIGH above 45.0, TEMP_OK at or below 42.0 */
    _iom361_set_sensor1(45.0f, 50.0f);
    sensor_read(base, &temp, &rh);
    check_float("threshold 45.0 C", temp, 45.0f);

    _iom361_set_sensor1(42.0f, 50.0f);
    sensor_read(base, &temp, &rh);
    check_float("threshold 42.0 C", temp, 42.0f);

    /* the register gives back 90.99998 for 91.0; rounding brings it back */
    _iom361_set_sensor1(91.0f, 12.3f);
    sensor_read(base, &temp, &rh);
    check_float("rounding: 91.0 C", temp, 91.0f);
    check_float("rounding: 12.3 %RH", rh, 12.3f);

    /* below zero */
    _iom361_set_sensor1(-12.3f, 99.9f);
    sensor_read(base, &temp, &rh);
    check_float("negative: -12.3 C", temp, -12.3f);
    check_float("negative: 99.9 %RH", rh, 99.9f);

    /* a failed read returns the iom361 code and leaves the outputs unchanged */
    temp = 111.0f;
    rh = 222.0f;
    check_int("bad base: return", sensor_read(base + 1, &temp, &rh), 1);
    check_float("bad base: temp unchanged", temp, 111.0f);
    check_float("bad base: rh unchanged", rh, 222.0f);

    check_int("temp NULL: return", sensor_read(base, NULL, &rh), 1);
    check_float("temp NULL: rh unchanged", rh, 222.0f);

    check_int("rh NULL: return", sensor_read(base, &temp, NULL), 1);
    check_float("rh NULL: temp unchanged", temp, 111.0f);

    printf("\n%d of %d tests passed\n", total - failures, total);
    return failures ? 1 : 0;
}
