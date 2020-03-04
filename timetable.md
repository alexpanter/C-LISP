
## Before optimizations ##

(define a 5)
execution in 0.100324 ms.
execution in 0.093761 ms.
execution in 0.078376 ms.
execution in 0.085919 ms.
execution in 0.081254 ms.
average: 0.0879268 ms.


(+ "Hello, " "world!")
execution in 0.059735 ms.
execution in 0.041479 ms.
execution in 0.052063 ms.
execution in 0.037304 ms.
execution in 0.040883 ms.
average: 0.0462928 ms.


(let a 1 in (let b 2 in (let c 3 in (+ c (+ b a)))))
execution in 0.164469 ms.
execution in 0.166789 ms.
execution in 0.166077 ms.
execution in 0.149304 ms.
execution in 0.163339 ms.
average: 0.1619956 ms.


(iota 10)
execution in 0.475345 ms.
execution in 0.424066 ms.
execution in 0.438378 ms.
execution in 0.425164 ms.
execution in 0.439691 ms.
average: 0.440529 ms.


(iota 40) // noted segmentation fault after each run!!
execution in 2.22752 ms.
execution in 2.18904 ms.
execution in 2.22867 ms.
execution in 2.21879 ms.
execution in 2.18801 ms.
average: 2.210406 ms.


(sum (reverse (iota 30)))
execution in 31.3982 ms.
execution in 32.7463 ms.
execution in 32.5781 ms.
execution in 31.209 ms.
execution in 30.963 ms.
average: 31.77892 ms.


(sort (reverse (iota 10)))
execution in 9.64046 ms.
execution in 8.51409 ms.
execution in 8.90934 ms.
execution in 7.48785 ms.
execution in 8.32437 ms.
average: 8.57522 ms.

