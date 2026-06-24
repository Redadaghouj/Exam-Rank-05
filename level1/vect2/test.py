#!/usr/bin/env python3
"""
Tester for vect2 — 42 School C++ operator overloading exercise.

Usage:
    python3 test_vect2.py                  # expects vect2.cpp, vect2.hpp, main.cpp in CWD
    python3 test_vect2.py /path/to/dir     # specify source directory

What it does:
    1. Compiles the project (c++98 -Wall -Wextra -Werror)
    2. Runs the provided main and checks output line by line
    3. Runs extra edge-case tests via injected mains
    4. Reports results clearly
"""

import subprocess
import sys
import os
import tempfile
import shutil
from pathlib import Path

# ─── Colors ───────────────────────────────────────────────────────────────────

GREEN  = "\033[92m"
RED    = "\033[91m"
YELLOW = "\033[93m"
CYAN   = "\033[96m"
BOLD   = "\033[1m"
RESET  = "\033[0m"

def ok(msg):    print(f"  {GREEN}✓{RESET} {msg}")
def fail(msg):  print(f"  {RED}✗{RESET} {msg}")
def info(msg):  print(f"  {CYAN}→{RESET} {msg}")
def header(msg):print(f"\n{BOLD}{YELLOW}{'─'*55}{RESET}\n{BOLD}{msg}{RESET}")

# ─── Compile helper ───────────────────────────────────────────────────────────

CXX      = "c++"
CXXFLAGS = ["-Wall", "-Wextra", "-Werror", "-std=c++98"]

def compile(src_dir: Path, out: Path, extra_main: str | None = None) -> tuple[bool, str]:
    """
    Compile vect2.cpp + (extra_main or main.cpp) into `out`.
    Returns (success, stderr).
    """
    with tempfile.TemporaryDirectory() as td:
        td = Path(td)
        shutil.copy(src_dir / "vect2.cpp", td / "vect2.cpp")
        shutil.copy(src_dir / "vect2.hpp", td / "vect2.hpp")

        if extra_main:
            main_path = td / "main.cpp"
            main_path.write_text(extra_main)
        else:
            shutil.copy(src_dir / "main.cpp", td / "main.cpp")

        result = subprocess.run(
            [CXX] + CXXFLAGS + ["-o", str(out), str(td / "vect2.cpp"), str(td / "main.cpp")],
            capture_output=True, text=True
        )
        return result.returncode == 0, result.stderr

def run(binary: Path, timeout=5) -> tuple[bool, str]:
    try:
        r = subprocess.run([str(binary)], capture_output=True, text=True, timeout=timeout)
        return True, r.stdout
    except subprocess.TimeoutExpired:
        return False, "TIMEOUT"
    except Exception as e:
        return False, str(e)

# ─── Test registry ────────────────────────────────────────────────────────────

PASS = 0
FAIL = 0

def check(label: str, got: str, expected: str):
    global PASS, FAIL
    if got.strip() == expected.strip():
        ok(label)
        PASS += 1
    else:
        fail(label)
        print(f"      expected: {repr(expected.strip())}")
        print(f"      got:      {repr(got.strip())}")
        FAIL += 1

def run_extra(label: str, src_dir: Path, bindir: Path, code: str, expected: str):
    """Compile an injected main, run it, check output."""
    global PASS, FAIL
    binary = bindir / "vect2_extra"
    ok_flag, stderr = compile(src_dir, binary, extra_main=code)
    if not ok_flag:
        fail(f"{label} — compilation failed:\n{stderr}")
        FAIL += 1
        return
    ran, out = run(binary)
    if not ran:
        fail(f"{label} — runtime error: {out}")
        FAIL += 1
        return
    check(label, out, expected)

# ─── Main test suite ──────────────────────────────────────────────────────────

EXPECTED_MAIN_OUTPUT = """\
v1: {0, 0}
v1: {0, 0}
v2: {1, 2}
v3: {1, 2}
v4: {1, 2}
{1, 2}
{3, 4}
{3, 4}
{1, 2}
v1: {-84, -168}
v2: {20, 40}
-v2: {-20, -40}
v1[1]: -168
v1[1]: 12
v3[1]: 2
v1 == v3: 0
v1 == v1: 1
v1 != v3: 1
v1 != v1: 0
"""

