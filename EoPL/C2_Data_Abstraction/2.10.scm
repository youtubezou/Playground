(load "2.05.scm")

(define extend-env*
  (lambda (env var-list val-list)
    (if (null? var-list)
        env
        (extend-env* (extend-env (car var-list) (car val-list) env)
                     (cdr var-list) (cdr val-list)))))
