#!/bin/sh
# regress.sh: as in HW6. Runs ./balance < each file in inputs/ and compares
# its output with expected/NAME.out and its exit status with expected/NAME.status.
# stderr is compared too, so a sanitizer report makes the test fail.
# Exits with 1 if anything differs, so that make test fails.
fail=0 total=0 passed=0
out=$(mktemp)

for input in inputs/*.txt; do
    total=$((total+1))
    name=$(basename "$input" .txt)
    ./balance < "$input" > "$out" 2>&1
    status=$?
    want=$(cat "expected/$name.status")
    if diff "$out" "expected/$name.out" > /dev/null && [ "$status" -eq "$want" ]; then
        echo "PASS  regression $name"
        passed=$((passed + 1))
    else
        echo "FAIL  regression $name (exit $status, want $want)"
        diff "$out" "expected/$name.out"
        fail=1
    fi
done

echo "$passed/$total regression tests passed"
rm -f "$out"
exit $fail
