#ifndef IMPROVED_HASHTABLE_H
#define IMPROVED_HASHTABLE_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define HASHTABLE_SIZE 2069 // a prime number

struct _linked_list_t {
  struct _linked_list_t* next;
  char* key;
  unsigned int data;
};

struct _hashtable_t {
  struct _linked_list_t** entries;
};

typedef struct _linked_list_t* LinkedList;
typedef struct _hashtable_t* Hashtable;

/* utility functions */
unsigned int hashtableHashFunc(const char* str)
{
  // hash = hashfunc (key)
  // index = hash % array_size

  unsigned long sum = 0ul;
  int len = strlen(str);

  for(int i = 0; i < len; i++)
  {
    sum += (i + 1) * str[i];
  }
  return sum % HASHTABLE_SIZE;
}

Hashtable hashtableCreate()
{
  Hashtable table = malloc(sizeof(struct _hashtable_t));
  table->entries = malloc(sizeof(struct _linked_list_t) * HASHTABLE_SIZE);
  for(int i = 0; i < HASHTABLE_SIZE; i++)
  {
    table->entries[i] = malloc(sizeof(struct _linked_list_t));
    table->entries[i]->key = NULL;
    table->entries[i]->data = 0;
    table->entries[i]->next = NULL;
  }
  return table;
}

void hashtableInsert(Hashtable hashtable, const char* key, unsigned int data)
{
  unsigned int index = hashtableHashFunc(key);
  int len = strlen(key);

  LinkedList list = hashtable->entries[index];
  LinkedList newList = malloc(sizeof(struct _linked_list_t));
  newList->next = list;
  newList->key = malloc(sizeof(char) * len);
  newList->data = data;
  strncpy(newList->key, key, len);

  hashtable->entries[index] = newList;
}

void hashtablePrint(Hashtable hashtable)
{
  for(int i = 0; i < HASHTABLE_SIZE; i++)
  {
    LinkedList list = hashtable->entries[i];
    while(list != NULL && list->key != NULL)
    {
      printf("%s --> %i\n", list->key, list->data);
      list = list->next;
    }
  }
}



#endif // IMPROVED_HASHTABLE_H
