(load "1_31.scm")

(define mark
  (lambda (node depth)
    (if (leaf? node)
        (leaf depth)
        (let ((next-depth (if (eqv? (contents-of node) 'red) (+ depth 1) depth)))
          (interior-node (contents-of node)
                         (mark (lson node) next-depth)
                         (mark (rson node) next-depth))))))

(define mark-leaves-with-red-depth
  (lambda (tree)
    (mark (interior-node 'red (lson tree) (rson tree)) 0)))

(equal? (mark-leaves-with-red-depth
         (interior-node 'red
                        (interior-node 'bar
                                       (leaf 26)
                                       (leaf 12))
                        (interior-node 'red
                                       (leaf 11)
                                       (interior-node 'quux
                                                      (leaf 117)
                                                      (leaf 14)))))
        (list 'red
         (list 'bar 1 1)
         (list 'red 2 (list 'quux 2 2))))
