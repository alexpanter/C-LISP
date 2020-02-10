#ifndef PLD_LISP_KEYWORD_H
#define PLD_LISP_KEYWORD_H

#include <string.h>

/* keyword types */
enum _keyword_t {
  KEYWORD_QUOTE,
  KEYWORD_LAMBDA,
  KEYWORD_DEFINE,
  KEYWORD_CONS,
  KEYWORD_SAVE,
  KEYWORD_LOAD,
  KEYWORD_EQUALS, // (equals a b) -- requires definition of values!
  KEYWORD_TRUE,
  KEYWORD_FALSE,
  // TODO: KEYWORD_WHEN ?
  KEYWORD_IF, // (if cond e1 e2) == if cond then e1 else e2
  KEYWORD_NOT,
  KEYWORD_MESSAGE, // (message "<format>" args...)
  KEYWORD_LET,
  KEYWORD_IN
};



/* typedefs for easy usage */
typedef enum _keyword_t Keyword;



/* keyword functions */
Keyword keywordMatch(const char* symbol)
{
  if(!strcmp(symbol,      "quote"))   return KEYWORD_QUOTE;
  else if(!strcmp(symbol, "lambda"))  return KEYWORD_LAMBDA;
  else if(!strcmp(symbol, "define"))  return KEYWORD_DEFINE;
  else if(!strcmp(symbol, "cons"))    return KEYWORD_CONS;
  else if(!strcmp(symbol, "save"))    return KEYWORD_SAVE;
  else if(!strcmp(symbol, "load"))    return KEYWORD_LOAD;
  else if(!strcmp(symbol, "equals"))  return KEYWORD_EQUALS;
  else if(!strcmp(symbol, "true"))    return KEYWORD_TRUE;
  else if(!strcmp(symbol, "false"))   return KEYWORD_FALSE;
  else if(!strcmp(symbol, "if"))      return KEYWORD_IF;
  else if(!strcmp(symbol, "not"))     return KEYWORD_NOT;
  else if(!strcmp(symbol, "message")) return KEYWORD_MESSAGE;
  else if(!strcmp(symbol, "let"))     return KEYWORD_LET;
  else if(!strcmp(symbol, "in"))      return KEYWORD_IN;
  else return -1;
}



#endif // PLD_LISP_KEYWORD_H
