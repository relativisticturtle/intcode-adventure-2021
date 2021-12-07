---
day: 2
prev: day01
this: day02
next: day03
title: Day 2
puzzle: Dive!
---
# {{ page.title }}

### Puzzle - [{{ page.puzzle }}](https://adventofcode.com/2021/day/{{ page.day }})

xyz...

### Solution

xyz...

**IntCode:** [{{ page.this }}.txt](2021/{{ page.this }}.txt) &#124; **C lite:** [{{ page.this }}.c](2021/{{ page.this }}.c)
{% capture RAW_INTCODE %}{% include_relative 2021/{{ page.this }}.txt %}{% endcapture %}

```
{{RAW_INTCODE}}
```

&nbsp;

# TBD