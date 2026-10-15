-- Lean 4 // Basics

-- Blackboard-bold notation for core numeric types.
notation "ℕ" => Nat
notation "ℤ" => Int
notation "ℚ" => Rat

-- 1. Simple function definitions
def add (a b : ℕ) : ℕ := a + b

def greet (name : String) : String := s!"Hello, {name}!"

-- 2. Pattern matching on natural numbers
def factorial : ℕ → ℕ
  | 0 => 1
  | n + 1 => (n + 1) * factorial n

-- 3. List operations
def sumList : List Nat → Nat
  | [] => 0
  | head :: tail => head + sumList tail

-- 4. Type annotations and inference
def isEven (n : Nat) : Bool :=
  n % 2 == 0

def filter_evens (nums : List Nat) : List Nat :=
  nums.filter isEven

-- 5. Fibonacci sequence
def fibonacci : Nat → Nat
  | 0 => 0
  | 1 => 1
  | n + 2 => fibonacci (n + 1) + fibonacci n

/- Theorem: fibonacci (n+1) / fibonacci n → φ as n → ∞,
   where φ = (1 + √5) / 2 is the golden ratio.

   This is NOT formalized below — proving it in Lean requires real
   number limits (Filter.Tendsto, nhds), which only exist in
   Mathlib. Mathlib already contains this exact result, but pulling
   it in just for this proof costs several GB, so here is the
   mathematical proof written out instead.

   --- Proof (via Binet's formula) ---

   Step 1: Solve the recurrence.
   fibonacci satisfies F(n+2) = F(n+1) + F(n), whose characteristic
   equation is x² = x + 1, i.e. x² - x - 1 = 0. Its two roots are:

     φ = (1 + √5) / 2  ≈ 1.618   (the golden ratio)
     ψ = (1 - √5) / 2  ≈ -0.618

   The general solution to the recurrence has the form
   F(n) = A·φⁿ + B·ψⁿ for constants A, B fixed by the initial
   conditions F(0) = 0, F(1) = 1:

     A + B = 0            (from F(0) = 0)          ⇒  B = -A
     A·φ + B·ψ = 1         (from F(1) = 1)
     A·(φ - ψ) = 1
     A = 1 / (φ - ψ) = 1 / √5        (since φ - ψ = √5)

   This gives Binet's formula:

     F(n) = (φⁿ - ψⁿ) / √5

   Step 2: Take the ratio F(n+1) / F(n).

     F(n+1)/F(n) = (φ^(n+1) - ψ^(n+1)) / (φⁿ - ψⁿ)

   Divide numerator and denominator by φⁿ:

     = (φ - ψ·(ψ/φ)ⁿ) / (1 - (ψ/φ)ⁿ)

   Step 3: Take the limit as n → ∞.

   Since |ψ| ≈ 0.618 < φ ≈ 1.618, the ratio r = ψ/φ satisfies
   |r| ≈ 0.382 < 1. A geometric sequence rⁿ with |r| < 1 converges
   to 0, so (ψ/φ)ⁿ → 0.

   Substituting into the ratio above:

     lim F(n+1)/F(n) = (φ - ψ·0) / (1 - 0) = φ

   which is the golden ratio, as claimed. ∎

   (Formalizing this in Lean would need: ℝ itself, the geometric
   sequence limit lemma for |r| < 1, and continuity of division at
   a nonzero denominator — all standard, but all Mathlib.) -/

-- 6. Proof example: simple equality
theorem add_comm (a b : Nat) : a + b = b + a := by
  omega

-- Main entry point
def main : IO Unit := do
  IO.println "=== Lean 4 Basics ==="
  IO.println ""

  -- Test add
  IO.println s!"add 3 5 = {add 3 5}"

  -- Test greet
  IO.println (greet "Lean")

  -- Test factorial
  IO.println s!"factorial 5 = {factorial 5}"

  -- Test sumList
  let nums : List Nat := [1, 2, 3, 4, 5]
  IO.println s!"sumList [1,2,3,4,5] = {sumList nums}"

  -- Test isEven
  IO.println s!"isEven 4 = {isEven 4}"
  IO.println s!"isEven 7 = {isEven 7}"

  -- Test filter_evens
  let evens := filter_evens nums
  IO.println s!"filter_evens [1,2,3,4,5] = {evens}"

  -- Test fibonacci
  IO.println s!"fibonacci 7 = {fibonacci 7}"

  IO.println ""
  IO.println "All tests completed!"
