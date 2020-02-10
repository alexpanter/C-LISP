#ifndef PLD_LISP_HASHTABLE_H
#define PLD_LISP_HASHTABLE_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define HASH_TABLE_SIZE 128

struct _hashtable_t;

enum _hashtable_entry_type_t {
  HASHTABLE_ENTRY_TYPE_EMPTY,
  HASHTABLE_ENTRY_TYPE_HASHTABLE,
  HASHTABLE_ENTRY_TYPE_STRING
};

union _hashtable_entry_value_t {
  struct _hashtable_t* hashtable;
  char* string;
};

struct _hashtable_entry_t {
  enum _hashtable_entry_type_t type;
  union _hashtable_entry_value_t* value;
};

struct _hashtable_t {
  struct _hashtable_entry_t entries[HASH_TABLE_SIZE];
};



/* typedefs for easy usage */
typedef struct _hashtable_t* Hashtable;
typedef struct _hashtable_entry_t* HashtableEntry;



/* utility functions */
Hashtable hashtableCreate()
{
  Hashtable table = malloc(sizeof(struct _hashtable_t));
  for(int i = 0; i < HASH_TABLE_SIZE; i++)
  {
    table->entries[i].type = HASHTABLE_ENTRY_TYPE_EMPTY;
    table->entries[i].value = NULL;
  }
  return table;
}


void hashtableInsert(Hashtable hashtable, const char* string)
{
  unsigned long len = strlen(string);
  if(len < 1) { return; }

  HashtableEntry entry = &hashtable->entries[(int)string[0]];

  if(entry->type == HASHTABLE_ENTRY_TYPE_EMPTY)
  {
    entry->type = HASHTABLE_ENTRY_TYPE_STRING;
    entry->value = malloc(sizeof(struct _hashtable_entry_t));
    entry->value->string = malloc(sizeof(char) * len);

    strncpy(entry->value->string, string, len);
  }
  else if(entry->type == HASHTABLE_ENTRY_TYPE_STRING)
  {
    if(!strcmp(string, entry->value->string)) { return; }
    char* existing = entry->value->string;

    entry->type = HASH
  }
}


void hashtablePrint(Hashtable hashtable)
{
  for(int i = 0; i < HASH_TABLE_SIZE; i++)
  {
    HashtableEntry entry = &hashtable->entries[i];
    if(entry->type != HASHTABLE_ENTRY_TYPE_EMPTY)
    {
      char* str = entry->value->string;
      printf("%s\n", str);
    }
  }
}



#endif // PLD_LISP_HASHTABLE_H
