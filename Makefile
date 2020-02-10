

clisp: testSexp.c
	gcc $< -o $@ -Werror -pedantic -O2 -lm

clean:
	rm -f clisp
