#include "improvedHashtable.h"

int main()
{
  Hashtable hashtable = hashtableCreate();


  hashtableInsert(hashtable, "!hej", 123u);
  hashtableInsert(hashtable, "hej", 1234u);
  hashtableInsert(hashtable, "dinner", 12345u);
  hashtableInsert(hashtable, "mormor", 123456u);
  hashtableInsert(hashtable, "mormor.", 1234567u);
  hashtableInsert(hashtable, ".mormor", 12345678u);
  hashtableInsert(hashtable, "bamse", 123456789u);


  hashtableInsert(hashtable, "abcdef", 9999u);
  hashtableInsert(hashtable, "bcdefa", 8888u);
  hashtableInsert(hashtable, "cdefab", 7777u);
  hashtableInsert(hashtable, "defabc", 6666u);

  hashtablePrint(hashtable);

  return 0;
}
