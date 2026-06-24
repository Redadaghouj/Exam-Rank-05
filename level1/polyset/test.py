#!/usr/bin/env python3
"""
Polyset Tester
Tests: searchable_array_bag, searchable_tree_bag, set
"""

import subprocess
import os
import sys
import tempfile
from dataclasses import dataclass, field
from typing import List, Optional

# ── Colors ────────────────────────────────────────────────────────────────────
GREEN  = "\033[92m"
RED    = "\033[91m"
YELLOW = "\033[93m"
CYAN   = "\033[96m"
BOLD   = "\033[1m"
RESET  = "\033[0m"
DIM    = "\033[2m"

# ── Config ─────────────────────────────────────────────────────────────────────
PROJECT_DIR = os.path.abspath(os.path.join(os.path.dirname(__file__), "."))
BINARY      = os.path.join(PROJECT_DIR, "polyset")
MAKEFILE    = os.path.join(PROJECT_DIR, "Makefile")

# ── Helpers ────────────────────────────────────────────────────────────────────
@dataclass
class TestResult:
    name:    str
    passed:  bool
    message: str = ""
    details: str = ""

results: List[TestResult] = []

def run_binary(*args: str, timeout: int = 5) -> subprocess.CompletedProcess:
    cmd = [BINARY] + [str(a) for a in args]
    return subprocess.run(cmd, capture_output=True, text=True, timeout=timeout)

def record(name: str, passed: bool, message: str = "", details: str = ""):
    results.append(TestResult(name, passed, message, details))
    icon = f"{GREEN}✓{RESET}" if passed else f"{RED}✗{RESET}"
    print(f"  {icon}  {name}")
    if not passed and details:
        for line in details.strip().splitlines():
            print(f"       {DIM}{line}{RESET}")

def section(title: str):
    print(f"\n{BOLD}{CYAN}━━━  {title}  ━━━{RESET}")

# ── Build ──────────────────────────────────────────────────────────────────────
def build() -> bool:
    section("BUILD")
    if not os.path.isfile(MAKEFILE):
        print(f"{RED}  Makefile not found at {MAKEFILE}{RESET}")
        return False

    r = subprocess.run(["make", "-C", PROJECT_DIR, "re"],
                       capture_output=True, text=True, timeout=30)
    ok = r.returncode == 0 and os.path.isfile(BINARY)
    if ok:
        print(f"  {GREEN}✓  Build successful{RESET}")
    else:
        print(f"  {RED}✗  Build failed{RESET}")
        print(r.stderr[-2000:])
    return ok

# ── Helper: parse the output produced by the original main ────────────────────
def parse_main_output(out: str) -> dict:
    """
    The main inserts values into tree_bag and array_bag, prints them,
    then checks has() for each value and value-1.
    Returns raw lines for manual inspection.
    """
    return {"lines": [l for l in out.splitlines() if l.strip()]}

# ── Test groups ────────────────────────────────────────────────────────────────

def test_no_args():
    section("NO-ARGS / EDGE CASES")
    r = run_binary()
    record("Exit 1 with no arguments", r.returncode == 1,
           details=f"got returncode={r.returncode}")

def test_single_value():
    section("SINGLE VALUE")
    r = run_binary(42)
    record("Returns 0", r.returncode == 0, details=r.stderr)

    lines = _content_lines(r.stdout)
    content_lines = [l for l in lines if "42" in l]
    record("Output contains inserted value 42",
           len(content_lines) >= 2,
           details=f"stdout:\n{r.stdout[:500]}")

def test_has_found_and_not_found():
    section("HAS — FOUND / NOT FOUND")
    r = run_binary(10, 20, 30)
    lines = r.stdout.splitlines()

    # main emits 4 bool lines per argv value:
    #   [0] t->has(v)      → 1
    #   [1] a->has(v)      → 1
    #   [2] t->has(v-1)    → 0
    #   [3] a->has(v-1)    → 0
    bool_lines = [l.strip() for l in lines if l.strip() in ("0", "1")]

    # 3 values × 4 lines = 12
    record("Correct number of has() output lines (12 for 3 values)",
           len(bool_lines) == 12,
           details=f"bool lines found: {bool_lines}")

    # groups of 4: [found_tree, found_array, notfound_tree, notfound_array]
    found     = [bool_lines[i]   for i in range(0, len(bool_lines), 4)] + \
                [bool_lines[i+1] for i in range(0, len(bool_lines), 4) if i+1 < len(bool_lines)]
    not_found = [bool_lines[i+2] for i in range(0, len(bool_lines), 4) if i+2 < len(bool_lines)] + \
                [bool_lines[i+3] for i in range(0, len(bool_lines), 4) if i+3 < len(bool_lines)]

    record("has(v) returns 1 for inserted values",
           all(v == "1" for v in found),
           details=f"found lines: {found}")

    record("has(v-1) returns 0 for non-inserted values",
           all(v == "0" for v in not_found),
           details=f"not-found lines: {not_found}")

