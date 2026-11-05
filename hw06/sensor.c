/* sensor.c: see sensor.h for each function's contract */

#include <stddef.h> /* for NULL */
#include <math.h>   /* for round */
#include "sensor.h"
#include "iom361_r4.h"

#define RAW_MASK       0x000FFFFFu  /* bits 19..0 hold the 20-bit sensor value */
#define RAW_FULL_SCALE 1048576.0    /* 2^20 */

// the registers come back within 0.0001 of what was set (91.0 reads as
// 90.99998), so round to 0.1 to get back the value that was set
static float round_tenth(double x) {
    return (float) (round(x * 10.0) / 10.0);
}

int sensor_read(uint32_t *base, time_t now, reading_t *r) {
    if (r == NULL) {
        return 1;
    }

    int rc;

    // read both registers before storing anything, so a failure changes nothing
    uint32_t raw_temp = iom361_readReg(base, TEMP_REG, &rc);
    if (rc != 0) {
        return rc;
    }

    uint32_t raw_rh = iom361_readReg(base, HUMID_REG, &rc);
    if (rc != 0) {
        return rc;
    }

    raw_temp &= RAW_MASK;
    raw_rh   &= RAW_MASK;

    // AHT20 formulas from the register table in iom361_r4.h
    r->t      = now;
    r->temp_c = round_tenth(raw_temp / RAW_FULL_SCALE * 200.0 - 50.0);
    r->rh_pct = round_tenth(raw_rh / RAW_FULL_SCALE * 100.0);

    return 0;
}
