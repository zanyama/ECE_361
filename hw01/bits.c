#include "bits.h"
#include <stdio.h>
static int valid_field(int pos, int width) {
    return (width >= 1 && width <= 32 && pos >= 0 && pos <= 32 - width);
}

static uint32_t make_mask(int width) {
    return (width == 32) ? 0xFFFFFFFFu : ((1u << width) - 1);
}

uint32_t get_field(uint32_t word, int pos, int width) {

    if (!valid_field(pos, width)) {
        return 0;
    }

    return (word >> pos) & make_mask(width);
}

void print_binary(uint32_t x, int width) {
    if (width < 1 || width > 32) {
        return;
    }
    for (int i = width - 1; i >= 0; i--) {
        printf("%c", ((x >> i) & 1) ? '1' : '0');

        if (i % 4 == 0 && i > 0) {
            printf(" ");
        }

        /* if i is a multiple of 4 and i > 0, print a space */
    }
}

uint32_t set_field(uint32_t word, int pos, int width, uint32_t value) {

    if (!valid_field(pos, width)) {
        return word;
    }

    uint32_t mask = make_mask(width);

    return (word & ~(mask << pos)) | ((value & mask) << pos);

}

int32_t sign_extend(uint32_t value, int width)
{
    if (width < 1 || width > 32) {
        return 0;
    }

    uint32_t mask = make_mask(width);
    value &= mask;

    if (width < 32 && (value & (1u << (width - 1)))) {
        value |= ~mask;
    }
    return (int32_t) value;
}
