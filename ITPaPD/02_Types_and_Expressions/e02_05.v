Require Import ZArith.

Open Scope Z_scope.

Definition add (a:Z) (b:Z) := a + b.
Definition add5 (a:Z) (b:Z) (c:Z) (d:Z) (e:Z) := add a (add (add b c) (add d e)).

Print add5.
