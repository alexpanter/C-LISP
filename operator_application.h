#ifndef PLD_LISP_OPERATOR_APPLICATION_H
#define PLD_LISP_OPERATOR_APPLICATION_H

#include <string.h>
#include <stdio.h>
#include <math.h>
#include "sexp.h"
#include "operator.h"
#include "exception.h"

/*
 * Since PLD C-LISP is a weakly-typed, interpreted language,
 * it is a design choice to implicitly convert operands from int to double
 * when performing calculations.
 */
Sexp applyEqualityOperator(Sexp num1, Sexp num2, Operator operator)
{
  if(num1->type == SEXP_TYPE_INTEGER && num2->type == SEXP_TYPE_INTEGER) {
    switch(operator)
    {
    case OPERATOR_EQUAL:
      return sexpCreateBoolean(num1->value.integer == num2->value.integer);
    case OPERATOR_LESS:
      return sexpCreateBoolean(num1->value.integer < num2->value.integer);
    case OPERATOR_LESS_EQUAL:
      return sexpCreateBoolean(num1->value.integer <= num2->value.integer);
    case OPERATOR_GREATER:
      return sexpCreateBoolean(num1->value.integer > num2->value.integer);
    case OPERATOR_GREATER_EQUAL:
      return sexpCreateBoolean(num1->value.integer >= num2->value.integer);
    default:
      printf("apply equality operator: invalid operator type\n");
      throwException();
      return NULL;
    }
  }

  else if(num1->type == SEXP_TYPE_INTEGER && num2->type == SEXP_TYPE_DOUBLE) {
    double arg1 = (double)num1->value.integer;
    switch(operator)
    {
    case OPERATOR_EQUAL:
      return sexpCreateBoolean(arg1 == num2->value.doubleFP);
    case OPERATOR_LESS:
      return sexpCreateBoolean(arg1 < num2->value.doubleFP);
    case OPERATOR_LESS_EQUAL:
      return sexpCreateBoolean(arg1 <= num2->value.doubleFP);
    case OPERATOR_GREATER:
      return sexpCreateBoolean(arg1 > num2->value.doubleFP);
    case OPERATOR_GREATER_EQUAL:
      return sexpCreateBoolean(arg1 >= num2->value.doubleFP);
    default:
      printf("apply equality operator: invalid operator type\n");
      throwException();
      return NULL;
    }
  }

  else if(num1->type == SEXP_TYPE_DOUBLE && num2->type == SEXP_TYPE_INTEGER) {
    double arg2 = (double)num2->value.integer;
    switch(operator)
    {
    case OPERATOR_EQUAL:
      return sexpCreateBoolean(num1->value.doubleFP == arg2);
    case OPERATOR_LESS:
      return sexpCreateBoolean(num1->value.doubleFP < arg2);
    case OPERATOR_LESS_EQUAL:
      return sexpCreateBoolean(num1->value.doubleFP <= arg2);
    case OPERATOR_GREATER:
      return sexpCreateBoolean(num1->value.doubleFP > arg2);
    case OPERATOR_GREATER_EQUAL:
      return sexpCreateBoolean(num1->value.doubleFP >= arg2);
    default:
      printf("apply equality operator: invalid operator type\n");
      throwException();
      return NULL;
    }
  }

  else if(num1->type == SEXP_TYPE_DOUBLE && num2->type == SEXP_TYPE_DOUBLE) {
    switch(operator)
    {
    case OPERATOR_EQUAL:
      return sexpCreateBoolean(num1->value.doubleFP == num2->value.doubleFP);
    case OPERATOR_LESS:
      return sexpCreateBoolean(num1->value.doubleFP < num2->value.doubleFP);
    case OPERATOR_LESS_EQUAL:
      return sexpCreateBoolean(num1->value.doubleFP <= num2->value.doubleFP);
    case OPERATOR_GREATER:
      return sexpCreateBoolean(num1->value.doubleFP > num2->value.doubleFP);
    case OPERATOR_GREATER_EQUAL:
      return sexpCreateBoolean(num1->value.doubleFP >= num2->value.doubleFP);
    default:
      printf("apply equality operator: invalid operator type\n");
      throwException();
      return NULL;
    }
  }

  else {
    printf("! cannot apply equality operator on values that are not numbers\n");
    throwException();
    return NULL;
  }
}

