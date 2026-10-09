#ifndef STATUS_H
#define STATUS_H

#include <stdint.h>

#define HEAT_POS       0
#define HEAT_WIDTH     1
#define COOL_POS       1
#define COOL_WIDTH     1
#define FAN_POS        2
#define FAN_WIDTH      1
#define FAULT_POS      3
#define FAULT_WIDTH    1
#define MODE_POS       4
#define MODE_WIDTH     3
#define RESERVED_POS   7
#define RESERVED_WIDTH 1
#define SETPOINT_POS   8
#define SETPOINT_WIDTH 8

#define MODE_MAX_VALID 4

/* 0=OFF 1=HEAT 2=COOL 3=AUTO 4=FAN_ONLY */

typedef struct {
    int heat;
    int cool;
    int fan;
    int fault;
    int mode;
    int reserved;
    int setpoint;
    int mode_valid;
} status_t;

status_t status_unpack(uint16_t word);

#endif
