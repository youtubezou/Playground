signature ARITH =
    sig
        type t
        val zero : t
        val sum : t * t -> t
        val diff : t * t -> t
        val prod : t * t -> t
        val quo : t * t -> t
    end;

structure Real : ARITH =
    struct
        type t = real
        val zero = 0.0
        val sum = Real.+
        val diff = Real.-
        val prod = Real.*
        val quo = Real./
    end;
