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

# ...with Nodes

In my IntCode-compiler design I settled for these nodes:

```
NodeProgram
NodeFunction
NodeScope
  NodeBlock
  NodeIfElse
  NodeFor
NodeGoto
NodeAssignment
NodeVariable
NodeCall
NodeExpression
```

### NodeProgram
Root node of the program. Has a list of functions (`NodeFunction`) and global variables (`NodeVariable`).

### NodeFunction
Function-definition: name, parameter list (`NodeVariable`) and function body (`NodeBlock`).

### NodeScope
Abstract class for nodes that have variable-scope. Holds a variable list (`NodeVariable`).

### NodeBlock
Defines a sequence of statements to execute in order. Statements are `NodeAssignment`, `NodeCall`, `NodeGoto`, `NodeIfElse` or `NodeFor`.

### NodeIfElse
A condition (`NodeExpression`) and statements (`NodeBlock`) to execute if evaluated zero or non-zero, respectively.

### NodeFor
A statement (`NodeAssignment` or `NodeCall`) to perform once before. A condition (`NodeExpression`) determining if next iteration is to occur. And a statement (`NodeAssignment` or `NodeCall`) to perform after every iteration. In each iteration a sequence of statements (`NodeBlock`) is executed.

### NodeGoto
Used for `return`, `continue` and statements. `return` jumps to function-exit. `continue` and `break` jumps to the *current* `for`-loop's next iteration and exit, respectively.

### NodeAssignment
An expression (`NodeExpression`) to be evaluated and a target (`NodeExpression`) to put the result. (The target is most commonly a variable or an array expression).

### NodeVariable
Name and scope (global or local).

### NodeCall
Name of function to call and a list of arguments (`NodeExpression`).

### NodeExpression
A text-string of the expression.





