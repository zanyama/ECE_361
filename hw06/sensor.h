#ifndef SENSOR_H
#define SENSOR_H

#include <stdint.h>
#include <time.h>

/*
 * sensor: reads the iom361 temperature and humidity registers and
 * converts them to degrees C and %RH, rounded to 0.1.
 * No printf in this module, and no calls to the _iom361_set_... test
 * functions: on a real board they would not exist.
 */

/* one sensor reading: when it was taken, in C, and in %RH */
typedef struct {
    time_t t;
    float  temp_c;
    float  rh_pct;
} reading_t;

/*
sensor_read: reads TEMP_REG and HUMID_REG, keeps bits 19..0 of each,
             converts them to degrees C and %RH, each rounded to 0.1,
             and stores them with the timestamp now in *r

    Takes:   the iom361 base address, the time of the reading, and the
             address where the reading is stored
    Returns: 0 on success, or the iom361 return code of the read that
             failed (1 if r is NULL); on failure *r is left unchanged
    Assumes: iom361_initialize() has been called and base is what it returned
*/
int sensor_read(uint32_t *base, time_t now, reading_t *r);

#endif
