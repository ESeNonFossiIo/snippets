(** * Euclid's theorem: there are infinitely many primes

    Stated constructively, "infinitely many" becomes "unbounded": for every
    [n] there exists a prime [p] with [p > n]. No set of primes is complete,
    because we can always exhibit one past any bound you name.

    The witness is a prime divisor of [n! + 1]. The classical presentation
    multiplies together a finite list of primes and adds one; using the
    factorial avoids carrying that list around, since [n!] is already
    divisible by every number in [1..n].

    ** Proof sketch

    Let [N = n! + 1].
    - [N >= 2], so [N] has some prime divisor [p].
    - Suppose [p <= n]. Then [p] divides [n!], because [n!] is the product
      [1 * 2 * ... * p * ... * n].
    - [p] also divides [N = n! + 1], so [p] divides the difference
      [N - n! = 1].
    - The only natural number dividing [1] is [1] itself, contradicting
      [p >= 2] (every prime is at least 2).
    - Hence [p > n].

    ** Building

    Compile and check with the accompanying makefile:
<<
    make        # compile: rocq compile prime_infinite.v
    make doc    # render this documentation to ./html
    make clean
>>

    The final [Print Assumptions] reports "Closed under the global context",
    i.e. the proof rests on no axioms.

    Tested with the Rocq Prover 9.2. *)

From Stdlib Require Import Arith Lia Factorial.

(** * Preamble

    Two ingredients the proof needs are missing from the standard library:

    - [prime] does exist, but in [Znumtheory], where it is a predicate on
      [Z]. We work over [nat], so we restate it here.
    - "every number greater than 1 has a prime divisor" is not in the
      library at all, under any name. ([Znumtheory.prime_divisors] is a
      different statement: it enumerates the divisors *of* a prime.) So we
      prove it below, along with the fact that [d <= n] implies [d | n!].

    Everything in this section is standard bookkeeping; the interesting
    argument is [prime_infinite] at the end. *)

(** A natural number is prime when it is at least 2 and its only divisors
    are 1 and itself. Requiring [2 <= p] is what excludes 0 and 1. *)
Definition prime (p : nat) : Prop :=
  2 <= p /\ forall d, Nat.divide d p -> d = 1 \/ d = p.

(** Divisibility is decidable, since [d | n] is equivalent to [n mod d = 0]
    and equality on [nat] is decidable.

    Proved with [Defined] rather than [Qed]: the term must stay transparent
    so that later proofs can actually compute with the decision. *)
Lemma divide_dec : forall d n, {Nat.divide d n} + {~ Nat.divide d n}.
Proof.
  intros d n.
  destruct (Nat.eq_dec (n mod d) 0) as [H | H].
  - left. apply Nat.Lcm0.mod_divide. exact H.
  - right. intro Hd. apply H. apply Nat.Lcm0.mod_divide. exact Hd.
Defined.

(** Bounded search: for a decidable predicate [P], either some [k < n]
    satisfies it, or none does. Straightforward induction on the bound [n].

    Note the left branch is a [sig] ([{k | ...}]) and not an [exists]. The
    [exists] version does not typecheck here: this sumbool lives in [Set],
    and a witness buried in a [Prop] cannot be eliminated to build it —
    "proofs can be eliminated only to build proofs". *)
Lemma bounded_dec :
  forall (P : nat -> Prop),
    (forall k, {P k} + {~ P k}) ->
    forall n, {k | k < n /\ P k} + {forall k, k < n -> ~ P k}.
Proof.
  intros P Pdec n. induction n as [| n IH].
  - right. intros k Hk. lia.
  - destruct IH as [[k [Hk HP]] | Hnone].
    + left. exists k. split; [lia | exact HP].
    + destruct (Pdec n) as [HP | HP].
      * left. exists n. split; [lia | exact HP].
      * right. intros k Hk. destruct (Nat.eq_dec k n) as [-> | Hne].
        -- exact HP.
        -- apply Hnone. lia.
Defined.

(** Every number [>= 2] has a prime divisor.

    By strong induction on [n]: search for a proper divisor [d] with
    [2 <= d < n]. If one exists, [d] is smaller than [n], so the induction
    hypothesis hands us a prime dividing [d], which then divides [n] by
    transitivity. If none exists, [n] has no divisors besides 1 and itself,
    which is precisely the definition of [n] being prime — so [n] divides
    itself and we are done. *)
Lemma prime_divisor :
  forall n, 2 <= n -> exists p, prime p /\ Nat.divide p n.
