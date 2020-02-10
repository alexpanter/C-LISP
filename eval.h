#ifndef PLD_LISP_EVAL_H
#define PLD_LISP_EVAL_H

#include "sexp.h"
#include "symtable.h"
#include "lex.h"
#include "keyword.h"
#include "operator.h"
#include "exception.h"
#include "io.h"
#include "syntree.h"
#include "string.h"
#include "operator_application.h"

/* global symbol table */
extern Symtable globalEnvironment;



/* function definitions for mutual recursion */
Sexp evalTryRules(Sexp rules, Sexp arguments, Symtable environment);
Sexp evalList(Sexp program, Symtable environment);
Sexp evalSexpConsNoMatch(Sexp program, Symtable environment);
Sexp evalSexp(Sexp program, Symtable environment);
Symtable evalMatchPattern(Sexp pattern, Sexp arguments);
void evalQuoteSexp(Sexp sexp);
Sexp evalSexpEquals(Sexp e1, Symtable env1, Sexp e2, Symtable env2);
Sexp evalListEquals(Sexp e1, Symtable env1, Sexp e2, Symtable env2);



/* evaluate a syntax tree (s-expression) */
Sexp evalList(Sexp program, Symtable environment)
{
  if(!program) {
    printf("eval list: program is null\n");
    return NULL;
  }
  if(!environment) {
    printf("eval list: environment is null\n");
    return NULL;
  }

  Sexp ret = NULL;
  switch(program->type)
  {
  case SEXP_TYPE_NIL:
    return sexpCreateNil();

  case SEXP_TYPE_BOOLEAN:
    // TODO: Is it a just assumption to skip this case?
    //return sexpCreateBoolean(program->value.boolean);

  case SEXP_TYPE_INTEGER:
    // TODO: Is it a just assumption to skip this case?
    //return sexpCreateInteger(program->value.integer);

  case SEXP_TYPE_DOUBLE:
    // TODO: Is it a just assumption to skip this case?
    //return sexpCreateDouble(program->value.double);

  case SEXP_TYPE_OPERATOR:
    // TODO: Is it a just assumption to skip this case?
    //return sexpCreateOperator(program->value.operator);

  case SEXP_TYPE_STRING:
    // TODO: Is it a just assumption to skip this case?
    //return sexpCreateString(program->value.string);

  case SEXP_TYPE_SYMBOL:
    printf("! arguments are not a list: ");
    sexpPrint(program);
    printf("\n");
    throwException();
    printf("Control should not reach this point!\n");
    return NULL;

  case SEXP_TYPE_CONS:
    ret = NULL;
    Sexp s1 = program->value.cons[0];
    Sexp s2 = program->value.cons[1];
    return sexpCreateCons(evalSexp(s1, environment), evalList(s2, environment));

  default:
    printf("eval list: Invalid S-expression type\n");
  }
  return ret;
}

Sexp evalTryRules(Sexp rules, Sexp arguments, Symtable environment)
{
  if(!rules) {
    printf("eval try rules: rules is null\n");
    return NULL;
  }
  if(!arguments) {
    printf("eval try rules: arguments is null\n");
    return NULL;
  }
  if(!environment) {
    printf("eval try rules: environment is null\n");
    return NULL;
  }

  Sexp ret = NULL;
  switch(rules->type)
  {
  case SEXP_TYPE_NIL:
    printf("! no patterns matched arguments ");
    sexpPrint(arguments);
    printf("\n");
    throwException();
    printf("Control should not reach this point!\n");
    return NULL;

  case SEXP_TYPE_CONS:
    ret = NULL;
    Sexp s1 = rules->value.cons[0]; // p
    Sexp s2 = rules->value.cons[1]; // Cons(e, rs1)
    if(s2->type == SEXP_TYPE_CONS) {
      Sexp s3 = s2->value.cons[0];  // e
      Sexp s4 = s2->value.cons[1];  // rs1
      Symtable newEnvironment = evalMatchPattern(s1, arguments);
      if(newEnvironment) {
        return evalSexp(s3, symtableCombine(newEnvironment, environment));
      }
      else {
        return evalTryRules(s4, arguments, environment);
      }
    } // fall-through to  default case

  case SEXP_TYPE_BOOLEAN:
  case SEXP_TYPE_SYMBOL:
  case SEXP_TYPE_INTEGER:
  case SEXP_TYPE_DOUBLE:
  case SEXP_TYPE_OPERATOR:
  case SEXP_TYPE_STRING:
  default:
    printf("! malformed rules ");
    sexpPrint(rules);
    printf("\n");
    throwException();
    printf("Control should not reach this point!\n");
    return NULL;
  }
  return ret;
}

