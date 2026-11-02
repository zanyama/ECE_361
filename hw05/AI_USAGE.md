# AI usage

**Tool:** Claude Opus 5.5 (Claude Code)

**What I used it for:**
- Turning my transition table into code. I filled out the table by hand
  (every state and event, its next state and action) and sent Claude a
  photo of it, and it turned that into lock_table in lock.c.
- The getchar() loop structure in main.c, and general hints on how to
  structure lock.c
- Working out the logic in check.c for checking whether a table is
  valid (the bonus)
- Writing tests/test_lock.c, tests/test_check.c, the Makefile, and the
  README together with Claude
- Walking through every file and explaining the parts I didn't know
  (enums, snprintf, tmpfile/fread, strtok, the search in check.c)
- A review against my HW4 style and for efficiency. That added a break
  in check_way_out, a Usage line in main.c, two tests for lock_event
  with a bad state (61 tests now, all pass), and wrapped long lines.
- Drafting step-by-step comments in the .c files, which I then went
  through and changed

**What I wrote:** The transition table itself: I worked out every
entry by hand on paper before it became lock_table. I also went
through every file and rewrote comments where
the draft didn't make sense to me: the comments on the lock_table rows,
the lock_event description, the code[] comment in lock.c, the comments
in main.c (the print comment is mine), and the usable() comment in
check.c.

**One thing it got wrong:**
A draft comment in main.c said "the next state comes back, the action
goes into a." I didn't know what "comes back" meant. It just meant
lock_step returns the next state, and the action gets written into a
through the pointer, because a function can only return one value. I
had it reworded so it says that.
