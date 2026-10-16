# AI usage

**Tool:** Claude Opus 5.5 (Claude Code)

**What I used it for:**
- Reviewing my minmax: it suggested keeping local copies and writing
  *min and *max once at the end, and starting the loop at a + 1 to avoid
  running one extra time unnecessarily
- Finding a compile error (a missing semicolon) and checking that ptr.c
  builds with -Wall -Wextra and no warnings
- Starter code for find_above, reverse, tests/test_ptr.c, the Makefile,
  and README.md. I read them, ran make test, and all tests pass.

**What I wrote:** ptr.h and its comments, swap_float, and minmax, with
feedback from Claude.

**One thing it got wrong:**
I tried to use nano to create a test file, and it suggested " ! nano
tests/test_ptr.c" to open the editor. It failed with "Too many errors
from stdin", since the ! prefix can't run interactive programs.
