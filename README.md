# Colossal Numbers Calculator

A stack-based calculator capable of performing arithmetic operations on **Colossal Numbers**. The project is implemented in C (C23 standard) and handles arbitrarily large numbers limited only by available heap memory.

## What are Colossal Numbers?
A "Colossal Number" is a recursive, non-positional representation of a non-negative integer introduced by Donald Knuth. 
The value of a colossal number is the sum of powers of two, where the exponents are themselves colossal numbers. 

For example:
* `0` is represented as `0`
* `1` is represented as `100` ($2^0$)
* `3` is represented as `1100100` ($2^1 + 2^0$)
* `1024` is represented as `111100100110000` ($2^{10}$)

The textual representation uses a parenthesis-like structure where `1` acts as an opening bracket and `0` as a closing bracket. A number is considered **normalized** when its digits (exponents) are also normalized and strictly sorted in descending order of value.

## Features
* **Arbitrary Precision:** Calculates values whose positional binary representations would have more digits than there are atoms in the universe (e.g., $2^{2^{2^{65536}}}$).
* **Stack-based Architecture:** Uses a Last-In-First-Out (LIFO) stack to evaluate reverse Polish notation-style commands.
* **Dynamic Memory Management:** Fully dynamic tree-like structures with deep normalization and strict memory leak prevention.

## Supported Commands
The calculator reads instructions from standard input (character by character):

| Command | Action |
| :--- | :--- |
| `1...0` | **Push:** Pushes the read colossal number onto the stack. |
| `.` | **Pop & Print:** Pops the top value from the stack, prints its normalized string representation, and frees it. |
| `:` | **Duplicate:** Duplicates the top element of the stack. |
| `^` | **Power of 2:** Pops the top value $x$, and pushes $2^x$. |
| `+` | **Add:** Pops two values, calculates their sum, and pushes the normalized result. |
| `*` | **Multiply:** Pops two values, calculates their product, and pushes the normalized result. |

## Build Instructions

The project strictly adheres to the C23 standard and should be compiled with GCC using rigorous warning and memory sanitization flags to ensure safety.

```bash
gcc -std=c23 -pedantic -Wall -Wextra -Wformat-security -Wduplicated-cond \
-Wfloat-equal -Wshadow -Wconversion -Wjump-misses-init -Wlogical-not-parentheses \
-Wnull-dereference -Wvla -Werror -fstack-protector-strong -fsanitize=undefined \
-fno-sanitize-recover -g -fno-omit-frame-pointer -O1 colossal.c -o colossal
