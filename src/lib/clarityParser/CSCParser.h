#ifndef CSC_PARSER_H
#define CSC_PARSER_H

#include "../utils/stdutils.h"

#define MAX_TOKEN_SIZE 4096
#define MAX_ERROR_SIZE 4096
#define PARSER_STATE_SIZE 13

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef enum
{
  START,
  ERROR,
  END,

  ON_BLOCK,          // {
  EXIT_BLOCK,        // }

  FUNCTION_CALL,     // [function]

  ON_DIRECTIVE,      // $
  DIRECTIVE_INCLUDE, // $include
  DIRECTIVE_LIB,     // $lib
  DIRECTIVE_STATE,   // $state

  ON_SELECTOR,       // body, h1, div, .gian, #fil
  EXTENDS_CLASS,     // extends
  EVENT,             // ->
} CSCParserState;

/////////////////////////////////////////////////////////

typedef struct
{
  int currentChar;
  int currentIndex;
  int currentLine;
} CSCParserInfo;

/////////////////////////////////////////////////////////

typedef struct
{
  struct CSSFile_t *file;
  struct CSSObjectList_t *objectList;
  struct CSSObject_t *currentObject;
  bool isState;
} CSCParserObjectState;
CSCParserObjectState *initCSCParserObjectState(string_t);

/////////////////////////////////////////////////////////

typedef struct
{
  CSCParserState previusState;
  CSCParserState currentState;

  CSCParserInfo parserInfo;

  char currentToken[MAX_TOKEN_SIZE];
  char currentError[MAX_ERROR_SIZE];

  CSCParserObjectState *objectState;
} CSCParser;

/**
 * @brief inizializzazione del parser
*/
CSCParser *initParser(string_t);

/**
 * @brief parsing di un file di cui viene dato il nome 
*/
int parse(CSCParser *, string_t);

/////////////////////////////////////////////////////////

/**
 * @brief corpo delle funzioni
*/

/**
 * @brief CSCParser
*/

CSCParser *initParser(string_t filename)
{
  CSCParser *parser = alloc_p(CSCParser, 1);

  parser->currentState = START;
  parser->previusState = START;

  parser->parserInfo.currentIndex = 0;
  parser->parserInfo.currentLine = 0;

  parser->objectState = initCSCParserObjectState(filename);

  return parser;
}

#include "CSCParserState.h"
#include "CSCParserlib.h"

int parse(CSCParser *parser, string_t filename)
{
  initParserStates(initCSSFile("index.css"));
  FILE *fp = fopen(filename, "r");

  int *readChar = &parser->parserInfo.currentChar;
  while (parser->currentState != END)
  {
    *readChar = fgetc(fp);
    if (*readChar == EOF)
    {
      watcha_END(parser);
      break;
    }
    watcha_STATUS(parser);
  }

  fclose(fp);
  closeFile(parser->objectState->file);

  return parser->currentState == END;
}

/**
 * @brief CSCParserObjectState
*/

CSCParserObjectState *initCSCParserObjectState(string_t filename)
{
  CSCParserObjectState *objectState = alloc_p(CSCParserObjectState, 1);

  objectState->file = initCSSFile(filename);
  objectState->objectList = initObjectList();

  objectState->isState = false;

  return objectState;
}

#endif