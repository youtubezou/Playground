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

Lemma imp_perm : (P -> Q -> R) -> (Q -> P -> R).
Proof.
    intros X q p.
    apply X.
    assumption.
    assumption.
Qed.
Print imp_perm.

Lemma ignore_Q : (P -> R) -> P -> Q -> R.
Proof.
    intros X p q.
    apply X.
    assumption.
Qed.
Print ignore_Q.

Lemma delta_imp : (P -> P -> Q) -> P -> Q.
Proof.
    intros X p.
    apply X.
    assumption.
    assumption.
Qed.
Print delta_imp.

Lemma delta_impR : (P -> Q) -> (P -> P -> Q).
Proof.
    intros X p.
    assumption.
Qed.
Print delta_impR.

Lemma diamond : (P -> Q) -> (P -> R) -> (Q -> R -> T) -> P -> T.
Proof.
    intros X Y Z p.
    apply Z.
    apply X.
    assumption.
    apply Y.
    assumption.
Qed.
Print diamond.

Lemma weak_peirce : ((((P -> Q) -> P) -> P) -> Q) -> Q.
Proof.
    intros X.
    apply X.
    intros Y.
    apply Y.
    intros p.
    apply X.
    intros Z.
    assumption.
Qed.
Print weak_peirce.

End Section_03_02.
