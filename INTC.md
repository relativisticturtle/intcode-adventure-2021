# **I**ntC's **n**ot **t**echnically **C**

Formally introducing \*drumroll\*: "**I**ntC's **n**ot **t**echnically **C**"  ("**IntC**" for short).

## Variables: `int`

That should be sufficient, don't you think? *True/false value?* That's an `int`. *Character?* `int`! *Pointer?* `int`, `int`, `int`! *Floating point number?* That's... those are forbidden! Shoo!

Declaration of an integer variable `a`:

```c
int a;
```

Declaration of an integer array `b` of 10 elements:

```c
int b[10];
```

(This allocates 10 integers somewhere and initializes `b` to point to the first element. After that `b` is an `int`, same as any other variable).

In global scope, and global scope *only*, it is allowed to initialize arrays with an integer list or an ASCII-string:

```c
int fibonacci[] = {1,1,2,3,5,8,13,21,34};
int message[] = "Hello World!";
```

In local scope (inside a function definition) this is not allowed. Nor is it allowed to initialize variables in their definition:

```c
  //  ... inside function definition
  int x = 42;   // NOT ALLOWED
  int y[] = {1, 2, 3};  // NOPE!
  int z;
  z = 42;       // OK
```

The ampersand `&` yields the address of a variable. The asterisk `*` is only used for multiplication and cannot be used for dereferencing pointers (as in C). The same thing can be achieved with array dereferencing at index 0: `ptr[0]`. Examples:

```c
int a;
int ptr;
int point[3];

ptr = &a;       // ptr is now the address of a
ptr[0] = 5;     // a is now 5
ptr = point;    // ptr is now same as point, the addres to a 3 element array
ptr[2] = 38;    // the 3rd element in that array is now 38
```

## Expressions

Expression evaluation can happen both on the right-hand side of an assignment, as well as in array dereferencing (`[]`) on either side of the assignment:

```c
result = 5*x + y*(9*buffer[4*i] - 7*buffer[j]);
buffer[(W+110)*y+x] = imag0[W*(y-55) + (x-55)];
```

or as an argument in a function call (or as the condition of an `if` or `for`):

```c
print(gamma_rate * epsilon_rate);

if(0<(y<55)+(H+54<y)+(x<55)+(W+54<x)) {
    // ...
}
```

The following operators are allowed:
`+`, `-`, `*`, `()` (is that an operator?), `==`, `<`, `>`, `<=`, `>=`.

Additionally, `X += ...` and `Y *= ...` expands to `X = X + (...)` and `Y = Y * (...)`.

Note the absence of "`/`" (division), "`!`" (negation) and "`!=`" (not-equals). The logical operators `&&` and `||` are also missing. The division-operator is understandably absent since it has no IntCode-instruction counterpart. (Neither does subtraction, but it is just a matter of throwing in an extra multiplication by `-1`). When it comes to the others I was just ~~lazy~~ minimalistic.

Some workarounds for the missing operators, instead of ..., write ... :

- `!condition` -> `(condition == 0)`
- `value != x` -> `(value == x) == 0` 
- `foo && bar` -> `foo * bar` (if only zero/non-zero is important)
- `foo || bar` -> `foo + bar` (careful if negative value are present)

Note: [Short-circuit evaluation](https://en.wikipedia.org/wiki/Short-circuit_evaluation) never happens.

## Functions

May take any number of arguments. Do *not* yield return values. Are defined as such:

```c
void function(int a, int b, int c) {
    // function body
}
```

**Note:**

- Must be declared as `void`.
- The left curly-brace, `{`, must be on the first line.
- The right curly-brace, `}`, must be alone on its line.

Passing data back from the function can only happen if it is passed a pointer (or if it writes to a global variable, but that'd be an anti-pattern IMHO).

**Example:**

```c
void sum(int result, int a, int b) {
    result[0] = a + b;
}

int main() {
    int res;
    sum(&res, 10, 13);
}
```

Functions return when the program flow reaches their end, or when it hits a `return`-statement.


## Control structures

Having control-structures is a real boon, but let's not be greedy! Surely you can make do with `if` (`else`) and `for`-loops?

This is their general structure:

```c
if(condition) {
    // block to execute if condition evaluates to non-zero
}
else if(other_condition) {
    // (optional) block to if instead other_condition evaluates to non-zero
}
else {
    // (optional) block to execute if no other condition evaluated to non-zero
}

int i; // (i cannot be declared in the for-loop)
for(i=0; i<10; i+=1) {
    // do this block for 10 iterations
}
```

Similar to function definitions, the format is quite strict for these structures:
- Everything up-to and including the left curly brace (`{`) must be on one line.
- The right curly brace (`} `) must be alone on its line.

## Input/output

`scan()` is a built-in that takes a memory location as argument and scans 1 integer from standard input into that.

**Note:** `IntC` is not aware if the input integer originates from a decimal number or an ASCII-coded character. This distinction must be made in the IntCode-VM. (In my [implementation](intcode_vm.cpp) ASCII is default and decimal mode is activated with the `-d` flag).

`print()` is a built-in that takes a value as argument and prints it to standard output.

**Note:** Same here, `IntC` does not know if the number should be printed in decimal or ASCII-form. This is up to the IntCode-VM.

## Entry point
An IntCode-program's execution starts at the `main()`-function, of which there must exist exactly one - taking no parameters.

```c
int message[] = "Hello world!";

void prints(int s) {
    for(; s[0]; s+=1) {
        print(s[0]);
    }
}

void main() {
    prints(message);
    print(10); // Line feed: '\n'
}
```

## Compiler

The one-and-only [IntCode compiler](intcode_cc.py), written in pure Python 3, no external dependencies.

To compile an IntC-file into beautiful IntCode:

```
python intcode_cc.py -i <input IntC-file> -o <output IntCode-file>
```
