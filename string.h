#ifndef PLD_LISP_STRING_H
#define PLD_LISP_STRING_H

#include "sexp.h"
#include "exception.h"

int stringArgumentIsConsInteger(const Sexp sexp)
{
  // verify that current pointer into args is indeed an integer type
  return sexp && sexp->type == SEXP_TYPE_CONS &&
         sexp->value.cons[0]->type == SEXP_TYPE_INTEGER;
}

int stringArgumentIsConsBoolean(const Sexp sexp)
{
  // verify that current pointer into args is indeed a boolean type
  return sexp && sexp->type == SEXP_TYPE_CONS &&
         sexp->value.cons[0]->type == SEXP_TYPE_BOOLEAN;
}

int stringArgumentIsConsString(const Sexp sexp)
{
  // verify that current pointer into args is indeed a string type
  return sexp && sexp->type == SEXP_TYPE_CONS &&
         sexp->value.cons[0]->type == SEXP_TYPE_STRING;
}

int stringArgumentIsList(const Sexp sexp)
{
  // verify that current pointer into args is indeed a list type
  return sexp && sexp->type == SEXP_TYPE_CONS &&
         (sexp->value.cons[0]->type == SEXP_TYPE_CONS ||
          sexp->value.cons[0]->type == SEXP_TYPE_NIL);
}

Sexp stringAdvanceArgsPointer(const Sexp argsPointer)
{
  if(!argsPointer) {
    printf("string advance args pointer: argsPointer is null\n");
    return NULL;
  }

  switch(argsPointer->type)
  {
  case SEXP_TYPE_NIL:
    return NULL;

  case SEXP_TYPE_CONS:
    return argsPointer->value.cons[1];

  case SEXP_TYPE_BOOLEAN:
  case SEXP_TYPE_INTEGER:
  case SEXP_TYPE_OPERATOR:
  case SEXP_TYPE_SYMBOL:
  case SEXP_TYPE_STRING:
    printf("! malformed message argument list\n");
    throwException();
    return NULL;

  default:
    printf("string advance args pointer: invalid args type\n");
    throwException();
    return NULL;
  }
}

void stringPrintMessage(const char* format, Sexp args)
{
  Sexp sexp = args;
  unsigned int len = strlen(format);
  unsigned int i = 0;

  while(i < len)
  {
    if(format[i] == '%' && i < len - 1) {
      switch(format[i + 1])
      {
      case 'i':
        if(!stringArgumentIsConsInteger(sexp)) {
          printf("! message argument did not match integer type\n");
          return;
        }
        else {
          printf("%i", sexp->value.cons[0]->value.integer);
          sexp = stringAdvanceArgsPointer(sexp);
        }
        break;

      case 'b':
        if(!stringArgumentIsConsBoolean(sexp)) {
          printf("! message argument did not match boolean type\n");
          return;
        }
        else {
          printf("%s", (sexp->value.cons[0]->value.boolean) ? "true" : "false");
          sexp = stringAdvanceArgsPointer(sexp);
        }
        break;

      case 's':
        if(!stringArgumentIsConsString(sexp)) {
          printf("! message argument did not match string type\n");
          return;
        }
        else {
          printf("%s", sexp->value.cons[0]->value.string);
          sexp = stringAdvanceArgsPointer(sexp);
        }
        break;

      case 'l':
        if(!stringArgumentIsList(sexp)) {
          printf("! message argument did not match list type\n");
          return;
        }
        else {
          sexpPrint(sexp->value.cons[0]);
          sexp = stringAdvanceArgsPointer(sexp);
        }
        break;

      case '%':
        break;

      default:
        printf("string print message: arg type not supported\n");
        return;
      }
      i += 2;
    }
    else {
      // TODO: Most likely, will be much faster to write to a buffer and
      // print this buffer when it's full.
      putchar(format[i]);
      i++;
    }
  }
  printf("\n");
}


#endif // PLD_LISP_STRING_H
