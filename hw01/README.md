# HW1: Bit-manipulation library and thermostat status decoder

## What it does
A small C library for reading and writing bit fields in 32-bit words
(bits.c), plus "status_unpack", which decodes a 16-bit thermostat
status word into a struct (status.c)

## Build and test
    make         # compiles bits.o and status.o (gcc -std=c11 -Wall -Wextra)
    make test    # builds and runs tests/test_bits
    make clean   # removes all build output

## Valid inputs
- width: 1-32
- pos: 0-31
- pos + width <= 32
- Bits are numbered from 0 (least significant).

## Out-of-range behavior
- get_field returns 0
- set_field returns word unchanged
- sign_extend with width outside 1-32 returns 0
- print_binary with width outside 1-32 prints nothing

Reasoning: the functions never crash or shift by an undefined amount,
and set_field never corrupts a word on a bad call.

## Boundaries
- Width 32: a mask of (1u << 32) - 1 is undefined in C, so make_mask()
  special-cases width 32 as 0xFFFFFFFF
- set_field masks value first, so a value too wide for its field
  cannot spill into neighboring bits
- sign_extend at width 32 returns the value as is
- print_binary prints no trailing newline; groups of four are counted
  from the right, so width 6 prints "00 1111".

## status_unpack
Fields are named constants in status.h (no magic numbers). The
setpoint is get_field then sign_extend, giving -128 to 127.
The raw mode is always stored. mode_valid is 1 for modes 0 to 4 and
0 for modes 5 to 7, so an invalid mode is reported, not hidden.
The reserved bit is reported in `reserved`, not rejected.

## Testing
tests/test_bits.c has 48 tests covering the boundaries above.

