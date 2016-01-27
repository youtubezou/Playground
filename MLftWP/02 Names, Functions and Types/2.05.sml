fun valid_date (d, m) = 1 <= d andalso (
        (m = "January" andalso d <= 31) orelse
        (m = "February" andalso d <= 28) orelse
        (m = "March" andalso d <= 31) orelse
        (m = "April" andalso d <= 30) orelse
        (m = "May" andalso d <= 31) orelse
        (m = "June" andalso d <= 30) orelse
        (m = "July" andalso d <= 31) orelse
        (m = "August" andalso d <= 31) orelse
        (m = "September" andalso d <= 30) orelse
        (m = "October" andalso d <= 31) orelse
        (m = "November" andalso d <= 30) orelse
        (m = "December" andalso d <= 31));