Symtable evalMatchPattern(Sexp pattern, Sexp arguments)
{
  if(!pattern) {
    printf("eval match pattern: pattern is null\n");
    return NULL;
  }
  if(!arguments) {
    printf("eval match pattern: arguments is null\n");
    return NULL;
  }

  if(pattern->type == SEXP_TYPE_NIL && arguments->type == SEXP_TYPE_NIL) {
    return symtableCreate(); // return empty list
  }
  else if(pattern->type == SEXP_TYPE_SYMBOL) {
    if((int)keywordMatch(pattern->value.symbol) != -1) {
      printf("! keyword %s can not be used in pattern\n", pattern->value.symbol);
      throwException();
      printf("Control should not reach this point!\n");
      return NULL; // return NULL
    }
    else {
      Symtable newSymtable = symtableCreate();
      symtableUpdate(newSymtable, pattern->value.symbol, arguments);
      return newSymtable;
    }
  }
  else if(pattern->type == SEXP_TYPE_CONS && arguments->type == SEXP_TYPE_CONS) {
    // using same naming convention as the F# LISP interpreter
    Sexp p1 = pattern->value.cons[0];
    Sexp p2 = pattern->value.cons[1];
    Sexp v1 = arguments->value.cons[0];
    Sexp v2 = arguments->value.cons[1];
    Symtable symtable1 = evalMatchPattern(p1, v1);
    Symtable symtable2 = evalMatchPattern(p2, v2);
    if(symtable1 && symtable2 && symtableDisjoint(symtable1, symtable2)) {
      return symtableCombine(symtable1, symtable2);
    }
    else {
      return NULL;
    }
  }
  else {
    return NULL;
  }
}

/* this function is used when saving the environment */
void evalQuoteSexp(Sexp sexp)
{
  if(!sexp) {
    printf("eval quote sexp: sexp is null\n");
    return;
  }

  switch(sexp->type)
  {
  case SEXP_TYPE_NIL:
    printf("()");
    return;

  case SEXP_TYPE_BOOLEAN:
    sexpPrint(sexp);
    return;

  case SEXP_TYPE_CONS:
    if(sexp->value.cons[0]->type == SEXP_TYPE_SYMBOL) {
      if(!strcmp(sexp->value.cons[0]->value.symbol, "quote") ||
         !strcmp(sexp->value.cons[0]->value.symbol, "lambda"))
      {
        sexpPrint(sexp);
        return;
      }
    } // fall-through to SEXP_TYPE_SYMBOL match case

    // TODO: Do "'(...)" instead of "(quote (...))" ?
  case SEXP_TYPE_SYMBOL:
    printf("(quote ");
    sexpPrint(sexp);
    printf(")");
    return;

  case SEXP_TYPE_INTEGER:
    sexpPrint(sexp);
    return;

  case SEXP_TYPE_DOUBLE:
    sexpPrint(sexp);
    return;

  case SEXP_TYPE_OPERATOR:
    sexpPrint(sexp);
    return;

  case SEXP_TYPE_STRING:
    sexpPrint(sexp);
    return;

  default:
    printf("eval quote sexp: Invalid S-expression type\n");
    return;
  }
}

/*
 * Since C does not support structured match cases with conditionals as in F#:
 * | Cons (Symbol "quote", Cons (v, Nil)) -> ,
 * this function handles the fallback case
 * | Cons (e1, pars) -> .
 */
