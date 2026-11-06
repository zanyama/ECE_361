#!/bin/sh
# regress.sh: copied from the HW2 regression test. Runs ./replay on every
# input file in scenarios/ and compares its output with the file of the same name
# in expected/.
# Exits with 1 if any output differs, so that make test fails.
fail=0

for input in scenarios/*.txt; do
    name=$(basename "$input")
    if ./replay "$input" | diff - "expected/$name" > /dev/null; then
        echo "PASS  regression $name"
    else
        echo "FAIL  regression $name"
        fail=1
    fi
done

exit $fail
