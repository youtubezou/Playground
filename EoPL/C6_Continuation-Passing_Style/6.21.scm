(define (cps-of-call-exp rator rands k-exp)
  (cps-of-exps (append (reverse rands) (list rator))
               (lambda (simples)
                 (let ((exps (reverse simples)))
                   (cps-call-exp (car exps)
                                 (append (cdr exps) (list k-exp)))))))
