#include "manager.h"


int main()
{
  memoryManagerInit();
  memoryManagerCreateString("hej");
  memoryManagerCreateString("goddag");
  char* test = memoryManagerCreateString("farvel");
  strcpy(test, "fuckaf");

  memoryManagerPrintStrings();

  return 0;
}
