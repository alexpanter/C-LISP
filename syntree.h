#ifndef PLD_LISP_SYNTREE_H
#define PLD_LISP_SYNTREE_H

#include "sexp.h"
#include "lex.h"
#include "operator.h"

/* syntree lexing position */
struct _syntree_lexing_position_t {
  LexTokenListElement position;
  unsigned int errors;
  int scope;
};



/* typedefs for easy usage */
typedef struct _syntree_lexing_position_t* LexingPosition;



/* functions for lexing positions type */
LexingPosition syntreeLexingPositionCreate()
{
  LexingPosition pos = malloc(sizeof(LexingPosition));
  pos->position = NULL;
  pos->errors = 0;
  pos->scope = 0;
  return pos;
}

void syntreeLexingPositionAdvance(LexingPosition pos)
{
  pos->position = pos->position->next;
}

void syntreeLexingPositionEnterScope(LexingPosition pos)
{
  pos->scope++;
}

void syntreeLexingPositionLeaveScope(LexingPosition pos)
{
  pos->scope--;
}



/* construct syntax tree (s-expression) from token list */
Sexp readKeyword(LexingPosition pos)
{
  Sexp sexp = NULL;
  if(pos->position->token->type != LEX_TOKEN_TYPE_KEYWORD) {
    printf("Parse error: Expected a keyword\n"); // exit(-1);
  } else {
    switch(pos->position->token->value.keyword)
    {
    case KEYWORD_CONS:    sexp = sexpCreateSymbol("cons");    break;
    case KEYWORD_DEFINE:  sexp = sexpCreateSymbol("define");  break;
    case KEYWORD_LAMBDA:  sexp = sexpCreateSymbol("lambda");  break;
    case KEYWORD_LOAD:    sexp = sexpCreateSymbol("load");    break;
    case KEYWORD_QUOTE:   sexp = sexpCreateSymbol("quote");   break;
    case KEYWORD_SAVE:    sexp = sexpCreateSymbol("save");    break;
    case KEYWORD_EQUALS:  sexp = sexpCreateSymbol("equals");  break;
    case KEYWORD_TRUE:    sexp = sexpCreateBoolean(1);        break;
    case KEYWORD_FALSE:   sexp = sexpCreateBoolean(0);        break;
    case KEYWORD_IF:      sexp = sexpCreateSymbol("if");      break;
    case KEYWORD_NOT:     sexp = sexpCreateSymbol("not");     break;
    case KEYWORD_MESSAGE: sexp = sexpCreateSymbol("message"); break;
    case KEYWORD_LET:     sexp = sexpCreateSymbol("let");     break;
    case KEYWORD_IN:      sexp = sexpCreateSymbol("in");      break;
    default:
      printf("Invalid recorded lex token keyword type!\n"); // exit(-1);
    }
  }
  syntreeLexingPositionAdvance(pos);
  return sexp;
}

Sexp readSymbol(LexingPosition pos)
{
  Sexp sexp = NULL;
  if(pos->position->token->type != LEX_TOKEN_TYPE_SYMBOL) {
    printf("Parse error: Expected a symbol\n"); // exit(-1);
  } else {
    sexp = sexpCreateSymbol(pos->position->token->value.symbol);
  }
  syntreeLexingPositionAdvance(pos);
  return sexp;
}

Sexp readInteger(LexingPosition pos)
{
  Sexp sexp = NULL;
  if(pos->position->token->type != LEX_TOKEN_TYPE_INTEGER) {
    printf("Parse error: Expected an integer\n"); // exit(-1);
  }
  else {
    sexp = sexpCreateInteger(pos->position->token->value.integer);
  }
  syntreeLexingPositionAdvance(pos);
  return sexp;
}

Sexp readDouble(LexingPosition pos)
{
  Sexp sexp = NULL;
  if(pos->position->token->type != LEX_TOKEN_TYPE_DOUBLE) {
    printf("Parse error: Expected a double floating-point\n"); // exit(-1);
  }
  else {
    sexp = sexpCreateDouble(pos->position->token->value.doubleFP);
  }
  syntreeLexingPositionAdvance(pos);
  return sexp;
}

Sexp readOperator(LexingPosition pos)
{
  Sexp sexp = NULL;
  if(pos->position->token->type != LEX_TOKEN_TYPE_OPERATOR) {
    printf("Parse error: Expected an operator\n"); // exit(-1);
  }
  else {
    sexp = sexpCreateOperator(pos->position->token->value.operator);
  }
  syntreeLexingPositionAdvance(pos);
  return sexp;
}

Sexp readString(LexingPosition pos)
{
  Sexp sexp = NULL;
  if(pos->position->token->type != LEX_TOKEN_TYPE_STRING) {
    printf("Parse error: Expected a string\n"); // exit(-1);
  }
  else {
    sexp = sexpCreateString(pos->position->token->value.string);
  }
  syntreeLexingPositionAdvance(pos);
  return sexp;
}

// for mutual recursion
Sexp readTail(LexingPosition pos);
Sexp readSexp(LexingPosition pos);


