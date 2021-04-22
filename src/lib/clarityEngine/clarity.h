#ifndef CSS_CSC_H
#define CSS_CSC_H

#include <ctype.h>
#include "./css/css.h"
#include "./css/css_file.h"

void CSC_extends(struct CSSObject_t *base, struct CSSObject_t *super)
{
  struct CSSProperty_t *temp = super->propertyList->head;
  while (temp != NULL)
  {
    _addProperty(base, temp->propertyName, temp->propertyValue);
    temp = temp->next;
  }
}

void CSC_split(char *prop, char *token[2])
{
  token[0] = strtok(prop, ":");
  token[1] = strtok(NULL, ":");
  token[1] = strtok(token[1], ";");
}

/**
 * non ho voglia di fare la mia versione
 * 
 * https://stackoverflow.com/questions/122616/how-do-i-trim-leading-trailing-whitespace-in-a-standard-way
*/
char *CSC_trim(char *str)
{
  char *end;

  // Trim leading space
  while (isspace((unsigned char)*str))
    str++;

  if (*str == 0) // All spaces?
    return str;

  // Trim trailing space
  end = str + strlen(str) - 1;
  while (end > str && isspace((unsigned char)*end))
    end--;

  // Write new null terminator character
  end[1] = '\0';

  return str;
}

#endif
