fun maxl lst = 
    if null lst then
        raise Empty
    else 
        let val n = hd lst
            val ns = tl lst
        in
            if null ns then
                n
            else
                let val m = hd ns
                    val ms = tl ns
                in 
                    if n > m then
                        maxl (n::ms)
                    else
                        maxl (m::ms)
                end
            end;
                     
maxl [1, 3, 5, 4, 2];
maxl []
