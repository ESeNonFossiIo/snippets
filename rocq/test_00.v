(** * A first Rocq proof

    A minimal starting point: define a couple of things, state a couple of
    theorems, prove them. The goal is to see the core rhythm of an
    interactive proof — [intros], [simpl], [reflexivity], [induction] — on
    something small enough to hold in your head.

    ** Building

    Compile and check with the accompanying makefile:
<<
    make        # compile: rocq compile test_00.v
    make doc    # render this documentation to ./html
    make clean
>>

    Tested with the Rocq Prover 9.2. *)

From Stdlib Require Import Arith Lia.

(** * Warm-up: booleans

    [negb] is the built-in boolean "not". We prove that negating twice is
    the identity. With only two possible inputs ([true] and [false]),
    [destruct] splits the goal into those two cases, and each case reduces
    to a literal equality that [reflexivity] closes by computation. *)
Lemma negb_involutive_demo : forall b : bool, negb (negb b) = b.
Proof.
  intro b.
  destruct b.
  - reflexivity. (* negb (negb true)  = negb false = true  *)
  - reflexivity. (* negb (negb false) = negb true  = false *)
Qed.

(** * Addition on naturals

    [Nat.add] is defined by recursion on its *first* argument:
[[
    0 + m     = m
    (S n) + m = S (n + m)
]]
    That makes [0 + n = n] a one-step computation ([simpl] then
    [reflexivity], or [reflexivity] alone), but [n + 0 = n] is not: [n] is a
    variable, not a literal [0] or [S _], so nothing reduces yet. It has to
    be proved by induction on [n]. *)

(** [0 + n = n] holds by computation alone: unfolding [Nat.add] on its first
    argument immediately gives [n]. *)
Lemma add_0_l_demo : forall n : nat, 0 + n = n.
Proof.
  intro n.
  reflexivity.
Qed.

(** [n + 0 = n] needs induction, since [n] is arbitrary and sits in the
    position [Nat.add] recurses on.
    - Base case [n = 0]: goal becomes [0 + 0 = 0], again by computation.
    - Step case [n = S n']: the induction hypothesis [IH : n' + 0 = n']
      lets us [rewrite] the goal [S n' + 0 = S n'] down to [S n' = S n']. *)
Lemma add_0_r_demo : forall n : nat, n + 0 = n.
Proof.
  intro n.
  induction n as [| n' IH].
  - reflexivity.
  - simpl. rewrite IH. reflexivity.
Qed.

(** * A tiny arithmetic fact

    Nothing deep here — just confirming the numerals compute the way you'd
    expect, and that [lia] (linear integer arithmetic) can close goals like
    this without any manual rewriting. *)
Lemma two_plus_two : 2 + 2 = 4.
Proof.
  lia.
Qed.

(** Sanity check: no axioms are used. Prints "Closed under the global
    context". *)
Print Assumptions add_0_r_demo.
