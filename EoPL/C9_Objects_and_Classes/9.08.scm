(require eopl)

; BEGIN: Scanner
(define scanner-spec
  '((white-sp (whitespace) skip)
    (comment ("%" (arbno (not #\newline))) skip)
    (identifier (letter (arbno (or letter digit "-" "?" "!"))) symbol)
    (number (digit (arbno digit)) number)
    (number ("-" digit (arbno digit)) number)))

(define scan&parse
  (sllgen:make-string-parser scanner-spec grammar))

; BEGIN: Program
(define-datatype program program?
  (a-program (class-decls (list-of class-decl?))
             (body expression?)))

(define (value-of-program pgm)
  (initialize-store!)
  (cases program pgm
    (a-program (class-decls body)
               (initialize-class-env! class-decls)
               (value-of body (empty-env)))))

(define (run src)
  (let ((pgm (scan&parse src)))
    (expval->val (value-of-program pgm))))

; BEGIN: Grammar
(define grammar
  '((program ((arbno class-decls) expression) a-program)
    (class-decls ("class" identifier "extends" identifier
                  (arbno "field" identifier) (arbno method-decls)) a-class-decl)
    (method-decls ("method" identifier "(" (separated-list identifier ",") ")" expression)
                  a-method-decl)
    (expression (number) const-exp)
    (expression (identifier) var-exp)
    (expression ("+" "(" expression "," expression ")") add-exp)
    (expression ("-" "(" expression "," expression ")") sub-exp)
    (expression ("zero?" "(" expression ")") zero?-exp)
    (expression ("if" expression "then" expression "else" expression) if-exp)
    (expression ("let" (arbno identifier "=" expression) "in" expression) let-exp)
    (expression ("cons" "(" expression "," expression ")") cons-exp)
    (expression ("car" "(" expression ")") car-exp)
    (expression ("cdr" "(" expression ")") cdr-exp)
    (expression ("null?" "(" expression ")") null?-exp)
    (expression ("emptylist") emptylist-exp)
    (expression ("list" "(" (separated-list expression ",") ")") list-exp)
    (expression ("newref" "(" expression ")") newref-exp)
    (expression ("deref" "(" expression ")") deref-exp)
    (expression ("setref" "(" expression "," expression ")") setref-exp)
    (expression ("begin" expression (arbno ";" expression) "end") begin-exp)
    (expression ("set" identifier "=" expression) set-exp)
    (expression ("proc" "(" (arbno identifier) ")" expression) proc-exp)
    (expression ("letrec" (arbno identifier "(" (arbno identifier) ")" "=" expression)
                 "in" expression)
                letrec-exp)
    (expression ("(" expression (arbno expression) ")") call-exp)
    (expression ("new" identifier "(" (separated-list expression ",") ")") new-object-exp)
    (expression ("send" expression identifier "(" (separated-list expression ",") ")")
                method-call-exp)
    (expression ("super" identifier "(" (separated-list expression ",") ")") super-call-exp)
    (expression ("instanceof" expression identifier) instanceof-exp)
    (expression ("fieldref" expression identifier) fieldref-exp)
    (expression ("fieldset" expression identifier "=" expression) fieldset-exp)
    (expression ("self") self-exp)))

; BEGIN: Value Type
(define identifier? symbol?)
(define reference? integer?)

(define-datatype expval expval?
  (num-val (num number?))
  (bool-val (bool boolean?))
  (proc-val (proc proc?))
  (list-val (lst list?))
  (obj-val (obj object?))
  (ref-val (ref reference?)))

(define (expval->num val)
  (cases expval val
    (num-val (num) num)
    (else (error 'num val))))

(define (expval->bool val)
  (cases expval val
    (bool-val (bool) bool)
    (else (error 'bool val))))

(define (expval->proc val)
  (cases expval val
    (proc-val (proc) proc)
    (else (error 'proc val))))

(define (expval->list val)
  (cases expval val
    (list-val (lst) lst)
    (else (error 'list val))))

(define (expval->obj val)
  (cases expval val
    (obj-val (obj) obj)
    (else (error 'obj val))))

(define (expval->ref val)
  (cases expval val
    (ref-val (ref) ref)
    (else (error 'ref val))))

(define (expval->val val)
  (cases expval val
    (num-val (num) num)
    (bool-val (bool) bool)
    (proc-val (proc) proc)
    (list-val (lst) (map expval->val lst))
    (obj-val (obj) obj)
    (ref-val (ref) (list 'ref ref))))

; BEGIN: Expression
(define-datatype expression expression?
  (const-exp (num number?))
  (var-exp (var identifier?))
  (add-exp (exp1 expression?)
           (exp2 expression?))
  (sub-exp (exp1 expression?)
           (exp2 expression?))
  (zero?-exp (exp expression?))
  (if-exp (cond expression?)
          (exp-t expression?)
          (exp-f expression?))
  (let-exp (vars (list-of identifier?))
           (exps (list-of expression?))
           (body expression?))
  (cons-exp (exp1 expression?)
            (exp2 expression?))
  (car-exp (exp expression?))
  (cdr-exp (exp expression?))
  (null?-exp (exp expression?))
  (emptylist-exp)
  (list-exp (exps (list-of expression?)))
  (newref-exp (exp expression?))
  (deref-exp (var expression?))
  (setref-exp (var expression?)
              (exp expression?))
  (begin-exp (exp expression?)
             (exps (list-of expression?)))
  (set-exp (var identifier?)
           (exp expression?))
  (proc-exp (vars (list-of identifier?))
            (body expression?))
  (letrec-exp (names (list-of identifier?))
              (varss (list-of (list-of identifier?)))
              (exps (list-of expression?))
              (body expression?))
  (call-exp (rator expression?)
            (rands (list-of expression?)))
  (new-object-exp (name identifier?)
                  (rands (list-of expression?)))
  (method-call-exp (exp expression?)
                   (name identifier?)
                   (rands (list-of expression?)))
  (super-call-exp (name identifier?)
                  (rands (list-of expression?)))
  (instanceof-exp (exp expression?)
                  (name identifier?))
  (fieldref-exp (obj-exp expression?)
                (name identifier?))
  (fieldset-exp (obj-exp expression?)
                (name identifier?)
                (val-exp expression?))
  (self-exp))

; BEGIN: Class Declaration
(define-datatype class-decl class-decl?
  (a-class-decl (class-name identifier?)
                (super-name identifier?)
                (field-names (list-of identifier?))
                (method-decls (list-of method-decl?))))

; BEGIN: Method Declaration
(define-datatype method-decl method-decl?
  (a-method-decl (method-name identifier?)
                 (vars (list-of identifier?))
                 (body expression?)))

; BEGIN: Object
(define-datatype object object?
  (an-object (class-name identifier?)
             (fields (list-of reference?))))

(define (object->class-name o)
  (cases object o
    (an-object (class-name fields) class-name)))

(define (object->fields o)
  (cases object o
    (an-object (class-name fields) fields)))

(define (new-object class-name)
  (an-object class-name
             (map (lambda (field-name)
                    (newref (list 'uninitialized-field field-name)))
                  (class->field-names (lookup-class class-name)))))

(define (lookup-field-ref obj name)
  (let ((names (class->field-names (lookup-class (object->class-name obj))))
        (refs (object->fields obj)))
    (define (lookup-rec names refs)
      (if (null? names)
          (error 'lookup-field-ref obj name)
          (if (eqv? name (car names))
              (car refs)
              (lookup-rec (cdr names) (cdr refs)))))
    (lookup-rec names refs)))

(define (fieldref obj name)
  (deref (lookup-field-ref obj name)))

(define (fieldset obj name val)
  (setref! (lookup-field-ref obj name) val))

; BEGIN: Method
(define-datatype method method?
  (a-method (vars (list-of identifier?))
            (body expression?)
            (super-name identifier?)
            (field-names (list-of identifier?))))

(define (apply-method m self args)
  (cases method m
    (a-method (vars body super-name field-names)
              (value-of body
                        (extend-env-vals
                         vars args
                         (extend-env-with-self-and-super
                          self super-name
                          (extend-env-refs field-names (object->fields self) (empty-env))))))))

; BEGIN: Class
(define-datatype class class?
  (a-class (super-name (maybe identifier?))
           (field-names (list-of identifier?))
           (method-env method-environment?)))

(define (class->super-name c)
  (cases class c
    (a-class (super-name field-names method-env) super-name)))

(define (class->field-names c)
  (cases class c
    (a-class (super-name field-names method-env) field-names)))

(define (class->method-env c)
  (cases class c
    (a-class (super-name field-names method-env) method-env)))

(define fresh-identifier
  (let ((sn 0))
    (lambda (identifier)
      (set! sn (+ sn 1))
      (string->symbol (string-append (symbol->string identifier)
                                     "%"
                                     (number->string sn))))))

(define (append-field-names super-fields new-fields)
  (if (null? super-fields)
      new-fields
      (cons (if (memq (car super-fields) new-fields)
                (fresh-identifier (car super-fields))
                (car super-fields))
            (append-field-names (cdr super-fields) new-fields))))

; BEGIN: Class Environment
(define the-class-env '())

(define (initialize-class-env! c-decls)
  (set! the-class-env (list (list 'object (a-class #f '() '()))))
  (for-each initialize-class-decl! c-decls))

(define (initialize-class-decl! c-decl)
  (cases class-decl c-decl
    (a-class-decl (c-name s-name f-names m-decls)
                  (let ((f-names (append-field-names (class->field-names (lookup-class s-name))
                                                     f-names)))
                    (add-to-class-env! c-name
                                       (a-class s-name f-names
                                                (merge-method-envs (class->method-env
                                                                    (lookup-class s-name))
                                                                   (method-decls->method-env
                                                                    m-decls s-name f-names))))))))

(define (add-to-class-env! class-name class)
  (set! the-class-env (cons (list class-name class)
                            the-class-env)))

(define (lookup-class name)
  (let ((maybe-pair (assq name the-class-env)))
    (if maybe-pair
        (cadr maybe-pair)
        (error 'lookup-class name))))

; BEGIN: Method Environment
(define (method-environment? x)
  ((list-of list?) x))

(define (find-method c-name name)
  (let ((m-env (class->method-env (lookup-class c-name))))
    (let ((maybe-pair (assq name m-env)))
      (if (pair? maybe-pair) (cadr maybe-pair)
          (error 'find-method name)))))

(define (method-decls->method-env m-decls super-name field-names)
  (map (lambda (m-decl)
         (cases method-decl m-decl
           (a-method-decl (method-name vars body)
                          (list method-name
                                (a-method vars body super-name field-names)))))
       m-decls))

(define (merge-method-envs super-m-env new-m-env)
  (append new-m-env super-m-env))

; BEGIN: Procedure
(define-datatype proc proc?
  (procedure (vars (list-of identifier?))
             (body expression?)
             (saved-env environment?)))

(define (apply-procedure rator vals)
  (cases proc rator
    (procedure (vars body saved-env)
               (value-of body (extend-env-vals vars vals saved-env)))))

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
  (extend-env-ref (var identifier?)
                  (ref reference?)
                  (saved-env environment?)))

(define (apply-env-ref env search-var)
  (cases environment env
    (empty-env ()
               (error 'apply-env-ref "Unbound variable." search-var))
    (extend-env-ref (var ref saved-env)
                    (if (eqv? search-var var)
                        ref
                        (apply-env-ref saved-env search-var)))))

(define (apply-env-val env var)
  (deref (apply-env-ref env var)))

(define (extend-env-val var val old-env)
  (extend-env-ref var (newref val) old-env))

(define (extend-env-vals vars vals env)
  (if (null? vars)
      env
      (extend-env-vals (cdr vars) (cdr vals) (extend-env-val (car vars) (car vals) env))))

(define (extend-env-refs vars refs env)
  (if (null? vars)
      env
      (extend-env-refs (cdr vars) (cdr refs) (extend-env-ref (car vars) (car refs) env))))

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

(define (extend-env-with-self-and-super self super-name env)
  (extend-env-val '%self self (extend-env-val '%super super-name env)))

; BEGIN: Evaluation
(define (last lst)
  (if (null? (cdr lst))
      (car lst)
      (last (cdr lst))))

(define (values-of-exps exps env)
  (map (lambda (exp) (value-of exp env)) exps))

(define (value-of exp env)
  (cases expression exp
    (const-exp (num)
               (num-val num))
    (var-exp (var)
             (apply-env-val env var))
    (add-exp (exp1 exp2)
             (let* ((val1 (value-of exp1 env))
                    (val2 (value-of exp2 env))
                    (num1 (expval->num val1))
                    (num2 (expval->num val2)))
                 (num-val (+ num1 num2))))
    (sub-exp (exp1 exp2)
             (let* ((val1 (value-of exp1 env))
                    (val2 (value-of exp2 env))
                    (num1 (expval->num val1))
                    (num2 (expval->num val2)))
                 (num-val (- num1 num2))))
    (zero?-exp (exp)
               (let* ((val (value-of exp env))
                      (num (expval->num val)))
                 (bool-val (zero? num))))
    (if-exp (cond exp-t exp-f)
            (let ((val (value-of cond env)))
              (if (expval->bool val)
                  (value-of exp-t env)
                  (value-of exp-f env))))
    (let-exp (vars exps body)
             (let ((vals (map (lambda (exp) (value-of exp env)) exps)))
               (value-of body (extend-env-vals vars vals env))))
    (cons-exp (exp1 exp2)
              (let ((val1 (value-of exp1 env))
                    (val2 (value-of exp2 env)))
                (let ((lst2 (expval->list val2)))
                  (list-val (cons val1 lst2)))))
    (car-exp (exp1)
             (let ((val1 (value-of exp1 env)))
               (let ((lst1 (expval->list val1)))
                 (car lst1))))
    (cdr-exp (exp1)
             (let ((val1 (value-of exp1 env)))
               (let ((lst1 (expval->list val1)))
                 (list-val (cdr lst1)))))
    (null?-exp (exp1)
               (let ((val1 (value-of exp1 env)))
                 (let ((lst1 (expval->list val1)))
                   (bool-val (null? lst1)))))
    (emptylist-exp ()
                   (list-val '()))
    (list-exp (exps)
              (list-val (values-of-exps exps env)))
    (newref-exp (exp)
                (ref-val (newref (value-of exp env))))
    (deref-exp (var)
               (let ((ref (expval->ref (value-of var env))))
                 (deref ref)))
    (setref-exp (var exp)
                (let ((ref (expval->ref (value-of var env)))
                      (val (value-of exp env)))
                  (setref! ref val)))
    (begin-exp (exp exps)
               (last (values-of-exps (cons exp exps) env)))
    (set-exp (var exp)
             (let ((ref (apply-env-ref env var))
                   (val (value-of exp env)))
               (setref! ref val)
               val))
    (proc-exp (vars body)
              (proc-val (procedure vars body env)))
    (letrec-exp (names varss exps body)
                (value-of body (extend-env-rec names varss exps env)))
    (call-exp (rator rands)
              (let ((proc (expval->proc (value-of rator env)))
                    (args (values-of-exps rands env)))
                (apply-procedure proc args)))
    (new-object-exp (class-name rands)
                    (let ((args (values-of-exps rands env))
                          (obj (new-object class-name)))
                      (apply-method (find-method class-name 'initialize)
                                    obj
                                    args)
                      obj))
    (method-call-exp (obj-exp method-name rands)
                     (let ((args (values-of-exps rands env))
                           (obj (value-of obj-exp env)))
                       (apply-method (find-method (object->class-name obj)
                                                  method-name)
                                     obj
                                     args)))
    (super-call-exp (method-name rands)
                    (let ((args (values-of-exps rands env))
                          (obj (apply-env-val env '%self)))
                      (apply-method (find-method (apply-env-val env '%super)
                                                 method-name)
                                    obj
                                    args)))
    (instanceof-exp (exp name)
                    (let ((obj (value-of exp env)))
                      (define (instanceof-rec n)
                        (if n
                            (if (eqv? n name)
                                #t
                                (instanceof-rec (class->super-name (lookup-class n))))
                            #f))
                      (bool-val (instanceof-rec (object->class-name obj)))))
    (fieldref-exp (obj-exp name)
                  (let ((obj (value-of obj-exp env)))
                    (fieldref obj name)))
    (fieldset-exp (obj-exp name val-exp)
                  (let ((obj (value-of obj-exp env))
                        (val (value-of val-exp env)))
                    (fieldset obj name val)))
    (self-exp ()
              (apply-env-val env '%self))))

; BEGIN: Test
(define (equal?! prog expect)
  (let ((actual (run prog)))
    (display "Expect: ")
    (display expect)
    (display "\nActual: ")
    (display actual)
    (display "\n")
    (if (equal? actual expect)
        (display "\n")
        (display "Wrong Answer!!\n\n"))))

(define prog-9-8 "
class c extends object
  field x
  method initialize()
      set x = 1

let o = new c()
in begin
     fieldset o x = 2;
     fieldref o x
   end
")
(equal?! prog-9-8 2)
