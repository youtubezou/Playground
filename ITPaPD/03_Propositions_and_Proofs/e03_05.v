Section section_for_cut_example.

    Variables P Q R T : Prop.

    Hypotheses (H : P -> Q)
               (H0 : Q -> R)
               (H1 : (P -> R) -> T -> Q)
               (H2 : (P -> R) -> T).

    Theorem cut_example : Q.
    Proof.
        cut (P -> R).
        intro H3.
        apply H1;  [assumption | apply H2; assumption].
        intro; apply H0; apply H; assumption.
    Qed.

    Print cut_example.

    Theorem cut_example' : Q.
    Proof.
        apply H1.
        intro p.
        apply H0.
        apply H.
        assumption.
        apply H2.
        intro p.
        apply H0.
        apply H.
        assumption.
    Qed.

    Print cut_example'.

End section_for_cut_example.