EXTRA_TESTS = [
    (
        "Default constructor → {0, 0}",
        '#include "vect2.hpp"\n#include <iostream>\nint main(){\nvect2 v;\nstd::cout << v << std::endl;\n}',
        "{0, 0}",
    ),
    (
        "Parameterized constructor",
        '#include "vect2.hpp"\n#include <iostream>\nint main(){\nvect2 v(3,-7);\nstd::cout << v << std::endl;\n}',
        "{3, -7}",
    ),
    (
        "Copy constructor",
        '#include "vect2.hpp"\n#include <iostream>\nint main(){\nvect2 a(5,10);\nvect2 b(a);\nstd::cout << b << std::endl;\n}',
        "{5, 10}",
    ),
    (
        "operator= (assignment)",
        '#include "vect2.hpp"\n#include <iostream>\nint main(){\nvect2 a(1,2),b;\nb=a;\nstd::cout << b << std::endl;\n}',
        "{1, 2}",
    ),
    (
        "Self-assignment safety",
        '#include "vect2.hpp"\n#include <iostream>\nint main(){\nvect2 a(9,9);\na=a;\nstd::cout << a << std::endl;\n}',
        "{9, 9}",
    ),
    (
        "operator+ (addition)",
        '#include "vect2.hpp"\n#include <iostream>\nint main(){\nvect2 a(1,2),b(3,4);\nstd::cout << (a+b) << std::endl;\n}',
        "{4, 6}",
    ),
    (
        "operator- (subtraction)",
        '#include "vect2.hpp"\n#include <iostream>\nint main(){\nvect2 a(5,7),b(2,3);\nstd::cout << (a-b) << std::endl;\n}',
        "{3, 4}",
    ),
    (
        "operator* scalar (right)",
        '#include "vect2.hpp"\n#include <iostream>\nint main(){\nvect2 a(2,3);\nstd::cout << (a*4) << std::endl;\n}',
        "{8, 12}",
    ),
    (
        "operator* scalar (left, 3*v)",
        '#include "vect2.hpp"\n#include <iostream>\nint main(){\nvect2 a(2,3);\nstd::cout << (4*a) << std::endl;\n}',
        "{8, 12}",
    ),
    (
        "Multiply by 0",
        '#include "vect2.hpp"\n#include <iostream>\nint main(){\nvect2 a(5,5);\nstd::cout << (a*0) << std::endl;\n}',
        "{0, 0}",
    ),
    (
        "Multiply by negative scalar",
        '#include "vect2.hpp"\n#include <iostream>\nint main(){\nvect2 a(3,4);\nstd::cout << (a*-2) << std::endl;\n}',
        "{-6, -8}",
    ),
    (
        "Unary minus operator-",
        '#include "vect2.hpp"\n#include <iostream>\nint main(){\nvect2 a(3,-4);\nstd::cout << (-a) << std::endl;\n}',
        "{-3, 4}",
    ),
    (
        "operator+= modifies lhs",
        '#include "vect2.hpp"\n#include <iostream>\nint main(){\nvect2 a(1,2),b(3,4);\na+=b;\nstd::cout << a << std::endl;\n}',
        "{4, 6}",
    ),
    (
        "operator-= modifies lhs",
        '#include "vect2.hpp"\n#include <iostream>\nint main(){\nvect2 a(5,7),b(2,3);\na-=b;\nstd::cout << a << std::endl;\n}',
        "{3, 4}",
    ),
    (
        "operator*= modifies lhs",
        '#include "vect2.hpp"\n#include <iostream>\nint main(){\nvect2 a(2,3);\na*=5;\nstd::cout << a << std::endl;\n}',
        "{10, 15}",
    ),
    (
        "Post-increment returns old value",
        '#include "vect2.hpp"\n#include <iostream>\nint main(){\nvect2 a(1,2);\nstd::cout << a++ << std::endl;\nstd::cout << a << std::endl;\n}',
        "{1, 2}\n{2, 3}",
    ),
    (
        "Pre-increment returns new value",
        '#include "vect2.hpp"\n#include <iostream>\nint main(){\nvect2 a(1,2);\nstd::cout << ++a << std::endl;\n}',
        "{2, 3}",
    ),
    (
        "Post-decrement returns old value",
        '#include "vect2.hpp"\n#include <iostream>\nint main(){\nvect2 a(3,4);\nstd::cout << a-- << std::endl;\nstd::cout << a << std::endl;\n}',
        "{3, 4}\n{2, 3}",
    ),
    (
        "Pre-decrement returns new value",
        '#include "vect2.hpp"\n#include <iostream>\nint main(){\nvect2 a(3,4);\nstd::cout << --a << std::endl;\n}',
        "{2, 3}",
    ),
    (
        "operator[] read x (index 0)",
        '#include "vect2.hpp"\n#include <iostream>\nint main(){\nvect2 a(7,8);\nstd::cout << a[0] << std::endl;\n}',
        "7",
    ),
    (
        "operator[] read y (index 1)",
        '#include "vect2.hpp"\n#include <iostream>\nint main(){\nvect2 a(7,8);\nstd::cout << a[1] << std::endl;\n}',
        "8",
    ),
    (
        "operator[] write",
        '#include "vect2.hpp"\n#include <iostream>\nint main(){\nvect2 a(0,0);\na[0]=5; a[1]=9;\nstd::cout << a << std::endl;\n}',
        "{5, 9}",
    ),
    (
        "const operator[] read",
        '#include "vect2.hpp"\n#include <iostream>\nint main(){\nconst vect2 a(3,4);\nstd::cout << a[0] << " " << a[1] << std::endl;\n}',
        "3 4",
    ),
    (
        "operator== true",
        '#include "vect2.hpp"\n#include <iostream>\nint main(){\nvect2 a(1,2),b(1,2);\nstd::cout << (a==b) << std::endl;\n}',
        "1",
    ),
    (
        "operator== false",
        '#include "vect2.hpp"\n#include <iostream>\nint main(){\nvect2 a(1,2),b(1,3);\nstd::cout << (a==b) << std::endl;\n}',
        "0",
    ),
    (
        "operator!= true",
        '#include "vect2.hpp"\n#include <iostream>\nint main(){\nvect2 a(1,2),b(1,3);\nstd::cout << (a!=b) << std::endl;\n}',
        "1",
    ),
    (
        "operator!= false",
        '#include "vect2.hpp"\n#include <iostream>\nint main(){\nvect2 a(1,2),b(1,2);\nstd::cout << (a!=b) << std::endl;\n}',
        "0",
    ),
    (
        "Subject spec: vect2(2,2)*2 == vect2(4,4)",
        '#include "vect2.hpp"\n#include <iostream>\nint main(){\nstd::cout << (vect2(2,2)*2==vect2(4,4)) << std::endl;\n}',
        "1",
    ),
    (
        "Chained += returns reference",
        '#include "vect2.hpp"\n#include <iostream>\nint main(){\nvect2 a(1,1),b(2,2);\nvect2 c;\nc = (a += b);\nstd::cout << a << std::endl;\nstd::cout << c << std::endl;\n}',
        "{3, 3}\n{3, 3}",
    ),
    (
        "Negative values",
        '#include "vect2.hpp"\n#include <iostream>\nint main(){\nvect2 a(-5,-10),b(-3,7);\nstd::cout << (a+b) << std::endl;\n}',
        "{-8, -3}",
    ),
    (
        "operator<< format matches subject spec",
        '#include "vect2.hpp"\n#include <iostream>\n#include <sstream>\nint main(){\nvect2 v(4,5);\nstd::ostringstream s1,s2;\ns1<<v;\ns2<<"{"<<v[0]<<", "<<v[1]<<"}";\nstd::cout<<(s1.str()==s2.str())<<std::endl;\n}',
        "1",
    ),
]

