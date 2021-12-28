---
day: 4
prev: day03
this: day04
next: day05
title: Day 4
puzzle: Giant Squid
---
# {{ page.title }}

## [{{ page.puzzle }}](https://adventofcode.com/2021/day/{{ page.day }})

`I2C2` was silent for a while, then:

*Can you repeat that?*

"Maybe it wants to play bingo?"

*You think the giant squid wants to play bingo?!*

"It must have noticed the bingo subsystem. What other reason could it have to attach itself to the hull?"

*Maybe it wants to eat you?*

You ignore `I2C2`'s comment and boot up the vessel's bingo subsystem. 100 boards appear on the screen.

"I will have you know I'm an expert bingo player", you say with confidence as you connect the external monitor so that the squid can partake in the game.

*How... How can you be an expert in a game of pure chance?*

"Haha, you'll see soon enough. Knowing the drawing sequence in advance has it advantages. I hope that squid is not a sore loser!"

*Because it might eat you?*

You pause. The bingo game has just finished loading on the screens. The squid reaches for the external monitor.

"You know what. Change of strategy! Let the squid win..."


## Solution


**IntCode:** [{{ page.this }}.txt](2021/{{ page.this }}.txt) &#124; **C lite:** [{{ page.this }}.c](2021/{{ page.this }}.c)
{% capture RAW_INTCODE %}{% include_relative 2021/{{ page.this }}.txt %}{% endcapture %}

```
{{RAW_INTCODE}}
```

&nbsp;

# Will it assemble?

*Continuation from Day 3*. Yesterday I said that we had already (more-or-less) written the assembly-code for solving part 1 of Day 1's puzzle. By "assembly-code" I here take as definition that every statement in the code occupies a single line and maps to a *single* IntCode-instruction.

Here's what we have so far, transcribed from the flowcharts:

```
p = &depth
*p = input      <-------,
p = p + 1               |
r = p < &depth + 2000   |
if r jump  -------------'
c = 0
q = &depth
s = q + 1       <-------,
r = *q < *s             |
c = c + r               |
q = q + 1               |
r = s < &depth + 1999   |
if r jump  -------------'
print c
halt
```

## Variables and variable-locations
The code has a few variables: `p`, `q`, `r`, `s` and `c`. For each of those variables we choose a memory location that doesn't overlap with the code. Let's assume we can fit all code within the first 95 integers, then we can choose locations `95` - `99` for those five variables, respectively.

To refer to the memory location of a variable I use the ampersand `&`. (As briefly mentioned yesterday, this is notation borrowed from C-language). So here `&p`, `&q`, `&r`, `&s` and `&c` would simply be substituted for `95`, `96`, `97`, `98` and `99` when the code is assembled to IntCode.

What about the `depth`-buffer? We use the buffer `depth` and have to choose a position (or range) in the memory it should occupy. Let's choose all positions from `100` up to (not including) `2100`. This does not overlap with the code (we assume) or the previous variable locations, and a length of 2000 positions was chosen because that's how many integers we needed for the puzzle-input. Good, we can now substitute `&depth` and `&depth + 2000` with `100` and `2100`, respectively, when assembling. That's all we need to do with regards to `depth` in this code. It never occurs without the `&`. It is never used as a variable in itself.

## Pointers and dereferencing
Dereferencing is indicated by an asterisk `*` (C-notation, again). To dereference means to take the value at which a variable is *pointing*. Variables that are used to point at stuff are called *pointers* (duh!). The dereferencing operator `*` is more-or-less the opposite of the location-of operator `&`.

**Example:** In our code above `p` was designated the memory location `95`. If the value of `p` (the integer at position `95`) would have value `234`, then `*p` means "the value at position `234`". If we would increment `p` by 1 `*p` would then become "the value at position `235`. (The value of `&p`, on the other hand, always remains `95`).

## How to generate the IntCode for each statement

Here are examples of how to implement various assembly-statements with IntCode:

