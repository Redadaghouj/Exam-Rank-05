#!/usr/bin/env python3
"""
Tester for the 42 'life' (Game of Life) exercise.
Usage:
    python3 test.py ./a.out
"""

import subprocess
import sys
import os

# ─── ANSI colors ────────────────────────────────────────────────────────────
GREEN  = "\033[92m"
RED    = "\033[91m"
YELLOW = "\033[93m"
CYAN   = "\033[96m"
RESET  = "\033[0m"
BOLD   = "\033[1m"

# ─── Reference implementation (Python) ──────────────────────────────────────

def simulate(commands: str, width: int, height: int, iterations: int) -> str:
    """Pure Python reference: draw board then run GoL."""
    grid = [[' '] * width for _ in range(height)]
    x, y, pen = 0, 0, False

    for c in commands:
        if   c == 'w' and y > 0:          y -= 1
        elif c == 's' and y < height - 1: y += 1
        elif c == 'a' and x > 0:          x -= 1
        elif c == 'd' and x < width - 1:  x += 1
        elif c == 'x':
            pen = not pen
        else:
            continue  # invalid → skip, no mark
        if pen:
            grid[y][x] = 'O'

    def neighbors(g, gy, gx):
        n = 0
        for dy in (-1, 0, 1):
            for dx in (-1, 0, 1):
                if dy == 0 and dx == 0:
                    continue
                ny, nx = gy + dy, gx + dx
                if 0 <= ny < height and 0 <= nx < width:
                    if g[ny][nx] == 'O':
                        n += 1
        return n

    for _ in range(iterations):
        new = [[' '] * width for _ in range(height)]
        for gy in range(height):
            for gx in range(width):
                nb = neighbors(grid, gy, gx)
                if grid[gy][gx] == 'O':
                    new[gy][gx] = 'O' if nb in (2, 3) else ' '
                else:
                    new[gy][gx] = 'O' if nb == 3 else ' '
        grid = new

    return '\n'.join(''.join(row) for row in grid) + '\n'


# ─── Test cases ──────────────────────────────────────────────────────────────

# Format: (description, commands, width, height, iterations)
TESTS = [
    # ── Subject examples ────────────────────────────────────────────────────
    ("Subject ex1: blinker 0 iter",     "sdxddssaaww",             5,  5,  0),
    ("Subject ex2: glider-like 0 iter", "sdxssdswdxddddsxaadwxwdxwaa", 10, 6, 0),
    ("Subject ex3: vertical bar 0",     "dxss",                    3,  3,  0),
    ("Subject ex4: vertical bar 1",     "dxss",                    3,  3,  1),
    ("Subject ex5: vertical bar 2",     "dxss",                    3,  3,  2),

    # ── Edge cases ──────────────────────────────────────────────────────────
    ("Empty board 0 iter",              "",                        5,  5,  0),
    ("Empty board 5 iter",              "",                        5,  5,  5),
    ("Single cell stays dead",          "x",                      3,  3,  1),  # pen on at (0,0) then no move
    ("Pen starts off, no marks",        "wwwwdddddssss",          5,  5,  0),

    # ── Boundary movement ───────────────────────────────────────────────────
    ("Stay at left edge",               "aaaxsss",               3,  5,  0),
    ("Stay at top edge",                "wwwxddd",               5,  3,  0),
    ("Stay at bottom edge",             "sssxddd",               5,  3,  0),
    ("Stay at right edge",              "dddxsss",               5,  3,  0),
    ("Corner clamp top-left",           "wwwaaaxs",              5,  5,  0),

    # ── Game of Life patterns ────────────────────────────────────────────────
    # Still life: 2x2 block — survives forever
    ("Block still life 3 iter",         "dxsdxwdxsaxd",         6,  6,  3),

    # Blinker oscillates with period 2
    ("Blinker period check iter=2",     "sdxddssaaww",           5,  5,  2),
    ("Blinker period check iter=4",     "sdxddssaaww",           5,  5,  4),

    # Larger board, many iterations
    ("Large board many iters",          "dxss",                 10, 10, 10),

    # ── Argument edge cases (we test these separately) ─────────────────────
    # (handled in run_edge_arg_tests below)
]


# ─── Runner ─────────────────────────────────────────────────────────────────

def run_binary(binary: str, commands: str, width: int, height: int, iterations: int,
               timeout: float = 5.0):
    """Run the binary and return (stdout, returncode, error_msg)."""
    try:
        result = subprocess.run(
            [binary, str(width), str(height), str(iterations)],
            input=commands,
            capture_output=True,
            text=True,
            timeout=timeout
        )
        return result.stdout, result.returncode, None
    except subprocess.TimeoutExpired:
        return None, None, "TIMEOUT"
    except FileNotFoundError:
        return None, None, f"Binary not found: {binary}"


