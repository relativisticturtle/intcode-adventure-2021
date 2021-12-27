---
day: 10
prev: day09
this: day10
next: day11
title: Day 10
puzzle: Syntax Scoring
---
# {{ page.title }}

## [{{ page.puzzle }}](https://adventofcode.com/2021/day/{{ page.day }})

*Syntax error. Syntax error. Syntax error...*

`I2C2` has been repeating the same two words over and over for an hour now. You think he might suffer from a syntax error.


## Solution

One of these problem where there is a really elegant and consise solution if you know the right data structure. Today that structure was a Stack.

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