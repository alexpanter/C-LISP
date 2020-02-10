#include <setjmp.h>
#include <time.h>
#include "sexp.h"
#include "io.h"
#include "lex.h"
#include "syntree.h"
#include "eval.h"
#include "symtable.h"
#include "exception.h"

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
    if(!strcmp(argv[i],      "--debug-lexing"))   { debugLexing   = 1; }
    else if(!strcmp(argv[i], "--debug-syntree"))  { debugSyntree  = 1; }
    else if(!strcmp(argv[i], "--debug-symtable")) { debugSymtable = 1; }
    else if(!strcmp(argv[i], "--debug-input"))    { debugInput    = 1; }
    else if(!strcmp(argv[i], "--debug-time"))     { debugTime     = 1; }
    else if(!strcmp(argv[i], "--debug-all")) {
      debugLexing = 1;
      debugSyntree = 1;
      debugSymtable = 1;
      debugInput = 1;
      debugTime = 1;
    }
    else { printf("Invalid argument '%s'", argv[i]); }
  }
  printf("PLD C-LISP v. 1.0\n");
  repl();

  symtableFree(globalEnvironment);
  return 0;
}
