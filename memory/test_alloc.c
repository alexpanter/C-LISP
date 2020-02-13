#include "alloc.h"
#include <assert.h>

void testLargeAllocation()
{
  const size_t SIZE = 10000000;
  void* ptr = memoryAlloc(SIZE);
  assert(memoryGetTotalAllocated() == SIZE);

  memoryFree(ptr);
  assert(memoryGetTotalAllocated() == 0);
}
void testManyAllocations()
{
  const size_t MANY = 1000;
  const size_t SIZE = 64;
  void* allocations[MANY];
  for(size_t i = 0; i < MANY; i++)
  {
    void* ptr = memoryAlloc(SIZE);
    assert(memoryGetTotalAllocated() == (i+1)*SIZE);
    allocations[i] = ptr;
  }
  const size_t TOTAL = MANY * SIZE;
  for(size_t i = 0; i < MANY; i++)
  {
    assert(memoryGetTotalAllocated() == TOTAL - i*SIZE);
    memoryFree(allocations[i]);
  }
  assert(memoryGetTotalAllocated() == 0);
}


int main()
{
  testLargeAllocation();
  testManyAllocations();

  return 0;
}
