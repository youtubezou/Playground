Section Section_03_02.

Variables P Q R T : Prop.

Lemma id_P : P -> P.
Proof.
    intro p.
    assumption.
Qed.
Print id_P.

Lemma id_PP : (P -> P) -> (P -> P).
Proof.
    intro S.
    assumption.
Qed.
Print id_PP.

Lemma imp_trans : (P -> Q) -> (Q -> R) -> P -> R.
Proof.
    intros X Y p.
    apply Y.
    apply X.
    assumption.
Qed.
Print imp_trans.

End Section_03_02.
