#ifndef PLD_LISP_LEX_H
#define PLD_LISP_LEX_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "keyword.h"
#include "operator.h"
#include "exception.h"

/* token types */
enum _lex_token_special_char_t {
  LEX_TOKEN_SPECIALCHAR_LPAR,
  LEX_TOKEN_SPECIALCHAR_RPAR,
  LEX_TOKEN_SPECIALCHAR_DOT,
  LEX_TOKEN_SPECIALCHAR_APOSTROPHE
};

enum _lex_token_type_t {
  LEX_TOKEN_TYPE_KEYWORD,
  LEX_TOKEN_TYPE_SPECIALCHAR,
  LEX_TOKEN_TYPE_SYMBOL,
  LEX_TOKEN_TYPE_INTEGER,
  LEX_TOKEN_TYPE_DOUBLE,
  LEX_TOKEN_TYPE_OPERATOR,
  LEX_TOKEN_TYPE_STRING
};

union _lex_token_value_t {
  Keyword keyword;
  Operator operator;
  enum _lex_token_special_char_t special_char;
  char* symbol;
  int integer;
  double doubleFP;
  char* string;
};

struct _lex_token_t {
  enum _lex_token_type_t type;
  union _lex_token_value_t value;
} _lex_token_t;



/* token list types */
struct _lex_token_list_element_t {
  struct _lex_token_t* token;
  struct _lex_token_list_element_t* next;
};

// singly linked list with a tail pointer for insertion
struct _lex_token_list_t {
  struct _lex_token_list_element_t* head;
  struct _lex_token_list_element_t* tail;
};



/* typedefs for easy usage */
typedef struct _lex_token_t* LexToken;
typedef struct _lex_token_list_t* LexTokenList;
typedef struct _lex_token_list_element_t* LexTokenListElement;
typedef enum _lex_token_special_char_t LexTokenSpecialchar;
typedef enum _lex_token_type_t LexTokenType;



/* token type functions */
LexToken lexTokenAlloc()
{
  return malloc(sizeof(struct _lex_token_t));
}

LexToken lexTokenCreateKeyword(Keyword keyword)
{
  LexToken token = lexTokenAlloc();
  token->type = LEX_TOKEN_TYPE_KEYWORD;
  token->value.keyword = keyword;
  return token;
}

LexToken lexTokenCreateSpecialchar(LexTokenSpecialchar specialchar)
{
  LexToken token = lexTokenAlloc();
  token->type = LEX_TOKEN_TYPE_SPECIALCHAR;
  token->value.special_char = specialchar;
  return token;
}

LexToken lexTokenCreateSymbol(char* symbol)
{
  LexToken token = lexTokenAlloc();
  token->type = LEX_TOKEN_TYPE_SYMBOL;
  token->value.symbol = malloc(strlen(symbol) * sizeof(char));
  strcpy(token->value.symbol, symbol);
  return token;
}

LexToken lexTokenCreateInteger(int integer)
{
  LexToken token = lexTokenAlloc();
  token->type = LEX_TOKEN_TYPE_INTEGER;
  token->value.integer = integer;
  return token;
}

LexToken lexTokenCreateDouble(double doubleFP)
{
  LexToken token = lexTokenAlloc();
  token->type = LEX_TOKEN_TYPE_DOUBLE;
  token->value.doubleFP = doubleFP;
  return token;
}

LexToken lexTokenCreateOperator(Operator operator)
{
  LexToken token = lexTokenAlloc();
  token->type = LEX_TOKEN_TYPE_OPERATOR;
  token->value.operator = operator;
  return token;
}

LexToken lexTokenCreateString(char* string)
{
  LexToken token = lexTokenAlloc();
  token->type = LEX_TOKEN_TYPE_STRING;
  token->value.string = malloc(strlen(string) * sizeof(char));
  strcpy(token->value.string, string);
  return token;
}

