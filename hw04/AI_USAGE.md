# AI usage

**Tool:** Claude Opus 5.5 (Claude Code)

**What I used it for:**
- Getting feedback on my tally.c, mostly about keeping the entries
  sorted, shifting the later entries up by one when a new name goes in
  the middle, and checking a name's length before I copy it
- Explaining the file structure and how all the pieces fit
  together
- Starter code for events.c, tests/test_tally.c, the Makefile,
  data/pump.txt, and README.md. I read through all of it, ran make test,
  and all 41 tests pass.
- Making sure make builds with -Wall -Wextra and no warnings, and getting
  the compiler error for Part 3.
- A final review against the assignment before I pushed. It caught a
  couple of small things: a typo in a tally.c comment, and that a line
  like "CARD " (trailing space) would count as a different name than
  "CARD", so events.c now trims trailing spaces and tabs.

**What I wrote:** tally.h and tally.c and their comments, with feedback
from Claude along the way. I identified a few repeated code blocks and
built helper functions (name_len(), find_pos(), and is_at()).
The door.txt part was confusing to me, so I decided to use the events
from my project (pump.txt).

**One thing it got wrong:**
When it wrote the Part 3 paragraph in README.md, it said my tally uses
binary search to find a name. It doesn't. My find_pos is just a linear
search: it walks through the sorted entries and stops at the first one
that isn't smaller. I caught it and had it fixed in the README.