# ─── Entry point ──────────────────────────────────────────────────────────────

def main():
    src_dir = Path(sys.argv[1]) if len(sys.argv) > 1 else Path(".")
    src_dir = src_dir.resolve()

    required = ["vect2.cpp", "vect2.hpp", "main.cpp"]
    missing  = [f for f in required if not (src_dir / f).exists()]
    if missing:
        print(f"{RED}Missing files in {src_dir}: {missing}{RESET}")
        sys.exit(1)

    with tempfile.TemporaryDirectory() as bindir:
        bindir = Path(bindir)

        # ── 1. Compilation ────────────────────────────────────────────────────
        header("1. COMPILATION  (-Wall -Wextra -Werror -std=c++98)")
        binary = bindir / "vect2"
        ok_flag, stderr = compile(src_dir, binary)
        if not ok_flag:
            fail("Compilation failed")
            print(stderr)
            sys.exit(1)
        ok("Compiled cleanly (no warnings, no errors)")

        # ── 2. Provided main output ───────────────────────────────────────────
        header("2. PROVIDED MAIN OUTPUT")
        ran, out = run(binary)
        if not ran:
            fail(f"Binary crashed or timed out: {out}")
            sys.exit(1)

        lines_got      = out.splitlines()
        lines_expected = EXPECTED_MAIN_OUTPUT.strip().splitlines()

        for i, (exp, got) in enumerate(zip(lines_expected, lines_got)):
            check(f"Line {i+1:>2}: expected {repr(exp)}", got, exp)

        if len(lines_got) != len(lines_expected):
            fail(f"Line count mismatch: expected {len(lines_expected)}, got {len(lines_got)}")
            FAIL  # already counted above

        # ── 3. Extra targeted tests ───────────────────────────────────────────
        header("3. EXTRA TARGETED TESTS")
        for label, code, expected in EXTRA_TESTS:
            run_extra(label, src_dir, bindir, code, expected)

        # ── Summary ───────────────────────────────────────────────────────────
        total = PASS + FAIL
        header("SUMMARY")
        print(f"  {GREEN}{PASS}{RESET} / {total} passed", end="")
        if FAIL:
            print(f"   {RED}{FAIL} FAILED{RESET}")
        else:
            print(f"   {GREEN}ALL GOOD{RESET}")
        print()

    sys.exit(0 if FAIL == 0 else 1)


if __name__ == "__main__":
    main()