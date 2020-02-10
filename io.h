#ifndef PLD_LISP_IO_H
#define PLD_LISP_IO_H

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "exception.h"

/* file contents types */
struct _filecontents_line_t {
  unsigned int linelength;
  char* line;
  struct _filecontents_line_t* next;
};

struct _filecontents_t {
  struct _filecontents_line_t* head;
};



/* typedefs for easy usage */
typedef struct _filecontents_line_t* FileContentsLine;
typedef struct _filecontents_t* FileContents;



/* global file contents to prevent memory leaks to due exception handling */
FileContents globalFileContents;



/* utility functions for file contents types */
FileContentsLine fileContentsLineCreate()
{
  FileContentsLine line = malloc(sizeof(struct _filecontents_line_t));
  line->linelength = 0;
  line->line = NULL;
  line->next = NULL;
  return line;
}

void fileContentsLineFree(FileContentsLine line)
{
  if(!line) {
    printf("file contents line free: line is null\n");
    return;
  }
  if(!line->line) {
    printf("file contents line free: line->line is null\n");
    return;
  }
  free(line->line);
  free(line);
}

void fileContentsFree()
{
  if(!globalFileContents) {
    return;
  }
  FileContentsLine line = globalFileContents->head;
  while(line)
  {
    FileContentsLine next = line->next;
    fileContentsLineFree(line);
    line = next;
  }
  free(globalFileContents);
}

FileContents fileContentsCreate()
{
  if(globalFileContents) {
    fileContentsFree();
  }
  FileContents contents = malloc(sizeof(struct _filecontents_t));
  contents->head = NULL;
  return contents;
}

void fileContentsAddLine(FileContents contents, const char* text)
{
  if(!contents) {
    printf("file contents add line: contents is null\n");
    return;
  }

  FileContentsLine line = fileContentsLineCreate();
  unsigned long len = strlen(text);
  line->linelength = len;
  line->line = malloc(sizeof(char) * len);
  strcpy(line->line, text);

  if(!contents->head) {
    contents->head = line;
  }
  else {
    line->next = contents->head;
    contents->head = line;
  }
}



/* functions for saving/loading LISP library files */
FileContents libraryLoad(const char* filename)
{
  FILE *file = fopen (filename, "r");
  if(!file) {
    printf("! could not open file %s\n", filename);
    throwException();
    printf("Control should not reach this point!\n");
    return NULL;
  }
  FileContents contents = fileContentsCreate();
  unsigned int bufsize = 128;
  char* input = malloc(sizeof(char) * bufsize);

  char c = EOF;
  unsigned int i = 0;
  while((c = fgetc(file)) != EOF) {
    if(i >= bufsize) {
      bufsize *= 2;
      char* newBuffer = malloc(sizeof(char) * bufsize);
      memset(newBuffer, '\0', bufsize);
      strcpy(newBuffer, input);
      free(input);
      input = newBuffer;
    }
    else if(c == '\n' || c == ';') {
      input[i] = '\0';
      fileContentsAddLine(contents, input);
      memset(input, '\0', i);
      i = 0;
      // skip until next newline
      if(c == ';') {
        while((c = fgetc(file)) != '\n') {}
      }
      continue;
    }
    else {
      input[i] = c;
      i++;
    }
  }
  free(input);
  return contents;
}

/* more clever function, which does the same as libraryLoad,
 * but separates LISP statements based on parantheses instead
 * of lines.
 */
//void librarySmartReadString(F)

FileContents librarySmartLoad(const char* filename)
{
  FILE *file = fopen (filename, "r");
  if(!file) {
    printf("! could not open file %s\n", filename);
    throwException();
    printf("Control should not reach this point!\n");
    return NULL;
  }
  FileContents contents = fileContentsCreate();
  unsigned int bufsize = 256;
  char* input = malloc(sizeof(char) * bufsize);

  char c = EOF;
  unsigned int i = 0;
  int scopeBalance = 0;
  int whitespace = 0;
  while((c = fgetc(file)) != EOF)
  {
    // no more space in buffer
    if(i >= bufsize - 1) {
      bufsize *= 2;
      fileContentsAddLine(contents, input);
      char* newBuffer = malloc(sizeof(char) * bufsize);
      strcpy(newBuffer, input);
      free(input);
      input = newBuffer;
    }

    // line comment -> skip until newline
    if(c == ';') {
      while((c = fgetc(file)) != '\n')
      {
        if(c == EOF) break;
      }
      whitespace = 0;
      continue;
    }

    // ignore whitespaces and scope balances within strings
    if(c == '"') {
      input[i] = c;
      i++;
      while((c = fgetc(file)) != '"')
      {
        if(c == EOF) break;
        input[i] = c;
        i++;
      }
      input[i] = c;
      i++;
      /* whitespace = 0; */
      continue;
    }

    // ignore extra whitespace characters
    if(c == ' ' || c == '\n' || c == '\t' || c == '\r') {
      if(whitespace) {
        continue;
      }
      whitespace = 1;
      c = ' ';
    }

    // parantheses -> scope checking
    else if(c == '(') {
      scopeBalance++;
      whitespace = 0;
    }
    else if(c == ')') {
      scopeBalance--;
      if(scopeBalance < 0) {
        printf("! syntax error: unmatched parantheses\n");
        throwException();
        printf("Control should not reach this point!\n");
        return NULL;
      }
      whitespace = 0;
    }
    else {
      whitespace = 0;
    }

    input[i] = c;
    i++;

    // finished reading S-expression
    if(scopeBalance == 0 && i > 1) {
      input[i+1] = '\0';
      fileContentsAddLine(contents, input);
      /* printf("NEW LINE: \"%s\"\n", input); */
      memset(input, '\0', bufsize);
      whitespace = 0;
      i = 0;
    }
  }
  free(input);
  return contents;
}


/* read from stdin for REPL user input */
char* inputBufferRead()
{
  unsigned int bufsize = 64;
  char* buffer = malloc(bufsize * sizeof(char));
  unsigned int index = 0;

  char ch;
  while(read(STDIN_FILENO, &ch, 1) > 0)
  {
    if(ch == '\n') break;

    // reallocate
    if(index >= bufsize - 1) { // space for terminating '\0'
      bufsize *= 2;
      char* newbuffer = malloc(bufsize * sizeof(char));
      strcpy(newbuffer, buffer);
      free(buffer);
      buffer = newbuffer;
    }

    buffer[index] = ch;
    index++;
  }
  buffer[index] = '\0';
  return buffer;
}

void inputBufferFree(char* buffer)
{
  if(!buffer) {
    printf("input buffer free: buffer is null\n");
    return;
  }
  free(buffer);
}

#endif // PLD_LISP_IO_H
