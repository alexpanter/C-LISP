#ifndef PLD_LISP_MEMORY_H
#define PLD_LISP_MEMORY_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MEM_SIZE (sizeof(size_t)) // will be 8 on a 64-bit system

static size_t totalBytesAllocated = 0;



/* memory management utility functions */

/*
 * In secret, allocate (sizeof(size_t)) bytes more than needed,
 * and then use the first (sizeof(size_t)) bytes to hold the size
 * of the allocated memory, for later counting amount of dynamically
 * allocated memory.
 */
void* memoryAlloc(size_t bytes)
{
  // how much we want to allocate, plus space for an
  // MEM_SIZE integer to hold the number of bytes.
  const size_t size = MEM_SIZE + bytes;
  size_t* ptr = malloc(size);

  // initialize all entries in the allocated block to 0, except
  // for the first MEM_SIZE bytes which holds the size of the block.
  memset(ptr, '\0', size);
  *ptr = bytes;

  // update the global memory counter
  totalBytesAllocated += bytes;

  void* ret = (void*)(ptr + MEM_SIZE); // heap allocations grow downwards
  return ret;
}

void memoryFree(void* ptr)
{
  if(!ptr) {
    printf("memory free: cannot free nullptr\n");
    exit(EXIT_FAILURE);
  }

  size_t* block = (size_t*)ptr - MEM_SIZE;
  printf("freeing block with size: %lu\n", *block);
  totalBytesAllocated -= *block;
  free((void*)block);
}

size_t memoryGetTotalAllocated()
{
  return totalBytesAllocated;
}


#undef MEM_SIZE
#endif // PLD_LISP_MEMORY_H
