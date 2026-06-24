# Exam Rank 05 — 42 Network
Solutions for the 1337 Coding School Exam Rank 05, covering C++ OOP exercises (level 1) and C algorithm problems (level 2).

## My Result
Passed on first attempt with **bigint** (level 1) and **bsq** (level 2).

---

## Subjects

### Level 1 — C++

| Subject | Description | Tester |
|---------|-------------|--------|
| `vect2` | 2D vector class — operator overloading, dot/cross products | [test.py](vect2/test.py) |
| `bigint` | Arbitrary-precision unsigned integer, full arithmetic operators | [test.py](bigint/test.py) |
| `polyset` | Bag and set containers without STL — union, intersection, search | [test.py](polyset/test.py) |

All level 1 subjects require **Orthodox Canonical Form**: default constructor, copy constructor, copy assignment operator, destructor.

### Level 2 — C

| Subject | Description | Tester |
|---------|-------------|--------|
| `bsq` | Largest square of empty cells in a grid — O(n·m) DP | [test.py](bsq/test.py) |
| `life` | Conway's Game of Life — n steps simulation with wrapping | [test.py](life/test.py) |

---

## Key Concepts

**C++ (level 1)**
- Orthodox Canonical Form
- Operator overloading (`+`, `-`, `*`, `<<`, `>>`, comparison, increment)
- Abstract classes, inheritance, polymorphism
- No STL — containers implemented from scratch

**C (level 2)**
- Dynamic programming (BSQ uses the classic 2D DP recurrence)
- Input parsing and map validation
- Cellular automata rules (Life)
- Manual memory management

---

## Disclaimer
Attempt each subject yourself before reading any solution. The exam gives you the same subject repeatedly until you pass or time runs out — reading a solution without understanding it will not help you.
