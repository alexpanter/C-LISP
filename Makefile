
MAIN_FILE=clisp.c
MAIN_FILE_EXE=clisp


build: $(MAIN_FILE)
	gcc $< -o $(MAIN_FILE_EXE) -Werror -pedantic -O2 -lm

clean:
	rm -f $(MAIN_FILE_EXE)
