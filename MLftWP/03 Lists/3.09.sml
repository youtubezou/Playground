fun zip ([], _) = []
  | zip (_, []) = []
  | zip (x::xs, y::ys) = (x, y) :: zip (xs, ys);

zip([1, 2], [3, 4, 5]);
zip([1, 2, 3], [4, 5]);
