#include "regex_types.h"
#include "regex_build.h"

void test(const char* str)
{
  Regex reg = regexBuild(str);
  regexPrintDebug(reg);
  printf("\n");
}

int main()
{
  test("a"); test("A"); test("z"); test("Z");
  test("0"); test("9"); test("");

  return 0;
}
