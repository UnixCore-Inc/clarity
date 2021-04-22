#ifndef CSS_H
#define CSS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../utils/stdutils.h"

/**
 * @brief questa struct rappresenta una singola proprietà css
*/
struct CSSProperty_t
{
  /**
   * @brief il nome della proprietà, es "color", "display", ecc
  */
  char *propertyName;

  /**
   * @brief il valore della proprietà, es "red", "block", ecc
  */
  char *propertyValue;

  /**
   * @brief siccome il tutto è stato fatto con le liste concatenate,
   *        questo elemento punta al prossimo elemento
  */
  struct CSSProperty_t *next;
};

/**
 * @brief crea una proprietà, con i valori definiti 
 *        dai parametri
*/
struct CSSProperty_t *newProperty(string_t, string_t);

/////////////////////////////////////////////////////////

/**
 * @brief una lista completa di proprietà
*/
struct CSSPropertyList_t
{
  /**
   * @brief siamo in una lista concatenata, 
   *        quindi questo elemento sarà la testa 
   *        della lista concatenata
  */
  struct CSSProperty_t *head;
};

/**
 * @brief inizializzazione della lista 
*/
struct CSSPropertyList_t *initCSSPropertyList();

/**
 * @brief aggiunge una proprietà alla lista
*/
void addProperty(struct CSSPropertyList_t *, struct CSSProperty_t *);

/**
 * @brief scrive in output tutta la lista a 
 *        partire da un nodo
*/
void printPropertyListFrom(struct CSSProperty_t *);

/**
 * @brief scrive in output la lista
*/
void printPropertyList(struct CSSPropertyList_t *);

/////////////////////////////////////////////////////////

/**
 * @brief una classe css completa, quindi con il nome
 *        del selettore e le proprietà
*/
struct CSSObject_t
{
  char *selector;
  struct CSSPropertyList_t *propertyList;
  struct CSSObject_t *next;
};

/**
 * @brief inizializzazione della classe con il nome del selettore
*/
struct CSSObject_t *initCSSObject(string_t);

/**
 * @brief per utilità più che altro
*/
void _addProperty(struct CSSObject_t *, string_t, string_t);

/**
 * @brief scrive in output la classe e tutte le sue proprietà
*/
void dbg(struct CSSObject_t *);

/**
 * @brief dio merd
*/
void clear(struct CSSObject_t *);

/////////////////////////////////////////////////////////

/**
 * @brief lista di oggetti
*/
struct CSSObjectList_t
{
  /**
   * @brief siamo in una lista concatenata, 
   *        quindi questo elemento sarà la testa 
   *        della lista concatenata
  */
  struct CSSObject_t *head;
};

/**
 * @brief inizializza la lista di oggetti
*/
struct CSSObjectList_t *initObjectList();

/**
 * @brief aggiunge un oggetto
*/
void addObject(struct CSSObjectList_t *, struct CSSObject_t *);

/**
 * @brief ottiene un oggetto, parametro: il nome dell oggetto
*/
struct CSSObject_t *getObject(struct CSSObjectList_t *, string_t);

/////////////////////////////////////////////////////////

/**
 * 
 * @brief CORPO DI TUTTE LE FUNZIONI
 * 
*/

/**
 * @brief CSSProperty_t
*/

struct CSSProperty_t *newProperty(string_t property, string_t value)
{
  struct CSSProperty_t *object = alloc_p(struct CSSProperty_t, 1);

  object->propertyName = alloc_p(char, 100);
  object->propertyValue = alloc_p(char, 100);

  strcpy(object->propertyName, property);
  strcpy(object->propertyValue, value);

  return object;
}

/**
 * @brief CSSPropertyList_t
*/

struct CSSPropertyList_t *initCSSPropertyList()
{
  return alloc_p(struct CSSPropertyList_t, 1);
}

void addProperty(struct CSSPropertyList_t *list, struct CSSProperty_t *property)
{
  property->next = list->head;
  list->head = property;
}

void printPropertyListFrom(struct CSSProperty_t *startProperty)
{
  if (startProperty == NULL)
    return;
  printf("  %s: %s;\n", startProperty->propertyName, startProperty->propertyValue);
  printPropertyListFrom(startProperty->next);
}

void printPropertyList(struct CSSPropertyList_t *list)
{
  printPropertyListFrom(list->head);
}

/**
 * @brief CSSObject_t
*/

struct CSSObject_t *initCSSObject(string_t name)
{
  struct CSSObject_t *object = alloc_p(struct CSSObject_t, 1);

  object->selector = alloc_p(char, 100);

  strcpy(object->selector, name);
  object->propertyList = initCSSPropertyList();

  return object;
}

void _addProperty(struct CSSObject_t *object, string_t property, string_t value)
{
  addProperty(object->propertyList, newProperty(property, value));
}

void dbg(struct CSSObject_t *object)
{
  printf("%s {\n", object->selector);
  printPropertyList(object->propertyList);
  printf("}\n");
}

void clear(struct CSSObject_t *CSSObject)
{
  free(CSSObject);
}

/**
 * @brief CSSObjectList_t
*/

struct CSSObjectList_t *initObjectList()
{
  return alloc_p(struct CSSObjectList_t, 1);
}

void addObject(struct CSSObjectList_t *list, struct CSSObject_t *object)
{
  object->next = list->head;
  list->head = object;
}

struct CSSObject_t *getObject(struct CSSObjectList_t *list, string_t selector)
{
  struct CSSObject_t *temp = list->head;
  while (temp != NULL)
  {
    if (!strcmp(temp->selector, selector))
      break;
    temp = temp->next;
  }
  return temp;
}

#endif