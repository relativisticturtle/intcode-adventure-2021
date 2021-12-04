# IntCode-adventure of 2021
## Background
IntCode is an instruction set invented for the Advent of Code 2019. While very limited compared to the x86-family, and other common instruction sets, it is powerful enough to be Turing-complete. Many puzzles this year were delivered as IntCode-programs the participants would run through IntCode VM's they had crafted themselves in earlier tasks.

The 2019's round of IntCode-problems taught me a lot of how the code I type maps into instructions a CPU would process. Fascinated and curious I asked myself: "Could I generate the IntCode to implement algorithms and solve tasks?". And even better: "Could I solve *Advent of Code-problems* with IntCode?". I didn't consider this seriously at first. I knew it could be done in theory (that's what Turing-complete means after all), but in practice? No way?! Right...?

Against my better judgement I became obsessed with the idea. I would not do handwritten IntCode. But if I make a tool to generate the integers of the IntCode? An IntCode-compiler if you will.

I have a rough idea of how a compiler may work, but have never undertaken anything such before. I also refrained from using external libraries or tools or even reading guides on the subject. I wanted the tool to be pure Python and entirely my own creation. 

I began coding, I became more invested in the project, I hit dead-ends, I overcame obstacles with creative and unholy fixes. I eventually managed to parse code written in a made-up C language-imitation (*very* limited).

And then came Dec 1st. Moment of truth. One star. Two stars. It worked! It actually worked!

This is my IntCode-adventure of 2021!

## Adventure
[Prologue](PROLOGUE.md)
