#ifndef SENSOR_H
#define SENSOR_H

#include <stdint.h>
#include <time.h>

/*
 * sensor: reads the iom361 temperature and humidity registers and
 * converts them to degrees C and %RH, rounded to 0.1.
 * No printf in this module.
 */

/* one sensor reading: when it was taken, in C, and in %RH */
typedef struct {
    time_t timestamp;
    float  temp_c;
    float  rh_pct;
} reading_t;

/*
sensor_read: reads TEMP_REG and HUMID_REG and converts them to degrees C
             and %RH, each rounded to 0.1

    Takes:   the iom361 base address, and the addresses where the
             temperature (C) and humidity (%RH) are stored
    Returns: 0 on success, or the iom361 return code if a register read
             fails (1 if temp_c or rh_pct is NULL); on failure *temp_c and
             *rh_pct are left unchanged
    Assumes: iom361_initialize() has been called and base is what it returned
*/

int sensor_read(uint32_t *base, float *temp_c, float *rh_pct);

#endif