Proof.
  intro n.
  induction n as [n IH] using (well_founded_induction Nat.lt_wf_0).
  intro Hn.
  (* Is there a proper divisor 2 <= d < n ? *)
  destruct (bounded_dec (fun d => 2 <= d /\ Nat.divide d n)) with (n := n)
    as [[d [Hdn [Hd2 Hdiv]]] | Hnone].
  { (* the predicate is decidable: conjunction of two decidable facts *)
    intro k. destruct (le_dec 2 k) as [Hk | Hk].
    - destruct (divide_dec k n) as [Hd | Hd].
      + left. split; assumption.
      + right. intros [_ C]. exact (Hd C).
    - right. intros [C _]. exact (Hk C). }
  - (* Yes: recurse on the smaller number d, then chain the divisibility. *)
    destruct (IH d Hdn Hd2) as [p [Hp Hpd]].
    exists p. split; [exact Hp |].
    apply Nat.divide_trans with (m := d); assumption.
  - (* No: n itself is prime, and n divides n. *)
    exists n. split; [| apply Nat.divide_refl].
    split; [exact Hn |].
    intros d Hd.
    assert (Hd_le : d <= n) by (apply Nat.divide_pos_le; [lia | exact Hd]).
    destruct (Nat.eq_dec d 1) as [-> | H1]; [left; reflexivity |].
    destruct (Nat.eq_dec d n) as [-> | Hne]; [right; reflexivity |].
    (* Any remaining d is a proper divisor, contradicting Hnone. The case
       split on d rules out 0 and 1 so that 2 <= d holds. *)
    exfalso.
    destruct d as [| [| d']].
    + (* d = 0 divides n, so n = 0, contradicting 2 <= n *)
      apply Nat.divide_0_l in Hd. lia.
    + exact (H1 eq_refl).
    + apply (Hnone (S (S d'))); [lia | split; [lia | exact Hd]].
Qed.

(** Every [d] with [1 <= d <= n] divides [n!], since [n!] is the product
    [1 * 2 * ... * d * ... * n].

    Induction on [n], unfolding [fact (S m) = S m * fact m]. Either [d] is
    the leading factor [S m], or [d <= m] and it already divides [fact m],
    which in turn divides [fact (S m)]. *)
Lemma divide_fact : forall n d, 1 <= d -> d <= n -> Nat.divide d (fact n).
Proof.
  intro n. induction n as [| m IH]; intros d H1 Hn.
  - (* n = 0 is impossible: it would need 1 <= d <= 0 *)
    lia.
  - assert (Hf : fact (S m) = S m * fact m) by reflexivity.
    rewrite Hf.
    destruct (Nat.eq_dec d (S m)) as [-> | Hne].
    + apply Nat.divide_factor_l.
    + apply Nat.divide_trans with (m := fact m).
      * apply IH; lia.
      * apply Nat.divide_factor_r.
Qed.

(** * The theorem *)

(** For every [n] there is a prime greater than [n]. *)
Theorem prime_infinite : forall n : nat, exists p : nat, prime p /\ p > n.
Proof.
  intros n.

  (* Step 1: Construct our massive number N.
     Instead of multiplying a list of primes, we use the factorial (n!) + 1. *)
  set (N := fact n + 1).

  (* Step 2: N > 1, since n! >= 1. This has to be established *before*
     prime_divisor is applied, because it is that lemma's hypothesis --
     it is an argument to pass in, not a goal left over afterwards. *)
  assert (HN : 2 <= N).
  { unfold N. pose proof (lt_O_fact n). lia. }

  (* Step 3: every number >= 2 has a prime divisor, so N has one. *)
  destruct (prime_divisor N HN) as [p [H_prime H_div]].

  (* Step 4: p is the answer to our goal. *)
  exists p.
  split.

  (* Goal A: Prove p is prime *)
  - exact H_prime.

  (* Goal B: Prove p > n *)
  - (* Proof by contradiction within the constructive bound:
       Assume p <= n.
       If p <= n, then p must divide n! (since n! is 1*2*...*p*...*n).
       We already know p divides (n! + 1).
       If p divides both X and X+1, p must divide their difference (1).
       The only integer that divides 1 is 1.
       But p is prime, meaning p >= 2.
       1 >= 2 is a contradiction. Therefore, p > n. *)
    destruct H_prime as [Hp2 _].
    destruct (le_gt_dec p n) as [Hle | Hgt]; [| exact Hgt].
    exfalso.
    assert (Hfact : Nat.divide p (fact n)) by (apply divide_fact; lia).
    assert (Hone : Nat.divide p 1).
    { replace 1 with (N - fact n) by (unfold N; lia).
      apply Nat.divide_sub_r; assumption. }
    apply Nat.divide_1_r in Hone.
    lia.
Qed.

(** Sanity check: no axioms are used. Prints "Closed under the global
    context". *)
Print Assumptions prime_infinite.