Sexp applyArithmeticOperator(Sexp num1, Sexp num2, Operator operator)
{
  if(num1->type == SEXP_TYPE_STRING && num2->type == SEXP_TYPE_STRING) {
    int len = 0;
    // space for terminating '\0' character
    len = strlen(num1->value.string) + strlen(num2->value.string) + 1;
    char buffer[len];
    int written;
    switch(operator)
    {
    case OPERATOR_PLUS:
      written = sprintf(buffer, "%s", num1->value.string);
      written += sprintf(&buffer[written], "%s", num2->value.string);
      return sexpCreateString(buffer);
    case OPERATOR_MINUS:
    case OPERATOR_MULTIPLY:
    case OPERATOR_DIVIDE:
    case OPERATOR_MODULUS:
      printf("! operator "); operatorPrint(operator);
      printf(" cannot be applied to strings\n");
      throwException();
      return NULL;
    default:
      printf("apply arithmetic operator: invalid operator type\n");
      throwException();
      return NULL;
    }
  }

  else if(num1->type == SEXP_TYPE_INTEGER && num2->type == SEXP_TYPE_INTEGER) {
    switch(operator)
    {
    case OPERATOR_PLUS:
      return sexpCreateInteger(num1->value.integer + num2->value.integer);
    case OPERATOR_MINUS:
      return sexpCreateInteger(num1->value.integer - num2->value.integer);
    case OPERATOR_MULTIPLY:
      return sexpCreateInteger(num1->value.integer * num2->value.integer);
    case OPERATOR_DIVIDE:
      if(num2->value.integer == 0) {
        printf("! divion by zero\n");
        throwException();
        return NULL;
      }
      return sexpCreateInteger(num1->value.integer / num2->value.integer);
    case OPERATOR_MODULUS:
      if(num2->value.integer == 0) {
        printf("! divion by zero\n");
        throwException();
        return NULL;
      }
      return sexpCreateInteger(num1->value.integer % num2->value.integer);
    default:
      printf("apply arithmetic operator: invalid operator type\n");
      throwException();
      return NULL;
    }
  }

  else if(num1->type == SEXP_TYPE_DOUBLE && num2->type == SEXP_TYPE_DOUBLE) {
    switch(operator)
    {
    case OPERATOR_PLUS:
      return sexpCreateDouble(num1->value.doubleFP + num2->value.doubleFP);
    case OPERATOR_MINUS:
      return sexpCreateDouble(num1->value.doubleFP - num2->value.doubleFP);
    case OPERATOR_MULTIPLY:
      return sexpCreateDouble(num1->value.doubleFP * num2->value.doubleFP);
    case OPERATOR_DIVIDE:
      return sexpCreateDouble(num1->value.doubleFP / num2->value.doubleFP);
    case OPERATOR_MODULUS:
      return sexpCreateDouble(fmod(num1->value.doubleFP, num2->value.doubleFP));
    default:
      printf("apply arithmetic operator: invalid operator type\n");
      throwException();
      return NULL;
    }
  }

  else if(num1->type == SEXP_TYPE_INTEGER && num2->type == SEXP_TYPE_DOUBLE) {
    double arg1 = (double)num1->value.integer;
    switch(operator)
    {
    case OPERATOR_PLUS:
      return sexpCreateDouble(arg1 + num2->value.doubleFP);
    case OPERATOR_MINUS:
      return sexpCreateDouble(arg1 - num2->value.doubleFP);
    case OPERATOR_MULTIPLY:
      return sexpCreateDouble(arg1 * num2->value.doubleFP);
    case OPERATOR_DIVIDE:
      return sexpCreateDouble(arg1 / num2->value.doubleFP);
    case OPERATOR_MODULUS:
      return sexpCreateDouble(fmod(arg1, num2->value.doubleFP));
    default:
      printf("apply arithmetic operator: invalid operator type\n");
      throwException();
      return NULL;
    }
  }

  else if(num1->type == SEXP_TYPE_DOUBLE && num2->type == SEXP_TYPE_INTEGER) {
    double arg2 = (double)num2->value.integer;
    switch(operator)
    {
    case OPERATOR_PLUS:
      return sexpCreateDouble(num1->value.doubleFP + arg2);
    case OPERATOR_MINUS:
      return sexpCreateDouble(num1->value.doubleFP - arg2);
    case OPERATOR_MULTIPLY:
      return sexpCreateDouble(num1->value.doubleFP * arg2);
    case OPERATOR_DIVIDE:
      return sexpCreateDouble(num1->value.doubleFP / arg2);
    case OPERATOR_MODULUS:
      return sexpCreateDouble(fmod(num1->value.doubleFP, arg2));
    default:
      printf("apply arithmetic operator: invalid operator type\n");
      throwException();
      return NULL;
    }
  }

  else {
    printf("! unsupported use of arithmetic operator on provided values\n");
    throwException();
    return NULL;
  }
}

Sexp applyOperator(Sexp num1, Sexp num2, Operator operator)
{
  if(!num1 || !num2) {
    printf("apply operator: either num1 or num2 is null\n");
    throwException();
    return NULL;
  }

  switch(operator)
  {
    // equality operators
  case OPERATOR_EQUAL:
  case OPERATOR_LESS:
  case OPERATOR_LESS_EQUAL:
  case OPERATOR_GREATER:
  case OPERATOR_GREATER_EQUAL:
    return applyEqualityOperator(num1, num2, operator);

    // arithmetic operators
  case OPERATOR_PLUS:
  case OPERATOR_MINUS:
  case OPERATOR_MULTIPLY:
  case OPERATOR_DIVIDE:
  case OPERATOR_MODULUS:
    return applyArithmeticOperator(num1, num2, operator);

  default:
    printf("operator application: invalid operator type\n");
    throwException();
    return NULL;
  }
}



#endif // PLD_LISP_OPERATOR_APPLICATION_H
