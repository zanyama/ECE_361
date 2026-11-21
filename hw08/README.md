# HW8: a binary search tree ADT, ported from BinarySearchTree.c

## What it does
- bst.h / bst.c: bst_t, a binary search tree of int, ported from
  TutorialsPoint's BinarySearchTree.c. The original kept one global tree
  (`struct node *root`), printed inside search, and never freed anything.
  Here there are no globals: struct bst (the root and a count of keys) and
  struct node are complete only in bst.c, so other files hold a bst_t *
  but cannot reach inside it. Smaller keys go left and all others go
  right, as in the original, so an equal key is kept, to the right.
  bst_contains is the original's search without the printing; it reports
  how many nodes it compared through an optional visits pointer.
- treestats.c: `./treestats < file` reads integers until the end of the
  input, inserts them in that order, and prints one line:

      n 7, height 3                                   exit status 0
      empty tree                                      exit status 0
      not a number after 4 values                     exit status 1

## Build and test
    make                     # builds treestats and the tests, no warnings
    ./treestats < file
    make test                # unit tests
    make memcheck            # rebuilds with -fsanitize=address,undefined and runs make test
    make clean               # removes everything the build made

## Tests
tests/test_bst.c prints PASS or FAIL per check and a summary, and exits
nonzero if any check fails, so make test stops there. It checks:

- the original's seven keys (27 14 35 10 19 31 42): size 7, height 3,
  31 found in 3 visits, 15 not found in 3
- a duplicate is kept: inserting 69 twice gives size 2 (and height 2,
  since the copy is the first one's right child)
- keys inserted in increasing order (0 to 9) give height 10, equal to
  their number
- searching an empty tree: not found, 0 visits (the crash below)
- destroying full trees: make memcheck reports no leaks

## The crash
I ported search faithfully first, loop and all. The empty-tree test
(tests/test_bst.c: create a tree, insert nothing, bst_contains(t, 5, &visits))
crashed it under make memcheck:

    bst.c:101:19: runtime error: member access within null pointer of type 'const struct node'

Line 101 was the loop condition, `while (current->data != key)`: in an
empty tree current starts as t->root, which is NULL, and the condition
reads current->data before anything checks current. The original's own
`if(current != NULL)` is inside the loop, so it comes too late.

The original program never noticed, because it never tried searching
through an empty tree. It always inserted its seven keys before searching:
the root was never NULL when the search ran.

The fix: bst_contains now checks for an empty tree before the loop and
returns false with 0 visits. The loop itself is unchanged.

## What order does to a tree
    $ seq 1 1023 | ./treestats
    n 1023, height 1023
    $ seq 1 1023 | sort -R | ./treestats
    n 1023, height 20

(sort -R shuffles differently each run, so the second height varies a
little.)

Insertion order can vastly affect the height- the more branches there are, the fewer layers will be used, and will have a smaller number of visits to find the key. If we were to input keys in increasing order, we would get one long branch of height 1023, which would necessitate up to 1023 visits for certain numbers,
whereas, with the shuffled order, it takes at most 20.

