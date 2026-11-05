 #include "panel.h"

//the switches that went from off to on, one bit per switch
uint32_t panel_pressed(uint32_t now, uint32_t before) {
    // before:0 -> ~before:1
    //    now:1
	return now & ~before;
}

//the switches that went from on to off
uint32_t panel_released(uint32_t now, uint32_t before) {
    //            before: 1
    //    now: 0 -> ~now: 1
	return ~now & before;
}

/* helper function to check if n is within the valid range (0-31) */
static int validRange(int n) {
    return n >= 0 && n <= 31;
}


/* turns LED n on or off by read-modify-write, so that
every other LED keeps its state; −1 if n is not 0 to 31 */
int panel_led(uint32_t *base, int n, int on) {

    // check if n is in valid range
    if (!validRange(n)) {
        return -1;
    }

    // read LED register
    uint32_t reg = *base;

    // create mask and a variable to store the result
    uint32_t mask = 1u << n;
    uint32_t led;

    // if led is off, use AND w/inverted mask (set to 0)
    if (on == 0) {
        led = reg & ~mask;
    } 
    // any other number, use OR (set to 1)
    else {
        led = reg | mask;
    }

    // update the LED registry
    *base = led;

    return 0;
}

//toggles LED n, the same way
int panel_toggle(uint32_t *base, int n) {
    
    // check if n is in valid range
    if (!validRange(n)) {
        return -1;
    }

    // read LED register
    uint32_t reg = *base;

    // create mask and a variable to store the result
    uint32_t mask = 1u << n;
    uint32_t led = reg ^ mask;

    *base = led;

    return 0;
}