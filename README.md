# Codename Gecko

I'm writing a programming language from scratch. It's a work in progress.

## Ambitions

* Minimal syntax (like Python),
* as fast as any compiled programming language,
* explicit garbage collection *or* static cycle checks (not sure if the latter is even possible),
* high-level tools for parallelization and async jobs instead of threads and mutexes,
* static read/write checker (s.t. threads cannot read and write to the same variable at the same time),
* safe by default, fast if needed (e.g. bounds checks, arithmetic overflow).

## Parser

Expression parsing

    /*
     * expr := sum
     * sum := multiplication [ "+" sum ]
     * multiplication := factor [ "*" multiplication ]
     * factor := "(" expression ")" | atom
     * atom := int_literal | string_literal | call
     * call := name [ "(" function_args ")" ]
     * function_args = expression [ "," function_args ]
    */

## Examples

### Simple

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
switch c[1]
    Success(el)
        switch b + el
            Success(sum)
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

### Enum

The enum concept was borrowed shamelessly from Rust.

```
enum Result
    Some: String
    None

result = do_something()

switch result
    Some(text)
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
