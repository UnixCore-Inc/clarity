#ifndef CSC_PARSER_LIB
#define CSC_PARSER_LIB

#include <string.h>

void watcha_END(CSCParser *);
void watcha_STATUS(CSCParser *);

void watcha_END(CSCParser *parser)
{   
  switch (parser->currentState)
  {
    case END:
      // fine  
      break;
    case ON_BLOCK:
      printf("yeah");
      SET_ERROR(parser, "NU BLOCK");
      break;
    case FUNCTION_CALL:
      SET_ERROR(parser, "NU FUNCTION CALL");
      break;
  }
}

void watcha_STATUS(CSCParser *parser)
{
  int *readChar = &parser->parserInfo.currentChar;
  ParserStates[parser->currentState](parser);
}

#endif