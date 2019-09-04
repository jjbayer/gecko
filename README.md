# Codename Gecko

I'm writing a programming language from scratch. It's a work in progress.

## Ambitions

* Minimal syntax (like Python),
* as fast as any compiled programming language,
* explicit garbage collection *or* static cycle checks (not sure if the latter is even possible),
* high-level tools for parallelization and async jobs instead of threads and mutexes,
* static read/write checker (s.t. threads cannot read and write to the same variable at the same time),
* safe by default, fast if needed (e.g. bounds checks, arithmetic overflow).

## Examples

### Simple

```

int x = 0

while x < 10
    x = x + 1

print(x)

```

### Parallelization

```
results = parfor i in 0..10
    yield i*i*i

# results now contains [0, 1, 8, 27, 64, ...]

```

Trying to write in a parfor block to an outside variable will raise a special exception.
