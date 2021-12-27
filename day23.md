---
day: 23
prev: day22
this: day23
next: day24
title: Day 23
puzzle: Amphipod
---
# {{ page.title }}

## [{{ page.puzzle }}](https://adventofcode.com/2021/day/{{ page.day }})

The amphipods give you their sincere thanks after receiving the plan for how they should organize themselves with minimum cost. You watch as the move according to the carefully orchestrated plan. When they are nearly finished you realize that there was actually an *even more* optimal solution.

"Stop! Everyone go back. Do you remember where you started? Let's do this again from the beginning!"

## Solution

Today I truly learned A-star :).

**IntCode:** [{{ page.this }}.txt](2021/{{ page.this }}.txt) &#124; **C lite:** [{{ page.this }}.c](2021/{{ page.this }}.c)
{% capture RAW_INTCODE %}{% include_relative 2021/{{ page.this }}.txt %}{% endcapture %}

```
{{RAW_INTCODE}}
```

&nbsp;

# Yo Dawg, I heard you like IntCode...

*...so I put IntCode in your IntCode VM so you can run IntCode while you run IntCode!*

It had to be done. Here's an IntCode virtual machine implemented in IntCode:

{% capture RAW_INTCODE %}{% include_relative intcode_vm.txt %}{% endcapture %}

```
{{RAW_INTCODE}}
```
Download: [intcode_vm.txt](intcode_vm.txt)

IntC-code: [intcode_vm.c](https://github.com/relativisticturtle/intcode-adventure-2021/blob/main/intcode_vm.c) from which the IntCode was generated.

## Usage

To run an IntCode-program `hello.txt` using this (IntCode-) IntCode VM, simply feed it through the standard input:

```
$ intcode_vm intcode_vm.txt < hello.txt
Hello Advent of Coders!
```

In the source file `hello.txt` the first line is the IntCode-program, and any following text (if any) is delegated to the program as *its* input. Check the [hello.txt](hello.txt) source for an example of providing input this way.

Otherwise one can concatenate program and input on the command-line, using `cat` (Bash) or `type` (Windows), for a more elegant separation of program and input in different files. Here is an example for solving the day 1 puzzle with the (IntCode-) IntCode VM:

```
cat day01.txt input01.txt | intcode_vm intcode_vm.txt           (Bash)
type day01.txt input01.txt 2>nul | intcode_vm intcode_vm.txt    (Windows)
```

...and if using my [IntCode VM-implementation](https://github.com/relativisticturtle/intcode-adventure-2021/blob/main/intcode_vm.cpp) one can even put `intcode_vm.txt` in the stack of concatenated inputs for an equivalent invocation:

```
cat intcode_vm.txt day01.txt input01.txt | intcode_vm           (Bash)
type intcode_vm.txt day01.txt input01.txt 2>nul | intcode_vm    (Windows)
```


## Non-ASCII ("decimal") mode
The (IntCode-) IntCode VM performs I/O in ASCI-mode per default, but has the option to operate in non-ASCII ("decimal") mode as well. To do that, append a `'D'` to the IntCode-program on the first line. See, e.g., my [golfed day 1 solution](2021/day01_golf.txt) for an example.

## Solving day 1 - again
For reference, consider again my [golfed day 1 solution](2021/day01_golf.txt):

```
> type 2021\day01_golf.txt 2021\input01.txt 2>nul | intcode_vm -v                
60 integers read
-------------------------
1448
1471
-------------------------
done! 31989 instructions executed.
```

- This was the correct answers for my puzzle-input.

Running the same program through a layer of (IntCode-) IntCode VM:

```
> type intcode_vm.txt 2021\day01_golf.txt 2021\input01.txt 2>nul | intcode_vm -v
8033 integers read
-------------------------
1448
1471
-------------------------
done! 266756368 instructions executed.
```

- Yes! Still the right answers!
- 269454756 instructions executed in approx 6 s.
- So, an average of 269454756 / 31989 = 8423.35 instructions in the IntCode VM to process 1 instruction in the IntCode program.


Once, more - right? *right?!* Let's run with one more layer of IntCode VMs:

```
> type intcode_vm.txt intcode_vm.txt 2021\day01_golf.txt 2021\input01.txt 2>nul | intcode_vm -v
8033 integers read
-------------------------
Illegal access: code[1008067+0]
```

Oops! Memory access error... Why? Ah, of course! To put it simply: We have attempted to put one of two *equally sized* boxes inside the other. This implementation of the IntCode VM statically allocates a buffer of 1M integers for the code, but it cannot put a program of its own size in there.

What to do? Give up? *No!* If we could only make `code[]`, the buffer holding the IntCode-program, start beyond all [code-sections, global & local memory-sections, and stack-sections](day11.md)... Hmm... The reason it is not already is because of the compiler design, but it can be circumvented. We simply hard-code the `code`-pointer to a memory address at 10000 (should be sufficient) without ever declaring it as an array of any sort. Modern compilers would probably complain. Mine? Not so much.

Second attempt, go!

```
> type intcode_vm.txt intcode_vm.txt 2021\day01_golf.txt 2021\input01.txt 2>nul | intcode_vm -v
8033 integers read
-------------------------
```

Waiting... No errors. No results either. Still waiting... Can I estimate how long this will take? Let's see. Probably same 8423.35-ratio of IntCode VM instructions to IntCode-program instructions. So 14 hours... Well, it's within 1 day at least ;).

*Update, ~12 hrs later:*

```
1448
1471
-------------------------
done! 2273420655766 instructions executed.
```

*Success! Still the correct answer!*

## Closing words
That was fun ;). At most I ran a stack of 3 IntCode-machines, 1 implemented in C++ and 2 implemented in IntCode, to run an IntCode-program solving the Day 1 puzzle.