Sexp evalSexpConsNoMatch(Sexp cons, Symtable environment)
{
  if(!cons) {
    printf("eval sexp cons no match: cons is null\n");
    return NULL;
  }
  if(!environment) {
    printf("eval sexp cons no match: environment is null\n");
    return NULL;
  }
  if(cons->type != SEXP_TYPE_CONS) {
    printf("eval sexp cons no match: cons is not of type cons\n");
    return NULL;
  }

  Sexp s1 = cons->value.cons[0];
  Sexp s2 = cons->value.cons[1];
  // check if keyword is being used as a symbol
  if((int)keywordMatch(s1->value.symbol) != -1) {
    printf("! malformed %s in expression\n", s1->value.symbol);
    throwException();
    printf("Control should not reach this point!\n");
    return NULL;
  }
  // try to match for function application
  else {
    Sexp newSexp = evalSexp(s1, environment);
    if(!newSexp) {
      printf("newSexp was null\n");
      return NULL;
    }
    if(newSexp->type == SEXP_TYPE_CONS &&
       newSexp->value.cons[0]->type == SEXP_TYPE_SYMBOL &&
       keywordMatch(newSexp->value.cons[0]->value.symbol) == KEYWORD_LAMBDA)
    {
      return evalTryRules(newSexp->value.cons[1], evalList(s2, environment),
                          environment);
    }
    else {
      printf("! ");
      sexpPrint(newSexp);
      printf(" can not be applied as a function\n");
      throwException();
      printf("Control should not reach this point!\n");
      return NULL;
    }
  }
}

// TODO: Does this function require symtables for each S-expression?
Sexp evalListEquals(Sexp e1, Symtable env1, Sexp e2, Symtable env2)
{
  // head of lists are not of same type -> will always be false
  if(e1->type != e2->type) {
    return sexpCreateBoolean(0);
  }
  // otherwise e2 must be of same type as e1, and we can omit this check
  else if(e1->type == SEXP_TYPE_CONS) {
    Sexp bool = evalSexpEquals(e1->value.cons[0], env1, e2->value.cons[0], env2);
    if(bool->type != SEXP_TYPE_BOOLEAN) {
      printf("Parse error: could not evaluate as boolean type!\n");
      throwException();
      printf("Control should not reach this point!\n");
      return NULL;
    }
    if(!bool->value.boolean) {
      return sexpCreateBoolean(0);
    }
    return evalListEquals(e1->value.cons[1], env1, e2->value.cons[1], env2);
  }
  else {
    return evalSexpEquals(e1, env1, e2, env2);
  }

  return sexpCreateBoolean(0);
}

Sexp evalSexpEquals(Sexp e1, Symtable env1, Sexp e2, Symtable env2)
{
  if(!e1) {
    printf("eval sexp equals: e1 is null\n");
    return NULL;
  }
  if(!e2) {
    printf("eval sexp equals: e2 is null\n");
    return NULL;
  }

  if(e1->type == SEXP_TYPE_NIL && e2->type == SEXP_TYPE_NIL) {
    return sexpCreateBoolean(1);
  }
  if(e1->type == SEXP_TYPE_NIL || e2->type == SEXP_TYPE_NIL) {
    return sexpCreateBoolean(0);
  }
  if(e1->type == SEXP_TYPE_BOOLEAN && e2->type == SEXP_TYPE_BOOLEAN) {
    return sexpCreateBoolean(e1->value.boolean == e2->value.boolean);
  }
  if(e1->type == SEXP_TYPE_SYMBOL && e2->type == SEXP_TYPE_SYMBOL) {
    return sexpCreateBoolean(!strcmp(e1->value.symbol, e2->value.symbol));
  }
  if(e1->type == SEXP_TYPE_STRING && e2->type == SEXP_TYPE_STRING) {
    return sexpCreateBoolean(!strcmp(e1->value.string, e2->value.string));
  }
  if(e1->type == SEXP_TYPE_INTEGER && e2->type == SEXP_TYPE_INTEGER) {
    return sexpCreateBoolean(e1->value.integer == e2->value.integer);
  }
  if(e1->type == SEXP_TYPE_DOUBLE && e2->type == SEXP_TYPE_DOUBLE) {
    // TODO: Should we match against a minimal arithmetic distance?
    return sexpCreateBoolean(e1->value.doubleFP == e2->value.doubleFP);
  }

  if(e1->type == SEXP_TYPE_INTEGER && e2->type == SEXP_TYPE_DOUBLE) {
    // TODO: Should we match against a minimal arithmetic distance?
    return sexpCreateBoolean(1e-8 > fabs((double)e1->value.integer - e2->value.doubleFP));
  }
  if(e1->type == SEXP_TYPE_DOUBLE && e2->type == SEXP_TYPE_INTEGER) {
    // TODO: Should we match against a minimal arithmetic distance?
    return sexpCreateBoolean(1e-8 > fabs(e1->value.doubleFP - (double)e2->value.integer));
  }

  if(e1->type == SEXP_TYPE_CONS && e2->type == SEXP_TYPE_CONS) {
    return evalListEquals(e1, env1, e2, env2);
  }
  if(e1->type == SEXP_TYPE_CONS || e2->type == SEXP_TYPE_CONS) {
    return sexpCreateBoolean(0);
  }

  return sexpCreateBoolean(0);
}



