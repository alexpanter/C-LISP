#ifndef PLD_LISP_SEXP_H
#define PLD_LISP_SEXP_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "operator.h"
#include "number.h"

/* s-expression types */
struct _sexp_t;

union _sexp_value_t {
  char* symbol;
  int boolean;
  void* nil; // must be a null pointer
  struct _sexp_t* cons[2];
  int integer;
  double doubleFP;
  Operator operator;
  char* string;
};

enum _sexp_type_t {
  SEXP_TYPE_SYMBOL,
  SEXP_TYPE_BOOLEAN,
  SEXP_TYPE_NIL,
  SEXP_TYPE_CONS,
  SEXP_TYPE_INTEGER,
  SEXP_TYPE_DOUBLE,
  SEXP_TYPE_OPERATOR,
  SEXP_TYPE_STRING
};

struct _sexp_t {
  enum _sexp_type_t type;
  union _sexp_value_t value;
};



/* typedefs for easy usage */
typedef struct _sexp_t* Sexp;



/* functions definitions for mutual recursion */
Sexp sexpCreateSymbol(const char* symbol);
Sexp sexpCreateBoolean(int bool);
Sexp sexpCreateNil();
Sexp sexpCreateCons(Sexp sexp1, Sexp sexp2);
Sexp sexpCreateInteger(int integer);
Sexp sexpCreateDouble(double doubleFP);
Sexp sexpCreateOperator(Operator operator);
Sexp sexpCreateString(const char* string);
Sexp sexpCopy(Sexp sexp);



/* S-expression type functions */
Sexp sexpAlloc()
{
  return malloc(sizeof(struct _sexp_t));
}

Sexp sexpCreateSymbol(const char* symbol)
{
  Sexp sexp = sexpAlloc();
  sexp->type = SEXP_TYPE_SYMBOL;
  sexp->value.symbol = malloc(strlen(symbol) * sizeof(char));
  strcpy(sexp->value.symbol, symbol);
  return sexp;
}

Sexp sexpCreateBoolean(int bool)
{
  Sexp sexp = sexpAlloc();
  sexp->type = SEXP_TYPE_BOOLEAN;
  sexp->value.boolean = bool;
  return sexp;
}

Sexp sexpCreateNil()
{
  Sexp sexp = sexpAlloc();
  sexp->type = SEXP_TYPE_NIL;
  sexp->value.nil = NULL;
  return sexp;
}

Sexp sexpCreateCons(Sexp sexp1, Sexp sexp2)
{
  Sexp sexp = sexpAlloc();
  sexp->type = SEXP_TYPE_CONS;
  sexp->value.cons[0] = sexpCopy(sexp1);
  sexp->value.cons[1] = sexpCopy(sexp2);
  return sexp;
}

Sexp sexpCreateInteger(int integer)
{
  Sexp sexp = sexpAlloc();
  sexp->type = SEXP_TYPE_INTEGER;
  sexp->value.integer = integer;
  return sexp;
}

Sexp sexpCreateDouble(double doubleFP)
{
  Sexp sexp = sexpAlloc();
  sexp->type = SEXP_TYPE_DOUBLE;
  sexp->value.doubleFP = doubleFP;
  return sexp;
}

Sexp sexpCreateOperator(Operator operator)
{
  Sexp sexp = sexpAlloc();
  sexp->type = SEXP_TYPE_OPERATOR;
  sexp->value.operator = operator;
  return sexp;
}

Sexp sexpCreateString(const char* string)
{
  Sexp sexp = sexpAlloc();
  sexp->type = SEXP_TYPE_STRING;
  sexp->value.string = malloc(strlen(string) * sizeof(char));
  strcpy(sexp->value.string, string);
  return sexp;
}

Sexp sexpCopy(Sexp sexp)
{
  if(!sexp) {
    printf("sexp copy: sexp is null\n"); // exit(-1);
    return NULL;
  }
  switch(sexp->type)
  {
  case SEXP_TYPE_SYMBOL:
    return sexpCreateSymbol(sexp->value.symbol);
  case SEXP_TYPE_BOOLEAN:
    return sexpCreateBoolean(sexp->value.boolean);
  case SEXP_TYPE_NIL:
    return sexpCreateNil();
  case SEXP_TYPE_CONS:
    return sexpCreateCons(sexp->value.cons[0], sexp->value.cons[1]);
  case SEXP_TYPE_INTEGER:
    return sexpCreateInteger(sexp->value.integer);
  case SEXP_TYPE_DOUBLE:
    return sexpCreateDouble(sexp->value.doubleFP);
  case SEXP_TYPE_OPERATOR:
    return sexpCreateOperator(sexp->value.operator);
  case SEXP_TYPE_STRING:
    return sexpCreateString(sexp->value.string);
  default:
    printf("Sexp copy: Invalid recorded sexp type!\n"); // exit(-1);
    return NULL;
  }
}

