# AI usage

**Tool:** Claude Opus 5.5 (Claude Code)

**How I used it:** One little step at a time. For each part (sensor,
panel, scenario, main, tests, Makefile, regress.sh) Claude told me the
next step and explained anything I didn't understand. It gave hints,
and I wrote the code. After each part, Claude reviewed it and pointed
out bugs, which I fixed. Before running
things I'd predict the output first, then run it and compare.

**What Claude wrote:**
- README.md / AI_USAGE.md (Claude drafted, I edited it)
- The last batch of tests in tests/test_hw06.c: panel_led with a bad
  LED number returns -1, a missing scenario file returns -1, a
  1000-line file loads, and a 1001-line file fails at line 1002
- The test data files for those tests: tests/data/first_tick.txt,
  nodata.txt, max.txt, toomany.txt

**What I wrote:** All the code in sensor.c, panel.c, scenario.c, and
main.c, plus the headers, all the comments, the Makefile, regress.sh,
the scenarios in scenarios/ and their expected traces in expected/,
and every other test in tests/test_hw06.c.

**One thing it got wrong:**
I asked to add a diagram to help the README, and Claude added a wide
ASCII flow diagram of what happens each tick. It was confusing: too
much on each line, and the arrows wrapped and crossed. Claude redid it
as one column, but it still confused more than it clarified, so I had
it replaced with a numbered list of the 6 steps main.c does each tick.
The register map became a table instead.
