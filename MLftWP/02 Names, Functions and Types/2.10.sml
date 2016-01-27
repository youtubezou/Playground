(*
powoftwo(8) => (8=1) orelse (even(8) andalso ...)
            => even(8) andalso powoftwo(8 div 2)
            => powoftwo(4)
            => powoftwo(2)
            => powoftwo(1)
            => (1=1) orelse (even(1) andalso ...)
            => true            
*)
