(** * Rocq basics: a guided tour of proof tactics

    This file introduces the core proof tactics used in [prime_infinite.v],
    starting from the simplest possible proofs.

    Read each example, then look at the tactic that closes the goal and ask:
    "What was the goal before this tactic, and why does this tactic work?" *)

From Stdlib Require Import Arith Lia.


(** * 1. Reflexivity

    The simplest proofs: both sides of [=] are definitionally equal, meaning
    Rocq can verify equality by computation alone. No reasoning needed. *)

Example one_plus_one : 1 + 1 = 2.
Proof. reflexivity. Qed.

Example three_times_four : 3 * 4 = 12.
Proof. reflexivity. Qed.


(** * 2. Introducing hypotheses

    [forall n, ...] proofs start by moving [n] from the goal into the context
    with [intro n]. Think of it as "fix an arbitrary [n]". *)

Lemma add_zero_r : forall n : nat, n + 0 = n.
Proof.
  intro n.
  (* Goal: n + 0 = n
     [n + 0] reduces to [n] by the definition of [+] on [nat].
     [simpl] applies that reduction, leaving [n = n]. *)
  simpl.
  reflexivity.
Qed.


(** * 3. Proof by induction

    When [simpl] and [reflexivity] are not enough, use [induction].
    [induction n] splits the goal into two subgoals:
      - base case : [n = 0]
      - inductive step : assuming the property holds for [n], prove it for [S n]

    This is the most important tactic for proofs about [nat]. *)

Lemma add_zero_l : forall n : nat, 0 + n = n.
Proof.
  intro n.
  induction n as [| n IH].
  - (* base case: n = 0. Goal: 0 + 0 = 0. Follows by computation. *)
    reflexivity.
  - (* inductive step.
       IH   : 0 + n = n
       Goal : 0 + S n = S n
       [simpl] reduces [0 + S n] to [S (0 + n)], then [IH] finishes it. *)
    simpl. rewrite IH. reflexivity.
Qed.

Lemma add_comm : forall n m : nat, n + m = m + n.
Proof.
  intros n m.
  induction n as [| n IH].
  - (* 0 + m = m + 0 *)
    simpl. rewrite add_zero_r. reflexivity.
  - (* S n + m = m + S n
       IH : n + m = m + n *)
    simpl.
    rewrite IH.
    (* Goal: S (m + n) = m + S n. This follows from a standard library lemma. *)
    rewrite Nat.add_succ_r.
    reflexivity.
Qed.


(** * 4. Linear arithmetic with [lia]

    [lia] ("Linear Integer/natural Arithmetic") decides goals that are linear
    combinations of hypotheses over [nat] or [Z]. It handles [+], [-], [*]
    by constants, [<=], [<], [=], and [<>].

    Rule of thumb: if your goal looks like arithmetic and [reflexivity] does
    not close it, try [lia] before writing a manual proof. *)

Lemma le_add_r : forall n m : nat, n <= n + m.
Proof.
  intros n m. lia.
Qed.

Lemma lt_succ : forall n : nat, n < S n.
Proof.
  intro n. lia.
Qed.

(** [lia] can also combine several hypotheses. *)
Lemma squeeze : forall a b c : nat, a <= b -> b <= c -> a <= c.
Proof.
  intros a b c H1 H2. lia.
Qed.


(** * 5. [destruct]: case analysis

    When a hypothesis or term has several constructors, [destruct] splits the
    goal into one subgoal per case.

    [nat] has two constructors: [O] (zero) and [S n] (successor). *)

Lemma pos_or_zero : forall n : nat, n = 0 \/ 0 < n.
Proof.
  intro n.
  destruct n as [| n'].
  - (* n = 0 *) left. reflexivity.
  - (* n = S n' *) right. lia.
Qed.

(** [Nat.eq_dec] gives a decidable equality: for any [n] and [m] you get
    either a proof of [n = m] or a proof of [n <> m].
    This is the same pattern used extensively in [prime_infinite.v]. *)

Lemma eq_or_ne : forall n m : nat, n = m \/ n <> m.
Proof.
  intros n m.
  destruct (Nat.eq_dec n m) as [Heq | Hne].
  - left.  exact Heq.
  - right. exact Hne.
Qed.


(** * 6. [exists]: providing a witness

    A goal [exists x, P x] is proved by [exists t] (supply the concrete
    witness [t]) followed by a proof of [P t].

    This is exactly how [prime_infinite] is proved: we exhibit the prime
    divisor of [n! + 1] as the witness. *)

Lemma exists_succ : forall n : nat, exists m, m = S n.
Proof.
  intro n.
  exists (S n).   (* the witness *)
  reflexivity.    (* S n = S n *)
Qed.

Lemma exists_gt : forall n : nat, exists m, m > n.
Proof.
  intro n.
  exists (S n).  (* S n is always greater than n *)
  lia.
Qed.


(** * 7. [exfalso] and proof by contradiction

    To prove [P] by contradiction: use [exfalso] to replace the goal with
    [False], then derive a contradiction from the hypotheses.

    In [prime_infinite.v] this appears in the key step: assuming [p <= n]
    leads to [p | 1], contradicting [p >= 2]. *)

Lemma not_lt_zero : forall n : nat, ~ (n < 0).
Proof.
  intro n.
  intro H.       (* H : n < 0 *)
  exfalso.       (* Goal becomes False *)
  lia.           (* n < 0 is impossible for nat *)
Qed.

Lemma contra_example : forall n : nat, n < n -> False.
Proof.
  intros n H. lia.
Qed.


(** * 8. Divisibility

    [Nat.divide d n] means [exists k, n = d * k].
    Key lemmas from the standard library:
      - [Nat.divide_refl]     : [d | d]
      - [Nat.divide_trans]    : [d | m -> m | n -> d | n]
      - [Nat.divide_add_r]    : [d | m -> d | n -> d | m + n]
      - [Nat.divide_sub_r]    : [d | m -> d | n -> d | m - n]  (when [n <= m])
      - [Nat.divide_1_r]      : [d | 1 -> d = 1]

    These are exactly the divisibility facts that power [prime_infinite]. *)

Lemma six_divides_twelve : Nat.divide 6 12.
Proof.
  exists 2. reflexivity.   (* 12 = 6 * 2 *)
Qed.

Lemma divide_self_plus_one_imp_one :
  forall d : nat, Nat.divide d 1 -> d = 1.
Proof.
  intros d H.
  apply Nat.divide_1_r.
  exact H.
Qed.


(** * Putting it all together

    The proof of [prime_infinite] uses, in order:
      [set]        — name a sub-expression ([N := fact n + 1])
      [assert]     — introduce an intermediate goal ([2 <= N])
      [destruct]   — unpack an [exists] or a case split
      [exists]     — supply a witness
      [exact]      — close a goal with a hypothesis that matches exactly
      [apply]      — close a goal using a lemma (unifying the conclusion)
      [lia]        — dispatch arithmetic obligations
      [exfalso]    — switch to deriving [False]

    All of these appear in the examples above. Open [prime_infinite.v] and
    find each one now that you have seen it in isolation. *)

Print Assumptions six_divides_twelve.