Sexp evalSexp(Sexp program, Symtable environment)
{
  Sexp ret = NULL;
  if(!program) {
    printf("eval sexp: program is null\n");
    return NULL;
  }
  if(!environment) {
    printf("eval sexp cons no match: environment is null\n");
    return NULL;
  }

  switch(program->type)
  {
  case SEXP_TYPE_NIL:
    return sexpCreateNil();

  case SEXP_TYPE_SYMBOL:
    if((int)keywordMatch(program->value.symbol) != -1) {
      printf("! keyword %s can not be used as variable\n", program->value.symbol);
      throwException();
      printf("Control should not reach this point!\n");
      return NULL;
    } else {
      // check if symbol is defined locally
      Sexp symbol = symtableLookupSymbol(environment, program->value.symbol);
      if(symbol) return symbol;

      // otherwise check global symbol table
      symbol = symtableLookupSymbol(globalEnvironment, program->value.symbol);
      if(symbol) return symbol;

      // if neither, symbol has not been defined
      printf("! undefined variable %s\n", program->value.symbol);
      throwException();
      printf("Control should not reach this point!\n");
      return NULL;
    }

  case SEXP_TYPE_BOOLEAN:
    return sexpCreateBoolean(program->value.boolean);

  case SEXP_TYPE_INTEGER:
    return sexpCreateInteger(program->value.integer);

  case SEXP_TYPE_DOUBLE:
    return sexpCreateDouble(program->value.doubleFP);

  case SEXP_TYPE_OPERATOR:
    printf("! operator "); operatorPrint(program->value.operator);
    printf(" can not be used as variable\n");
    throwException();
    printf("Control should not reach this point!\n");
    return NULL;

  case SEXP_TYPE_STRING:
    return sexpCreateString(program->value.string);

  case SEXP_TYPE_CONS:
    ret = NULL;
    Sexp s1 = program->value.cons[0];
    Sexp s2 = program->value.cons[1];

    if(s1->type == SEXP_TYPE_SYMBOL) {
      switch(keywordMatch(s1->value.symbol))
      {
      case KEYWORD_QUOTE:
        if(s2->type == SEXP_TYPE_CONS) {
          Sexp s3 = s2->value.cons[0];
          Sexp s4 = s2->value.cons[1];
          if(s4->type == SEXP_TYPE_NIL) {
            return s3;
          }
        }
        return evalSexpConsNoMatch(program, environment);

      case KEYWORD_LAMBDA:
        return program;

      // Cons (Symbol "define", Cons (Symbol x, Cons (e, Nil)))
      // program   s1            s2    s3        s4   s5  s6
      case KEYWORD_DEFINE:
        if(s2->type == SEXP_TYPE_CONS) {
          Sexp s3 = s2->value.cons[0];
          Sexp s4 = s2->value.cons[1];
          if(s3->type == SEXP_TYPE_SYMBOL && s4->type == SEXP_TYPE_CONS) {
            Sexp s5 = s4->value.cons[0];
            Sexp s6 = s4->value.cons[1];
            if(s6->type == SEXP_TYPE_NIL) {
              if((int)keywordMatch(s3->value.symbol) != -1) {
                printf("! keyword %s can not be redefined\n", s3->value.symbol);
                throwException();
                printf("Control should not reach this point!\n");
                return NULL;
              } else {
                Sexp newValue = evalSexp(s5, environment);
                symtableUpdate(globalEnvironment, s3->value.symbol, newValue);
                //sexpFree(newValue);
                return sexpCreateNil();
              }
            }
          }
        }
        return evalSexpConsNoMatch(program, environment);

      // Cons(Symbol "cons", Cons(e1, Cons(e2, Nil)))
      // program   s1         s2  s3   s4  s5   s6
      case KEYWORD_CONS:
        if(s2->type == SEXP_TYPE_CONS) {
          Sexp s3 = s2->value.cons[0];
          Sexp s4 = s2->value.cons[1];
          if(s4->type == SEXP_TYPE_CONS) {
            Sexp s5 = s4->value.cons[0];
            Sexp s6 = s4->value.cons[1];
            if(s6->type == SEXP_TYPE_NIL) {
              return sexpCreateCons(evalSexp(s3, environment),
                                    evalSexp(s5, environment));
            }
          }
        }
        return evalSexpConsNoMatch(program, environment);

      case KEYWORD_SAVE:
        printf("eval sexp: Save is not implemented\n");
        return NULL;

      case KEYWORD_LOAD:
        // | Cons (Symbol "save", Cons (Symbol f, Nil))
        if(s2->type != SEXP_TYPE_CONS ||
           s2->value.cons[0]->type != SEXP_TYPE_SYMBOL)
        {
          return evalSexpConsNoMatch(program, environment);
        }
        const char* symbol = s2->value.cons[0]->value.symbol;
        unsigned long len = strlen(symbol) + 4; // add space for ".le\0"
        char* filename = malloc(sizeof(char) * len);
        snprintf(filename, len, "%s.le", symbol);
        FileContents lib = librarySmartLoad(filename);
        FileContentsLine line = lib->head;
        while(line)
        {
          LexTokenList tokenlist = transformBufferToTokenList(line->line);
          Sexp expression = transformTokenListToSexp(tokenlist);
          Sexp evalutation = evalSexp(expression, symtableCreate());
          if(!evalutation) {
            printf("! error while reading %s\n", filename);
            throwException();
            printf("Control should not reach this point!\n");
            return NULL;
          }
          line = line->next;
        }
        return sexpCreateNil();

      case KEYWORD_EQUALS:
        // would be in F# :
        // | Cons (Symbol "equals", Cons(e1, Cons(e2, Nil))
        //   program    s1           s2
        ret = NULL;
        if(s2->type == SEXP_TYPE_CONS &&
           s2->value.cons[1]->type == SEXP_TYPE_CONS &&
           s2->value.cons[1]->value.cons[1]->type == SEXP_TYPE_NIL)
        {
          Sexp e1 = evalSexp(s2->value.cons[0], environment);
          Sexp e2 = evalSexp(s2->value.cons[1]->value.cons[0], environment);
          return evalSexpEquals(e1, environment, e2, environment);
        }
        return evalSexpConsNoMatch(program, environment);

      case KEYWORD_IF:
        // would be in F# :
        // | Cons (Symbol "if", Cons(cond, Cons(e1, Cons(e2, Nil))))
        //  program   s1         s2
        if(s2->type == SEXP_TYPE_CONS &&
           s2->value.cons[1]->type == SEXP_TYPE_CONS &&
           s2->value.cons[1]->value.cons[1]->type == SEXP_TYPE_CONS &&
           s2->value.cons[1]->value.cons[1]->value.cons[1]->type == SEXP_TYPE_NIL)
        {
          Sexp cond = evalSexp(s2->value.cons[0], environment);
          if(cond->type == SEXP_TYPE_BOOLEAN) {
            Sexp e1 = s2->value.cons[1]->value.cons[0];
            Sexp e2 = s2->value.cons[1]->value.cons[1]->value.cons[0];
            if(cond->value.boolean) {
              return evalSexp(e1, environment);
            }
            else {
              return evalSexp(e2, environment);
            }
          }
          else {
            printf("! condition expression must be a boolean\n");
            throwException();
            printf("Control should not reach this point!\n");
            return NULL;
          }
        }
        return evalSexpConsNoMatch(program, environment);

      case KEYWORD_NOT:
        // would be in F# :
        // | Cons(Symbol "not", Cons(e, Nil))
        if(s2->type == SEXP_TYPE_CONS &&
           s2->value.cons[1]->type == SEXP_TYPE_NIL)
        {
          Sexp e = evalSexp(s2->value.cons[0], environment);
          if(e->type == SEXP_TYPE_BOOLEAN) {
            return sexpCreateBoolean(!e->value.boolean);
          }
          else {
            printf("! condition expression must be a boolean\n");
            throwException();
            printf("Control should not reach this point!\n");
            return NULL;
          }
        }
        return evalSexpConsNoMatch(program, environment);

      case KEYWORD_MESSAGE:
        // would be in F# :
        // | Cons(Symbol "message", Cons(String format, args..)
        if(s2->type == SEXP_TYPE_CONS &&
           s2->value.cons[0]->type == SEXP_TYPE_STRING) {
          const char* string = s2->value.cons[0]->value.string;
          Sexp args = evalList(s2->value.cons[1], environment);
          stringPrintMessage(string, args);
          return sexpCreateNil();
        }
        else {
          printf("eval sexp: message NOT correctly formatted\n");
          throwException();
          printf("Control should not reach this point!\n");
          return NULL;
        }

      case KEYWORD_LET:
        // would be in F# :
        // | Cons(Symbol "let", Cons(Symbol k, Cons(e1, Cons(Symbol "in", Cons(e2, Nil)))))
        //          s1            s2   k        s3  e1   s4     s5         s6  e2
        // (let k e1 in e2)
        if(s2->type == SEXP_TYPE_CONS &&
           s2->value.cons[0]->type == SEXP_TYPE_SYMBOL &&
           s2->value.cons[1]->type == SEXP_TYPE_CONS)
        {
          Sexp s3 = s2->value.cons[1];
          Sexp e1 = s3->value.cons[0];
          Sexp s4 = s3->value.cons[1];
          if(s4->type == SEXP_TYPE_CONS &&
             s4->value.cons[0]->type == SEXP_TYPE_SYMBOL &&
             keywordMatch(s4->value.cons[0]->value.symbol) == KEYWORD_IN &&
             s4->value.cons[1]->type == SEXP_TYPE_CONS &&
             s4->value.cons[1]->value.cons[1]->type == SEXP_TYPE_NIL)
          {
            Sexp s5 = s4->value.cons[0];
            Sexp s6 = s4->value.cons[1];
            Sexp e2 = s6->value.cons[0];
            Sexp k = s2->value.cons[0];

            // 1) bind evaluation of e1 to k (new symtable)
            Sexp e1_eval = evalSexp(e1, environment);
            symtableUpdate(environment, s2->value.cons[0]->value.symbol, e1_eval);

            // 2) return evaluation of e2 (with new symtable)
            return evalSexp(e2, environment);
          }
        }
        // fall-back to here, if all other checks fail!
        printf("! malformed let-binding\n");
        throwException();
        printf("Control should not reach this point!\n");
        return NULL;

      default:
        return evalSexpConsNoMatch(program, environment);
      } // switch(keywordMatch(s1->value.symbol))

    } // if(s1->type == SEXP_TYPE_SYMBOL)


    // would be in F# :
    // | Cons(Operator op, Cons(arg1, Cons(arg2, Nil)))
    if(s1->type == SEXP_TYPE_OPERATOR &&
       s2->type == SEXP_TYPE_CONS &&
       s2->value.cons[1]->type == SEXP_TYPE_CONS &&
       s2->value.cons[1]->value.cons[1]->type == SEXP_TYPE_NIL)
    {
      Sexp arg1 = evalSexp(s2->value.cons[0], environment);
      Sexp arg2 = evalSexp(s2->value.cons[1]->value.cons[0], environment);
      return applyOperator(arg1, arg2, s1->value.operator);
    }
    else {
      printf("! invalid use of operator\n");
      throwException();
      printf("Control should not reach this point!\n");
      return NULL;
    }

    return evalSexpConsNoMatch(program, environment);

  default:
    printf("Invalid s-expression type\n"); // return exit(-1);
    return NULL;
  } // switch(program)

  return ret;
}





#endif // PLD_LISP_EVAL_H
