/*
 * =====================================================================
 *          Verze:  1.0
 *      Vytvoreno:  11/12/2014 04:22:15 PM
 *         Autori:  Tomáš Coufal, Roman Halík, Yurij Hladyuk, Jakub Jochlík
 *          Login:  xcoufa09, xhalik01, xhlady00, xjochl00
 *        Projekt:  IFJ
 * =====================================================================
 */

#include "btree.h"


btree SymbolTableInit()
{
  btree table;
  table.root = table.last = NULL;
  return table;
}

/*   Vlozeni noveho prvku do tabulky symbolu
 * ---------------------------------------------------------------------
 * - zapoji novy prvek do tabulky symbolu na spravne misto
 * - vstupnimy parametry funkce jsou tabulka symbolu a prvek, ktery ma
 *   byt vlozen
 */
int SymbolTableInsert(struct node ** leaf, struct node * insert)
{
  if (*leaf == NULL)
  {
    *leaf = insert;
    return EXIT_SUCCESS;
  }
  else if (insert->keyValue > (*leaf)->keyValue)
  {
    return SymbolTableInsert(&(*leaf)->rightNode, insert);
  }
  else if (insert->keyValue < (*leaf)->keyValue)
  {
    return SymbolTableInsert(&(*leaf)->leftNode, insert);
  }
  else
    return EXIT_INTERNAL_ERROR;
}

/*   Zruseni tabulky symbolu
 * ---------------------------------------------------------------------
 * - zrusi tabulku symbolu (nerekurzivne z duvodu rychlosti)
 */
int SymbolTableDispose(struct node * leaf)
{
  if (leaf != NULL)
  {
    SymbolTableDispose(leaf->leftNode);
    SymbolTableDispose(leaf->rightNode);
    free(leaf);
  }
  return EXIT_SUCCESS;
}

/*   Vyhledani prvku v tabulce symbolu
 * ---------------------------------------------------------------------
 * - prohleda tabulku, nejspis bude rozhodovat jestli pre/in/postorder
 * - vraci ukazatel na uzel pokud je hledany prvek nalezen nebo NULL
 */
struct node * SymbolTableSearch(struct node * leaf, int key)
{
  if (leaf != NULL)
  {
    if (key == leaf->keyValue) { return leaf; }
    else if (key < leaf->keyValue) {return SymbolTableSearch(leaf->leftNode, key); }
    else {return SymbolTableSearch(leaf->rightNode, key); }
  }
  return NULL;
}
