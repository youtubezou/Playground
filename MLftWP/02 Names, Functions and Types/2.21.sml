fun introot n = 
    if n = 0
        then (0, 0)
    else let val n' = n div 4
             val (k, d) = introot n'
             val a = 4 * (n' - d)
             val b = a + 4 * k + 1
         in if b > n then 
                (2 * k, n - a)
            else
                (2 * k + 1, n - b)
         end;

introot(99);