void lexTokenFree(LexToken token)
{
  switch(token->type)
  {
  case LEX_TOKEN_TYPE_KEYWORD:     break;
  case LEX_TOKEN_TYPE_SPECIALCHAR: break;
  case LEX_TOKEN_TYPE_SYMBOL:
    free(token->value.symbol);
    break;
  case LEX_TOKEN_TYPE_INTEGER:     break;
  case LEX_TOKEN_TYPE_DOUBLE:      break;
  case LEX_TOKEN_TYPE_OPERATOR:    break;
  case LEX_TOKEN_TYPE_STRING:
    free(token->value.string);
    break;
  default:
    printf("lex token free: Invalid token type!\n"); // exit(-1);
  }
  free(token);
}

void lexTokenPrint(LexToken token)
{
  switch(token->type)
  {
  case LEX_TOKEN_TYPE_KEYWORD:
    switch(token->value.keyword)
    {
    case KEYWORD_QUOTE:   printf("QUOTE");   break;
    case KEYWORD_LAMBDA:  printf("LAMBDA");  break;
    case KEYWORD_DEFINE:  printf("DEFINE");  break;
    case KEYWORD_CONS:    printf("CONS");    break;
    case KEYWORD_SAVE:    printf("SAVE");    break;
    case KEYWORD_LOAD:    printf("LOAD");    break;
    case KEYWORD_EQUALS:  printf("EQUALS");  break;
    case KEYWORD_TRUE:    printf("TRUE");    break;
    case KEYWORD_FALSE:   printf("FALSE");   break;
    case KEYWORD_IF:      printf("IF");      break;
    case KEYWORD_NOT:     printf("NOT");     break;
    case KEYWORD_MESSAGE: printf("MESSAGE"); break;
    case KEYWORD_LET:     printf("LET");     break;
    case KEYWORD_IN:      printf("IN");      break;
    default:
      printf("lex token print: Invalid keyword!\n"); // exit(-1);
    }
    break;
  case LEX_TOKEN_TYPE_SPECIALCHAR:
    switch(token->value.special_char)
    {
    case LEX_TOKEN_SPECIALCHAR_LPAR:       printf("LPAR");       break;
    case LEX_TOKEN_SPECIALCHAR_RPAR:       printf("RPAR");       break;
    case LEX_TOKEN_SPECIALCHAR_DOT:        printf("DOT");        break;
    case LEX_TOKEN_SPECIALCHAR_APOSTROPHE: printf("APOSTROPHE"); break;
    default:
      printf("lex token print: Invalid specialchar!\n"); // exit(-1);
    }
    break;
  case LEX_TOKEN_TYPE_SYMBOL:
    printf("SYMBOL(%s)", token->value.symbol);
    break;
  case LEX_TOKEN_TYPE_INTEGER:
    printf("INT(%i)", token->value.integer);
    break;
  case LEX_TOKEN_TYPE_DOUBLE:
    printf("DOUBLE(%lg)", token->value.doubleFP);
    break;
  case LEX_TOKEN_TYPE_OPERATOR:
    printf("OPERATOR("); operatorPrint(token->value.operator); printf(")");
    break;
  case LEX_TOKEN_TYPE_STRING:
    printf("STRING(\"%s\")", token->value.string);
    break;
  default:
    printf("lex token print: Invalid token type!\n"); // exit(-1);
  }
}



/* token list functions */
LexTokenList lexTokenListCreate()
{
  LexTokenList list = malloc(sizeof(struct _lex_token_list_t));
  list->head = NULL;
  list->tail = NULL;
  return list;
}

void lexTokenListFree(LexTokenList list)
{
  LexTokenListElement element = list->head;
  while(element)
  {
    LexTokenListElement next = element->next;
    lexTokenFree(element->token);
    element = next;
  }
  free(list);
}

void lexTokenListAdd(LexTokenList list, LexToken token)
{
  if(!list->head) {
    list->head = malloc(sizeof(struct _lex_token_list_element_t));
    list->head->token = token;
    list->head->next = NULL;
    list->tail = list->head;
    return;
  }
  LexTokenListElement new = malloc(sizeof(struct _lex_token_list_element_t));
  new->token = token;
  new->next = NULL;
  list->tail->next = new;
  list->tail = new;
}

void lexTokenListPrint(LexTokenList list)
{
  printf("[");
  LexTokenListElement element = list->head;
  while(element)
  {
    if(!element->next) {
      lexTokenPrint(element->token);
    } else {
      lexTokenPrint(element->token);
      printf(", ");
    }
    element = element->next;
  }
  printf("]\n");
}



