#!/bin/bash

# rip_tester.sh — bigint showdown
# Usage: ./tester.sh

# Colors
RED="\033[31m"
GREEN="\033[32m"
CYAN="\033[36m"
YELLOW="\033[33m"
BOLD="\033[1m"
RESET="\033[0m"

# Compile
echo -e "${CYAN}${BOLD}💀 Compiling bigint...${RESET}"
rm -f bigint
c++ -Wall -Wextra -Werror main.cpp bigint.cpp -std=c++98 -o bigint

if [ $? -ne 0 ]; then
    echo -e "${RED}Compilation failed.${RESET}"
    exit 1
fi
echo -e "${GREEN}✅ Compilation OK${RESET}\n"

# Run and capture output
echo -e "${CYAN}${BOLD}🚀 Running program...${RESET}"
./bigint > out.log 2>&1

echo -e "${YELLOW}${BOLD}======== PROGRAM OUTPUT ========${RESET}"
cat out.log
echo -e "${YELLOW}${BOLD}================================${RESET}\n"

# Expected outputs (exact lines)
expected_outputs=(
    "a = 42"
    "b = 21"
    "c = 0"
    "d = 1337"
    "e = 1337"
    "a + b = 63"
    "(c += a) = 42"
    "b = 21"
    "++b = 22"
    "b++ = 22"
    "(b << 10) + 42 = 230000000042"
    "(d <<= 4) = 13370000"
    "(d >>= 2) = 133700"
    "a =42"
    "d =133700"
    "(d < a) = 0"
    "(d <= a) = 0"
    "(d > a) = 1"
    "(d >= a) = 1"
    "(d == a) = 0"
    "(d != a) = 1"
)

# Labels (for nicer display)
labels=(
    "Init: a"
    "Init: b"
    "Init: c"
    "Init: d"
    "Init: e"
    "Addition"
    "c += a"
    "b before ++"
    "Prefix ++"
    "Postfix ++"
    "Left shift + add"
    "Left shift assign"
    "Right shift assign"
    "Ostream a"
    "Ostream d"
    "Compare d < a"
    "Compare d <= a"
    "Compare d > a"
    "Compare d >= a"
    "Compare d == a"
    "Compare d != a"
)

echo -e "${CYAN}${BOLD}🧪 Checking exact output matches...${RESET}\n"
ok=0
ko=0

for ((i=0; i<${#expected_outputs[@]}; i++)); do
    pattern="${expected_outputs[$i]}"
    label="${labels[$i]}"
    if grep -Fxq "$pattern" out.log; then
        echo -e "${GREEN}OK${RESET}  $label  | expected: ${YELLOW}$pattern${RESET}"
        ((ok++))
    else
        echo -e "${RED}KO${RESET}  $label  | expected: ${YELLOW}$pattern${RESET}"
        ((ko++))
    fi
done

# Summary
echo -e "\n${CYAN}${BOLD}📊 Summary:${RESET}"
echo -e "${GREEN}${ok} OK${RESET} / ${RED}${ko} KO${RESET}"

if [ $ko -eq 0 ]; then
    echo -e "${GREEN}${BOLD}🏆 All tests passed perfectly.${RESET}"
else
    echo -e "${RED}${BOLD}💥 Some tests failed. Check out out.log for clues.${RESET}"
fi

rm -f bigint