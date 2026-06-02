#!/bin/bash

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m'

# Compile life.c
echo "Compiling life.c..."
if ! cc -Wall -Wextra -Werror -o life life_gemini.c; then
    echo -e "${RED}Compilation failed!${NC}"
    exit 1
fi
echo -e "${GREEN}Compilation successful!${NC}"
echo ""

# Run a single test
run_test() {
    local desc="$1"
    local width="$2"
    local height="$3"
    local iterations="$4"
    local input="$5"
    local expected="$6"

    echo -n "$desc: "

    output=$(echo -n "$input" | ./life "$width" "$height" "$iterations")
    
    if [ "$output" == "$expected" ]; then
        echo -e "${GREEN}PASS${NC}"
    else
        echo -e "${RED}FAIL${NC}"
        echo "Input: $input"
        echo "Expected:"
        echo "$expected" | cat -e
        echo "Got:"
        echo "$output" | cat -e
        echo ""
    fi
}

# ========== TEST CASES ==========

# Test 1: pen drawing, no iterations
run_test "Test 1 - Simple pen draw 5x5 iter 0" 5 5 0 "sdxddssaaww" "     
 OOO 
 O O 
 OOO 
     "

# Test 2: pen draw 10x6 iter 0
run_test "Test 2 - Complex pen draw 10x6 iter 0" 10 6 0 "sdxssdswdxdddxsaddawxwdxwaa" "          
 O   OOO  
 O     O  
 OOO  O   
  O  OOO  
          "

# Test 3: pen draw 3x3 iter 0
run_test "Test 3 - 3x3 iter 0" 3 3 0 "dxss" " O 
 O 
 O "

# Test 4: 3x3 blinker, iter 1
run_test "Test 4 - 3x3 iter 1 (blinker)" 3 3 1 "dxss" "   
OOO
   "

# Test 5: 3x3 blinker, iter 2
run_test "Test 5 - 3x3 iter 2 (blinker)" 3 3 2 "dxss" " O 
 O 
 O "

# Test 6: invalid width/height/iterations
echo -n "Test 6 - Invalid width/height/iterations: "
if ./life -1 3 1 2>/dev/null; then
    echo -e "${RED}FAIL${NC}"
else
    echo -e "${GREEN}PASS${NC}"
fi

echo ""
echo "All tests done."

rm life