# Advent of IntCode

Welcome to my Advent of IntCode 2021 adventure!


## What is IntCode?
IntCode was invented for, and heavily featured in, the [Advent of Code 2019](https://adventofcode.com/2019). While IntCode (and many other [esotoric languages](https://esolangs.org/wiki/Esoteric_programming_language)) may be referred to as a *language*, I think that *instruction set* would be a more appropriate term. The "code" consists of a single line of comma-separated integers, like so:

```
3,100,1001,1,1,1,1007,1,2100,98,1005,98,0,1101,0,0,99,1101,100,0,26,1001,26,1,27,7,0,0,98,1,99,98,99,1001,26,1,26,1007,27,2099,98,1005,98,21,4,99,107,1,23,98,1005,98,45,1101,3,0,23,1105,1,13
```


## What have I done?
I set out to solve as many of the [Advent of Code 2021's](https://adventofcode.com/2019) problems as possible using IntCode. The "code" above solves the [day 1's problems](https://adventofcode.com/2021/day/1). However, this is the only problem for which I *actually* hand-coded an IntCode-solution.

It might have been fun to solve all problems by hand-coding IntCode-instructions - *NOT!* Instead I came prepared by having developed an (almost-) fully functional IntCode-compiler for this event.

I have never coded anything resembling a compiler before. However, from curiosity and general interest in computer science I had a (*very*) vague idea of what one was supposed to do. I challenged myself to write this tool myself because, you know... er... fun?

So did I *really* solve the problems in IntCode?

At the very least: the solutions were produced by IntCode-programs that my compiler had generated from [IntC](INTC.md) (my made-up language "**I**ntC's **n**ot **t**echnically **C**"). And I did it all by myself, without relying on libraries or other external tools.

## How did it go?

I succeeded in solving all problems (except [day 24](https://adventofcode.com/2021/day/24) which turned out to be a delightful [pen-and-paper](assets/day24.png) problem).

In retrospect it *was fun* (yes, really!) and it *was an adventure!*

## Resources

[IntCode Virtual Machine (VM)](VIRTUALMACHINE.md) to run the IntCode.

[IntCode compiler](intcode_cc.py), written in pure Python 3, no external dependencies.

[**I**ntC's **n**ot **t**echnically **C**'s](INTC.md) language definition.

To compile an IntC-file with the IntCode-compiler:

```
python intcode_cc.py -i <input IntC-file> -o <output IntCode-file>
```
[Yo Dawg, I heard you like IntCode...](YODAWG.md)
