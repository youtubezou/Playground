signature ARITH =
    sig
        type t
        val zero : t
        val sum : t * t -> t
        val diff : t * t -> t
        val prod : t * t -> t
        val quo : t * t -> t
    end;

structure Rational : ARITH =
    struct
        type t = int * int
        
        fun gcd (a, b) = if b = 0 then a else gcd (b, a mod b)
        val zero = (0, 1)
        fun reduce (a, b) = 
            let val aa = abs a
                val ab = abs b
                val g = gcd (aa, ab)
                val pos = (a < 0 andalso b < 0)
            in if (a < 0 andalso b < 0) orelse (a >= 0 andalso b >= 0) then
                   (aa div g, ab div g)
               else
                   (~ (aa div g), ab div g)
            end
        fun sum ((a, b), (c, d)) = reduce (a * d + b * c, b * d)
        fun diff (a, (b, c)) = sum (a, (~b, c))
        fun prod ((a, b), (c, d)) = reduce (a * c, b * d)
        fun quo (a, (b, c)) = prod (a, (c, b))
    end;
    
Rational.sum((1, 3), (1, 4));
Rational.diff((1, 3), (1, 4));
Rational.prod((1, 3), (3, 4));
Rational.quo((1, 3), (3, 4));
