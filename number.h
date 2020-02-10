#ifndef PLD_LISP_NUMBER_H
#define PLD_LISP_NUMBER_H

#include <string.h>
#include <stdio.h>

#define MAX_NUMBER_LENGTH_IN_CHAR_BUFFER 32



void numberPrintDouble(double number)
{
  char buffer[32];
  memset(buffer, '0', 32);
  int written = snprintf(buffer, 32, "%f", number);
  int dot;
  for(dot = 0; dot < 32; dot++) {
    if(buffer[dot] == '.') break;
  }
  int trailing = dot + 1;
  for(int i = written - 1; i > dot; i--) {
    if(buffer[i] != '0') {
      trailing = i;
      break;
    }
  }
  buffer[trailing + 1] = '\0';
  printf("%s", buffer);
}



#endif // PLD_LISP_NUMBER_H
