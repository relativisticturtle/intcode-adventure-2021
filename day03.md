---
day: 3
prev: day02
this: day03
next: day04
title: Day 3
puzzle: Binary Diagnostic
---
# {{ page.title }}

## Puzzle - [{{ page.puzzle }}](https://adventofcode.com/2021/day/{{ page.day }})

You stare at the diagnostic report for a while. Everyone knows Elves are expert engineers, but they sometimes overlook certain things... like life-support (or readability of diagnostic reports). Can you trust them to maintain air with sufficient oxygen level and non-toxic levels carbon dioxide? The question is rhetorical. You already know. You have work to do.

## Solution

**Part 1:** Just loop through the bit positions and set `gamma`'s bits to most common ("`= counter[l] > 500`"). Same - but opposite - for `epsilon`. (Could have used the complementary of `gamma`, but nevermind...)

**Part 2:** Same, but now have to use a mask for the "oxygen generator" and "CO2 scrubber" -rating to bookkeep which lines to consider for every bit-position.

**IntCode:** [{{ page.this }}.txt](2021/{{ page.this }}.txt) &#124; **C lite:** [{{ page.this }}.c](2021/{{ page.this }}.c)
{% capture RAW_INTCODE %}{% include_relative 2021/{{ page.this }}.txt %}{% endcapture %}

```
{{RAW_INTCODE}}
```

&nbsp;

# Will it assemble?

*Continuation from Day 2*. Yesterday I said that we had already written the assembly-code for solving part 1 of Day 1's puzzle. By "assembly-code" I here take as definition that every statement in the code occupies a single line and maps to a *single* IntCode-instruction.

I spoke *almost* truthfully when I said that it was already complete assembly-code. Here's what we have so far, transcribed from the flowcharts:

```
p = &depth
*p = input      <-------,
p = p + 1               |
r = p < &depth + 2000   |
if r jump  -------------'
c = 0
p = &depth + 1
q = p - 1       <-------,
r = *p < *q             |
c = c + r               |
p = p + 1               |
r = p < &depth + 2000   |
if r jump  -------------'
print c
```

## Variables and variable-locations
The code has a few variables: `p`, `q`, `r` and `c`. For each of those variables we choose a memory location that doesn't overlap with the code. Let's assume we can fit all code within the first 95 integers, then we can choose locations `96` - `99` for those four variables, respectively.

To refer to the memory location of a variable I use the ampersand `&`. (As briefly mentioned yesterday, this is notation borrowed from C-language). So here `&p`, `&q`, `&r` and `&c` would simply be substituted for `96`, `97`, `98` and `99` when the code is assembled to IntCode.

What about the `depth`-buffer? We use the buffer `depth` and have to choose a position (or range) in the memory it should occupy. Let's choose all positions from `100` up to (not including) `2100`. This does not overlap with the code (we assume) or the previous variable locations, and a length of 2000 positions was chosen because that's how many integers we needed for the puzzle-input. Good, we can now substitute `&depth` and `&depth + 2000` with `100` and `2100`, respectively, when assembling. That's all we need to do with regards to `depth` in this code. It never occurs without the `&`. It is never used as a variable in itself.

## Pointers and dereferencing
Dereferencing is indicated by an asterisk `*` (C-notation, again). To dereference means to take the value at which a variable is *pointing*. Variables that are used to point at stuff are called *pointers* (duh!). The dereferencing operator `*` is more-or-less the opposite of the location-of operator `&`.

**Example:** In our code above `p` was designated the memory location `96`. If the value of `p` (the integer at position `96`) would have value `234`, then `*p` means "the value at position `234`". If we would increment `p` by 1 `*p` would then become "the value at position `235`. (The value of `&p`, on the other hand, always remains `96`).

## How to generate the IntCode for each statement

Here are examples of how to implement various assembly-statements with IntCode:

| Type        | Example       | IntCode | Comment |
| ----------- | ------------- | ------------------- | ------- |
| Assignment  | `p = 541`     | `01101, 541, 0, &p` | Use addition-operation to store result of 541 + 0 to location of `p`.|
|             | `p = q`       | `01001, &q, 0, &p`  | Same, but use location of `x` in *position*-mode.|
| Addition    | `p = 541 + 5` | `01101, 541, 5, &p` | Same, but now with a non-zero second argument.|
|             | `p = q + 5`   | `01001, &q, 5, &p`  | |
|             | `p = q + r`   | `00001, &q, &r, &p` | |
| Subtraction | `p = 541 - 5` | `01101, 541, -5, &p`| Addition, but with a negative constant.|
|             | `p = q - 5`   | `01001, &q, -5, &p` | |
|             | `p = q - r`   | - | No IntCode single-instruction for subtraction by *non-constant*. |

The variable locations (`&p`, `&q`, `&r`) are substituted with the chosen memory locations (`96`, `97` and `98` in our example).

The other binary operations of IntCode (`*`, `<`, `==`) are similar to Addition in the table above. For now we take care to avoid missing operators ("subtraction by non-constant", `!=`, `>`, `<=`, `>=`, ... ).

## More examples

Our code also has Input, Print and conditional Jump instructions. They have straightforward IntCode counterparts:

| Type        | Example       | IntCode | Comment |
| ----------- | ------------- | ------------------- | ------- |
| Input       | `p = input`   | `00103, &p` | Get an integer from input and store at `p`'s location. |
|             | `*p = input`  | `00003, &p` | Same, but store at the location `p` is pointing to. |
| Print       | `print 35`    | `00104, 35` | Print the number 46 (ASCII for `#`). |
|             | `print p`     | `00004, &p` | Print value of `p`. |
| Conditional jump | `if r jump 10`| `1005, &r, 10` | `r` is most commonly the result of evaluating a condition. |
| Unconditional jump | `if 1 jump 10`| `1105, 1, 10` | |

## Jump-locations
Normally (sadly?) I don't code jump-locations with ASCII-art. We should change the arrows to the numerical value of the jump location. Do we know what they will be? Yes, we do. The premise of this exercise was that we had used code in which every statement can be codified to exactly one IntCode-instruction. The IntCode-instructions may be of length 1 - 4 integers, but we know which for each instruction.

**First pass:** Lay out the IntCode-instructions from beginning to end. Put place-holders for the jump-instructions' targets, while recording where all locations of the jump-targets end up.

**Second pass:** Replace the place-holders by the recorded target-locations.
