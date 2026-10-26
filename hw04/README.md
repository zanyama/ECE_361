# HW4: a tally ADT

## What it does
A tally counts how often each name occurs. Callers use it only through
tally.h; the struct is completed in tally.c and nowhere else.
- tally_create / tally_destroy: hand out and give back one of TALLY_MAX
  tallies from a static pool in tally.c (no malloc)
- tally_add: counts one more occurrence of a name and returns its new
  count, or -1 if the name is empty, too long, NULL, or new while the
  tally is full
- tally_count, tally_size: how often a name was added, and how many
  different names the tally holds
- tally_print: one line per name, "name count", in strcmp order

Entries are kept sorted by name: a new name goes where it belongs, and
the later entries move up by one.

events.c reads lines from standard input, removes trailing spaces and
tabs (so "CARD " counts as CARD), skips blank lines and lines starting
with #, adds the rest to one tally, and prints it, followed by
"names: N, lines skipped: S".

## Build and test
    make          # builds events
    make test     # builds and runs tests/test_tally
    make run      # ./events < data/pump.txt
    make clean    # removes everything the build made

data/pump.txt is a fuel pump's event log: two sales, plus one line that
is too long for a name, so "lines skipped" is 1.

## Tests
tests/test_tally.c checks counting up, a name never added, an empty
name, names of exactly TALLY_LEN and TALLY_LEN + 1 characters, NULL
arguments, a full tally (a new name is refused, an existing one still
counts), TALLY_MAX tallies and then one more (NULL), create after
destroy, two independent tallies, and the print order. It prints PASS
or FAIL per check and a summary, and exits nonzero if any check fails.

## What the opaque type prevents
I added `printf("%d\n", t->n);` to a file that includes only tally.h:

    peek.c: In function 'main':
    peek.c:6:21: error: dereferencing pointer to incomplete type 'tally_t' {aka 'struct tally'}
        6 |     printf("%d\n", t->n);
          |                     ^~

tally.h only says that struct tally exists, not what is in it, so any
other file can hold a tally_t * and pass it around but can't read or
change its members. That keeps callers from breaking the rules tally.c
depends on: the entries stay sorted, n matches the number of entries in
use, and every name fits and ends in '\0'. If events.c could write
t->n or an entry directly, find_pos could stop at the wrong place, or
the insert shift could read garbage or write past the array. It also means the inside can
change (a tree instead of a sorted array, for example) without any
caller changing, as long as tally.h stays the same. The line was
removed afterward, so the code compiles.
