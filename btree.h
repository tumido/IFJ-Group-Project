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
#include "ilist.h"

/*  Struktura pro binarni strom
 * --------------------------------------------------------------------
 * - obsahuje klic, podle ktereho je ve stromu razena, odkaz na
 *   podstromy a dalsi data...
 */

struct node
{
  char keyValue[BUFSIZE]; // klic jmeno funkce, promenne
  void * data;
  void * funcData;
  bool defined;
  key type;

  struct node * leftNode;
  struct node * rightNode;
};

typedef struct
{
  struct node * local;
  struct node * global;
} btree;

/*   Data v uzlu pro funkci
 * ---------------------------------------------------------------------
 * - specifike data v uzlu uchovavane pro funkci
 * - parametry funkce jsou uchovavane jako linearni seznam
 */
struct funcParam
{
  char keyValue[BUFSIZE];
  key type;
  struct funcParam * next;
};

typedef struct
{
  key retType; // navratovy typ
  btree * table; // lokalni tablulka symbolu
  struct funcParam * param; // linearni seznam parametru
  tListOfInstr ilist; // list instrukci funkce
  bool defined; // byla definovana?
} funcData;


/*   Vytvoreni tabulky symbolu
 * ---------------------------------------------------------------------
 * - lze provest jednoduchym, lec ucinnym
 *   struct node * global = NULL;
 */
void SymbolTableInit(btree * table);
void SymbolTableInitLocal(btree * table, btree * global);

/*   Vytvoreni uzlu z dat
 * ---------------------------------------------------------------------
 * - z poskytnutych dat vytvorim uzel pro tabulku symbolu
 * - vytvatim uzly pro promenne a pro funkce
 */
struct node * SymbolTableCreateNode(char * name, key type, void * data);
struct node * SymbolTableCreateFunctionNode(char * name, key type, struct funcParam * param, bool defined);

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
int FunctionParamsListDispose(struct funcParam * paramList);

/*   Vyhledani prvku v tabulce symbolu
 * ---------------------------------------------------------------------
 * - prohleda tabulku, nejspis bude rozhodovat jestli pre/in/postorder
 * - vraci ukazatel na uzel pokud je hledany prvek nalezen nebo NULL
 */
struct node * SymbolTableSearch(btree * table, char * key);
struct node * __SymbolTableSearch(struct node * leaf, char * key);

int SymbolTableCopy(btree * tableOriginal, btree * tableNew);
struct node * __SymbolTableCopy(struct node * nd);


int SymbolTableRestore(btree * tableOriginal, btree * tableNew, char * key);
int __SymbolTableRestore(struct node * ndO, struct node * nd, char * key);
#endif
