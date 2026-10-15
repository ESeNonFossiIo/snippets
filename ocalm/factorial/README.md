# Factorial (OCaml)

A minimal OCaml example for learning the language: computing `n!`
two different ways.

## Build & run

```sh
make build          # compiles factorial.ml -> ./factorial
make run            # builds, then runs with the default n=10
make run N=15       # builds, then runs with n=15
make clean          # removes build artifacts
```

Or without `make`:

```sh
ocamlopt -o factorial factorial.ml
./factorial 15
```

Requires an OCaml compiler (`ocamlopt`) on your `PATH` — installed via
[opam](https://opam.ocaml.org/), your package manager, or similar.

## What's in `factorial.ml`

The file defines the same function two ways to illustrate a core OCaml
idiom (tail recursion), then reads a number from the command line and
prints both results.

### `let` and `let rec`

```ocaml
let rec factorial n =
  if n <= 1 then 1
  else n * factorial (n - 1)
```

- `let` binds a name to a value (functions are values in OCaml).
- `let rec` is needed whenever a function calls itself — a plain `let`
  can't see its own name while it's still being defined.
- `if/then/else` is an **expression**, not a statement: it evaluates
  to a value (here, an `int`), which is why the function body is just
  one expression with no explicit `return`.
- Types are inferred. `factorial : int -> int` is deduced from how `n`
  is used (compared with `<=`, multiplied) — no type annotation needed.

### Tail recursion

```ocaml
let factorial_tail n =
  let rec aux n acc =
    if n <= 1 then acc
    else aux (n - 1) (acc * n)
  in
  aux n 1
```

In the plain version, each call waits for the recursive call to return
before it can multiply — so the pending multiplications stack up (one
per call) until the base case is hit. `factorial_tail` avoids that by
passing the running product along as an extra argument (`acc`,
starting at `1`). The recursive call to `aux` is the last thing that
happens in its branch — a *tail call* — so the compiler compiles it
into a loop instead of growing the stack. This is the standard OCaml
(and general functional-programming) pattern for writing loops that
are safe on large inputs: an inner `aux`/`loop` helper carrying an
accumulator, wrapped by a public function that supplies the initial
accumulator value.

### Reading command-line arguments

```ocaml
let () =
  let n =
    if Array.length Sys.argv > 1 then
      try int_of_string Sys.argv.(1)
      with Failure _ ->
        Printf.eprintf "error: %s is not a valid integer\n" Sys.argv.(1);
        exit 1
    else
      10
  in
  ...
```

- `let () = ...` is how OCaml runs top-level side-effecting code: the
  bound value must be `()` (`unit`, OCaml's "no value" type), which is
  what a block of print statements/`exit` naturally produces.
- `Sys.argv` is an array of strings; `Sys.argv.(0)` is the program
  name, so real arguments start at index `1`.
- `try ... with Failure _ -> ...` is OCaml's exception handling.
  `int_of_string` raises `Failure` on invalid input; `Failure _` is a
  pattern that matches that exception regardless of its message.

### Printing

```ocaml
Printf.printf "factorial(%d)      = %d\n" n (factorial n);
```

`Printf.printf` works like C's `printf`: a format string with typed
placeholders (`%d` for `int`, `%s` for `string`, ...). Unlike C, the
compiler type-checks the format string against the arguments you pass.

## Caveat: integer overflow

OCaml's native `int` is 63-bit on a 64-bit system, with no overflow
checking — `factorial 21` and beyond will silently produce a wrong
(wrapped) result. Try it:

```sh
make run N=25
```

Fixing this (e.g. with arbitrary-precision integers via the `zarith`
library) is a good next exercise once you're comfortable with the
basics shown here.