| Type        | Example       | IntCode | Comment |
| ----------- | ------------- | ------------------- | ------- |
| Assignment  | `p = 541`     | `01101, 541, 0, &p` | Use addition-operation to store result of 541 + 0 to location of `p`.|
|             | `p = q`       | `01001, &q, 0, &p`  | Same, but use location of `q` in *position*-mode.|
| Addition    | `p = 541 + 5` | `01101, 541, 5, &p` | Same as assignment but with a non-zero second parameter.|
|             | `p = q + 5`   | `01001, &q, 5, &p`  | |
|             | `p = q + r`   | `00001, &q, &r, &p` | |
| Subtraction | `p = 541 - 5` | `01101, 541, -5, &p`| Addition, but with a negative constant.|
|             | `p = q - 5`   | `01001, &q, -5, &p` | |
| Input       | `p = input`   | `00003, &p`         | Scan an integer into `p`. |
| Print       | `print 35`    | `00104, 35`         | Print the number 46 (ASCII for `#`). |
|             | `print p`     | `00004, &p`         | Print value of `p`. |
| Conditional jump | `if r jump 10`| `1005, &r, 10` | `r` is most commonly the result of evaluating a condition. |
| Unconditional jump | `if 1 jump 10`| `1105, 1, 10` | |

The other *binary* operations of IntCode (`*`, `<`, `==`) are similar to Addition in the table above. For now we take care to avoid missing operators ("subtraction by non-constant", `!=`, `>`, `<=`, `>=`, ... ).

## Jump-locations
Normally (sadly?) I don't code jump-locations with ASCII-art. We should change the arrows to the numerical value of the jump location. Do we know what they will be? Yes, we do. The premise of this exercise was that we had used code in which every statement can be codified to exactly one IntCode-instruction. The IntCode-instructions may be of length 1 - 4 integers, but we know which for each instruction.

**First pass:** Lay out the IntCode-instructions from beginning to end. Put place-holders for the jump-instructions' targets, while recording where all locations of the jump-targets end up.

**Second pass:** Replace the place-holders by the recorded target-locations.

## Hurdles

So we have kind-of invented a limited assembly-language. A few things to note:

### There is no instruction for subtracting by a non-constant

This is annoying, but we can work around it by squeezing in an extra multiply-by-minus-one instruction somewhere when needed.

### No operation can dereference a pointer (i.e., take the value of `*p`)

Ouch! The `input`-operation cannot target `*p`. The comparison `r = *q < *s` is not supported. This *is* a problem. (Why did we even go through all the hassle of introducing dereferencing to begin with?!)

### We are not making use of the *relative* parameter mode.

We are not setting the *relative base* (`RB`), nor using it ("relative parameter mode"). *Maybe* it could be used to dereference pointers? Probably... (maybe), but let's stay away from it - for now.

## Time to cheat!
The pointer dereference occurs once for `p`, `q` and `s` each in the statments "`*p = input`" and "`r = *q < *s`". The IntCode we *would* like to produce for this statement is something like:

| Statement     | IntCode           |
| ------------- | ----------------- |
| `*p = input`  | `003, ?`          |
| `r = *q < *s` | `00007, ?, ?, &r` |

Because:

- It's clear that the op-codes need to be `03` (for `input`) and `07` (for `<`).
- Parameter-mode can only be `0` (for "position mode"). The "immediate parameter mode" would be invalid (for `*p = input`) or just not be of any help (for `r = *q < *s`).

Now, is there any way we can fill in the `?` so that the `input`-statement stores its value at where `p` is pointing? And likewise for the comparison, so that it is made for the values at which `q` and `s` are pointing?

...*and yes, can you believe it, there is one way!* We store `p`, `q` and `s` inside the IntCode-instructions, at the locations of the `?`s. Up to now I have said that all variables, `p`, `q` and `s` included, must be located *after* the code section. Previously `p`, `q` and `s` were located at positions `95` - `97`, but now we put them *inside* the code section at said comparison-statement's first and second parameter:

| Statement     | IntCode           |
| ------------- | ----------------- |
| `*p = input`  | `003, (p)`          |
| `r = *q < *s` | `00007, (q), (s), &r` |

