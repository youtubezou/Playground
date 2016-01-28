fun fib n =
    let fun itfib (n, prev, curr) =
            if n = 1 then
                curr
            else
                itfib(n - 1, curr, prev + curr)
    in itfib(n, 0, 1)
    end;
    
fib(1);
fib(2);
fib(3);
fib(10);
