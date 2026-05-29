#!/usr/bin/env python3
import subprocess
import sys

EXPECTED = [
    ("a = 42",                    "constructor(42)"),
    ("b = 21",                    "constructor(21)"),
    ("c = 0",                     "default constructor"),
    ("d = 1337",                  "constructor(1337)"),
    ("e = 1337",                  "copy constructor"),
    ("a + b = 63",                "42 + 21"),
    ("(c += a) = 42",             "0 += 42"),
    ("b = 21",                    "b unchanged after +="),
    ("++b = 22",                  "pre-increment"),
    ("b++ = 22",                  "post-increment returns old value"),
    ("(b << 10) + 42 = 230000000042", "23 << 10 = 230000000000, + 42"),
    ("(d <<= 4) = 13370000",      "1337 <<= 4"),
    ("(d >>= 2) = 133700",        "13370000 >>= bigint(2)"),
    ("a =42",                     "a unchanged"),
    ("d =133700",                 "d after shifts"),
    ("(d < a) = 0",               "133700 < 42"),
    ("(d <= a) = 0",              "133700 <= 42"),
    ("(d > a) = 1",               "133700 > 42"),
    ("(d >= a) = 1",              "133700 >= 42"),
    ("(d == a) = 0",              "133700 == 42"),
    ("(d != a) = 1",              "133700 != 42"),
]

GREEN  = "\033[92m"
RED    = "\033[91m"
YELLOW = "\033[93m"
RESET  = "\033[0m"
BOLD   = "\033[1m"

def compile_project():
    print(f"{BOLD}── Compiling ───────────────────────────────{RESET}")
    r = subprocess.run(
        ["g++", "-std=c++98", "-Wall", "-Wextra", "-o", "bigint_test",
         "main.cpp", "bigint.cpp"],
        capture_output=True, text=True
    )
    if r.returncode != 0:
        print(f"{RED}FAILED{RESET}\n{r.stderr}")
        sys.exit(1)
    print(f"{GREEN}OK{RESET}\n")

def run_binary():
    r = subprocess.run(["./bigint_test"], capture_output=True, text=True)
    if r.returncode != 0:
        print(f"{RED}Runtime error:{RESET}\n{r.stderr}")
        sys.exit(1)
    return r.stdout.strip().splitlines()

def run_tests(lines):
    print(f"{BOLD}── Tests ───────────────────────────────────{RESET}")
    passed = failed = 0

    for i, (exp_line, desc) in enumerate(EXPECTED):
        got = lines[i] if i < len(lines) else "<missing>"
        ok  = got == exp_line
        if ok:
            passed += 1
            print(f"  {GREEN}✓{RESET}  {desc}")
        else:
            failed += 1
            print(f"  {RED}✗{RESET}  {desc}")
            print(f"       expected : {YELLOW}{exp_line}{RESET}")
            print(f"       got      : {RED}{got}{RESET}")

    if len(lines) != len(EXPECTED):
        print(f"\n{RED}Line count: expected {len(EXPECTED)}, got {len(lines)}{RESET}")

    print(f"\n{BOLD}── Result ──────────────────────────────────{RESET}")
    if failed == 0:
        print(f"{GREEN}{BOLD}ALL {passed} TESTS PASSED{RESET}")
    else:
        print(f"{RED}{BOLD}{failed} FAILED{RESET} / {passed} passed")
    return failed == 0

if __name__ == "__main__":
    compile_project()
    lines = run_binary()
    ok = run_tests(lines)
    sys.exit(0 if ok else 1)
