---
day: 2
prev: day01
this: day02
next: day03
title: Day 2
puzzle: Dive!
---
# {{ page.title }}

## [{{ page.puzzle }}](https://adventofcode.com/2021/day/{{ page.day }})

You show `I2C2` the planned course.

"It doesn't make sense!"

*What do you mean?*

"We are in the ocean. Why all the maneuvering? What obstacles are we trying to avoid? Why not go the straight path?"

*Or we can follow the planned course...*

"No! That's to inefficient. Calculate the end position so that we can go there directly"

(Again, just because you are talking with an inaminate object does *not* mean you are crazy).


## Solution

Today's input was marginally harder to process, but not much. Rather than considering the whole word ("forward", "up", "down") it suffices to consider the first character ("f", "u", "d") and then discard up-to-and-including the next whitespace character.

**IntCode:** [{{ page.this }}.txt](2021/{{ page.this }}.txt) &#124; **C lite:** [{{ page.this }}.c](2021/{{ page.this }}.c)
{% capture RAW_INTCODE %}{% include_relative 2021/{{ page.this }}.txt %}{% endcapture %}

```
{{RAW_INTCODE}}
```

&nbsp;

# The IntCode VM

To my knowledge there are no CPU-architecture that runs IntCode natively. To run an IntCode-program one needs an IntCode Virtual Machine - "IntCode VM" for short. This is completely analogous to Java bytecode and the JVM.

Participants in Advent of Code 2019 where tasked with successively building an IntCode VM to solve many of the puzzles. There should therefore be plenty of implementations of this virtual machine spread out in cyberspace. IntCode may very well win the category of "language with highest number of interpreters"(?).

If you participated that year and still have your IntCode VM laying around I do recommend you to dust it off and take it for a spin :).

If you do not have one already, or if you for some reason don't want to use it (clunky, inconvenient I/O, missing dependencies, ...?) I got you covered with my implementation:

- [Direct download](intcode_vm.cpp)
- [On GitHub](https://github.com/relativisticturtle/intcode-adventure-2021/blob/main/intcode_vm.cpp)
- C++ and standard libraries

On these pages I will occasionally write commands on how to run IntCode-programs using this virtual machine. If you use your own (thumbs up!) you need to adapt those commands to what your implementation would expect. Other issues to be aware of are:

- How is `EOF` handled? What value is returned if attempting a `03`-operation when the input-buffer is empty? (My IntCode-programs assume `0` or a negative value).
- Is input and output performed with decimal numbers or ASCII-encoded text? (My virtual machine defaults to ASCII-encoded, but has the option to perform I/O in decimal mode as well).

## My IntCode VM

During the 2019-round I coded a virtual machine running Python. The emphasis then was on flexibility rather than speed - you never knew what tomorrow's challenge would be. For this adventure I wanted something fast and minimal. I do *not* make any claim that it is the fastest or smallest implementation around - but it's much more so than what I had before at least.

### Usage

The virtual machine "`intcode_vm`" parses the IntCode-program to execute either from a file or from standard input.

From file:

```
intcode_vm hello.txt
```

From standard input:

```
intcode_vm
<type your program here>
```

or:

```
cat hello.txt | intcode_vm          (Bash)
type hello.txt 2>nul | intcode_vm   (Windows)
```

Only the first line is used in either case. In the latter case the standard input is delegated to the IntCode-program after this line. Thus input can be provided to the program either directly in the text-file (see [hello.txt](hello.txt) for an example) or by concatenation on the command-line:

```
cat day01.txt input01.txt | intcode_vm          (Bash)
type day01.txt input01.txt 2>nul | intcode_vm   (Windows)
```

(Here `day01.txt` and `input01.txt` are the IntCode-program and puzzle-input, respectively, for Day 1).

### Options

To set the virtual machine in "decimal" (non-ASCII) mode:

- Include the switch `-d` in the command-line invocation (`intcode_vm -d`), or
- Have the IntCode-program end with a `D`. See [day01_golf.txt](2021/day01_golf.txt) for example.

For some stats (program-length, instructions executed), include the switch `-v`.


### Build using [MSVC](https://docs.microsoft.com/en-us/cpp/build/projects-and-build-systems-cpp):

If you are at home with using the IDE, use that.

I used the [command-line tools](https://docs.microsoft.com/en-us/cpp/build/reference/compiler-command-line-syntax):

```
cl /EHsc /O2 intcode_vm.cpp
```

... if the build environment is not already set up, run the appropriate `vcvars`-script before. For me this was:

```
"C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64"
```

### Build using [GCC](https://gcc.gnu.org)

Had I been on a GNU/Linux-computer (or had I had my MinGW up and ready to go) the GNU Compiler Collection would have been my first choice. Not tested, but *should* work:

```
g++ -O2 -o intcode intcode_vm.cpp
```