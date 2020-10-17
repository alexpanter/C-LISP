# C-LISP
Interpreter for a LISP language, written in C.

Began as an assignment during the course Programming Language Design (PLD)
at the University of Copenhagen under the guidance of lector Torben Ægidius Mogensen.

Was later extended with more advanced features such as:
- booleans, integers, and floats
- strings and print-formatting
- a complete list library (includes an implementation of merge sort)
- lambda closures using `(let <symbol> in <expression>)`.

Planned features:
- more clever memory management to remove all memory leaks (many are present!)
- optimized evaluation
- a math library: pow, ceil, floor, trunc, sqrt

Is now intended, and maintained, as an open-source project.
TODO: Will be released under the GNU General Public License for free usage/modification,
whenever the original source (here) is still cited.

## Project aim ##

To provide programmers with a handy calculator (with more powerfull expressions that e.g.
Python due to the brevity of functional programming techniques).

Is only ever intended as a terminal application.

Building a compiler will likely never be necessary.
For all conceived purposes, having an interpreter is sufficient for C-LISP.

Enjoy!
