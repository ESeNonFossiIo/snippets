# Recursive Functions in OCaml

This document summarizes the theory of recursive functions demonstrated in `factorial.ml`.

## What is Recursion?

A recursive function is a function that **calls itself** to solve a smaller instance of the same problem. Every recursive function needs:

1. **Base case**: A condition where the function stops calling itself and returns a value directly
2. **Recursive case**: The function calls itself with a simpler/smaller input

## Standard Recursion: Factorial

### Mathematical Definition

The factorial of `n` (written `n!`) is:
- `0! = 1`
- `1! = 1`
- `n! = n × (n-1)!` for `n > 1`

### OCaml Implementation

```ocaml
let rec factorial n =
  if n <= 1 then 1
  else n * factorial (n - 1)
```

**How it works:**

- **Base case** (`n <= 1`): Returns `1` immediately
- **Recursive case** (`n > 1`): Multiplies `n` by the result of `factorial (n - 1)`

**Execution trace for `factorial 4`:**

```
factorial 4
= 4 * factorial 3
= 4 * (3 * factorial 2)
= 4 * (3 * (2 * factorial 1))
= 4 * (3 * (2 * 1))
= 4 * (3 * 2)
= 4 * 6
= 24
```

### The Call Stack

Each recursive call pushes a new frame onto the call stack. For `factorial 4`, the stack grows:

```
factorial 4 → factorial 3 → factorial 2 → factorial 1 → 1 (return)
```

Then **unwinds** as results propagate back up.

**Problem:** For large `n`, this uses `O(n)` memory on the stack, risking a **stack overflow**.

---

## Tail Recursion: Optimizing Recursion

A recursive function is **tail-recursive** if the recursive call is the **last operation** in the function. Many compilers (including OCaml) optimize tail recursion to use **constant memory** by reusing the same stack frame.

### Tail-Recursive Factorial

```ocaml
let factorial_tail n =
  let rec aux n acc =
    if n <= 1 then acc
    else aux (n - 1) (acc * n)
  in
  aux n 1
```

**Key idea:** Use an **accumulator** (`acc`) to build up the result as you recurse, instead of waiting to multiply on the way back.

**Execution trace for `factorial_tail 4`:**

```
aux 4 1
→ aux 3 4       (acc = 1 * 4)
→ aux 2 12      (acc = 4 * 3)
→ aux 1 24      (acc = 12 * 2)
→ 24            (base case: return acc)
```

**Advantage:** The recursive call happens **last** (in the `else` branch), so OCaml can reuse the stack frame instead of growing the stack. This uses **O(1) memory** instead of **O(n)**.

---

## Pattern Matching and Conditionals

OCaml uses `if...then...else` for conditionals, which is really pattern matching on boolean values.

```ocaml
if n <= 1 then 1 else n * factorial (n - 1)
```

This is equivalent to asking: *"Does `n <= 1` match true? If yes, return 1. Otherwise, return `n * factorial (n - 1)`."*

---

## Exception Handling

The `factorial.ml` program uses exception handling to parse command-line arguments safely.

```ocaml
try int_of_string Sys.argv.(1)
with Failure _ -> ...
```

- `int_of_string` converts a string to an integer
- If the string is invalid, it raises a `Failure` exception
- The `try...with` catches the exception and handles it gracefully (prints error, exits)

---

## Command-Line Arguments

The program reads arguments from `Sys.argv`, an array of all command-line arguments:

- `Sys.argv.(0)` = program name (e.g., `"factorial"`)
- `Sys.argv.(1)` = first user argument (e.g., `"5"`)
- `Array.length Sys.argv` = total number of arguments (including program name)

```ocaml
if Array.length Sys.argv > 1 then
  int_of_string Sys.argv.(1)
else
  10  (* default value *)
```

---

## Comparing Approaches

| Aspect | Standard Recursion | Tail Recursion |
|--------|-------------------|-----------------|
| Stack usage | O(n) | O(1) |
| Readability | Often simpler | Requires accumulator |
| Performance | Slower for large n | Faster, no stack overflow risk |
| When to use | Small inputs, mathematical clarity | Production code, large inputs |

---

## Key Takeaways

1. **Recursion** solves problems by breaking them into smaller subproblems
2. **Every recursive function needs a base case** to stop recursion
3. **Tail recursion** is optimized by the compiler to use constant memory
4. **Accumulators** track results during tail-recursive calls
5. **Exception handling** (`try...with`) makes programs robust against invalid input

For the factorial problem, both implementations compute the same result, but tail-recursive version scales better to larger inputs.
