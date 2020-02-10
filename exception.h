#ifndef PLD_LISP_EXCEPTION_H
#define PLD_LISP_EXCEPTION_H

#include <setjmp.h>
#include <stdio.h>

/* jump buffer */
jmp_buf jumpbuffer;

/* exception-handling */
void throwException()
{
  /* printf("caught exception\n"); */
  longjmp(jumpbuffer, 0);
  printf("Control should not reach this point\n");
}

#endif // PLD_LISP_EXCEPTION_H
