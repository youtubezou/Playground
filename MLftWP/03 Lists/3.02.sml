fun last [] = raise Empty
  | last [x] = x
  | last (n::m::s) = last (m::s);

last [1, 2, 3];
last [];
