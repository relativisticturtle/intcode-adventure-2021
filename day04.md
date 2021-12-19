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

# IntCoding

Our IntCode-program from yesterday solved part 1 of Day 1's puzzle, but we should of course solve part 2 with it before we let it go. Once you figure out that taking the difference over a "rolling average of 3 elements" is equivalent to the difference between values *3 positions* apart in the sequence. That's why the high level description of part 1 and part 2 were so similar:

![High-level description of the algorithm solving Day 1's puzzle](/assets/flowchart_step123.png)

...and in the detailed break down of part 1 and 2 we only need to make teeny-tiny change to the flow chart to get our part 2 solver.

![Step 2 - "Count occurences of depth[i] < depth[i+1]"](/assets/flowchart_step2.png)
![Step 2 - "Count occurences of depth[i] < depth[i+3]"](/assets/flowchart_step3.png)

So modifying our existing code and have one IntCode-program for part 1 and another for part 2 is easy. They will only differ at a single point (*Which? Left as exercise to the reader!*).

## Single program part 1 and 2 solver
If you want instead to have the part 1 and part 2 solver in a single program, running the *entire* sequence of the high level description and printing both puzzle answers in a single go, there's one complication. Remember from yesterday: Our hack to perform pointer dereferencing *only* works if the pointer is dereferenced at most at a *single location*. Now check the flow charts above. Yes, we are using both `*q` and `*s` twice, once each in part 1, and once each in part 2. There's an easy fix for that though: More variables! Let's introduce `u` and `v` and use them instead of `q` and `s`, respectively, for part 2.

The code to add for part 2 (after first pass, and moving `halt` to the end) will then become:

```
Position    Statement               IntCode
---------------------------------------------------------------
    ...
    50      c = 0                   1101,  0,  0,  &c,
    54      u = &depth              1101,  &depth,  0,  &u,
    58      v = u + 3               1001 (<C>),  &u,  3,  &v,
    62      r = *u < *v             7,  0 (u),  0 (v),  &r,
    66      c = c + r               1,  &c,  &r,  &c,
    70      u = u + 1               1001,  &u,  1,  &u,
    74      r = v < &depth + 1999   1007,  &v,  &depth + 1999,  &r,
    78      if r jump <C>           1005,  &r, <C>,
    81      print c                 4,  &c,
    83      halt                    99


Position    Variable- or jump-location
-------------------------------------
    ...
    58      <C>
    63      &u
    64      &v
    ...
```

```
1101,100,0,5,3,0,1001,5,1,5,1007,5,2100,98,1005,98,4,1101,0,0,99,1101,100,0,30,1001,30,1,31,7,0,0,98,1,99,98,99,1001,30,1,30,1007,31,2099,98,1005,98,25,4,99,1101,0,0,99,1101,100,0,63,1001,63,3,64,7,0,0,98,1,99,98,99,1001,63,1,63,1007,64,2099,98,1005,98,58,4,99,99
```

## Golfing
Our current program is 84 integers long. Quite good! We guessed we could write the code with less than 95 integers yesterday and we succeeded with a small margin of 11 integers. (Had we exceeded our initial guess we would just have to reassign `r`, `c` and `depth[]` to higher positions).

Now, let's try to optimize for shorter code (golfing).

### Item 1 - Code reuse

We are basically solving part 1 and part 2 of the puzzle with the same algorithm. Can it be reused? We need the following to happen after part 1:

1. If second iteration, `halt`
2. Modify "`s = q + 1`" into "`s = q + 3`" (at position `<B> + 2`)
3. Jump to "`c = 0`" (at position `<B> - 8`)


### Item 2 - Skip unnecessary initialization

Since `p` is now living in the code section we can omit `p = &depth` and set it directly. While `c = 0` is superfluous in the *first* iteration (by specification all IntCode beyond the code section should be zero-initialized), we do need to reset it for the *second* iteration.

### Item 3 - Making use of accidental halt-instructions

Every reference to the variable `c` generates a `99` in the IntCode-program, and from **Item 1** above we know we would like to jump to some `halt` instruction - of number `99`. No need to put more of them in the code!

### The new part 2

This is the new part 2, piggybacking on part 1:

![Step 2 - "Count occurences of depth[i] < depth[i+3]"](/assets/flowchart_step3_golf.png)

(Excuse my notation for code position relative point `<B>` in the code).

Implementing this change as well as **Item 2** and **3** gives us:

```
Position    Statement               IntCode
---------------------------------------------------------------
    00      *p = input              3 (<A>),  100 (p),
    02      p = p + 1               1001,  &p,  1,  &p,
    06      r = p < &depth + 2000   1007,  &p,  &depth + 2000,  &r,
    10      if r jump <A>           1005,  &r,  <A>,
    13      c = 0                   1101,  0,  0,  &c,
    17      q = &depth              1101,  &depth,  0,  &q,
    21      s = q + 1               1001 (<B>),  &q,  1,  &s,
    25      r = *q < *s                7,  0 (q),  0 (s),  &r,
    29      c = c + r                  1,  &c,  &r,  &c,
    33      q = q + 1               1001,  &q,  1,  &q,
    37      r = s < &depth + 1999   1007,  &s,  &depth + 1999,  &r,
    41      if r jump <B>           1005,  &r, <B>,
    44      print c                     4, &c,
    46      r = 1 < (<B> + 2)        107,   1, <B> + 2, &r,
    50      if r jump <halt>        1005,  &r, <halt>,
    53      (<B> + 2) = 3           1101,   3,  0, <B> + 2,
    57      jump <B> - 8            1105,   1,  <B> - 8


Position    Variable- or jump-location
-------------------------------------
    00      <A>
    01      &p
    21      <B>
    26      &q
    27      &s
    98      &r
    99      &c
   100      &depth
```

A second pass with all substitutions yields:

```
3,100,1001,1,1,1,1007,1,2100,98,1005,98,0,1101,0,0,99,1101,100,0,26,1001,26,1,27,7,0,0,98,1,99,98,99,1001,26,1,26,1007,27,2099,98,1005,98,21,4,99,107,1,23,98,1005,98,45,1101,3,0,23,1105,1,13
```

Now down to 60 integers. (Yeah!)

That concludes the handcoding of Day 1's puzzle. Great job!