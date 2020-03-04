#ifndef PLD_LISP_MEMORY_MANAGER_H
#define PLD_LISP_MEMORY_MANAGER_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define MEM_SIZE_DEFAULT 512
#define MEM_SIZE_STRING_DEFAULT 32

/* Implementation of dynamic array */
struct _dynamic_array_t {
  size_t size;
  size_t capacity;
  void* data;
};



/* typedefs for easy usage */
typedef struct _dynamic_array_t* DynamicArray;

DynamicArray memoryManagerCreateDynamicArray(size_t size)
{
  DynamicArray da = malloc(sizeof(struct _dynamic_array_t));
  da->data = malloc(size);
  da->capacity = size;
  da->size = 0;
  return da;
}



/* global data structures for easy usage */
DynamicArray __string_manager__;



char* memoryManagerCreateStringFromLength(size_t length)
{
  const size_t totalLength = length + 1; // saving space for null character
  if(__string_manager__->size + totalLength > __string_manager__->capacity) {
    // saving old values
    void* data = __string_manager__->data;
    size_t size = __string_manager__->size;

    // update the string manager
    __string_manager__->capacity *= 2;
    __string_manager__->size += totalLength;
    __string_manager__->data = malloc(__string_manager__->capacity * 2);
    memcpy(__string_manager__->data, data, size);
    void* ptr = &((char*)__string_manager__->data)[size];
    return ptr;
  } else {
    void* ptr = &((char*)__string_manager__->data)[__string_manager__->size];
    __string_manager__->size += totalLength;
    return ptr;
  }
}

char* memoryManagerCreateString(const char* str)
{
  const size_t length = strlen(str) + 1; // saving space for null character
  if(__string_manager__->size + length > __string_manager__->capacity) {
    // saving old values
    void* data = __string_manager__->data;
    size_t size = __string_manager__->size;

    // update the string manager
    __string_manager__->capacity *= 2;
    __string_manager__->size += length;
    __string_manager__->data = malloc(__string_manager__->capacity * 2);
    memcpy(__string_manager__->data, data, size);
    char* ptr = &((char*)__string_manager__->data)[size];
    strncpy(ptr, str, length);
    return ptr;
  } else {
    void* ptr = &((char*)__string_manager__->data)[__string_manager__->size];
    strncpy(ptr, str, length);
    __string_manager__->size += length;
    return ptr;
  }
}



void memoryManagerInit()
{
  __string_manager__ = memoryManagerCreateDynamicArray(MEM_SIZE_STRING_DEFAULT);
}

void memoryManagerPrintStrings()
{
  char* ptr = __string_manager__->data;
  size_t sz = __string_manager__->size;

  for(size_t i = 0; i < sz; i++)
  {
    printf("%i %c\n", *ptr, *ptr);
    //putchar(*ptr);
    //if(*ptr == '\0') { putchar('\n'); }
    ptr++;
  }
}


#undef MEM_SIZE_DEFAULT

#endif // PLD_LISP_MEMORY_MANAGER_H
