/*
 * =====================================================================
 *          Verze:  1.0
 *      Vytvoreno:  11/12/2014 04:23:00 PM
 *         Autori:  Tomáš Coufal, Roman Halík, Yurij Hladyuk, Jakub Jochlík
 *          Login:  xcoufa09, xhalik01, xhlady00, xjochl00
 *        Projekt:  IFJ
 * =====================================================================
 */

#ifndef BTREE_INCLUDED
#define BTREE_INCLUDED
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "io.h"
#include "strings.h"
#define BUFSIZE 256

/*  Struktura pro binarni strom
 * --------------------------------------------------------------------
 * - obsahuje klic, podle ktereho je ve stromu razena, odkaz na
 *   podstromy a dalsi data...
 */

struct node
{
  char keyValue[BUFSIZE]; // klic jmeno funkce, promenne
  void * data;
  lexType type;

  struct node * leftNode;
  struct node * rightNode;
};

typedef struct
{
  struct node * root;
  struct node * last;
} btree;

struct funcData
{
  lexType retType;
  unsigned int numberOfParams;
  btree * localTable;
  bool defined;
};

/*   Vytvoreni tabulky symbolu
 * ---------------------------------------------------------------------
 * - lze provest jednoduchym, lec ucinnym
 *   struct node * root = NULL;
 */
void SymbolTableInit(btree * table);


struct node * SymbolTableCreateNode(char * name, key type);

/*   Vlozeni noveho prvku do tabulky symbolu
 * ---------------------------------------------------------------------
 * - zapoji novy prvek do tabulky symbolu na spravne misto
 * - vstupnimy parametry funkce jsou tabulka symbolu a prvek, ktery ma
 *   byt vlozen
 */
int SymbolTableInsert(btree * table, struct node * insert);
int __SymbolTableInsert(struct node ** leaf, struct node * insert);

/*   Zruseni tabulky symbolu
 * ---------------------------------------------------------------------
 * - zrusi tabulku symbolu (nerekurzivne z duvodu rychlosti)
 */
int SymbolTableDispose(btree * table);
int __SymbolTableDispose(struct node ** leaf);

/*   Vyhledani prvku v tabulce symbolu
 * ---------------------------------------------------------------------
 * - prohleda tabulku, nejspis bude rozhodovat jestli pre/in/postorder
 * - vraci ukazatel na uzel pokud je hledany prvek nalezen nebo NULL
 */
struct node * SymbolTableSearch(btree * table, char * key);
struct node * __SymbolTableSearch(struct node * leaf, char * key);

#endif
