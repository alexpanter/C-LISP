#ifndef PLD_LISP_REGEX_BUILD_H
#define PLD_LISP_REGEX_BUILD_H

#include <stdlib.h>
#include <string.h>
#include "regex_types.h"

Regex regexBuild(const char* string)
{
  int len = strlen(string);
  Regex regex = regexAlloc();

  for(int i = 0; i < len; i++)
  {
    char ch = string[i];
    if('A' <= ch && ch <= 'z') {
      regex->type = REGEX_TYPE_SYMBOL;
      regex->value.symbol = ch;
    }
    else if('0' <= ch && ch <= '9') {
      regex->type = REGEX_TYPE_DIGIT;
      regex->value.digit = ch;
    }
  }

  return regex;
}


#endif // PLD_LISP_REGEX_BUILD_H
