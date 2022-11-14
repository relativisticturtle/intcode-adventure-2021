# The Virtual Machine (VM)

## Background
To my knowledge there are no CPU-architecture that runs IntCode natively. To run an IntCode-program one needs an IntCode Virtual Machine - "IntCode VM" for short. This is completely analogous to Java bytecode and the JVM.

Participants in Advent of Code 2019 where tasked with successively building an IntCode VM to solve many of the puzzles. There should therefore be plenty of implementations of this virtual machine spread out in cyberspace. IntCode may very well win the category of "language with highest number of interpreters"(?).

If you participated that year and still have your IntCode VM laying around I do recommend you to dust it off and take it for a spin :).

## My implementation
If you do not have one already, or if you for some reason don't want to use it (clunky, inconvenient I/O, missing dependencies, ...?) I got you covered with my implementation:

**[intcode_vm.cpp](intcode_vm.cpp)**

### Usage

The virtual machine "`intcode_vm`" parses the IntCode-program to execute either from a file or from standard input.

From file:

```console
> intcode_vm hello.txt
```

From standard input:

```console
> intcode_vm
<type your program here>
```

or:

```console
$ cat hello.txt | ./intcode_vm        (Bash)
> type hello.txt 2>nul | intcode_vm   (Windows)
```

Only the first line is used in either case. In the latter case the standard input is delegated to the IntCode-program after this line. Thus input can be provided to the program either directly in the text-file (see [hello.txt](hello.txt) for an example) or by concatenation on the command-line:

```console
$ cat day01.txt input01.txt | ./intcode_vm        (Bash)
> type day01.txt input01.txt 2>nul | intcode_vm   (Windows)
```

### Options

To set the virtual machine in "decimal" (non-ASCII) mode:

- Include the switch `-d` in the command-line invocation (`intcode_vm -d`), or
- Have the IntCode-program end with a `D`. See [day01_golf.txt](2021/day01_golf.txt) for example.

For some stats (program-length, instructions executed), include the switch `-v`.

(Here `day01.txt` and `input01.txt` are the IntCode-program and puzzle-input, respectively, for Day 1).

## Notes

If you use your own IntCode VM (thumbs up!) to explore the code in this repository, then naturally the way the VM is launched and takes input will differ. Other issues to be aware of:

- What value is returned if attempting a `03`-operation when the input-buffer is empty?
  - (My IntCode-programs assume `0` or a negative value).
- Is input and output performed with decimal numbers or ASCII-encoded text?
  - (My virtual machine defaults to ASCII-encoded
  - ...but has the option to perform I/O in decimal mode as well).