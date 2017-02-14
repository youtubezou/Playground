Require Import ZArith.

Eval compute in ((fun x:Z => Zplus (Zplus (Zmult 2 (Zmult x x)) (Zmult 3 x)) 3) 2%Z).
Eval compute in ((fun x:Z => Zplus (Zplus (Zmult 2 (Zmult x x)) (Zmult 3 x)) 3) 3%Z).
Eval compute in ((fun x:Z => Zplus (Zplus (Zmult 2 (Zmult x x)) (Zmult 3 x)) 3) 4%Z).