/* token utility functions */

/*
 * int lexReadIntFromBuffer() :
 *
 * @dest : buffer where to copy read bytes
 * @src : buffer from which to read the integer
 * @len : input buffer length
 * @i : starting read index in src buffer
 * returns: number of bytes read, will be 0 on error
 */
void lexBadInteger()
{
  printf("! malformed integer constant\n");
  throwException();
}
void lexBadDouble()
{
  printf("! malformed floating-point constant\n");
  throwException();
}
int lexReadNumberFromBuffer(char dest[32], const char* src,
                         unsigned int len, unsigned int i,
                         int* isFloatingPoint)
{
  int ret = 0;
  if(i < 0 || i >= len) {
    printf("lex read int from buffer: invalid index i\n");
    throwException();
    return 0;
  }
  *isFloatingPoint = 0;

  // reading negative number
  if(src[i] == '-' && i < len - 1 && src[i + 1] != '0') {
    dest[0] = '-';
    ret++;
    for(int k = 1; k < 32; k++)
    {
      if(src[i + k] == '.') {
        (*isFloatingPoint)++;
        dest[k] = '.';
        ret++;
        if(*isFloatingPoint > 1) { lexBadDouble(); }
      }

      else if('a' <= src[i + k] && src[i + k] <= 'z') {
        if(*isFloatingPoint) { lexBadDouble(); }
        else { lexBadInteger(); }
        return 0;
      }

      else if('0' <= src[i + k] && src[i + k] <= '9') {
        dest[k] = src[i + k];
        ret++;
      }

      else { break; }
    }
  }

  // reading floating-point number between 0 and 1
  else if(src[i] == '0' && i < len - 1 && src[i + 1] == '.') {
    for(int k = 0; k < 32; k++)
    {
      if(src[i + k] == '.') {
        (*isFloatingPoint)++;
        dest[k] = '.';
        ret++;
        if(*isFloatingPoint > 1) { lexBadDouble(); }
      }

      else if('0' <= src[i + k] && src[i + k] <= '9') {
        dest[k] = src[i + k];
        ret++;
      }

      else if('a' <= src[i + k] && src[i + k] <= 'z') {
        lexBadDouble();
        return 0;
      }

      else { break; }
    }
  }

  // reading 0
  else if(src[i] == '0') {
    if(i < len - 1) {
      if('a' <= src[i + 1] && src[i + 1] <= 'z') {
        lexBadInteger();
        return 0;
      }

      else if('0' <= src[i + 1] && src[i + 1] <= '9') {
        lexBadInteger();
        return 0;
      }

      else if(src[i + 1] == '.') {
        lexBadInteger();
        return 0;
      }

      dest[0] = '0';
      ret++;
    }
    else {
      dest[0] = '0';
      ret++;
    }
  }

  // reading positive number
  else if('1' <= src[i] && src[i] <= '9') {
    dest[0] = src[i];
    ret++;
    for(int k = 1; k < 32; k++)
    {
      if(src[i + k] == '.') {
        (*isFloatingPoint)++;
        dest[k] = '.';
        ret++;
        if(*isFloatingPoint > 1) { lexBadDouble(); }
      }

      else if('0' <= src[i + k] && src[i + k] <= '9') {
        dest[k] = src[i + k];
        ret++;
      }

      else if('a' <= src[i + k] && src[i + k] <= 'z') {
        lexBadInteger();
        return 0;
      }

      else { break; }
    }
  }

  else {
    (*isFloatingPoint > 0) ? lexBadDouble() : lexBadInteger();
  }
  dest[ret] = '\0';
  return ret;
}

void lexBadString()
{
  printf("! malformed string\n");
  throwException();
}

