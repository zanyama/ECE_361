#include <stdio.h>
#include "iom361_r4.h"

#define DEVICE_NAME  "Fuel pump"
#define NUM_SWITCHES 3
#define NUM_LEDS     2

int main(void)
{
    int rc;
    uint32_t *base = iom361_initialize(NUM_SWITCHES, NUM_LEDS, &rc);
    if (base == NULL) {
        fprintf(stderr, "iom361_initialize failed: %d\n", rc);
        return 1;
    }

    printf("%s\n", DEVICE_NAME);
    return 0;
}
