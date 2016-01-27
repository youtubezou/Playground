fun odd n = n mod 2 = 1;
fun even n = n mod 2 = 0;

fun gcd(m, n) = 
    if m = n then 
        m
    else if even m andalso even n then
        2 * gcd(m div 2, n div 2)
    else if even m andalso odd n then
        gcd(m div 2, n)
    else if odd m andalso even n then
        gcd(m, n div 2)
    else if m < n then
        gcd((n - m) div 2, m)
    else
        gcd(n, m);
        
gcd(36, 6);
gcd(6, 36);
