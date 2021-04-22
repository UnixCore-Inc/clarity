#ifndef CSC_PARSER_STATE_H
#define CSC_PARSER_STATE_H

#include <string.h>

#include "CSCParser.h"
#include "../clarityEngine/clarity.h"
#include "../clarityEngine/css/css.h"
#include "../clarityEngine/css/css_file.h"

#define INCLUDE_KEYWORD "include"
#define STATE_KEYWORD "state"
#define LIB_KEYWORD "lib"
#define EXTENDS_KEYWORD "extends"
#define EVENT_KEYWORD "->"

#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_BLUE "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN "\x1b[36m"
#define ANSI_COLOR_RESET "\x1b[0m"

#define TRACE_ERROR(p)   \
  p->currentState = END; \
  fprintf(stderr, "Errore alla linea: %d, LOG: %s%s%s", p->parserInfo.currentLine, ANSI_COLOR_RED, p->currentError, ANSI_COLOR_RESET);

#define SWITCH_STATE(p, state)       \
  p->previusState = p->currentState; \
  p->currentState = state;

#define INCREASE_TOKEN(parser) \
  parser->currentToken[parser->parserInfo.currentIndex++] = parser->parserInfo.currentChar;

#define TOKEN_EQ(parser, str) \
  !strcmp(parser->currentToken, str)

#define RESET_CURRENT_TOKEN(parser)                \
  memset(parser->currentToken, 0, MAX_TOKEN_SIZE); \
  parser->parserInfo.currentIndex = 0;

#define SET_ERROR(p, error) \
  p->currentState = ERROR;  \
  strcpy(p->currentError, error);

#define INCREASE_LINE_IF_NEW(parser)          \
  if (parser->parserInfo.currentChar == '\n') \
  {                                           \
    parser->parserInfo.currentLine++;         \
    return;                                   \
  }

typedef void (*ParserState)(CSCParser *);
ParserState ParserStates[PARSER_STATE_SIZE] = {};

void initParserStates(struct CSSFile_t *);

void _START(CSCParser *parser)
{
  switch (parser->parserInfo.currentChar)
  {
  case '\n':
  case '\r':
    parser->parserInfo.currentLine++;
    break;
  case '$':
    SWITCH_STATE(parser, ON_DIRECTIVE);
    break;
  case '.':
  case '#':
  default:
    INCREASE_TOKEN(parser);
    SWITCH_STATE(parser, ON_SELECTOR);
    break;
  }
}

void _ERROR(CSCParser *parser)
{
  TRACE_ERROR(parser);
}

void _END(CSCParser *parser)
{
  printf("FINE DIREI");
}

void _ON_BLOCK(CSCParser *parser)
{
  INCREASE_LINE_IF_NEW(parser);
  if (parser->parserInfo.currentChar == '{')
  {
    SET_ERROR(parser, "MAGARI CHIUDI LA GRAFA\n");
    RESET_CURRENT_TOKEN(parser);
    return;
  }
  else if (parser->parserInfo.currentChar == '}')
  {
    if (!parser->objectState->isState)
    {
      writeCSSObject(parser->objectState->file, parser->objectState->currentObject);
    }
    SWITCH_STATE(parser, EXIT_BLOCK);
    RESET_CURRENT_TOKEN(parser);
    return;
  }
  else if (parser->parserInfo.currentChar == '[')
  {
    SWITCH_STATE(parser, FUNCTION_CALL);
    RESET_CURRENT_TOKEN(parser);
    return;
  }
  else if (parser->parserInfo.currentChar == ';')
  {
    // printf("INSTR: %s\n", parser->currentToken);
    char *token[2];
    CSC_split(CSC_trim(parser->currentToken), token);
    _addProperty(parser->objectState->currentObject, token[0], token[1]);

    RESET_CURRENT_TOKEN(parser);
    return;
  }
  INCREASE_TOKEN(parser);
}

void _EXIT_BLOCK(CSCParser *parser)
{
  SWITCH_STATE(parser, START);
}

void _FUNCTION_CALL(CSCParser *parser)
{
  if (parser->parserInfo.currentChar == ']')
  {
    printf("FUNCTION CALL: %s\n", parser->currentToken);
    struct CSSObject_t *state = getObject(parser->objectState->objectList, parser->currentToken);
    if (state == NULL)
    {
      SET_ERROR(parser, "FUNZIONE NON DICHIARATA\n");
    }
    else
    {
      CSC_extends(parser->objectState->currentObject, state);
      SWITCH_STATE(parser, ON_BLOCK);
    }
    RESET_CURRENT_TOKEN(parser);
    return;
  }
  INCREASE_TOKEN(parser);
}

