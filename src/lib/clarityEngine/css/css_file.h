#ifndef CSS_FILE_H
#define CSS_FILE_H

#include <stdio.h>
#include "../../utils/stdutils.h"
#include "css.h"

struct CSSFile_t
{
  /**
   * @brief il nome del file
  */
  char *name;

  /**
   * @brief il puntatore al file
  */
  FILE *fp;
};

/**
 * @brief inizializzazione
*/
struct CSSFile_t *initCSSFile(string_t);

/**
 * @brief aggiunge un oggetto css ad un file 
*/
void writeCSSObject(struct CSSFile_t *, struct CSSObject_t *);

/**
 * @brief aggiunge una lista di oggetti css ad un file 
*/
void writeCSSObjectList(struct CSSFile_t *, struct CSSObjectList_t *);

/**
 * @brief chiusura del file
*/
void closeFile(struct CSSFile_t *);

/////////////////////////////////////////////////////////

/**
 * @brief corpo delle funzioni
*/

/**
 * @brief CSSFile_t
*/

struct CSSFile_t *initCSSFile(string_t CSSFileName)
{
  struct CSSFile_t *CSSFile = alloc_p(struct CSSFile_t, 1);

  CSSFile->name = alloc_p(char, 100);
  strcpy(CSSFile->name, CSSFileName);
  CSSFile->fp = fopen(CSSFile->name, "w+");

  return CSSFile;
}

void writeCSSObject(struct CSSFile_t *CSSFile, struct CSSObject_t *CSSObject)
{
  struct CSSProperty_t *head = CSSObject->propertyList->head;

  fprintf(CSSFile->fp, "%s {\n", CSSObject->selector);
  while (head != NULL)
  {
    fprintf(CSSFile->fp, "\t%s: %s;\n", head->propertyName, head->propertyValue);

    head = head->next;
  }
  fprintf(CSSFile->fp, "}\n\n");
}

void writeCSSObjectList(struct CSSFile_t *CSSFile, struct CSSObjectList_t *CSSObjectList)
{
  struct CSSObject_t *head = CSSObjectList->head;
  while (head != NULL)
  {
    writeCSSObject(CSSFile, head);
    head = head->next;
  }
}

void closeFile(struct CSSFile_t *CSSFile)
{
  fclose(CSSFile->fp);
}

#endif