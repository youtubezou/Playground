fun nth ((l::ls), 0) = l
  | nth ((l::ls), n) = nth (ls, n - 1);

nth ([1, 2, 3], 0);
nth ([1, 2, 3], 1);
