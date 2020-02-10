// hash table implementation of <string, 'T>
type HashTable<'T> =
    | Empty
    | Data of string * 'T
    | Table of HashTable<'T>[]

let createHashTable<'T>() : HashTable<'T> =
    Table <| Array.map (fun _ -> HashTable<'T>.Empty) [|'a' .. 'z'|]

let rec insert(table: HashTable<'T>, name: string, i: int, data: 'T) =
    if i >= name.Length then Empty
    else
        match table with
        | Empty -> HashTable<'T>.Data(name, data)
        | Data(s, d) ->
            if i >= s.Length
        | Table(t) ->
            let ch = name.[i]
            if not <| System.Char.IsLetter(ch) then
                failwithf "Char '%c' is not a letter\n" ch
            let ci = int ch
            let s = if System.Char.IsLower(ch) then ci - 97 else ci - 65

            match t.[s] with
            | Empty -> t.[s] <- HashTable<'T>.Data(name, data) ; Table(t)
            | Data(s, d) -> ()
            | Table(t) -> ()
