# Lean 4 Basics

Introduction to Lean 4 programming through simple examples.

## What is Lean 4?

Lean 4 is a dependently-typed programming language and proof assistant. It combines:
- **Functional programming** (like OCaml, Haskell)
- **Type system** (very expressive, supports proofs)
- **Theorem proving** (formal verification of mathematical statements)

## Contents

### `basics.lean`

Demonstrates core Lean 4 concepts:

1. **Function definitions** — `add`, `greet` with type annotations
2. **Pattern matching** — `factorial` using case analysis on `Nat`
3. **List operations** — `sumList` with recursive list handling
4. **Filtering** — `filter_evens` using list methods
5. **Tail recursion** — `fib_acc` for efficient Fibonacci
6. **Proofs** — `add_comm` showing that addition is commutative

## Running the Code

### Prerequisites

Make sure Lean 4 is installed:

```bash
lean --version
```

If not installed, see [Lean 4 installation](https://lean-lang.org/lean4/doc/quickstart.html).

### Using Make

```bash
make check    # Type-check the file (no execution)
make run      # Run the program
make all      # Check and run (default)
make clean    # Remove build artifacts
make help     # Show available targets
```

### Manual Commands

```bash
# Type-check only (verify syntax and types)
lean basics.lean

# Compile and run with main function
lean --run basics.lean
```

## Expected Output

```
=== Lean 4 Basics ===

add 3 5 = 8
Hello, Lean!
factorial 5 = 120
sumList [1,2,3,4,5] = 15
isEven 4 = true
isEven 7 = false
filter_evens [1,2,3,4,5] = [2, 4]
fibonacci 7 = 13

All tests completed!
```

## Key Concepts

### Type Annotations

```lean
def add (a b : Nat) : Nat := a + b
```

- `a b : Nat` — both `a` and `b` have type `Nat` (natural numbers)
- `: Nat` — function returns a `Nat`
- `:=` — defines the function body

### Pattern Matching

```lean
def factorial : Nat → Nat
  | 0 => 1
  | n + 1 => (n + 1) * factorial n
```

Matches on the structure of `Nat`:
- Base case: `0 => 1`
- Recursive case: `n + 1 => ...`

### List Operations

```lean
def sumList : List Nat → Nat
  | [] => 0
  | head :: tail => head + sumList tail
```

- `[]` — empty list
- `head :: tail` — cons (head + rest)

### Proofs (Theorems)

```lean
theorem add_comm (a b : Nat) : a + b = b + a := by
  omega
```

- `theorem` — declares a provable statement
- `by` — enters tactic mode
- `omega` — automation tactic that solves arithmetic

### IO and Main

```lean
def main : IO Unit := do
  IO.println "Hello"
  IO.println s!"Value: {value}"
```

- `IO Unit` — an action that performs I/O and returns nothing (unit)
- `do` — monadic syntax for sequencing actions
- `s!"..."` — string interpolation

## Next Steps

1. Modify `add`, `greet`, or `factorial` to experiment
2. Add your own functions
3. Try to write a proof of a property
4. Explore the [Lean 4 documentation](https://lean-lang.org/lean4/doc/)

## Resources

- [Lean 4 Official Documentation](https://lean-lang.org/lean4/doc/)
- [Functional Programming in Lean](https://leanprover.github.io/functional_programming_in_lean/)
- [Mathematics in Lean](https://leanprover-community.github.io/)
