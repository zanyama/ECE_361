# Bonus: a table checker

`int fsm_check(table, start, out)` in check.c prints one line per
problem and returns how many it found:

- **undefined:** an entry left out. A_UNDEFINED is 0 in action_t, so an
  entry that a designated initializer skips is zero, which is
  A_UNDEFINED, and the checker can see it was forgotten. If A_NONE were
  0, a forgotten entry would look like "do nothing" on purpose.
- **out of range:** a next state that is not a state, so
  lock_table[next] would read past the table on the next step.
- **unreachable:** a breadth-first search from start over the table.
  It keeps a queue of states to visit and a seen[] array, takes the
  oldest state off the queue, and adds every state its entries lead to
  that hasn't been seen yet. Any state never seen is unreachable.
- **no way out:** a state where every entry goes back to itself.

Undefined and out-of-range entries are not followed in the search or
counted as a way out, since they don't go anywhere real.

Run it with `make bonus`, which builds and runs tests/test_check.c.
Each broken copy below is lock_table copied with memcpy and then
changed in one place.

## 1. An entry left out: GOT2 + RIGHT

    t[GOT2][E_RIGHT] = (transition_t) { 0 };

    undefined: GOT2 RIGHT
    unreachable: OPEN

This is what happens if the `[E_RIGHT]` line is missing from the
`[GOT2]` row. It compiles without a warning, and the entry becomes
`{ LOCKED, A_UNDEFINED }`. Typing 3 1 4 then sends the lock back to
LOCKED on the 4 with no action, so the right code never opens the door.
GOT2 + RIGHT was the only way into OPEN, so the checker also reports
OPEN as unreachable.

## 2. A state with no way out: OPEN

    t[OPEN][E_CLOSE]   = (transition_t) { OPEN, A_LOCK };
    t[OPEN][E_TIMEOUT] = (transition_t) { OPEN, A_LOCK };

    no way out: OPEN

Every entry is filled in and in range, so checking the entries alone
would miss this. But once the door opens, closing it or the timer
running out leaves the lock in OPEN. The action says LOCK, but the
state never changes, so the next 3 1 4 does nothing and the door stays
unlocked for good. For a lock that is the worst bug there is.

## 3. A next state out of range: GOT1 + RIGHT

    t[GOT1][E_RIGHT].next = N_STATES;

    out of range: GOT1 RIGHT -> 4
    unreachable: GOT2
    unreachable: OPEN

A typo like writing N_STATES (or a state from another table) instead of
GOT2. After 3 1, the state is 4, and the next lock_step reads
lock_table[4][e], which is past the end of the table: undefined
behavior, so the lock does whatever is in that memory. The search
doesn't follow the bad entry, so GOT2 and OPEN are also unreachable,
and the door could never open.

## What the checker doesn't catch
GOT2 + RIGHT going to `{ LOCKED, A_BEEP }` is reported only as
"unreachable: OPEN", and a table where 3 3 1 4 opens has no problems at
all. The checker finds tables that are broken as a machine, not ones
that are the wrong lock; that is what tests/test_lock.c is for.
