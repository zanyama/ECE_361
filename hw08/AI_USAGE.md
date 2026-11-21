# AI usage

**Tool:** Claude Opus 5.5 (Claude Code)

**How I used it:** One step at a time, like HW6 and HW7. For each part
(bst.h, each function in bst.c, the tests, the crash, treestats.c)
Claude explained anything I didn't understand:
which includes to include in the header, the tree struct vs the node struct,
counting visits (and why the found node has to be counted after the
loop), *visits vs visits, & vs &&, recursion on a tree for height and
destroy, scanf's return values, and stderr. It gave hints and skeletons,
and I wrote the code.
After each step Claude compiled it, ran small checks of its own (not
submitted), and pointed out bugs, which I fixed.

**What Claude wrote:**
- The empty Takes/Returns/Assumes comment template in bst.h (I filled
  in every comment)
- Skeletons I filled in: the node_height helper, and the local-counter
  pattern for visits
- The bst_insert error check in treestats.c (the out-of-memory
  fprintf, destroy, return 1 block)
- The check_bool printf with ?: for true/false in tests/test_bst.c
- A draft of README.md, including pasting in my two treestats outputs.
  I wrote the sentence on why the original never noticed the crash and
  the two sentences on insertion order, and edited the rest
- A draft of this file, which I edited

**What I wrote:** All the code in bst.h, bst.c, treestats.c and
tests/test_bst.c except what is listed above, including the set_visits
helper (my idea) and the count field in struct bst. I adapted my HW7
Makefile. I found the crash with the empty-tree test under make
memcheck, copied the report, and then fixed bst_contains.

I also wrote a debug program (debug/show.c, not submitted) that prints
a tree sideways, to visualize the tree shapes.

**One thing it got wrong:** When it pointed me to where the missing
visit should be counted, it gave line numbers counted from the top of
bst_contains instead of the file, so they didn't match my editor. I
noticed and it corrected them.