Sexp readTail(LexingPosition pos)
{
  if(!pos->position) {
    return sexpCreateNil();
  }
  Sexp head = NULL;

  switch(pos->position->token->type)
  {
  case LEX_TOKEN_TYPE_KEYWORD:
    head = readKeyword(pos);
    return sexpCreateCons(head, readTail(pos));

  case LEX_TOKEN_TYPE_SYMBOL:
    head = readSymbol(pos);
    return sexpCreateCons(head, readTail(pos));

  case LEX_TOKEN_TYPE_SPECIALCHAR:
    switch(pos->position->token->value.special_char)
    {
    case LEX_TOKEN_SPECIALCHAR_APOSTROPHE:
      syntreeLexingPositionAdvance(pos);
      head = readSexp(pos);
      Sexp quote = sexpCreateSymbol("quote");
      Sexp tail = readTail(pos);
      Sexp nil = sexpCreateNil();
      return sexpCreateCons(sexpCreateCons(quote, sexpCreateCons(head, nil)), tail);

    case LEX_TOKEN_SPECIALCHAR_DOT:
      syntreeLexingPositionAdvance(pos);
      head = readSexp(pos);
      Sexp close = readTail(pos);
      if(close->type != SEXP_TYPE_NIL) {
        printf("Syntax error: missing close paranthesis\n");
        pos->errors++;
        return NULL;
      }
      return head;

    case LEX_TOKEN_SPECIALCHAR_LPAR:
      syntreeLexingPositionAdvance(pos);
      syntreeLexingPositionEnterScope(pos);
      head = readTail(pos);
      return sexpCreateCons(head, readTail(pos));

    case LEX_TOKEN_SPECIALCHAR_RPAR:
      syntreeLexingPositionLeaveScope(pos);
      syntreeLexingPositionAdvance(pos);
      return sexpCreateNil();

    default:
      printf("Invalid recorded lex token specialchar type!\n"); // exit(-1);
      pos->errors++;
    }
    break;

  case LEX_TOKEN_TYPE_INTEGER:
    head = readInteger(pos);
    return sexpCreateCons(head, readTail(pos));

  case LEX_TOKEN_TYPE_DOUBLE:
    head = readDouble(pos);
    return sexpCreateCons(head, readTail(pos));

  case LEX_TOKEN_TYPE_OPERATOR:
    head = readOperator(pos);
    return sexpCreateCons(head, readTail(pos));

  case LEX_TOKEN_TYPE_STRING:
    head = readString(pos);
    return sexpCreateCons(head, readTail(pos));

  default:
    printf("Invalid recorded lex token type!\n"); // exit(-1);
    pos->errors++;
  }
  printf("Syntax error: Could not parse tail\n");
  pos->errors++;
  return head;
}

Sexp readSexp(LexingPosition pos)
{
  if(!pos->position) {
    return sexpCreateNil();
  }
  Sexp sexp = NULL;

  switch(pos->position->token->type)
  {
  case LEX_TOKEN_TYPE_KEYWORD:
    return readKeyword(pos);

  case LEX_TOKEN_TYPE_SYMBOL:
    return readSymbol(pos);

  case LEX_TOKEN_TYPE_SPECIALCHAR:
    switch(pos->position->token->value.special_char)
    {
    case LEX_TOKEN_SPECIALCHAR_APOSTROPHE:
      sexp = sexpCreateSymbol("quote");
      syntreeLexingPositionAdvance(pos);
      return sexpCreateCons(sexp, sexpCreateCons(readSexp(pos), sexpCreateNil()));

    case LEX_TOKEN_SPECIALCHAR_DOT:
      printf("Syntax error: unexpected dot\n");
      pos->errors++;
      break;

    case LEX_TOKEN_SPECIALCHAR_LPAR:
      syntreeLexingPositionAdvance(pos);
      syntreeLexingPositionEnterScope(pos);
      return readTail(pos);

    case LEX_TOKEN_SPECIALCHAR_RPAR:
      printf("Syntax error: unexpected rpar\n");
      pos->errors++;
      break;

    default:
      printf("Invalid recorded lex token specialchar type!\n"); // exit(-1);
      pos->errors++;
    }
    break;

  case LEX_TOKEN_TYPE_INTEGER:
    return readInteger(pos);

  case LEX_TOKEN_TYPE_DOUBLE:
    return readDouble(pos);

  case LEX_TOKEN_TYPE_OPERATOR:
    return readOperator(pos);

  case LEX_TOKEN_TYPE_STRING:
    return readString(pos);

  default:
    printf("Invalid recorded lex token type!\n"); // exit(-1);
    pos->errors++;
  }
  printf("Syntax error: Could not parse s-expression\n");
  pos->errors++;
  return sexp;
}


Sexp transformTokenListToSexp(LexTokenList list)
{
  LexingPosition pos = syntreeLexingPositionCreate();
  pos->position = list->head;
  Sexp sexp = readSexp(pos);

  if(pos->position) {
    printf("Parse error: Invalid code input\n");
    sexpFree(sexp);
    return NULL;
  }
  if(pos->errors > 0) {
    sexpFree(sexp);
    return NULL;
  }
  if(pos->scope) {
    printf("Syntax error: unmatched number of paranthesis\n");
    sexpFree(sexp);
    return NULL;
  }

  return sexp;
}



#endif // PLD_LISP_SYNTREE_H
