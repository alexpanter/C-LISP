module msort

let rec merge = function
    | [], []                  -> []
    | x::xs, []               -> x :: merge (xs, [])
    | [], y::ys               -> y :: merge (ys, [])
    | x::xs, y::ys when x < y -> x :: merge (xs, (y::ys))
    | x::xs, y::ys            -> y :: merge ((x::xs), ys)

let rec split = function
    | [], xs, ys         -> (xs, ys)
    | l::[], xs, ys      -> (l::xs, ys)
    | l::ls::lss, xs, ys -> split (lss, (l::xs), (ls::ys))

let rec mergeSort lst =
     if List.length lst < 2 then lst
     else let (a, b) = split (lst, [], []) in merge (mergeSort a, mergeSort b)
