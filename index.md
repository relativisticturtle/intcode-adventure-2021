# Welcome to my IntCode-adventure!

On these pages I shall tell the story of how I solve the [Advent of Code 2021](https://adventofcode.com/2021)-problems with algorithms implemented in IntCode.

To this end I prepared a tool, an IntCode-compiler, to generate the IntCode-program from a made-up C language imitation ("C lite"). For me this was a first. I have never coded anything resembling a compiler before. However, from curiosity and general interest in computer science I had a (*very*) vague idea of what one was supposed to do. I challenged myself to write this tool myself because, you know... er... fun? And just in time for this year's round of AoC the pieces came together and I tried it on the day 1 puzzles. Moment of truth. One star. Two stars. It works! It actually works!

So, to be clear:

This is *not* a HOWTO on writing quality compilers (or quality code in general...).

This is my IntCode-adventure of 2021!

## Adventure
[Prologue - It begins (again)](prologue.md)

[Day 1 - What is IntCode?](day01.md)

[Day 2](day02.md)

[Day 3](day03.md)

[Day 4](day04.md)

[Day 5](day05.md)

[Day 6](day06.md)


## Background
The 2019's round of IntCode-problems taught me a lot of how the code I type maps into instructions a CPU would process. Fascinated and curious I asked myself: "Could I generate the IntCode to implement algorithms and solve tasks?".

Against my better judgement I became obsessed with the idea. Realistically, I would not be able to hand-write the IntCode for anything but the most trivial tasks. But if I made a compiler... I have a rough idea of how a compiler may work, but have never undertaken anything such before.  

I began coding, I became more invested in the project, I hit dead-ends, I came up with creative fixes, I came up with unholy fixes ... and I eventually managed to parse code written in the made-up C language-imitation.

And then came Dec 1st.
