---
day: 2
prev: day01
this: day02
next: day03
title: Day 2
puzzle: Dive!
---
# {{ page.title }}

### [{{ page.puzzle }}](https://adventofcode.com/2021/day/{{ page.day }})

You show `I2C2` the planned course.

"It doesn't make sense!"

*What do you mean?*

"We are in the ocean. Why all the maneuvering? What obstacles are we trying to avoid? Why not go the straight path?"

*Or we can follow the planned course...*

"No! That's to inefficient. Calculate the end position so that we can go there directly"

(Again, you are *not* crazy talking with an inanimate object).

&nbsp;

### Solution

Today's input was marginally harder to process, but not much. Rather than considering the whole word ("forward", "up", "down") it suffices to consider the first character ("f", "u", "d") and then discard up-to-and-including the next whitespace character.

**IntCode:** [{{ page.this }}.txt](2021/{{ page.this }}.txt) &#124; **C lite:** [{{ page.this }}.c](2021/{{ page.this }}.c)
{% capture RAW_INTCODE %}{% include_relative 2021/{{ page.this }}.txt %}{% endcapture %}

```
{{RAW_INTCODE}}
```

&nbsp;

# Handcoding an IntCode-program

Coding IntCode by hand *can* be done. Let's revisit the Day 1 puzzle and design an IntCode-program that solves the problem. I design the algorithm as a program flow with 3 distinct steps: 

![High-level description of the algorithm solving Day 1's puzzle](/docs/assets/day2_fig1_v2.png)

In the first step all depth values are scanned into a buffer, here called `depth`. In the second step the buffer is iterated through, counting all occurences of *increasing* depths. The third step does the same as the second, except that it checks for an increase against the depth value 3 steps prior.

Beginning with the first step, it is now broken down further into low level instructions:

![Step 1 - "Scan input -> depth[]"](/docs/assets/day2_fig2_v2.png)

Same with the second step:

![Step 2 - "Count occurences of depth[i-1] < depth[i]"](/docs/assets/day2_fig3.png)

