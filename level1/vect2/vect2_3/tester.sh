#!/bin/bash

# --- Configuration ---
EXEC="vect2"
LOG_FILE="vect2_results.log"
TEMP_OUT=".tmp_out"

# --- UI Colors & Formatting ---
RED='\033[1;31m'
GREEN='\033[1;32m'
CYAN='\033[1;36m'
YELLOW='\033[1;33m'
BOLD='\033[1m'
NC='\033[0m' # No Color

# --- Initialization ---
rm -f $LOG_FILE
clear
echo -e "${CYAN}┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓${NC}"
echo -e "${CYAN}┃${NC}          ${BOLD}VECT2 AUTOMATED PROJECT TESTER${NC}                 ${CYAN}┃${NC}"
echo -e "${CYAN}┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛${NC}"

# --- Cleanup Function ---
cleanup() {
    rm -f $TEMP_OUT
    if [ -f "Makefile" ]; then
        make fclean > /dev/null 2>&1
    fi
    echo -e "\n${YELLOW}🧹  Cleanup: make fclean executed.${NC}"
}
trap cleanup EXIT

# --- Build Phase ---
echo -ne "${BOLD}🚀  Building with Makefile... ${NC}"
if [ -f "Makefile" ]; then
    make > /dev/null 2>&1
    if [ $? -ne 0 ]; then
        echo -e "${RED}BUILD FAILED${NC}"
        exit 1
    fi
    echo -e "${GREEN}SUCCESS${NC}\n"
else
    echo -e "${RED}NO MAKEFILE FOUND${NC}"
    exit 1
fi

# --- Fixed Header ---
printf "${BOLD}%-25s %-10s${NC}\n" "TEST STEP" "STATUS"
echo -e "${CYAN}------------------------------------------------------------${NC}"

# --- Execution ---
./$EXEC > $TEMP_OUT 2>&1
STATUS=$?

# Step 1: Execution Status
printf "${BOLD}%-25s${NC} " "Execution"
if [ $STATUS -eq 0 ]; then
    echo -e "${GREEN}[ PASS ]${NC}"
else
    echo -e "${RED}[ FAIL ]${NC}"
fi

# Step 2: Format Verification
printf "${BOLD}%-25s${NC} " "Operator << Format"
V1_AUTO=$(grep "v1:" $TEMP_OUT | head -n 1 | awk '{print $2}')
V1_MANUAL=$(grep "v1:" $TEMP_OUT | sed -n '2p' | awk '{print $2}')

if [ "$V1_AUTO" == "$V1_MANUAL" ] && [ ! -z "$V1_AUTO" ]; then
    echo -e "${GREEN}[ PASS ]${NC}"
else
    echo -e "${RED}[ FAIL ]${NC}"
fi

# --- Log Full Output ---
{
    echo "VECT2 TEST LOG - $(date)"
    echo "----------------------------------------"
    cat $TEMP_OUT
} > $LOG_FILE

# --- Live Summary ---
echo -e "${CYAN}------------------------------------------------------------${NC}"
echo -e "${BOLD}PROGRAM OUTPUT:${NC}"
echo -e "${YELLOW}"
cat $TEMP_OUT
echo -e "${NC}"
echo -e "${CYAN}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"
echo -e "📂  ${BOLD}Full log kept in: $LOG_FILE${NC}"