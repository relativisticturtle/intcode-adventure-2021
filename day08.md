---
day: 8
prev: day07
this: day08
next: day09
title: Day 8
puzzle: Seven Segment Search
---
# {{ page.title }}

## [{{ page.puzzle }}](https://adventofcode.com/2021/day/{{ page.day }})

With a smug smile you ask `I2C2`,

"Who thought the Christmas lights, wouldn't be of help?"

`I2C2` doesn't say anything but makes irritated buzzing noise.

"...and what was it that attracted the swarm of crab-submarines to our rescue?"

*Yeah, yeah...*, `I2C2` mutters, *Can you fix the seven-segment display now?!*


## Solution

Segments were identified by counting in how many of the 10 digits they appear, and whether they appear in the "`4`". These 2 pieces of information were sufficient.

**IntCode:** [{{ page.this }}.txt](2021/{{ page.this }}.txt) &#124; **C lite:** [{{ page.this }}.c](2021/{{ page.this }}.c)
{% capture RAW_INTCODE %}{% include_relative 2021/{{ page.this }}.txt %}{% endcapture %}

```
{{RAW_INTCODE}}
```

&nbsp;

# Decorating your Syntax tree

The IntC-language we specified yesterday is suitable when writing source code, but for further processing we want to transform the code into a *Syntax tree*. The leaves of this syntax tree will be statements and expressions, and the parent nodes are `if`- statements, `for`-statements, functions, and more.

Here is an illustration of how source code is mapped to a Syntax tree:

![The Syntax tree of a Hello World-program](assets/syntax_tree.png)

(Rough diagram. Many details skimmed over.)

A *good* compiler typically does a series of pre-processing steps, tokenization, and other clever manipulations before generating the syntax tree (unlike my mess of an IntCode-compiler ...).