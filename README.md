# Gecko Programming Language

I'm writing a programming language from scratch. It's a work in progress.

## Ambitions

* Minimal syntax (like Python),
* as fast as any compiled programming language,
* explicit garbage collection (runs when the user calls it),
* high-level tools for parallelization and async jobs instead of threads and mutexes,
* static read/write checker (s.t. threads cannot read and write to the same variable at the same time),
* safe by default, fast if needed (e.g. bounds checks, overflow errors).

## Roadmap

* ✔ Integer literals
* ✔ Float literals
* ✔ Arithmetic expressions
* ✔ Assignments
* ✔ If-Then-Else
* ✔ "while"-loops
* ✔ Range-based "for"-loops
* ✔ User functions
* "visit" construct
* List literals
* Map literals
* ◑ Garbage collection
* ✔ Template parameters for system types
* Template parameters for user functions
* Template parameters for user types
* Translate instructions to llvm
* Multithreading constructs

## Parser

Expression parsing

    expr := or
    or := and [ "or" or ]
    and := comparison [ "and" and ]
    comparison = sum [ comparator sum ]*
    comparator = "<" | "<=" | ">" | ">=" | "==" | "!="
    sum := multiplication [ "+" sum ]
    multiplication := factor [ "*" multiplication ]
    factor := [ "not" ] singular
    singular := "(" expression ")" | atom
    atom := bool_literal | int_literal | string_literal | call
    call := name [ "(" function_args ")" ]
    function_args = expression [ "," function_args ]

## Examples

```

int x = 0

while x < 10
    x = x + 1

print(x)

```

## Error handling dilemma

I want the language to be safe by default, i.e. an expression like ``a = b + c[1]``
should perform a bounds check and an overflow check at runtime.

I also want Rust-style error handling, i.e. every outcome of an operation must be handled
explicitly. However, it would be annoying to have to re-write the statement above as

```
visit c[1]
    Integer c1
        visit b + c1
            Integer sum
                a = sum
            Overflow
                ...
    OutOfBounds
        ...
```

This would be much easier with C++ style exceptions, but then you don't get the benefits of explicit exception handling. The '?'-operator from Rust would make the syntax a bit better,

```
a = (b + c[1]?)?
```

but it does not make me happy. Plus it seduces the developer to just add question marks all over the place, in which case error handling would be just as incomplete as in C++.

I hope I will come up with a variant to make this safe and convenient at the same time.

## Future Work

### Variant / visit

Used for error handling and optionals, similar to ``enum`` in Rust.

Example:

```
result = do_something()

visit result
    String text
        print("Received text: %s", text)
    None
        print("Received nothing :(")
```

### Parallelization

```
results = parfor i in 0..10
    yield i*i*i

# results now contains [0, 1, 8, 27, 64, ...]

```

Trying to write in a parfor block to an outside variable will raise a special exception.
