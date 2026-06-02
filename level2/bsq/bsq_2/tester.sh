#!/bin/bash

RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m'

echo "BSQ Strict Tester (Subject Compliant)"

# ---------- VALID TESTS ----------

cat > valid1.log << 'EOF'
9.ox
...........................
....o......................
............o..............
...........................
....o......................
...............o...........
...........................
......o..............o.....
..o.......o................
EOF

cat > valid2.log << 'EOF'
3-#+
---
-#-
---
EOF

cat > valid3.log << 'EOF'
1.ox
.....
EOF

# ---------- ERROR TESTS ----------

cat > err1.log << 'EOF'
3..x
...
...
...
EOF

cat > err2.log << 'EOF'
3.ox
...
..
...
EOF

cat > err3.log << 'EOF'
5.ox
...
...
...
EOF

cat > err4.log << 'EOF'
3.ox
...
.a.
...
EOF

cat > err5.log << 'EOF'
3.ox

...
...
EOF

cat > err6.log << 'EOF'
abc.ox
...
...
...
EOF

cat > err7.log << 'EOF'
0.ox
EOF

cat > err8.log << 'EOF'
EOF

cat > err9.log << 'EOF'
3.ox
EOF

# ---------- TEST RUNNER ----------

run_test() {
    file=$1
    expected=$2

    output=$(./bsq "$file")

    if [[ "$expected" == "error" ]]; then
        if [[ "$output" == "Error: invalid map" ]]; then
            echo -e "${GREEN}PASS${NC} $file"
        else
            echo -e "${RED}FAIL${NC} $file"
            echo "Output: $output"
        fi
    else
        if [[ "$output" == "Error: invalid map" ]]; then
            echo -e "${RED}FAIL${NC} $file (unexpected error)"
        else
            echo -e "${GREEN}PASS${NC} $file"
        fi
    fi
}

echo "Compiling..."
cc -Wall -Wextra -Werror -o bsq bsq_free_gemini.c || exit 1

echo "Running tests..."

run_test valid1.log success
run_test valid2.log success
run_test valid3.log success

run_test err1.log error
run_test err2.log error
run_test err3.log error
run_test err4.log error
run_test err5.log error
run_test err6.log error
run_test err7.log error
run_test err8.log error
run_test err9.log error

echo ""
echo "STDIN test:"
echo "cat valid1.log | ./bsq"