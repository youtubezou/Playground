Require Import ZArith.

Open Scope Z_scope.

Section add.
  Variables a b c d e : Z.
  Definition add5 := a + b + c + d + e.
End add.

Print add5.