This works for this particular statement, but does it interfere with anything else? Not really. The other statements do not care *where* `p`, `q` and `s` are stored. It's probably not a good practice to mix data and code section in a program, but this time we got away with it because the variables in question were only dereferenced one time each. Had we needed to dereference any of them at another place this would not have worked.

What about the numerical value of the positions? `&p`, `&q` and `&s`? No sweat! Those we get the same way as the jump-locations, by making 2 passes.

## The IntCode (at last!)

### First pass

```
Position    Statement               IntCode
---------------------------------------------------------------
    00      p = &depth              1101,  &depth,  0,  &p,
    04      *p = input              3 (<A>),  0 (p),
    06      p = p + 1               1001,  &p,  1,  &p,
    10      r = p < &depth + 2000   1007,  &p,  &depth + 2000,  &r,
    14      if r jump <A>           1005,  &r,  <A>,
    17      c = 0                   1101,  0,  0,  &c,
    21      q = &depth              1101,  &depth,  0,  &q,
    25      s = q + 1               1001 (<B>),  &q,  1,  &s,
    29      r = *q < *s             7,  0 (q),  0 (s),  &r,
    33      c = c + r               1,  &c,  &r,  &c,
    37      q = q + 1               1001,  &q,  1,  &q,
    41      r = s < &depth + 1999   1007,  &s,  &depth + 1999,  &r,
    45      if r jump <B>           1005,  &r, <B>,
    48      print c                 4,  &c,
    50      halt                    99


Position    Variable- or jump-location
-------------------------------------
    04      <A>
    05      &p
    25      <B>
    30      &q
    31      &s
    98      &r
    99      &c
   100      &depth
```

### Second pass
```
Position    Statement           IntCode
-------------------------------------------------------
    00      p = 100             1101,  100,    0,    5,
    04      *p = input             3,    0,
    06      p = p + 1           1001,    5,    1,    5,
    10      r = p < 2100        1007,    5, 2100,   98,
    14      if r jump 04        1005,   98,    4,
    17      c = 0               1101,    0,    0,   99,
    21      q = 100             1101,  100,    0,   30,
    25      s = q + 1           1001,   30,    1,   31,
    29      r = *q < *s            7,    0,    0,   98,
    33      c = c + r              1,   99,   98,   99,
    37      q = q + 1           1001,   30,    1,   30,
    41      r = s < 2099        1007,   31, 2099,   98,
    45      if r jump 25        1005,   98,   25,
    48      print c                4,   99,
    50      halt                  99
```

### Final output
```
1101,100,0,5,3,0,1001,5,1,5,1007,5,2100,98,1005,98,4,1101,0,0,99,1101,100,0,30,1001,30,1,31,7,0,0,98,1,99,98,99,1001,30,1,30,1007,31,2099,98,1005,98,25,4,99,99
```
Please try this IntCode-program on your input from the [2021 Day 1 puzzle](https://adventofcode.com/2021/day/1). It should solve part 1 of the puzzle.

## Running the IntCode
As discussed on day 2, you need an IntCode VM. If you have our own, great! Let's put it to some use :). If not, don't worry, I have you covered. The [day 2-page](day02.md#the-intcode-vm) has a C++ implementation with instructions on how to build and use.

**Note 1:** The IntCode-program we constructed here reads and writes direct integers ("decimal", non-ASCII mode). This is different from the other IntCode puzzle solutions I have uploaded here. They read and write ASCII-encoded strings. While this makes the programs more flexible and capable with respect to puzzle-input format, and more, the code to process the ASCII-strings would be many times the current program in size. Therefore, when running the program through the IntCode VM, make sure it does *NOT* operate in ASCII-mode.

With the [provided IntCode VM](day02.md#the-intcode-vm) this is achieved by supplying the `-d` flag when running:

```
intcode_vm -d day01_manually.txt < input01.txt
```

...or by appending a `D` to the IntCode-program itself:
```
1101,100,0,...<same as above>...,25,4,99,99,D
```

**Note 2:** Remember that we have hardcoded the input size from the sonar sweep to 2000 values. The program will not be accurate for other sizes.