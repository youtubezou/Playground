fun to_minute (hours, minutes, m) = 
    if m = "PM" then 
        to_minute(hours + 12, minutes, "AM")
    else
        hours * 60 + minutes;

fun comes_before(a, b) = to_minute(a) < to_minute(b);

comes_before((11, 59, "AM"), (1, 15, "PM"));
comes_before((1, 15, "PM"), (11, 59, "AM"));
