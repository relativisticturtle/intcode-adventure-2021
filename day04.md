---
day: 4
prev: day03
this: day04
next: day05
title: Day 4
puzzle: Giant Squid
---
# {{ page.title }}

## Puzzle - [{{ page.puzzle }}](https://adventofcode.com/2021/day/{{ page.day }})

xyz...

## Solution

xyz...

**IntCode:** [{{ page.this }}.txt](2021/{{ page.this }}.txt) &#124; **C lite:** [{{ page.this }}.c](2021/{{ page.this }}.c)
{% capture RAW_INTCODE %}{% include_relative 2021/{{ page.this }}.txt %}{% endcapture %}

```
{{RAW_INTCODE}}
```

&nbsp;

# TBD