(require eopl)

(define scanner-spec
  '((white-sp (whitespace) skip)
    (comment ("%" (arbno (not #\newline))) skip)
    (identifier (letter (arbno (or letter digit))) symbol)
    (number (digit (arbno digit)) number)))

(define scan&parse
  (sllgen:make-string-parser scanner-spec grammar))

(define report-expval-extractor-error
  (lambda (type val)
    (error type "Extraction type error: " val)))

(define-datatype program program?
  (a-program
   (exp1 expression?)))

(define translation-of-program
  (lambda (pgm)
    (cases program pgm
      (a-program (exp1)
                 (a-program (translation-of exp1 (empty-senv)))))))

(define run
  (lambda (string)
    (car
     (value-of-program
      (translation-of-program
       (a-program
        (scan&parse string)))))))
  
(define (value-of-program pgm)
  (cases program pgm
    (a-program (exp)
               (value-of exp (empty-env) (empty-store)))))

(define (identifier? x)
  (and (symbol? x)
       (not (eqv? x 'lambda))))

(define (reference? v)
  (integer? v))
  
(define (empty-store)
  '())

(define (newref store val)
  (let* ((next-ref (length store))
         (next-store (append store (list val))))
    (list next-ref next-store)))

(define (deref store ref)
  (list-ref store ref))

(define (setref store ref val)
  (define (setref-rec store-rec ref-rec)
    (cond ((null? store-rec) (error 'setref "Invalid reference: " ref store))
          ((zero? ref-rec) (cons val (cdr store-rec)))
          (else (cons (car store-rec)
                      (setref-rec (cdr store-rec)
                                  (- ref-rec 1))))))
  (setref-rec store ref))

(define-datatype expval expval?
  (num-val
   (num number?))
  (bool-val
   (bool boolean?))
  (list-val
   (lst list?))
  (proc-val
   (proc proc?))
  (ref-val
   (ref reference?)))

(define expval->num
  (lambda (val)
    (cases expval val
      (num-val (num) num)
      (else (report-expval-extractor-error 'num val)))))

(define expval->bool
  (lambda (val)
    (cases expval val
      (bool-val (bool) bool)
      (else (report-expval-extractor-error 'bool val)))))

(define expval->list
  (lambda (val)
    (cases expval val
      (list-val (lst) lst)
      (else (report-expval-extractor-error 'list val)))))

(define expval->proc
  (lambda (val)
    (cases expval val
      (proc-val (proc) proc)
      (else (report-expval-extractor-error 'proc val)))))

(define expval->ref
  (lambda (val)
    (cases expval val
      (ref-val (ref) ref)
      (else (report-expval-extractor-error 'ref val)))))

(define expval->val
  (lambda (val)
    (cases expval val
      (num-val (num) num)
      (bool-val (bool) bool)
      (list-val (lst) (map expval->val lst))
      (proc-val (proc) proc)
      (ref-val (ref) ref))))

(define empty-senv
  (lambda ()
    '()))

(define extend-senv
  (lambda (vars senv)
    (cons (list vars 'value) senv)))

(define extend-senv-letrec
  (lambda (vars senv)
    (cons (list vars 'letrec) senv)))

(define apply-senv
  (lambda (senv var)
    (define (apply-senv-sub pos vars)
      (if (null? vars)
          (let ((result (apply-senv (cdr senv) var)))
            (cons (+ (car result) 1) (cdr result)))
          (if (eqv? (car vars) var)
              (list 0 pos (cadar senv))
              (apply-senv-sub (+ pos 1) (cdr vars)))))
    (if (null? senv)
        (error 'apply-senv "Unbound variable: " var)
        (apply-senv-sub 0 (caar senv)))))

(define environment?
  (lambda (x)
    ((list-of (list-of expval?)) x)))

(define empty-env
  (lambda ()
    '()))

(define extend-env
  (lambda (vals env)
    (cons vals env)))

(define apply-env
  (lambda (env n pos)
    (list-ref (list-ref env n) pos)))

(define restore-env
  (lambda (env n)
    (if (zero? n)
        env
        (restore-env (cdr env) (- n 1)))))

(define-datatype proc proc?
  (procedure
   (body expression?)
   (saved-env environment?)))

(define (apply-procedure proc1 vals store)
  (cases proc proc1
    (procedure (body saved-env)
               (value-of body (extend-env vals saved-env) store))))

(define grammar
  '((expression (number) const-exp)
    (expression (identifier) var-exp)
    (expression ("+" "(" expression "," expression ")") add-exp)
    (expression ("-" "(" expression "," expression ")") sub-exp)
    (expression ("*" "(" expression "," expression ")") mul-exp)
    (expression ("/" "(" expression "," expression ")") div-exp)
    (expression ("minus" "(" expression ")") minus-exp)
    (expression ("zero?" "(" expression ")") zero?-exp)
    (expression ("equal?" "(" expression "," expression ")") equal?-exp)
    (expression ("less?" "(" expression "," expression ")") less?-exp)
    (expression ("greater?" "(" expression "," expression ")") greater?-exp)
    (expression ("if" expression "then" expression "else" expression) if-exp)
    (expression ("cond" (arbno expression "==>" expression) "end") cond-exp)
    (expression ("cons" "(" expression "," expression ")") cons-exp)
    (expression ("car" "(" expression ")") car-exp)
    (expression ("cdr" "(" expression ")") cdr-exp)
    (expression ("list" "(" (arbno expression) ")") list-exp)
    (expression ("null?" "(" expression ")") null?-exp)
    (expression ("emptylist") emptylist-exp)
    (expression ("unpack" (arbno identifier) "=" expression "in" expression) unpack-exp)
    (expression ("let" (arbno identifier "=" expression) "in" expression) let-exp)
    (expression ("proc" "(" (arbno identifier) ")" expression) proc-exp)
    (expression ("letrec" (arbno identifier "(" (arbno identifier) ")" "=" expression )
                 "in" expression) letrec-exp)
    (expression ("newref" "(" expression ")") newref-exp)
    (expression ("deref" "(" expression ")") deref-exp)
    (expression ("setref" "(" expression "," expression ")") setref-exp)
    (expression ("begin" expression (arbno ";" expression) "end") begin-exp)
    (expression ("(" expression (arbno expression) ")") call-exp)))

(define (nameless-var-exp? var)
  (integer? var))

(define-datatype expression expression?
  (const-exp
   (num number?))
  (var-exp
   (var identifier?))
  (nameless-var-exp
   (n nameless-var-exp?)
   (pos nameless-var-exp?))
  (add-exp
   (exp1 expression?)
   (exp2 expression?))
  (sub-exp
   (exp1 expression?)
   (exp2 expression?))
  (mul-exp
   (exp1 expression?)
   (exp2 expression?))
  (div-exp
   (exp1 expression?)
   (exp2 expression?))
  (minus-exp
   (exp expression?))
  (zero?-exp
   (exp expression?))
  (equal?-exp
   (exp1 expression?)
   (exp2 expression?))
  (less?-exp
   (exp1 expression?)
   (exp2 expression?))
  (greater?-exp
   (exp1 expression?)
   (exp2 expression?))
  (if-exp
   (exp1 expression?)
   (exp2 expression?)
   (exp3 expression?))
  (cond-exp
   (conds (list-of expression?))
   (exps (list-of expression?)))
  (cons-exp
   (exp1 expression?)
   (exp2 expression?))
  (car-exp
   (exp expression?))
  (cdr-exp
   (exp expression?))
  (list-exp
   (exps (list-of expression?)))
  (null?-exp
   (exp expression?))
  (emptylist-exp)
  (unpack-exp
   (vars (list-of identifier?))
   (exp1 expression?)
   (exp2 expression?))
  (nameless-unpack-exp
   (exp1 expression?)
   (exp2 expression?))
  (let-exp
   (vars (list-of identifier?))
   (exps (list-of expression?))
   (body expression?))
  (nameless-let-exp
   (exps (list-of expression?))
   (body expression?))
  (proc-exp
   (vars (list-of identifier?))
   (body expression?))
  (letrec-exp
   (names (list-of identifier?))
   (varss (list-of (list-of identifier?)))
   (exps (list-of expression?))
   (body expression?))
  (nameless-letrec-exp
   (exps (list-of expression?))
   (body expression?))
  (nameless-letrec-var-exp
   (n nameless-var-exp?)
   (pos nameless-var-exp?))
  (nameless-proc-exp
   (body expression?))
  (newref-exp
   (exp expression?))
  (deref-exp
   (var expression?))
  (nameless-deref-exp
   (var expression?))
  (setref-exp
   (var expression?)
   (exp expression?))
  (nameless-setref-exp
   (var expression?)
   (exp expression?))
  (begin-exp
    (exp expression?)
    (exps (list-of expression?)))
  (call-exp
   (rator expression?)
   (rands (list-of expression?))))

(define translation-of
  (lambda (exp senv)
    (let ((trans-list (lambda (exp) (translation-of exp senv))))
      (cases expression exp
        (const-exp (num) (const-exp num))
        (var-exp (var)
                 (let ((result (apply-senv senv var)))
                   (if (eqv? (caddr result) 'letrec)
                       (nameless-letrec-var-exp (car result) (cadr result))
                       (nameless-var-exp (car result) (cadr result)))))
        (add-exp (exp1 exp2)
                 (add-exp (translation-of exp1 senv)
                          (translation-of exp2 senv)))
        (sub-exp (exp1 exp2)
                 (sub-exp (translation-of exp1 senv)
                          (translation-of exp2 senv)))
        (mul-exp (exp1 exp2)
                 (mul-exp (translation-of exp1 senv)
                          (translation-of exp2 senv)))
        (div-exp (exp1 exp2)
                 (div-exp (translation-of exp1 senv)
                          (translation-of exp2 senv)))
        (minus-exp (exp)
                   (minus-exp (translation-of exp senv)))
        (zero?-exp (exp)
                   (zero?-exp (translation-of exp senv)))
        (equal?-exp (exp1 exp2)
                    (equal?-exp (translation-of exp1 senv)
                                (translation-of exp2 senv)))
        (less?-exp (exp1 exp2)
                   (less?-exp (translation-of exp1 senv)
                              (translation-of exp2 senv)))
        (greater?-exp (exp1 exp2)
                      (greater?-exp (translation-of exp1 senv)
                                    (translation-of exp2 senv)))
        (if-exp (exp1 exp2 exp3)
                (if-exp (translation-of exp1 senv)
                        (translation-of exp2 senv)
                        (translation-of exp3 senv)))
        (cond-exp (conds exps)
                  (cond-exp (map trans-list conds)
                            (map trans-list exps)))
        (cons-exp (exp1 exp2)
                  (cons-exp (translation-of exp1 senv)
                            (translation-of exp2 senv)))
        (car-exp (exp)
                 (car-exp (translation-of exp senv)))
        (cdr-exp (exp)
                 (cdr-exp (translation-of exp senv)))
        (list-exp (exps)
                  (list-exp (map trans-list exps)))
        (null?-exp (exp)
                   (null?-exp (translation-of exp senv)))
        (emptylist-exp () (emptylist-exp))
        (unpack-exp (vars exp1 exp2)
                    (let ((trans1 (translation-of exp1 senv)))
                      (define (extend-senv-rec vars senv)
                        (if (null? vars)
                            senv
                            (extend-senv-rec (cdr vars) (extend-senv (car vars) senv))))
                      (let ((trans2 (translation-of exp2 (extend-senv-rec vars senv))))
                        (nameless-unpack-exp trans1 trans2))))
        (let-exp (vars exps body)
                 (nameless-let-exp (map trans-list exps)
                                   (translation-of body (extend-senv vars senv))))
        (proc-exp (vars body)
                  (nameless-proc-exp (translation-of body (extend-senv vars senv))))
        (letrec-exp (names varss exps body)
                    (let ((named-env (extend-senv-letrec names senv)))
                      (define (exp-rec varss exps)
                        (if (null? exps)
                            '()
                            (cons (translation-of (car exps) (extend-senv (car varss) named-env))
                                  (exp-rec (cdr varss) (cdr exps)))))
                      (nameless-letrec-exp (exp-rec varss exps)
                                           (translation-of body named-env))))
        (call-exp (rator rands)
                  (call-exp
                   (translation-of rator senv)
                   (map trans-list rands)))
        (newref-exp (exp)
                    (newref-exp (translation-of exp senv)))
        (deref-exp (var)
                   (nameless-deref-exp (translation-of var senv)))
        (setref-exp (var exp)
                    (nameless-setref-exp (translation-of var senv)
                                         (translation-of exp senv)))
        (begin-exp (exp exps)
                   (begin-exp (translation-of exp senv)
                              (map trans-list exps)))
        (else (error 'translation-of "Invalid expression: " exp))))))

(define (get-val value-of-result)
  (car value-of-result))

(define (get-store value-of-result)
  (cadr value-of-result))

(define (value-of-two-num extract construct op exp1 exp2 env store)
  (let* ((ret1 (value-of exp1 env store))
         (val1 (get-val ret1))
         (num1 (extract val1))
         (store1 (get-store ret1))
         (ret2 (value-of exp2 env store1))
         (val2 (get-val ret2))
         (num2 (extract val2))
         (store2 (get-store ret2)))
    (list (construct (op num1 num2)) store2)))

(define (value-of-list exps env store)
  (if (null? exps)
      (list '() store)
      (let* ((ret-head (value-of (car exps) env store))
             (val-head (get-val ret-head))
             (store-head (get-store ret-head))
             (ret-rest (value-of-list (cdr exps) env store-head))
             (val-rest (get-val ret-rest))
             (store-rest (get-store ret-rest)))
        (list (cons val-head val-rest) store-rest))))

(define (last l)
  (cond ((null? (cdr l)) (car l))
        (else (last (cdr l)))))

(define value-of
  (lambda (exp env store)
    (cases expression exp
      (const-exp (num) (list (num-val num) store))
      (add-exp (exp1 exp2) (value-of-two-num expval->num num-val + exp1 exp2 env store))
      (sub-exp (exp1 exp2) (value-of-two-num expval->num num-val - exp1 exp2 env store))
      (mul-exp (exp1 exp2) (value-of-two-num expval->num num-val * exp1 exp2 env store))
      (div-exp (exp1 exp2) (value-of-two-num expval->num num-val quotient exp1 exp2 env store))
      (minus-exp (exp)
                 (let* ((ret (value-of exp env store))
                        (val (get-val ret))
                        (num (expval->num val))
                        (next-store (get-store ret)))
                   (list (num-val (- num)) next-store)))
      (zero?-exp (exp)
                 (let* ((ret (value-of exp env store))
                        (val (get-val ret))
                        (num (expval->num val))
                        (next-store (get-store ret)))
                   (list (bool-val (zero? num)) next-store)))
      (equal?-exp (exp1 exp2) (value-of-two-num expval->num bool-val = exp1 exp2 env store))
      (less?-exp (exp1 exp2) (value-of-two-num expval->num bool-val < exp1 exp2 env store))
      (greater?-exp (exp1 exp2) (value-of-two-num expval->num bool-val > exp1 exp2 env store))
      (if-exp (exp1 exp2 exp3)
              (let* ((ret1 (value-of exp1 env store))
                     (val1 (get-val ret1))
                     (store1 (get-store ret1)))
                (if (expval->bool val1)
                    (value-of exp2 env store1)
                    (value-of exp3 env store1))))
      (cond-exp (conds exps)
                (define (cond-exp-rec conds exps store)
                  (if (null? conds)
                      (error 'cond "No condition matched.")
                      (let* ((ret1 (value-of (car conds) env store))
                             (val1 (get-val ret1))
                             (store1 (get-store ret1)))
                        (if (expval->bool val1)
                            (value-of (car exps) env store1)
                            (cond-exp-rec (cdr conds) (cdr exps) store1)))))
                (cond-exp-rec conds exps store))
      (cons-exp (exp1 exp2)
                (let* ((ret1 (value-of exp1 env store))
                       (val1 (get-val ret1))
                       (store1 (get-store ret1))
                       (ret2 (value-of exp1 env store1))
                       (val2 (get-val ret2))
                       (store2 (get-store ret2)))
                  (list (list-val (cons val1
                                        (expval->list val2))) store2)))
      (car-exp (exp)
               (let* ((ret (value-of exp env store))
                      (val (get-val ret))
                      (next-store (get-store ret)))
                 (list (car (expval->list val)) next-store)))
      (cdr-exp (exp)
               (let* ((ret (value-of exp env store))
                      (val (get-val ret))
                      (next-store (get-store ret)))
                 (list (list-val (cdr (expval->list val))) next-store)))
      (list-exp (exps)
                (let* ((rets (value-of-list exps env store))
                       (vals (get-val rets))
                       (next-store (get-store rets)))
                  (list (list-val vals) next-store)))
      (null?-exp (exp)
                 (let* ((ret (value-of exp env store))
                        (val (get-val ret))
                        (lst (expval->list val))
                        (next-store (get-store ret)))
                   (list (bool-val (null? lst)) next-store)))
      (emptylist-exp ()
                     (list (list-val '()) store))
      (nameless-unpack-exp (exp1 exp2)
                (let* ((ret1 (value-of exp1 env store))
                       (val1 (get-val ret1))
                       (lst1 (expval->list val1))
                       (store1 (get-store ret1)))
                  (value-of exp2 (extend-env lst1 env) store1)))
      (call-exp (rator rands)
                (let* ((ret-rator (value-of rator env store))
                       (val-rator (get-val ret-rator))
                       (store-rator (get-store ret-rator))
                       (proc (expval->proc val-rator))
                       (ret-rand (value-of-list rands env store-rator))
                       (vals (get-val ret-rand))
                       (store-end (get-store ret-rand)))
                  (apply-procedure proc vals store-end)))
      (nameless-var-exp (n pos)
                        (list (apply-env env n pos) store))
      (nameless-let-exp (exps body)
                        (let* ((rets (value-of-list exps env store))
                               (vals (get-val rets))
                               (next-store (get-store rets)))
                          (value-of body (extend-env vals env) next-store)))
      (nameless-proc-exp (body)
                         (list (proc-val (procedure body env))) store)
      (nameless-letrec-exp (exps body)
                           (let ((procs (map (lambda (exp)
                                               (proc-val (procedure exp env)))
                                             exps)))
                             (value-of body (extend-env procs env) store)))
      (nameless-letrec-var-exp (n pos)
                               (let ((env (restore-env env n)))
                                 (let ((proc1 (expval->proc (apply-env env n pos))))
                                   (cases proc proc1
                                     (procedure (body saved-env)
                                                (list (proc-val (procedure body env)) store))))))
      (newref-exp (exp)
                  (let* ((ret (value-of exp env store))
                         (val (get-val ret))
                         (next-store (get-store ret))
                         (ret-ref (newref next-store val))
                         (ref (get-val ret-ref))
                         (end-store (get-store ret-ref)))
                    (list (ref-val ref) end-store)))
      (nameless-deref-exp (var)
                          (let* ((ret (value-of var env store))
                                 (val (get-val ret))
                                 (next-store (get-store ret))
                                 (ref (expval->ref val)))
                            (list (deref store ref) next-store)))
      (nameless-setref-exp (var exp)
                           (let* ((ret1 (value-of var env store))
                                  (val1 (get-val ret1))
                                  (ref (expval->ref val1))
                                  (store1 (get-store ret1))
                                  (ret2 (value-of exp env store1))
                                  (val2 (get-val ret2))
                                  (store2 (get-store ret2))
                                  (next-store (setref store2 ref val2)))
                             (list val2 next-store)))
      (begin-exp (exp exps)
                 (let* ((rets (value-of-list (cons exp exps) env store))
                        (vals (get-val rets))
                        (next-store (get-store rets)))
                   (list (last vals) next-store)))
      (else (error 'value-of "Invalid expression: " exp)))))

(define (equal?! actual expect)
  (if (equal? actual expect)
      (display "Passed\n")
      (begin
        (display "Actual: ")
        (display actual)
        (display " Expect: ")
        (display expect)
        (display "\n"))))

(define program-op "*(-(2, 1), +(/(10, 5), 1))")
(equal?! (expval->val (run program-op)) 3)

(define program-3
  "let x = newref(newref(0))
   in begin
        setref(deref(x), 11);
        deref(deref(x))
      end")
(equal?! (expval->val (run program-3)) 11)

(define program-4
  "let x = newref(0)
   in let y = list(setref(x, 1) deref(x))
      in car(cdr(y))")
(equal?! (expval->val (run program-4)) 1)
