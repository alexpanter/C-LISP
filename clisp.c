#include <setjmp.h>
#include <time.h>
#include "sexp.h"
#include "io.h"
#include "lex.h"
#include "syntree.h"
#include "eval.h"
#include "symtable.h"
#include "exception.h"

/* C-Lisp REPL help */
void printHelp()
{
  printf("C-Lisp version 0.1 -- a free lisp interpreter.\n");
  printf("Published under the GNU General Public License.\n");
  printf("Join the community at https://github.com/alexpanter/C-Lisp\n\n");

  printf("Interpreter options:\n");

  printf("  %-20s", "--debug-lexing");
  printf("Print lexing output\n");

  printf("  %-20s", "--debug-syntree");
  printf("Print syntax tree produced from lexing output\n");

  printf("  %-20s", "--debug-symtable");
  printf("Print symbol table (global bindings) after input execution\n");

  printf("  %-20s", "--debug-input");
  printf("Print user input\n");

  printf("  %-20s", "--debug-time");
  printf("Print execution time\n");

  printf("  %-20s", "--debug-all");
  printf("Enable all interpreter options (verbose output!)\n");
}



/* global symtable */
extern Symtable globalEnvironment;

/* debugging options */
int debugLexing = 0;
int debugSyntree = 0;
int debugSymtable = 0;
int debugInput = 0;
int debugTime = 0;

/* exception handling */
extern jmp_buf jumpbuffer;

/* timer */
struct timespec begin, end;
/* gather average time for benchmarks */
double timeSum = 0.0;
int timeCount = 0;


/* REPL-loop */
void repl()
{
  /* create return point from caught exceptions */
  setjmp(jumpbuffer);

  // TODO: deallocate used memory for sexp and symtable types!

  while(1)
  {
    /* read input */
    printf("> ");
    fflush(stdout);
    char* input = inputBufferRead();

    /* exit REPL */
    if(!strcmp(input, "#exit")) {
      inputBufferFree(input);
      break;
    }
    if(!strcmp(input, "#bindings")) {
      symtablePrint(globalEnvironment);
      inputBufferFree(input);
      continue;
    }
    if(!strcmp(input, "#help")) {
      printf("#exit     -> exit LISP repl\n");
      printf("#bindings -> show global bindings\n");
      inputBufferFree(input);
      continue;
    }
    if(!strcmp(input, "#avgtime")) {
      printf("average: %g ms.\n", timeSum / (double)timeCount);
      timeSum = 0.0;
      timeCount = 0;
      inputBufferFree(input);
      continue;
    }

    /* debug input */
    if(debugInput) {
      printf("INPUT: \"%s\"\n", input);
    }

    /* debug time */
    if(debugTime) {
      clock_gettime(CLOCK_MONOTONIC, &begin);
    }

    /* lexing */
    LexTokenList list = transformBufferToTokenList(input);
    inputBufferFree(input);
    if(!list) {
      printf("! invalid input\n");
      continue;
    }

    /* debug lexing */
    if(debugLexing) {
      printf("TOKENLIST: ");
      lexTokenListPrint(list);
    }

    /* construct syntax tree */
    Sexp sexp = transformTokenListToSexp(list);
    lexTokenListFree(list);
    if(!sexp) {
      continue;
    }

    /* debug syntree */
    if(debugSyntree) {
      printf("\nSEXP DEBUG: ");
      sexpPrintDebug(sexp);
      printf("\n");
      printf("SEXP: ");
      sexpPrint(sexp);
      printf("\n");
    }

    /* evaluate input */
    Symtable symtable = symtableCreate();
    Sexp result = evalSexp(sexp, symtable);
    if(result) {
      printf("= ");
      sexpPrint(result);
      printf("\n");
    }

    /* evaluate time */
    if(debugTime) {
      clock_gettime(CLOCK_MONOTONIC, &end);
      double elapsed = (end.tv_sec - begin.tv_sec) * 1000.0
          + (end.tv_nsec - begin.tv_nsec) * 1E-6;
      timeSum += elapsed;
      timeCount++;
      printf("execution in %g ms.\n", elapsed);
    }

    /* debug symtable */
    if(debugSymtable) {
      printf("GLOBAL BINDINGS:\n");
      symtablePrint(globalEnvironment);
    }

    /* garbage collection */
    sexpFree(sexp);
    sexpFree(result);
    symtableFree(symtable);
  }
}



int main(int argc, char** argv)
{
  globalEnvironment = symtableCreate();

  for(int i = 1; i < argc; i++) {
    if(!strcmp(argv[i], "--help")) { printHelp(); goto END_REPL; } // ok, exit.
    else if(!strcmp(argv[i], "--debug-lexing"  )) { debugLexing   = 1; }
    else if(!strcmp(argv[i], "--debug-syntree" )) { debugSyntree  = 1; }
    else if(!strcmp(argv[i], "--debug-symtable")) { debugSymtable = 1; }
    else if(!strcmp(argv[i], "--debug-input"   )) { debugInput    = 1; }
    else if(!strcmp(argv[i], "--debug-time"    )) { debugTime     = 1; }
    else if(!strcmp(argv[i], "--debug-all"     )) {
      debugLexing = debugSyntree = debugSymtable = debugInput = debugTime = 1;
    }
    else { printf("Invalid argument '%s'", argv[i]); }
  }
  printf("PLD C-LISP v. 0.1\n");
  repl();

 END_REPL:
  symtableFree(globalEnvironment);
  return EXIT_SUCCESS;
}
