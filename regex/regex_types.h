#ifndef PLD_LISP_REGEX_TYPES_H
#define PLD_LISP_REGEX_TYPES_H

#include <stdlib.h>
#include <stdio.h>

/* regex types */
union _regex_value_t {
  void* epsilon;
  char symbol;
  char digit;
  union _regex_value_t* or[2];
  char range[2];
};

enum _regex_type_t {
  REGEX_TYPE_EPSILON,
  REGEX_TYPE_SYMBOL,
  REGEX_TYPE_DIGIT,
  REGEX_TYPE_OR,
  REGEX_TYPE_RANGE
};

struct _regex_t {
  enum _regex_type_t type;
  union _regex_value_t value;
};



/* typedefs for easy usage */
typedef struct _regex_t* Regex;



/* functions for regex types */
Regex regexAlloc()
{
  return malloc(sizeof(struct _regex_t));
}

Regex regexCreateEpsilon()
{
  Regex regex = regexAlloc();
  regex->type = REGEX_TYPE_EPSILON;
  regex->value.epsilon = (void*)NULL;
  return regex;
}

Regex regexCreateSymbol(char symbol)
{
  if(!('A' <= symbol && symbol <= 'z')) {
    printf("regex create symbol: symbol is not within [a-zA-Z] valid range\n");
    return NULL;
  }
  Regex regex = regexAlloc();
  regex->type = REGEX_TYPE_SYMBOL;
  regex->value.symbol = symbol;
  return regex;
}

Regex regexCreateDigit(char digit)
{
  if(!('0' <= digit && digit <= '9')) {
    printf("regex create digit: digit is not within [0-9] valid range\n");
    return NULL;
  }
  Regex regex = regexAlloc();
  regex->type = REGEX_TYPE_DIGIT;
  regex->value.digit = digit;
  return regex;
}

void regexPrintDebug(Regex regex)
{
  switch(regex->type)
  {
  case REGEX_TYPE_EPSILON:
    printf("Epsilon");
    break;
  case REGEX_TYPE_SYMBOL:
    printf("Symbol \"%c\"", regex->value.symbol);
    break;
  case REGEX_TYPE_OR:
    printf("regex print debug: or not implemented\n");
    break;
  case REGEX_TYPE_RANGE:
    printf("regex print debug: range not implemented\n");
    break;
  default:
    printf("regex print debug: invalid regex type\n");
  }
}



#endif // PLD_LISP_REGEX_TYPES_H


/*

"a"  ->  Symbol a


 */
