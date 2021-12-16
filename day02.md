---
day: 2
prev: day01
this: day02
next: day03
title: Day 2
puzzle: Dive!
---
# {{ page.title }}

## [{{ page.puzzle }}](https://adventofcode.com/2021/day/{{ page.day }})

You show `I2C2` the planned course.

"It doesn't make sense!"

*What do you mean?*

"We are in the ocean. Why all the maneuvering? What obstacles are we trying to avoid? Why not go the straight path?"

*Or we can follow the planned course...*

"No! That's to inefficient. Calculate the end position so that we can go there directly"

(Again, just because you are talking with an inaminate object does *not* mean you are crazy).


## Solution

Today's input was marginally harder to process, but not much. Rather than considering the whole word ("forward", "up", "down") it suffices to consider the first character ("f", "u", "d") and then discard up-to-and-including the next whitespace character.

**IntCode:** [{{ page.this }}.txt](2021/{{ page.this }}.txt) &#124; **C lite:** [{{ page.this }}.c](2021/{{ page.this }}.c)
{% capture RAW_INTCODE %}{% include_relative 2021/{{ page.this }}.txt %}{% endcapture %}

```
{{RAW_INTCODE}}
```

&nbsp;

# Handcoding an IntCode-program

Coding IntCode by hand *can* be done. Let's revisit the Day 1 puzzle and code an IntCode-program that solves the problem. I found that flowcharts were of great help when designing the algorithm, so let's do that! First, break down the program on a high level to three steps:

![High-level description of the algorithm solving Day 1's puzzle](/docs/assets/day2_fig1_v2.png)

- In the first step all depth values are scanned into a buffer, here called `depth`.
- In the second step the buffer is iterated through, counting all occurences of *increasing* depths.
- The third step does the same as the second, except that it checks for an increase against the depth value 3 steps prior.

Going into more details on the first step, it is now further broken down into low level instructions:

![Step 1 - "Scan input -> depth[]"](/docs/assets/day2_fig2_v2.png)

**Comments**
- The integer `p` is used as a pointer into the `depth`-buffer.
- "`&depth`" here means the start address of this buffer. (Only "`depth`" would have been more accurate - using C-notation, but I wanted to emphasize that it is an address. Or "`&depth[0]`" would have worked.)
- The input is assumed to be 2000 in length. That's why it is checked whether the pointer is within this offset at every iteration.

For the second step the process is broken down in a similar fashion:

![Step 2 - "Count occurences of depth[i-1] < depth[i]"](/docs/assets/day2_fig3.png)

**Comments**
- `c` is the variable counting occurrences
- `q` is lagging `p` by one step.
- Borrowing again from C-notation, the asterisk (`*`) means to dereference `p` and `q`, respectively. I.e., take the values in the `depth`-buffer at which each is pointing to.
- We compare `*p` and `*q` but we don't have to implement any if-block for the comparison. The comparison itself is an operator that stores `0` or `1` into `r`, which we can simply add to the counter `c`.

...and just like that we have written the *assembly code* solving part 1 of the puzzle! Not convinced? You think assembly code should have [cryptic mnemonics](https://en.wikipedia.org/wiki/Assembly_language#Mnemonics) and instructions such as "`MOV EAX, 2Ah`" and "`HCF`". (That's how I usually think of assembly code anyway).

The important thing here is that every statement for step 1 and 2 in the figures above can be replaced with a single IntCode-instruction. How? *Stay tuned for the continuation on Day 3!*