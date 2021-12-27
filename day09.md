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
