# HW5: a combination lock as a table

## What it does
The lock opens on the code 3-1-4. Each right digit moves on, and the
third one opens the door (UNLOCK). A wrong digit beeps (BEEP) and starts
over from the beginning. If the 5-second timer runs out while a code is
being typed, the lock starts over silently. While open, digits do
nothing; closing the door or the timer running out locks it again
(LOCK). Closing the door while locked or while typing changes nothing.

- lock.h: the state, event, and action enums, transition_t, and the
  function contracts
- lock.c: lock_table, all 16 entries with designated initializers;
  lock_step, which is one table lookup with no if or switch on the
  state; lock_event, which turns a character into an event; and the
  state, event, and action names ("?" if out of range)
- main.c: starts in LOCKED, reads standard input one character at a
  time, and prints one line per event, e.g.
  `GOT2 4 RIGHT -> OPEN UNLOCK`, then `final: STATE`. Characters that
  are not events (spaces, newlines, other letters) are skipped.

lock_event uses `code[] = "314"`: in state s the right digit is
code[s]. In OPEN that is the '\0', which no digit matches, so every
digit is WRONG while open, and the table makes that do nothing.

## Build and test
    make          # builds lock
    make test     # builds and runs tests/test_lock
    make bonus    # builds and runs tests/test_check (see BONUS.md)
    make run      # ./lock < data/codes.txt
    make clean    # removes everything the build made

Input characters: a digit is typed on the keypad, `c` is the door
closing, `t` is the timer running out.

data/codes.txt has one case per line, and each line ends LOCKED so the
next starts clean: the code then close, 3 3 1 4, 3 1 t 4, the code then
a timeout, a digit while open, a close while typing, c and t while
locked, and a wrong digit in the middle followed by the right code.

## Tests
tests/test_lock.c checks every one of the 16 table entries (next state
and action, written out in the test, not read from lock_table), that no
entry is A_UNDEFINED, lock_step with a NULL action, lock_event for a
right and a wrong digit in each state and for digits while open, c and
t in every state, other characters (-1), the sequences 3 1 4 (opens),
3 3 1 4 (does not), 3 1 t 4 (does not), and 3 1 4 c (locks again) plus
a few more, and the names, including "?" out of range. It prints PASS
or FAIL per check and a summary, and exits nonzero if any check fails.

## State diagram

                RIGHT (3)          RIGHT (1)          RIGHT (4) / UNLOCK
    +--------+           +------+           +------+              +------+
    | LOCKED | --------> | GOT1 | --------> | GOT2 | -----------> | OPEN |
    +--------+           +------+           +------+              +------+
      ^  ^  ^                |                  |                     |
      |  |  |                |                  |                     |
      |  |  +----------------+                  |                     |
      |  |   WRONG / BEEP, TIMEOUT              |                     |
      |  |                                      |                     |
      |  +--------------------------------------+                     |
      |      WRONG / BEEP, TIMEOUT                                    |
      |                                                               |
      +---------------------------------------------------------------+
             CLOSE / LOCK, TIMEOUT / LOCK

    Stays in the same state:
      LOCKED: WRONG / BEEP, CLOSE, TIMEOUT
      GOT1:   CLOSE
      GOT2:   CLOSE
      OPEN:   RIGHT, WRONG (digits do nothing)

An arrow without an action does A_NONE.

## Transition table
Each cell is the next state and the action.

| state  | E_RIGHT       | E_WRONG      | E_CLOSE      | E_TIMEOUT    |
|--------|---------------|--------------|--------------|--------------|
| LOCKED | GOT1, NONE    | LOCKED, BEEP | LOCKED, NONE | LOCKED, NONE |
| GOT1   | GOT2, NONE    | LOCKED, BEEP | GOT1, NONE   | LOCKED, NONE |
| GOT2   | OPEN, UNLOCK  | LOCKED, BEEP | GOT2, NONE   | LOCKED, NONE |
| OPEN   | OPEN, NONE    | OPEN, NONE   | LOCKED, LOCK | LOCKED, LOCK |

LOCKED + TIMEOUT can't really happen, since no timer runs while
locked, but every entry is filled in so none is left A_UNDEFINED.

## Why 3 3 1 4 does not open
The first 3 goes LOCKED -> GOT1. In GOT1 the right digit is 1, so the
second 3 is E_WRONG, and lock_table[GOT1][E_WRONG] is
{ LOCKED, A_BEEP }: it beeps and starts over. Then 1 and 4 are both
wrong in LOCKED. To make it open, change that entry to
{ GOT1, A_BEEP }: the second 3 then stays in GOT1, and 1 4 opens. The
catch is that the table only sees E_WRONG, not which digit, so every
wrong digit in GOT1 would stay in GOT1, and 3 5 1 4 would open too.
Doing it only for a 3 would take a new event (a digit that starts the
code over) from lock_event.
