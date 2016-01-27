fun digit i = String.sub("0123456789", i);
str (digit ~1);  (* Exception *)
str (digit 10);  (* Exception *)
