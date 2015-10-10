(load "1_31.scm")

(define double-tree
  (lambda (node)
    (if (leaf? node)
        (+ node node)
        (interior-node (contents-of node)
                       (double-tree (lson node))
                       (double-tree (rson node))))))

(equal? (double-tree (list 'a (list 'b 3 (list 'c 4 1)) 2))
        (list 'a (list 'b 6 (list 'c 8 2)) 4))