void sexpPrintDebug(Sexp sexp)
{
  if(!sexp) {
    printf("sexp print: sexp is null\n");
    return;
  }
  switch(sexp->type)
  {
  case SEXP_TYPE_SYMBOL:
    printf("Symbol \"%s\"", sexp->value.symbol);
    break;
  case SEXP_TYPE_BOOLEAN:
    printf("Boolean(%s)", (sexp->value.boolean) ? "true" : "false");
    break;
  case SEXP_TYPE_NIL:
    printf("Nil");
    break;
  case SEXP_TYPE_CONS:
    printf("Cons(");
    sexpPrintDebug(sexp->value.cons[0]);
    printf(", ");
    sexpPrintDebug(sexp->value.cons[1]);
    printf(")");
    break;
  case SEXP_TYPE_INTEGER:
    printf("Int %i", sexp->value.integer);
    break;
  case SEXP_TYPE_DOUBLE:
    printf("Double "); numberPrintDouble(sexp->value.doubleFP);
    break;
  case SEXP_TYPE_OPERATOR:
    printf("Operator '");
    operatorPrint(sexp->value.operator);
    printf("'");
    break;
  case SEXP_TYPE_STRING:
    printf("String \"%s\"", sexp->value.string);
    break;
  default:
    printf("Sexp print: Invalid recorded sexp type!\n"); // exit(-1);
  }
}

/* function signature for mutual recursion */
void sexpPrintTail(Sexp sexp);
void sexpPrint(Sexp sexp);

void sexpPrintTail(Sexp sexp)
{
  if(!sexp) {
    printf("sexp print tail: sexp is null\n");
    return;
  }
  switch(sexp->type)
  {
  case SEXP_TYPE_SYMBOL:
    printf(". %s)", sexp->value.symbol);
    break;
  case SEXP_TYPE_BOOLEAN:
    printf(" %s", (sexp->value.boolean) ? "true" : "false");
    break;
  case SEXP_TYPE_NIL:
    printf(")");
    break;
  case SEXP_TYPE_CONS:
    printf(" ");
    sexpPrint(sexp->value.cons[0]);
    sexpPrintTail(sexp->value.cons[1]);
    break;
  case SEXP_TYPE_INTEGER:
    printf(" %i", sexp->value.integer);
    break;
  case SEXP_TYPE_DOUBLE:
    printf(" "); numberPrintDouble(sexp->value.doubleFP);
    break;
  case SEXP_TYPE_OPERATOR:
    printf(" ");
    operatorPrint(sexp->value.operator);
    break;
  case SEXP_TYPE_STRING:
    printf(". \"%s\")", sexp->value.string);
    break;
  default:
    printf("Sexp print tail: Invalid recorded sexp type!\n"); // exit(-1);
  }
}

void sexpPrint(Sexp sexp)
{
  if(!sexp) {
    printf("sexp print: sexp is null\n");
    return;
  }
  switch(sexp->type)
  {
  case SEXP_TYPE_SYMBOL:
    printf("%s", sexp->value.symbol);
    break;
  case SEXP_TYPE_BOOLEAN:
    printf("%s", (sexp->value.boolean) ? "true" : "false");
    break;
  case SEXP_TYPE_NIL:
    printf("()");
    break;
  case SEXP_TYPE_CONS:
    if(sexp->value.cons[0]->type == SEXP_TYPE_SYMBOL &&
       !strcmp(sexp->value.cons[0]->value.symbol, "quote") &&
       sexp->value.cons[1]->type == SEXP_TYPE_CONS &&
       sexp->value.cons[1]->value.cons[1]->type == SEXP_TYPE_NIL)
    {
      printf("'");
      sexpPrint(sexp->value.cons[1]->value.cons[0]);
    } else {
      printf("(");
      sexpPrint(sexp->value.cons[0]);
      sexpPrintTail(sexp->value.cons[1]);
    }
    break;
  case SEXP_TYPE_INTEGER:
    printf("%i", sexp->value.integer);
    break;
  case SEXP_TYPE_DOUBLE:
    numberPrintDouble(sexp->value.doubleFP);
    break;
  case SEXP_TYPE_OPERATOR:
    operatorPrint(sexp->value.operator);
    break;
  case SEXP_TYPE_STRING:
    printf("\"%s\"", sexp->value.string);
    break;
  default:
    printf("Sexp print: Invalid recorded sexp type\n"); // exit(-1);
  }
}

void sexpFree(struct _sexp_t* sexp)
{
  if(!sexp) {
    //printf("Cannot free sexp that is already null!\n"); // exit(-1);
    return;
  }
  switch(sexp->type)
  {
  case SEXP_TYPE_SYMBOL:
    free(sexp->value.symbol);
    break;
  case SEXP_TYPE_BOOLEAN:
    break;
  case SEXP_TYPE_NIL:
    break;
  case SEXP_TYPE_CONS:
    sexpFree(sexp->value.cons[0]);
    sexpFree(sexp->value.cons[1]);
    break;
  case SEXP_TYPE_INTEGER:
    break;
  case SEXP_TYPE_DOUBLE:
    break;
  case SEXP_TYPE_OPERATOR:
    break;
  case SEXP_TYPE_STRING:
    free(sexp->value.string);
    break;
  default:
    printf("sexp free: Invalid recorded sexp type\n"); // exit(-1);
    return;
  }
  free(sexp);
}





#endif // PLD_LISP_SEXP_H
