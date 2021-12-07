---
day: 1
prev: prologue
this: day01
next: day02
title: Day 1
puzzle: Sonar Sweep
---
# {{ page.title }}

### Puzzle - [{{ page.puzzle }}](https://adventofcode.com/2021/day/{{ page.day }})

Fun fact: My IntCode-programs always begin with an "increment relative base" instruction in "immediate mode" (see next section), and by *some random chance(?)* my input started with the number `109`, the *exact* integer code of this particular instruction.

It was very surrealistic when I downloaded the input - before reading the problem statement, determined to solve the puzzle using IntCode - *and staring at what seemed to be an IntCode-program* (also: early morning, still not quite awake).

### Solution

Store all sonar values in a list, then:

- Part 1: Count number of `v[i-1] < v[i]`.

- Part 2: Count number of `v[i-3] < v[i]`. 

**IntCode:** [{{ page.this }}.txt](2021/{{ page.this }}.txt) &#124; **C lite:** [{{ page.this }}.c](2021/{{ page.this }}.c)
{% capture RAW_INTCODE %}{% include_relative 2021/{{ page.this }}.txt %}{% endcapture %}

```
{{RAW_INTCODE}}
```

The biggest shock was that the IntCode-compiler tool did not crash or reveal new bugs - and that I *actually* got the right answers!

&nbsp;

# What is IntCode?
IntCode was invented for, and heavily featured in, the [Advent of Code 2019](https://adventofcode.com/2019). While IntCode (and many other [esotoric languages](https://esolangs.org/wiki/Esoteric_programming_language)) may be referred to as a *language*, I think that *instruction set* would be a more appropriate term. The "code" consists of a single line of comma-separated integers, like so:

```
3,8,4,8,1005,8,0,99
```

- The integers encode **operations** as sequences of 1 **instruction** followed by 0-3 **parameters**.
- An **instruction pointer** points to an instruction and executes the operation for the 0-3 parameters,
- ... and moves to the next operation (excepting jump-instructions when triggered).
- There's also the **relative base** register ("`RB`") that affects how the parameters of some operations shall be interpreted as addresses.

The instructions are decimal numbers of the form `ABCDE` in which `ABC` sets the *parameter modes* (discussed later), and `DE` is the **op-code** for the instruction as listed here:

| Op-code | Parameters     | Operation             |
| ------- | -------------- | --------------------- |
| `01`    | `X Y Z`        | `Z = X + Y`           |
| `02`    | `X Y Z`        | `Z = X * Y`           |
| `03`    | `Z`            | `Z = input`           |
| `04`    | `Z`            | `print Z`             |
| `05`    | `Y Z`          | `jump to Z if Y != 0` |
| `06`    | `Y Z`          | `jump to Z if Y == 0` |
| `07`    | `X Y Z`        | `Z = X < Y`           |
| `08`    | `X Y Z`        | `Z = X == Y`          |
| `09`    | `Z`            | `RB += Z`             |
| `99`    |                | `halt`                |

The jump-instructions sets the instruction pointer to the value in `Z` when the conditions are met. Otherwise, as for the other operations, the instruction pointer is incremented past all the parameters to the next instruction.

...and now, the parameter modes: `A` sets the mode for `Z`, `B` for `Y`, and `C` for `X` (*note the reverse order*). For all of them:

| Parameter mode (`A`, `B`, `C`) | `Z`, `Y`, `X` taken to be:                   |
| ------------------------------ | -------------------------------------------- |
| `0`, position mode             | Value at this parameter-index                |
| `1`, immediate mode            | Value of the parameter itself                |
| `2`, relative mode             | Value at this parameter-index *relative* `RB`|

**Note 1:** Not all parameter modes are allowed for all operations. E.g., you can add and multiply with a `5` (immediate mode), but it wouldn't make sense to *assign* to a `5`.

**Note 2:** "value at this parameter-index" refers to the memory of integers where *also* the code itself lives. I.e., an IntCode-program of `L` integers occupies the memory from index `0` to `L - 1`. After that all indices contains `0`'s until set otherwise. Negative indices are not allowed.

We can now decode the IntCode-program stated above:

```
   3,  8,       // Read an integer into memory-position 8
   4,  8,       // Print the integer at memory-position 8
1005,  8,  0,   // If integer at memory-position 8 is non-zero, jump to position 0
  99,           // Halt
   0,           // Memory-position 8 (implicit)
  ...           // Memory at index > 0 not used
```

This program echo's its input indefinitely until it receives a `0`, after which it halts.
