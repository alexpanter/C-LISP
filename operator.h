#ifndef PLD_LISP_OPERATOR_H
#define PLD_LISP_OPERATOR_H

#include <stdio.h>

/* keyword types */
enum _operator_t {
  OPERATOR_EQUAL,
  OPERATOR_LESS,
  OPERATOR_LESS_EQUAL,
  OPERATOR_GREATER,
  OPERATOR_GREATER_EQUAL,
  OPERATOR_PLUS,
  OPERATOR_MINUS,
  OPERATOR_MULTIPLY,
  OPERATOR_DIVIDE,
  OPERATOR_MODULUS
};



/* typedefs for easy usage */
typedef enum _operator_t Operator;



/* utility functions for operator type */
void operatorPrint(Operator operator)
{
  switch(operator)
  {
  case OPERATOR_EQUAL:         printf("=");  break;
  case OPERATOR_LESS:          printf("<");  break;
  case OPERATOR_LESS_EQUAL:    printf("<="); break;
  case OPERATOR_GREATER:       printf(">");  break;
  case OPERATOR_GREATER_EQUAL: printf(">="); break;
  case OPERATOR_PLUS:          printf("+");  break;
  case OPERATOR_MINUS:         printf("-");  break;
  case OPERATOR_MULTIPLY:      printf("*");  break;
  case OPERATOR_DIVIDE:        printf("/");  break;
  case OPERATOR_MODULUS:       printf("%%"); break;
  default:
    printf("operator print: invalid operator type\n");
    break;
  }
}



#endif // PLD_LISP_OPERATOR_H
