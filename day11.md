---
day: 11
prev: day10
this: day11
next: day12
title: Day 11
puzzle: Dumbo Octopus
---
# {{ page.title }}

## [{{ page.puzzle }}](https://adventofcode.com/2021/day/{{ page.day }})


## Solution


**IntCode:** [{{ page.this }}.txt](2021/{{ page.this }}.txt) &#124; **C lite:** [{{ page.this }}.c](2021/{{ page.this }}.c)
{% capture RAW_INTCODE %}{% include_relative 2021/{{ page.this }}.txt %}{% endcapture %}

```
{{RAW_INTCODE}}
```

&nbsp;

# Memory layout

I have touched upon the the memory layout of the IntCode-programs I build before. There is the code, global memory and local memory. Beyond that is what I call the stack memory. I don't know if it is the appropriate term, but anyway: it holds temporary calculations during expression evaluation.

![The memory layout of an IntCode-program](assets/memory_layout.png)

During the compilation process the *local variables* of a function are assigned a position relative the memory frame of a function call. During execution a memory frame is allocated every time a function-call is made by incrementing the `RB`-register a certain amount.

Each memory frame contains:
- The parameters with which the function was called.
- Local variables.
- The address at which the function was invoked, so that execution can return once the function finishes.