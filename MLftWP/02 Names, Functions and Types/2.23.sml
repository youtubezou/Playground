fun p n = 1 + f (n - 1)
    and f n = if n <= 0 then 0 else p n + f (n - 1);

p(1);
p(2);
p(10);

(*
P_n = 1 + \sum_{k=1}^{n-1} P_k 
    = 1 + P_{n-1} + \sum_{k=1}^{n-2}
    = P_{n-1} + P_{n-1}
    = 2^{n-1}
*)
