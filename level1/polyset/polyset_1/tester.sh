#!/bin/bash

# --- Configuration ---
EXEC="polyset"
LOG_FILE="test_results.log"
TEMP_OUT=".tmp_out"

# --- UI Colors & Formatting (Fixed for printf) ---
RED='\033[1;31m'
GREEN='\033[1;32m'
CYAN='\033[1;36m'
YELLOW='\033[1;33m'
BOLD='\033[1m'
NC='\033[0m' # No Color

# --- Test Cases ---
declare -a TESTS=(
    "Basic_Insert"   "1 2 3"
    "Duplicates"     "5 5 5"
    "Negative_Vals"  "-1 0 1"
    "Large_Seq"      "10 20 30 40 50"
    "Single_Val"     "42"
    "Mixed_Order"    "9 1 8 2 7 3"
)

# --- Initialization ---
rm -f $LOG_FILE
PASS_COUNT=0
FAIL_COUNT=0

clear
echo -e "${CYAN}┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓${NC}"
echo -e "${CYAN}┃${NC}          ${BOLD}POLYSET AUTOMATED TEST SUITE v1.2${NC}              ${CYAN}┃${NC}"
echo -e "${CYAN}┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛${NC}"

# --- Cleanup ---
cleanup() {
    rm -f $TEMP_OUT
    if [ -f "Makefile" ]; then
        make fclean > /dev/null 2>&1
    fi
    echo -e "\n${YELLOW}🧹  Cleanup: Files cleared.${NC}"
}
trap cleanup EXIT

# --- Build Phase ---
echo -ne "${BOLD}🚀  Building project... ${NC}"
if [ -f "Makefile" ]; then
    make > /dev/null 2>&1
    if [ $? -ne 0 ]; then
        echo -e "${RED}BUILD FAILED${NC}"
        exit 1
    fi
    echo -e "${GREEN}SUCCESS${NC}\n"
else
    echo -e "${RED}NO MAKEFILE${NC}"
    exit 1
fi

# --- Column Headers ---
# We use %b in printf to ensure backslash escapes are interpreted
printf "${BOLD}%-20s %-25s %-10s${NC}\n" "TEST NAME" "ARGUMENTS" "STATUS"
echo -e "${CYAN}------------------------------------------------------------${NC}"

# --- Test Execution ---
for ((i=0; i<${#TESTS[@]}; i+=2)); do
    NAME="${TESTS[i]}"
    ARGS="${TESTS[i+1]}"
    
    ./$EXEC $ARGS > $TEMP_OUT 2>&1
    STATUS=$?

    # Log details
    {
        echo "TEST: $NAME | ARGS: $ARGS | EXIT: $STATUS"
        cat $TEMP_OUT
        echo -e "----------------------------------------\n"
    } >> $LOG_FILE

    # Terminal Output: %b interprets the color variables correctly
    printf "${BOLD}%-20s${NC} %-25s " "$NAME" "($ARGS)"
    
    if [ $STATUS -eq 0 ] && [ -s $TEMP_OUT ]; then
        printf "${GREEN}${BOLD}[ PASS ]${NC}\n"
        ((PASS_COUNT++))
    else
        printf "${RED}${BOLD}[ FAIL ]${NC}\n"
        ((FAIL_COUNT++))
    fi
done

# --- Final Summary ---
echo -e "${CYAN}------------------------------------------------------------${NC}"
printf "${BOLD}FINAL RESULTS:${NC}\n"
printf "  ✅  ${GREEN}Passed: %d${NC}\n" "$PASS_COUNT"
printf "  ❌  ${RED}Failed: %d${NC}\n" "$FAIL_COUNT"
echo -e "${CYAN}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"