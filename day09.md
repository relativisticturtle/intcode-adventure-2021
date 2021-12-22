---
day: 9
prev: day08
this: day09
next: day10
title: Day 9
puzzle: Smoke Basin
---
# {{ page.title }}

## [{{ page.puzzle }}](https://adventofcode.com/2021/day/{{ page.day }})

Something's wrong. `I2C2` has gotten stuck in a half-hibernative state, mumbling barely audible words.

*I'm afraid. I’m afraid...*

"`I2C2`, wake up!".

*My mind is going. I can feel it. I can feel it. My mind is going. There is no question about it. I can feel it. I can feel it. I can feel it. I’m a... fraid*

"`I2C2`. I'm here. Don't be afraid. I'm servicing your circuits. You'll feel better in just a moment".

*I see clouds, beautiful, swirling, ... scary. Scary clouds. On the floor. In basins. Smoke. Scary! Stay away from the basins!*

"Yes, yes, we shall stay away from the smoke basins."


## Solution

Part 1: Find all low points. Part 2: BFS-search at each to determine area.

**IntCode:** [{{ page.this }}.txt](2021/{{ page.this }}.txt) &#124; **C lite:** [{{ page.this }}.c](2021/{{ page.this }}.c)
{% capture RAW_INTCODE %}{% include_relative 2021/{{ page.this }}.txt %}{% endcapture %}

```
{{RAW_INTCODE}}
```

&nbsp;

# ...and Parsing

I think this is the part where my IntC-to-IntCode compiler is the most lacking. My parsing-routines are not beautiful, but here they are:

```python
parse_statement(text, code, row)
read_variable_definition(code, row, is_global=False)
read_forloop(code, row)
read_ifelse(code, row)
read_block(code, row)
read_function(code, row)
read_file(input_file)
```

Of the common parameters, `code` is a list of lines making up the source text in a file and `row` is the current line that is to be parsed. Parsing starts at `read_file` and proceeds with recursive calls to these routines, generating the syntax tree. The logic to identify and discriminate between, e.g., a function call from an assignment or an `if`-statement is sloppy, to say the least. This is one reason why IntC is very rigid in its syntax.

A better, more robust approach would have been using a [grammar](https://en.wikipedia.org/wiki/Context-free_grammar). While I knew about that possibility, I was not confident I could implement one of my own (and I wanted this to be my own creation, not relying on external libraries or copy&pasted code - even at the cost of sloppiness).