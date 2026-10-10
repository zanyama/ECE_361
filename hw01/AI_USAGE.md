# AI usage

**Tool:** Claude Sonnet 5.5 (chat)

**What I used it for:**
- Explaining masks, shifts, and two's complement
- Clarifying how git, gcc, make, and C work
- Reviewing my code and compiler errors
- Suggesting boundary test cases
- Creating the Makefile and test harness starter code

**Who wrote what:** I wrote get_field, set_field, the valid_field and
make_mask helpers, and status_unpack myself, with some pointers from 
Claude. Claude supplied the starting code for sign_extend, print_binary,
status.h, the Makefile, and the test harness, which I typed in,
tested, and can explain.
I typed in a lot of the code instead of copy/paste, so there was an 
occasional typo here and there that would get caught when 
compiling/testing

**One thing I had to fix:** After I refactored the repeated validity
check into a valid_field helper function, every get_field call returned 
0 and every set_field returned the word unchanged. Claude had suggested
"if (!valid_field(...))", but I wrote "~" (bitwise NOT) instead of "!" 
(logical NOT). valid_field returns "1" for valid, and ~1 = 0xFFFFFFFE
always evaluates to "true", because 0xFFFFFFFE is nonzero, so every call
took the "invalid" branch. I found it because the output was always 0
or the unchanged word, which is exactly what the invalid branch returns. 
Claude pointed at the two callers, and I fixed them.
