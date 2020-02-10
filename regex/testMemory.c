#include "memory.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct {
  int someInt;
  char someChar;
  double someDouble;
} hej;


int main()
{
  printf("sizeof(hej): %lu\n", sizeof(hej));
  printf("allocated: %lu\n", memoryGetTotalAllocated());

  hej* s1 = memoryAlloc(sizeof(hej));
  s1->someInt = 5;
  printf("allocated: %lu\n", memoryGetTotalAllocated());

  hej* s2 = memoryAlloc(sizeof(hej));
  s2->someInt = 33;
  printf("allocated: %lu\n", memoryGetTotalAllocated());

  hej* s3 = memoryAlloc(sizeof(hej));
  s3->someChar = 'a';
  printf("allocated: %lu\n", memoryGetTotalAllocated());

  hej* s4 = memoryAlloc(sizeof(hej));
  s4->someChar = 'k';
  printf("allocated: %lu\n", memoryGetTotalAllocated());

  hej* s5 = memoryAlloc(sizeof(hej));
  s5->someDouble = 44.3;
  printf("allocated: %lu\n", memoryGetTotalAllocated());

  memoryFree(s2);
  printf("\nallocated: %lu\n", memoryGetTotalAllocated());

  memoryFree(s3);
  printf("allocated: %lu\n", memoryGetTotalAllocated());

  memoryFree(s4);
  printf("allocated: %lu\n", memoryGetTotalAllocated());

  memoryFree(s5);
  printf("allocated: %lu\n", memoryGetTotalAllocated());

  memoryFree(s1);
  printf("allocated: %lu\n", memoryGetTotalAllocated());

  return 0;
}
