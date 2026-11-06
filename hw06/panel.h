#ifndef PANEL_H
#define PANEL_H

#include <stdint.h>

/* panel: interface with LEDs and switches on the panel */

/*
panel_pressed: given previous and current states, returns which switches have
               gone from off to on

    Takes:   now: the switches register just read
             before: the previous switches register read
    Returns: one bit per switch. 1 if the switch had been off and is now on, 0 otherwise
*/
uint32_t panel_pressed(uint32_t now, uint32_t before);

/*
panel_released: given previous and current states, returns which switches have
                gone from on to off

    Takes:   now: the switches register just read
             before: the previous switches register read
    Returns: one bit per switch. 1 if the switch had been on and is now off, 0 otherwise
*/
uint32_t panel_released(uint32_t now, uint32_t before);

/*
panel_led: turns LED n on or off, leaving the others unchanged

    Takes:   base: pointer to the LED register
             n: the LED number (0-31)
             on: whether to turn the LED on (!0) or off (0)
    Returns: -1 if n is not 0-31, otherwise 0
    Assumes: base is not NULL, points to the LED register, and that the register
             can be read back
*/
int panel_led(uint32_t *base, int n, int on);

/*
panel_toggle: toggles LED n, leaving the others unchanged

    Takes:   base: pointer to the LED register
             n: the LED number (0-31)
    Returns: -1 if n is not 0-31, otherwise 0
    Assumes: base is not NULL, points to the LED register, and that the register
             can be read back
*/
int panel_toggle(uint32_t *base, int n);

#endif
