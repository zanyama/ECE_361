# AI usage

**Tool:** Claude Opus 5.5 (Claude Code)

**How I used it:** One step at a time, like HW6. For each part
(istack, the tests, balance.c, regress.sh, Part 4) Claude guided me
through the steps and explained anything I didn't understand: getchar
and EOF, line/column, the stack, '(' vs "(", size_t vs int, exit statuses,
and what each sanitizer report meant. It gave hints, and I wrote the code.
After each step Claude built it, ran the tests, and pointed out bugs,
which I fixed.

**What Claude wrote:**
- The comments at the top of istack.h, istack.c, tests/test_istack.c
  and balance.c
- opener_for in balance.c (I wrote closer_for from it)
- Drafts of the comments in balance.c and of the Part 4 sentences in
  README.md, which I edited to make the most sense to me
- Moving each istack_destroy in tests/test_istack.c to right after
  that stack's last use
- The makefile and README, which I made edits to

**What I wrote:** All the code in istack.c, istack.h and balance.c
except what is listed above, the tests in tests/test_istack.c,
regress.sh and its pass count. I made each bug, ran make memcheck, and copied the report.

**One thing it got wrong:**
Nothing that I noticed.
