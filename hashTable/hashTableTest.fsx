(*
 create empty:
 --

 insert "abe"
 -- |a: abe

 insert "banana"
 -- |a: abe
    |b: banana

 insert "appelsin"
 -- |a: |b: abe
        |p: appelsin
    |b: banana

 insert "ananas"
 -- |a: |b: abe
        |n: ananas
        |p: appelsin
    |b: banana

 insert "citron"
 -- |a: |b: abe
        |n: ananas
        |p: appelsin
    |b: banana
    |c: citron

 insert "clementine"
 -- |a: |b: abe
        |n: ananas
        |p: appelsin
    |b: banana
    |c: |i: citron
        |l: clementine

 insert "clarity"
 -- |a: |b: abe
        |n: ananas
        |p: appelsin
    |b: banana
    |c: |i: citron
        |l: |a: clarity
            |e: clementine

 *)
