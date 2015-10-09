(require eopl)

; BEGIN: Scanner
(define scanner-spec
  '((white-sp (whitespace) skip)
    (comment ("%" (arbno (not #\newline))) skip)
    (identifier (letter (arbno (or letter digit))) symbol)
    (number (digit (arbno digit)) number)
    (number ("-" digit (arbno digit)) number)))

(define scan&parse
  (sllgen:make-string-parser scanner-spec grammar))

(define-datatype program program?
  (a-program
   (stat statement?)))

(define (value-of-program pgm)
  (cases program pgm
    (a-program (exp)
               (result-of/k exp (empty-env) (end-main-thread-cont)))))

(define (run prog)
  (initialize-store!)
  (initialize-scheduler! timeslice)
  (value-of-program (scan&parse prog)))

; BEGIN: Value type
(define (identifier? x)
  (symbol? x))

(define (reference? v)
  (integer? v))

(define-datatype expval expval?
  (num-val
   (num number?))
  (bool-val
   (bool boolean?))
  (list-val
   (lst list?))
  (proc-val
   (proc proc?))
  (cont-val
   (cont continuation?))
  (mutex-val
   (mut mutex?))
  (ref-val
   (ref reference?)))

(define (expval->num val)
  (cases expval val
    (num-val (num) num)
    (else (error 'num val))))

(define (expval->bool val)
  (cases expval val
    (bool-val (bool) bool)
    (else (error 'bool val))))

(define (expval->list val)
  (cases expval val
    (list-val (lst) lst)
    (else (error 'list val))))

(define (expval->proc val)
  (cases expval val
    (proc-val (proc) proc)
    (else (error 'proc val))))

(define (expval->cont val)
  (cases expval val
    (cont-val (cont) cont)
    (else (error 'cont val))))

(define (expval->mutex val)
  (cases expval val
    (mutex-val (mutex) mutex)
    (else (error 'mutex val))))

(define (expval->ref val)
  (cases expval val
    (ref-val (ref) ref)
    (else (error 'ref val))))

(define (expval->val val)
    (cases expval val
      (num-val (num) num)
      (bool-val (bool) bool)
      (list-val (lst)
                (map (lambda (val) (expval->val val)) lst))
      (proc-val (proc) proc)
      (cont-val (cont) cont)
      (mutex-val (mutex) 'mutex)
      (ref-val (ref) (list 'ref ref))))

; BEGIN: Mutex
(define-datatype mutex mutex?
  (a-mutex
   (ref-to-closed? reference?)
   (ref-to-wait-queue reference?)))

(define (new-mutex)
  (a-mutex (newref #f)
           (newref (empty-queue))))

(define (wait-for-mutex m th)
  (cases mutex m
    (a-mutex (ref-to-closed? ref-to-wait-queue)
             (cond
               ((deref ref-to-closed?) (setref! ref-to-wait-queue
                                                (enqueue (deref ref-to-wait-queue) th))
                                       (run-next-thread))
               (else (setref! ref-to-closed? #t)
                     (run-thread th))))))

(define (signal-mutex m th)
  (cases mutex m
    (a-mutex (ref-to-closed? ref-to-wait-queue)
             (let ((closed? (deref ref-to-closed?))
                   (wait-queue (deref ref-to-wait-queue)))
               (if closed?
                   (if (empty? wait-queue)
                       (setref! ref-to-closed? #f)
                       (dequeue wait-queue
                                (lambda (first-waiting-th other-waiting-ths)
                                  (place-on-ready-queue! first-waiting-th)
                                  (setref! ref-to-wait-queue
                                           other-waiting-ths))))
                   0)
               (run-thread th)))))

; BEGIN: Store
(define (store? x)
  (list-of x))

(define (empty-store)
  (make-vector 0))

(define the-store 'uninitialized)

(define (get-store) the-store)

(define (initialize-store!)
  (set! the-store (empty-store)))

(define (newref val)
  (let* ((next-ref (vector-length the-store))
         (next-store (make-vector (+ next-ref 1) val)))
    (define (newref-rec idx)
      (if (equal? idx next-ref)
          0
          (begin (vector-set! next-store idx (vector-ref the-store idx))
                 (newref-rec (+ idx 1)))))
    (newref-rec 0)
    (set! the-store next-store)
    next-ref))

(define (deref ref)
  (vector-ref the-store ref))

(define (setref! ref val)
  (vector-set! the-store ref val)
  ref)

; BEGIN: Environment
(define-datatype environment environment?
  (empty-env)
  (extend-env-ref
   (var identifier?)
   (ref reference?)
   (old-env environment?)))

(define (apply-env-ref env search-var)
  (cases environment env
    (empty-env ()
               (error 'apply-env-ref "Unbound variable." search-var))
    (extend-env-ref (var ref old-env)
                    (if (eqv? search-var var)
                        ref
                        (apply-env-ref old-env search-var)))))

(define (apply-env-val env var)
  (deref (apply-env-ref env var)))

(define (extend-env-val var val old-env)
  (extend-env-ref var (newref val) old-env))

(define (extend-env-vals vars vals env)
  (if (null? vars)
      env
      (extend-env-vals (cdr vars) (cdr vals) (extend-env-val (car vars) (car vals) env))))

(define (extend-env-rec names varss exps old-env)
  (let ((next-env (extend-env-vals names
                                   (build-list (length names) (lambda (x) 'undefined))
                                   old-env)))
    (define (extend-env-rec-sub names varss exps)
      (if (null? names)
          next-env
          (let* ((ref (apply-env-ref next-env (car names)))
                 (proc (procedure (car varss) (car exps) next-env)))
            (setref! ref (proc-val proc))
            (extend-env-rec-sub (cdr names) (cdr varss) (cdr exps)))))
    (extend-env-rec-sub names varss exps)))

(define (extend-env-sub name vars stat old-env)
  (let* ((next-env (extend-env-val name 'undefined old-env))
         (ref (apply-env-ref next-env name))
         (sub (subroutine vars stat next-env)))
    (setref! ref (proc-val sub))
    next-env))

; BEGIN: Grammar
(define grammar
  '((program (statement) a-program)
    (expression (number) const-exp)
    (expression (identifier) var-exp)
    (expression ("+" "(" expression "," expression ")") add-exp)
    (expression ("-" "(" expression "," expression ")") sub-exp)
    (expression ("*" "(" expression "," expression ")") mul-exp)
    (expression ("/" "(" expression "," expression ")") div-exp)
    (expression ("zero?" "(" expression ")") zero?-exp)
    (expression ("equal?" "(" expression "," expression ")") equal?-exp)
    (expression ("less?" "(" expression "," expression ")") less?-exp)
    (expression ("greater?" "(" expression "," expression ")") greater?-exp)
    (expression ("if" expression "then" expression "else" expression) if-exp)
    (expression ("cons" "(" expression "," expression ")") cons-exp)
    (expression ("car" "(" expression ")") car-exp)
    (expression ("cdr" "(" expression ")") cdr-exp)
    (expression ("list" "(" (arbno expression) ")") list-exp)
    (expression ("null?" "(" expression ")") null?-exp)
    (expression ("emptylist") emptylist-exp)
    (expression ("let" (arbno identifier "=" expression) "in" expression) let-exp)
    (expression ("letrec" (arbno identifier "(" (arbno identifier) ")" "=" expression)
                          "in" expression) letrec-exp)
    (expression ("ref" identifier) ref-exp)
    (expression ("newref" "(" expression ")") newref-exp)
    (expression ("deref" "(" expression ")") deref-exp)
    (expression ("setref" "(" expression "," expression ")") setref-exp)
    (expression ("set" identifier "=" expression) set-exp)
    (expression ("print" "(" expression ")") print-exp)
    (expression ("begin" (separated-list expression ";") "end") begin-exp)
    (expression ("proc" "(" (arbno identifier) ")" expression) proc-exp)
    (expression ("(" expression (arbno expression) ")") call-exp)
    (expression ("try" expression "catch" "(" identifier ")" expression) try-exp)
    (expression ("raise" expression) raise-exp)
    (expression ("letcc" identifier "in" expression) letcc-exp)
    (expression ("throw" expression "to" expression) throw-exp)
    (expression ("spawn" "(" expression ")") spawn-exp)
    (expression ("mutex" "(" ")") mutex-exp)
    (expression ("wait" "(" expression ")") wait-exp)
    (expression ("signal" "(" expression ")") signal-exp)
    (expression ("yield") yield-exp)
    (statement (identifier "=" expression) assign-stat)
    (statement ("print" expression) print-stat)
    (statement ("read" identifier) read-stat)
    (statement ("{" (separated-list statement ";") "}") seq-stat)
    (statement ("if" expression statement statement) if-stat)
    (statement ("while" expression statement) while-stat)
    (statement ("do" statement "while" expression) dowhile-stat)
    (statement ("sub" identifier "(" (arbno identifier) ")" statement "end" statement) sub-stat)
    (statement ("var" (separated-list identifier ",") ";" statement) var-stat)))

; BEGIN: Expression
(define-datatype expression expression?
  (const-exp (num number?))
  (var-exp (var identifier?))
  (add-exp (exp1 expression?)
           (exp2 expression?))
  (sub-exp (exp1 expression?)
           (exp2 expression?))
  (mul-exp (exp1 expression?)
           (exp2 expression?))
  (div-exp (exp1 expression?)
           (exp2 expression?))
  (zero?-exp (exp expression?))
  (equal?-exp (exp1 expression?)
              (exp2 expression?))
  (less?-exp (exp1 expression?)
             (exp2 expression?))
  (greater?-exp (exp1 expression?)
                (exp2 expression?))
  (if-exp (cond expression?)
          (exp-t expression?)
          (exp-f expression?))
  (cons-exp (exp1 expression?)
            (exp2 expression?))
  (car-exp (exp expression?))
  (cdr-exp (exp expression?))
  (list-exp (exps (list-of expression?)))
  (null?-exp (exp expression?))
  (emptylist-exp)
  (let-exp (vars (list-of identifier?))
           (exps (list-of expression?))
           (body expression?))
  (letrec-exp (names (list-of identifier?))
              (varss (list-of (list-of identifier?)))
              (exps (list-of expression?))
              (body expression?))
  (ref-exp (var identifier?))
  (newref-exp (exp expression?))
  (deref-exp (var expression?))
  (setref-exp (var expression?)
              (exp expression?))
  (set-exp (var identifier?)
           (exp expression?))
  (begin-exp (exps (list-of expression?)))
  (print-exp (exp expression?))
  (proc-exp (vars (list-of identifier?))
            (body expression?))
  (call-exp (rator expression?)
            (rands (list-of expression?)))
  (try-exp (exp expression?)
           (var identifier?)
           (handler expression?))
  (raise-exp (exp expression?))
  (letcc-exp (var identifier?)
             (exp expression?))
  (throw-exp (exp1 expression?)
             (exp2 expression?))
  (spawn-exp (exp expression?))
  (mutex-exp)
  (wait-exp (exp expression?))
  (signal-exp (exp expression?))
  (yield-exp))

; BEGIN: Statement
(define-datatype statement statement?
  (assign-stat (var identifier?)
               (exp expression?))
  (print-stat (exp expression?))
  (read-stat (var identifier?))
  (seq-stat (stat (list-of statement?)))
  (if-stat (cond expression?)
           (stat-t statement?)
           (stat-f statement?))
  (while-stat (cond expression?)
              (stat statement?))
  (dowhile-stat (stat statement?)
                (cond expression?))
  (sub-stat (name identifier?)
            (vars (list-of identifier?))
            (stat1 statement?)
            (stat2 statement?))
  (var-stat (vars (list-of identifier?))
            (stat statement?)))

; BEGIN: Continuation
(define-datatype continuation continuation?
  (dual-1-cont (op procedure?)
               (exp2 expression?)
               (env environment?)
               (saved-cont continuation?))
  (dual-2-cont (op procedure?)
               (val1 expval?)
               (saved-cont continuation?))
  (div-1-cont (exp2 expression?)
              (env environment?)
              (saved-cont continuation?))
  (div-2-cont (val1 expval?)
              (saved-cont continuation?))
  (compare-1-cont (op procedure?)
                  (exp2 expression?)
                  (env environment?)
                  (saved-cont continuation?))
  (compare-2-cont (op procedure?)
                  (val1 expval?)
                  (saved-cont continuation?))
  (zero?-cont (saved-cont continuation?))
  (if-cont (exp-t expression?)
           (exp-f expression?)
           (env environment?)
           (saved-cont continuation?))
  (cons-1-cont (exp2 expression?)
               (env environment?)
               (saved-cont continuation?))
  (cons-2-cont (val1 expval?)
               (saved-cont continuation?))
  (car-cont (saved-cont continuation?))
  (cdr-cont (saved-cont continuation?))
  (list-cont (exps (list-of expression?))
             (vals (list-of expval?))
             (env environment?)
             (saved-cont continuation?))
  (null?-cont (saved-cont continuation?))
  (let-cont (vars (list-of identifier?))
            (exps (list-of expression?))
            (vals (list-of expval?))
            (body expression?)
            (env environment?)
            (saved-cont continuation?))
  (newref-cont (saved-cont continuation?))
  (deref-cont (saved-cont continuation?))
  (setref-1-cont (exp expression?)
                 (env environment?)
                 (saved-cont continuation?))
  (setref-2-cont (var expval?)
                 (saved-cont continuation?))
  (set-cont (ref reference?)
            (saved-cont continuation?))
  (print-cont (saved-cont continuation?))
  (begin-cont (exps (list-of expression?))
              (env environment?)
              (saved-cont continuation?))
  (rator-cont (rands (list-of expression?))
              (env environment?)
              (saved-cont continuation?))
  (rands-cont (rator expval?)
              (rands (list-of expression?))
              (vals (list-of expval?))
              (env environment?)
              (saved-cont continuation?))
  (try-cont (var identifier?)
            (handler expression?)
            (env environment?)
            (saved-cont continuation?))
  (raise-cont (saved-cont continuation?))
  (throw-1-cont (exp2 expression?)
                (env environment?)
                (saved-cont continuation?))
  (throw-2-cont (val1 expval?)
                (saved-cont continuation?))
  (spawn-cont (saved-cont continuation?))
  (wait-cont (saved-cont continuation?))
  (signal-cont (saved-cont continuation?))
  (assign-stat-cont (ref reference?)
                    (saved-cont continuation?))
  (print-stat-cont (saved-cont continuation?))
  (seq-stat-cont (stats (list-of statement?))
                 (env environment?)
                 (saved-cont continuation?))
  (if-stat-cont (stat-t statement?)
                (stat-f statement?)
                (env environment?)
                (saved-cont continuation?))
  (while-1-stat-cont (exp expression?)
                     (stat statement?)
                     (env environment?)
                     (saved-cont continuation?))
  (while-2-stat-cont (exp expression?)
                     (stat statement?)
                     (env environment?)
                     (saved-cont continuation?))
  (end-main-thread-cont)
  (end-subthread-cont))

(define (apply-cont cont val)
  (if (time-expired?)
      (begin (place-on-ready-queue! (new-thread (lambda () (apply-cont cont val))))
             (run-next-thread))
      (begin (decrement-timer!)
             (cases continuation cont
               (dual-1-cont (op exp2 env saved-cont)
                            (let ((val1 val))
                              (value-of/k exp2 env (dual-2-cont op val1 saved-cont))))
               (dual-2-cont (op val1 saved-cont)
                            (let ((val2 val))
                              (let ((num1 (expval->num val1))
                                    (num2 (expval->num val2)))
                                (apply-cont saved-cont (num-val (op num1 num2))))))
               (div-1-cont (exp2 env saved-cont)
                           (let ((val1 val))
                             (value-of/k exp2 env (div-2-cont val1 saved-cont))))
               (div-2-cont (val1 saved-cont)
                           (let ((val2 val))
                             (let ((num1 (expval->num val1))
                                   (num2 (expval->num val2)))
                               (if (zero? num2)
                                   (begin (display "Divison by zero.\n")
                                          (apply-handler 235 saved-cont))
                                   (apply-cont saved-cont (num-val (quotient num1 num2)))))))
               (compare-1-cont (op exp2 env saved-cont)
                               (let ((val1 val))
                                 (value-of/k exp2 env (compare-2-cont op val1 saved-cont))))
               (compare-2-cont (op val1 saved-cont)
                               (let ((val2 val))
                                 (let ((num1 (expval->num val1))
                                       (num2 (expval->num val2)))
                                   (apply-cont saved-cont (bool-val (op num1 num2))))))
               (zero?-cont (saved-cont)
                           (apply-cont saved-cont (bool-val (zero? (expval->num val)))))
               (if-cont (exp-t exp-f env saved-cont)
                        (if (expval->bool val)
                            (value-of/k exp-t env saved-cont)
                            (value-of/k exp-f env saved-cont)))
               (cons-1-cont (exp2 env saved-cont)
                            (let ((val1 val))
                              (value-of/k exp2 env (cons-2-cont val1 saved-cont))))
               (cons-2-cont (val1 saved-cont)
                            (let ((val2 val))
                              (apply-cont saved-cont (list-val (cons val1 (expval->list val2))))))
               (car-cont (saved-cont)
                         (apply-cont saved-cont (car (expval->list val))))
               (cdr-cont (saved-cont)
                         (apply-cont saved-cont (list-val (cdr (expval->list val)))))
               (list-cont (exps vals env saved-cont)
                          (let ((next-val (cons val vals)))
                            (if (null? exps)
                                (apply-cont saved-cont (list-val (reverse next-val)))
                                (value-of/k (car exps) env (list-cont (cdr exps) next-val env saved-cont)))))
               (null?-cont (saved-cont)
                           (apply-cont saved-cont (bool-val (null? (expval->list val)))))
               (let-cont (vars exps vals body env saved-cont)
                         (let ((next-val (cons val vals)))
                           (if (null? exps)
                               (value-of/k body (extend-env-vals vars (reverse next-val) env) saved-cont)
                               (value-of/k (car exps) env (let-cont vars (cdr exps)
                                                                    next-val
                                                                    body env saved-cont)))))
               (newref-cont (saved-cont)
                            (let ((ref (newref val)))
                              (apply-cont saved-cont (ref-val ref))))
               (deref-cont (saved-cont)
                           (let ((real-val (deref (expval->ref val))))
                             (apply-cont saved-cont real-val)))
               (set-cont (ref saved-cont)
                         (setref! ref val)
                         (apply-cont saved-cont val))
               (setref-1-cont (exp env saved-cont)
                              (value-of/k exp env (setref-2-cont val saved-cont)))
               (setref-2-cont (var saved-cont)
                              (begin (setref! (expval->ref var) val)
                                     (apply-cont saved-cont val)))
               (print-cont (saved-cont)
                           (display (expval->val val))
                           (display "\n")
                           (apply-cont saved-cont val))
               (begin-cont (exps env saved-cont)
                           (if (null? exps)
                               (apply-cont saved-cont val)
                               (value-of/k (car exps) env (begin-cont (cdr exps) env saved-cont))))
               (rator-cont (rands env saved-cont)
                           (if (null? rands)
                               (apply-procedure/k (expval->proc val) '() saved-cont)
                               (value-of/k (car rands) env (rands-cont val (cdr rands) '() env saved-cont))))
               (rands-cont (rator rands vals env saved-cont)
                           (let ((next-val (cons val vals)))
                             (if (null? rands)
                                 (apply-procedure/k (expval->proc rator) (reverse next-val) saved-cont)
                                 (value-of/k (car rands) env (rands-cont rator (cdr rands)
                                                                         next-val env saved-cont)))))
               (try-cont (var handler env saved-cont)
                         (apply-cont saved-cont val))
               (raise-cont (saved-cont)
                           (apply-handler val saved-cont))
               (throw-1-cont (exp2 env saved-cont)
                             (value-of/k exp2 env (throw-2-cont val saved-cont)))
               (throw-2-cont (val1 saved-cont)
                             (cases expval val
                               (cont-val (cont)
                                         (apply-cont saved-cont (apply-cont cont val1)))
                               (else saved-cont val)))
               (spawn-cont (saved-cont)
                           (let ((proc (expval->proc val)))
                             (place-on-ready-queue!
                              (new-thread (lambda ()
                                            (apply-procedure/k proc '() (end-subthread-cont)))))
                             (apply-cont saved-cont val)))
               (wait-cont (saved-cont)
                          (wait-for-mutex (expval->mutex val)
                                          (new-thread (lambda () (apply-cont saved-cont val)))))
               (signal-cont (saved-cont)
                            (signal-mutex (expval->mutex val)
                                          (new-thread (lambda () (apply-cont saved-cont val)))))
               (assign-stat-cont (ref saved-cont)
                                 (setref! ref val)
                                 (apply-cont saved-cont val))
               (print-stat-cont (saved-cont)
                                (display (expval->val val))
                                (display "\n")
                                (apply-cont saved-cont val))
               (seq-stat-cont (stats env saved-cont)
                              (if (null? stats)
                                  (apply-cont saved-cont val)
                                  (result-of/k (car stats) env (seq-stat-cont (cdr stats) env saved-cont))))
               (if-stat-cont (stat-t stat-f env saved-cont)
                             (if (expval->bool val)
                                 (result-of/k stat-t env saved-cont)
                                 (result-of/k stat-f env saved-cont)))
               (while-1-stat-cont (exp stat env saved-cont)
                                  (if (expval->bool val)
                                      (result-of/k stat env (while-2-stat-cont exp stat env saved-cont))
                                      (apply-cont saved-cont val)))
               (while-2-stat-cont (exp stat env saved-cont)
                                  (value-of/k exp env (while-1-stat-cont exp stat env saved-cont)))
               (end-main-thread-cont ()
                                     (set-final-answer! val)
                                     (run-next-thread))
               (end-subthread-cont ()
                                   (run-next-thread))))))

; BEGIN: Procedure
(define-datatype proc proc?
  (procedure (vars (list-of identifier?))
             (body expression?)
             (saved-env environment?))
  (subroutine (vars (list-of identifier?))
              (body statement?)
              (saved-env environment?)))

(define (apply-procedure/k proc1 vals cont)
  (cases proc proc1
    (procedure (vars body saved-env)
               (if (equal? (length vars) (length vals))
                   (value-of/k body (extend-env-vals vars vals saved-env) cont)
                   (begin (display "Wrong number of arguments\n")
                          (apply-handler 777 cont))))
    (subroutine (vars body saved-env)
                (if (equal? (length vars) (length vals))
                    (result-of/k body (extend-env-vals vars vals saved-env) cont)
                    (begin (display "Wrong number of arguments\n")
                           (apply-handler 999 cont))))))

; BEGIN: Handler
(define (apply-handler val cont)
  (cases continuation cont
    (try-cont (var handler env saved-cont)
              (value-of/k handler (extend-env-val var val env) saved-cont))
    (dual-1-cont (op exp2 env saved-cont)
                 (apply-handler val saved-cont))
    (dual-2-cont (op val1 saved-cont)
                 (apply-handler val saved-cont))
    (div-1-cont (exp2 env saved-cont)
                 (apply-handler val saved-cont))
    (div-2-cont (val1 saved-cont)
                 (apply-handler val saved-cont))
    (compare-1-cont (op exp2 env saved-cont)
                    (apply-handler val saved-cont))
    (compare-2-cont (op val1 saved-cont)
                    (apply-handler val saved-cont))
    (zero?-cont (saved-cont)
                (apply-handler val saved-cont))
    (if-cont (exp-t exp-f env saved-cont)
             (apply-handler val saved-cont))
    (cons-1-cont (exp2 env saved-cont)
                 (apply-handler val saved-cont))
    (cons-2-cont (val1 saved-cont)
                 (apply-handler val saved-cont))
    (car-cont (saved-cont)
              (apply-handler val saved-cont))
    (cdr-cont (saved-cont)
              (apply-handler val saved-cont))
    (list-cont (exps vals env saved-cont)
               (apply-handler val saved-cont))
    (null?-cont (saved-cont)
                (apply-handler val saved-cont))
    (let-cont (vars exps vals body env saved-cont)
              (apply-handler val saved-cont))
    (newref-cont (saved-cont)
                 (apply-handler val saved-cont))
    (deref-cont (saved-cont)
                (apply-handler val saved-cont))
    (set-cont (ref saved-cont)
              (apply-handler val saved-cont))
    (setref-1-cont (exp env saved-cont)
                   (apply-handler val saved-cont))
    (setref-2-cont (var saved-cont)
                   (apply-handler val saved-cont))
    (print-cont (saved-cont)
                (apply-handler val saved-cont))
    (begin-cont (exps env saved-cont)
                (apply-handler val saved-cont))
    (rator-cont (rands env saved-cont)
                (apply-handler val saved-cont))
    (rands-cont (rator rands vals env saved-cont)
                (apply-handler val saved-cont))
    (raise-cont (saved-cont)
                (apply-handler val saved-cont))
    (throw-1-cont (exp2 env saved-cont)
                  (apply-handler val saved-cont))
    (throw-2-cont (val saved-cont)
                  (apply-handler val saved-cont))
    (spawn-cont (saved-cont)
                (apply-handler val saved-cont))
    (wait-cont (saved-cont)
               (apply-handler val saved-cont))
    (signal-cont (saved-cont)
                 (apply-handler val saved-cont))
    (assign-stat-cont (ref saved-cont)
                      (apply-handler val saved-cont))
    (print-stat-cont (saved-cont)
                     (apply-handler val saved-cont))
    (seq-stat-cont (stats env saved-cont)
                   (apply-handler val saved-cont))
    (if-stat-cont (stat-t stat-f env saved-cont)
                  (apply-handler val saved-cont))
    (while-1-stat-cont (exp stat env saved-cont)
                       (apply-handler val saved-cont))
    (while-2-stat-cont (exp stat env saved-cont)
                       (apply-handler val saved-cont))
    (end-main-thread-cont ()
                          (error 'apply-handler "Uncaught exception" val))
    (end-subthread-cont ()
                        (error 'apply-handler "Uncaught exception" val))))

; BEGIN: Queue
(define (empty-queue)
  '())

(define (empty-queue? q)
  (null? q)) 

(define (enqueue q val)
  (if (null? q)
      (list val)
      (cons (car q)
            (enqueue (cdr q) val))))

(define (dequeue q proc)
  (proc (car q) (cdr q)))

; BEGIN: Thread
(define-datatype thread thread?
  (a-thread (proc procedure?)
            (time-remaining integer?)))

(define (new-thread proc)
  (a-thread proc the-max-time-slice))

(define (restore-thread-time t)
  (cases thread t
    (a-thread (proc time-remaining)
              (new-thread proc))))

(define (run-thread t)
  (cases thread t
    (a-thread (proc time-remaining)
              (set! the-time-remaining time-remaining)
              (proc))))

; BEGIN: Scheduler
(define timeslice 300)
(define the-ready-queue 'uninitialized)
(define the-final-answer 'uninitialized)
(define the-max-time-slice 'uninitialized)
(define the-time-remaining 'uninitialized)

(define (initialize-scheduler! ticks)
  (set! the-ready-queue (empty-queue))
  (set! the-final-answer 'uninitialized)
  (set! the-max-time-slice ticks)
  (set! the-time-remaining ticks))

(define (place-on-ready-queue! th)
  (set! the-ready-queue (enqueue the-ready-queue th)))

(define (run-next-thread)
  (if (empty-queue? the-ready-queue)
      the-final-answer
      (dequeue the-ready-queue
               (lambda (first-ready-thread other-ready-threads)
                 (set! the-ready-queue other-ready-threads)
                 (set! the-time-remaining the-max-time-slice)
                 (run-thread first-ready-thread)))))

(define (set-final-answer! val)
  (set! the-final-answer val))

(define (time-expired?)
  (zero? the-time-remaining))

(define (decrement-timer!)
  (set! the-time-remaining (- the-time-remaining 1)))

; BEGIN: Evaluation
(define (value-of/k exp env cont)
  (cases expression exp
    (const-exp (num)
               (apply-cont cont (num-val num)))
    (var-exp (var)
             (apply-cont cont (apply-env-val env var)))
    (add-exp (exp1 exp2)
             (value-of/k exp1 env (dual-1-cont + exp2 env cont)))
    (sub-exp (exp1 exp2)
             (value-of/k exp1 env (dual-1-cont - exp2 env cont)))
    (mul-exp (exp1 exp2)
             (value-of/k exp1 env (dual-1-cont * exp2 env cont)))
    (div-exp (exp1 exp2)
             (value-of/k exp1 env (div-1-cont exp2 env cont)))
    (zero?-exp (exp)
               (value-of/k exp env (zero?-cont cont)))
    (equal?-exp (exp1 exp2)
                (value-of/k exp1 env (compare-1-cont = exp2 env cont)))
    (less?-exp (exp1 exp2)
               (value-of/k exp1 env (compare-1-cont < exp2 env  cont)))
    (greater?-exp (exp1 exp2)
                  (value-of/k exp1 env (compare-1-cont > exp2 env  cont)))
    (if-exp (cond exp-t exp-f)
            (value-of/k cond env (if-cont exp-t exp-f env cont)))
    (cons-exp (exp1 exp2)
              (value-of/k exp1 env (cons-1-cont exp2 env cont)))
    (car-exp (exp)
             (value-of/k exp env (car-cont cont)))
    (cdr-exp (exp)
             (value-of/k exp env (cdr-cont cont)))
    (list-exp (exps)
              (if (null? exps)
                  (apply-cont cont (list-val '()))
                  (value-of/k (car exps) env (list-cont (cdr exps) '() env cont))))
    (null?-exp (exp)
               (value-of/k exp env (null?-cont cont)))
    (emptylist-exp ()
                   (apply-cont cont (list-val '())))
    (let-exp (vars exps body)
             (if (null? vars)
                 (value-of/k body env cont)
                 (value-of/k (car exps) env (let-cont vars (cdr exps) '() body env cont))))
    (letrec-exp (names varss exps body)
                (value-of/k body (extend-env-rec names varss exps env) cont))
    (ref-exp (var)
             (apply-cont cont (ref-val (apply-env-ref env var))))
    (newref-exp (exp)
                (value-of/k exp env (newref-cont cont)))
    (deref-exp (var)
               (value-of/k var env (deref-cont cont)))
    (setref-exp (var exp)
                (value-of/k var env (setref-1-cont exp env cont)))
    (set-exp (var exp)
             (value-of/k exp env (set-cont (apply-env-ref env var) cont)))
    (print-exp (exp)
               (value-of/k exp env (print-cont cont)))
    (begin-exp (exps)
               (apply-cont (begin-cont exps env cont) (num-val 0)))
    (proc-exp (vars body)
              (apply-cont cont (proc-val (procedure vars body env))))
    (call-exp (rator rands)
              (value-of/k rator env (rator-cont rands env cont)))
    (try-exp (exp var handler)
             (value-of/k exp env (try-cont var handler env cont)))
    (raise-exp (exp)
               (value-of/k exp env (raise-cont cont)))
    (letcc-exp (var exp)
               (value-of/k exp (extend-env-val var (cont-val cont) env) cont))
    (throw-exp (exp1 exp2)
               (value-of/k exp1 env (throw-1-cont exp2 env cont)))
    (spawn-exp (exp)
               (value-of/k exp env (spawn-cont cont)))
    (mutex-exp ()
               (apply-cont cont (mutex-val (new-mutex))))
    (wait-exp (exp)
              (value-of/k exp env (wait-cont cont)))
    (signal-exp (exp)
                (value-of/k exp env (signal-cont cont)))
    (yield-exp ()
               (place-on-ready-queue! (a-thread (lambda () (apply-cont cont (num-val 99)))
                                                the-time-remaining))
               (run-next-thread))))

(define (result-of/k stat env cont)
  (cases statement stat
    (assign-stat (var exp)
                 (value-of/k exp env (assign-stat-cont (apply-env-ref env var) cont)))
    (print-stat (exp)
                (value-of/k exp env (print-stat-cont cont)))
    (read-stat (exp)
               (apply-cont cont (num-val (read))))
    (seq-stat (stats)
              (apply-cont (seq-stat-cont stats env cont) (num-val 0)))
    (if-stat (cond stat-t stat-f)
             (value-of/k cond env (if-stat-cont stat-t stat-f env cont)))
    (while-stat (exp stat)
                (value-of/k exp env (while-1-stat-cont exp stat env cont)))
    (dowhile-stat (stat exp)
                  (result-of/k stat env (while-2-stat-cont exp stat env cont)))
    (sub-stat (name vars stat1 stat2)
              (result-of/k stat2 (extend-env-sub name vars stat1 env) cont))
    (var-stat (vars stat)
              (result-of/k stat (extend-env-vals vars
                                                 (make-list (length vars) (lambda (x) 'undefined))
                                                 env) cont))))

; BEGIN: Tests
(define (test prog expect)
  (display "Expect: \n")
  (if (list? expect)
      (map (lambda (x)
             (display x)
             (display "\n")) expect)
      (begin (display expect)
             (display "\n")))
  (display "Actual: \n")
  (run prog)
  (display "\n"))

(define program-thread-1 "var dummy; { dummy =
letrec noisy (l) = if null?(l)
                   then 0
                   else begin
                            print(car(l));
                            (noisy cdr(l))
                        end
in begin
       spawn(proc () (noisy list(1 2 3 4 5))) ;
       spawn(proc () (noisy list(6 7 8 9 10))) ;
       print(100);
       33
   end
}")
(test program-thread-1 '())

(define program-thread-2 "var dummy; { dummy =
let buffer = 0
in let producer = proc (n)
           letrec pwait(k) = if zero?(k)
                            then set buffer = n
                            else begin
                                print(-(k,-200));
                                (pwait -(k,1))
                            end
           in (pwait 5)
   in let consumer = proc (d)
              letrec busywait (k) = if zero?(buffer)
                                    then begin
                                             print(-(k,-100));
                                             (busywait -(k,-1))
                                         end
                                    else buffer
              in (busywait 0)
      in begin
             spawn(proc () (producer 44));
             print(300);
             (consumer 86)
         end
}")
(test program-thread-2 '())

(define program-mutex "var dummy; { dummy =
let x = 0
in let mut = mutex()
   in let incrx = proc ()
                      begin
                          wait(mut);
                          set x = -(x,-1);
                          print(x);
                          signal(mut)
                      end
      in begin
             spawn(incrx);
             spawn(incrx);
             spawn(incrx)
         end
}")
(test program-mutex '(1 2 3))

(define program-yield "var dummy; { dummy =
let x = 0
in let mut = mutex()
   in let incrx = proc ()
                      begin
                          wait(mut);
                          set x = -(x,-1);
                          signal(mut);
                          yield;
                          print(x)
                      end
      in begin
             spawn(incrx);
             spawn(incrx);
             spawn(incrx)
         end
}")
(test program-yield '(3 3 3))
