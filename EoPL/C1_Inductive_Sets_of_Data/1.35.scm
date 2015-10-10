(load "1_31.scm")

(define number-leaves-rev
  (lambda (node num)
    (if (leaf? node)
        (list num num)
        (let ((left-ret (number-leaves-rev (lson node) num)))
          (let ((right-ret (number-leaves-rev (rson node) (+ (car left-ret) 1))))
            (list (car right-ret)
                  (car node)
                  (if (null? (cddr left-ret))
                      (cadr left-ret)
                      (cdr left-ret))
                  (if (null? (cddr right-ret))
                      (cadr right-ret)
                      (cdr right-ret))))))))

(define number-leaves
  (lambda (tree)
    (cdr (number-leaves-rev tree 0))))

(equal? (number-leaves
         (interior-node 'foo
                        (interior-node 'bar
                                       (leaf 26)
                                       (leaf 12))
                        (interior-node 'baz
                                       (leaf 11)
                                       (interior-node 'quux
                                                      (leaf 117)
                                                      (leaf 14)))))
        '(foo
          (bar 0 1)
          (baz
           2
           (quux 3 4))))
