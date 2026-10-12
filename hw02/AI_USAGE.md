# AI usage

**Tool:** Claude Sonnet 5.5 (chat)

**What I used it for:**
- Figuring out how to split readings.c and what goes in each header
- Understanding recursion and what `a + 1` means on an array
- Starter code for stats_min(), stats_mean(), histogram.c, reading.c,
  main.c, the Makefile, and tests/test_stats.c. I typed these in, ran
  them, and checked that the regression test passes.
- Checking my code and my git commits

**What I wrote:** the header comments, stats_max, and the loop in
stats_longest_run_above() (adapted from readings.c), with hints from Claude.

**One thing I had to fix:** stats_longest_run_above() had the loop but
never returned anything. I had to build a stats_run_t(), set length and
start, and return it. Claude pointed it out when I shared my stats.c.
Without it, gcc warns "control reaches end of non-void function", and
the homework allows no warnings. After the fix I compiled with
-Wall -Wextra and got no output.