def run_edge_arg_tests(binary: str) -> list:
    """Test invalid argument handling — should exit non-zero without crashing."""
    results = []
    edge_cases = [
        ("Too few args",      [binary]),
        ("Too many args",     [binary, "5", "5", "5", "extra"]),
        ("Zero width",        [binary, "0", "5", "5"]),
        ("Zero height",       [binary, "5", "0", "5"]),
        ("Negative iter",     [binary, "5", "5", "-1"]),
        ("Negative width",    [binary, "-1", "5", "5"]),
    ]
    for name, cmd in edge_cases:
        try:
            r = subprocess.run(cmd, input="", capture_output=True, text=True, timeout=3)
            passed = r.returncode != 0
            results.append((name, passed, f"exit={r.returncode}", "exit≠0"))
        except subprocess.TimeoutExpired:
            results.append((name, False, "TIMEOUT", "exit≠0"))
    return results


# ─── Pretty print ────────────────────────────────────────────────────────────

def show_diff(expected: str, got: str, width: int, height: int):
    exp_lines = expected.splitlines()
    got_lines = got.splitlines() if got else []
    # pad to same length
    while len(got_lines) < len(exp_lines):
        got_lines.append('')
    print(f"  {'EXPECTED':^{width+4}}    {'GOT':^{width+4}}")
    for e, g in zip(exp_lines, got_lines):
        marker = "  " if e == g else "!!"
        print(f"  |{e:<{width}}|  {marker}  |{g:<{width}}|")


def print_header(text: str):
    print(f"\n{BOLD}{CYAN}{'═'*60}{RESET}")
    print(f"{BOLD}{CYAN}  {text}{RESET}")
    print(f"{BOLD}{CYAN}{'═'*60}{RESET}")


# ─── Main ────────────────────────────────────────────────────────────────────

def main():
    if len(sys.argv) < 2:
        print(f"Usage: {sys.argv[0]} <binary>")
        sys.exit(1)

    binary = sys.argv[1]
    if not os.path.isfile(binary):
        print(f"{RED}Binary not found: {binary}{RESET}")
        sys.exit(1)
    if not os.access(binary, os.X_OK):
        print(f"{RED}Binary not executable: {binary}{RESET}")
        sys.exit(1)

    print_header(f"Testing: {binary}")

    passed = 0
    failed = 0
    failures = []

    # ── Functional tests ──────────────────────────────────────────────────
    print(f"\n{BOLD}[ Functional Tests ]{RESET}")
    for name, commands, w, h, iters in TESTS:
        expected = simulate(commands, w, h, iters)
        got, rc, err = run_binary(binary, commands, w, h, iters)

        if err:
            status = f"{RED}ERROR: {err}{RESET}"
            failed += 1
            failures.append((name, expected, None, err))
        elif got == expected:
            status = f"{GREEN}PASS{RESET}"
            passed += 1
        else:
            status = f"{RED}FAIL{RESET}"
            failed += 1
            failures.append((name, expected, got, None))

        print(f"  {status}  {name}  [{w}x{h} iter={iters}]")

    # ── Argument tests ────────────────────────────────────────────────────
    print(f"\n{BOLD}[ Argument / Edge Tests ]{RESET}")
    arg_results = run_edge_arg_tests(binary)
    for name, ok, got_val, expected_val in arg_results:
        if ok:
            print(f"  {GREEN}PASS{RESET}  {name}")
            passed += 1
        else:
            print(f"  {RED}FAIL{RESET}  {name}  (got {got_val}, want {expected_val})")
            failed += 1

    # ── Summary ───────────────────────────────────────────────────────────
    total = passed + failed
    print_header("Results")
    color = GREEN if failed == 0 else RED
    print(f"  {color}{BOLD}{passed}/{total} passed{RESET}")

    if failures:
        print(f"\n{BOLD}[ Failure Details ]{RESET}")
        for name, expected, got, err in failures:
            print(f"\n  {RED}✗ {name}{RESET}")
            if err:
                print(f"    Error: {err}")
            else:
                # find width from expected
                lines = expected.splitlines()
                w = max((len(l) for l in lines), default=10)
                show_diff(expected, got, w, len(lines))

    sys.exit(0 if failed == 0 else 1)


if __name__ == "__main__":
    main()