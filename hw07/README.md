# HW7: a growing stack and a bracket checker

## What it does
- istack.h / istack.c: istack_t, a stack of int that grows as it fills.
  It starts with room for ISTACK_START (8) values on the heap and doubles
  with realloc when full; the room never shrinks. The struct is opaque:
  it is complete only in istack.c, so other files can hold an istack_t *
  but not reach inside it. push reallocs into a second pointer, so a
  failed realloc leaves the stack unchanged.
- balance.c: `./balance < file` reads standard input one character at a
  time, pushes each (, [, {, and pops for each ), ], }. Every bracket
  counts, also in comments and strings. It stops at the first error and
  prints one line (lines and columns count from 1):

      balanced: 6 pairs, deepest 3                    exit status 0
      line 1, column 8: found ')', expected ']'       exit status 1
      line 1, column 12: found ')', nothing open      exit status 1
      end of input: 2 still open, innermost '('       exit status 1

  deepest is the most brackets open at once. The stack is destroyed on
  every path.

## Build and test
    make                     # builds balance, no warnings
    ./balance < inputs/balanced.txt
    make test                # unit tests, then the regression test
    make memcheck            # rebuilds with -fsanitize=address,undefined and runs make test
    make clean               # removes everything the build made

make memcheck finds leaks on Linux only. On macOS, find them with

    leaks --atExit -- ./tests/test_istack

## Tests
tests/test_istack.c prints PASS or FAIL per check and a summary, and
exits nonzero if any check fails, so make test stops there. It checks:

- pop and peek on an empty stack return -1 and leave *v alone
- last in, first out
- full at 8; the 9th push doubles the room to 16 and keeps every value
- 1000 pushes and pops
- istack_to_array is bottom first, and a copy (changing it does not
  change the stack)
- NULL arguments
- two stacks are independent
- destroying a stack that is not empty (make memcheck shows no leak)

regress.sh runs ./balance on every file in inputs/ and compares its
output with expected/NAME.out and its exit status with
expected/NAME.status:

| input           | what it tests                                   | expected |
|-----------------|-------------------------------------------------|----------|
| balanced.txt    | C code, brackets in a comment and a string      | balanced: 8 pairs, deepest 3 |
| deep.txt        | 12 open at once, deeper than ISTACK_START       | balanced: 12 pairs, deepest 12 |
| mismatch.txt    | a closer that does not match                    | line 1, column 13: found ']', expected ')' |
| extra_close.txt | a closer with nothing open                      | line 4, column 1: found '}', nothing open |
| unclosed.txt    | brackets still open at the end                  | end of input: 2 still open, innermost '(' |
| empty.txt       | an empty file                                   | balanced: 0 pairs, deepest 0 |

## Part 4: breaking it on purpose
Each bug was made in turn, run under make memcheck, and then undone; the
code as pushed passes make memcheck cleanly.

### (1) Destroying the same stack twice
    ==151854==ERROR: AddressSanitizer: heap-use-after-free on address 0x603000000010 at pc 0x55d1ea237a46 bp 0x7ffd4ae6b990 sp 0x7ffd4ae6b980

The first istack_destroy freed the struct, so the second one reads s->data from that freed struct before it could free anything again.

### (2) Pushing onto a stack after destroying it
    ==152099==ERROR: AddressSanitizer: heap-use-after-free on address 0x603000000018 at pc 0x561d60e37afa bp 0x7ffc765753d0 sp 0x7ffc765753c0

After istack_destroy(s) freed the struct, s still held its old address, a dangling pointer. So istack_push reads s->count from freed memory. Its address is 8 higher than the previous error message because count comes after the 8-byte data pointer in the struct.


### (3) Leaving out istack_destroy in balance.c
    ==151615==ERROR: LeakSanitizer: detected memory leaks

56 bytes leaked in 2 allocations. Direct leak of 24 bytes (the struct) and Indirect leak of 32 bytes (data array). Without destroying, the struct and data array are never freed.