char* lexReadStringFromBuffer(const char* src, unsigned int len, unsigned int i)
{
  unsigned int bufsize = 64;
  char* dest = malloc(sizeof(char) * bufsize);
  dest[0] = '\0';

  if(i < 0 || i >= len) {
    lexBadString();
    free(dest);
    throwException();
    return NULL;
  }

  if(src[i] != '"') {
    lexBadString();
    free(dest);
    throwException();
    return NULL;
  }
  int stringNotClosed = 1; // is the beginning '"' matched by a closing '"'

  unsigned int k = 0; // for insertion
  unsigned int index = i + 1; // for reading
  while(index < len)
  {
    if(k >= bufsize - 1) {
      bufsize *= 2;
      char* newbuf = malloc(sizeof(char) * bufsize);
      strcpy(newbuf, dest);
      free(dest);
      dest = newbuf;
    }

    if(src[index] == '"') {
      stringNotClosed = 0;
      break;
    }
    else {
      dest[k] = src[index];
      k++;
      index++;
    }
  }

  if(stringNotClosed) {
    lexBadString();
    free(dest);
    throwException();
    return NULL;
  }

  dest[k] = '\0';
  return dest;
}

LexTokenList transformBufferToTokenList(char* buffer)
{
  LexTokenList list = lexTokenListCreate();

  unsigned int len = strlen(buffer);
  for(unsigned int i = 0; i < len; i++)
  {
    if(buffer[i] == '(') {
      lexTokenListAdd(list, lexTokenCreateSpecialchar
                      (LEX_TOKEN_SPECIALCHAR_LPAR));
      continue;
    }
    if(buffer[i] == ')') {
      lexTokenListAdd(list, lexTokenCreateSpecialchar
                      (LEX_TOKEN_SPECIALCHAR_RPAR));
      continue;
    }
    if(buffer[i] == '.') {
      lexTokenListAdd(list, lexTokenCreateSpecialchar
                      (LEX_TOKEN_SPECIALCHAR_DOT));
      continue;
    }
    if(buffer[i] == '\'') {
      lexTokenListAdd(list, lexTokenCreateSpecialchar
                      (LEX_TOKEN_SPECIALCHAR_APOSTROPHE));
      continue;
    }
    if(buffer[i] == ' ') {
      continue;
    }
    if(buffer[i] == '<' && i < len - 1 && buffer[i + 1] == '=') {
      lexTokenListAdd(list, lexTokenCreateOperator(OPERATOR_LESS_EQUAL));
      i++;
      continue;
    }
    if(buffer[i] == '>' && i < len - 1 && buffer[i + 1] == '=') {
      lexTokenListAdd(list, lexTokenCreateOperator(OPERATOR_GREATER_EQUAL));
      i++;
      continue;
    }
    if(buffer[i] == '=') {
      lexTokenListAdd(list, lexTokenCreateOperator(OPERATOR_EQUAL));
      continue;
    }
    if(buffer[i] == '<' && buffer[i]) {
      lexTokenListAdd(list, lexTokenCreateOperator(OPERATOR_LESS));
      continue;
    }
    if(buffer[i] == '>' && buffer[i]) {
      lexTokenListAdd(list, lexTokenCreateOperator(OPERATOR_GREATER));
      continue;
    }
    if(buffer[i] == '+') {
      lexTokenListAdd(list, lexTokenCreateOperator(OPERATOR_PLUS));
      continue;
    }
    if(buffer[i] == '-' && i < len - 1 && buffer[i + 1] == ' ') {
      lexTokenListAdd(list, lexTokenCreateOperator(OPERATOR_MINUS));
      i++;
      continue;
    }
    if(buffer[i] == '*' && i < len - 1 && buffer[i + 1] == '*') {
      lexTokenListAdd(list, lexTokenCreateOperator(OPERATOR_POWER));
      i++;
      continue;
    }
    if(buffer[i] == '*') {
      lexTokenListAdd(list, lexTokenCreateOperator(OPERATOR_MULTIPLY));
      continue;
    }
    if(buffer[i] == '/') {
      lexTokenListAdd(list, lexTokenCreateOperator(OPERATOR_DIVIDE));
      continue;
    }
    if(buffer[i] == '%') {
      lexTokenListAdd(list, lexTokenCreateOperator(OPERATOR_MODULUS));
      continue;
    }
    if('a' <= buffer[i] && buffer[i] <= 'z') {
      char keywordOrSymbol[128];
      keywordOrSymbol[0] = buffer[i];
      i++;

      for(unsigned int k = 1; k < 128; k++)
      {
        if('a' <= buffer[i] && buffer[i] <= 'z') {
          keywordOrSymbol[k] = buffer[i];
          i++;
          continue;
        }
        keywordOrSymbol[k] = '\0';
        break;
      }
      i--; // do not advance 1 character too much

      // if the string cannot be matched as a keyword, it must be a symbol
      switch(keywordMatch(keywordOrSymbol)) {
      case KEYWORD_QUOTE:
        lexTokenListAdd(list, lexTokenCreateKeyword(KEYWORD_QUOTE));
        continue;
      case KEYWORD_LAMBDA:
        lexTokenListAdd(list, lexTokenCreateKeyword(KEYWORD_LAMBDA));
        continue;
      case KEYWORD_DEFINE:
        lexTokenListAdd(list, lexTokenCreateKeyword(KEYWORD_DEFINE));
        continue;
      case KEYWORD_CONS:
        lexTokenListAdd(list, lexTokenCreateKeyword(KEYWORD_CONS));
        continue;
      case KEYWORD_SAVE:
        lexTokenListAdd(list, lexTokenCreateKeyword(KEYWORD_SAVE));
        continue;
      case KEYWORD_LOAD:
        lexTokenListAdd(list, lexTokenCreateKeyword(KEYWORD_LOAD));
        continue;
      case KEYWORD_EQUALS:
        lexTokenListAdd(list, lexTokenCreateKeyword(KEYWORD_EQUALS));
        continue;
      case KEYWORD_TRUE:
        lexTokenListAdd(list, lexTokenCreateKeyword(KEYWORD_TRUE));
        continue;
      case KEYWORD_FALSE:
        lexTokenListAdd(list, lexTokenCreateKeyword(KEYWORD_FALSE));
        continue;
      case KEYWORD_IF:
        lexTokenListAdd(list, lexTokenCreateKeyword(KEYWORD_IF));
        continue;
      case KEYWORD_NOT:
        lexTokenListAdd(list, lexTokenCreateKeyword(KEYWORD_NOT));
        continue;
      case KEYWORD_MESSAGE:
        lexTokenListAdd(list, lexTokenCreateKeyword(KEYWORD_MESSAGE));
        continue;
      case KEYWORD_LET:
        lexTokenListAdd(list, lexTokenCreateKeyword(KEYWORD_LET));
        continue;
      case KEYWORD_IN:
        lexTokenListAdd(list, lexTokenCreateKeyword(KEYWORD_IN));
        continue;
      default:
        lexTokenListAdd(list, lexTokenCreateSymbol(keywordOrSymbol));
        continue;
      }
    }

    // not a symbol, and not a character -> match an integer
    else if(buffer[i] == '-' || ('0' <= buffer[i] && buffer[i] <= '9')) {
      char number[32];
      int isFloatingPoint;
      int readNumber = lexReadNumberFromBuffer(number, buffer, len, i,
                                            &isFloatingPoint);
      if(!readNumber) {
        lexTokenListFree(list);
        (isFloatingPoint) ? lexBadDouble() : lexBadInteger();
        return NULL;
      }
      i += readNumber - 1; // do not advance 1 character too much
      /* printf("readNumber: %s\n", number); */

      int intValue;
      double doubleValue;
      if(isFloatingPoint && sscanf(number, "%lg", &doubleValue) == 1) {
        lexTokenListAdd(list, lexTokenCreateDouble(doubleValue));
        continue;
      }
      else if(sscanf(number, "%i", &intValue) == 1) {
        lexTokenListAdd(list, lexTokenCreateInteger(intValue));
        continue;
      }
      else {
        lexTokenListFree(list);
        (isFloatingPoint) ? lexBadDouble() : lexBadInteger();
        return NULL;
      }
    }

    else if(buffer[i] == '"' && i < len - 1) {
      char* string = lexReadStringFromBuffer(buffer, len, i);
      if(!string) {
        lexTokenListFree(list);
        lexBadString();
        return NULL;
      }
      else {
        i += strlen(string) + 1; // skip the ending '"'
        lexTokenListAdd(list, lexTokenCreateString(string));
        free(string);
        continue;
      }
    }

    // control should not reach this point, unless none of the above cases were true
    // so we print an error message indicating a lexical error
    /* printf("Lexical error on position %i\n", i); */
    return NULL;
  }
  return list;
}



#endif // PLD_LISP_LEX_H
