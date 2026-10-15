(* factorial.ml

   Run with: ./factorial [n]
   If n is omitted, it defaults to 10.
 *)

(* Standard implementation. *)
let rec factorial n =
  if n <= 1 then 1
  else n * factorial (n - 1)

(* Tail-recursive version. *)
let factorial_tail n =
  let rec aux n acc =
    if n <= 1 then acc
    else aux (n - 1) (acc * n)
  in
  aux n 1

(* **************************************** *)
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
  if n < 0 then begin
    Printf.printf "factorial is undefined for negative numbers (%d)\n" n;
    exit 1
  end;
  Printf.printf "factorial(%d)      = %d\n" n (factorial n);
  Printf.printf "factorial_tail(%d) = %d\n" n (factorial_tail n)