def _content_lines(stdout: str) -> list:
    """Filter out all debug lines emitted by tree_bag internals."""
    skip = ("create node", "destroy", "duplicate value")
    return [l for l in stdout.splitlines()
            if l.strip() and not any(l.startswith(p) for p in skip)]

def test_duplicate_handling():
    section("DUPLICATES")
    r = run_binary(5, 5, 5)
    lines = _content_lines(r.stdout)
    first_print = lines[0] if lines else ""
    record("tree_bag deduplicated: only one '5' in first print",
           first_print.strip() == "5",
           details=f"first print line: '{first_print}'")

def test_set_no_duplicates():
    """
    The set must reject duplicates.  We exercise this via the main's set code:
    main inserts the same argv values into set sa / st, then inserts {1,2,3,4}.
    We can't easily parse set output here without rewriting main, so we at
    least verify the binary doesn't crash and returns 0.
    """
    section("SET — NO DUPLICATES / BEHAVIOUR")
    r = run_binary(1, 2, 3, 4)
    record("Binary exits 0 with set operations", r.returncode == 0,
           details=r.stderr)

def test_clear_then_use():
    section("CLEAR THEN RE-USE")
    # After clear, copy-constructed tmp.print() should print nothing
    # (because the bag was cleared before copy).
    r = run_binary(7, 8, 9)
    # We just verify no crash and exit 0
    record("No crash after clear() and copy-construction on empty bag",
           r.returncode == 0, details=r.stderr)

def test_sorted_tree_output():
    section("TREE SORTED OUTPUT")
    r = run_binary(5, 3, 8, 1, 4)
    lines = _content_lines(r.stdout)
    if lines:
        first = lines[0]
        nums = list(map(int, first.split()))
        record("tree_bag prints values in sorted (in-order) order",
               nums == sorted(nums),
               details=f"printed: {first}")
    else:
        record("tree_bag prints values in sorted order", False,
               details="no parseable output line")

def test_array_insertion_order():
    section("ARRAY INSERTION ORDER")
    vals = [9, 3, 6, 1]
    r = run_binary(*vals)
    lines = _content_lines(r.stdout)
    if len(lines) >= 2:
        # second print line is array_bag (insertion order)
        arr_line = lines[1]
        nums = list(map(int, arr_line.split()))
        record("array_bag preserves insertion order",
               nums == vals,
               details=f"expected {vals}, got {nums}")
    else:
        record("array_bag preserves insertion order", False,
               details="not enough output lines")

def test_large_input():
    section("LARGE INPUT (stress)")
    vals = list(range(1, 51))   # 50 distinct values
    r = run_binary(*vals, timeout=10)
    record("Handles 50 values without crash", r.returncode == 0,
           details=r.stderr[:300] if r.returncode != 0 else "")

def test_negative_values():
    section("NEGATIVE VALUES")
    r = run_binary(-5, -3, -1, 0, 2)
    record("Handles negative integers without crash", r.returncode == 0,
           details=r.stderr)
    lines = _content_lines(r.stdout)
    if lines:
        nums = list(map(int, lines[0].split()))
        record("Negative values sorted correctly in tree",
               nums == sorted(nums),
               details=f"got: {nums}")



# ── Summary ────────────────────────────────────────────────────────────────────
def summary():
    total  = len(results)
    passed = sum(1 for r in results if r.passed)
    failed = total - passed

    print(f"\n{BOLD}{'━'*45}{RESET}")
    print(f"{BOLD}  RESULTS: {GREEN}{passed} passed{RESET}  "
          f"{RED if failed else ''}{failed} failed{RESET}  / {total} total")
    print(f"{BOLD}{'━'*45}{RESET}\n")

    if failed:
        print(f"{RED}{BOLD}  Failed tests:{RESET}")
        for r in results:
            if not r.passed:
                print(f"    {RED}✗  {r.name}{RESET}")
        sys.exit(1)
    else:
        print(f"{GREEN}{BOLD}  All tests passed!{RESET}\n")

# ── Entry point ────────────────────────────────────────────────────────────────
if __name__ == "__main__":
    print(f"\n{BOLD}{'━'*45}")
    print(f"  POLYSET TESTER")
    print(f"{'━'*45}{RESET}")

    if not build():
        print(f"{RED}Aborting: build failed.{RESET}")
        sys.exit(1)

    test_no_args()
    test_single_value()
    test_has_found_and_not_found()
    test_duplicate_handling()
    test_set_no_duplicates()
    test_clear_then_use()
    test_sorted_tree_output()
    test_array_insertion_order()
    test_large_input()
    test_negative_values()

    summary()