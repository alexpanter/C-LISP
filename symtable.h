#ifndef PLD_LISP_SYMTABLE_H
#define PLD_LISP_SYMTABLE_H

#include "sexp.h"
#include "exception.h"


/* jump buffer for exception handling */
extern jmp_buf jumpbuffer;



/* symbol table types */
struct _symtable_binding_t {
  char* symbol;
  Sexp value;
};

struct _symtable_element_t {
  struct _symtable_binding_t* binding;
  struct _symtable_element_t* next;
};

struct _symtable_t {
  struct _symtable_element_t* head;
};



/* typedefs for easy usage */
typedef struct _symtable_binding_t* SymtableBinding;
typedef struct _symtable_element_t* SymtableElement;
typedef struct _symtable_t* Symtable;



/* symbol table functions */
SymtableBinding symtableBindingAlloc()
{
  return malloc(sizeof(struct _symtable_binding_t));
}

SymtableElement symtableElementAlloc()
{
  return malloc(sizeof(struct _symtable_element_t));
}

SymtableBinding symtableBindingCreate(const char* symbol, Sexp sexp)
{
  SymtableBinding binding = symtableBindingAlloc();
  binding->symbol = malloc(strlen(symbol) * sizeof(char));
  strcpy(binding->symbol, symbol);
  binding->value = sexpCopy(sexp);
  return binding;
}

void symtableBindingFree(SymtableBinding binding)
{
  if(!binding) {
    printf("symtable binding free: binding is null\n");
    return;
  }
  if(binding->symbol) free(binding->symbol);
  if(binding->value) sexpFree(binding->value);
  free(binding);
}

SymtableBinding symtableBindingCopy(SymtableBinding binding)
{
  return symtableBindingCreate(binding->symbol, binding->value);
}

Symtable symtableCreate()
{
  Symtable symtable = malloc(sizeof(struct _symtable_t));
  symtable->head = NULL;
  return symtable;
}

void symtableFree(Symtable symtable)
{
  if(!symtable) {
    printf ("symtable free: Symtable is null\n");
    return;
  }
  SymtableElement element = symtable->head;
  while(element)
  {
    symtableBindingFree(element->binding);
    SymtableElement next = element->next;
    free(element);
    element = next;
  }
  free(symtable);
}

void symtablePrint(Symtable symtable)
{
  if(!symtable) {
    printf ("symtable print: Symtable is null\n");
    return;
  }
  SymtableElement element = symtable->head;
  while(element)
  {
    printf("%s |--> ", element->binding->symbol);
    sexpPrint(element->binding->value);
    printf("\n");
    element = element->next;
  }
}

int symtableContainsBinding(Symtable symtable, const char* symbol)
{
  SymtableElement element = symtable->head;
  while(element)
  {
    if(!strcmp(element->binding->symbol, symbol)) {
      return 1;
    }
    element = element->next;
  }
  return 0;
}

Sexp symtableLookupSymbol(Symtable symtable, const char* symbol)
{
  SymtableElement element = symtable->head;
  while(element)
  {
    if(!strcmp(element->binding->symbol, symbol)) {
      return sexpCopy(element->binding->value);
    }
    element = element->next;
  }
  return NULL;
}

int symtableDisjoint(Symtable symtable1, Symtable symtable2)
{
  SymtableElement element1 = symtable1->head;
  while(element1)
  {
    SymtableElement element2 = symtable2->head;
    while(element2)
    {
      if(!strcmp(element1->binding->symbol, element2->binding->symbol)) {
        printf("! repeated variable %s in pattern\n",
               element1->binding->symbol);
        throwException();
        printf("Control should not reach this point!\n");
        return 0;
      }
      element2 = element2->next;
    }
    element1 = element1->next;
  }
  return 1;
}

// if symbol is already bound keep it but store the new in front
void symtableUpdate(Symtable symtable, const char* symbol, Sexp value)
{
  SymtableBinding newBinding = symtableBindingCreate(symbol, value);

  // symtable is empty
  if(!symtable->head) {
    SymtableElement newElement = symtableElementAlloc();
    newElement->binding = newBinding;
    newElement->next = NULL;
    symtable->head = newElement;
    return;
  }

  // the first binding in symtable matches the symbol
  if(!strcmp(symtable->head->binding->symbol, symbol)) {
    SymtableElement newElement = symtableElementAlloc();
    newElement->binding = newBinding;
    newElement->next = symtable->head;
    symtable->head = newElement;
    return;
  }

  // iterate until the position before binding should be inserted
  SymtableElement element = symtable->head;
  while(element)
  {
    if(!element->next) {
      break;
    }
    if(!strcmp(symbol, element->next->binding->symbol)) {
      break;
    }
    element = element->next;
  }

  // bind symbol after current position
  SymtableElement newElement = symtableElementAlloc();
  newElement->binding = newBinding;
  newElement->next = element->next;
  element->next = newElement;
  return;
}

Symtable symtableCopy(Symtable symtable)
{
  Symtable newSymtable = symtableCreate();
  SymtableElement newPtr = newSymtable->head;

  // insert symtable 1
  SymtableElement element = symtable->head;
  while(element)
  {
    SymtableElement newElement = symtableElementAlloc();
    newElement->binding = symtableBindingCopy(element->binding);
    if(!newSymtable->head) {
      newSymtable->head = newElement;
      newPtr = newElement;
    } else {
      newPtr->next = newElement;
      newPtr = newElement;
    }

    element = element->next;
  }

  return newSymtable;
}

Symtable symtableCombine(Symtable symtable1, Symtable symtable2)
{
  Symtable newSymtable = symtableCreate();
  SymtableElement newPtr = newSymtable->head;

  // insert symtable 1
  SymtableElement element = symtable1->head;
  while(element)
  {
    SymtableElement newElement = symtableElementAlloc();
    newElement->binding = symtableBindingCopy(element->binding);
    if(!newSymtable->head) {
      newSymtable->head = newElement;
      newPtr = newElement;
    } else {
      newPtr->next = newElement;
      newPtr = newElement;
    }

    element = element->next;
  }

  // insert symtable 2
  element = symtable2->head;
  while(element)
  {
    SymtableElement newElement = symtableElementAlloc();
    newElement->binding = symtableBindingCopy(element->binding);
    if(!newSymtable->head) {
      newSymtable->head = newElement;
      newPtr = newElement;
    } else {
      newPtr->next = newElement;
      newPtr = newElement;
    }

    element = element->next;
  }

  return newSymtable;
}



/* declaring symbol table as global */
Symtable globalEnvironment;



#endif // PLD_LISP_SYMTABLE_H
