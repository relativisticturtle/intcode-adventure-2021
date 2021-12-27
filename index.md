---
title: Advent of IntCode 2021
---

# Welcome to Advent of IntCode 2021!

Two years in space. Lost, abandoned, forgotten... but the Astronaut did *not* give up.

Against all odds (and Erisian bugs (and the space police)), the Astronaut has made a heroic return!

Now glory and celebration befitting a hero awaits our adventurer from 2019!

...except in the extremely unlikely case of a [sleigh-key dropping accident](https://adventofcode.com/2021/day/1)...

The ~~Astronaut~~ Aquanaut embarks on a new adventure!

## Adventure

[Prologue - It begins (again)](prologue.md)

[Day 1 - What is IntCode?](day01.md)

[Day 2 - The IntCode VM](day02.md)

[Day 3 - Handcoding an IntCode-program](day03.md)

[Day 4 - Will it assemble?](day04.md)

[Day 5 - &#9835; Harder, Better, Faster, Stronger &#9835;](day05.md)

[Day 6 - Reflections on the hand-coding](day06.md)

[Day 7 - **I**ntC's **n**ot **t**echnically **C**](day07.md)

[Day 8 - Decorating your Syntax tree](day08.md)

[Day 9 - ...with Nodes](day09.md)

[Day 10 - ...and Parsing](day10.md)

[Day 11 - Memory layout](day11.md)

[Day 12](day12.md)

[Day 13](day13.md)

[Day 14](day14.md)

[Day 15](day15.md)

[Day 16](day16.md)

[Day 17](day17.md)

[Day 18](day18.md)

[Day 19](day19.md)

[Day 20](day20.md)

[Day 21](day21.md)

[Day 22](day22.md)

[Day 23 - Yo Dawg, I heard you like IntCode...](day23.md)

[Day 24](day24.md)

[Day 25](day25.md)


## IntCode-compiler

Here it is, enjoy! ;)

- [On GitHub](https://github.com/relativisticturtle/intcode-adventure-2021/blob/main/intcode_cc.py)

- [Direct download](intcode_cc.py)

Pure Python 3, no external dependencies.

To compile an [IntC-file](day07.md):

```
python intcode_cc.py -i <input IntC-file> -o <output IntCode-file>
```

To compile with many source-files, just list them after the `-i`. Example from day 9:

```
python intcode_cc.py -i math.c string.c sort.c 2021/day09.c -o 2021/day09.txt
```


## Resources

- GitHub-page: [intcode-adventure-2021](https://github.com/relativisticturtle/intcode-adventure-2021)
- IntCode-VM: [intcode_vm.cpp](intcode_vm.cpp) (should you need one)
- Advent of Code: [2021](https://adventofcode.com/2021) the puzzles I have been solving, [2019](https://adventofcode.com/2019) *Spaaaaaaaaace!*
