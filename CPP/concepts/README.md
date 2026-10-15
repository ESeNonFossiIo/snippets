# Concepts and `requires`

A **concept** is a named, compile-time boolean predicate over a template
parameter. A **`requires`-expression** is how you build one out of "does this
expression compile" checks. Together they put a readable contract on a
template: instead of a type parameter that silently accepts anything and
fails deep inside the function body (or worse, compiles into something
subtly wrong), the compiler checks the contract at the call site and names
the concept that failed.

They replace what used to be done with SFINAE (`std::enable_if`, trailing
return types probing for expression validity, tag dispatch) — the same
compile-time filtering, but expressed directly instead of as a trick that
exploits template substitution failure.

| File | What it demonstrates |
|------|------------------------|
| [`01_basic_concept.cpp`](01_basic_concept.cpp) | Defining a concept (`Numeric`) and constraining a function template with it |
| [`02_requires_clause.cpp`](02_requires_clause.cpp) | The four syntaxes for attaching a constraint to a template parameter |
| [`03_requires_expression.cpp`](03_requires_expression.cpp) | Building a concept (`Container`) from a `requires { ... }` expression: type, compound, and simple requirements |
| [`04_standard_concepts.cpp`](04_standard_concepts.cpp) | The library concepts in `<concepts>` — `same_as`, `convertible_to`, `integral`, `floating_point`, `equality_comparable`, `totally_ordered`, `regular`, `movable` |
| [`05_concept_overloading.cpp`](05_concept_overloading.cpp) | Subsumption: overload resolution picks the more specific of two concepts, and plain overloading on disjoint concepts |
| [`06_constrained_class_template.cpp`](06_constrained_class_template.cpp) | A class template constrained by a concept, plus a member function template with its own independent `requires` clause |

## Building

```bash
make all      # build every example
make test     # build, then run every example
make clean    # remove build artefacts
```

Built with `g++ -std=c++20 -Wall -Wextra` (GCC 16.1.1 here); every file
compiles warning-free.

## The four ways to spell a constraint

`02_requires_clause.cpp` writes the same constraint on `T` four times:

```cpp
template <typename T> requires std::integral<T>   T f(T x) { ... }  // requires clause
template <std::integral T>                        T f(T x) { ... }  // constrained parameter
auto f(std::integral auto x) { ... }                                 // abbreviated template
template <typename T> T f(T x) requires std::integral<T> { ... }     // trailing requires clause
```

Reach for the constrained-parameter or abbreviated forms when the constraint
is a single concept; reach for a `requires` clause (leading or trailing) when
it's a boolean expression combining several (`Concept1<T> && Concept2<T, U>`).

## `requires`-expression requirement kinds

`03_requires_expression.cpp`'s `Container` concept uses three of the four
kinds a `requires { ... }` block can contain:

- **type requirement** — `typename T::value_type;` — T must have that nested
  type (or type alias).
- **compound requirement** — `{ c.begin() } -> std::same_as<typename T::iterator>;`
  — the expression must compile *and* its result type must satisfy the
  concept named after `->`.
- **simple requirement** — a bare expression statement, e.g.
  `c.begin() != c.end();` — checks only that the expression is well-formed.
  **It does not check whether the expression is true.** An empty container
  still satisfies that line; there's no way to assert a runtime value inside
  a `requires`-expression, only that an expression type-checks.

(The fourth kind, a **nested requirement** — `requires SomeConcept<T>;`
inside the block — lets you embed an ordinary concept check alongside the
others; none of these files happen to need one on its own.)

## Subsumption

`05_concept_overloading.cpp` defines `concept B = A<T> && requires(T x){...}`.
Because `B`'s definition literally contains `A<T>`, the compiler can prove
that anything satisfying `B` also satisfies `A` — `B` *subsumes* `A`. When
two overloads are otherwise equally good candidates and one is constrained by
the subsuming concept, that one wins overload resolution. This is the piece
SFINAE couldn't do: a `bool`-valued trait that happens to evaluate `true` for
both cases gives the compiler no way to rank one over the other, since it
never sees the boolean *structure*, only the final value.

## What a concept violation actually looks like

Calling `square("hi")` against `01_basic_concept.cpp`'s `Numeric`-constrained
`square` (not in the file — it wouldn't compile) produces:

```
error: no matching function for call to 'square(const char [3])'
  • candidate: 'template<class T> requires Numeric<T> T square(T)'
    • constraints not satisfied
      • required for the satisfaction of 'Numeric<T>' [with T = const char*]
      • the expression 'is_arithmetic_v<T> [with T = const char*]' evaluated to 'false'
```

The error names the concept (`Numeric`) and the exact subexpression that
failed (`is_arithmetic_v<const char*>`), rather than the wall of "candidate
template ignored: substitution failure" text a `std::enable_if`-based
overload set produces for the same mistake.
