---
day: 4
basename: day04
---
# Day {{ page.day }}

**Puzzle:** [Giant Squid](https://adventofcode.com/2021/day/{{ page.day }})

**Strategy:**

# TBD


# Code solutions

**IntCode:** [{{ page.basename }}.txt](2021/{{ page.basename }}.txt)
{% capture RAW_INTCODE %}{% include_relative 2021/{{ page.basename }}.txt %}{% endcapture %}

```
{{RAW_INTCODE}}
```

**C lite:** [{{ page.basename }}.c](2021/{{ page.basename }}.c)
{% capture RAW_CLITE %}{% include_relative 2021/{{ page.basename }}.c %}{% endcapture %}

```c
{{RAW_CLITE}}
```

