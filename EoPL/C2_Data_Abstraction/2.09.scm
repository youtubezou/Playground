(load "2.05.scm")

(define has-binding?
  (lambda (env s)
    (if (null? env)
        #f
        (if (eqv? s (caar env))
            #t
            (has-binding? (cdr env) s)))))