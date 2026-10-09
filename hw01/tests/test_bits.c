#include <stdio.h>
#include "bits.h"
#include "status.h"

static int failures = 0;
static int total = 0;

static void check_u32(const char *name, uint32_t got, uint32_t want)
{
    total++;
    if (got == want) {
        printf("PASS %s\n", name);
    } else {
        printf("FAIL %s: got 0x%X, want 0x%X\n", name, got, want);
        failures++;
    }
}

static void check_i32(const char *name, int32_t got, int32_t want)
{
    total++;
    if (got == want) {
        printf("PASS %s\n", name);
    } else {
        printf("FAIL %s: got %d, want %d\n", name, got, want);
        failures++;
    }
}

int main(void)
{
    /* get_field */
    check_u32("get_field 0x1631 pos8 w8", get_field(0x1631, 8, 8), 22);
    check_u32("get_field width 32", get_field(0xFFFFFFFFu, 0, 32), 0xFFFFFFFFu);
    check_u32("get_field width 1 pos 31", get_field(0x80000000u, 31, 1), 1);
    check_u32("get_field invalid width 0", get_field(0xFF, 0, 0), 0);
    check_u32("get_field pos+width > 32", get_field(0xFF, 30, 4), 0);

    check_u32("set_field width 1", set_field(0x0, 3, 1, 1), 0x08);
    check_u32("set_field width 32", set_field(0x12345678u, 0, 32, 0xABCDu), 0xABCDu);
    check_u32("set_field pos 31", set_field(0x0, 31, 1, 1), 0x80000000u);
    check_u32("set_field too wide", set_field(0x0, 4, 3, 0xF), 0x70);
    check_u32("set_field pos+width > 32", set_field(0xFF, 30, 4, 1), 0xFF);
    check_u32("set_field width 0", set_field(0xFF, 0, 0, 1), 0xFF);
    check_u32("set_field keeps neighbors", set_field(0xFFFF, 4, 4, 0), 0xFF0F);
    /* ---- get_field: extra boundaries ---- */
    check_u32("get_field width 33 invalid", get_field(0xFF, 0, 33), 0);
    check_u32("get_field pos -1 invalid", get_field(0xFF, -1, 4), 0);
    check_u32("get_field mode field", get_field(0x1631, 4, 3), 3);

    /* ---- sign_extend ---- */
    check_i32("sign_extend 0xF8 w8", sign_extend(0xF8, 8), -8);
    check_i32("sign_extend 0x7F w8 (largest positive)", sign_extend(0x7F, 8), 127);
    check_i32("sign_extend 0x80 w8 (most negative)", sign_extend(0x80, 8), -128);
    check_i32("sign_extend w1 value 1", sign_extend(1, 1), -1);
    check_i32("sign_extend w1 value 0", sign_extend(0, 1), 0);
    check_i32("sign_extend w32 all ones", sign_extend(0xFFFFFFFFu, 32), -1);
    check_i32("sign_extend w32 most negative", sign_extend(0x80000000u, 32), INT32_MIN);
    check_i32("sign_extend ignores high bits", sign_extend(0x1F8, 8), -8);
    check_i32("sign_extend width 0 invalid", sign_extend(0xFF, 0), 0);
    check_i32("sign_extend width 33 invalid", sign_extend(0xFF, 33), 0);

    /* ---- status_unpack ---- */
    status_t s;

    s = status_unpack(0x1631);   /* handout example */
    check_i32("0x1631 setpoint", s.setpoint, 22);
    check_i32("0x1631 mode", s.mode, 3);
    check_i32("0x1631 heat", s.heat, 1);
    check_i32("0x1631 cool", s.cool, 0);
    check_i32("0x1631 fan", s.fan, 0);
    check_i32("0x1631 fault", s.fault, 0);
    check_i32("0x1631 reserved", s.reserved, 0);
    check_i32("0x1631 mode_valid", s.mode_valid, 1);

    s = status_unpack(0xEC00);   /* negative setpoint */
    check_i32("0xEC00 setpoint", s.setpoint, -20);
    check_i32("0xEC00 mode", s.mode, 0);
    check_i32("0xEC00 heat", s.heat, 0);

    s = status_unpack(0x000C);   /* fan + fault */
    check_i32("0x000C fan", s.fan, 1);
    check_i32("0x000C fault", s.fault, 1);
    check_i32("0x000C heat", s.heat, 0);
    check_i32("0x000C setpoint", s.setpoint, 0);

    s = status_unpack(0x0050);   /* invalid mode 5 */
    check_i32("0x0050 mode", s.mode, 5);
    check_i32("0x0050 mode_valid", s.mode_valid, 0);

    s = status_unpack(0x0070);   /* invalid mode 7 */
    check_i32("0x0070 mode", s.mode, 7);
    check_i32("0x0070 mode_valid", s.mode_valid, 0);

    s = status_unpack(0x0080);   /* reserved bit set */
    check_i32("0x0080 reserved", s.reserved, 1);
    check_i32("0x0080 mode", s.mode, 0);

    s = status_unpack(0x7F00);   /* largest setpoint */
    check_i32("0x7F00 setpoint", s.setpoint, 127);

    s = status_unpack(0x8000);   /* most negative setpoint */
    check_i32("0x8000 setpoint", s.setpoint, -128);

    printf("\n%d of %d tests passed\n", total - failures, total);
    return failures ? 1 : 0;
}
