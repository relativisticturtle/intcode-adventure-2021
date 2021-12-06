# IntCode-adventure of 2021
Hi and welcome to my IntCode-adventure!

On these pages I shall report on my progress solving the [Advent of Code 2021](https://adventofcode.com/2021)-problems with algorithms implemented in IntCode.

To this end I prepared a tool, an IntCode-compiler, to generate the IntCode-program from a made-up C language imitation ("C lite"). For me this was a first. I have never coded anything resembling a compiler before. However, from curiosity and general interest in computer science I had a (*very*) vague idea of what one was supposed to do. I challenged myself to write this tool myself because, you know... er... fun? And just in time for this year's round of AoC the pieces came together and I tried it on the day 1 puzzles. Moment of truth. One star. Two stars. It works! It actually works!

So, to be clear:

This is *not* a HOWTO on writing quality compilers (or quality code in general...).

This is my Advent of Code adventure!

## Adventure
[Prologue - It begins (again)](prologue.md)

[Day 1 - What is IntCode?](day01.md)

[Day 2 - Compiler-handwaving](day02.md)

<ul>
  {% for post in site.posts %}
    <li>
      <a href="{{ post.url | relative_url }}">{{ post.title }}</a>
    </li>
  {% endfor %}
</ul>



## Background
The 2019's round of IntCode-problems taught me a lot of how the code I type maps into instructions a CPU would process. Fascinated and curious I asked myself: "Could I generate the IntCode to implement algorithms and solve tasks?".

Against my better judgement I became obsessed with the idea. Realistically, I would not be able to hand-write the IntCode for anything but the most trivial tasks. But if I made a compiler... I have a rough idea of how a compiler may work, but have never undertaken anything such before. I also refrained from using external libraries or tools or even reading guides on the subject. I wanted the tool to be pure Python and entirely my own creation. 

I began coding, I became more invested in the project, I hit dead-ends, I overcame obstacles with creative and unholy fixes. I eventually managed to parse code written in a made-up C language-imitation.

And then came Dec 1st. 

This is my IntCode-adventure of 2021!


