#include "status.h"
#include "bits.h"

status_t status_unpack(uint16_t word)
{
    status_t s;
    s.heat = get_field(word, HEAT_POS, HEAT_WIDTH);
    s.cool = get_field(word, COOL_POS, COOL_WIDTH);
    s.fan = get_field(word, FAN_POS, FAN_WIDTH);
    s.fault = get_field(word, FAULT_POS, FAULT_WIDTH);
    s.mode = get_field(word, MODE_POS, MODE_WIDTH);
    s.reserved = get_field(word, RESERVED_POS, RESERVED_WIDTH);
    s.setpoint = get_field(word, SETPOINT_POS, SETPOINT_WIDTH);
    /* cool, fan, fault, mode, reserved: same pattern */
    s.setpoint = sign_extend(get_field(word, SETPOINT_POS, SETPOINT_WIDTH), SETPOINT_WIDTH);
    s.mode_valid = (s.mode <= MODE_MAX_VALID);;
    return s;
}