void _ON_DIRECTIVE(CSCParser *parser)
{
  INCREASE_LINE_IF_NEW(parser);
  if (parser->parserInfo.currentChar == ' ')
  {
    if (TOKEN_EQ(parser, INCLUDE_KEYWORD))
    {
      SWITCH_STATE(parser, DIRECTIVE_INCLUDE);
    }
    else if (TOKEN_EQ(parser, STATE_KEYWORD))
    {
      SWITCH_STATE(parser, DIRECTIVE_STATE);
    }
    else if (TOKEN_EQ(parser, LIB_KEYWORD))
    {
      SWITCH_STATE(parser, DIRECTIVE_LIB);
    }
    RESET_CURRENT_TOKEN(parser);
    return;
  }
  INCREASE_TOKEN(parser);
}

void _DIRECTIVE_INCLUDE(CSCParser *parser)
{
  if (parser->parserInfo.currentChar == '\n')
  {
    printf("INCLUDE %s\n", parser->currentToken);

    SWITCH_STATE(parser, START);
    RESET_CURRENT_TOKEN(parser);
    return;
  }
  INCREASE_TOKEN(parser);
}

void _DIRECTIVE_LIB(CSCParser *parser)
{
  // !lib
}

void _DIRECTIVE_STATE(CSCParser *parser)
{
  if (parser->parserInfo.currentChar == '{')
  {
    printf("STATE %s\n", CSC_trim(parser->currentToken));

    parser->objectState->currentObject = initCSSObject(parser->currentToken);
    parser->objectState->isState = true;
    addObject(parser->objectState->objectList, parser->objectState->currentObject);

    printf("AGGIUNTO %s\n", parser->objectState->currentObject->selector);
    SWITCH_STATE(parser, ON_BLOCK);
    RESET_CURRENT_TOKEN(parser);
    return;
  }
  INCREASE_TOKEN(parser);
}

void _ON_SELECTOR(CSCParser *parser)
{
  if (parser->parserInfo.currentChar == ' ')
  {
    if (TOKEN_EQ(parser, EXTENDS_KEYWORD))
    {
      SWITCH_STATE(parser, EXTENDS_CLASS);
      RESET_CURRENT_TOKEN(parser);
      return;
    }
    else if (TOKEN_EQ(parser, EVENT_KEYWORD))
    {
      SWITCH_STATE(parser, EVENT);
      RESET_CURRENT_TOKEN(parser);
      return;
    }
    else
    {
      printf("SELECTOR %s\n", CSC_trim(parser->currentToken));

      parser->objectState->currentObject = initCSSObject(parser->currentToken);
      parser->objectState->isState = false;
      addObject(parser->objectState->objectList, parser->objectState->currentObject);

      printf("AGGIUNTO %s\n", parser->objectState->currentObject->selector);
    }
    RESET_CURRENT_TOKEN(parser);
    return;
  }
  else if (parser->parserInfo.currentChar == '{')
  {
    // printf("{ %s\n", parser->currentToken);

    SWITCH_STATE(parser, ON_BLOCK);
    RESET_CURRENT_TOKEN(parser);
    return;
  }
  INCREASE_TOKEN(parser);
}

void _EXTENDS_CLASS(CSCParser *parser)
{
  if (parser->parserInfo.currentChar == ',' || parser->parserInfo.currentChar == '{')
  {
    printf("EXTENDED %s\n", CSC_trim(parser->currentToken));
    CSC_extends(parser->objectState->currentObject, getObject(parser->objectState->objectList, CSC_trim(parser->currentToken)));

    SWITCH_STATE(parser, parser->parserInfo.currentChar == ',' ? EXTENDS_CLASS : ON_BLOCK);
    RESET_CURRENT_TOKEN(parser);
    return;
  }
  INCREASE_TOKEN(parser);
}

void _EVENT(CSCParser *parser)
{
  if (parser->parserInfo.currentChar == '{')
  {
    printf("EVENT %s\n", parser->currentToken);

    SWITCH_STATE(parser, ON_BLOCK);
    RESET_CURRENT_TOKEN(parser);
    return;
  }
  INCREASE_TOKEN(parser);
}

void initParserStates(struct CSSFile_t *file)
{
  ParserStates[START] = _START;
  ParserStates[ERROR] = _ERROR;
  ParserStates[END] = _END;

  ParserStates[ON_BLOCK] = _ON_BLOCK;
  ParserStates[EXIT_BLOCK] = _EXIT_BLOCK;

  ParserStates[FUNCTION_CALL] = _FUNCTION_CALL;

  ParserStates[ON_DIRECTIVE] = _ON_DIRECTIVE;
  ParserStates[DIRECTIVE_INCLUDE] = _DIRECTIVE_INCLUDE;
  ParserStates[DIRECTIVE_LIB] = _DIRECTIVE_LIB;
  ParserStates[DIRECTIVE_STATE] = _DIRECTIVE_STATE;

  ParserStates[ON_SELECTOR] = _ON_SELECTOR;
  ParserStates[EXTENDS_CLASS] = _EXTENDS_CLASS;
  ParserStates[EVENT] = _EVENT;
}

#endif
