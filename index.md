---
title: Advent of IntCode 2021
---

# Welcome to Advent of IntCode 2021!

Two years in space. Lost, abandoned, forgotten... but the Astronaut did *not* give up.

Against all odds (and Erisian bugs (and the space police)), the Astronaut has made a heroic return!

Now glory and celebration befitting a hero awaits our adventurer from 2019!

...except in the extremely unlikely case of a [sleigh-key dropping accident](https://adventofcode.com/2021/day/1)...

The Astronaut - now Aquanaut - embarks on a new adventure.

## Adventure
[Prologue - It begins (again)](prologue.md)

[Day 1 - What is IntCode?](day01.md)

[Day 2 - Will it assemble?](day02.md)

[Day 3 - Syntax ahead!](day03.md)

[Day 4 - Call on me](day04.md)

[Day 5 - Stack it up](day05.md)

[Day 6](day06.md)

[Day 7](day07.md)

[Day 8](day08.md)

[Day 9](day09.md)

[Day 10](day10.md)

[Day 11](day11.md)

[Day 12](day12.md)


## Background
On these pages I shall tell the story of how I solve the [Advent of Code 2021](https://adventofcode.com/2021)-problems with algorithms implemented in IntCode.

The larger part of the telling will be on the tool I wrote for the purpose, an IntCode-compiler, if you will. I'll try to describe its design, beginning at a basic level, and advancing to the fully functional tool. I do this in the hope that it may demystify how compilers transform human-readable code to machine-instructions.

I have never coded anything resembling a compiler before. However, from curiosity and general interest in computer science I had a (*very*) vague idea of what one was supposed to do. I challenged myself to write this tool myself because, you know... er... fun? And just in time for this year's round of AoC the pieces came together and I tried it on the day 1 puzzles. Moment of truth. One star. Two stars. It worked! It actually worked!

So, to be clear:

This is *not* a HOWTO on writing quality compilers (or quality code in general...).

This is my IntCode-adventure of 2021!