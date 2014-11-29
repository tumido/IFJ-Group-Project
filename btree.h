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
#include "io.h"

/*  Struktura pro binarni strom
 * --------------------------------------------------------------------
 * - obsahuje klic, podle ktereho je ve stromu razena, odkaz na
 *   podstromy a dalsi data...
 */


struct node
{
  int keyValue; // co si dame jako klic?
 /* char Name[80];  // jmenno promenne nebo funkce
  int dataType=0; // datovy typ promenne nebo navratovy typ funkce
  int value;    // hodnota promenne, u funkce return value
  int prototyp=0;  // u funkce pokud 0 neni prototyp jestli 1 tak prototyp
  parametry par;   // parametry funkce*/
  // dalsi data v uzlu
  struct node * leftNode;
  struct node * rightNode;
};

typedef struct btreeS
{
  struct node * root;
  struct node * last;
} btree;

/*   Vytvoreni tabulky symbolu
 * ---------------------------------------------------------------------
 * - lze provest jednoduchym, lec ucinnym
 *   struct node * root = NULL;
 */


/*   Vlozeni noveho prvku do tabulky symbolu
 * ---------------------------------------------------------------------
 * - zapoji novy prvek do tabulky symbolu na spravne misto
 * - vstupnimy parametry funkce jsou tabulka symbolu a prvek, ktery ma
 *   byt vlozen
 */
int SymbolTableInsert(struct node ** leaf, struct node * insert);

/*   Zruseni tabulky symbolu
 * ---------------------------------------------------------------------
 * - zrusi tabulku symbolu (nerekurzivne z duvodu rychlosti)
 */
int SymbolTableDispose(struct node * leaf);

/*   Vyhledani prvku v tabulce symbolu
 * ---------------------------------------------------------------------
 * - prohleda tabulku, nejspis bude rozhodovat jestli pre/in/postorder
 * - vraci ukazatel na uzel pokud je hledany prvek nalezen nebo NULL
 */
struct node * SymbolTableSearch(struct node * leaf, int key);

#endif
